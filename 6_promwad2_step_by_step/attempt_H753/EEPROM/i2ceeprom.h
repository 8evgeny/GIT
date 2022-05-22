/*!
 \file i2ceeprom.h

*/

#ifndef I2CEEPROM_H
#define I2CEEPROM_H

#include <stdint.h>
#include "stm32h7xx_hal.h"


/*!
 \brief Implementation: ensures that only one instance of the singleton class ever exists
        and provide global access to that instance.

 \class Eeprom_i2c i2c.h "i2c.h"
*/

    uint32_t timeOutMax = 300;
    FlagStatus i2cReadReady = RESET ;
    FlagStatus i2cWriteReady = RESET ;

HAL_StatusTypeDef i2c1_readData(uint16_t DevAddress, uint16_t MemAddress, uint8_t *pData, uint16_t Size);

HAL_StatusTypeDef i2c1_writeData(uint16_t DevAddress, uint16_t MemAddress, uint8_t *pData, uint16_t Size);

enum {
    FAST_MODE_PLUS = 0x00200922,
    STANDART_MODE = 0x20404768,
    FAST_MODE = 0x6000030D
};


//void I2C1Init(void);

void EEPROM_IO_Init(void);

void simpleEEPROM_test();
void simpleEEPROM_test2();

HAL_StatusTypeDef EEPROM_IO_WriteData(uint16_t DevAddress, uint16_t MemAddress, uint8_t *pBuffer, uint32_t BufferSize);

HAL_StatusTypeDef EEPROM_IO_ReadData(uint16_t DevAddress, uint16_t MemAddress, uint8_t *pBuffer, uint32_t BufferSize);

HAL_StatusTypeDef EEPROM_IO_IsDeviceReady(uint16_t DevAddress, uint32_t Trials);


#endif // I2CEEPROM_H
