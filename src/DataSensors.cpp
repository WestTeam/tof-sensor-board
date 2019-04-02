// Copyright (c) 2019 All Rights Reserved WestBot

#include "modules/comm/Utils.hpp"

#include "DataSensors.hpp"

using namespace WestBot;
using namespace chibios_rt;

DataSensors::DataSensors()
    : BaseStaticThread< 512 >()
    , _delayMs( 10 )
{
}

void DataSensors::addVL6180X()
{
    // TODO: XXX
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

    while( 1 )
    {
        _data.dist_mm = 0; // TODO: XXX
        sleep( TIME_MS2I( _delayMs ) );
    }
}
