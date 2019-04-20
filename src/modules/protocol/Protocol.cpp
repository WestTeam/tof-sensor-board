// Copyright (c) 2019 All Rights Reserved WestBot

#include "Protocol.hpp"

using namespace WestBot::Modules::Protocol;

uint16_t WestBot::Modules::Protocol::protocolCrc( uint8_t* msg, uint16_t size )
{
	uint16_t crc = 0;

	while( --size > 4 )
    {
		crc += msg[ size ];
    }

	return crc;
}
