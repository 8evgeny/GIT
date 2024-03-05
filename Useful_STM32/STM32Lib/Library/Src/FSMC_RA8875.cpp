/*
 * FSMC_RA8855.cpp
 *
 *  Created on: 20 дек. 2021 г.
 *      Author: User
 */

#include "FSMC_RA8875.h"
#include "glcdfont.c"

#ifndef _swap_int16_t
  #define _swap_int16_t(a, b) { int16_t t = a; a = b; b = t; }
#endif

#ifndef pgm_read_byte
 #define pgm_read_byte(addr) (*(const unsigned char *)(addr))
#endif

#ifndef pgm_read_word
 #define pgm_read_word(addr) (*(const unsigned short *)(addr))
#endif

#ifndef pgm_read_dword
 #define pgm_read_dword(addr) (*(const unsigned long *)(addr))
#endif

inline GFXglyph * pgm_read_glyph_ptr(const GFXfont *gfxFont, uint8_t c)
{
  return gfxFont->glyph + c;
}

inline uint8_t * pgm_read_bitmap_ptr(const GFXfont *gfxFont)
{
  return gfxFont->bitmap;
}


struct RA8875_TypeDef *LCD_RA8875;
#define swapvals(a, b) { typeof(a) t = a; a = b; b = t; }

// Конструктор, инициализация системных переменных
FSMC_RA8875::FSMC_RA8875(uint8_t Ax_Pin, uint8_t NEx_Pin, GPIO_TypeDef* ResetPort, uint16_t ResetPin,
    GPIO_TypeDef* BlackOutPort, uint16_t BlackOutPin)
{
  _AddressSetupTime       = 0;//15;
  _AddressHoldTime        = 5;//5;//15;
  _DataSetupTime          = 10;//0;//255;
  _BusTurnAroundDuration  = 5;//0;//15;
  _CLKDivision            = 0;//0;//16;
  _DataLatency            = 0;//0;//17;
  _AccessMode             = FSMC_ACCESS_MODE_A;

  _FSMC_Initialized       = 0;
  _FSMC_Ax                = Ax_Pin;
  _FSMC_NEx               = NEx_Pin;

  _GPIO_Reset             = ResetPort;
  _GPIO_Pin_Reset         = ResetPin;

  _GPIO_Blackout          = BlackOutPort;
  _GPIO_Pin_Blackout      = BlackOutPin;
}

/***********************************************************
 * Графика
 ***********************************************************/
// Заполнение цветом текущего окна
void FSMC_RA8875::fillWindow(uint16_t color)
{
  _line_addressing(0, 0, WIDTH-1, HEIGHT-1);
  setForegroundColor(color);
  WR_REG(RA8875_DCR);
  WR_DATA(0xB0);
  WaitPoll(RA8875_DCR, RA8875_DCR_LINESQUTRI_STATUS);
}

// Заполнение цветом всего дисплея
void FSMC_RA8875::clearScreen(uint16_t color)//0.69b24
{
  setActiveWindow();
  fillWindow(color);
}


void FSMC_RA8875::drawPixel(int16_t x, int16_t y, uint16_t color)
{
  //setXY(x,y);
//  if (_textMode) _setTextMode(false);//we are in text mode?
  setXY(x,y);
  WR_REG(RA8875_MRWC);
//  if (_color_bpp > 8)
//  {
    WR_DATA(color >> 8);
    WR_DATA(color &0xFF);
    //writeData16(color);
//  } else {//TOTEST:layer bug workaround for 8bit color!
//    WR_DATA(_color16To8bpp(color));
//  }
}

void FSMC_RA8875::drawFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color)
{
  if (h < 1) h = 1;
  h < 2 ? drawPixel(x,y,color) : drawLine(x, y, x, (y+h)-1, color);
}

void FSMC_RA8875::drawFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color)
{
  if (w < 1) w = 1;
  w < 2 ? drawPixel(x,y,color) : drawLine(x, y, (w+x)-1, y, color);
}


// Линия
void FSMC_RA8875::drawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color)
{
  if (_portrait) { swapvals(x0,y0); swapvals(x1,y1);}
//  if (_textMode) _setTextMode(false);//we are in text mode?
  if (color != _foreColor) setForegroundColor(color);//0.69b30 avoid 3 useless SPI calls

  _line_addressing(x0,y0,x1,y1);

  WriteReg(RA8875_DCR,0x80);
  WaitPoll(RA8875_DCR, RA8875_DCR_LINESQUTRI_STATUS);
}

// Прямоугольники
void FSMC_RA8875::drawRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color)
{
  //RA8875 не любит такого
  if (w < 2 || h < 2) return;//it cannot be!
//  if (w < 1 || h < 1) return;//it cannot be!
//  if (w < 2 && h < 2){ //render as pixel
//    drawPixel(x,y,color);
//  } else {       //render as rect
    _rect_helper(x,y,(w+x)-1,(h+y)-1,color,false);//thanks the experimentalist
//  }
}

void FSMC_RA8875::fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color)
{
  //RA8875 не любит такого
  if (w < 2 || h < 2) return;//it cannot be!
//  if (w < 1 || h < 1) return;//it cannot be!
//  if (w < 2 && h < 2){ //render as pixel
//    drawPixel(x,y,color);
//  } else {       //render as rect
    _rect_helper(x,y,(x+w)-1,(y+h)-1,color,true);//thanks the experimentalist
//  }
}

// Окружности
void FSMC_RA8875::drawCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color)
{
  _circle_helper(x0, y0, r, color, false);
}

void FSMC_RA8875::fillCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color)
{
  _circle_helper(x0, y0, r, color, true);
}

// Рисование треугольников
void FSMC_RA8875::drawTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color)
{
  _triangle_helper(x0, y0, x1, y1, x2, y2, color, false);
}

void FSMC_RA8875::fillTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color)
{
  _triangle_helper(x0, y0, x1, y1, x2, y2, color, true);
}

// Элипсы
void FSMC_RA8875::drawEllipse(int16_t xCenter, int16_t yCenter, int16_t longAxis, int16_t shortAxis, uint16_t color)
{
  _ellipseCurve_helper(xCenter, yCenter, longAxis, shortAxis, 255, color, false);
}

void FSMC_RA8875::fillEllipse(int16_t xCenter, int16_t yCenter, int16_t longAxis, int16_t shortAxis, uint16_t color)
{
  _ellipseCurve_helper(xCenter, yCenter, longAxis, shortAxis, 255, color, true);
}

// Сектора элипсов
void FSMC_RA8875::drawCurve(int16_t xCenter, int16_t yCenter, int16_t longAxis, int16_t shortAxis, uint8_t curvePart, uint16_t color)
{
  curvePart = curvePart % 4; //limit to the range 0-3
  if (_portrait) {//fix a problem with rotation
    if (curvePart == 0) {
      curvePart = 2;
    } else if (curvePart == 2) {
      curvePart = 0;
    }
  }
  _ellipseCurve_helper(xCenter, yCenter, longAxis, shortAxis, curvePart, color, false);
}

void FSMC_RA8875::fillCurve(int16_t xCenter, int16_t yCenter, int16_t longAxis, int16_t shortAxis, uint8_t curvePart, uint16_t color)
{
  curvePart = curvePart % 4; //limit to the range 0-3
  if (_portrait) {//fix a problem with rotation
    if (curvePart == 0) {
      curvePart = 2;
    } else if (curvePart == 2) {
      curvePart = 0;
    }
  }
  _ellipseCurve_helper(xCenter, yCenter, longAxis, shortAxis, curvePart, color, true);
}

// Прямоугольники со скруглёнными краями
void FSMC_RA8875::drawRoundRect(int16_t x, int16_t y, int16_t w, int16_t h, int16_t r, uint16_t color)
{
  if (r == 0) drawRect(x,y,w,h,color);
  if (w < 2 || h < 2) return;//it cannot be!
//  if (w < 1 || h < 1) return;//it cannot be!
//  if (w < 2 && h < 2){ //render as pixel
//    drawPixel(x,y,color);
//  } else {       //render as rect
    if (w < h && (r * 2) >= w) r = (w / 2) - 1;
    if (w > h && (r * 2) >= h) r = (h / 2) - 1;
    if (r == w || r == h) drawRect(x,y,w,h,color);
    _roundRect_helper(x, y, (x + w) - 1, (y + h) - 1, r, color, false);
//  }
}

void FSMC_RA8875::fillRoundRect(int16_t x, int16_t y, int16_t w, int16_t h, int16_t r, uint16_t color)
{
  if (r == 0) fillRect(x,y,w,h,color);
  if (w < 2 || h < 2) return;//it cannot be!
//  if (w < 1 || h < 1) return;//it cannot be!
//  if (w < 2 && h < 2){ //render as pixel
//    drawPixel(x,y,color);
//  } else {       //render as rect
    if (w < h && (r * 2) >= w) r = (w / 2) - 1;
    if (w > h && (r  *2) >= h) r = (h / 2) - 1;
    if (r == w || r == h) drawRect(x,y,w,h,color);
    _roundRect_helper(x, y, (x + w) - 1, (y + h) - 1, r, color, true);
//  }
}

void FSMC_RA8875::drawPolygon(int16_t cx, int16_t cy, uint8_t sides, int16_t diameter, float rot, uint16_t color)
{
  float PI = 3.1415926535897932384626433832795;
  _center_helper(cx,cy);
  sides = (sides > 2? sides : 3);
  float dtr = (PI / 180.0) + PI;
  float rads = 360.0 / sides;//points spacd equally
  uint8_t i;
  for (i = 0; i < sides; i++) {
    drawLine(
      cx + (sin((i*rads + rot) * dtr) * diameter),
      cy + (cos((i*rads + rot) * dtr) * diameter),
      cx + (sin(((i+1)*rads + rot) * dtr) * diameter),
      cy + (cos(((i+1)*rads + rot) * dtr) * diameter),
      color);
  }
}

/***********************************************************
 * Работа с текстом
 ***********************************************************/
void FSMC_RA8875::setFontScale(uint8_t scale)
{
  _scaleX = _scaleY = scale;
//  setFontScale(scale,scale);
}

void FSMC_RA8875::setFontScale(uint8_t xscale,uint8_t yscale)
{
  _scaleX = xscale;
  _scaleY = yscale;
}

/*
 *
 */
void FSMC_RA8875::setXY(int16_t x, int16_t y)
{
  setX(x);
  setY(y);
}

void FSMC_RA8875::setX(int16_t x)
{
  if (x < 0) x = 0;
  if (_portrait){//fix 0.69b21
    if (x >= HEIGHT) x = HEIGHT-1;
    WriteReg(RA8875_CURV0, x & 0xFF);
    WriteReg(RA8875_CURV0+1, x >> 8);
  } else {
    if (x >= WIDTH) x = WIDTH-1;
    WriteReg(RA8875_CURH0, x & 0xFF);
    WriteReg(RA8875_CURH0+1, (x >> 8));
  }
}

void FSMC_RA8875::setY(int16_t y)
{
  if (y < 0) y = 0;
  if (_portrait){//fix 0.69b21
    if (y >= WIDTH) y = WIDTH-1;
    WriteReg(RA8875_CURH0, y & 0xFF);
    WriteReg(RA8875_CURH0+1, (y >> 8));
  } else {
    if (y >= HEIGHT) y = HEIGHT-1;
    WriteReg(RA8875_CURV0, y & 0xFF);
    WriteReg(RA8875_CURV0+1, y >> 8);
  }
}

/***********************************************************
 * Вспомогательная графика
 ***********************************************************/
void FSMC_RA8875::_line_addressing(int16_t x0, int16_t y0, int16_t x1, int16_t y1)
{
  //X0
  WriteReg(RA8875_DLHSR0,    x0 & 0xFF);
  WriteReg(RA8875_DLHSR0 + 1,x0 >> 8);
  //Y0
  WriteReg(RA8875_DLVSR0,    y0 & 0xFF);
  WriteReg(RA8875_DLVSR0 + 1,y0 >> 8);
  //X1
  WriteReg(RA8875_DLHER0,    x1 & 0xFF);
  WriteReg(RA8875_DLHER0 + 1,x1 >> 8);
  //Y1
  WriteReg(RA8875_DLVER0,    y1 & 0xFF);
  WriteReg(RA8875_DLVER0 + 1,y1 >> 8);
}

void FSMC_RA8875::_curve_addressing(int16_t x0, int16_t y0, int16_t x1, int16_t y1)
{
  //center
  WriteReg(RA8875_DEHR0,    x0 & 0xFF);
  WriteReg(RA8875_DEHR0 + 1,x0 >> 8);
  WriteReg(RA8875_DEVR0,    y0 & 0xFF);
  WriteReg(RA8875_DEVR0 + 1,y0 >> 8);
  //long,short ax
  WriteReg(RA8875_ELL_A0,    x1 & 0xFF);
  WriteReg(RA8875_ELL_A0 + 1,x1 >> 8);
  WriteReg(RA8875_ELL_B0,    y1 & 0xFF);
  WriteReg(RA8875_ELL_B0 + 1,y1 >> 8);
}

// Рисуется прямоугольник
void FSMC_RA8875::_rect_helper(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color, bool filled)
{
  if (w < 1 || h < 1) return;//why draw invisible rects?
  if (w >= _width) return;
  if (h >= _height) return;

  if (_portrait) {swapvals(x,y); swapvals(w,h);}

  _checkLimits_helper(x,y);

//  if (_textMode) _setTextMode(false);//we are in text mode?
  if (color != _foreColor) setForegroundColor(color);

  _line_addressing(x,y,w,h);

  WR_REG(RA8875_DCR);
  filled == true ? WR_DATA(0xB0) : WR_DATA(0x90);
  WaitPoll(RA8875_DCR, RA8875_DCR_LINESQUTRI_STATUS);
}

// Рисуется окружность, но похоже на ошибку, задаётся не радиус, а диаметр.
void FSMC_RA8875::_circle_helper(int16_t x0, int16_t y0, int16_t r, uint16_t color, bool filled)
{
  if(_portrait) swapvals(x0, y0); //0.69b21
// Это место пока оставим в покое
//  if(r < 1) r = 1;
//  if(r < 2)
//  { //NEW
//    drawPixel(x0, y0, color);
//    return;
//  }
  if(r > HEIGHT / 2) r = (HEIGHT / 2) - 1; //Недокументированное ограничение контроллера RA8875

//  if(_textMode) _setTextMode(false); //we are in text mode?
  if(color != _foreColor) setForegroundColor(color);

  WriteReg(RA8875_DCHR0, x0 & 0xFF);
  WriteReg(RA8875_DCHR0 + 1, x0 >> 8);

  WriteReg(RA8875_DCVR0, y0 & 0xFF);
  WriteReg(RA8875_DCVR0 + 1, y0 >> 8);
  WriteReg(RA8875_DCRR, r);

  WR_REG(RA8875_DCR);
  filled == true ? WR_DATA(RA8875_DCR_CIRCLE_START | RA8875_DCR_FILL) : WR_DATA(RA8875_DCR_CIRCLE_START | RA8875_DCR_NOFILL);
  WaitPoll(RA8875_DCR, RA8875_DCR_CIRCLE_STATUS); //ZzZzz
}

// Отрисовка треугольника
void FSMC_RA8875::_triangle_helper(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color, bool filled)
{
  if (x0 >= _width || x1 >= _width || x2 >= _width) return;
  if (y0 >= _height || y1 >= _height || y2 >= _height) return;

  if (_portrait) {swapvals(x0,y0); swapvals(x1,y1); swapvals(x2,y2);}

  if (x0 == x1 && y0 == y1){
    drawLine(x0, y0, x2, y2,color);
    return;
  } else if (x0 == x2 && y0 == y2){
    drawLine(x0, y0, x1, y1,color);
    return;
  } else if (x0 == x1 && y0 == y1 && x0 == x2 && y0 == y2) {//new
        drawPixel(x0, y0, color);
    return;
  }

  if (y0 > y1) {swapvals(y0, y1); swapvals(x0, x1);}

    if (y1 > y2) {swapvals(y2, y1); swapvals(x2, x1);}

    if (y0 > y1) {swapvals(y0, y1); swapvals(x0, x1);}

  if (y0 == y2) { // Handle awkward all-on-same-line case as its own thing
    int16_t a, b;
        a = b = x0;
    if (x1 < a) {
      a = x1;
    } else if (x1 > b) {
      b = x1;
    }
        if (x2 < a) {
      a = x2;
    } else if (x2 > b) {
      b = x2;
    }
//        drawFastHLine(a, y0, b-a+1, color);
        return;
    }

//  if (_textMode) _setTextMode(false);//we are in text mode?

  if (color != _foreColor) setForegroundColor(color);//0.69b30 avoid several SPI calls

  //_checkLimits_helper(x0,y0);
  //_checkLimits_helper(x1,y1);

  _line_addressing(x0,y0,x1,y1);
  //p2

  WriteReg(RA8875_DTPH0,    x2 & 0xFF);
  WriteReg(RA8875_DTPH0 + 1,x2 >> 8);
  WriteReg(RA8875_DTPV0,    y2 & 0xFF);
  WriteReg(RA8875_DTPV0 + 1,y2 >> 8);

  WR_REG(RA8875_DCR);
  filled == true ? WR_DATA(0xA1) : WR_DATA(0x81);

  WaitPoll(RA8875_DCR, RA8875_DCR_LINESQUTRI_STATUS);
}

// Отрисовка элипсов
void FSMC_RA8875::_ellipseCurve_helper(int16_t xCenter, int16_t yCenter, int16_t longAxis, int16_t shortAxis, uint8_t curvePart,uint16_t color, bool filled)
{
  _center_helper(xCenter,yCenter);//use CENTER?

  if (_portrait) {
    swapvals(xCenter,yCenter);
    swapvals(longAxis,shortAxis);
    if (longAxis > _height/2) longAxis = (_height / 2) - 1;
    if (shortAxis > _width/2) shortAxis = (_width / 2) - 1;
  } else {
    if (longAxis > _width/2) longAxis = (_width / 2) - 1;
    if (shortAxis > _height/2) shortAxis = (_height / 2) - 1;
  }
//  if (longAxis == 1 && shortAxis == 1) {
//    drawPixel(xCenter,yCenter,color);
//    return;
//  }
  _checkLimits_helper(xCenter,yCenter);

//  if (_textMode) _setTextMode(false);//we are in text mode?

  if (color != _foreColor) setForegroundColor(color);

  _curve_addressing(xCenter,yCenter,longAxis,shortAxis);
  WR_REG(RA8875_ELLIPSE);

  if (curvePart != 255){
    curvePart = curvePart % 4; //limit to the range 0-3
    filled == true ? WR_DATA(0xD0 | (curvePart & 0x03)) : WR_DATA(0x90 | (curvePart & 0x03));
  } else {
    filled == true ? WR_DATA(0xC0) : WR_DATA(0x80);
  }
  WaitPoll(RA8875_ELLIPSE, RA8875_ELLIPSE_STATUS);
}

// Прямоугольник со скруглёнными краями
void FSMC_RA8875::_roundRect_helper(int16_t x, int16_t y, int16_t w, int16_t h, int16_t r, uint16_t color, bool filled)
{
  if (_portrait) {swapvals(x,y); swapvals(w,h);}

//  if (_textMode) _setTextMode(false);//we are in text mode?

  if (color != _foreColor) setForegroundColor(color);//0.69b30 avoid several SPI calls


  _line_addressing(x,y,w,h);

  WriteReg(RA8875_ELL_A0,    r & 0xFF);
  WriteReg(RA8875_ELL_A0 + 1,r >> 8);
  WriteReg(RA8875_ELL_B0,    r & 0xFF);
  WriteReg(RA8875_ELL_B0 + 1,r >> 8);

  WR_REG(RA8875_ELLIPSE);
  filled == true ? WR_DATA(0xE0) : WR_DATA(0xA0);
  WaitPoll(RA8875_ELLIPSE, RA8875_DCR_LINESQUTRI_STATUS);
}

/***********************************************************
 * Работа с текстом
 ***********************************************************/
void  FSMC_RA8875::setCursor(uint16_t x, uint16_t y)
{
  _cursorX = x;
  _cursorY = y;
}

size_t FSMC_RA8875::write(uint8_t c)
{
  if( !gfxFont)
  { // 'Classic' built-in font

    if(c=='\n')
    {                        // Newline?
      _cursorX = 0;                     // Reset x to zero,
      _cursorY += _scaleY*8;        // advance y one line
    } else if(c!='\r')
    {                 // Ignore carriage returns
      if(wrap&&((_cursorX+_scaleX*6)>_width))
      { // Off right?
        _cursorX = 0;                 // Reset x to zero,
        _cursorY += _scaleY*8;    // advance y one line
      }
      drawChar(_cursorX, _cursorY, c, textcolor, textbgcolor, _scaleX, _scaleY);
      _cursorX += _scaleX*6;          // Advance x one char
    }

  } else
  { // Custom font

    if(c=='\n')
    {
      _cursorX = 0;
      _cursorY += (int16_t) _scaleY*(uint8_t) pgm_read_byte( &gfxFont->yAdvance);
    } else if(c!='\r')
    {
      uint8_t first = pgm_read_byte( &gfxFont->first);
      if((c>=first)&&(c<=(uint8_t) pgm_read_byte( &gfxFont->last)))
      {
        GFXglyph *glyph = pgm_read_glyph_ptr(gfxFont, c-first);
        uint8_t w = pgm_read_byte( &glyph->width), h = pgm_read_byte( &glyph->height);
        if((w>0)&&(h>0))
        { // Is there an associated bitmap?
          int16_t xo = (int8_t) pgm_read_byte( &glyph->xOffset); // sic
          if(wrap&&((_cursorX+_scaleX*(xo+w))>_width))
          {
            _cursorX = 0;
            _cursorY += (int16_t) _scaleY*(uint8_t) pgm_read_byte( &gfxFont->yAdvance);
          }
          drawChar(_cursorX, _cursorY, c, textcolor, textbgcolor, _scaleX, _scaleY);
        }
        _cursorX += (uint8_t) pgm_read_byte( &glyph->xAdvance)*(int16_t) _scaleX;
      }
    }

  }
  return 1;
}



void FSMC_RA8875::drawChar(int16_t x, int16_t y, unsigned char c, uint16_t color, uint16_t bg, uint8_t size)
{
  drawChar(x, y, c, color, bg, size, size);
}



void FSMC_RA8875::drawChar(int16_t x, int16_t y, unsigned char c, uint16_t color, uint16_t bg, uint8_t size_x, uint8_t size_y)
{

  if( !gfxFont)
  { // 'Classic' built-in font

    if((x>=_width)|| // Clip right
        (y>=_height)|| // Clip bottom
        ((x+6*size_x-1)<0)|| // Clip left
        ((y+8*size_y-1)<0))   // Clip top
      return;

    if( !_cp437&&(c>=176)) c++; // Handle 'classic' charset behavior

    for(int8_t i = 0; i<5; i++)
    { // Char bitmap = 5 columns
      uint8_t line = pgm_read_byte( &font[c*5+i]);
      for(int8_t j = 0; j<8; j++, line >>= 1)
      {
        if(line&1)
        {
          if(size_x==1&&size_y==1) drawPixel(x+i, y+j, color);
          else fillRect(x+i*size_x, y+j*size_y, size_x, size_y, color);
        } else if(bg!=color)
        {
          if(size_x==1&&size_y==1) drawPixel(x+i, y+j, bg);
          else fillRect(x+i*size_x, y+j*size_y, size_x, size_y, bg);
        }
      }
    }
    if(bg!=color)
    { // If opaque, draw vertical line for last column
      if(size_x==1&&size_y==1) drawFastVLine(x+5, y, 8, bg);
      else fillRect(x+5*size_x, y, size_x, 8*size_y, bg);
    }
  } else
  { // Custom font

    // Character is assumed previously filtered by write() to eliminate
    // newlines, returns, non-printable characters, etc.  Calling
    // drawChar() directly with 'bad' characters of font may cause mayhem!

    c -= (uint8_t) pgm_read_byte( &gfxFont->first);
    GFXglyph *glyph = pgm_read_glyph_ptr(gfxFont, c);
    uint8_t *bitmap = pgm_read_bitmap_ptr(gfxFont);

    uint16_t bo = pgm_read_word( &glyph->bitmapOffset);
    uint8_t w = pgm_read_byte( &glyph->width), h = pgm_read_byte( &glyph->height);
    int8_t xo = pgm_read_byte( &glyph->xOffset), yo = pgm_read_byte( &glyph->yOffset);
    uint8_t xx, yy, bits = 0, bit = 0;
    int16_t xo16 = 0, yo16 = 0;

    if(size_x>1||size_y>1)
    {
      xo16 = xo;
      yo16 = yo;
    }

    // Todo: Add character clipping here

    // NOTE: THERE IS NO 'BACKGROUND' COLOR OPTION ON CUSTOM FONTS.
    // THIS IS ON PURPOSE AND BY DESIGN.  The background color feature
    // has typically been used with the 'classic' font to overwrite old
    // screen contents with new data.  This ONLY works because the
    // characters are a uniform size; it's not a sensible thing to do with
    // proportionally-spaced fonts with glyphs of varying sizes (and that
    // may overlap).  To replace previously-drawn text when using a custom
    // font, use the getTextBounds() function to determine the smallest
    // rectangle encompassing a string, erase the area with fillRect(),
    // then draw new text.  This WILL infortunately 'blink' the text, but
    // is unavoidable.  Drawing 'background' pixels will NOT fix this,
    // only creates a new set of problems.  Have an idea to work around
    // this (a canvas object type for MCUs that can afford the RAM and
    // displays supporting setAddrWindow() and pushColors()), but haven't
    // implemented this yet.

    for(yy = 0; yy<h; yy++)
    {
      for(xx = 0; xx<w; xx++)
      {
        if( !(bit++&7))
        {
          bits = pgm_read_byte( &bitmap[bo++]);
        }
        if(bits&0x80)
        {
          if(size_x==1&&size_y==1)
          {
            drawPixel(x+xo+xx, y+yo+yy, color);
          } else
          {
            fillRect(x+(xo16+xx)*size_x, y+(yo16+yy)*size_y, size_x, size_y, color);
          }
        }
        bits <<= 1;
      }
    }
  } // End classic vs custom font
}


/***********************************************************
 * Управление цветом
 ***********************************************************/
uint16_t FSMC_RA8875::colorInterpolation(uint16_t color1,uint16_t color2,uint16_t pos,uint16_t div)
{
    if (pos == 0) return color1;
    if (pos >= div) return color2;
  uint8_t r1,g1,b1;
  Color565ToRGB(color1,r1,g1,b1);//split in r,g,b
  uint8_t r2,g2,b2;
  Color565ToRGB(color2,r2,g2,b2);//split in r,g,b
  return colorInterpolation(r1,g1,b1,r2,g2,b2,pos,div);
}

uint16_t FSMC_RA8875::colorInterpolation(uint8_t r1,uint8_t g1,uint8_t b1,uint8_t r2,uint8_t g2,uint8_t b2,uint16_t pos,uint16_t div)
{
    if (pos == 0) return Color565(r1,g1,b1);
    if (pos >= div) return Color565(r2,g2,b2);
  float pos2 = (float)pos/div;
  return Color565(
        (uint8_t)(((1.0 - pos2) * r1) + (pos2 * r2)),
        (uint8_t)((1.0 - pos2) * g1 + (pos2 * g2)),
        (uint8_t)(((1.0 - pos2) * b1) + (pos2 * b2))
  );
}


void FSMC_RA8875::setBackgroundColor(uint16_t color)
{
  _backColor = color;//keep track
  WriteReg(RA8875_BGCR0,((color & 0xF800) >> 11));//_RA8875colorMask[_colorIndex]));//11
  WriteReg(RA8875_BGCR0+1,((color & 0x07E0) >> 5));//_RA8875colorMask[_colorIndex+1]));//5
  WriteReg(RA8875_BGCR0+2,((color & 0x001F)) );//>> _RA8875colorMask[_colorIndex+2]));//0
}

void FSMC_RA8875::setBackgroundColor(uint8_t R,uint8_t G,uint8_t B)
{
  _backColor = Color565(R,G,B);//keep track
  WriteReg(RA8875_BGCR0,R);
  WriteReg(RA8875_BGCR0+1,G);
  WriteReg(RA8875_BGCR0+2,B);
}

/***********************************************************
 * Управление ориентацией и окнами
 ***********************************************************/
void FSMC_RA8875::setRotation(uint8_t rotation)//0.69b32 - less code
{
//  uint8_t _FNCR1_Reg = 0;
  _rotation = rotation % 4; //limit to the range 0-3
  switch (_rotation) {
  case 0:
    _portrait = false;
    _scanDirection(0,0);
    _TP_MinX = TOUCSRCAL_XLOW; _TP_MinY = TOUCSRCAL_YHIGH; _TP_MaxX = TOUCSRCAL_XHIGH; _TP_MaxY = TOUCSRCAL_YLOW;
    break;
  case 1:
    //90
    _portrait = true;
    _scanDirection(1,0);
//    #if defined(USE_RA8875_TOUCH)
//      if (!_calibrated) {
//        _tsAdcMinX = 1023; _tsAdcMinY = 0; _tsAdcMaxX = 0; _tsAdcMaxY = 1023;
//      } else {
        _TP_MinX = TOUCSRCAL_YHIGH; _TP_MinY = TOUCSRCAL_XHIGH; _TP_MaxX = TOUCSRCAL_YLOW; _TP_MaxY = TOUCSRCAL_XLOW;
//      }
//    #endif
    break;
  case 2:
    //180
    _portrait = false;
    _scanDirection(1,1);
    _TP_MinX = TOUCSRCAL_XHIGH; _TP_MinY = TOUCSRCAL_YLOW; _TP_MaxX = TOUCSRCAL_XLOW; _TP_MaxY = TOUCSRCAL_YHIGH;
    break;
  case 3:
    //270
    _portrait = true;
    _scanDirection(0,1);
    _TP_MinX = TOUCSRCAL_XLOW; _TP_MinY = TOUCSRCAL_YHIGH; _TP_MaxX = TOUCSRCAL_XHIGH; _TP_MaxY = TOUCSRCAL_YLOW;
    swapvals(_TP_MinX, _TP_MinY);
    swapvals(_TP_MaxX, _TP_MaxY);
    break;
  }
  if (_portrait){
    _width = HEIGHT;
    _height = WIDTH;
    _FNCR1_Reg |= (1 << 4);
  } else {
    _width = WIDTH;
    _height = HEIGHT;
    _FNCR1_Reg &= ~(1 << 4);
  }
  WriteReg(RA8875_FNCR1,_FNCR1_Reg);
  setActiveWindow();
}

// Активное окно с определёнными границами
void FSMC_RA8875::setActiveWindow(int16_t XL,int16_t XR ,int16_t YT ,int16_t YB)
{
  if (_portrait){ swapvals(XL,YT); swapvals(XR,YB);}

  if (XR >= WIDTH) XR = WIDTH;
  if (YB >= HEIGHT) YB = HEIGHT;

  _activeWindowXL = XL; _activeWindowXR = XR;
  _activeWindowYT = YT; _activeWindowYB = YB;
  _updateActiveWindow(false);
}

// Активное окно во весь экран
void FSMC_RA8875::setActiveWindow(void)
{
  _activeWindowXL = 0; _activeWindowXR = WIDTH;
  _activeWindowYT = 0; _activeWindowYB = HEIGHT;
  if (_portrait){swapvals(_activeWindowXL,_activeWindowYT); swapvals(_activeWindowXR,_activeWindowYB);}
  _updateActiveWindow(true);
}

// Обновление активного окна
void FSMC_RA8875::_updateActiveWindow(bool full)
{
  if (full){
    // X
    WriteReg(RA8875_HSAW0,    0x00);
    WriteReg(RA8875_HSAW0 + 1,0x00);
    WriteReg(RA8875_HEAW0,    (WIDTH) & 0xFF);
    WriteReg(RA8875_HEAW0 + 1,(WIDTH) >> 8);
    // Y
    WriteReg(RA8875_VSAW0,    0x00);
    WriteReg(RA8875_VSAW0 + 1,0x00);
    WriteReg(RA8875_VEAW0,    (HEIGHT) & 0xFF);
    WriteReg(RA8875_VEAW0 + 1,(HEIGHT) >> 8);
  } else {
    // X
    WriteReg(RA8875_HSAW0,    _activeWindowXL & 0xFF);
    WriteReg(RA8875_HSAW0 + 1,_activeWindowXL >> 8);
    WriteReg(RA8875_HEAW0,    _activeWindowXR & 0xFF);
    WriteReg(RA8875_HEAW0 + 1,_activeWindowXR >> 8);
    // Y
    WriteReg(RA8875_VSAW0,     _activeWindowYT & 0xFF);
    WriteReg(RA8875_VSAW0 + 1,_activeWindowYT >> 8);
    WriteReg(RA8875_VEAW0,    _activeWindowYB & 0xFF);
    WriteReg(RA8875_VEAW0 + 1,_activeWindowYB >> 8);
  }
}

/***********************************************************
 * Вспомогательные системные функции
 ***********************************************************/
void FSMC_RA8875::_setTextMode(bool m)
{
//  uint8_t _MWCR0_Reg = 0;
  if (m == _textMode) return;
  WR_REG(RA8875_MWCR0);
  //if (m != 0){//text
  if (m){//text
    _MWCR0_Reg |= (1 << 7);
    _textMode = true;
  } else {//graph
    _MWCR0_Reg &= ~(1 << 7);
    _textMode = false;
  }
  WR_DATA(_MWCR0_Reg);
}

void FSMC_RA8875::_scanDirection(bool invertH, bool invertV)
{
//  uint8_t _DPCR_Reg = 0;
  invertH == true ? _DPCR_Reg |= (1 << 3) : _DPCR_Reg &= ~(1 << 3);
  invertV == true ? _DPCR_Reg |= (1 << 2) : _DPCR_Reg &= ~(1 << 2);
  WriteReg(RA8875_DPCR,_DPCR_Reg);
}

/***********************************************************
 * Управление цветом
 ***********************************************************/
void FSMC_RA8875::setForegroundColor(uint16_t color)
{
  _foreColor = color;
  WriteReg(RA8875_FGCR0,((color & 0xF800) >> 11));
  WriteReg(RA8875_FGCR0+1,((color & 0x07E0) >> 5));
  WriteReg(RA8875_FGCR0+2,((color & 0x001F) ));
}

/***********************************************************
 * Инициализация дисплея и системные функции
 ***********************************************************/
// Инициализация FSMC
bool FSMC_RA8875::FSMC_Init(enum RA8875sizes Size)
{
  _size = Size;
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
  return true;
}

// Инициализация необходимых портов
void FSMC_RA8875::FSMC_GPIO_Init()
{
  uint16_t Temp = 0;

  if (_FSMC_Initialized) {return;}
  _FSMC_Initialized = 1;

  // Разрешаем тактирование на порты и FSMC
  RCC->AHB1ENR |= RCC_AHB1ENR_GPIODEN | RCC_AHB1ENR_GPIOEEN;
  RCC->AHB3ENR |= RCC_AHB3ENR_FSMCEN;

  _SetSpeed(_FSMC_GPIO_Speed);
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
      _SetPinAlternate(GPIOD, GPIO_PIN_11, GPIO_AF12_FSMC);
      _TFT_Ax   = 0x0001FFFE;
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
  _SetPin(_GPIO_Reset, _GPIO_Pin_Reset, Output, Pull_Up);             // Инициализация вывода сброса
  _DigitalWriteBit(_GPIO_Reset, _GPIO_Pin_Reset, High);                // Уснтанавливаем его в 1
  _SetPin(_GPIO_Blackout, _GPIO_Pin_Blackout, Output, No_Pull);       // Инициализация вывода управления подсветкой
}

// Установка базового адреса
void FSMC_RA8875::_setBaseAddress(uint32_t Base, uint32_t Data)
{
  LCD_RA8875 = ((RA8875_TypeDef*) ((uint32_t) (Base | Data)));
}

// Инициализация дисплея
void FSMC_RA8875::Init(void)
{
  if (_size == RA8875_480x80)
  {
    _width = WIDTH = 480;
    _height = HEIGHT = 80;
  }
  else if (_size == RA8875_480x128)
  {
    _width = WIDTH = 480;
    _height = HEIGHT = 128;
  }
  else if (_size == RA8875_480x272)
  {
    _width = WIDTH = 480;
    _height = HEIGHT = 272;
  }
  else if (_size == RA8875_800x480)
  {
    _width = WIDTH = 800;
    _height = HEIGHT = 480;
  }
  else
  {
//    return false;
  }
  _activeWindowXL = 0;
  _activeWindowXR = WIDTH;
  _activeWindowYT = 0;
  _activeWindowYB = HEIGHT;
  _textMode = false;

  _DPCR_Reg = 0b00000000;
  _MWCR0_Reg = 0b00000000;
  _FNCR0_Reg = 0b00000000;
  _FNCR1_Reg = 0b00000000;
  _FWTSET_Reg = 0b00000000;
  _SFRSET_Reg = 0b00000000;

  _FNTinterline = 0;


  // Сброс
  _DigitalWriteBit(_GPIO_Reset, _GPIO_Pin_Reset, Low);
  delay(100);
  _DigitalWriteBit(_GPIO_Reset, _GPIO_Pin_Reset, High);
  delay(100);

  // Инициализируем PLL
  if (_size == RA8875_480x80 || _size == RA8875_480x128 || _size == RA8875_480x272)
  {
    WriteReg(RA8875_PLLC1, RA8875_PLLC1_PLLDIV1 + 10);
    delay(1);
    WriteReg(RA8875_PLLC2, RA8875_PLLC2_DIV4);
    delay(1);
  }
  else /* (_size == RA8875_800x480) */
  {
    WriteReg(RA8875_PLLC1, RA8875_PLLC1_PLLDIV1 + 11);
    delay(1);
    WriteReg(RA8875_PLLC2, RA8875_PLLC2_DIV4);
    delay(1);
  }

  // Инициализируем дисплей
  WriteReg(RA8875_SYSR, RA8875_SYSR_16BPP | RA8875_SYSR_MCU8);

  /* ВременнЫе характеристики */
  uint8_t   pixclk;
  uint8_t   hsync_start;
  uint8_t   hsync_pw;
  uint8_t   hsync_finetune;
  uint8_t   hsync_nondisp;
  uint8_t   vsync_pw;
  uint16_t  vsync_nondisp;
  uint16_t  vsync_start;

  // Устанавливаем корректирующие значения дисплея перед использованием
  if(_size == RA8875_480x80)
  {
    pixclk = RA8875_PCSR_PDATL | RA8875_PCSR_4CLK;
    hsync_nondisp   = 10;
    hsync_start     = 8;
    hsync_pw        = 48;
    hsync_finetune  = 0;
    vsync_nondisp   = 3;
    vsync_start     = 8;
    vsync_pw        = 10;
    _voffset        = 192; // Используем 80 нижних пикселов в 272 пиксельном контроллере
  }
  else if(_size == RA8875_480x128 || _size == RA8875_480x272)
  {
    pixclk = RA8875_PCSR_PDATL | RA8875_PCSR_4CLK;
    hsync_nondisp   = 10;
    hsync_start     = 8;
    hsync_pw        = 48;
    hsync_finetune  = 0;
    vsync_nondisp   = 3;
    vsync_start     = 8;
    vsync_pw        = 10;
    _voffset        = 0;
  }
  else // (_size == RA8875_800x480)
  {
    pixclk = RA8875_PCSR_PDATL | RA8875_PCSR_2CLK;
    hsync_nondisp   = 26;
    hsync_start     = 32;
    hsync_pw        = 96;
    hsync_finetune  = 0;
    vsync_nondisp   = 32;
    vsync_start     = 23;
    vsync_pw        = 2;
    _voffset        = 0;
  }

  WriteReg(RA8875_PCSR, pixclk);
  delay(1);

  // Горизонтальные установки
  WriteReg(RA8875_HDWR, (_width / 8) - 1);                            // H width: (HDWR + 1) * 8 = 480
  WriteReg(RA8875_HNDFTR, RA8875_HNDFTR_DE_HIGH + hsync_finetune);
  WriteReg(RA8875_HNDR, (hsync_nondisp - hsync_finetune - 2) / 8);    // H non-display: HNDR * 8 + HNDFTR + 2 = 10
  WriteReg(RA8875_HSTR, hsync_start / 8 - 1);                         // Hsync start: (HSTR + 1)*8
  WriteReg(RA8875_HPWR, RA8875_HPWR_LOW + (hsync_pw / 8 - 1));        // HSync pulse width = (HPWR+1) * 8

  // Вертикальные установки
  WriteReg(RA8875_VDHR0, (uint16_t) (_height - 1 + _voffset) & 0xFF);
  WriteReg(RA8875_VDHR1, (uint16_t) (_height - 1 + _voffset) >> 8);
  WriteReg(RA8875_VNDR0, vsync_nondisp - 1);                          // V non-display period = VNDR + 1
  WriteReg(RA8875_VNDR1, vsync_nondisp >> 8);
  WriteReg(RA8875_VSTR0, vsync_start - 1);                            // Vsync start position = VSTR + 1
  WriteReg(RA8875_VSTR1, vsync_start >> 8);
  WriteReg(RA8875_VPWR, RA8875_VPWR_LOW + vsync_pw - 1);              // Vsync pulse width = VPWR + 1

  // Установка активного окна по X
  WriteReg(RA8875_HSAW0, 0);                                          // Горизонтальная стартовая точка
  WriteReg(RA8875_HSAW1, 0);
  WriteReg(RA8875_HEAW0, (uint16_t) (_width - 1) & 0xFF);             // Горизонтальная конечная точка
  WriteReg(RA8875_HEAW1, (uint16_t) (_width - 1) >> 8);

  // Установка активного окна по Y
  WriteReg(RA8875_VSAW0, 0 + _voffset);                               // Вертикальная стартовая точка
  WriteReg(RA8875_VSAW1, 0 + _voffset);
  WriteReg(RA8875_VEAW0, (uint16_t) (_height - 1 + _voffset) & 0xFF); // Вертикальная конечная точка
  WriteReg(RA8875_VEAW1, (uint16_t) (_height - 1 + _voffset) >> 8);

  // Очистка окна
  _setTextMode(false);
  WriteReg(RA8875_MCLR, RA8875_MCLR_START | RA8875_MCLR_FULL);
  delay(500);
  WriteReg(0x8A, 0x90);

  // Включаем подсветку экрана
  _DigitalWriteBit(_GPIO_Blackout, _GPIO_Pin_Blackout, High);

  clearScreen (COLOR_BLACK);

}

/*
 * Запись чтение регистров/данных дисплея
 * В данном варианте поменяны местами регистр адреса с регистром данных
 * так как у этого вида контроллера RS сигнал инверсный.
 */
void FSMC_RA8875::WR_DATA(uint16_t regval)
{
  regval=regval;
  LCD_RA8875->LCD_REG=regval;
}

void FSMC_RA8875::WR_REG(uint16_t data)
{
  data=data;
  LCD_RA8875->LCD_RAM=data;
}

uint16_t FSMC_RA8875::RD_DATA(void)
{
  uint16_t ram;
  ram=LCD_RA8875->LCD_REG;
  return ram;
}

void FSMC_RA8875::WriteReg(uint16_t LCD_Reg, uint16_t LCD_RegValue)
{
  LCD_RA8875->LCD_RAM = LCD_Reg;
  LCD_RA8875->LCD_REG = LCD_RegValue;
}

uint16_t FSMC_RA8875::ReadReg(uint16_t LCD_Reg)
{
  WR_REG(LCD_Reg);
//  delay(5); // !!!!!!!!!!!!!!!!! Вот она эта сука, которая тормозила вывод.
  return RD_DATA();
}

/*******************************************************************************
 * Системные утилиты управлением дисплея
 *******************************************************************************/
void FSMC_RA8875::GPIOX(bool on)                                      // Есть не у всех моделей
{
  if (on) WriteReg(RA8875_GPIOX, 1); else WriteReg(RA8875_GPIOX, 0);
}

void FSMC_RA8875::PWM1out(uint8_t p) { WriteReg(RA8875_P1DCR, p); }   //Управление яркостью дисплея (Не у всех)
void FSMC_RA8875::PWM2out(uint8_t p) { WriteReg(RA8875_P2DCR, p); }   // Тоже яркость, для других моделей

void FSMC_RA8875::PWM1config(bool on, uint8_t clock)
{
  if (on)
    WriteReg(RA8875_P1CR, RA8875_P1CR_ENABLE | (clock & 0xF));
  else
    WriteReg(RA8875_P1CR, RA8875_P1CR_DISABLE | (clock & 0xF));
}

void FSMC_RA8875::PWM2config(bool on, uint8_t clock)
{
  if (on)
    WriteReg(RA8875_P2CR, RA8875_P2CR_ENABLE | (clock & 0xF));
  else
    WriteReg(RA8875_P2CR, RA8875_P2CR_DISABLE | (clock & 0xF));
}

void FSMC_RA8875::displayOn(bool on)                                  // Включение/выключение дисплея
{
  if (on)
    WriteReg(RA8875_PWRR, RA8875_PWRR_NORMAL | RA8875_PWRR_DISPON);
  else
    WriteReg(RA8875_PWRR, RA8875_PWRR_NORMAL | RA8875_PWRR_DISPOFF);
}


bool FSMC_RA8875::WaitPoll(uint8_t regname, uint8_t waitflag)         // Ожидание выполнения команды, можно будет сделать на прерывании
{
  while (1)
  {
    uint8_t temp = ReadReg(regname);
    if(!(temp & waitflag)) return true;
  }
  return false; // MEMEFIX: yeah i know, unreached! - add timeout?
}

void FSMC_RA8875::_waitBusy(uint8_t res)
{
  uint8_t temp;
  unsigned long start = millis();//M.Sandercock
  do {
    if (res == 0x01) WR_REG(RA8875_DMACR);//dma
    temp = RD_DATA();
    if ((millis() - start) > 10) return;
  } while ((temp & res) == res);
}


void FSMC_RA8875::TestTXT(void)
{
//  // 1. Переходим в графический режим
//  Graphic_Mode();
//  // 2. Команта записи в CGRAM
//  Write_To_CGRAM();
//  // 3. Выбор первого символа в CGRAM
//  CGRAM_Select_Number(0);
//  // 4. Команда записи
//  LCD_CmdWrite(0x02);//MRWC
//  // 5. Набивам выбранную ячейку знакогенератора
//  for(i=0;i<16;i++)
//  {
//   LCD_DataWrite(gImage_Font_8x16[i]);
//   Chk_Busy();
//  }
//  // Шаги 3, 4, 5 - повторяются по количеству символов
//  //----- Вывод символа -----
//  // Переключаемся в обычный режим
//  Write_To_Bank1and2();
//  Write_To_Bank1();
//
//  LCD_CmdWrite(0x21);//
//  LCD_DataWrite(0x20);
//
//  LCD_CmdWrite(0x2E);//
//  LCD_DataWrite(0x00);
//  // Говорим что шрифт из CGRAM
//  CGRAM_Font();
//  // Устанавливаем цвет вывода
//  Text_color(0xffff);
//  Background_color(0x0000);
//  // Переключаемся текстовый режим
//  Text_Mode();
//  // Далее задаём координату и выводим один и тот же символ три раза
//  Font_Coordinate(0,0);
//  LCD_CmdWrite(0x02);//MRWC
//  LCD_DataWrite(0x00); // == CGRAM_Select_Number
//  Chk_Busy();
//
//  Font_Coordinate(0,100);
//  LCD_CmdWrite(0x02);//MRWC
//  LCD_DataWrite(0x00); // == CGRAM_Select_Number
//  Chk_Busy();
//
//  Font_Coordinate(8,100);
//  LCD_CmdWrite(0x02);//MRWC
//  LCD_DataWrite(0x00); // == CGRAM_Select_Number
//  Chk_Busy();
}

/*
 * ToDo Работа с тач скрином на резистивной матрице
 */
void FSMC_RA8875::TP_IntEnable(GPIO_TypeDef *GPIO_Port, uint16_t GPIO_Pin)
{
  uint8_t Temp = 0;
  _GPIO_Tp_Port = GPIO_Port;
  _GPIO_Tp_Pin  = GPIO_Pin;

  // Включение прерывания контроллеру дисплея
  WR_REG(RA8875_INT_IE);
  Temp = RD_DATA();
  Temp |= RA8875_TouchPanel_IE;
  WR_DATA(Temp);

  // Включение автоматического сканирования
  WR_REG(RA8875_TPCR1);
  Temp = RD_DATA();
  Temp &= ((~RA8875_TPCR1_MANUAL) | RA8875_TPCR1_DEBOUNCE);
  WR_DATA(Temp);

  // Включение тачскрина, ожидание 4096 тактов, частота тактирования АЦП делится на 4
  WR_REG(RA8875_TPCR0);
  WR_DATA(RA8875_TPCR0_EN |
      RA8875_TPCR0_WAIT_4096CLK |
      RA8875_TPCR0_ADCCLK_DIV4);

}

bool FSMC_RA8875::TP_isTouch(void)
{
  uint8_t Temp = 0;
  WR_REG(RA8875_INT_IF);
  Temp = RD_DATA();
  WR_DATA(RA8875_TouchPanel_IFC);
  if(Temp & RA8875_TouchPanel_IFC)
    return true;
  else
    return false;
}

void FSMC_RA8875::TP_GetCoordinate(uint16_t *X_Coord, uint16_t *Y_Coord)
{
  uint16_t Temp;
  WR_REG(RA8875_TPXH);
  Temp = RD_DATA();
  *X_Coord = (Temp << 2);
  WR_REG(RA8875_TPYH);
  Temp = RD_DATA();
  *Y_Coord = (Temp << 2);
  WR_REG(RA8875_TPXYL);
  Temp = RD_DATA();
  *X_Coord |= (Temp & 0x03);
  *Y_Coord |= ((Temp >> 2) & 0x03 );
  *X_Coord &= 0x3FF;
  *Y_Coord &= 0x3FF;

  *X_Coord = constrain(map(*X_Coord,_TP_MinX,_TP_MaxX,0,_width-1),0,_width-1);
  *Y_Coord = constrain(map(*Y_Coord,_TP_MinY,_TP_MaxY,0,_height-1),0,_height-1);
}

//void FSMC_RA8875::TP_IntEnable(bool IE)
//{
//  uint8_t Temp = 0;
//  WR_REG(RA8875_INT_IE);
//  if(IE)                                                              // Прерывание разрешено
//  {
//    Temp = RD_DATA();
//    Temp |= RA8875_TouchPanel_IE;
//    WR_DATA(Temp);
//  }
//  else
//  {
//    Temp = RD_DATA();
//    Temp &= ~RA8875_TouchPanel_IE;
//    WR_DATA(Temp);
//  }
//}

//extern "C" void EXTI15_10_IRQHandler(void)
//{
//  if((EXTI->IMR & EXTI_IMR_IM13) && (EXTI->EMR & EXTI_EMR_EM13))
//  {
//    EXTI->PR |= EXTI_PR_PR13;
//  }
//
//}










