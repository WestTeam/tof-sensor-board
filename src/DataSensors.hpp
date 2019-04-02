// Copyright (c) 2019 All Rights Reserved WestBot

// ========================================================================== //
//  System: Data sensors polling thread
//  Author: Jeremy S.
// ========================================================================== //

#ifndef WESTBOT_DATASENSORS_HPP_
#define WESTBOT_DATASENSORS_HPP_

#include "ch.hpp"
#include "hal.h"

// TODO: XXX Add include for sensors drivers

namespace WestBot {

// ========================================================================== //
// Data Acquisition thread class. It starts a thread that polls sensors data.
// ========================================================================== //
class DataSensors : public chibios_rt::BaseStaticThread< 512 >
{
public:
    struct Data_t
    {
        double dist_mm;
    };

    DataSensors();

    void addVL6180X();

    void setPollingDelayMs( int delayMs );

    Data_t getDataStructure();

protected:
    void main() override;

private:
    Data_t _data;
    int _delayMs;
};

}

#endif // WESTBOT_DATASENSORS_HPP_
