/*
 * GFX_TFT.cpp
 *
 *  Created on: 8 апр. 2021 г.
 *      Author: User
 */

#include <GFX_TFT.h>

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


GFX_TFT::GFX_TFT(int16_t w, int16_t h):WIDTH(w), HEIGHT(h)
{
  _width              = WIDTH;
  _height             = HEIGHT;
  rotation            = 0;
  cursor_y            = cursor_x    = 0;
  textsize_x          = textsize_y  = 1;
  textcolor           = textbgcolor = 0xFFFF;
  wrap                = true;
  _cp437              = false;
  gfxFont             = NULL;

}

void GFX_TFT::writePixel(uint16_t x, uint16_t y, uint16_t color)
{
  drawPixel(x, y, color);
}

/*
 * Настройка текста
 */

void GFX_TFT::setTextSize(uint8_t s)
{
  setTextSize(s, s);
}

void GFX_TFT::setTextSize(uint8_t s_x, uint8_t s_y)
{
  textsize_x = (s_x>0) ? s_x : 1;
  textsize_y = (s_y>0) ? s_y : 1;
}

void GFX_TFT::setFont(const GFXfont *f)
{
  if(f)
  {            // Font struct pointer passed in?
    if( !gfxFont)
    { // And no current font struct?
      // Switching from classic to new font behavior.
      // Move cursor pos down 6 pixels so it's on baseline.
      cursor_y += 6;
    }
  } else if(gfxFont)
  { // NULL passed.  Current font struct defined?
    // Switching from new to classic font behavior.
    // Move cursor pos up 6 pixels so it's at top-left of char.
    cursor_y -= 6;
  }
  gfxFont = (GFXfont *) f;
}


/*
 * Графические функции
 */

void GFX_TFT::fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color) // todo Переделано
{
  drawFillFastRect(x, y, w, h, color);
}

void GFX_TFT::drawTriangle( int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color )
{
  drawLine( x0, y0, x1, y1, color );
  drawLine( x1, y1, x2, y2, color );
  drawLine( x2, y2, x0, y0, color );
}

void GFX_TFT::fillTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color)
{

  int16_t a, b, y, last;

  // Sort coordinates by Y order (y2 >= y1 >= y0)
  if(y0>y1)
  {
    _swap_int16_t(y0, y1);
    _swap_int16_t(x0, x1);
  }
  if(y1>y2)
  {
    _swap_int16_t(y2, y1);
    _swap_int16_t(x2, x1);
  }
  if(y0>y1)
  {
    _swap_int16_t(y0, y1);
    _swap_int16_t(x0, x1);
  }

  if(y0==y2)
  { // Handle awkward all-on-same-line case as its own thing
    a = b = x0;
    if(x1<a) a = x1;
    else if(x1>b) b = x1;
    if(x2<a) a = x2;
    else if(x2>b) b = x2;
    drawFastHLine(a, y0, b-a+1, color);
    return;
  }

  int16_t dx01 = x1-x0, dy01 = y1-y0, dx02 = x2-x0, dy02 = y2-y0, dx12 = x2-x1, dy12 = y2-y1;
  int32_t sa = 0, sb = 0;

  // For upper part of triangle, find scanline crossings for segments
  // 0-1 and 0-2.  If y1=y2 (flat-bottomed triangle), the scanline y1
  // is included here (and second loop will be skipped, avoiding a /0
  // error there), otherwise scanline y1 is skipped here and handled
  // in the second loop...which also avoids a /0 error here if y0=y1
  // (flat-topped triangle).
  if(y1==y2) last = y1;   // Include y1 scanline
  else last = y1-1; // Skip it

  for(y = y0; y<=last; y++)
  {
    a = x0+sa/dy01;
    b = x0+sb/dy02;
    sa += dx01;
    sb += dx02;
    if(a>b) _swap_int16_t(a, b);
    drawFastHLine(a, y, b-a+1, color);
  }


  // For lower part of triangle, find scanline crossings for segments
  // 0-2 and 1-2.  This loop is skipped if y1=y2.
  sa = (int32_t) dx12*(y-y1);
  sb = (int32_t) dx02*(y-y0);
  for(; y<=y2; y++)
  {
    a = x1+sa/dy12;
    b = x0+sb/dy02;
    sa += dx12;
    sb += dx02;
    if(a>b) _swap_int16_t(a, b);
    drawFastHLine(a, y, b-a+1, color);
  }
}



void GFX_TFT::fillRoundRect(int16_t x, int16_t y, int16_t w, int16_t h, int16_t r, uint16_t color)
{
  int16_t max_radius = ((w<h) ? w : h)/2; // 1/2 minor axis
  if(r>max_radius) r = max_radius;
  // smarter version
  fillRect(x+r, y, w-2*r, h, color);
  // draw four corners
  fillCircleHelper(x+w-r-1, y+r, r, 1, h-2*r-1, color);
  fillCircleHelper(x+r, y+r, r, 2, h-2*r-1, color);
}



void GFX_TFT::drawRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color)
{
  drawFastHLine(x, y, w, color);
  drawFastHLine(x, y+h-1, w, color);
  drawFastVLine(x, y, h, color);
  drawFastVLine(x+w-1, y, h, color);
}



void GFX_TFT::drawCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color)
{
  int16_t f = 1-r;
  int16_t ddF_x = 1;
  int16_t ddF_y = -2*r;
  int16_t x = 0;
  int16_t y = r;

  writePixel(x0, y0+r, color);
  writePixel(x0, y0-r, color);
  writePixel(x0+r, y0, color);
  writePixel(x0-r, y0, color);

  while(x<y)
  {
    if(f>=0)
    {
      y--;
      ddF_y += 2;
      f += ddF_y;
    }
    x++;
    ddF_x += 2;
    f += ddF_x;

    writePixel(x0+x, y0+y, color);
    writePixel(x0-x, y0+y, color);
    writePixel(x0+x, y0-y, color);
    writePixel(x0-x, y0-y, color);
    writePixel(x0+y, y0+x, color);
    writePixel(x0-y, y0+x, color);
    writePixel(x0+y, y0-x, color);
    writePixel(x0-y, y0-x, color);
  }
}


void GFX_TFT::drawCircleHelper(int16_t x0, int16_t y0, int16_t r, uint8_t cornername, uint16_t color)
{
  int16_t f = 1-r;
  int16_t ddF_x = 1;
  int16_t ddF_y = -2*r;
  int16_t x = 0;
  int16_t y = r;

  while(x<y)
  {
    if(f>=0)
    {
      y--;
      ddF_y += 2;
      f += ddF_y;
    }
    x++;
    ddF_x += 2;
    f += ddF_x;
    if(cornername&0x4)
    {
      writePixel(x0+x, y0+y, color);
      writePixel(x0+y, y0+x, color);
    }
    if(cornername&0x2)
    {
      writePixel(x0+x, y0-y, color);
      writePixel(x0+y, y0-x, color);
    }
    if(cornername&0x8)
    {
      writePixel(x0-y, y0+x, color);
      writePixel(x0-x, y0+y, color);
    }
    if(cornername&0x1)
    {
      writePixel(x0-y, y0-x, color);
      writePixel(x0-x, y0-y, color);
    }
  }
}



void GFX_TFT::drawRoundRect(int16_t x, int16_t y, int16_t w, int16_t h, int16_t r, uint16_t color)
{
  int16_t max_radius = ((w<h) ? w : h)/2; // 1/2 minor axis
  if(r>max_radius) r = max_radius;
  // smarter version
  drawFastHLine(x+r, y, w-2*r, color); // Top
  drawFastHLine(x+r, y+h-1, w-2*r, color); // Bottom
  drawFastVLine(x, y+r, h-2*r, color); // Left
  drawFastVLine(x+w-1, y+r, h-2*r, color); // Right
  // draw four corners
  drawCircleHelper(x+r, y+r, r, 1, color);
  drawCircleHelper(x+w-r-1, y+r, r, 2, color);
  drawCircleHelper(x+w-r-1, y+h-r-1, r, 4, color);
  drawCircleHelper(x+r, y+h-r-1, r, 8, color);
}



void GFX_TFT::fillCircle( int16_t x0, int16_t y0, int16_t r, uint16_t color )
{
  drawFastVLine( x0, y0 - r, 2 * r + 1, color );
  fillCircleHelper( x0, y0, r, 3, 0, color );
}



void GFX_TFT::fillCircleHelper(int16_t x0, int16_t y0, int16_t r, uint8_t corners, int16_t delta, uint16_t color)
{

  int16_t f = 1-r;
  int16_t ddF_x = 1;
  int16_t ddF_y = -2*r;
  int16_t x = 0;
  int16_t y = r;
  int16_t px = x;
  int16_t py = y;

  delta++; // Avoid some +1's in the loop

  while(x<y)
  {
    if(f>=0)
    {
      y--;
      ddF_y += 2;
      f += ddF_y;
    }
    x++;
    ddF_x += 2;
    f += ddF_x;
    // These checks avoid double-drawing certain lines, important
    // for the SSD1306 library which has an INVERT drawing mode.
    if(x<(y+1))
    {
      if(corners&1) drawFastVLine(x0+x, y0-y, 2*y+delta, color);
      if(corners&2) drawFastVLine(x0-x, y0-y, 2*y+delta, color);
    }
    if(y!=py)
    {
      if(corners&1) drawFastVLine(x0+py, y0-px, 2*px+delta, color);
      if(corners&2) drawFastVLine(x0-py, y0-px, 2*px+delta, color);
      py = y;
    }
    px = x;
  }
}



void GFX_TFT::drawFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color) // todo Правлено
{
  drawFillFastRect(x, y, 1, h, color);
}

void GFX_TFT::drawFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color) // todo Правлено
{
  if((x>=_width)||(y<0)||(y>=_height)) return;
  uint16_t x2 = x+w-1;
  if(x2<0) return;

  // Clip left/right
  if(x<0)
  {
    x = 0;
    w = x2+1;
  }
  if(x2>=_width) w = _width-x;

  drawFillFastRect(x, y, w, 1, color);
}



void GFX_TFT::drawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color)
{
  int16_t steep = abs(y1-y0)>abs(x1-x0);
  if(steep)
  {
    _swap_int16_t(x0, y0);
    _swap_int16_t(x1, y1);
  }

  if(x0>x1)
  {
    _swap_int16_t(x0, x1);
    _swap_int16_t(y0, y1);
  }

  int16_t dx, dy;
  dx = x1-x0;
  dy = abs(y1-y0);

  int16_t err = dx/2;
  int16_t ystep;

  if(y0<y1)
  {
    ystep = 1;
  } else
  {
    ystep = -1;
  }

  for(; x0<=x1; x0++)
  {
    if(steep)
    {
      writePixel(y0, x0, color);
    } else
    {
      writePixel(x0, y0, color);
    }
    err -= dy;
    if(err<0)
    {
      y0 += ystep;
      err += dx;
    }
  }
}

void GFX_TFT::setRotation(uint8_t x)
{
  rotation = (x & 3);
  switch (rotation)
  {
    case 0:
    case 2:
      _width = WIDTH;
      _height = HEIGHT;
      break;
    case 1:
    case 3:
      _width = HEIGHT;
      _height = WIDTH;
      break;
  }
}



/*
 * Установочные функции
 */

void GFX_TFT::fillScreen(uint16_t color)
{
  fillRect(0, 0, _width, _height, color);
  bgcolor = color;
}


void GFX_TFT::CursorXY(uint16_t x, uint16_t y)
{

}


void GFX_TFT::setCursor(uint16_t Xpos, uint16_t Ypos)
{
  cursor_x = Xpos;
  cursor_y = Ypos;
  CursorXY(Xpos, Ypos);
}



/*
 * Медленный вывод графики
 */
void GFX_TFT::drawRGBBitmap(int16_t x, int16_t y, const uint16_t bitmap[], int16_t w, int16_t h)
{
    for (int16_t j = 0; j < h; j++, y++)
  {
    for (int16_t i = 0; i < w; i++)
    {
      writePixel(x + i, y, pgm_read_word(&bitmap[j * w + i]));
    }
  }
}

void GFX_TFT::drawRGBBitmap(int16_t x, int16_t y, uint16_t *bitmap, int16_t w, int16_t h)
{
  for (int16_t j = 0; j < h; j++, y++)
  {
    for (int16_t i = 0; i < w; i++)
    {
      writePixel(x + i, y, bitmap[j * w + i]);
    }
  }
}

void GFX_TFT::drawRGBBitmap(int16_t x, int16_t y, const uint16_t bitmap[], const uint8_t mask[], int16_t w, int16_t h)
{
  int16_t bw = (w + 7) / 8; // Bitmask scanline pad = whole byte
  uint8_t byte = 0;
  for (int16_t j = 0; j < h; j++, y++)
  {
    for (int16_t i = 0; i < w; i++)
    {
      if(i & 7) byte <<= 1;
      else byte = pgm_read_byte(&mask[j * bw + i / 8]);
      if(byte & 0x80)
      {
        writePixel(x + i, y, pgm_read_word(&bitmap[j * w + i]));
      }
    }
  }
}

void GFX_TFT::drawRGBBitmap(int16_t x, int16_t y, uint16_t *bitmap, uint8_t *mask, int16_t w, int16_t h)
{
  int16_t bw = (w + 7) / 8; // Bitmask scanline pad = whole byte
  uint8_t byte = 0;
  for (int16_t j = 0; j < h; j++, y++)
  {
    for (int16_t i = 0; i < w; i++)
    {
      if(i & 7) byte <<= 1;
      else byte = mask[j * bw + i / 8];
      if(byte & 0x80)
      {
        writePixel(x + i, y, bitmap[j * w + i]);
      }
    }
  }
}

/*
void GFX_TFT::drawRGBBitmap(uint16_t x, uint16_t y, const uint16_t *bitmap, uint16_t w, uint16_t h)
{
  uint32_t index = 0;
  uint32_t totalpoint;

  totalpoint = (w+x)*(h+y);

  setWindow(x, y, x+w, y+h);

  LCD->LCD_REG = CMD_RAM_WRITE;

  for(index = 0; index < totalpoint; index++)
  {
    LCD->LCD_RAM = *bitmap;
    bitmap++;
  }
}
*/

/*
 * Очень страшная функция
 */

void GFX_TFT::setWindow(uint16_t sx,uint16_t sy,uint16_t width,uint16_t height)
{

}

size_t GFX_TFT::write(uint8_t c)
{
  if( !gfxFont)
  { // 'Classic' built-in font

    if(c=='\n')
    {                        // Newline?
      cursor_x = 0;                     // Reset x to zero,
      cursor_y += textsize_y*8;        // advance y one line
    } else if(c!='\r')
    {                 // Ignore carriage returns
      if(wrap&&((cursor_x+textsize_x*6)>_width))
      { // Off right?
        cursor_x = 0;                 // Reset x to zero,
        cursor_y += textsize_y*8;    // advance y one line
      }
      drawChar(cursor_x, cursor_y, c, textcolor, textbgcolor, textsize_x, textsize_y);
      cursor_x += textsize_x*6;          // Advance x one char
    }

  } else
  { // Custom font

    if(c=='\n')
    {
      cursor_x = 0;
      cursor_y += (int16_t) textsize_y*(uint8_t) pgm_read_byte( &gfxFont->yAdvance);
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
          if(wrap&&((cursor_x+textsize_x*(xo+w))>_width))
          {
            cursor_x = 0;
            cursor_y += (int16_t) textsize_y*(uint8_t) pgm_read_byte( &gfxFont->yAdvance);
          }
          drawChar(cursor_x, cursor_y, c, textcolor, textbgcolor, textsize_x, textsize_y);
        }
        cursor_x += (uint8_t) pgm_read_byte( &glyph->xAdvance)*(int16_t) textsize_x;
      }
    }

  }
  return 1;
}



void GFX_TFT::drawChar(int16_t x, int16_t y, unsigned char c, uint16_t color, uint16_t bg, uint8_t size)
{
  drawChar(x, y, c, color, bg, size, size);
}



void GFX_TFT::drawChar(int16_t x, int16_t y, unsigned char c, uint16_t color, uint16_t bg, uint8_t size_x, uint8_t size_y)
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
          if(size_x==1&&size_y==1) writePixel(x+i, y+j, color);
          else fillRect(x+i*size_x, y+j*size_y, size_x, size_y, color);
        } else if(bg!=color)
        {
          if(size_x==1&&size_y==1) writePixel(x+i, y+j, bg);
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
            writePixel(x+xo+xx, y+yo+yy, color);
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

