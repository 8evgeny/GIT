//****************************************************************************** 
//  Секция include: здесь подключается заголовочный файл к модулю 
//****************************************************************************** 
#include <GFX_Button.h>


#ifndef min
#define min(a,b) (((a) < (b)) ? (a) : (b))
#endif

/**************************************************************************/
/*!
   @brief    Create a simple drawn button UI element
*/
/**************************************************************************/
GFX_Button::GFX_Button(void)
{
  _gfx = 0;
}

/**************************************************************************/
/*!
   @brief    Initialize button with our desired color/size/settings
   @param    gfx     Pointer to our display so we can draw to it!
   @param    x       The X coordinate of the center of the button
   @param    y       The Y coordinate of the center of the button
   @param    w       Width of the buttton
   @param    h       Height of the buttton
   @param    outline  Color of the outline (16-bit 5-6-5 standard)
   @param    fill  Color of the button fill (16-bit 5-6-5 standard)
   @param    textcolor  Color of the button label (16-bit 5-6-5 standard)
   @param    label  Ascii string of the text inside the button
   @param    textsize The font magnification of the label text
*/
/**************************************************************************/
// Classic initButton() function: pass center & size
void GFX_Button::initButton(GFX_TFT *gfx, int16_t x, int16_t y, uint16_t w, uint16_t h, uint16_t outline, uint16_t fill,
                            uint16_t textcolor, const char *label, uint8_t textsize)
{
  // Tweak arguments and pass to the newer initButtonUL() function...
  initButtonUL(gfx, x - (w / 2), y - (h / 2), w, h, outline, fill, textcolor, label, textsize);
}

/**************************************************************************/
/*!
   @brief    Initialize button with our desired color/size/settings
   @param    gfx     Pointer to our display so we can draw to it!
   @param    x       The X coordinate of the center of the button
   @param    y       The Y coordinate of the center of the button
   @param    w       Width of the buttton
   @param    h       Height of the buttton
   @param    outline  Color of the outline (16-bit 5-6-5 standard)
   @param    fill  Color of the button fill (16-bit 5-6-5 standard)
   @param    textcolor  Color of the button label (16-bit 5-6-5 standard)
   @param    label  Ascii string of the text inside the button
   @param    textsize_x The font magnification in X-axis of the label text
   @param    textsize_y The font magnification in Y-axis of the label text
*/
/**************************************************************************/
// Classic initButton() function: pass center & size
void GFX_Button::initButton(GFX_TFT *gfx, int16_t x, int16_t y, uint16_t w, uint16_t h, uint16_t outline, uint16_t fill,
                            uint16_t textcolor, const char *label, uint8_t textsize_x, uint8_t textsize_y)
{
  // Tweak arguments and pass to the newer initButtonUL() function...
  initButtonUL(gfx, x - (w / 2), y - (h / 2), w, h, outline, fill, textcolor, label, textsize_x, textsize_y);
}

/**************************************************************************/
/*!
   @brief    Initialize button with our desired color/size/settings, with upper-left coordinates
   @param    gfx     Pointer to our display so we can draw to it!
   @param    x1       The X coordinate of the Upper-Left corner of the button
   @param    y1       The Y coordinate of the Upper-Left corner of the button
   @param    w       Width of the buttton
   @param    h       Height of the buttton
   @param    outline  Color of the outline (16-bit 5-6-5 standard)
   @param    fill  Color of the button fill (16-bit 5-6-5 standard)
   @param    textcolor  Color of the button label (16-bit 5-6-5 standard)
   @param    label  Ascii string of the text inside the button
   @param    textsize The font magnification of the label text
*/
/**************************************************************************/
void GFX_Button::initButtonUL(GFX_TFT *gfx, int16_t x1, int16_t y1, uint16_t w, uint16_t h, uint16_t outline, uint16_t fill,
                              uint16_t textcolor, const char *label, uint8_t textsize)
{
  initButtonUL(gfx, x1, y1, w, h, outline, fill, textcolor, label, textsize, textsize);
}

/**************************************************************************/
/*!
   @brief    Initialize button with our desired color/size/settings, with upper-left coordinates
   @param    gfx     Pointer to our display so we can draw to it!
   @param    x1       The X coordinate of the Upper-Left corner of the button
   @param    y1       The Y coordinate of the Upper-Left corner of the button
   @param    w       Width of the buttton
   @param    h       Height of the buttton
   @param    outline  Color of the outline (16-bit 5-6-5 standard)
   @param    fill  Color of the button fill (16-bit 5-6-5 standard)
   @param    textcolor  Color of the button label (16-bit 5-6-5 standard)
   @param    label  Ascii string of the text inside the button
   @param    textsize_x The font magnification in X-axis of the label text
   @param    textsize_y The font magnification in Y-axis of the label text
*/
/**************************************************************************/
void GFX_Button::initButtonUL(GFX_TFT *gfx, int16_t x1, int16_t y1, uint16_t w, uint16_t h, uint16_t outline, uint16_t fill,
                              uint16_t textcolor, const char *label, uint8_t textsize_x, uint8_t textsize_y)
{
  _x1 = x1;
  _y1 = y1;
  _w = w;
  _h = h;
  _outlinecolor = outline;
  _fillcolor = fill;
  _textcolor = textcolor;
  _textsize_x = textsize_x;
  _textsize_y = textsize_y;
  _gfx = gfx;
//  _label        = label;
  strncpy(_label, label, 9);
}

/**************************************************************************/
/*!
   @brief    Draw the button on the screen
   @param    inverted Whether to draw with fill/text swapped to indicate 'pressed'
*/
/**************************************************************************/
void GFX_Button::drawButton(bool inverted)
{
  uint16_t fill, outline, text;
  int16_t PosX, PosY;
  uint16_t ColorFont, ColorBG;
  uint16_t Size_X = _gfx->getTextWidth();
  uint16_t Size_Y = _gfx->getTextHeight();

  PosX = _gfx->getCursorX();
  PosY = _gfx->getCursorY();
  ColorFont = _gfx->getColorFont();
  ColorBG = _gfx->getColorBG();

  if(!inverted)
  {
    fill = _fillcolor;
    outline = _outlinecolor;
    text = _textcolor;
  }
  else
  {
    fill = _textcolor;
    outline = _outlinecolor;
    text = _fillcolor;
  }

  uint8_t r = min(_w, _h) / 4; // Corner radius
  _gfx->fillRoundRect(_x1, _y1, _w, _h, r, fill);
  _gfx->drawRoundRect(_x1, _y1, _w, _h, r, outline);

  _gfx->setCursor(_x1 + (_w / 2) - (strlen(_label) * 3 * _textsize_x), _y1 + (_h / 2) - (4 * _textsize_y));
  _gfx->setTextColor(text);
  _gfx->setTextSize(_textsize_x, _textsize_y);
  _gfx->print(_label);
  _gfx->setCursor(PosX, PosY);
  _gfx->setTextColor(ColorFont, ColorBG);
  _gfx->setTextSize(Size_X, Size_Y);
}

/**************************************************************************/
/*!
    @brief    Helper to let us know if a coordinate is within the bounds of the button
    @param    x       The X coordinate to check
    @param    y       The Y coordinate to check
    @returns  True if within button graphics outline
*/
/**************************************************************************/
bool GFX_Button::contains(int16_t x, int16_t y)
{
  return ((x >= _x1) && (x < (int16_t) (_x1 + _w)) && (y >= _y1) && (y < (int16_t) (_y1 + _h)));
}

/**************************************************************************/
/*!
   @brief    Query whether the button was pressed since we last checked state
   @returns  True if was not-pressed before, now is.
*/
/**************************************************************************/
bool GFX_Button::justPressed() { return (currstate && !laststate); }

/**************************************************************************/
/*!
   @brief    Query whether the button was released since we last checked state
   @returns  True if was pressed before, now is not.
*/
/**************************************************************************/
bool GFX_Button::justReleased() { return (!currstate && laststate); }

// -------------------------------------------------------------------------

// GFXcanvas1, GFXcanvas8 and GFXcanvas16 (currently a WIP, don't get too
// comfy with the implementation) provide 1-, 8- and 16-bit offscreen
// canvases, the address of which can be passed to drawBitmap() or
// pushColors() (the latter appears only in a couple of GFX-subclassed TFT
// libraries at this time).  This is here mostly to help with the recently-
// added proportionally-spaced fonts; adds a way to refresh a section of the
// screen without a massive flickering clear-and-redraw...but maybe you'll
// find other uses too.  VERY RAM-intensive, since the buffer is in MCU
// memory and not the display driver...GXFcanvas1 might be minimally useful
// on an Uno-class board, but this and the others are much more likely to
// require at least a Mega or various recent ARM-type boards (recommended,
// as the text+bitmap draw can be pokey).  GFXcanvas1 requires 1 bit per
// pixel (rounded up to nearest byte per scanline), GFXcanvas8 is 1 byte
// per pixel (no scanline pad), and GFXcanvas16 uses 2 bytes per pixel (no
// scanline pad).
// NOT EXTENSIVELY TESTED YET.  MAY CONTAIN WORST BUGS KNOWN TO HUMANKIND.

//****************************************************************************** 
//  ENF OF FILE 
//****************************************************************************** 

