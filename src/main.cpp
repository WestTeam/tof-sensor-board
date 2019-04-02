// Copyright (c) 2019 All Rights Reserved WestBot

// System and ChibiOS related includes
#include "ch.hpp"
#include "hal.h"

#include "chprintf.h"
#include "shell.h"

// Hal includes for communication init
#include "hal/I2c.hpp"

#include "modules/comm/Utils.hpp"
#include "System.hpp"

#include "modules/sensors/VL6180X.hpp"

using namespace chibios_rt;

namespace
{
    // CONFIGURATION FOR I2C
    // STM32L4 series use I2Cv2 so configuration is a little bit different than
    // STM32F107
    // Configure clock to 100kHz
    const WestBot::Hal::I2c::conf_t i2c2Conf
    {
        & I2CD2,
        {
            STM32_TIMINGR_PRESC( 8U ) | STM32_TIMINGR_SCLDEL( 4U ) |
            STM32_TIMINGR_SDADEL( 2U ) | STM32_TIMINGR_SCLH( 15U ) |
            STM32_TIMINGR_SCLL( 19U ),
            0,
            0,
        },
        TIME_MS2I( 100 )
    };

    // Configuration for Shell
    const ShellConfig shellCfg =
    {
        ( BaseSequentialStream* ) & SD2,
        NULL
    };
}

// ========================================================================== //
// Command line related.
// ========================================================================== //

#define SHELL_WA_SIZE THD_WORKING_AREA_SIZE( 2048 )

static thread_t* shelltp = nullptr;

//
// Shell exit event.
//
static void ShellHandler( eventid_t id )
{
    ( void ) id;
    if( chThdTerminatedX( shelltp ) )
    {
        chThdWait( shelltp );
        shelltp = NULL;
    }
}

// Application entry point
int main( void )
{
    static const evhandler_t evhndl[] =
    {
        ShellHandler,
    };
    event_listener_t el0;

    // System initializations.
    // - HAL initialization, this also initializes the configured device drivers
    //   and performs the board-specific initializations.
    // - Kernel initialization, the main() function becomes a thread and the
    //   RTOS is active.
    halInit();
    System::init();

    // Init communication peripherals(I2c, Spi, Uart, Can...)
    // Set pad mode for I2C2
    palSetPadMode(
        GPIOB,
        13,
        PAL_MODE_ALTERNATE( 4 ) | PAL_STM32_OTYPE_OPENDRAIN );
    palSetPadMode(
        GPIOB,
        14,
        PAL_MODE_ALTERNATE( 4 ) | PAL_STM32_OTYPE_OPENDRAIN );

    // Init I2C2 driver
    WestBot::Hal::I2c i2c2( i2c2Conf );

    // Init the system after...
    WestBot::System sys( i2c2 );
    sys.init();

    // Test VL6180X
    WestBot::Modules::Sensors::VL6180X vl6180x( & I2CD2 );
    vl6180x.init();

    // Shell manager initialization.
    shellInit();

    chEvtRegister( & shell_terminated, & el0, 0 );
    while( 1 )
    {
        if( ! shelltp )
        {
            // Welcome shell user first...
            sys.printCliMsg();

            shelltp = chThdCreateFromHeap(
                NULL,
                SHELL_WA_SIZE,
                "shell",
                NORMALPRIO + 1,
                shellThread,
                ( void * ) & shellCfg );
        }

        DEBUG_PRINT( 1, KNRM "========================\r\n" );
        DEBUG_PRINT( 1, KNRM "========================\r\n" );

        chEvtDispatch(
            evhndl,
            chEvtWaitOneTimeout( ALL_EVENTS, TIME_MS2I( 500 ) ) );
    }

    return 0;
}
