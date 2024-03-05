/*
 * File:   main.h
 * Author: User
 *
 * Created on 18 сентября 2019 г., 13:05
 */

// Блокируем повторное включение этого модуля 
#ifndef GFX_BUTTON_H_
#define	GFX_BUTTON_H_

  //****************************************************************************** 
  //  Секция include: здесь подключаются заголовочные файлы используемых модулей 
  //****************************************************************************** 
#include "GFX_TFT.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define Inverted true

class GFX_Button
{

  public:
    GFX_Button(void);
    // "Классический" initButton() отрисовка относительно координат центра и размера
    void initButton(GFX_TFT *gfx, int16_t x, int16_t y, uint16_t w, uint16_t h, uint16_t outline, uint16_t fill,
        uint16_t textcolor, const char *label, uint8_t textsize);
    void initButton(GFX_TFT *gfx, int16_t x, int16_t y, uint16_t w, uint16_t h, uint16_t outline, uint16_t fill,
        uint16_t textcolor, const char *label, uint8_t textsize_x, uint8_t textsize_y);
    // New/alt initButton() uses upper-left corner & size
    void initButtonUL(GFX_TFT *gfx, int16_t x1, int16_t y1, uint16_t w, uint16_t h, uint16_t outline, uint16_t fill,
        uint16_t textcolor, const char *label, uint8_t textsize);
    void initButtonUL(GFX_TFT *gfx, int16_t x1, int16_t y1, uint16_t w, uint16_t h, uint16_t outline, uint16_t fill,
        uint16_t textcolor, const char *label, uint8_t textsize_x, uint8_t textsize_y);
    void drawButton(bool inverted = false);
    bool contains(int16_t x, int16_t y);

    /**********************************************************************/
    /*!
     @brief    Sets button state, should be done by some touch function
     @param    p  True for pressed, false for not.
     */
    /**********************************************************************/
    void press(bool p)
    {
      laststate = currstate;
      currstate = p;
    }

    bool justPressed();
    bool justReleased();

    /**********************************************************************/
    /*!
     @brief    Query whether the button is currently pressed
     @returns  True if pressed
     */
    /**********************************************************************/
    bool isPressed(void){return currstate;};

  private:
    GFX_TFT *_gfx;
    int16_t _x1, _y1; // Coordinates of top-left corner
    uint16_t _w, _h;
    uint8_t _textsize_x;
    uint8_t _textsize_y;
    uint16_t _outlinecolor, _fillcolor, _textcolor;
    char _label[10];

    bool currstate, laststate;
};

// Закрывающий #endif к блокировке повторного включения 
#endif	/* MAIN_H */

//****************************************************************************** 
//  ENF OF FILE 
//****************************************************************************** /* 
