// Copyright (c) 2019 All Rights Reserved WestBot

// ========================================================================== //
//  System: Alive thread
//  Author: Jeremy S.
// ========================================================================== //

#ifndef WESTBOT_ALIVE_HPP_
#define WESTBOT_ALIVE_HPP_

#include "ch.hpp"
#include "hal.h"

namespace WestBot {

// ========================================================================== //
// Blinker thread class. It blink the onboard LED and nothing else.
// ========================================================================== //
class Alive : public chibios_rt::BaseStaticThread< 32 >
{
public:
    Alive( int delayMs );

    void setDelayMs( int newDelayMs );

protected:
    void main() override;

private:
    int _delayMs;
};

}

#endif // WESTBOT_ALIVE_HPP_
