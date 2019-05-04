// Copyright (c) 2019 All Rights Reserved WestBot

#include "ch.hpp"
#include "hal.h"

#include "System.hpp"

#include "Alive.hpp"
#include "modules/comm/Utils.hpp"

//#define NO_VL6180X

namespace
{
    // UART configuration
    const SerialConfig uartCfg =
    {
        115200, // bit rate
        0,
        0,
        0
    };
}

volatile uint8_t dataId = 0;
volatile uint8_t dataValue = 0;
volatile uint8_t dataValid = 0;
volatile uint8_t dataStatus = 0;

class Vl6180Thread : public chibios_rt::BaseStaticThread< 256 >
{
public:
    Vl6180Thread( const WestBot::Modules::Sensors::VL6180X& sensor )
        : BaseStaticThread< 256 >()
        , _sensor( sensor )
    {
    }

protected:
    void main() override
    {
        setName( "Vl6180x" );

        while( 1 )
        {
            WestBot::System::Data_t data = distance();

            // Proximity detected
            if( data.dist_mm < 20 )
            {
                palSetPad( GPIOA, 6 ); // Turn the red led ON
            }
            else
            {
                palClearPad( GPIOA, 6 ); // Turn the red led ON
            }

            dataValid = 0;
            dataValue = data.dist_mm;
            dataStatus = data.status;
            dataId++;
            dataValid = 1;

            //DEBUG_PRINT( 1, KRED "Dist:%d\r\n", dataValue );
            sleep( TIME_MS2I( 100 ) );
        }
    }

private:
    WestBot::System::Data_t distance()
    {
        WestBot::System::Data_t data;
    #ifndef NO_VL6180X
        data.status = _sensor.measureDistance( & data.dist_mm );
    #else
      data.status = 0;
    #endif

        return data;
    }

private:
    WestBot::Modules::Sensors::VL6180X _sensor;
};

static WestBot::Alive alive;

// Init the system and all peripherals
WestBot::System::System()
    : _vl6180x( & I2CD2 )
    , _state( WestBot::System::State::Unknown )
{
}

void WestBot::System::init()
{
    // Set pad mode for UART 3 (UART 2 is already set in board.h)
    palSetPadMode( GPIOB, 3, PAL_MODE_ALTERNATE( 7 ) );
    palSetPadMode( GPIOB, 4, PAL_MODE_ALTERNATE( 7 ) );

    // Activates the serial driver 2
    sdStart( & SD2, & uartCfg );

#ifndef NO_VL6180X
    // Configure VL6180X before starting pullind data from it
    if( ! _vl6180x.init() )
    {
        //DEBUG_PRINT( 1, KRED "Failed to init VL6180X sensors\r\n" );
        NVIC_SystemReset();
    }

    // TODO: XXX DO NOT FORGET TO HOLD PIN TO HIGH BEFORE CHANGING I2C ADDR
    // IF NEEDED !!!

#endif

    // On start ensuite les threads
    alive.start( NORMALPRIO + 20 );
    static Vl6180Thread distanceSensor( _vl6180x );
    distanceSensor.start( NORMALPRIO + 20 );
}

void WestBot::System::process()
{
    readIncomingData();

    static uint8_t localDataId = 0;
    static uint8_t localDataValid = 0;
    if( dataValid == 1 )
    {
        uint8_t newDataId = dataId;
        uint8_t newDataValue = dataValue;
        uint8_t newDataStatus = dataStatus;
        if( localDataValid == 0 || newDataId != localDataId )
        {
            if( dataValid == 1 && dataId == newDataId )
            {
                localDataId = newDataId;
                localDataValid = 1;

                WestBot::System::dataframe_t distanceData;

                // Init the data struct
                distanceData.header.fanion = PROTOCOL_FANION;
                distanceData.header.size = sizeof( WestBot::System::dataframe_t );
                distanceData.header.crc = 0;
                distanceData.header.id = 0;

                distanceData.data.dist_mm = newDataValue;
                distanceData.data.status = newDataStatus;

                distanceData.header.crc = WestBot::Modules::Protocol::protocolCrc(
                    ( uint8_t* ) & distanceData,
                    sizeof( WestBot::System::dataframe_t ) );
                sdWrite(
                    & SD2,
                    ( uint8_t* ) & distanceData,
                    sizeof( WestBot::System::dataframe_t ) );
            }
        }
    }
}

//
// Private methods
//
void WestBot::System::readIncomingData()
{
    static WestBot::System::dataframe_t distanceData;

    static uint8_t* ptr = ( uint8_t * ) & distanceData;
    static int remaining = 0;
    int nbBytesRead = 0;

    switch( _state )
    {
    case WestBot::System::State::Unknown:
    {
        ptr = ( uint8_t * ) & distanceData;

        nbBytesRead = sdAsynchronousRead( & SD2, ptr, 1 );

        if( nbBytesRead != 1 )
        {
            return;
        }
        else
        {
            ptr++;
            if( distanceData.header.fanion == 0xA5 )
            {
                _state = WestBot::System::State::MagicDetect;
                remaining = 8;
            }
        }
    }
        break;

    case WestBot::System::State::MagicDetect:
    {
        nbBytesRead = sdAsynchronousRead(
            & SD2,
            ptr,
            remaining );

        if( nbBytesRead != 0 )
        {
            ptr += nbBytesRead;
            remaining = remaining - nbBytesRead;
            if( remaining == 0 )
            {
                const uint16_t crc = WestBot::Modules::Protocol::protocolCrc(
                        ( uint8_t* ) & distanceData,
                        sizeof( WestBot::System::dataframe_t ) );

                if( crc != distanceData.header.crc )
                {
                    goto err;
                }

                if( distanceData.data.status == 0 )
                {
                    distanceData.header.id++;
                    const uint16_t crc = WestBot::Modules::Protocol::protocolCrc(
                            ( uint8_t* ) & distanceData,
                            sizeof( WestBot::System::dataframe_t ) );
                    distanceData.header.crc = crc;

                    sdWrite(
                        & SD2,
                        ( uint8_t* ) & distanceData,
                        sizeof( WestBot::System::dataframe_t ) );
                }
                else
                {
                    // TODO: Log
                    goto err;
                }

err:
                _state = WestBot::System::State::Unknown;
            }
        }
        break;
    }
    }
}
