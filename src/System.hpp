// Copyright (c) 2019 All Rights Reserved WestBot

// ========================================================================== //
//  System: Manage the board structure
//  Author: Jeremy S.
// ========================================================================== //

#ifndef WESTBOT_SYSTEM_HPP_
#define WESTBOT_SYSTEM_HPP_

#include "modules/protocol/Protocol.hpp"
#include "modules/sensors/VL6180X.hpp"

namespace WestBot {

class System
{
public:
    typedef struct
    {
        uint8_t dist_mm;
        uint8_t status;
    } __attribute__( ( packed ) ) Data_t;

    typedef struct
    {
        WestBot::Modules::Protocol::ProtocolHeader header;
        Data_t data;
    } __attribute__( ( packed ) ) dataframe_t;

    enum State
    {
        Unknown = 0,
        MagicDetect
    };

    System();

    void init();

    void process();

private:
    void readIncomingData();

private:
    Modules::Sensors::VL6180X _vl6180x;
    State _state;
};

}

#endif // WESTBOT_SYSTEM_HPP_
