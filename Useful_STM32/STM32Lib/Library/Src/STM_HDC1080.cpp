/*
 * STM_HDC1080.cpp
 *
 *  Created on: 28 мар. 2022 г.
 *      Author: User
 */

#include "STM_HDC1080.h"

S_HDC1080::S_HDC1080()
{

}

uint16_t S_HDC1080::Init(I2C* I2C_Ptr){ return Init(I2C_Ptr, 0x40); }

uint16_t S_HDC1080::Init(I2C* I2C_Ptr, uint8_t address)
{
  uint16_t Temp = 0;
  _i2c_address  = address;
  _I2C_Ptr      = I2C_Ptr;
  /* === Проверяем HDC1080 это или что то другое === */
  _DeviceID = _readRegister(0xFF);                                                                 // Читаем регистр ID
  if(_DeviceID != 0x1050) return false;                                                        // Если нужный ID (bme/bmp280), то всё в порядке

  Temp = _readRegister(0x02);
  Temp |= (HUMIDITY_RESOLUTION_14BIT | TEMPERATURE_RESOLUTION_14BIT);// | MODE_MEASUREMENT);
  _writeRegister(0x02, Temp);

  return _DeviceID;
}

float     S_HDC1080::readHumidity(void)
{
  uint8_t Buff[2] = {0x01,};
//  _writeRegister(0x01, 0x01);
  _I2C_Ptr->MasterWrite(_i2c_address, Buff, 1, 11);
  delay(6);
  uint16_t rawT = _readRegister(0x01);
  return (rawT / pow(2, 16)) * 100.0;
}

float     S_HDC1080::readTemperature(void)
{
  _writeRegister(0x00, 0x00);
  delay(10);
  uint16_t rawT = _readRegister(0x00);
  return (rawT / pow(2, 16)) * 165.0 - 40.0;
}

uint16_t  S_HDC1080::getSerialID(void)
{
  return _readRegister(0xFE);
}

uint64_t  S_HDC1080::getManufacturerID(void)
{
  uint64_t Temp = 0;
  Temp = _readRegister(0xFB);
  Temp = Temp << 16;
  Temp |= _readRegister(0xFC);
  Temp = Temp << 16;
  Temp |= _readRegister(0xFD);
  return Temp;
}

/* = Читать слово из регистра HDC1080 = */
uint16_t S_HDC1080::_readRegister(uint8_t address)
{
  uint8_t Temp;
  uint8_t Buff[2];

  Temp = _I2C_Ptr->MasterWrite(_i2c_address, &address, 1, 12);
  if(Temp != I2C_OK) return Temp;
  Temp = _I2C_Ptr->MasterRead(_i2c_address, Buff, 2, 12);
  if(Temp != I2C_OK) return Temp;
  return ((Buff[0] << 8) | Buff[1]);
}

/* = Записать байт в регистр BME280 = */
bool S_HDC1080::_writeRegister(uint8_t address, uint16_t data)
{
  uint8_t Buff[2];
  Buff[0] = data;
//  _I2C_Ptr->MasterMemWrite(_i2c_address, address, Size_Add_8bit, Buff, 2, 12);
  _I2C_Ptr->MasterWrite(_i2c_address, Buff, 1, 12);
  return true;
}

