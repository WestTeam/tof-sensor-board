// Copyright (c) 2019 All Rights Reserved WestBot

// ========================================================================== //
//  High level class to get data from TCS sensor
//  Author: Jeremy S.
// ========================================================================== //

#ifndef WESTBOT_MODULES_SENSORS_TCS3472_HPP_
#define WESTBOT_MODULES_SENSORS_TCS3472_HPP_

#include "ch.hpp"
#include "hal.h"

// INTEGRATION_TIME = 2.4 ms × (256 − ATIME)
// => ATIME = 256 − Integration Time / 2.4 ms
#define TCS3472_INTEGRATION_TIME_2_4_MS (0xFF)
#define TCS3472_INTEGRATION_TIME_24_MS  (0xF6)
#define TCS3472_INTEGRATION_TIME_101_MS (0xD5)
#define TCS3472_INTEGRATION_TIME_154_MS (0xC0)
#define TCS3472_INTEGRATION_TIME_700_MS (0x00)
#define TCS3472_INTEGRATION_TIME_MS(t)  ((uint8_t)(256U - (10 * t) / 24))

#define TCS34721_GAIN_1X                (0x00)
#define TCS34721_GAIN_4X                (0x01)
#define TCS34721_GAIN_16X               (0x02)
#define TCS34721_GAIN_60X               (0x03)

namespace WestBot {
namespace Modules {
namespace Sensors {

class TCS3472
{
public:
    TCS3472( I2CDriver* i2c );
    TCS3472( I2CDriver* i2c, uint8_t addr );

    bool init();

    bool readColor( uint16_t* rgbc );

private:
    // Check if sensor is present on I2C
    bool ping();
    void configure( uint8_t gain, uint8_t integrationTime );

    void writeRegister( uint8_t reg, uint8_t val );
    uint8_t readRegister( uint8_t reg );

private:
    I2CDriver* _i2c;
    uint8_t _deviceAddr;
    systime_t _timeout;
};

}
}
}

#endif // WESTBOT_MODULES_SENSORS_TCS3472_HPP_
