// Copyright (c) 2019 All Rights Reserved WestBot

#include "modules/comm/Utils.hpp"

#include "DataSensors.hpp"
#include "System.hpp"

using namespace WestBot;
using namespace chibios_rt;

//#define NO_VL6180X

static DataSensors::Data_t _data;

DataSensors::DataSensors( Modules::Sensors::VL6180X* vl6180x )
    : BaseStaticThread< 128 >()
    , _vl6180x( vl6180x )
    , _delayMs( 1 )
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

    uint8_t distance_mm;
    uint8_t status;

    while( 1 )
    {
        status = _vl6180x->measureDistance( & distance_mm );

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
}
