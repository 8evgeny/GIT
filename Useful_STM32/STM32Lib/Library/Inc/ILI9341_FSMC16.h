/*
 * FSMC32_TFT.h
 *
 *  Created on: 8 апр. 2021 г.
 *      Author: User
 */

#ifndef FSMC32_TFT_H_
#define FSMC32_TFT_H_

#include "GFX_TFT.h"
#include "STM32.h"

#define CMD_DISPLAY_ON  0x29
#define CMD_DISPLAY_OFF 0x28
#define CMD_SET_X       0x2A
#define CMD_SET_Y       0x2B
#define CMD_RAM_WRITE   0x2C

#define MADCTL  0x36
#define MAD_MY  0x80
#define MAD_MX  0x40
#define MAD_MV  0x20
#define MAD_ML  0x10
#define MAD_BGR 0x08
#define MAD_MH  0x04
#define MAD_SS  0x02
#define MAD_GS  0x01
#define MAD_RGB 0x00

#define CMD_INVOFF  0x20
#define CMD_INVON   0x21

#define FSMC_ACCESS_MODE_A 0b00;
#define FSMC_ACCESS_MODE_B 0b01;
#define FSMC_ACCESS_MODE_C 0b10;
#define FSMC_ACCESS_MODE_D 0b11;

enum FSMC_Ax
{
  FSMC_A0,
  FSMC_A1,
  FSMC_A2,
  FSMC_A3,
  FSMC_A4,
  FSMC_A5,
  FSMC_A6,
  FSMC_A7,
  FSMC_A8,
  FSMC_A9,
  FSMC_A10,
  FSMC_A11,
  FSMC_A12,
  FSMC_A13,
  FSMC_A14,
  FSMC_A15,
  FSMC_A16,
  FSMC_A17,
  FSMC_A18,
  FSMC_A19,
  FSMC_A20,
  FSMC_A21,
  FSMC_A22,
  FSMC_A23
};

enum FSMC_NE
{
  FSMC_NE1,
  FSMC_NE2,
  FSMC_NE3,
  FSMC_NE4,
};

enum Display
{
  ILI9341,
  ILI9468
};

struct LCD_TypeDef
{
  uint16_t LCD_REG;
  uint16_t LCD_RAM;
};

class ILI9341_FSMC16 : public GFX_TFT
{
  public:
    ILI9341_FSMC16();
    void FSMC_Init(uint8_t Ax_Pin, uint8_t NEx_Pin);
    void FSMC_Speed(uint8_t AddressSetupTime, uint8_t AddressHoldTime, uint8_t DataSetupTime, uint8_t BusTurnAroundDuration,
                      uint8_t CLKDivision, uint8_t DataLatency, uint8_t AccessMode);
//    void FSMC_GPIO_Speed(uint8_t Speed);
    virtual void fillScreen(uint16_t Color);
//    void    setWindow(uint16_t sx,uint16_t sy,uint16_t width,uint16_t height);
    virtual void setWindow(uint16_t sx,uint16_t sy,uint16_t width,uint16_t height);
    virtual void drawFillFastRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color);

    virtual void  drawPixel(uint16_t x, uint16_t y, uint16_t color);
    virtual void  setRotation(uint8_t x);

    void drawRGBBitmap(uint16_t x, uint16_t y, const uint16_t *bitmap, uint16_t w, uint16_t h);

    // -------------------- Служебные --------------------------------
    void setPinBlackout(GPIO_TypeDef* GPIO, uint16_t GPIO_PIN, bool Dir);
    void setPinReset(GPIO_TypeDef* GPIO, uint16_t GPIO_PIN, bool Dir);

  private:
    void FSMC_GPIO_Init(void);
    // Системные функции
    virtual void CursorXY(uint16_t x, uint16_t y);

    uint8_t   _AddressSetupTime;
    uint8_t   _AddressHoldTime;
    uint8_t   _DataSetupTime;
    uint8_t   _BusTurnAroundDuration;
    uint8_t   _CLKDivision;
    uint8_t   _DataLatency;
    uint8_t   _AccessMode;
    uint8_t   _FSMC_Initialized;
    uint8_t   _FSMC_Ax, _FSMC_NEx;
    uint32_t  _TFT_Base, _TFT_Ax;
    uint8_t   _BTCR = 0;
    uint8_t   _BWTR = 0;

    GPIO_TypeDef *_GPIO_Blackout;
    uint16_t      _GPIO_Pin_Blackout;
    bool          Flag_Blackout = 0;
    bool          Direction_Blackout;

    GPIO_TypeDef *_GPIO_Reset;
    uint16_t      _GPIO_Pin_Reset;
    bool          Flag_Reset = 0;
    bool          Direction_Reset;

//    uint8_t       _GPIO_Speed = Speed_Low;

  protected:
    void      _setBaseAddress(uint32_t Base, uint32_t Data);
    void      Init(void);

};


#endif /* DEVICE_STM32F4XX_INC_FSMC32_TFT_H_ */
