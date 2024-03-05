/*
 * GFX_TFT.h
 *
 *  Created on: 8 апр. 2021 г.
 *      Author: User
 */

#ifndef GFX_TFT_H_
#define GFX_TFT_H_

#include "STM32.h"
#include "gfxfont.h"
#include "math.h"
#include "Print.h"
#include "Colors.h"

//#define DEC 10
//#define HEX 16
//#define OCT 8
//#define BIN 2


class GFX_TFT : public Print
{
  public:
    GFX_TFT(int16_t w, int16_t h);
    virtual void drawFillFastRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color) = 0;
    virtual void drawPixel(uint16_t x, uint16_t y, uint16_t color) = 0;
    virtual void writePixel(uint16_t x, uint16_t y, uint16_t color);
    virtual void setRotation(uint8_t x);
    virtual void fillScreen(uint16_t Color);
    virtual void setWindow(uint16_t sx,uint16_t sy,uint16_t width,uint16_t height);
    void setCursor(uint16_t Xpos, uint16_t Ypos);                   // Установка курсора с изменением переменных для print()
    virtual void CursorXY(uint16_t x, uint16_t y);                          // Установка курсора без изменения переменных для print()

    // Настройка текста
    void setTextSize(uint8_t s);
    void setTextSize(uint8_t sx, uint8_t sy);
//    void setFont(const GFXfont *f = NULL);
    void setFont(const GFXfont *f = NULL);
    void cp437(bool x=true) { _cp437 = x; }
    void setTextColor(uint16_t c) { textcolor = textbgcolor = c; }
    void setTextColor(uint16_t c, uint16_t bg) {textcolor   = c; textbgcolor = bg;}
    void setTextWrap(bool w) { wrap = w; }

    // Функции возвращающие текущие установки дисплея
    int16_t   getWidth(void) const { return _width; }
    int16_t   getHeight(void) const { return _height; }
    uint8_t   getRotation(void) const { return rotation; }
    int16_t   getCursorX(void) const { return cursor_x; }
    int16_t   getCursorY(void) const { return cursor_y; }
//    int16_t   getID(void) const {return _id;}
    uint16_t  getColorFont(void) {return textcolor;}
    uint16_t  getTextColorBG(void) {return textbgcolor;}
    uint16_t  getColorBG(void) {return bgcolor;}
    uint8_t   getTextWidth(void) {return textsize_x;}
    uint8_t   getTextHeight(void) {return textsize_y;}

    // Графические функции
    void drawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color);
    void drawFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color);
    void drawFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color);
    void drawRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color);
    void drawCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color);
    void drawCircleHelper(int16_t x0, int16_t y0, int16_t r, uint8_t cornername, uint16_t color);
    void fillCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color);
    void fillCircleHelper(int16_t x0, int16_t y0, int16_t r, uint8_t cornername, int16_t delta, uint16_t color);
    void drawTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color);
    void fillTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color);
    void drawRoundRect(int16_t x0, int16_t y0, int16_t w, int16_t h, int16_t radius, uint16_t color);
    void fillRoundRect(int16_t x0, int16_t y0, int16_t w, int16_t h, int16_t radius, uint16_t color);
    void fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color);
    uint16_t get16RGB(uint8_t R, uint8_t G, uint8_t B){ return (((R&248)|(G>>5)) << 8) | ((G&28) << 3 | B >> 3);};

    // Вывод графики
//    void drawRGBBitmap(uint16_t x, uint16_t y, const uint16_t *bitmap, uint16_t w, uint16_t h);
    void drawRGBBitmap(int16_t x, int16_t y, const uint16_t bitmap[], int16_t w, int16_t h);
    void drawRGBBitmap(int16_t x, int16_t y, uint16_t *bitmap, int16_t w, int16_t h);
    void drawRGBBitmap(int16_t x, int16_t y, const uint16_t bitmap[], const uint8_t mask[], int16_t w, int16_t h);
    void drawRGBBitmap(int16_t x, int16_t y, uint16_t *bitmap, uint8_t *mask, int16_t w, int16_t h);


    // Страшная функция
    void drawChar(int16_t x, int16_t y, unsigned char c, uint16_t color, uint16_t bg, uint8_t size);
    void drawChar(int16_t x, int16_t y, unsigned char c, uint16_t color, uint16_t bg, uint8_t size_x, uint8_t size_y);

//    int16_t
//      _width,         ///< Ширина дисплея. Модифицируется при повороте.
//      _height,        ///< Высота дисплея. Модифицируется при повороте.
//      WIDTH,          ///< Физическая ширина дисплея в пикселях - константа
//      HEIGHT;         ///< Физическая высота дисплея в пикселях - константа
//    uint8_t
//     rotation;

  private:
    uint16_t
      _id;
    int16_t
      cursor_x,       ///< Координата X. Начало вывода текста функцией print()
      cursor_y;       ///< Координата Y. Начало вывода текста функцией print()
    uint16_t
      textcolor,      ///< 16-bit цвет фона для функции print()
      textbgcolor,    ///< 16-bit цвет текста для функции print()
      bgcolor;
    uint8_t
      textsize_x,     ///< Desired magnification in X-axis of text to print()
      textsize_y;     ///< Desired magnification in Y-axis of text to print()
//      rotation;       ///< Display rotation (0 thru 3)
    bool
      wrap,           ///< If set, 'wrap' text at right edge of display
      _cp437;         ///< If set, use correct CP437 charset (default is off)
    GFXfont
      *gfxFont;         ///< Pointer to special font

  protected:
    int16_t
      _width,         ///< Ширина дисплея. Модифицируется при повороте.
      _height,        ///< Высота дисплея. Модифицируется при повороте.
      WIDTH,          ///< Физическая ширина дисплея в пикселях - константа
      HEIGHT;         ///< Физическая высота дисплея в пикселях - константа
    uint8_t
      rotation;
    // Функции из ардуины
      virtual size_t write(uint8_t);
      inline  size_t write(unsigned long n) { return write((uint8_t)n); }
      inline  size_t write(long n) { return write((uint8_t)n); }
      inline  size_t write(unsigned int n) { return write((uint8_t)n); }
      inline  size_t write(int n) { return write((uint8_t)n); }
      using   Print::write; // используется для write(str) и write(buf, size) как Print
};

#endif /* INC_GFX_TFT_H_ */
