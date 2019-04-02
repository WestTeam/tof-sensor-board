// Copyright (c) 2019 All Rights Reserved WestBot

#include "hal/Gpio.hpp"

#include "Alive.hpp"

using namespace WestBot;

Alive::Alive( int delayMs )
    : BaseStaticThread< 32 >()
    , _delayMs( delayMs )
{
}

void Alive::setDelayMs( int newDelayMs )
{
    _delayMs = newDelayMs;
}

void Alive::main()
{
    setName( "alive" );

    WestBot::Hal::Gpio ledGreen( { GPIOA, GPIOA_LED_GREEN } );

    while( 1 )
    {
        ledGreen.toggle();
        sleep( TIME_MS2I( _delayMs ) );
    }
}
