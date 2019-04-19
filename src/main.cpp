// Copyright (c) 2019 All Rights Reserved WestBot

// System and ChibiOS related includes
#include "ch.hpp"
#include "hal.h"

#include "DataSensors.hpp"
#include "System.hpp"
#include "modules/comm/Utils.hpp"
#include "modules/crc/Crc.hpp"

using namespace chibios_rt;

// TODO: Move this...
typedef struct
{
    uint8_t trameId = 0x01;
    uint32_t data;
    uint8_t magicNumber = 0xA5;
} dataframe_t;

dataframe_t distanceData;

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

    void sendTrame()
    {
        // Data structures
        WestBot::DataSensors::Data_t tmpData;
        tmpData = WestBot::DataSensors::getDataStructure();

        distanceData.data = tmpData.dist_mm;
        DEBUG_PRINT(
            1,
            KNRM "%d%d%d\r\n",
            distanceData.trameId,
            distanceData.data,
            distanceData.magicNumber );
    }
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

    while( 1 )
    {
        sendTrame();
        chThdSleepMilliseconds( 10 );
    }

    return 0;
}
