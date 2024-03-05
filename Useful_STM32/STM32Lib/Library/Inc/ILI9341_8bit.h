/*
 * ILI9341_8bit.h
 *
 *  Created on: 17 нояб. 2021 г.
 *      Author: User
 *      240x320
 *
 *  Подключение
 *  _G
 */

#ifndef INC_ILI9341_8BIT_H_
#define INC_ILI9341_8BIT_H_

#include <stdint.h> 
#include <STM32.h>
#include "GFX_TFT.h"

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

enum TFT_Half
{
    FHW = false,                                            // Младший байт
    SHW = true                                              // Старший байт
};

class ILI9341 : public GFX_TFT
{
  public:
    ILI9341();
    void Init(GPIO_TypeDef *GPIOx_Half, bool HalfWords,       // Порт данных, пины данных FHW - 0-7, SHW - 8-15
        GPIO_TypeDef  *GPIOx_Reset,                     // Порт сброса
        uint16_t      TFT_Reset,                        // Пин сброса
        GPIO_TypeDef  *GPIOx_CS,                        // Порт выбора кристалла
        uint16_t      TFT_CS,                           // Пин выбора кристалла
        GPIO_TypeDef  *GPIOx_RS,                        // Порт выбора Регистра/Данных
        uint16_t      TFT_RS,                           // Пин выбора Регистра/Данных (Register/DataSelection)
        GPIO_TypeDef  *GPIOx_WR,                        // Порт строба записи
        uint16_t      TFT_WR,                           // Пин строба записи
        GPIO_TypeDef  *GPIOx_RD,                        // Порт строба чтения
        uint16_t      TFT_RD);                            // Пин строба чтения

    virtual void fillScreen(uint16_t Color);
    void setWindow(uint16_t sx,uint16_t sy,uint16_t width,uint16_t height);

    virtual void  drawPixel(uint16_t x, uint16_t y, uint16_t color);
    virtual void  setRotation(uint8_t x);

    void drawRGBBitmap(uint16_t x, uint16_t y, const uint16_t *bitmap, uint16_t w, uint16_t h);

  private:
    GPIO_TypeDef  *_GPIOx;
    uint16_t      _GPIO_Pin;
    GPIO_TypeDef  *_GPIOx_Half;                             // Порт 8ми разрядного интерфейса, выводы идут подряд.
    bool          _TFT_Pin_Half;                            // Выводы 8ми разрядного интерфейса. Разница 1я половина, 2я половина.
                                                            // В переменной _HalfWords

    GPIO_TypeDef  *_GPIOx_Reset;                            // Порт сброса
    uint16_t      _TFT_Reset;                               // Пин сброса
    GPIO_TypeDef  *_GPIOx_CS;                               // Порт выбора кристалла
    uint16_t      _TFT_CS;                                  // Пин выбора кристалла
    GPIO_TypeDef  *_GPIOx_RS;                               // Порт выбора Регистра/Данных
    uint16_t      _TFT_RS;                                  // Пин выбора Регистра/Данных (Register/DataSelection)
    GPIO_TypeDef  *_GPIOx_WR;                               // Порт строба записи
    uint16_t      _TFT_WR;                                  // Пин строба записи
    GPIO_TypeDef  *_GPIOx_RD;                               // Порт строба чтения
    uint16_t      _TFT_RD;                                  // Пин строба чтения
    virtual void CursorXY(uint16_t x, uint16_t y);

    void WR_REG(uint8_t regval);
    void WR_DATA(uint8_t data);
//    void WriteReg(uint16_t LCD_Reg, uint16_t LCD_RegValue);
    uint8_t RD_DATA(void);

  protected:
};

#endif /* INC_ILI9341_8BIT_H_ */
