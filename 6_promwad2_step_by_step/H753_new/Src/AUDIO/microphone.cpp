#include "microphone.h"
#ifndef SC2BOARD
#include "can_stm32f7xx.h"
#endif

void Microphone::start()
{
    if (status != micStatus::ON) {
        I2C::getInstance()->writeRegister(TLV320AIC3254::I2C_ADDRESS, 0x00, 0x00);
        I2C::getInstance()->writeRegister(TLV320AIC3254::I2C_ADDRESS, 0x52, 0x00);
        status = micStatus::ON;

#ifndef SC2BOARD
        CAN::getInstance()->configLed(0, true, 0, 0, 0, CAN::mikeLed);
#endif
    }
}

void Microphone::stop()
{
    if (status != micStatus::OFF) {
        I2C::getInstance()->writeRegister(TLV320AIC3254::I2C_ADDRESS, 0x00, 0x00);
        I2C::getInstance()->writeRegister(TLV320AIC3254::I2C_ADDRESS, 0x52, 0x88);
        status = micStatus::OFF;

#ifndef SC2BOARD
        CAN::getInstance()->configLed(0, false, 0, 0, 0, CAN::mikeLed);
#endif
    }
}
