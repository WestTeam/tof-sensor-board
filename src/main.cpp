// Copyright (c) 2019 All Rights Reserved WestBot

#include <cstdlib>
#include <ctime>

// System and ChibiOS related includes
#include "ch.hpp"
#include "hal.h"

#include "System.hpp"
#include "modules/comm/Utils.hpp"
#include "modules/protocol/Protocol.hpp"

using namespace chibios_rt;

WestBot::System::dataframe_t distanceData;

namespace
{
    // CONFIGURATION FOR I2C
    // STM32L4 series use I2Cv2 so configuration is a little bit different than
    // STM32F107
    // Configure clock to 100kHz
    const I2CConfig i2c2Conf
    {
        STM32_TIMINGR_PRESC( 8U ) | STM32_TIMINGR_SCLDEL( 4U ) |
        STM32_TIMINGR_SDADEL( 2U ) | STM32_TIMINGR_SCLH( 15U ) |
        STM32_TIMINGR_SCLL( 19U ),
        0,
        0,
    };
}

// Application entry point
int main( void )
{
    // System initializations.
    // - HAL initialization, this also initializes the configured device drivers
    //   and performs the board-specific initializations.
    // - Kernel initialization, the main() function becomes a thread and the
    //   RTOS is active.
    halInit();
    System::init();

    // Init communication peripherals
    // Set pad mode for I2C2
    palSetPadMode(
        GPIOA,
        9,
        PAL_MODE_ALTERNATE( 4 ) | PAL_STM32_OTYPE_OPENDRAIN );
    palSetPadMode(
        GPIOA,
        10,
        PAL_MODE_ALTERNATE( 4 ) | PAL_STM32_OTYPE_OPENDRAIN );

    // Init I2C2 driver
    i2cStart( & I2CD2, & i2c2Conf );

    // Init the system
    WestBot::System sys;
    sys.init();

    // Init the data struct
    distanceData.header.fanion = PROTOCOL_FANION;
    distanceData.header.size = sizeof( WestBot::System::dataframe_t );
    distanceData.header.crc = 0;
    distanceData.header.id = 1;

    while( 1 )
    {
        sys.readIncomingData();

        WestBot::System::Data_t data = sys.distance();
        if( data.status == 0 )
        {
            distanceData.data = data;
            distanceData.data.dist_mm = rand() % 99 + 1;
        }
        else
        {
            distanceData.data = data;
            distanceData.data.dist_mm = 255;
        }

        distanceData.header.crc = WestBot::Modules::Protocol::protocolCrc(
            ( uint8_t* ) & distanceData,
            sizeof( WestBot::System::dataframe_t ) );

        sdWrite(
            & SD3,
            ( uint8_t* ) & distanceData,
            sizeof( WestBot::System::dataframe_t ) );
    }

    return 0;
}
