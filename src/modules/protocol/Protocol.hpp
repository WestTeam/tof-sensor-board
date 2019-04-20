// Copyright (c) 2019 All Rights Reserved WestBot

// ========================================================================== //
//  Protocol helpers functions
//  Author: Jeremy S.
// ========================================================================== //

#ifndef WESTBOT_MODULES_PROTOCOL_PROTOCOL_HPP_
#define WESTBOT_MODULES_PROTOCOL_PROTOCOL_HPP_

#include "ch.hpp"
#include "hal.h"

#define PROTOCOL_FANION 0xA5

namespace WestBot {
namespace Modules {
namespace Protocol {

typedef struct
{
    uint8_t     fanion;
    uint16_t    size;
    uint16_t    crc; // sum data + id
    uint16_t    id;
} __attribute__( ( packed ) ) ProtocolHeader;

uint16_t protocolCrc( uint8_t* msg, uint16_t size );

}
}
}

#endif // WESTBOT_MODULES_PROTOCOL_PROTOCOL_HPP_
