// Copyright (c) 2019 All Rights Reserved WestBot

// ========================================================================== //
//  Utility macro for serial communication
//  Author: Jeremy S.
// ========================================================================== //

#ifndef WESTBOT_COMM_UTILS_HPP_
#define WESTBOT_COMM_UTILS_HPP_

#include <stdio.h>
#include <string.h>

#include "ch.hpp"
#include "hal.h"

#include "chprintf.h"

// Terminal color defines
#define KNRM  "\x1B[0m"
#define KRED  "\x1B[31m"
#define KGRN  "\x1B[32m"
#define KYEL  "\x1B[33m"
#define KBLU  "\x1B[34m"
#define KMAG  "\x1B[35m"
#define KCYN  "\x1B[36m"
#define KWHT  "\x1B[37m"

#define DEBUG
#define DLEVEL 3
#ifdef DEBUG
#define DEBUG_PRINT( level, ... ) \
    do { \
        if( DLEVEL >= (level) ) \
            chprintf( (BaseSequentialStream * )&SD3, __VA_ARGS__); \
   } while (0)
#else
#define DEBUG_PRINT( level, ... )
#endif

#define DEBUG
#define DLEVEL 3
#ifdef DEBUG
#define CLI_PRINT( level, ... ) \
    do { \
        if( DLEVEL >= (level) ) \
            chprintf( (BaseSequentialStream * )&SD2, __VA_ARGS__); \
   } while (0)
#else
#define CLI_PRINT( level, ... )
#endif

#endif // WESTBOT_COMM_UTILS_HPP_
