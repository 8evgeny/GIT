/*
 * ST7735_SPI.h
 *
 *  Created on: 17 июн. 2022 г.
 *      Author: User
 */

#ifndef ST7735_SPI_H_
#define ST7735_SPI_H_

#include "STM32.h"
#include "GFX_TFT.h"
#if defined(SPI_SOFT)
  #include "SPI_Soft.h"
#else
  #include "SPI.h"
#endif

#define ST_CMD_DELAY      0x80    // special signifier for command lists
#define DC_Data High
#define DC_Comm Low
/*
 * Регистры характерные для ST7735
 */
#define INITR_GREENTAB    0x00
#define INITR_REDTAB      0x01
#define INITR_BLACKTAB    0x02
#define INITR_18GREENTAB  INITR_GREENTAB
#define INITR_18REDTAB    INITR_REDTAB
#define INITR_18BLACKTAB  INITR_BLACKTAB
#define INITR_144GREENTAB 0x01
#define INITR_MINI160x80  0x04
#define INITR_HALLOWING   0x05

// Some register settings
#define ST7735_MADCTL_BGR 0x08
#define ST7735_MADCTL_MH  0x04

#define ST7735_FRMCTR1    0xB1
#define ST7735_FRMCTR2    0xB2
#define ST7735_FRMCTR3    0xB3
#define ST7735_INVCTR     0xB4
#define ST7735_DISSET5    0xB6

#define ST7735_PWCTR1     0xC0
#define ST7735_PWCTR2     0xC1
#define ST7735_PWCTR3     0xC2
#define ST7735_PWCTR4     0xC3
#define ST7735_PWCTR5     0xC4
#define ST7735_VMCTR1     0xC5

#define ST7735_PWCTR6     0xFC

#define ST7735_GMCTRP1    0xE0
#define ST7735_GMCTRN1    0xE1

/*
 * Общие регистры для линейки ST77xx
 */
#define ST77XX_NOP        0x00
#define ST77XX_SWRESET    0x01
#define ST77XX_RDDID      0x04
#define ST77XX_RDDST      0x09

#define ST77XX_SLPIN      0x10
#define ST77XX_SLPOUT     0x11
#define ST77XX_PTLON      0x12
#define ST77XX_NORON      0x13

#define ST77XX_INVOFF     0x20
#define ST77XX_INVON      0x21
#define ST77XX_DISPOFF    0x28
#define ST77XX_DISPON     0x29
#define ST77XX_CASET      0x2A
#define ST77XX_RASET      0x2B
#define ST77XX_RAMWR      0x2C
#define ST77XX_RAMRD      0x2E

#define ST77XX_PTLAR      0x30
#define ST77XX_COLMOD     0x3A
#define ST77XX_MADCTL     0x36

#define ST77XX_MADCTL_MY  0x80
#define ST77XX_MADCTL_MX  0x40
#define ST77XX_MADCTL_MV  0x20
#define ST77XX_MADCTL_ML  0x10
#define ST77XX_MADCTL_RGB 0x00

#define ST77XX_RDID1      0xDA
#define ST77XX_RDID2      0xDB
#define ST77XX_RDID3      0xDC
#define ST77XX_RDID4      0xDD

enum DisplayType
{
  Type_0,
  Type_1,
  Type_1x8_128x160,
  Type_3,
  Type_4
};

class ST7735_SPI : public GFX_TFT
{
  public:
    ST7735_SPI(SPI* SPI,
               GPIO_TypeDef* GPIO_CS, int16_t GPIO_PIN_CS,
               GPIO_TypeDef* GPIO_DC, int16_t GPIO_PIN_DC,
               GPIO_TypeDef* GPIO_Blk, int16_t GPIO_PIN_Blk,
               GPIO_TypeDef* GPIO_RST, int16_t GPIO_PIN_RST);

    ST7735_SPI(SPI* SPI,
               GPIO_TypeDef* GPIO_CS, int16_t GPIO_PIN_CS,
               GPIO_TypeDef* GPIO_DC, int16_t GPIO_PIN_DC,
               GPIO_TypeDef* GPIO_Blk, int16_t GPIO_PIN_Blk);
    void      Init(uint8_t Type);
    virtual   void drawFillFastRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color);
    virtual   void  drawPixel(uint16_t x, uint16_t y, uint16_t color);
    virtual   void fillScreen(uint16_t Color);
    void      setWindow(uint16_t sx,uint16_t sy,uint16_t width,uint16_t height);
    virtual   void setRotation(uint8_t rot);
    void      setDisplayBlk(bool setblk);
    void      setInverse(bool inv);
    uint32_t  getID(void);

  private:
    GPIO_TypeDef* _GPIO_CS;
    int16_t       _GPIO_PIN_CS;
    GPIO_TypeDef* _GPIO_DC;
    int16_t       _GPIO_PIN_DC;
    GPIO_TypeDef* _GPIO_Blk;
    int16_t       _GPIO_PIN_Blk;
    GPIO_TypeDef* _GPIO_RST = NULL;
    int16_t       _GPIO_PIN_RST;
    SPI*          _SPI;

    void    CS_Set(bool Set){_DigitalWriteBit(_GPIO_CS, _GPIO_PIN_CS, Set);}
    void    DC_Set(bool Set){_DigitalWriteBit(_GPIO_DC, _GPIO_PIN_DC, Set);}
    void    Blk_Set(bool Set){_DigitalWriteBit(_GPIO_Blk, _GPIO_PIN_Blk, Set);}
    void    RST_Set(bool Set){_DigitalWriteBit(_GPIO_RST, _GPIO_PIN_RST, Set);}
    void    displayInit(const uint8_t *addr);
    void    sendCommand(uint8_t cmd, const uint8_t *addr, uint8_t numArgs);
    virtual void CursorXY(uint16_t x, uint16_t y);
    uint8_t tabcolor;

  protected:
    uint8_t       _colstart = 0, ///< Some displays need this changed to offset
                  _rowstart = 0, ///< Some displays need this changed to offset
                  _xstart = 0,
                  _ystart = 0;
};

#endif /* ST7735_SPI_H_ */
