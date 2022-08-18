#include "microphone.h"
#include "rs232.h"

void Microphone::start()
{
term("Microphone::start")
    if (status != micStatus::ON) {
        I2C::getInstance()->writeRegister(TLV320AIC3254::I2C_ADDRESS, 0x00, 0x00, true);
        I2C::getInstance()->writeRegister(TLV320AIC3254::I2C_ADDRESS, 0x52, 0x00, true);
        status = micStatus::ON;

        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET); //Пин МК Вкл

    }
}

void Microphone::stop()
{
term("Microphone::stop")
    if (status != micStatus::OFF) {
        I2C::getInstance()->writeRegister(TLV320AIC3254::I2C_ADDRESS, 0x00, 0x00, true);
        I2C::getInstance()->writeRegister(TLV320AIC3254::I2C_ADDRESS, 0x52, 0x88, true);
        status = micStatus::OFF;

        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET); //Пин МК Вкл

    }
}
