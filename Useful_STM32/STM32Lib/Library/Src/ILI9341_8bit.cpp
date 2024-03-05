/*
 * ILI9341_8bit.cpp
 *
 *  Created on: 17 нояб. 2021 г.
 *      Author: User
 */

#include  "ILI9341_8bit.h"      // Включаем файл заголовка для нашего модуля 


ILI9341::ILI9341():GFX_TFT(240,320)
{
  _GPIO_Pin = 0;
}

void ILI9341::Init(GPIO_TypeDef *GPIOx_Half, bool HalfWords,
    GPIO_TypeDef  *GPIOx_Reset, uint16_t TFT_Reset,
    GPIO_TypeDef  *GPIOx_CS, uint16_t TFT_CS,
    GPIO_TypeDef  *GPIOx_RS, uint16_t TFT_RS,
    GPIO_TypeDef  *GPIOx_WR, uint16_t TFT_WR,
    GPIO_TypeDef  *GPIOx_RD, uint16_t TFT_RD)
{
  // Инициализируем внутринние переменные
  _GPIOx_Half   = GPIOx_Half;
  _TFT_Pin_Half = HalfWords;                      // Позже может не понадобится. Определять можно по 0x00FF и 0xFF00

  _GPIOx_Reset  = GPIOx_Reset;                    // Порт и вывод сброса
  _TFT_Reset    = TFT_Reset;
  _GPIOx_CS     = GPIOx_CS;                       // Порт и вывод выбора кристалла
  _TFT_CS       = TFT_CS;
  _GPIOx_RS     = GPIOx_RS;                       // Порд и вывод команды = 0 и данных = 1
  _TFT_RS       = TFT_RS;
  _GPIOx_WR     = GPIOx_WR;                       // Порт и вывод строба записи
  _TFT_WR       = TFT_WR;
  _GPIOx_RD     = GPIOx_RD;                       // Порт и вывод строба чтения
  _TFT_RD       = TFT_RD;

//  uint8_t Temp;

  _SetSpeed(Speed_Low);                                     // Устанавливаем скорость порта
  _SetPin(_GPIOx_Reset, _TFT_Reset, Output, No_Pull);       // Все выводы на выход и в высокое состояние
  _DigitalWriteBit(_GPIOx_Reset, _TFT_Reset, IO_Level::High);
  _SetPin(_GPIOx_CS, _TFT_CS, Output, No_Pull);
  _DigitalWriteBit(_GPIOx_CS, _TFT_CS, IO_Level::High);
  _SetPin(_GPIOx_RS, _TFT_RS, Output, No_Pull);
  _DigitalWriteBit(_GPIOx_RS, _TFT_RS, IO_Level::High);
  _SetPin(_GPIOx_WR, _TFT_WR, Output, No_Pull);
  _DigitalWriteBit(_GPIOx_WR, _TFT_WR, IO_Level::High);
  _SetPin(_GPIOx_RD, _TFT_RD, Output, No_Pull);
  _DigitalWriteBit(_GPIOx_RD, _TFT_RD, IO_Level::High);

  if(!_TFT_Pin_Half)                                        // Инициализируем первую половину порта
    _SetPin(_GPIOx_Half, GPIO_PIN_0 |
        GPIO_PIN_1 |
        GPIO_PIN_2 |
        GPIO_PIN_3 |
        GPIO_PIN_4 |
        GPIO_PIN_5 |
        GPIO_PIN_6 |
        GPIO_PIN_7, Output, No_Pull);
  else                                                      // Инициализируем вторую половину порта
    _SetPin(_GPIOx_Half, GPIO_PIN_8 |
        GPIO_PIN_9 |
        GPIO_PIN_10 |
        GPIO_PIN_11 |
        GPIO_PIN_12 |
        GPIO_PIN_13 |
        GPIO_PIN_14 |
        GPIO_PIN_15, Output, No_Pull);
  // Хардварный сброс
  delay(100);
  _DigitalWriteBit(_GPIOx_Reset, _TFT_Reset, IO_Level::Low);
  delay(100);
  _DigitalWriteBit(_GPIOx_Reset, _TFT_Reset, IO_Level::High);
  delay(100);
  // Закончили хардварный сброс

  WR_REG(0x00);
  for(uint8_t i=0; i<3; i++)
  {
    _DigitalWriteBit(_GPIOx_WR, _TFT_WR, IO_Level::Low);
    _DigitalWriteBit(_GPIOx_WR, _TFT_WR, IO_Level::High);
  }
  _DigitalWriteBit(_GPIOx_CS, _TFT_CS, IO_Level::High);

//  uint8_t Temp1, Temp2, Temp3, Temp4;
//  WR_REG(0x04);
//  delayMicroseconds(50);
//  Temp1 = RD_DATA();
//  Temp2 = RD_DATA();
//  Temp3 = RD_DATA();
//  Temp4 = RD_DATA();
//  _DigtalWriteBit(_GPIOx_CS, _TFT_CS, IO_Level::High);
}

void ILI9341::WR_REG(uint8_t regval)
{
//  uint16_t Data;

  _DigitalWriteBit(_GPIOx_CS, _TFT_CS, IO_Level::Low);       // Выбираем экран
  _DigitalWriteBit(_GPIOx_RS, _TFT_RS, IO_Level::Low);       // Выбираем запись команды

  if(!_TFT_Pin_Half)                                        // Определяемся в какую половину пишем
  {
    _GPIOx_Half->BSRR = 0x00FF << 16;
    _GPIOx_Half->BSRR = regval;
  }
  else
  {
    _GPIOx_Half->BSRR = 0xFF00 << 16;
    _GPIOx_Half->BSRR = regval << 16;
  }

  _DigitalWriteBit(_GPIOx_WR, _TFT_WR, IO_Level::Low);       // Дёргаем WR вниз/вверх
  _DigitalWriteBit(_GPIOx_WR, _TFT_WR, IO_Level::High);
//  _DigtalWriteBit(_GPIOx_CS, _TFT_CS, IO_Level::High);      // Отключаем выбор экрана
}

void ILI9341::WR_DATA(uint8_t data)
{
//  uint16_t Data;

  _DigitalWriteBit(_GPIOx_RS, _TFT_RS, IO_Level::High);      // Выбираем запись данных

  if(!_TFT_Pin_Half)                                        // Определяемся в какую половину пишем
  {
    _GPIOx_Half->BSRR = 0x00FF << 16;
    _GPIOx_Half->BSRR = data;
  }
  else
  {
    _GPIOx_Half->BSRR = 0xFF00 << 16;
    _GPIOx_Half->BSRR = data << 16;
  }

  _DigitalWriteBit(_GPIOx_WR, _TFT_WR, IO_Level::Low);       // Дёргаем WR вниз/вверх
  _DigitalWriteBit(_GPIOx_WR, _TFT_WR, IO_Level::High);
}

uint8_t ILI9341::RD_DATA(void)
{
  uint16_t Data;
//  WR_REG(Reg);
//  delayMicroseconds(50);

  _DigitalWriteBit(_GPIOx_RS, _TFT_RS, IO_Level::Low);      // Выбираем запись данных
  delayMicroseconds(50);
//  _DigtalWriteBit(_GPIOx_CS, _TFT_CS, IO_Level::Low);       // Выбираем экран
  // Настраиваем на вход
  if(!_TFT_Pin_Half)                                      // Инициализируем первую половину порта
    _SetPin(_GPIOx_Half, GPIO_PIN_0 |
        GPIO_PIN_1 |
        GPIO_PIN_2 |
        GPIO_PIN_3 |
        GPIO_PIN_4 |
        GPIO_PIN_5 |
        GPIO_PIN_6 |
        GPIO_PIN_7, Output, No_Pull);
  else                                                    // Инициализируем вторую половину порта
    _SetPin(_GPIOx_Half, GPIO_PIN_8 |
        GPIO_PIN_9 |
        GPIO_PIN_10 |
        GPIO_PIN_11 |
        GPIO_PIN_12 |
        GPIO_PIN_13 |
        GPIO_PIN_14 |
        GPIO_PIN_15, Output, No_Pull);
  _DigitalWriteBit(_GPIOx_WR, _TFT_RD, IO_Level::Low);       // Дёргаем RD вниз/вверх
  _DigitalWriteBit(_GPIOx_WR, _TFT_RD, IO_Level::High);
//  _DigtalWriteBit(_GPIOx_CS, _TFT_CS, IO_Level::High);      // Отключаем выбор экрана

  Data = _GPIOx_Half->IDR;
  if(_TFT_Pin_Half) Data  >>= 8;

  // Настраиваем на выход
  if(!_TFT_Pin_Half)                                      // Инициализируем первую половину порта
    _SetPin(_GPIOx_Half, GPIO_PIN_0 |
        GPIO_PIN_1 |
        GPIO_PIN_2 |
        GPIO_PIN_3 |
        GPIO_PIN_4 |
        GPIO_PIN_5 |
        GPIO_PIN_6 |
        GPIO_PIN_7, Output, No_Pull);
  else                                                    // Инициализируем вторую половину порта
    _SetPin(_GPIOx_Half, GPIO_PIN_8 |
        GPIO_PIN_9 |
        GPIO_PIN_10 |
        GPIO_PIN_11 |
        GPIO_PIN_12 |
        GPIO_PIN_13 |
        GPIO_PIN_14 |
        GPIO_PIN_15, Output, No_Pull);

  return (uint8_t) Data;
}

void ILI9341::drawPixel(uint16_t x, uint16_t y, uint16_t color)
{
  if((x<0)||(y<0)||(x>=_width)||(y>=_height)) return;

//  _DigtalWriteBit(_GPIOx_CS, _TFT_CS, IO_Level::Low);       // Выбираем экран

  CursorXY(x, y);
  WR_REG(CMD_RAM_WRITE);
  WR_DATA(color >> 8);
  WR_DATA(color & 0xFF);

  _DigitalWriteBit(_GPIOx_CS, _TFT_CS, IO_Level::High);      // Отключаем выбор экрана
}

void ILI9341::CursorXY(uint16_t x, uint16_t y)
{
  WR_REG(CMD_SET_X);
  WR_DATA(x>>8);
  WR_DATA(x&0XFF);
  WR_DATA(x>>8);
  WR_DATA(x&0XFF);
  _DigitalWriteBit(_GPIOx_CS, _TFT_CS, IO_Level::High);      // Отключаем выбор экрана
  WR_REG(CMD_SET_Y);
  WR_DATA(y>>8);
  WR_DATA(y&0XFF);
  WR_DATA(y>>8);
  WR_DATA(y&0XFF);
  _DigitalWriteBit(_GPIOx_CS, _TFT_CS, IO_Level::High);      // Отключаем выбор экрана
}

void ILI9341::setWindow(uint16_t sx, uint16_t sy, uint16_t width, uint16_t height)
{
//  WR_REG(CMD_SET_X);
//  WR_DATA(sx>>8);
//  WR_DATA(sx&0XFF);
//  WR_DATA(width>>8);
//  WR_DATA(width&0XFF);
//  WR_REG(CMD_SET_Y);
//  WR_DATA(sy>>8);
//  WR_DATA(sy&0XFF);
//  WR_DATA(height>>8);
//  WR_DATA(height&0XFF);
}

void ILI9341::fillScreen(uint16_t color)
{
  uint32_t index=0;
  uint32_t totalpoint=_width;
  _DigitalWriteBit(_GPIOx_CS, _TFT_CS, IO_Level::Low);       // Выбираем экран

  totalpoint *= _height;
  setWindow(0, 0, _width, _height);
  WR_REG(CMD_RAM_WRITE);
  for(index=0;index<totalpoint;index++)
  {
    WR_DATA(color >> 8);
    WR_DATA(color & 0xFF);
  }
  _DigitalWriteBit(_GPIOx_CS, _TFT_CS, IO_Level::High);      // Выбираем экран
}

void ILI9341::setRotation(uint8_t x)
{
//  WR_REG(MADCTL);
//  rotation = x % 8;
//  switch (rotation)
//  {
//    case 0: // Portrait
//      WR_DATA(MAD_BGR | MAD_MX);
//      _width = WIDTH;
//      _height = HEIGHT;
//      break;
//    case 1: // Landscape (Portrait + 90)
//      WR_DATA(MAD_BGR | MAD_MV);
//      _width = HEIGHT;
//      _height = WIDTH;
//      break;
//    case 2: // Inverter portrait
//      WR_DATA(MAD_BGR | MAD_MY);
//      _width = WIDTH;
//      _height = HEIGHT;
//      break;
//    case 3: // Inverted landscape
//      WR_DATA(MAD_BGR | MAD_MV | MAD_MX | MAD_MY);
//      _width = HEIGHT;
//      _height = WIDTH;
//      break;
//    case 4: // Portrait
//      WR_DATA(MAD_BGR | MAD_MX | MAD_MY);
//      _width = WIDTH;
//      _height = HEIGHT;
//      break;
//    case 5: // Landscape (Portrait + 90)
//      WR_DATA(MAD_BGR | MAD_MV | MAD_MX);
//      _width = HEIGHT;
//      _height = WIDTH;
//      break;
//    case 6: // Inverter portrait
//      WR_DATA(MAD_BGR);
//      _width = WIDTH;
//      _height = HEIGHT;
//      break;
//    case 7: // Inverted landscape
//      WR_DATA(MAD_BGR | MAD_MV | MAD_MY);
//      _width = HEIGHT;
//      _height = WIDTH;
//      break;
//  }
}


//****************************************************************************** 
//  ENF OF FILE 
//****************************************************************************** 

