/*
 * HX8347_8bit.h
 *
 *  Created on: 17 нояб. 2021 г.
 *      Author: User
 *      240x320
 *
 *  Подключение
 *  _G
 */

#ifndef INC_HX8347_8BIT_H_
#define INC_HX8347_8BIT_H_

#include <stdint.h> 
#include <STM32.h>
#include "GFX_TFT.h"

// Регитстры часто используемых команд
#define CMD_DISPLAY_ON  0x29
#define CMD_DISPLAY_OFF 0x28
#define CMD_SET_X       0x2A
#define CMD_SET_Y       0x2B
#define CMD_RAM_WRITE   0x2C

// Часто используемые команды
#define MADCTL  0x16
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


class HX8347 : public GFX_TFT
{
  public:
    HX8347();
    void Init(GPIO_TypeDef *GPIOx_Data, uint16_t Data_Pin,           // Порт данных, младший пин
        GPIO_TypeDef  *GPIOx_Reset,                     // Порт сброса
        uint16_t      TFT_Reset,                        // Пин сброса
        GPIO_TypeDef  *GPIOx_CS,                        // Порт выбора кристалла
        uint16_t      TFT_CS,                           // Пин выбора кристалла
        GPIO_TypeDef  *GPIOx_RS,                        // Порт выбора Регистра/Данных
        uint16_t      TFT_RS,                           // Пин выбора Регистра/Данных (Register/DataSelection)
        GPIO_TypeDef  *GPIOx_WR,                        // Порт строба записи
        uint16_t      TFT_WR,                           // Пин строба записи
        GPIO_TypeDef  *GPIOx_RD,                        // Порт строба чтения
        uint16_t      TFT_RD);                          // Пин строба чтения

    virtual void fillScreen(uint16_t Color);
    void setWindow(uint16_t sx,uint16_t sy,uint16_t width,uint16_t height);

    virtual void  drawPixel(uint16_t x, uint16_t y, uint16_t color);
    virtual void  setRotation(uint8_t x);

    void drawRGBBitmap(uint16_t x, uint16_t y, const uint16_t *bitmap, uint16_t w, uint16_t h);
    uint8_t GetID(void);

  private:
    uint8_t         _FirstPin;
    uint16_t        _PinMask;
    GPIO_TypeDef  	*_GPIOx_Data;                             // Порт 8ми разрядного интерфейса, выводы идут подряд.
    uint16_t        _Data_Pin;

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

    void    WR_REG(uint8_t regval);                         // Запись в регистр
    void    WR_DATA(uint8_t data);                          // Запись данных в регистр указанный предыдущей командой
    uint8_t RD_REG(uint8_t Reg);                            // Чтение регистра
    uint8_t RD_DATA(void);                                  // Чтение данных
    void    writeReg(uint8_t Reg, uint8_t Data);            // Запись в нужный регистр нужных данных
  protected:
};

#endif /* INC_HX8347_8BIT_H_ */
