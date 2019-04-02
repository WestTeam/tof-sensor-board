// Copyright (c) 2019 All Rights Reserved WestBot

#include "I2c.hpp"

using namespace WestBot::Hal;

I2c::I2c( conf_t config )
    : _driver( config.driver )
    , _config( config.config )
    , _timeout( config.timeout )
{
}

bool I2c::init()
{
    i2cAcquireBus( _driver );
    i2cStart( _driver, & _config );
    i2cReleaseBus( _driver );
    return true;
}

bool I2c::probe( uint32_t address )
{
    uint8_t data[ 1 ] = { 0 };
	return ( write( data, 1, address ) );
}

bool I2c::write( const uint8_t* buffer, uint32_t nbytes, uint32_t address )
{
    msg_t status = MSG_OK;

    i2cAcquireBus( _driver );
    i2cStart( _driver, & _config );
    uint8_t rxbuf[ 1 ] = { 0 };
    status = i2cMasterTransmitTimeout(
        _driver,
        address,
        buffer,
        nbytes,
        rxbuf,
        0,
        _timeout );

    if( status == MSG_TIMEOUT )
    {
        i2cStop( _driver );
        i2cStart( _driver, & _config );
    }

    i2cReleaseBus( _driver );

    return ( status == MSG_OK );
}

bool I2c::read( uint8_t* buffer, uint32_t nbytes, uint32_t address )
{
    msg_t status = MSG_OK;

    i2cAcquireBus( _driver );
    i2cStart( _driver, & _config );
    status = i2cMasterReceiveTimeout(
        _driver,
        address,
        buffer,
        nbytes,
        _timeout );
    i2cReleaseBus( _driver );

    return ( status == MSG_OK );
}

bool I2c::transfer(
    uint8_t* out_buffer,
    uint32_t ntxbytes,
    uint8_t* in_buffer,
    uint32_t nrxbytes,
    uint32_t address )
{
    msg_t status = MSG_OK;

    i2cAcquireBus( _driver );
    i2cStart( _driver, & _config );
    status = i2cMasterTransmitTimeout(
        _driver,
        address,
        out_buffer,
        ntxbytes,
        in_buffer,
        nrxbytes,
        _timeout );
    i2cReleaseBus( _driver );

    return ( status == MSG_OK );
}
