// Copyright (c) 2019 All Rights Reserved WestBot

#include "modules/comm/Utils.hpp"

#include "DataSensors.hpp"
#include "System.hpp"

using namespace WestBot;
using namespace chibios_rt;

//#define NO_VL6180X

static DataSensors::Data_t _data;

DataSensors::DataSensors()
    : BaseStaticThread< 128 >()
    , _vl6180x( & I2CD2 )
    , _delayMs( 10 )
{
}

void DataSensors::setPollingDelayMs( int delayMs )
{
    _delayMs = delayMs;
}

DataSensors::Data_t DataSensors::getDataStructure()
{
    return _data;
}

void DataSensors::main()
{
    setName( "sensors_data_polling" );

#ifndef NO_VL6180X
    // Configure VL6180X before starting pullind data from it
    if( ! _vl6180x.init() )
    {
        DEBUG_PRINT( 1, KRED "Failed to init VL6180X sensors\r\n" );
        // DO NO START THREADS and GO TO TRAP MODE
        WestBot::System::trap();
        return;
    }

    // TODO: XXX DO NOT FORGET TO HOLD PIN TO HIGH BEFORE CHANGING I2C ADDR
    // IF NEEDED !!!

    uint8_t distance_mm;
    uint8_t status;

    while( 1 )
    {
        status = _vl6180x.measureDistance( & distance_mm );

        _data.dist_mm = distance_mm;

        // Proximity detected
        if( distance_mm < 20 )
        {
            palSetPad( GPIOA, 6 ); // Turn the red led ON
        }
        else
        {
            palClearPad( GPIOA, 6 ); // Turn the red led ON
        }

        _data.status = status;

        sleep( TIME_MS2I( _delayMs ) );
    }
#endif
}
