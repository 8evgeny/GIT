#include "microphone.h"
#include "rs232.h"

extern uint8_t pinMkState;
void Microphone::start()
{
term("Microphone::start")
    if (status != micStatus::ON) {
        I2C::getInstance()->writeRegister(TLV320AIC3254::I2C_ADDRESS, 0x00, 0x00, true);
        I2C::getInstance()->writeRegister(TLV320AIC3254::I2C_ADDRESS, 0x52, 0x00, true);
        status = micStatus::ON;
        pinMkState = pinMkSet;
    }
}

void Microphone::stop()
{
term("Microphone::stop")
    if (status != micStatus::OFF) {
        I2C::getInstance()->writeRegister(TLV320AIC3254::I2C_ADDRESS, 0x00, 0x00, true);
        I2C::getInstance()->writeRegister(TLV320AIC3254::I2C_ADDRESS, 0x52, 0x88, true);
        status = micStatus::OFF;
        pinMkState = pinMkReset;
    }
}
