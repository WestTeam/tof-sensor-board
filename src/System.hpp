// Copyright (c) 2019 All Rights Reserved WestBot

// ========================================================================== //
//  System: Manage the board structure
//  Author: Jeremy S.
// ========================================================================== //

#ifndef WESTBOT_SYSTEM_HPP_
#define WESTBOT_SYSTEM_HPP_

#include "modules/sensors/VL6180X.hpp"

namespace WestBot {

class System
{
public:
    System();

    void init();

private:
    void printBootMsg();

    void trap();

private:
    Modules::Sensors::VL6180X _vl6180x;
};

}

#endif // WESTBOT_SYSTEM_HPP_
