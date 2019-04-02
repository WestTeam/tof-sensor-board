// Copyright (c) 2019 All Rights Reserved WestBot

// ========================================================================== //
//  System: Manage the board structure
//  Author: Jeremy S.
// ========================================================================== //

#ifndef WESTBOT_SYSTEM_HPP_
#define WESTBOT_SYSTEM_HPP_

#include "ch.hpp"
#include "hal.h"

#include "modules/comm/Utils.hpp"
#include "modules/sensors/VL6180X.hpp"

#include "Alive.hpp"
#include "DataSensors.hpp"

namespace WestBot {

class System
{
public:
    System( I2CDriver* i2c2 );

    void init();

    void printCliMsg();

    DataSensors::Data_t sensorsData();

private:
    void printBootMsg();

private:
    WestBot::Modules::Sensors::VL6180X _vl6180x;

    WestBot::Alive* _alive;
    WestBot::DataSensors* _sensors;
};

}

#endif // WESTBOT_SYSTEM_HPP_
