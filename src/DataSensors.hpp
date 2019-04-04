// Copyright (c) 2019 All Rights Reserved WestBot

// ========================================================================== //
//  System: Data sensors polling thread
//  Author: Jeremy S.
// ========================================================================== //

#ifndef WESTBOT_DATASENSORS_HPP_
#define WESTBOT_DATASENSORS_HPP_

#include "ch.hpp"
#include "hal.h"

#include "modules/sensors/VL6180X.hpp"

namespace WestBot {

// ========================================================================== //
// Data Acquisition thread class. It starts a thread that polls sensors data.
// ========================================================================== //
class DataSensors : public chibios_rt::BaseStaticThread< 128 >
{
public:
    struct Data_t
    {
        uint8_t dist_mm;
        uint8_t status;
    };

    DataSensors();

    void setPollingDelayMs( int delayMs );

    static Data_t getDataStructure();

protected:
    void main() override;

private:
    Modules::Sensors::VL6180X _vl6180x;
    int _delayMs;
};

}

#endif // WESTBOT_DATASENSORS_HPP_
