// Copyright (c) 2019 All Rights Reserved WestBot

// ========================================================================== //
//  High level class to get data from VL6180X sensor
//  Author: Jeremy S.
// ========================================================================== //

#ifndef WESTBOT_MODULES_SENSORS_VL6180X_HPP_
#define WESTBOT_MODULES_SENSORS_VL6180X_HPP_

#include <memory>

#include "ch.hpp"
#include "hal.h"

namespace WestBot {
namespace Modules {
namespace Sensors {

class VL6180X
{
public:
    using Ptr = std::shared_ptr< VL6180X >;

    VL6180X( I2CDriver* i2c );
    VL6180X( I2CDriver* i2c, uint8_t addr );

    void init();

    // Returns 0 if the distance was measured correctly,
    // the error code otherwise.
    uint8_t measureDistance( uint8_t* out_mm );

    // Change default I2C address
    void changeI2cAddress( uint8_t address );

private:
    // Check if sensor is present on I2C
    bool ping();

    // Sends initial configuration to device.
    void configure();

    void writeRegister( uint16_t reg, uint8_t val );
    uint8_t readRegister( uint16_t reg );

private:
    I2CDriver* _i2c;
    uint8_t _deviceAddr;
};

}
}
}

#endif // WESTBOT_MODULES_SENSORS_VL6180X_HPP_
