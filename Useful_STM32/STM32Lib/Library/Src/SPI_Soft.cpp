/*
 * SPI_Soft.cpp
 *
 *  Created on: 16 июн. 2022 г.
 *      Author: User
 */

#include "SPI_Soft.h"

SPI::SPI(GPIO_TypeDef* GPIO_MOSI, uint16_t GPIO_PIN_MOSI,
                   GPIO_TypeDef* GPIO_MISO, uint16_t GPIO_PIN_MISO,
                   GPIO_TypeDef* GPIO_CLK, uint16_t GPIO_PIN_CLK)
{
  _GPIO_MOSI      = GPIO_MOSI;
  _GPIO_PIN_MOSI  = GPIO_PIN_MOSI;
  _GPIO_MISO      = GPIO_MISO;
  _GPIO_PIN_MISO  = GPIO_PIN_MISO;
  _GPIO_CLK       = GPIO_CLK;
  _GPIO_PIN_CLK   = GPIO_PIN_CLK;
}

void SPI::Init(uint8_t DataSize, uint8_t Mode)
{
  _SetSpeed(High);
  if(_GPIO_MOSI != NULL) _SetPin(_GPIO_MOSI, _GPIO_PIN_MOSI, Output, No_Pull);
  if(_GPIO_MISO != NULL) _SetPin(_GPIO_MISO, _GPIO_PIN_MISO, Output, No_Pull);
  _SetPin(_GPIO_CLK, _GPIO_PIN_CLK, Output, No_Pull);
  _dataSize = DataSize;
}


void SPI::Transmit(uint16_t Data)
{
  if(_dataSize == Data_16bit)
  {
    write((uint8_t) (Data >> 8));
    write((uint8_t) (Data & 0xFF));
  }
  else write((uint8_t) (Data & 0xFF));

  _DigitalWriteBit(_GPIO_MOSI, _GPIO_PIN_MOSI, Low);
}

void SPI::write(uint8_t Data)
{
  _FlagBSY = false;
  uint8_t Temp = Data;
  for(uint8_t I=0; I<8; I++)
  {
    if(Temp & 0x80)
      _DigitalWriteBit(_GPIO_MOSI, _GPIO_PIN_MOSI, High);
    else
      _DigitalWriteBit(_GPIO_MOSI, _GPIO_PIN_MOSI, Low);
    Temp = Temp << 1;
    _DigitalWriteBit(_GPIO_CLK, _GPIO_PIN_CLK, High);
    _DigitalWriteBit(_GPIO_CLK, _GPIO_PIN_CLK, Low);
  }
  _FlagBSY = true;
}

size_t SPI::Receive(uint8_t* Buff, uint8_t Length)
{
  return 0;
}


