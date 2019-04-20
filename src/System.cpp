// Copyright (c) 2019 All Rights Reserved WestBot

#include "ch.hpp"
#include "hal.h"

#include "System.hpp"

#include "Alive.hpp"
#include "modules/comm/Utils.hpp"

#define NO_VL6180X

namespace
{
    // UART configuration
    const SerialConfig uartCfg =
    {
        9600, // bit rate
        0,
        0,
        0
    };
}

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
    palSetPadMode( GPIOB, 10, PAL_MODE_ALTERNATE( 7 ) );
    palSetPadMode( GPIOB, 11, PAL_MODE_ALTERNATE( 7 ) );

    // Activates the serial driver 2 for debug
    sdStart( & SD2, & uartCfg );

    // Activates the serial driver 3 for debug
    sdStart( & SD3, & uartCfg );

#ifndef NO_VL6180X
    // Configure VL6180X before starting pullind data from it
    if( ! _vl6180x.init() )
    {
        DEBUG_PRINT( 1, KRED "Failed to init VL6180X sensors\r\n" );
        trap();
    }

    // TODO: XXX DO NOT FORGET TO HOLD PIN TO HIGH BEFORE CHANGING I2C ADDR
    // IF NEEDED !!!

#endif

    // On start ensuite les threads
    alive.start( NORMALPRIO + 20 );
}

WestBot::System::Data_t WestBot::System::distance()
{
    WestBot::System::Data_t data;
#ifndef NO_VL6180X
    data.status = _vl6180x.measureDistance( & data.dist_mm );
#endif
    return data;
}

void WestBot::System::readIncomingData()
{
    WestBot::System::dataframe_t distanceData;

    static uint8_t* ptr = ( uint8_t * ) & distanceData;
    static int remaining = 0;
    int nbBytesRead = 0;

    switch( _state )
    {
    case WestBot::System::State::Unknown:
    {
        ptr = ( uint8_t * ) & distanceData;

        nbBytesRead = sdAsynchronousRead( & SD3, ptr, 1 );

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
            & SD3,
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
                    return;
                }

                if( distanceData.data.status == 0 )
                {
                    // TODO: forward the trame
                }
                else
                {
                    // TODO: Log
                    return;
                }
            }
        }
        break;
    }
    }
}

//
// Private methods
//
void WestBot::System::trap()
{
    palSetPadMode( GPIOA, 5, PAL_MODE_OUTPUT_PUSHPULL );

    while( 1 )
    {
        palTogglePad( GPIOA, 5 );
        chThdSleepMilliseconds( 50 );
    }
}
