/*
 * 8bit TFT Library for STM32F103
 * based on MCUFRIEND_kbv.cpp by David Prentice
 * https://github.com/prenticedavid/MCUFRIEND_kbv
 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */


#ifndef _RM68140_8BIT
#define _RM68140_8BIT

#include <STM32.h>
#include <math.h>
#include "gfxfont.h"
//#include "Print.h"
//#include <Adafruit_GFX.h>

/*
If you use TFT of different resolution, change this.
*/
//#define TFTWIDTH  240
//#define TFTHEIGHT 320
//#define WIDTH  240
//#define HEIGHT 320

// Color definitions
#define BLACK       0x0000      /*   0,   0,   0 */
#define NAVY        0x000F      /*   0,   0, 128 */
#define DARKGREEN   0x03E0      /*   0, 128,   0 */
#define DARKCYAN    0x03EF      /*   0, 128, 128 */
#define MAROON      0x7800      /* 128,   0,   0 */
#define PURPLE      0x780F      /* 128,   0, 128 */
#define OLIVE       0x7BE0      /* 128, 128,   0 */
#define LIGHTGRAY   0xC618      /* 192, 192, 192 */
#define DARKGRAY    0x7BEF      /* 128, 128, 128 */
#define BLUE        0x001F      /*   0,   0, 255 */
#define GREEN       0x07E0      /*   0, 255,   0 */
#define CYAN        0x07FF      /*   0, 255, 255 */
#define RED         0xF800      /* 255,   0,   0 */
#define MAGENTA     0xF81F      /* 255,   0, 255 */
#define YELLOW      0xFFE0      /* 255, 255,   0 */
#define WHITE       0xFFFF      /* 255, 255, 255 */
#define ORANGE      0xFD20      /* 255, 165,   0 */
#define GREENYELLOW 0xAFE5      /* 173, 255,  47 */
#define PINK        0xF81F
#define GRAY        0x5AEB

/*
Define pins and Output Data Registers
*/

//#define TFT_DATA       GPIOA
//Port data |D7 |D6 |D5 |D4 |D3 |D2 |D1 |D0 |
//Pin stm32 |PA7|PA6|PA5|PA4|PA3|PA2|PA1|PA0|

#define TFT_DATA       GPIOB
//Port data |D7 |D6 |D5 |D4 |D3 |D2 |D1 |D0 |
//Pin stm32 |PB7|PB6|PB5|PB4|PB3|PB2|PB1|PB0|

//#define TFT_CNTRL      GPIOB
//Control pins |RD  |WR  |RS |CS |RST |
//Pin stm32    |PB11|PB10|PB9|PB8|PA11|
//#define TFT_RD         PB3
//#define TFT_WR         PB4
//#define TFT_RS         PB5
//#define TFT_CS         PB6
//#define TFT_RST        PB7
//#define TFT_RD_MASK    digitalPinToBitMask(TFT_RD)
//#define TFT_WR_MASK    digitalPinToBitMask(TFT_WR)
//#define TFT_RS_MASK    digitalPinToBitMask(TFT_RS)
//#define TFT_CS_MASK    digitalPinToBitMask(TFT_CS)
//#define TFT_RST_MASK   digitalPinToBitMask(TFT_RST)

//#define RD_ACTIVE    TFT_CNTRL->regs->BRR  = TFT_RD_MASK
//#define RD_IDLE      TFT_CNTRL->regs->BSRR = TFT_RD_MASK
//#define WR_ACTIVE    TFT_CNTRL->regs->BRR  = TFT_WR_MASK
//#define WR_IDLE      TFT_CNTRL->regs->BSRR = TFT_WR_MASK
//#define CD_COMMAND   TFT_CNTRL->regs->BRR  = TFT_RS_MASK
//#define CD_DATA      TFT_CNTRL->regs->BSRR = TFT_RS_MASK
//#define CS_ACTIVE    TFT_CNTRL->regs->BRR  = TFT_CS_MASK
//#define CS_IDLE      TFT_CNTRL->regs->BSRR  = TFT_CS_MASK
//#define RST_ACTIVE   TFT_CNTRL->regs->BRR  = TFT_RST_MASK
//#define RST_IDLE     TFT_CNTRL->regs->BSRR  = TFT_RST_MASK

#define DATA_Port     GPIOB
#define WR_Pin        GPIO_PIN_10
#define WR_GPIO_Port  GPIOB
#define RD_Pin        GPIO_PIN_11
#define RD_GPIO_Port  GPIOB
#define RST_Pin       GPIO_PIN_11
#define RST_GPIO_Port GPIOA
#define BK_Pin        GPIO_PIN_12
#define BK_GPIO_Port  GPIOA
#define CS_Pin        GPIO_PIN_8
#define CS_GPIO_Port  GPIOB
#define RS_Pin        GPIO_PIN_9
#define RS_GPIO_Port  GPIOB

#define RD_ACTIVE    RD_GPIO_Port->BRR   = GPIO_PIN_11
#define RD_IDLE      RD_GPIO_Port->BSRR  = GPIO_PIN_11
#define WR_ACTIVE    WR_GPIO_Port->BRR   = GPIO_PIN_10
#define WR_IDLE      WR_GPIO_Port->BSRR  = GPIO_PIN_10
#define CD_COMMAND   RS_GPIO_Port->BRR   = GPIO_PIN_9                           // Переключаем выход CD в 0. Пойдёт команда
#define CD_DATA      RS_GPIO_Port->BSRR  = GPIO_PIN_9                           // Переключаем выход CD в 1. Пойдут данные
#define CS_ACTIVE    CS_GPIO_Port->BRR   = GPIO_PIN_8
#define CS_IDLE      CS_GPIO_Port->BSRR  = GPIO_PIN_8
#define RST_ACTIVE   RST_GPIO_Port->BRR  = GPIO_PIN_11
#define RST_IDLE     RST_GPIO_Port->BSRR = GPIO_PIN_11

#ifndef RD_STROBE
#define RD_STROBE  {RD_ACTIVE; RD_IDLE;}
#endif
#define WR_STROBE { WR_ACTIVE; WR_IDLE; }
#define swap(a, b) { int16_t t = a; a = b; b = t; }

#define CMD_DISPLAY_ON  0x29
#define CMD_DISPLAY_OFF 0x28
#define CMD_SET_X       0x2A
#define CMD_SET_Y       0x2B
#define CMD_RAM_WRITE   0x2C

class RM68140_8bit // : public Adafruit_GFX {
{
 public:

  RM68140_8bit(void);
  
//  void     setResolution(int16_t width, int16_t height);
//  void     setOffset(int16_t offset);
  void    init(void);
  void     setAddrWindow(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1);
  void      drawPixel(int16_t x, int16_t y, uint16_t color);
  void    fillScreen(uint16_t color);
  //  void     drawLine(int16_t x0, int16_t y0,int16_t x1, int16_t y1, uint16_t color);
//  void     drawPixel(int16_t x, int16_t y, uint16_t color);
//  void     pushColors8(uint8_t * block, int16_t n, bool first);
//  void     pushColors(uint16_t * block, int16_t n, bool first);
//  void     drawFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color);
//  void     drawFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color);
//  void     fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color);
//  uint16_t color565(uint8_t r, uint8_t g, uint8_t b);
//  int16_t  readGRAM(int16_t x, int16_t y, uint16_t * block, int16_t w, int16_t h);
//  uint16_t readPixel(int16_t x, int16_t y) { uint16_t color; readGRAM(x, y, &color, 1, 1); return color; }
//  void     setRotation(uint8_t r);
//  void     vertScroll(int16_t top, int16_t scrollines, int16_t offset);
//  void     invertDisplay(boolean i);

  /* These are not for current use, 8-bit protocol only! */
  //uint8_t  readdata(void),
//  uint8_t  readcommand8(uint8_t reg);
//  uint16_t read16bits(void);
//  uint16_t readReg16(uint16_t reg);
//  uint16_t readReg16Index(uint16_t reg, int8_t index);
//  uint32_t readReg32(uint16_t reg);
//  uint32_t readReg40(uint16_t reg);
//  uint16_t readID(void);


 private:
  void WR_REG(uint8_t Reg);
  void WR_DATA(uint8_t Data);
  //uint8_t  tabcolor;
//  uint8_t  read8(void);
  void     setReadDataBus(void);
  void     setWriteDataBus(void);
//  void     write8(uint8_t);
//  void     writeCmdByte(uint8_t c);
//  void     writeCmdWord(uint16_t c);
//  void     WriteCmd(uint16_t c);
//  void     writeDataByte(uint8_t d);
//  void     writeDataWord(uint16_t d);
//  void     WriteCmdData(uint16_t cmd, uint16_t dat);
//  void     WriteCmdParamN(uint8_t cmd, int8_t N, uint8_t * block);
//  void     WriteCmdParam4(uint8_t cmd, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4);
//  void     init_table(const void *table, int16_t size);
//  void     init_table16(const void *table, int16_t size);
//  uint16_t _lcd_xor, _lcd_capable;
//  uint16_t _lcd_ID, _lcd_rev, _lcd_madctl, _lcd_drivOut, _MC, _MP, _MW, _SC, _EC, _SP, _EP;
//  int16_t  _lcd_width = 0, _lcd_height = 0, _lcd_offset=0;
//  uint16_t WIDTH = 240, HEIGHT = 320;
//  uint16_t _height, _width, height, width, rotation;
  uint16_t
    _id;
  int16_t
    WIDTH,          ///< Физическая ширина дисплея в пикселях - константа
    HEIGHT;         ///< Физическая высота дисплея в пикселях - константа
  int16_t
    _width,         ///< Ширина дисплея. Модифицируется при повороте.
    _height,        ///< Высота дисплея. Модифицируется при повороте.
    cursor_x,       ///< Координата X. Начало вывода текста функцией print()
    cursor_y;       ///< Координата Y. Начало вывода текста функцией print()
  uint16_t
    textcolor,      ///< 16-bit цвет фона для функции print()
    textbgcolor;    ///< 16-bit цвет текста для функции print()
  uint8_t
    textsize_x,     ///< Desired magnification in X-axis of text to print()
    textsize_y,     ///< Desired magnification in Y-axis of text to print()
    rotation;       ///< Display rotation (0 thru 3)
  bool
    wrap,           ///< If set, 'wrap' text at right edge of display
    _cp437;         ///< If set, use correct CP437 charset (default is off)
GFXfont
  *gfxFont;         ///< Pointer to special font

};

#endif //endif of the header file
