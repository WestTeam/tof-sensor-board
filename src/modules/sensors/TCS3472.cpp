// Copyright (c) 2019 All Rights Reserved WestBot

#include "TCS3472.hpp"

#define TCS3472X_I2C_ADDR               (0x29) // TCS34725, TCS34727
// #define TCS3472X_I2C_ADDR               (0x39) // TCS34721, TCS34723
#define TCS3472X_ID                     (0x44) // TCS34721, TCS34725
// #define TCS3472X_ID                     (0x4D) // TCS34723, TCS34727

#define TCS3472_CMD                     (1 << 7)
#define TCS3472_CMD_AUTO_INC            (1 << 5) | TCS3472_CMD
#define TCS3472_CMD_SPECIAL_FN          (3 << 5) | TCS3472_CMD

#define TCS3472_REG_ENABLE              (0x00)
#define TCS3472_REG_ATIME               (0x01)
#define TCS3472_REG_WTIME               (0x03)
#define TCS3472_REG_AILTL               (0x04)
#define TCS3472_REG_AILTH               (0x05)
#define TCS3472_REG_AIHTL               (0x06)
#define TCS3472_REG_AIHTH               (0x07)
#define TCS3472_REG_PERS                (0x0C)
#define TCS3472_REG_CONFIG              (0x0D)
#define TCS3472_REG_CONTROL             (0x0F)
#define TCS3472_REG_ID                  (0x12)
#define TCS3472_REG_STATUS              (0x13)
#define TCS3472_REG_CDATAL              (0x14)
#define TCS3472_REG_CDATAH              (0x15)
#define TCS3472_REG_RDATAL              (0x16)
#define TCS3472_REG_RDATAH              (0x17)
#define TCS3472_REG_GDATAL              (0x18)
#define TCS3472_REG_GDATAH              (0x19)
#define TCS3472_REG_BDATAL              (0x1A)
#define TCS3472_REG_BDATAH              (0x1B)

#define TCS3472_ENABLE_PON              (0x01)
#define TCS3472_ENABLE_AEN              (0x02)

using namespace WestBot::Modules::Sensors;

TCS3472::TCS3472( I2CDriver* i2c )
    : TCS3472( i2c, TCS3472X_I2C_ADDR )
{
}

TCS3472::TCS3472( I2CDriver* i2c, uint8_t addr )
    : _i2c( i2c )
    , _deviceAddr( addr )
    , _timeout( TIME_MS2I( 100 ) )
{
}

bool TCS3472::init()
{
    palSetPadMode( GPIOA, 4, PAL_STM32_MODE_OUTPUT );
    palSetPad( GPIOA, 4 ); // enable light

    if( ! ping() )
    {
        //DEBUG_PRINT( 1, KNRM "[VL6180X] Chip not detected\r\n" );
        return false;
    }

    //DEBUG_PRINT( 1, KNRM "[VL6180X] Chip detected\r\n" );

    configure( TCS34721_GAIN_16X, TCS3472_INTEGRATION_TIME_101_MS );
    return true;
}


bool TCS3472::readColor( uint16_t* rgbc )
{
    uint8_t txbuf[ 1 ] = {TCS3472_REG_CDATAL | TCS3472_CMD_AUTO_INC};
    uint8_t rxbuf[ 8 ];

    if( MSG_OK != i2cMasterTransmitTimeout(
        _i2c,
        TCS3472X_I2C_ADDR,
        txbuf,
        1,
        rxbuf,
        8,
        _timeout ) )
    {
        return false;
    }

    rgbc[ 3 ] = rxbuf[ 0 ] | ( ( uint16_t ) rxbuf[ 1 ] << 8 );
    rgbc[ 0 ] = rxbuf[ 2 ] | ( ( uint16_t ) rxbuf[ 3 ] << 8 );
    rgbc[ 1 ] = rxbuf[ 4 ] | ( ( uint16_t ) rxbuf[ 5 ] << 8 );
    rgbc[ 2 ] = rxbuf[ 6 ] | ( ( uint16_t ) rxbuf[ 7 ] << 8 );

    return true;
}

//
// Private methods
//
bool TCS3472::ping()
{
    const uint8_t id = readRegister( TCS3472_REG_ID );
    if( TCS3472X_ID == id )
    {
        return true;
    }
    else
    {
        return false;
    }
}

void TCS3472::configure( uint8_t gain, uint8_t integrationTime )
{
    writeRegister(
        TCS3472_REG_ENABLE,
        TCS3472_ENABLE_PON | TCS3472_ENABLE_AEN );
    writeRegister( TCS3472_REG_ATIME, integrationTime );
    writeRegister( TCS3472_REG_CONTROL, gain );
    writeRegister( TCS3472_REG_WTIME, 0xFF ); // minimal wait time
    writeRegister( TCS3472_REG_CONFIG, 0x00 ); // WLONG = 0
}

void TCS3472::writeRegister( uint8_t reg, uint8_t val )
{
    uint8_t txbuf[ 2 ];
    txbuf[0] = reg | TCS3472_CMD_AUTO_INC;
    txbuf[1] = val;
    i2cMasterTransmitTimeout(
        _i2c,
        TCS3472X_I2C_ADDR,
        txbuf,
        2,
        NULL,
        0,
        _timeout );
}

uint8_t TCS3472::readRegister( uint8_t reg )
{
    uint8_t txbuf[ 1 ] = { reg | TCS3472_CMD_AUTO_INC };
    uint8_t rxbuf[ 1 ];
    i2cMasterTransmitTimeout(
        _i2c,
        TCS3472X_I2C_ADDR,
        txbuf,
        1,
        rxbuf,
        1,
        _timeout );

    return rxbuf[ 0 ];
}
