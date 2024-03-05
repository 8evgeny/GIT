/*
 * AT24Cxx.h
 *
 *  Created on: 28 февр. 2022 г.
 *      Author: User
 */

#ifndef AT24CXX_H_
#define AT24CXX_H_

#include "STM32.h"
#include "I2C.h"

enum AT24C
{
  AT24C02,
  AT24C04,
  AT24C08,
  AT24C16,
  AT24C32,
  AT24C64,
  AT24C128,
  AT24C256,
  AT24C512,
  AT24C1024
};

class AT24Cxx// : public I2C
{
  public:
    AT24Cxx(I2C* I2C_Ptr, uint8_t DevAddress, uint8_t AT24C_Type, uint8_t MemAddSize);

    // Чтение/запись массива. Формат: Адрес в EEPROM, Буфер, Количество байт
    int16_t Read(uint16_t MemAddress, uint8_t* DataBuff, uint16_t Size);        // Чтение массива
    int16_t Read(uint16_t MemAddress, const void* DataBuff, uint16_t Size);        // Чтение массива по указателю
    int16_t Write(uint16_t MemAddress, uint8_t* DataBuff, uint16_t Size);       // Запись массива
    int16_t Write(uint16_t MemAddress, const void* DataBuff, uint16_t Size);    // Запись массива по указателю

    // Чтение/запись одного байта. Формат: Адрес в EEPROM
    int16_t Read(uint16_t MemAddress);                                          // Чтение одного байта
    int16_t Write(uint16_t MemAddress, uint8_t Data);                           // Запись одного байта

  private:
    uint16_t  _Pages;
    uint8_t   _BytePerPages;
    uint8_t   _DevAddress;
    uint8_t   _AT24C_Type;
    uint8_t   _MemAddSize;
    uint16_t  _MemAddress;
    uint16_t  _Size;
    uint16_t  _ChipSize;
    I2C*      _I2C_Ptr;

    bool _CheckSpace(uint16_t Address, uint16_t Size);
};

#endif /* AT24CXX_H_ */
