// Copyright (c) 2019 All Rights Reserved WestBot

#include "hal/Gpio.hpp"

#include "System.hpp"

#define NO_VL6180X 1

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
}

// Init the system and all peripherals
WestBot::System::System( I2CDriver* i2c2 )
    : _vl6180x( i2c2 )
{
    _alive = new WestBot::Alive( 125 );
    _sensors = new WestBot::DataSensors();
}

void WestBot::System::init()
{
    // Set pad mode for UART 3 (UART 2 is already set in board.h)
    palSetPadMode( GPIOB, 10, PAL_MODE_ALTERNATE( 7 ) );
    palSetPadMode( GPIOB, 11, PAL_MODE_ALTERNATE( 7 ) );

    // Activates the serial driver 3 for debug
    sdStart( & SD3, & uartCfg );

    // Activates the serial driver 2 for shell
    sdStart( & SD2, & uartCfg );

    // Welcome the user
    printBootMsg();

#ifndef NO_VL6180X
    // Configure VL6180X before starting pullind data from it
    if( ! _vl6180x.init() )
    {
        DEBUG_PRINT( 1, KRED "Failed to init VL6180X sensors\r\n" );
        // DO NO START THREADS and GO TO TRAP MODE
        trap();
        return;
    }
#endif

    // On start ensuite les threads
    _alive->start( NORMALPRIO + 20 );

    // TODO: XXX DO NOT FORGET TO HOLD PIN TO HIGH BEFORE CHANGING I2C ADDR
    // IF NEEDED !!!

    _sensors->addVL6180X(
        std::make_shared< WestBot::Modules::Sensors::VL6180X >( _vl6180x ) );
    _sensors->start( NORMALPRIO + 10 );
}

void WestBot::System::printCliMsg()
{
    //Display boot sys info:
    CLI_PRINT( 1, KGRN "Kernel:       %s\r\n", CH_KERNEL_VERSION );
    #ifdef CH_COMPILER_NAME
        CLI_PRINT( 1, KGRN "Compiler:     %s\r\n", CH_COMPILER_NAME );
    #endif
    CLI_PRINT( 1, KGRN "Architecture: %s\r\n", PORT_ARCHITECTURE_NAME );
    #ifdef CH_CORE_VARIANT_NAME
        CLI_PRINT( 1, KGRN "Core Variant: %s\r\n", CH_CORE_VARIANT_NAME );
    #endif
    #ifdef CH_PORT_INFO
        CLI_PRINT( 1, KGRN "Port Info:    %s\r\n", CH_PORT_INFO );
    #endif
    #ifdef PLATFORM_NAME
        CLI_PRINT( 1, KGRN "Platform:     %s\r\n", PLATFORM_NAME );
    #endif
    #ifdef BOARD_NAME
        CLI_PRINT( 1, KGRN "Board:        %s\r\n", BOARD_NAME );
    #endif
    #ifdef __DATE__
    #ifdef __TIME__
        CLI_PRINT( 1, KGRN "Build time:   %s%s%s\r\n", __DATE__, " - ", __TIME__ );
    #endif
    #endif

    // Set color cursor to normal
    CLI_PRINT( 1, KNRM "" );
}

WestBot::DataSensors::Data_t WestBot::System::sensorsData()
{
    return _sensors->getDataStructure();
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

void WestBot::System::trap()
{
    WestBot::Hal::Gpio ledGreen( { GPIOA, GPIOA_LED_GREEN } );

    while( 1 )
    {
        ledGreen.toggle();
        chThdSleepMilliseconds( 50 );
    }
}
