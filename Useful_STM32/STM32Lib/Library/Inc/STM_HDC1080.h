/*
 * STM_HDC1080.h
 *
 *  Created on: 28 мар. 2022 г.
 *      Author: User
 */

#ifndef SRC_STM_HDC1080_H_
#define SRC_STM_HDC1080_H_

#include "STM32.h"
#include "stdlib.h"
#include "I2C.h"
#include "math.h"

#define RESET                         0x8000
#define HEATER                        0x2000
#define MODE_MEASUREMENT              0x1000
#define BATERRY_STATUS                0x0800
#define TEMPERATURE_RESOLUTION_14BIT  0x0000
#define TEMPERATURE_RESOLUTION_11BIT  0x0400
#define HUMIDITY_RESOLUTION_14BIT     0x0000
#define HUMIDITY_RESOLUTION_11BIT     0x0100
#define HUMIDITY_RESOLUTION_8BIT      0x0200


class S_HDC1080
{
  public:
    S_HDC1080();
    uint16_t  Init(I2C* I2C_Ptr);                             // Инициализация сенсора с адресом по умолчанию 0x76
    uint16_t  Init(I2C* I2C_Ptr, uint8_t address);            // Инициализация сенсора с нестандартным адресом
    uint16_t  getSerialID(void);
    uint64_t  getManufacturerID(void);
    float     readHumidity(void);
    float     readTemperature(void);

  private:
    uint8_t   _i2c_address = 0x40;                            // HDC1080 адрес на шине I2C
    I2C*      _I2C_Ptr;
    uint64_t  _SerialID;
    uint16_t  _ManufacturerID;
    uint16_t  _DeviceID;


    uint16_t  _readRegister(uint8_t address);
    bool      _writeRegister(uint8_t address, uint16_t data);

};

#endif /* SRC_STM_HDC1080_H_ */
