// Copyright (c) 2019 All Rights Reserved WestBot

// ========================================================================== //
//  User commands
//  Author: Jeremy S.
// ========================================================================== //

#ifndef WESTBOT_COMM_COMMANDS_HPP_
#define WESTBOT_COMM_COMMANDS_HPP_

#include <stdlib.h>

#include "ch.h"
#include "hal.h"

#include "chprintf.h"
#include "shell.h"

#include "../../DataSensors.hpp"

// System
static void cmd_reset( BaseSequentialStream* chp, int argc, char* argv[] )
{
    ( void ) argv;
    if( argc > 0 )
    {
        chprintf(chp, "Usage: reset\r\n");
        return;
    }

    NVIC_SystemReset();
}

// Sensors commands
static void cmd_get_dist( BaseSequentialStream* chp, int argc, char* argv[] )
{
    ( void ) argv;
    if( argc > 0 )
    {
        chprintf(chp, "Usage: get_dist\r\n");
        return;
    }

    WestBot::DataSensors::Data_t data =
        WestBot::DataSensors::getDataStructure();

    chprintf( chp, "[VL6180X] Dist [mm]: %d\r\n", data.dist_mm );

}

static const ShellCommand user_commands[] =
{
    { "reset", cmd_reset },
    { "get_dist", cmd_get_dist },
    { NULL, NULL }
};

#endif // WESTBOT_COMM_COMMANDS_HPP_
