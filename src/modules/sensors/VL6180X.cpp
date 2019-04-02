// Copyright (c) 2019 All Rights Reserved WestBot

#include "../comm/Utils.hpp"

#include "VL6180X.hpp"
#include "VL6180XRegisters.hpp"

#define DEFAULT_VL6180X_ADDR 0x29

using namespace WestBot::Modules::Sensors;

VL6180X::VL6180X( I2CDriver* i2c )
    : VL6180X( i2c, DEFAULT_VL6180X_ADDR )
{
}

VL6180X::VL6180X( I2CDriver* i2c, uint8_t addr )
    : _i2c( i2c )
    , _deviceAddr( addr )
{
}

void VL6180X::init()
{
    if( ! ping() )
    {
        DEBUG_PRINT( 1, KNRM "[VL6180X] Chip not detected\r\n" );
        return;
    }

    DEBUG_PRINT( 1, KNRM "[VL6180X] Chip detected\r\n" );

    configure();
}

uint8_t VL6180X::measureDistance( uint8_t* out_mm )
{
    uint8_t status;
    uint8_t mm;

    // Wait for device ready.
    do
    {
        status = readRegister( RESULT__RANGE_STATUS );
    } while( ( status & ( 1 << 0 ) ) == 0 );

    // Start measurement.
    writeRegister( SYSRANGE__START, 0x01 );

    // Wait for measurement ready.
    do
    {
        status = readRegister( RESULT__INTERRUPT_STATUS_GPIO );
    } while( ( status & ( 1 << 2 ) ) == 0 );

    // Read result.
    mm = readRegister( RESULT__RANGE_VAL );
    *out_mm = mm;

    // Clear interrupt flags.
    writeRegister( SYSTEM__INTERRUPT_CLEAR, 0x07 );

    // Wait for _device ready.
    do
    {
        status = readRegister( RESULT__RANGE_STATUS );
    } while( ( status & ( 1 << 0 ) ) == 0 );

    // Return error code.
    return status >> 4;
}

void VL6180X::changeI2cAddress( uint8_t address )
{
    writeRegister( I2C_SLAVE__DEVICE_ADDRESS, address );
    _deviceAddr = address;
}

//
// Private methods
//
bool VL6180X::ping()
{
    uint8_t id = readRegister( IDENTIFICATION__MODEL_ID );
    if( 0xB4 == id )
    {
        return true;
    }
    else
    {
        return false;
    }
}

void VL6180X::configure()
{
    while( readRegister( 0x16 ) != 0x01 )
    {
        // WAIT
    }

    // clear SYSTEM__FRESH_OUT_OF_RESET
    writeRegister( 0x016, 0x00 );

    static const struct
    {
        uint16_t reg_addr;
        uint8_t value;
    } init_tab[] = {
        /* Mandatory: Private registers. */
        {0x0207, 0x01},
        {0x0208, 0x01},
        {0x0096, 0x00},
        {0x0097, 0xfd},
        {0x00e3, 0x00},
        {0x00e4, 0x04},
        {0x00e5, 0x02},
        {0x00e6, 0x01},
        {0x00e7, 0x03},
        {0x00f5, 0x02},
        {0x00d9, 0x05},
        {0x00db, 0xce},
        {0x00dc, 0x03},
        {0x00dd, 0xf8},
        {0x009f, 0x00},
        {0x00a3, 0x3c},
        {0x00b7, 0x00},
        {0x00bb, 0x3c},
        {0x00b2, 0x09},
        {0x00ca, 0x09},
        {0x0198, 0x01},
        {0x01b0, 0x17},
        {0x01ad, 0x00},
        {0x00ff, 0x05},
        {0x0100, 0x05},
        {0x0199, 0x05},
        {0x01a6, 0x1b},
        {0x01ac, 0x3e},
        {0x01a7, 0x1f},
        {0x0030, 0x00},
        /* Recommended : Public registers - See data sheet for more detail */
        /* Enables polling for New Sample ready when measurement completes */
        {SYSTEM__MODE_GPIO1, 0x10},
        /* Set the averaging sample period (compromise between lower noise and
     * increased execution time) */
        {READOUT__AVERAGING_SAMPLE_PERIOD, 0x30},
        /* Sets the light and dark gain (upper nibble). Dark gain should not be
     * changed.*/
        {SYSALS__ANALOGUE_GAIN, 0x46},
        /* sets the # of range measurements after which auto calibration of system
     * is performed */
        {SYSRANGE__VHV_REPEAT_RATE, 0xFF},
        /* Set ALS integration time to 100ms */
        {SYSALS__INTEGRATION_PERIOD, 0x63},
        /* perform a single temperature calibration of the ranging sensor */
        {SYSRANGE__VHV_RECALIBRATE, 0x01},
        /* Configure interrupt on new sample ready. Required for polling to work. */
        {SYSTEM__INTERRUPT_CONFIG_GPIO, 0x24},
    };

    const size_t init_tab_len = sizeof(init_tab) / sizeof(init_tab[0]);
    size_t i;
    for (i = 0; i < init_tab_len; i++) {
        writeRegister( init_tab[i].reg_addr, init_tab[i].value);
    }
}

void VL6180X::writeRegister( uint16_t reg, uint8_t val )
{
    uint8_t buf[] = { ( reg >> 8 ), reg & 0xff, val };
    i2cMasterTransmit( _i2c, _deviceAddr, buf, 3, NULL, 0 );
}

uint8_t VL6180X::readRegister( uint16_t reg )
{
    uint8_t ret;
    uint8_t buf[] = { ( reg >> 8 ), reg & 0xff };
    i2cMasterTransmit( _i2c, _deviceAddr, buf, 2, & ret, 1 );
    return ret;
}
