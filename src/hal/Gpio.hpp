// Copyright (c) 2019 All Rights Reserved WestBot

// ========================================================================== //
//  GPIO wrapper for ChibiOS/HAL
//  Author: Jeremy S.
// ========================================================================== //

#ifndef WESTBOT_HAL_GPIO_HPP_
#define WESTBOT_HAL_GPIO_HPP_

#include "hal.h"

namespace WestBot {
namespace Hal {

// TODO: Add support for GPIO configuration
class Gpio
{
public:

    /**
     * \brief GPIO configuration
     */
    struct conf_t
    {
        ioportid_t      port;
        ioportmask_t    pin;
    };

    /**
     * \brief Constructor
     */
    Gpio( conf_t config );

    /**
     * @brief   Hardware initialization
     *
     * @return  true        Success
     * @return  false       Error
     */
    bool init();

    /**
     * @brief   Write 1 to the gpio
     *
     * @return  true        Success
     * @return  false       Failed
     */
    bool set_high();

    /**
     * @brief   Write 0 to the gpio
     *
     * @return  true        Success
     * @return  false       Failed
     */
    bool set_low();

    /**
     * @brief   Toggle the gpio value
     * @details Writes 0 if currently high, writes 1 if currently low
     *
     * @return  true        Success
     * @return  false       Failed
     */
    bool toggle();

    /**
     * @brief   Write to the gpio pin
     *
     * @param   level       Value to write
     *
     * @return  true        Success
     * @return  false       Failed
     */
    bool write( bool level );

    /**
     * @brief   Read the current gpio level
     *
     * @return  Level
     */
    bool read( void );

private:
    ioportid_t      _port;
    ioportmask_t    _pin;
    bool            _level;
};

}
}

#endif // WESTBOT_HAL_GPIO_HPP_
