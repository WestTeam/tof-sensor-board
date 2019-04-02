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
class DataSensors : public chibios_rt::BaseStaticThread< 512 >
{
public:
    struct Data_t
    {
        uint8_t dist_mm;
        uint8_t status;
    };

    DataSensors();

    void addVL6180X( const Modules::Sensors::VL6180X::Ptr& vl6180x );

    void setPollingDelayMs( int delayMs );

    Data_t getDataStructure();

protected:
    void main() override;

private:
    Modules::Sensors::VL6180X::Ptr _vl6180x;

    Data_t _data;
    int _delayMs;
};

}

#endif // WESTBOT_DATASENSORS_HPP_
