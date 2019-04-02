// Copyright (c) 2018 All Rights Reserved WestBot

#include "Gpio.hpp"

using namespace WestBot::Hal;

Gpio::Gpio( conf_t config )
    : _port( config.port )
    , _pin( config.pin )
    , _level( false )
{
}

bool Gpio::init()
{
    return set_low();
}

bool Gpio::set_high()
{
    palSetPad( _port, _pin );
    _level = true;
    return true;
}

bool Gpio::set_low()
{
    palClearPad( _port, _pin );
    _level = false;
    return true;
}

bool Gpio::toggle()
{
    _level = ! _level;
    write( _level );
    return true;
}

bool Gpio::write( bool level )
{
    if( level )
    {
        set_high();
    }
    else
    {
        set_low();
    }

    return true;
}

bool Gpio::read()
{
    if( palReadPad( _port, _pin ) == PAL_HIGH )
    {
        _level = true;
    }
    else
    {
        _level = false;
    }

    return _level;
}
