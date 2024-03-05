/*
 * HX8347_8bit.cpp
 *
 *  Created on: 17 нояб. 2021 г.
 *      Author: User
 */

#include  "HX8347_8bit.h"      // Включаем файл заголовка для нашего модуля



HX8347::HX8347():GFX_TFT(240,320)
{
  //_GPIO_Pin = 0;
}

void HX8347::Init(GPIO_TypeDef *GPIOx_Data, uint16_t Data_Pin,
		GPIO_TypeDef *GPIOx_Reset,	uint16_t TFT_Reset,
		GPIO_TypeDef *GPIOx_CS, uint16_t TFT_CS,
		GPIO_TypeDef *GPIOx_RS, uint16_t TFT_RS,
		GPIO_TypeDef *GPIOx_WR, uint16_t TFT_WR,
		GPIO_TypeDef *GPIOx_RD,	uint16_t TFT_RD)
{
  // Инициализируем внутринние переменные
  _GPIOx_Data   = GPIOx_Data;
  _Data_Pin     = Data_Pin;

  _GPIOx_Reset  = GPIOx_Reset;                    // Порт и вывод сброса
  _TFT_Reset    = TFT_Reset;
  _GPIOx_CS     = GPIOx_CS;                       // Порт и вывод выбора кристалла
  _TFT_CS       = TFT_CS;
  _GPIOx_RS     = GPIOx_RS;                       // Порт и вывод команды = 0 и данных = 1
  _TFT_RS       = TFT_RS;
  _GPIOx_WR     = GPIOx_WR;                       // Порт и вывод строба записи
  _TFT_WR       = TFT_WR;
  _GPIOx_RD     = GPIOx_RD;                       // Порт и вывод строба чтения
  _TFT_RD       = TFT_RD;

//  uint8_t Temp;

  _SetSpeed(Speed_WeryHigh);                                     // Устанавливаем скорость порта
  _SetEdge(High);
  _SetPin(_GPIOx_Reset, _TFT_Reset, Output, No_Pull);            // Все выводы на выход и в высокое состояние
  _SetPin(_GPIOx_CS, _TFT_CS, Output, No_Pull);
  _SetPin(_GPIOx_RS, _TFT_RS, Output, No_Pull);
  _SetPin(_GPIOx_WR, _TFT_WR, Output, No_Pull);
  _SetPin(_GPIOx_RD, _TFT_RD, Output, No_Pull);
  _SetEdge(Low);

  _FirstPin = 0;
  uint16_t PinShift = _Data_Pin;

  while(PinShift)                                                     // Подсчитываем номер пина с которого начинается вывод
  {
    _FirstPin++;
    PinShift = PinShift >> 1;
  }
  _FirstPin--;

  _PinMask = 0;
  uint8_t Size = 8;
  while(Size--)                                                       // Собираем список инициализируемых пинов
  {
    _PinMask = _PinMask << 1;
    _PinMask |= _Data_Pin;
  }

  _SetPull(_PinMask);
  _SetPin(_GPIOx_Data, _PinMask, Output, No_Pull);

  // Хардварный сброс
  delay(100);
  _DigitalWriteBit(_GPIOx_Reset, _TFT_Reset, IO_Level::Low);
  delay(100);
  _DigitalWriteBit(_GPIOx_Reset, _TFT_Reset, IO_Level::High);
  delay(100);
  // Инициализация
  /* TE signal ON */
  writeReg(0x2D, 0x1D); // Cycle control 1 (GDON)
  writeReg(0x2E, 0x83); // Cycle control 2 (GDOF)
  writeReg(0xE4, 0x02); // Power saving 1 (EQVCI_M1)

  /* Power Voltage Setting */
  writeReg(0xE5, 0x26); // Power saving 2 (EQGND_M1)
  writeReg(0xE6, 0x26); // Power saving 3 (EQVCI_M0)
  writeReg(0xE7, 0x02); // Power saving 4 (EQGND_M0)
  writeReg(0xE8, 0x6E); // Source OP control_Normal (OPON_N)
  writeReg(0xE9, 0x46); // Source OP control_IDLE (OPON_I)

  /* VCOM offset */
  writeReg(0xEA, 0x00); // Power control internal use (1) (STBA)
  writeReg(0xEB, 0x24); // Power control internal use (2) (STBA)
  writeReg(0xEC, 0x00); // Source control internal use (1) (PTBA)

  delay(5);

  writeReg(0xED, 0xc4); // Source control internal use (2) (PTBA)
  writeReg(0xF3, 0x00); // Power saving 5 (SEQVCI_M1)

  delay(5);

  /* Display ON Setting */
  writeReg(0xF4, 0x00); // Power saving 6 (SEQGND_M0)

  delay(40);

  writeReg(0x1B, 0x1E); // Power control 2 (VRH[5:0])
  writeReg(0x1A, 0x04); // Power control 1 (BT[2:0])
  writeReg(0x24, 0x7F); // VCOM Control 2 (VMH)
  writeReg(0x25, 0x17); // VCOM Control 3 (VML)
  writeReg(0x23, 0x75); // VCOM Control 1 (VMF)
  //writeReg(0xE2, 0x04); // no such register for HX8347-I
  writeReg(0x19, 0x01); // OSC Control 1 (OSC_EN[0])
  writeReg(0x1C, 0x03); // Power control 3 (AP[2:0])
  writeReg(0x01, 0x00); // Display mode control

  /* Power Voltage Setting */
  writeReg(0x1F, 0x88); // Power control 6

  delay(5);

  writeReg(0x1F, 0x80);  // Power control 6

  delay(5);

  writeReg(0x1F, 0x90); // Power control 6

  delay(5);

  writeReg(0x1F, 0xD4);  // Power control 6

  delay(5);

  writeReg(0x18, 0x66); // OSC Control 2
  writeReg(0x1A, 0x02); // Power control 1
  writeReg(0x17, 0x05); // COLMOD
  writeReg(0x36, 0x09); // Panel characteristic
  writeReg(0x28, 0x38); // Display control 3

  delay(40);

  writeReg(0x28, 0x3C); // Display control 3

  delay(40);

  /* Gamma 2.2 Setting */
  writeReg(0x40, 0x00);
  writeReg(0x41, 0x02);
  writeReg(0x42, 0x06);
  writeReg(0x43, 0x32);
  writeReg(0x44, 0x33);
  writeReg(0x45, 0x3B);
  writeReg(0x46, 0x26);
  writeReg(0x47, 0x7D);
  writeReg(0x48, 0x01);
  writeReg(0x49, 0x15);
  writeReg(0x4A, 0x1A);
  writeReg(0x4B, 0x1A);
  writeReg(0x4C, 0x14);
  writeReg(0x50, 0x04);
  writeReg(0x51, 0x0C);
  writeReg(0x52, 0x0D);
  writeReg(0x53, 0x39);
  writeReg(0x54, 0x3D);
  writeReg(0x55, 0x3F);
  writeReg(0x56, 0x02);
  writeReg(0x57, 0x59);
  writeReg(0x58, 0x0B);
  writeReg(0x59, 0x05);
  writeReg(0x5A, 0x05);
  writeReg(0x5B, 0x0A);
  writeReg(0x5C, 0x1E);
  writeReg(0x5D, 0x88);

  /*    PAGE 2  */
  writeReg(0xFF, 0x02); // Page select
  writeReg(0x0C, 0x40); // Partial area end row 2

  /*    PAGE 0  */
  writeReg(0xFF, 0x00); // Page select

  delay(100);

  writeReg(0x16, 0x10); // Memory access control, rotation here

}

uint8_t HX8347::GetID(void)
{
  return RD_REG(0x00);
}

void HX8347::drawPixel(uint16_t x, uint16_t y, uint16_t color)
{
  if((x<0)||(y<0)||(x>=_width)||(y>=_height)) return;

  CursorXY(x, y);
  WR_DATA(color >> 8);
  WR_DATA(color & 0xFF);

  _DigitalWriteBit(_GPIOx_CS, _TFT_CS, IO_Level::High);      // Отключаем выбор экрана
}

void HX8347::CursorXY(uint16_t x, uint16_t y)
{
  setWindow(x, y, x, y);
}

void HX8347::setWindow(uint16_t sx, uint16_t sy, uint16_t width, uint16_t height)
{
  writeReg(0x02, sx >> 8); // Column address start 2
  writeReg(0x03, sx & 0xFF); // Column address start 1
  writeReg(0x04, width >> 8); // Column address end 2
  writeReg(0x05, width & 0xFF); // Column address end 1
  writeReg(0x06, sy >> 8); // Row address start 2
  writeReg(0x07, sy & 0xFF); // Row address start 1
  writeReg(0x08, height >> 8); // Row address end 2
  writeReg(0x09, height & 0xFF); // Row address end 1

  WR_REG(0x22);
}

void HX8347::fillScreen(uint16_t color)
{
   setWindow(0, 0, _width - 1, _height - 1);
   for (int i = 0; i < _width * _height; i++)
   {
     WR_DATA(color >> 8);
     WR_DATA(color & 0xFF);
   }
   _DigitalWriteBit(_GPIOx_CS, _TFT_CS, IO_Level::High);      // Отключаем выбор экрана
}

void HX8347::setRotation(uint8_t x)
{
  WR_REG(MADCTL);
  rotation = x % 8;
  switch (rotation)
  {
    case 0: // Portrait
      WR_DATA(0x00);
      _width = WIDTH;
      _height = HEIGHT;
      break;
    case 1: // Landscape (Portrait + 90)
      WR_DATA(MAD_MV |MAD_MX);
      _width = HEIGHT;
      _height = WIDTH;
      break;
    case 2: // Portrait
      WR_DATA(MAD_MX | MAD_MY);
      _width = WIDTH;
      _height = HEIGHT;
      break;
    case 3: // Landscape (Portrait + 90)
      WR_DATA(MAD_MY | MAD_MV);
      _width = HEIGHT;
      _height = WIDTH;
      break;
  }
}

/*
 * Системные функции
 */
void    HX8347::writeReg(uint8_t Reg, uint8_t Data)
{
  WR_REG(Reg);
  WR_DATA(Data);
  _DigitalWriteBit(_GPIOx_CS, _TFT_CS, IO_Level::High);      // Отключаем выбор экрана
}

void HX8347::WR_REG(uint8_t regval)
{
  _DigitalWriteBit(_GPIOx_CS, _TFT_CS, IO_Level::Low);       // Выбираем экран
  _DigitalWriteBit(_GPIOx_RS, _TFT_RS, IO_Level::Low);       // Выбираем запись команды

  _DigitalWrite(_GPIOx_Data, regval, _FirstPin, _PinMask);

  _DigitalWriteBit(_GPIOx_WR, _TFT_WR, IO_Level::Low);       // Дёргаем WR вниз/вверх
  _DigitalWriteBit(_GPIOx_WR, _TFT_WR, IO_Level::High);
}

void HX8347::WR_DATA(uint8_t data)
{
  _DigitalWriteBit(_GPIOx_RS, _TFT_RS, IO_Level::High);      // Выбираем запись данных

  _DigitalWrite(_GPIOx_Data, data, _FirstPin, _PinMask);

  _DigitalWriteBit(_GPIOx_WR, _TFT_WR, IO_Level::Low);       // Дёргаем WR вниз/вверх
  _DigitalWriteBit(_GPIOx_WR, _TFT_WR, IO_Level::High);
}

uint8_t HX8347::RD_DATA(void)
{
  uint16_t Data;

  _DigitalWriteBit(_GPIOx_RS, _TFT_RS, IO_Level::Low);      // Выбираем запись данных
  _DigitalWriteBit(_GPIOx_CS, _TFT_CS, IO_Level::Low);       // Выбираем экран
  // Настраиваем на вход
  _SetPin(_GPIOx_Data, _PinMask, Input, No_Pull);

  _DigitalWriteBit(_GPIOx_RD, _TFT_RD, IO_Level::Low);       // Дёргаем RD вниз/вверх !!!!!!! Это правильно?
  _DigitalWriteBit(_GPIOx_RD, _TFT_RD, IO_Level::High);

  Data = (uint8_t) _DigitalRead(_GPIOx_Data, _FirstPin, _PinMask);

  _SetPin(_GPIOx_Data, _PinMask, Output, No_Pull);
  return (uint8_t) Data;
}

uint8_t HX8347::RD_REG(uint8_t Reg)
{
  uint8_t Data;

  // Пишем регистр или команду (строб записи)
  WR_REG(Reg);
  // Перенастраиваем порт на вход
  _SetPin(_GPIOx_Data, _PinMask, Input, No_Pull);

  _DigitalWriteBit(_GPIOx_RS, _TFT_RS, IO_Level::High);      // Выбираем чтение данных

  Data = (uint8_t) _DigitalRead(_GPIOx_Data, _FirstPin, _PinMask);

  _DigitalWriteBit(_GPIOx_RD, _TFT_RD, IO_Level::Low);      // Строб чтения
  _DigitalWriteBit(_GPIOx_RD, _TFT_RD, IO_Level::High);      //

  // Перенастраиваем порты на выход
  _SetPin(_GPIOx_Data, _PinMask, Output, No_Pull);
  return Data;
}

//****************************************************************************** 
//  ENF OF FILE 
//****************************************************************************** 

