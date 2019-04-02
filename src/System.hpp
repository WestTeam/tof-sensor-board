// Copyright (c) 2019 All Rights Reserved WestBot

// ========================================================================== //
//  System: Manage the board structure
//  Author: Jeremy S.
// ========================================================================== //

#ifndef WESTBOT_SYSTEM_HPP_
#define WESTBOT_SYSTEM_HPP_

#include "hal/I2c.hpp"

#include "modules/comm/Utils.hpp"

#include "Alive.hpp"
#include "DataSensors.hpp"

namespace WestBot {

class System
{
public:
    System( Hal::I2c& i2c2 );

    void init();

    void printCliMsg();

    DataSensors::Data_t sensorsData();

private:
    void printBootMsg();

private:
    Hal::I2c& _i2c2;

    WestBot::Alive* _alive;
    WestBot::DataSensors* _sensors;
};

}

#endif // WESTBOT_SYSTEM_HPP_
