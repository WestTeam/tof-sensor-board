// Copyright (c) 2019 All Rights Reserved WestBot

#include <math.h>

#include "Alive.hpp"

using namespace WestBot;

namespace
{
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

Alive::Alive()
    : BaseStaticThread< 32 >()
{
}

void Alive::main()
{
    setName( "alive" );

    // Enter PWM mode
    palSetPadMode( GPIOA, 0, PAL_MODE_ALTERNATE( 1 ) );
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
