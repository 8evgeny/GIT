#ifndef TLV320AIC3254_H
#define TLV320AIC3254_H

#include <cstdint>

#include "stm32f7xx_hal.h"
#include "stm32f7xx_hal_sai.h"

#include "sai.h"
#include "i2c.h"
#include "cmsis_os.h"

class TLV320AIC3254
{

public:

    I2C *pI2C;
    SAI *pSAI;

    TLV320AIC3254();

    /*!
     \brief This method is required to initialize the audio codec.

     \fn codecInit
    */
    void codecInit();

    constexpr static uint32_t I2C_ADDRESS = 49; /*! Device address on the bus */

};

#endif // TLV320AIC3254_H
