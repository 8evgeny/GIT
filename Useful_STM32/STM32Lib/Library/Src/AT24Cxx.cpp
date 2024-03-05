/*
 * AT24Cxx.cpp
 *
 *  Created on: 28 февр. 2022 г.
 *      Author: User
 */

#include "AT24Cxx.h"

AT24Cxx::AT24Cxx(I2C* I2C_Ptr, uint8_t DevAddress, uint8_t AT24C_Type, uint8_t MemAddSize)
{
  _DevAddress = DevAddress;
  _MemAddSize = MemAddSize;
  _I2C_Ptr    = I2C_Ptr;

  switch (AT24C_Type)
  {
    case AT24C02:
      _Pages = 32;
      _BytePerPages = 8;
      break;
    case AT24C04:
      _Pages = 32;
      _BytePerPages = 16;
      break;
    case AT24C08:
      _Pages = 64;
      _BytePerPages = 16;
      break;
    case AT24C16:
      _Pages = 128;
      _BytePerPages = 16;
      break;
    case AT24C32:
      _Pages = 128;
      _BytePerPages = 32;
      break;
    case AT24C64:
      _Pages = 256;
      _BytePerPages = 32;
      break;
    case AT24C128:
      _Pages = 256;
      _BytePerPages = 64;
      break;
    case AT24C256:
      _Pages = 512;
      _BytePerPages = 64;
      break;
    case AT24C512:
      _Pages = 512;
      _BytePerPages = 128;
      break;
    default:
      break;
  }
    _ChipSize = _Pages * _BytePerPages;
}

int16_t AT24Cxx::Read(uint16_t MemAddress, uint8_t* DataBuff, uint16_t Size)
{
  _MemAddress = MemAddress;
  _Size       = Size;

  return _I2C_Ptr->MasterMemRead(_DevAddress, _MemAddress, _MemAddSize, DataBuff, _Size, 15);
}

int16_t AT24Cxx::Write(uint16_t MemAddress, uint8_t *DataBuff, uint16_t Size)
{
  _MemAddress = MemAddress;
  _Size = Size;

  // Проверяем адрес и размер EEPROM, войдёт ли наш блок?
  if(!_CheckSpace(_MemAddress, _Size)) return 0;                      // Да ну на. Не влезет.

  uint8_t *page = DataBuff;
  uint16_t left = _Size;
  uint16_t toWrite;

  // Начиниаем запись
  while (left != 0)
  {
    // Вычисляем количество байт записываемых в текущую страницу
    // Если адрес не выровнен относительно страницы, пишем невыравненный кусок, затем полные страницы и остаток, если будет.
    if((_MemAddress % _BytePerPages) != 0)
    {
      toWrite = (((_MemAddress / _BytePerPages) + 1) * _BytePerPages) - _MemAddress;
      if(toWrite > _Size) toWrite = _Size;
    }
    else
    {
      if(left <= _BytePerPages) toWrite = left; else toWrite = _BytePerPages;
    }

    // Производим запись одной страницы или её части
    // Запись сорвалась, возвращаем количество записанных байт.
    if(_I2C_Ptr->MasterMemWrite(_DevAddress, _MemAddress, _MemAddSize, page, toWrite, 15) != 0) return _Size - left;

    delay(11);                                                        // После записи страницы, задержка не менее 10мс, что бы буфер сбросился.

    // Перерасчёт всех счётчиков
    left -= toWrite;
    _MemAddress += toWrite;
    page = DataBuff + toWrite;
  }

  return _Size;
}

int16_t AT24Cxx::Read(uint16_t MemAddress)
{
  uint8_t Buff[1];
  int16_t Temp;
  Temp = _I2C_Ptr->MasterMemRead(_DevAddress, MemAddress, _MemAddSize, Buff, 1, 15);
  if(Temp != 0) return Temp; else return Buff[0];
}

int16_t AT24Cxx::Write(uint16_t MemAddress, uint8_t Data)
{
  uint8_t Buff = Data;
  int16_t Temp;
  Temp = _I2C_Ptr->MasterMemWrite(_DevAddress, MemAddress, _MemAddSize, &Buff, 1, 15);
  delay(11);
  return Temp;
}

/*
 * Служебные функции
 */

bool AT24Cxx::_CheckSpace(uint16_t Address, uint16_t Size)
{
  // Переменная не сдурела? Не равна 0
  if(_ChipSize != 0)
  {
    // Проверяем стартовый адрес и размер записываемых данных. А влезет?
    if((Address >= _ChipSize) || ((Address + Size) >= _ChipSize)) return false;
    else return true;
  }

  return true;
}

int16_t AT24Cxx::Write(uint16_t MemAddress, const void* DataBuff, uint16_t Size)
{
  return Write(MemAddress, (uint8_t*) DataBuff, Size);
}

int16_t AT24Cxx::Read(uint16_t MemAddress, const void* DataBuff, uint16_t Size)
{
  return Read(MemAddress, (uint8_t*) DataBuff, Size);
}

