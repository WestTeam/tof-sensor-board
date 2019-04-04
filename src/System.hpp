// Copyright (c) 2019 All Rights Reserved WestBot

// ========================================================================== //
//  System: Manage the board structure
//  Author: Jeremy S.
// ========================================================================== //

#ifndef WESTBOT_SYSTEM_HPP_
#define WESTBOT_SYSTEM_HPP_

#include "modules/comm/Utils.hpp"

namespace WestBot {

class System
{
public:
    System();

    void init();

    static void trap();

private:
    void printBootMsg();
};

}

#endif // WESTBOT_SYSTEM_HPP_
