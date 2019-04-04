// Copyright (c) 2019 All Rights Reserved WestBot

#include "ch.hpp"
#include "hal.h"

#include <math.h>

#include "System.hpp"

#include "DataSensors.hpp"


using namespace chibios_rt;

namespace
{
    // UART configuration
    const SerialConfig uartCfg =
    {
        115200, // bit rate
        0,
        0,
        0
    };

    const PWMConfig ledConf
    {
        1000000, // 1MHz PWM clock frequency
        2500,    // Initial PWM period 20ms ( 50hz (20ms) for standard servo/ESC, 400hz for fast servo/ESC (2.5ms = 2500) )
        NULL,  // No callback
        {
            { PWM_OUTPUT_ACTIVE_HIGH, NULL },
            { PWM_OUTPUT_DISABLED, NULL },
            { PWM_OUTPUT_DISABLED, NULL },
            { PWM_OUTPUT_DISABLED, NULL }
        },
        0,
        0,
    #if STM32_PWM_USE_ADVANCED
        0,
    #endif
    };
}

// DONT KNOW WHY but it saves a lot of space to declare the thread like this...
class Alive : public BaseStaticThread< 32 >
{
protected:
    void main() override
    {
        setName( "alive" );

        // Enter PWM mode
        palSetPadMode( GPIOA, 5, PAL_MODE_ALTERNATE( 1 ) );
        pwmStart( & PWMD2, & ledConf );
        pwmEnableChannel( & PWMD2, 0, 0 );

        while( 1 )
        {
            // Sin pulse to the LED
            float in, out;
            for( in = 0; in < 6.283; in = in + 0.00628 )
            {
                out = sin( in ) * 5000 + 5000;
                int bright = floor( out );
                pwmEnableChannel(
                    & PWMD2,
                    0,
                    PWM_PERCENTAGE_TO_WIDTH( & PWMD2, bright ) );
                in = in + .001 * out /255;
                sleep( TIME_MS2I( 4 ) );
            }
        }
  }

public:
    Alive()
        : BaseStaticThread< 32 >()
    {
    }
};

static Alive alive;

// Init the system and all peripherals
WestBot::System::System()
    : _vl6180x( & I2CD2 )
{
}

void WestBot::System::init()
{
    // Set pad mode for UART 3 (UART 2 is already set in board.h)
    palSetPadMode( GPIOB, 10, PAL_MODE_ALTERNATE( 7 ) );
    palSetPadMode( GPIOB, 11, PAL_MODE_ALTERNATE( 7 ) );

    // Activates the serial driver 2 for debug
    sdStart( & SD2, & uartCfg );

    // Activates the serial driver 3 for debug
    sdStart( & SD3, & uartCfg );

    // Welcome the user
    printBootMsg();

#ifndef NO_VL6180X
    // Configure VL6180X before starting pullind data from it
    if( ! _vl6180x.init() )
    {
        DEBUG_PRINT( 1, KRED "Failed to init VL6180X sensors\r\n" );
        trap();
    }

    // TODO: XXX DO NOT FORGET TO HOLD PIN TO HIGH BEFORE CHANGING I2C ADDR
    // IF NEEDED !!!

    static WestBot::DataSensors sensors( &_vl6180x );
    sensors.start( NORMALPRIO + 10 );

#endif

    // On start ensuite les threads
    alive.start( NORMALPRIO + 20 );
}

void WestBot::System::trap()
{
    palSetPadMode( GPIOA, 5, PAL_MODE_OUTPUT_PUSHPULL );

    while( 1 )
    {
        palTogglePad( GPIOA, 5 );
        chThdSleepMilliseconds( 50 );
    }
}

//
// Private methods
//
void WestBot::System::printBootMsg()
{
    //Display boot sys info:
    DEBUG_PRINT( 1, KGRN "Kernel:       %s\r\n", CH_KERNEL_VERSION );
    #ifdef CH_COMPILER_NAME
        DEBUG_PRINT( 1, KGRN "Compiler:     %s\r\n", CH_COMPILER_NAME );
    #endif
    DEBUG_PRINT( 1, KGRN "Architecture: %s\r\n", PORT_ARCHITECTURE_NAME );
    #ifdef CH_CORE_VARIANT_NAME
        DEBUG_PRINT( 1, KGRN "Core Variant: %s\r\n", CH_CORE_VARIANT_NAME );
    #endif
    #ifdef CH_PORT_INFO
        DEBUG_PRINT( 1, KGRN "Port Info:    %s\r\n", CH_PORT_INFO );
    #endif
    #ifdef PLATFORM_NAME
        DEBUG_PRINT( 1, KGRN "Platform:     %s\r\n", PLATFORM_NAME );
    #endif
    #ifdef BOARD_NAME
        DEBUG_PRINT( 1, KGRN "Board:        %s\r\n", BOARD_NAME );
    #endif
    #ifdef __DATE__
    #ifdef __TIME__
        DEBUG_PRINT( 1, KGRN "Build time:   %s%s%s\r\n", __DATE__, " - ", __TIME__ );
    #endif
    #endif

    // Set color cursor to normal
    DEBUG_PRINT( 1, KNRM "" );
}
