/*
 * ILI9341_FSMC16.cpp
 *
 *  Created on: 8 апр. 2021 г.
 *      Author: User
 */

#include "ILI9341_FSMC16.h"

struct LCD_TypeDef *LCD;

void      WR_REG(uint16_t regval);
void      WR_DATA(uint16_t data);
void      WriteReg(uint16_t LCD_Reg, uint16_t LCD_RegValue);
uint16_t  RD_DATA(void);
uint16_t  ReadReg(uint16_t LCD_Reg);


ILI9341_FSMC16::ILI9341_FSMC16():GFX_TFT(240, 320)
{
  _AddressSetupTime      = 15; //0;//15;
  _AddressHoldTime       = 15; //5;//15;
  _DataSetupTime         = 255; //0;//255;
  _BusTurnAroundDuration = 15; //0;//15;
  _CLKDivision           = 16; //0;//16;
  _DataLatency           = 17; //0;//17;
  _AccessMode            = FSMC_ACCESS_MODE_A;

  _FSMC_Initialized = 0;
  _FSMC_Ax = FSMC_A18;
  _TFT_Base = 0x60000000;
  _TFT_Ax   = 0x0007FFFE;
}

void ILI9341_FSMC16::FSMC_Speed(uint8_t AddressSetupTime, uint8_t AddressHoldTime, uint8_t DataSetupTime, uint8_t BusTurnAroundDuration,
                  uint8_t CLKDivision, uint8_t DataLatency, uint8_t AccessMode)
{
  _AddressSetupTime      = AddressSetupTime; //0;//15;
  _AddressHoldTime       = AddressHoldTime; //5;//15;
  _DataSetupTime         = DataSetupTime; //0;//255;
  _BusTurnAroundDuration = BusTurnAroundDuration; //0;//15;
  _CLKDivision           = CLKDivision; //0;//16;
  _DataLatency           = DataLatency; //0;//17;
  _AccessMode            = AccessMode;
}

void ILI9341_FSMC16::FSMC_Init(uint8_t Ax_Pin, uint8_t NEx_Pin)
{
  _FSMC_Ax  = Ax_Pin;
  _FSMC_NEx = NEx_Pin;

  FSMC_GPIO_Init();

  FSMC_Bank1->BTCR[_BTCR] &= ((uint32_t)~(FSMC_BCR1_MBKEN     | FSMC_BCR1_MUXEN    | FSMC_BCR1_MTYP     |
                                      FSMC_BCR1_MWID      | FSMC_BCR1_FACCEN   | FSMC_BCR1_BURSTEN  |
                                      FSMC_BCR1_WAITPOL   | FSMC_BCR1_WRAPMOD  | FSMC_BCR1_WAITCFG  |
                                      FSMC_BCR1_WREN      | FSMC_BCR1_WAITEN   | FSMC_BCR1_EXTMOD   |
                                      FSMC_BCR1_ASYNCWAIT | FSMC_BCR1_CPSIZE   | FSMC_BCR1_CBURSTRW));
  FSMC_Bank1->BTCR[_BTCR] |= 0x1090;

  FSMC_Bank1->BTCR[_BTCR+1] = ((uint32_t)~(FSMC_BTR1_ADDSET  | FSMC_BTR1_ADDHLD | FSMC_BTR1_DATAST |
                                     FSMC_BTR1_BUSTURN | FSMC_BTR1_CLKDIV | FSMC_BTR1_DATLAT |
                                     FSMC_BTR1_ACCMOD));
  FSMC_Bank1->BTCR[_BTCR+1] = _AddressSetupTime |
                        (_AddressHoldTime << 4U) |
                        (_DataSetupTime << 8U) |
                        (_BusTurnAroundDuration << 16U) |
                        ((_CLKDivision - 1U) << 20U) |
                        ((_DataLatency - 2U) << 24U) |
                        _AccessMode;

  FSMC_Bank1E->BWTR[_BWTR] = 0xFFFFFFF;
  FSMC_Bank1->BTCR[_BTCR] |= FSMC_BCR1_MBKEN;
  _setBaseAddress(_TFT_Base, _TFT_Ax);
  Init();
}

void ILI9341_FSMC16::FSMC_GPIO_Init()
{
  uint16_t Temp = 0;

  if (_FSMC_Initialized) {return;}
  _FSMC_Initialized = 1;

  // Разрешаем тактирование на порты и FSMC
  RCC->AHB1ENR |= RCC_AHB1ENR_GPIODEN | RCC_AHB1ENR_GPIOEEN;
  RCC->AHB3ENR |= RCC_AHB3ENR_FSMCEN;

  _SetSpeed(Speed_WeryHigh);
  // FSMC GPIO Конфигурация
//  PE7   ------> FSMC_D4
//  PE8   ------> FSMC_D5
//  PE9   ------> FSMC_D6
//  PE10  ------> FSMC_D7
//  PE11  ------> FSMC_D8
//  PE12  ------> FSMC_D9
//  PE13  ------> FSMC_D10
//  PE14  ------> FSMC_D11
//  PE15  ------> FSMC_D12
//  PD0   ------> FSMC_D2
//  PD1   ------> FSMC_D3
//  PD4   ------> FSMC_NOE                                                        // RD - чтение. Активный 0
//  PD5   ------> FSMC_NWE                                                        // WR - запись. Активный 0
//  PD7   ------> FSMC_NE1                                                        // CS - выбор. Активный 0
//  PG9   ------> FSMC_NE2                                                        // CS - выбор. Активный 0
//  PG10  ------> FSMC_NE3                                                        // CS - выбор. Активный 0
//  PG12  ------> FSMC_NE4                                                        // CS - выбор. Активный 0
//  PD8   ------> FSMC_D13
//  PD9   ------> FSMC_D14
//  PD10  ------> FSMC_D15
//  PF0   ------> FSMC_A0                                                         // RS - выбор данные/команда
//  PF1   ------> FSMC_A1                                                         // RS - выбор данные/команда
//  PF2   ------> FSMC_A2                                                         // RS - выбор данные/команда
//  PF3   ------> FSMC_A3                                                         // RS - выбор данные/команда
//  PF4   ------> FSMC_A4                                                         // RS - выбор данные/команда
//  PF5   ------> FSMC_A5                                                         // RS - выбор данные/команда
//  PF12  ------> FSMC_A6                                                         // RS - выбор данные/команда
//  PF13  ------> FSMC_A7                                                         // RS - выбор данные/команда
//  PF14  ------> FSMC_A8                                                         // RS - выбор данные/команда
//  PF15  ------> FSMC_A9                                                         // RS - выбор данные/команда
//  PG0   ------> FSMC_A10                                                        // RS - выбор данные/команда
//  PG1   ------> FSMC_A11                                                        // RS - выбор данные/команда
//  PG2   ------> FSMC_A12                                                        // RS - выбор данные/команда
//  PG3   ------> FSMC_A13                                                        // RS - выбор данные/команда
//  PG4   ------> FSMC_A14                                                        // RS - выбор данные/команда
//  PG5   ------> FSMC_A15                                                        // RS - выбор данные/команда
//  PD11  ------> FSMC_A16                                                        // RS - выбор данные/команда
//  PD12  ------> FSMC_A17                                                        // RS - выбор данные/команда
//  PD13  ------> FSMC_A18                                                        // RS - выбор данные/команда
//  PD14  ------> FSMC_D0
//  PD15  ------> FSMC_D1

  Temp = GPIO_PIN_7 | GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10 |
         GPIO_PIN_11 | GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15;
  _SetPinAlternate(GPIOE, Temp, GPIO_AF12_FSMC);

  Temp = 0;
  Temp = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_4 | GPIO_PIN_5 |
         GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10 |
         GPIO_PIN_14 | GPIO_PIN_15;
  _SetPinAlternate(GPIOD, Temp, GPIO_AF12_FSMC);

  switch (_FSMC_NEx)
  {
    case FSMC_NE1 :
      _SetPinAlternate(GPIOD, GPIO_PIN_7, GPIO_AF12_FSMC);
      _BTCR = 0;
      _BWTR = 0;
      _TFT_Base = 0x60000000;
      break;
    case FSMC_NE2 :
      _SetPinAlternate(GPIOG, GPIO_PIN_9, GPIO_AF12_FSMC);
      _BTCR = 2;
      _BWTR = 1;
      _TFT_Base = 0x64000000;
      break;
    case FSMC_NE3 :
      _SetPinAlternate(GPIOG, GPIO_PIN_10, GPIO_AF12_FSMC);
      _BTCR = 4;
      _BWTR = 2;
      _TFT_Base = 0x68000000;
      break;
    case FSMC_NE4 :
      _SetPinAlternate(GPIOG, GPIO_PIN_12, GPIO_AF12_FSMC);
      _BTCR = 6;
      _BWTR = 3;
      _TFT_Base = 0x6C000000;
      break;
    default:
      break;
  }

  switch (_FSMC_Ax)
  {
    case FSMC_A0:

      break;
    case FSMC_A1:

      break;
    case FSMC_A2:

      break;
    case FSMC_A3:

      break;
    case FSMC_A4:

      break;
    case FSMC_A5:

      break;
    case FSMC_A6:
      _SetPinAlternate(GPIOF, GPIO_PIN_12, GPIO_AF12_FSMC);
      _TFT_Ax   = 0x0000007E;
      break;
    case FSMC_A7:

      break;
    case FSMC_A8:

      break;
    case FSMC_A9:

      break;
    case FSMC_A10:
      _SetPinAlternate(GPIOG, GPIO_PIN_0, GPIO_AF12_FSMC);
      _TFT_Ax   = 0x000007FE;
      break;
    case FSMC_A11:

      break;
    case FSMC_A12:

      break;
    case FSMC_A13:

      break;
    case FSMC_A14:

      break;
    case FSMC_A15:

      break;
    case FSMC_A16:

      break;
    case FSMC_A17:

      break;
    case FSMC_A18:
      _SetPinAlternate(GPIOD, GPIO_PIN_13, GPIO_AF12_FSMC);
      _TFT_Ax   = 0x0007FFFE;
      break;
    case FSMC_A19:

      break;
    case FSMC_A20:

      break;
    case FSMC_A21:

      break;
    case FSMC_A22:

      break;
    case FSMC_A23:

      break;
    default:
      break;
  }
  _SetSpeed(Speed_Low);
}

void ILI9341_FSMC16::_setBaseAddress(uint32_t Base, uint32_t Data)
{
  LCD = ((LCD_TypeDef*) ((uint32_t) (Base | Data)));
}

void ILI9341_FSMC16::Init()
{
  if(Flag_Reset)
  {
    if(Direction_Reset) _GPIO_Reset->BSRR |= _GPIO_Pin_Reset;
    else _GPIO_Reset->BSRR |= (_GPIO_Pin_Reset << 16);

    delay(10);

    if(Direction_Reset) _GPIO_Reset->BSRR |= (_GPIO_Pin_Reset << 16);
    else _GPIO_Reset->BSRR |= _GPIO_Pin_Reset;
  }


  delay(130);
      WriteReg(0x0000, 0x0001);

      WR_REG(0xCB);      // Power Control A
      WR_DATA(0x39);
      WR_DATA(0x2C);
      WR_DATA(0x00);
      WR_DATA(0x34);
      WR_DATA(0x02);

      WR_REG(0xCF);      // Power Control B
      WR_DATA(0x00);
      WR_DATA(0xC1);
      WR_DATA(0X30);

      WR_REG(0xE8);      // Driver Timing Control A
      WR_DATA(0x85);
      WR_DATA(0x00);
      WR_DATA(0x78);

      WR_REG(0xEA);      // Driver Timing Control B
      WR_DATA(0x00);
      WR_DATA(0x00);

      WR_REG(0xED);      // Power on Sequence
      WR_DATA(0x64);
      WR_DATA(0x03);
      WR_DATA(0X12);
      WR_DATA(0X81);

      WR_REG(0xF7);      // Pump Ratio
      WR_DATA(0x20);

      WR_REG(0xC0);      //?   //Power control
      WR_DATA(0x23);     //    VRH[5:0]

      WR_REG(0xC1);      //?    Power control
      WR_DATA(0x10);     //    SAP[2:0];BT[3:0]

      WR_REG(0xC5);      //?  VCM control
      WR_DATA(0x3E);     //3F
      WR_DATA(0x28);     //3C

      WR_REG(0xC7);      //?  VCM control2
      WR_DATA(0X86);

      WR_REG(0x36);    // Memory Access Control
      WR_DATA(0x48);

      WR_REG(0x3A);    // Pixel Format
      WR_DATA(0x55);

      WR_REG(0xB1);    // Frame Ratio Control, Standard RGB Color
      WR_DATA(0x00);
      WR_DATA(0x18);

      WR_REG(0xB6);    //? Display Function Control
      WR_DATA(0x08);   //?
      WR_DATA(0x82);   //?
      WR_DATA(0x27);   //?

      WR_REG(0xF2);    // 3Gamma Function Disable
      WR_DATA(0x00);

      WR_REG(0x26);    //  Gamma curve selected
      WR_DATA(0x01);

      WR_REG(0xE0);    //?   Positive Gamma Correction
      WR_DATA(0x0F);   //?
      WR_DATA(0x31);   //?
      WR_DATA(0x2B);   //?
      WR_DATA(0x0C);   //?
      WR_DATA(0x0E);   //?
      WR_DATA(0x08);   //?
      WR_DATA(0x4E);   //?
      WR_DATA(0XF1);   //?
      WR_DATA(0x37);   //?
      WR_DATA(0x07);   //?
      WR_DATA(0x10);   //?
      WR_DATA(0x03);   //?
      WR_DATA(0x0E);   //?
      WR_DATA(0x09);   //?
      WR_DATA(0x00);   //?

      WR_REG(0XE1);    //?  Negative Gamma Correction
      WR_DATA(0x00);   //?
      WR_DATA(0x0E);   //?
      WR_DATA(0x14);   //?
      WR_DATA(0x03);   //?
      WR_DATA(0x11);   //?
      WR_DATA(0x07);   //?
      WR_DATA(0x31);   //?
      WR_DATA(0xC1);   //?
      WR_DATA(0x48);   //?
      WR_DATA(0x08);   //?
      WR_DATA(0x0F);   //?
      WR_DATA(0x0C);   //?
      WR_DATA(0x31);   //?
      WR_DATA(0x36);   //?
      WR_DATA(0x0F);   //?

      WR_REG(0x11);     //Exit Sleep
      delay(120);

      WR_REG (CMD_DISPLAY_ON); //display on


fillScreen (COLOR_BLACK);

  if(Flag_Blackout)
  {
    if(Direction_Blackout) _GPIO_Blackout->BSRR |= _GPIO_Pin_Blackout;
    else _GPIO_Blackout->BSRR |= (_GPIO_Pin_Blackout << 16);
  }
}

void ILI9341_FSMC16::setWindow(uint16_t sx, uint16_t sy, uint16_t width, uint16_t height)
{
  WR_REG(CMD_SET_X);
  WR_DATA(sx>>8);
  WR_DATA(sx&0XFF);
  WR_DATA(width>>8);
  WR_DATA(width&0XFF);
  WR_REG(CMD_SET_Y);
  WR_DATA(sy>>8);
  WR_DATA(sy&0XFF);
  WR_DATA(height>>8);
  WR_DATA(height&0XFF);
}

void ILI9341_FSMC16::fillScreen(uint16_t color)
{
  uint32_t index=0;
  uint32_t totalpoint=_width;
  totalpoint *= _height;
  setWindow(0, 0, _width, _height);
  LCD->LCD_REG = CMD_RAM_WRITE;
//  LCD->LCD_RAM = CMD_RAM_WRITE;
  for(index=0;index<totalpoint;index++)
  {
    LCD->LCD_RAM=color;
//    LCD->LCD_REG=color;
  }
}

/*
 * Системные функции
 */
void ILI9341_FSMC16::setPinBlackout(GPIO_TypeDef* GPIO, uint16_t GPIO_PIN, bool Dir)
{
  _GPIO_Blackout = GPIO;
  _GPIO_Pin_Blackout = GPIO_PIN;
  Flag_Blackout = true;
  Direction_Blackout = Dir;
  _SetPin(_GPIO_Blackout, _GPIO_Pin_Blackout, Output, No_Pull);
  if(Dir) _GPIO_Blackout->BSRR |= (_GPIO_Pin_Blackout << 16); else _GPIO_Blackout->BSRR |= _GPIO_Pin_Blackout;
}

void ILI9341_FSMC16::setPinReset(GPIO_TypeDef* GPIO, uint16_t GPIO_PIN, bool Dir)
{
  _GPIO_Reset = GPIO;
  _GPIO_Pin_Reset = GPIO_PIN;
  Flag_Reset = true;
  Direction_Reset = Dir;
  _SetPin(_GPIO_Reset, _GPIO_Pin_Reset, Output, No_Pull);
  if(Dir) _GPIO_Reset->BSRR |= (_GPIO_Pin_Reset << 16); else _GPIO_Reset->BSRR |= _GPIO_Pin_Reset;
}

void WR_REG(uint16_t regval)
{
  regval=regval;
  LCD->LCD_REG=regval;
}

void WR_DATA(uint16_t data)
{
  data=data;
  LCD->LCD_RAM=data;
}

uint16_t RD_DATA(void)
{
  uint16_t ram;
  ram=LCD->LCD_RAM;
  return ram;
}

void WriteReg(uint16_t LCD_Reg, uint16_t LCD_RegValue)
{
  LCD->LCD_REG = LCD_Reg;
  LCD->LCD_RAM = LCD_RegValue;
}

uint16_t ReadReg(uint16_t LCD_Reg)
{
  WR_REG(LCD_Reg);
  delay(5);
  return RD_DATA();
}

void ILI9341_FSMC16::setRotation(uint8_t x)
{
  WR_REG(MADCTL);
  rotation = x % 8;
  switch (rotation)
  {
    case 0: // Portrait
      WR_DATA(MAD_BGR | MAD_MX);
      _width = WIDTH;
      _height = HEIGHT;
      break;
    case 1: // Landscape (Portrait + 90)
      WR_DATA(MAD_BGR | MAD_MV);
      _width = HEIGHT;
      _height = WIDTH;
      break;
    case 2: // Inverter portrait
      WR_DATA(MAD_BGR | MAD_MY);
      _width = WIDTH;
      _height = HEIGHT;
      break;
    case 3: // Inverted landscape
      WR_DATA(MAD_BGR | MAD_MV | MAD_MX | MAD_MY);
      _width = HEIGHT;
      _height = WIDTH;
      break;
    case 4: // Portrait
      WR_DATA(MAD_BGR | MAD_MX | MAD_MY);
      _width = WIDTH;
      _height = HEIGHT;
      break;
    case 5: // Landscape (Portrait + 90)
      WR_DATA(MAD_BGR | MAD_MV | MAD_MX);
      _width = HEIGHT;
      _height = WIDTH;
      break;
    case 6: // Inverter portrait
      WR_DATA(MAD_BGR);
      _width = WIDTH;
      _height = HEIGHT;
      break;
    case 7: // Inverted landscape
      WR_DATA(MAD_BGR | MAD_MV | MAD_MY);
      _width = HEIGHT;
      _height = WIDTH;
      break;
  }
}

void ILI9341_FSMC16::CursorXY(uint16_t x, uint16_t y)
{
  WR_REG(CMD_SET_X);
  WR_DATA(x>>8);
  WR_DATA(x&0XFF);
  WR_DATA(x>>8);
  WR_DATA(x&0XFF);
  WR_REG(CMD_SET_Y);
  WR_DATA(y>>8);
  WR_DATA(y&0XFF);
  WR_DATA(y>>8);
  WR_DATA(y&0XFF);
}

void ILI9341_FSMC16::drawPixel(uint16_t x, uint16_t y, uint16_t color)
{
  if((x<0)||(y<0)||(x>=_width)||(y>=_height)) return;

  CursorXY(x, y);
  LCD->LCD_REG = CMD_RAM_WRITE;
  LCD->LCD_RAM = color;
}

/*
 * Быстрый вывод графики
 */
void ILI9341_FSMC16::drawRGBBitmap(uint16_t x, uint16_t y, const uint16_t *bitmap, uint16_t w, uint16_t h)
{
  uint32_t index = 0;
  uint32_t totalpoint;

  totalpoint = w*h;
  setWindow(x, y, x+w, y+h);
  LCD->LCD_REG = CMD_RAM_WRITE;
  for(index = 0; index < totalpoint; index++)
  {
    LCD->LCD_RAM = *bitmap;
    bitmap++;
  }
}

void ILI9341_FSMC16::drawFillFastRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color)
{
  uint32_t index = 0;
  uint32_t totalpoint = w;
  totalpoint *= h;

  setWindow(x, y, x+w-1, y+h-1);
  LCD->LCD_REG = CMD_RAM_WRITE;

  for (index = 0; index < totalpoint; index++)
  {
    LCD->LCD_RAM=color;
  }
}
