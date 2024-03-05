/*
 * ST7735_SPI.cpp
 *
 *  Created on: 17 июн. 2022 г.
 *      Author: User
 */

#include "ST7735_SPI.h"

const uint8_t  Rcmd1[] =
  {                       // 7735R init, part 1 (red or green tab)
    15,                             // 15 commands in list:
    ST77XX_SWRESET,   ST_CMD_DELAY, //  1: Software reset, 0 args, w/delay
      150,                          //     150 ms delay
    ST77XX_SLPOUT,    ST_CMD_DELAY, //  2: Out of sleep mode, 0 args, w/delay
      255,                          //     500 ms delay
    ST7735_FRMCTR1, 3,              //  3: Framerate ctrl - normal mode, 3 arg:
      0x01, 0x2C, 0x2D,             //     Rate = fosc/(1x2+40) * (LINE+2C+2D)
    ST7735_FRMCTR2, 3,              //  4: Framerate ctrl - idle mode, 3 args:
      0x01, 0x2C, 0x2D,             //     Rate = fosc/(1x2+40) * (LINE+2C+2D)
    ST7735_FRMCTR3, 6,              //  5: Framerate - partial mode, 6 args:
      0x01, 0x2C, 0x2D,             //     Dot inversion mode
      0x01, 0x2C, 0x2D,             //     Line inversion mode
    ST7735_INVCTR,  1,              //  6: Display inversion ctrl, 1 arg:
      0x07,                         //     No inversion
    ST7735_PWCTR1,  3,              //  7: Power control, 3 args, no delay:
      0xA2,
      0x02,                         //     -4.6V
      0x84,                         //     AUTO mode
    ST7735_PWCTR2,  1,              //  8: Power control, 1 arg, no delay:
      0xC5,                         //     VGH25=2.4C VGSEL=-10 VGH=3 * AVDD
    ST7735_PWCTR3,  2,              //  9: Power control, 2 args, no delay:
      0x0A,                         //     Opamp current small
      0x00,                         //     Boost frequency
    ST7735_PWCTR4,  2,              // 10: Power control, 2 args, no delay:
      0x8A,                         //     BCLK/2,
      0x2A,                         //     opamp current small & medium low
    ST7735_PWCTR5,  2,              // 11: Power control, 2 args, no delay:
      0x8A, 0xEE,
    ST7735_VMCTR1,  1,              // 12: Power control, 1 arg, no delay:
      0x0E,
    ST77XX_INVOFF,  0,              // 13: Don't invert display, no args
    ST77XX_MADCTL,  1,              // 14: Mem access ctl (directions), 1 arg:
      0xC8,                         //     row/col addr, bottom-top refresh
    ST77XX_COLMOD,  1,              // 15: set color mode, 1 arg, no delay:
      0x05 };                        //     16-bit color

const uint8_t  Rcmd2red[] = {                    // 7735R init, part 2 (red tab only)
    2,                              //  2 commands in list:
    ST77XX_CASET,   4,              //  1: Column addr set, 4 args, no delay:
      0x00, 0x00,                   //     XSTART = 0
      0x00, 0x7F,                   //     XEND = 127
    ST77XX_RASET,   4,              //  2: Row addr set, 4 args, no delay:
      0x00, 0x00,                   //     XSTART = 0
      0x00, 0x9F };                 //     XEND = 159

const uint8_t  Rcmd3[] = {                       // 7735R init, part 3 (red or green tab)
    4,                              //  4 commands in list:
    ST7735_GMCTRP1, 16      ,       //  1: Gamma Adjustments (pos. polarity), 16 args + delay:
      0x02, 0x1c, 0x07, 0x12,       //     (Not entirely necessary, but provides
      0x37, 0x32, 0x29, 0x2d,       //      accurate colors)
      0x29, 0x25, 0x2B, 0x39,
      0x00, 0x01, 0x03, 0x10,
    ST7735_GMCTRN1, 16      ,       //  2: Gamma Adjustments (neg. polarity), 16 args + delay:
      0x03, 0x1d, 0x07, 0x06,       //     (Not entirely necessary, but provides
      0x2E, 0x2C, 0x29, 0x2D,       //      accurate colors)
      0x2E, 0x2E, 0x37, 0x3F,
      0x00, 0x00, 0x02, 0x10,
    ST77XX_NORON,     ST_CMD_DELAY, //  3: Normal display on, no args, w/delay
      10,                           //     10 ms delay
    ST77XX_DISPON,    ST_CMD_DELAY, //  4: Main screen turn on, no args w/delay
      100 };                        //     100 ms delay


ST7735_SPI::ST7735_SPI(SPI* SPI,
                       GPIO_TypeDef* GPIO_CS, int16_t GPIO_PIN_CS,
                       GPIO_TypeDef* GPIO_DC, int16_t GPIO_PIN_DC,
                       GPIO_TypeDef* GPIO_Blk, int16_t GPIO_PIN_Blk,
                       GPIO_TypeDef* GPIO_RST = NULL, int16_t GPIO_PIN_RST = 0) : GFX_TFT(128, 160)
{
  _SPI          = SPI;
  _GPIO_CS      = GPIO_CS;
  _GPIO_PIN_CS  = GPIO_PIN_CS;
  _GPIO_DC      = GPIO_DC;
  _GPIO_PIN_DC  = GPIO_PIN_DC;
  _GPIO_Blk     = GPIO_Blk;
  _GPIO_PIN_Blk = GPIO_PIN_Blk;
  _GPIO_RST     = GPIO_RST;
  _GPIO_PIN_RST = GPIO_PIN_RST;
}

ST7735_SPI::ST7735_SPI(SPI* SPI,
                       GPIO_TypeDef* GPIO_CS, int16_t GPIO_PIN_CS,
                       GPIO_TypeDef* GPIO_DC, int16_t GPIO_PIN_DC,
                       GPIO_TypeDef* GPIO_Blk = NULL, int16_t GPIO_PIN_Blk = 0) : GFX_TFT(128, 160)
{
  _SPI          = SPI;
  _GPIO_CS      = GPIO_CS;
  _GPIO_PIN_CS  = GPIO_PIN_CS;
  _GPIO_DC      = GPIO_DC;
  _GPIO_PIN_DC  = GPIO_PIN_DC;
  _GPIO_Blk     = GPIO_Blk;
  _GPIO_PIN_Blk = GPIO_PIN_Blk;
}

void ST7735_SPI::Init(uint8_t Type)
{
//  _SPI->Transmit(0x00);
  //--------------- Инициализация портов -----------------------------
  _SetSpeed(Speed_WeryHigh);
  _SetPin(_GPIO_CS, _GPIO_PIN_CS, Output, No_Pull);
  CS_Set(High);
  _SetPin(_GPIO_DC, _GPIO_PIN_DC, Output, No_Pull);
  DC_Set(High);                                             // Изначально стоит на передаче данных

  if(_GPIO_Blk != NULL)
  {
    _SetPin(_GPIO_Blk, _GPIO_PIN_Blk, Output, No_Pull);
    Blk_Set(Low);
  }

  if(_GPIO_RST != NULL)
  {
    _SetPin(_GPIO_RST, _GPIO_PIN_RST, Output, No_Pull);
    RST_Set(High);
  }
  _SetSpeed(Speed_Low);

  // Сбрасываем дисплей, если есть такой вывод
  if(_GPIO_RST != NULL)
  {
    RST_Set(Low);
    delay(100);
    RST_Set(High);
  }

  // Далее из библиотеки Adafruit ST77xx - ST7735
  displayInit(Rcmd1);

  if(Type == INITR_GREENTAB)
  {
//    displayInit(Rcmd2green);
    _colstart = 2;
    _rowstart = 1;
  }
  else if((Type == INITR_144GREENTAB) || (Type == INITR_HALLOWING))
  {
    _height   = 128;
    _width    = 128;
//    displayInit(Rcmd2green144);
    _colstart = 2;
    _rowstart = 3; // For default rotation 0
  } else if(Type == INITR_MINI160x80)
  {
    _height   = 160;
    _width    = 80;
//    displayInit(Rcmd2green160x80);
    _colstart = 24;
    _rowstart = 0;
  }
  else
  {
    // colstart, rowstart left at default '0' values
    displayInit(Rcmd2red);
  }

  displayInit(Rcmd3);

  // Black tab, change MADCTL color filter
  if((Type == Type_1x8_128x160) || (Type == INITR_MINI160x80))
  {
    uint8_t data = 0xC0;
    CS_Set(Low);
    sendCommand(ST77XX_MADCTL, &data, 1);
    CS_Set(High);
  }

  if(Type == INITR_HALLOWING)
  {
    // Hallowing is simply a 1.44" green tab upside-down:
    tabcolor = INITR_144GREENTAB;
    setRotation(2);
  }
  else
  {
    tabcolor = Type;
    setRotation(0);
  }
  if(_GPIO_Blk != NULL) Blk_Set(High);
}

/*
 * Графика
 */
// -------------------------------------------------------------------
void ST7735_SPI::setWindow(uint16_t sx, uint16_t sy, uint16_t width, uint16_t height)
{
  uint8_t Data[] = {(uint8_t)(sx>>8), (uint8_t)(sx&0xFF), (uint8_t)((width)>>8), (uint8_t)((width)&0xFF)};
  sendCommand(0x2A, Data, 4);
  uint8_t Data1[] = {(uint8_t)(sy>>8), (uint8_t)(sy&0xFF), (uint8_t)((height)>>8), (uint8_t)((height)&0xFF)};
  sendCommand(0x2B, Data1, 4);
}

void ST7735_SPI::CursorXY(uint16_t x, uint16_t y)
{
  uint8_t Data[] = {(uint8_t)(x>>8), (uint8_t)(x&0xFF), (uint8_t)(x>>8), (uint8_t)(x&0xFF)};
  sendCommand(0x2A, Data, 4);
  uint8_t Data1[] = {(uint8_t)(y>>8), (uint8_t)(y&0xFF), (uint8_t)(y>>8), (uint8_t)(y&0xFF)};
  sendCommand(0x2B, Data1, 4);
}

void ST7735_SPI::fillScreen(uint16_t color)
{
  uint32_t index=0;
  uint32_t totalpoint=_width;
  totalpoint *= _height;

  CS_Set(Low);
  setWindow(0, 0, _width, _height);
  while(_SPI->getFlagBSY()){}
  DC_Set(DC_Comm);
  _SPI->Transmit(0x2C);
  while(_SPI->getFlagBSY()){}
  DC_Set(DC_Data);

  for(index=0;index<totalpoint;index++)
  {
    _SPI->Transmit(color>>8);
    _SPI->Transmit(color&0xFF);
  }
  while(_SPI->getFlagBSY()){}
  CS_Set(High);
}

void ST7735_SPI::drawPixel(uint16_t x, uint16_t y, uint16_t color)
{
  if((x<0)||(y<0)||(x>=_width)||(y>=_height)) return;

  CS_Set(Low);
  CursorXY(x, y);
  DC_Set(DC_Comm);
  _SPI->Transmit(0x2C);
  while(_SPI->getFlagBSY()){}
  DC_Set(DC_Data);

  _SPI->Transmit(color>>8);
  _SPI->Transmit(color&0xFF);
  while(_SPI->getFlagBSY()){}
  CS_Set(High);
}

void ST7735_SPI::drawFillFastRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color)
{
  uint32_t index = 0;
  uint32_t totalpoint = w;
  totalpoint *= h;

  CS_Set(Low);
  setWindow(x, y, x+w-1, y+h-1);
  while (_SPI->getFlagBSY()){}
  DC_Set(DC_Comm);
  _SPI->Transmit(0x2C);
  while (_SPI->getFlagBSY()){}
  DC_Set(DC_Data);

  for (index = 0; index < totalpoint; index++)
  {
    _SPI->Transmit(color >> 8);
    _SPI->Transmit(color & 0xFF);
  }
  while (_SPI->getFlagBSY()){}
  CS_Set(High);
}

void ST7735_SPI::setRotation(uint8_t rot)
{
  uint8_t madctl = 0;

  rotation = rot % 4; // can't be higher than 3

  // For ST7735 with GREEN TAB (including HalloWing)...
  if((tabcolor == INITR_144GREENTAB) || (tabcolor == INITR_HALLOWING))
  {
    // ..._rowstart is 3 for rotations 0&1, 1 for rotations 2&3
    _rowstart = (rotation < 2) ? 3 : 1;
  }

  switch (rotation)
  {
    case 0:
      if((tabcolor == Type_1x8_128x160) || (tabcolor == INITR_MINI160x80))
      {
        madctl = ST77XX_MADCTL_MX | ST77XX_MADCTL_MY | ST77XX_MADCTL_RGB;
      }
      else
      {
        madctl = ST77XX_MADCTL_MX | ST77XX_MADCTL_MY | ST7735_MADCTL_BGR;
      }

      if(tabcolor == INITR_144GREENTAB)
      {
        _height = 128;
        _width = 128;
      }
      else if(tabcolor == INITR_MINI160x80)
      {
        _height = 160;
        _width = 80;
      }
      else
      {
        _height = 160;
        _width = 128;
      }
      _xstart = _colstart;
      _ystart = _rowstart;
      break;
    case 1:
      if((tabcolor == Type_1x8_128x160) || (tabcolor == INITR_MINI160x80))
      {
        madctl = ST77XX_MADCTL_MY | ST77XX_MADCTL_MV | ST77XX_MADCTL_RGB;
      }
      else
      {
        madctl = ST77XX_MADCTL_MY | ST77XX_MADCTL_MV | ST7735_MADCTL_BGR;
      }

      if(tabcolor == INITR_144GREENTAB)
      {
        _width = 128;
        _height = 128;
      }
      else if(tabcolor == INITR_MINI160x80)
      {
        _width = 160;
        _height = 80;
      }
      else
      {
        _width = 160;
        _height = 128;
      }
      _ystart = _colstart;
      _xstart = _rowstart;
      break;
    case 2:
      if((tabcolor == Type_1x8_128x160) || (tabcolor == INITR_MINI160x80))
      {
        madctl = ST77XX_MADCTL_RGB;
      }
      else
      {
        madctl = ST7735_MADCTL_BGR;
      }

      if(tabcolor == INITR_144GREENTAB)
      {
        _height = 128;
        _width = 128;
      }
      else if(tabcolor == INITR_MINI160x80)
      {
        _height = 160;
        _width = 80;
      }
      else
      {
        _height = 160;
        _width = 128;
      }
      _xstart = _colstart;
      _ystart = _rowstart;
      break;
    case 3:
      if((tabcolor == Type_1x8_128x160) || (tabcolor == INITR_MINI160x80))
      {
        madctl = ST77XX_MADCTL_MX | ST77XX_MADCTL_MV | ST77XX_MADCTL_RGB;
      }
      else
      {
        madctl = ST77XX_MADCTL_MX | ST77XX_MADCTL_MV | ST7735_MADCTL_BGR;
      }

      if(tabcolor == INITR_144GREENTAB)
      {
        _width = 128;
        _height = 128;
      }
      else if(tabcolor == INITR_MINI160x80)
      {
        _width = 160;
        _height = 80;
      }
      else
      {
        _width = 160;
        _height = 128;
      }
      _ystart = _colstart;
      _xstart = _rowstart;
      break;
  }

  CS_Set(Low);
  sendCommand(ST77XX_MADCTL, &madctl, 1);
  CS_Set(High);
}

void ST7735_SPI::setDisplayBlk(bool setblk)
{
  DC_Set(DC_Comm);
  CS_Set(Low);
  if(setblk)
  {
    if(_GPIO_Blk != NULL) Blk_Set(High);
    _SPI->Transmit(ST77XX_DISPON);
  }
  else
  {
    if(_GPIO_Blk != NULL) Blk_Set(Low);
    _SPI->Transmit(ST77XX_DISPOFF);
  }
  while(_SPI->getFlagBSY()){}
  CS_Set(High);
  DC_Set(DC_Data);
}

void ST7735_SPI::setInverse(bool inv)
{
  DC_Set(DC_Comm);
  CS_Set(Low);
  if(inv)
  {
    _SPI->Transmit(ST77XX_INVON);
  }
  else
  {
    _SPI->Transmit(ST77XX_INVOFF);
  }
  while(_SPI->getFlagBSY()){}
  CS_Set(High);
  DC_Set(DC_Data);
}

/*
 * Локальные функции инициализации железа
 */
// -------------------- Инициализация железа дисплея -----------------
void ST7735_SPI::displayInit(const uint8_t *addr) // Готово
{

  uint8_t  numCommands, cmd, numArgs;
  uint16_t ms;

  CS_Set(Low);
  numCommands = *(const unsigned char *) (addr++);          // Количество команд в массиве
  while(numCommands--)                                      // Цикл чтения команд
  {
    cmd = *(const unsigned char *) (addr++);                // Чтение команды
    numArgs  = *(const unsigned char *) (addr++);           // Количество аргументов команды
    ms       = numArgs & ST_CMD_DELAY;                      // Если старший бит установлен, это задержка.
    numArgs &= ~ST_CMD_DELAY;                               // Убираем бит задержки
    sendCommand(cmd, addr, numArgs);
    addr += numArgs;

    if(ms)
    {
      ms = *(const unsigned char *) (addr++);               // Чтение задержки
      if(ms == 255) ms = 500;                               // Если задержка 255, увеличиваем ей до 500
      delay(ms);
    }
  }
  while(_SPI->getFlagBSY()){}
  CS_Set(High);
}

// -------------------- Отсылка команды и данных ---------------------
void ST7735_SPI::sendCommand(uint8_t cmd, const uint8_t *addr, uint8_t numArgs) // Готово
{
  uint16_t Temp = 0;
  DC_Set(Low);
  _SPI->Transmit(cmd);
  while(_SPI->getFlagBSY()){}
  DC_Set(High);

  while(numArgs--)
  {
    Temp = *(const unsigned char*) (addr++);
    _SPI->Transmit(Temp);
  }
  while(_SPI->getFlagBSY()){}
}

uint32_t  ST7735_SPI::getID(void)
{
  uint8_t RxBuff[4] = {0,};
  uint32_t _ID = 0;
  CS_Set(Low);
  DC_Set(Low);
  _SPI->Transmit(0x04);
  while(_SPI->getFlagBSY()){}
  DC_Set(High);

  _SPI->Receive(RxBuff, 4);
  _ID = (RxBuff[0] << 24) | (RxBuff[1] << 16) | (RxBuff[2] << 8) | RxBuff[3];
  CS_Set(High);
  return _ID;
}

