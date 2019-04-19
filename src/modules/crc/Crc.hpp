// Copyright (c) 2019 All Rights Reserved WestBot

// ========================================================================== //
//  Crc helpers functions
//  Author: Jeremy S.
// ========================================================================== //

#ifndef WESTBOT_MODULES_CRC_CRC_HPP_
#define WESTBOT_MODULES_CRC_CRC_HPP_

#include "ch.hpp"
#include "hal.h"

namespace WestBot {
namespace Modules {
namespace Crc {

// CRC-16-ANSI/IBM algorithm
uint16_t crc16( uint16_t init, const void* data, size_t length );

}
}
}

#endif // WESTBOT_MODULES_CRC_CRC_HPP_
