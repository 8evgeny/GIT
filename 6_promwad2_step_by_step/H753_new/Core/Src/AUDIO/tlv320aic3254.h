#ifndef TLV320AIC3254_H
#define TLV320AIC3254_H

#include <cstdint>

#include "stm32h7xx_hal.h"
#include "stm32h7xx_hal_sai.h"

#include "sai.h"
#include "i2c.h"
#include "cmsis_os.h"

struct Aic3254Configs {
    uint8_t regOffset;
    uint8_t regVal;
};

class TLV320AIC3254
{

public:

    I2C *pI2C;
    SAI *pSAI;

    TLV320AIC3254();
    static TLV320AIC3254 *getInstance();
    /*!
     \brief This method is required to initialize the audio codec.

     \fn codecInit
    */
    void codecInit();
    void softReset();
    void configPll();
    void configPower();
    void configDAC();
    void configADC();
    void configFilter();
    void mute();
    constexpr static uint32_t I2C_ADDRESS = 49; /*! Device address on the bus */
private:
    static TLV320AIC3254 *pInstance;
};

#endif // TLV320AIC3254_H
