// Copyright (c) 2019 All Rights Reserved WestBot

// ========================================================================== //
//  I2C peripheral driver using ChibiOS/HAL
//  Author: Jeremy S.
// ========================================================================== //

#ifndef WESTBOT_HAL_I2C_HPP_
#define WESTBOT_HAL_I2C_HPP_

#include "hal.h"

namespace WestBot {
namespace Hal {

// I2C peripheral driver using ChibiOS/HAL
class I2c
{
public:
    // Configuration structure
    struct conf_t
    {
        I2CDriver* driver;
        I2CConfig config;
        systime_t timeout;
    };

    // Default constructor
    I2c( conf_t config );

    // Hardware initialization
    bool init();

    /**
     * \brief   Test if a chip answers for a given I2C address
     *
     * \param   address     Slave adress
     *
     * \return  True        Slave found
     * \return  False       Slave not found
     */
    bool probe( uint32_t address );

    /**
     * \brief   Write multiple bytes to a I2C slave device
     *
     * \param   buffer      Data buffer
     * \param   nbytes      Number of bytes to write
     * \param   address     Slave adress
     *
     * \return  False       Data not written
     */
    bool write( const uint8_t* buffer, uint32_t nbytes, uint32_t address );

    /**
     * \brief   Read multiple bytes to a I2C slave device
     *
     * \param   buffer      Data buffer
     * \param   nbytes      Number of bytes to read
     * \param   address     Slave adress
     *
     * \return  True        Data successfully read
     * \return  False       Data not read
     */
    bool read( uint8_t* buffer, uint32_t nbytes, uint32_t address );

    /**
     * \brief   Write then Read data to/from an I2C device
     *
     * \param   out_buffer  Data buffer (output)
     * \param   ntxbytes    Number of bytes to write
     * \param   in_buffer   Data buffer (input)
     * \param   nrxbytes    Number of bytes to read
     *
     * \return  true        Success
     * \return  false       Failed
     */
    bool transfer(
        uint8_t* out_buffer,
        uint32_t ntxbytes,
        uint8_t* in_buffer,
        uint32_t nrxbytes,
        uint32_t address );

private:
    I2CDriver* _driver;
    I2CConfig _config;
    systime_t _timeout;
};

}
}

#endif // WESTBOT_HAL_I2C_HPP_
