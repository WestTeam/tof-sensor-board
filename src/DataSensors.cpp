// Copyright (c) 2019 All Rights Reserved WestBot

#include "modules/comm/Utils.hpp"

#include "DataSensors.hpp"

using namespace WestBot;
using namespace chibios_rt;

static DataSensors::Data_t _data;

DataSensors::DataSensors()
    : BaseStaticThread< 512 >()
    , _vl6180x( nullptr )
    , _delayMs( 10 )
{
}

void DataSensors::addVL6180X( const Modules::Sensors::VL6180X::Ptr& vl6180x )
{
    _vl6180x = vl6180x;
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
        _data.status = status;

        sleep( TIME_MS2I( _delayMs ) );
    }
}
