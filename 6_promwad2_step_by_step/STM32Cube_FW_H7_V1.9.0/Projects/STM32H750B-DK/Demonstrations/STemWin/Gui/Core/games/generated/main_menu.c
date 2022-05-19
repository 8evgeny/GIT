/*********************************************************************
*                SEGGER Microcontroller GmbH & Co. KG                *
*        Solutions for real time microcontroller applications        *
*                           www.segger.com                           *
**********************************************************************
*                                                                    *
* C-file generated by                                                *
*                                                                    *
*        Bitmap Converter for emWin V5.28.                           *
*        Compiled Jan 30 2015, 16:40:04                              *
*                                                                    *
*        (c) 1998 - 2015 Segger Microcontroller GmbH & Co. KG        *
*                                                                    *
**********************************************************************
*                                                                    *
* Source file: main_menu                                             *
* Dimensions:  48 * 32                                               *
* NumColors:   65536 colors + 8 bit alpha channel                    *
*                                                                    *
**********************************************************************
*/

#include <stdlib.h>

#include "GUI.h"

#ifndef GUI_CONST_STORAGE
  #define GUI_CONST_STORAGE const
#endif

extern GUI_CONST_STORAGE GUI_BITMAP bmmain_menu;

#if defined ( __ICCARM__ )
#pragma location="ExtQSPIFlashSection" 
#else
__attribute__((section(".ExtQSPIFlashSection")))  
#endif
static GUI_CONST_STORAGE unsigned long _acHomeButton[] = {
  0x81,0xFF,0xFF, 0x66,0xFF,0xFF, 0x66,0xFF,0xFF, 0x66,0xFF,0xFF, 0x66,0xFF,0xFF, 0x66,0xFF,0xFF, 0x66,0xFF,0xFF, 0x66,0xFF,0xFF, 0x66,0xFF,0xFF, 0x66,0xFF,0xFF, 0x66,0xFF,0xFF, 0x66,0xFF,0xFF, 0x66,0xFF,0xFF, 0x66,0xFF,0xFF, 0x66,0xFF,0xFF, 
        0x66,0xFF,0xFF, 0x66,0xFF,0xFF, 0x66,0xFF,0xFF, 0x66,0xFF,0xFF, 0x66,0xFF,0xFF, 0x66,0xFF,0xFF, 0x81,0xFF,0xFF,
  0xC7,0xFF,0xFF, 0xAD,0xFF,0xFF, 0xAD,0xFF,0xFF, 0xAD,0xFF,0xFF, 0xAD,0xFF,0xFF, 0xAD,0xFF,0xFF, 0xAD,0xFF,0xFF, 0xAD,0xFF,0xFF, 0xAD,0xFF,0xFF, 0xAD,0xFF,0xFF, 0xAD,0xFF,0xFF, 0xAD,0xFF,0xFF, 0xAD,0xFF,0xFF, 0xAD,0xFF,0xFF, 0xAD,0xFF,0xFF, 
        0xAD,0xFF,0xFF, 0xAD,0xFF,0xFF, 0xAD,0xFF,0xFF, 0xAD,0xFF,0xFF, 0xAD,0xFF,0xFF, 0xAD,0xFF,0xFF, 0xC7,0xFF,0xFF,
  0xFF,0x00,0x00, 0xFF,0x00,0x00, 0xFF,0x00,0x00, 0xFF,0x00,0x00, 0xFF,0x00,0x00, 0xFF,0x00,0x00, 0xFF,0x00,0x00, 0xFF,0x00,0x00, 0xFF,0x00,0x00, 0xFF,0x00,0x00, 0xFF,0x00,0x00, 0xFF,0x00,0x00, 0xFF,0x00,0x00, 0xFF,0x00,0x00, 0xFF,0x00,0x00, 
        0xFF,0x00,0x00, 0xFF,0x00,0x00, 0xFF,0x00,0x00, 0xFF,0x00,0x00, 0xFF,0x00,0x00, 0xFF,0x00,0x00, 0xFF,0x00,0x00,
  0xFF,0x00,0x00, 0xFF,0x00,0x00, 0xFF,0x00,0x00, 0xFF,0x00,0x00, 0xFF,0x00,0x00, 0xFF,0x00,0x00, 0xFF,0x00,0x00, 0xFF,0x00,0x00, 0xFF,0x00,0x00, 0xFF,0x00,0x00, 0xFF,0x00,0x00, 0xFF,0x00,0x00, 0xFF,0x00,0x00, 0xFF,0x00,0x00, 0xFF,0x00,0x00, 
        0xFF,0x00,0x00, 0xFF,0x00,0x00, 0xFF,0x00,0x00, 0xFF,0x00,0x00, 0xFF,0x00,0x00, 0xFF,0x00,0x00, 0xFF,0x00,0x00,
  0xFF,0x00,0x00, 0xFF,0x00,0x00, 0xFF,0x00,0x00, 0xFF,0x00,0x00, 0xFF,0x00,0x00, 0xFF,0x00,0x00, 0xFF,0x00,0x00, 0xFF,0x00,0x00, 0xFF,0x00,0x00, 0xFF,0x00,0x00, 0xFF,0x00,0x00, 0xFF,0x00,0x00, 0xFF,0x00,0x00, 0xFF,0x00,0x00, 0xFF,0x00,0x00, 
        0xFF,0x00,0x00, 0xFF,0x00,0x00, 0xFF,0x00,0x00, 0xFF,0x00,0x00, 0xFF,0x00,0x00, 0xFF,0x00,0x00, 0xFF,0x00,0x00,
  0xFF,0x00,0x00, 0xFF,0x00,0x00, 0xFF,0x00,0x00, 0xFF,0x00,0x00, 0xFF,0x00,0x00, 0xFF,0x00,0x00, 0xFF,0x00,0x00, 0xFF,0x00,0x00, 0xFF,0x00,0x00, 0xFF,0x00,0x00, 0xFF,0x00,0x00, 0xFF,0x00,0x00, 0xFF,0x00,0x00, 0xFF,0x00,0x00, 0xFF,0x00,0x00, 
        0xFF,0x00,0x00, 0xFF,0x00,0x00, 0xFF,0x00,0x00, 0xFF,0x00,0x00, 0xFF,0x00,0x00, 0xFF,0x00,0x00, 0xFF,0x00,0x00,
  0xFF,0x00,0x00, 0xFF,0x00,0x00, 0xFF,0x00,0x00, 0xFF,0x00,0x00, 0xFF,0x00,0x00, 0xFF,0x00,0x00, 0xFF,0x00,0x00, 0xFF,0x00,0x00, 0xFF,0x00,0x00, 0xFF,0x00,0x00, 0xFF,0x00,0x00, 0xFF,0x00,0x00, 0xFF,0x00,0x00, 0xFF,0x00,0x00, 0xFF,0x00,0x00, 
        0xFF,0x00,0x00, 0xFF,0x00,0x00, 0xFF,0x00,0x00, 0xFF,0x00,0x00, 0xFF,0x00,0x00, 0xFF,0x00,0x00, 0xFF,0x00,0x00,
  0xB6,0xFF,0xFF, 0x99,0xFF,0xFF, 0x99,0xFF,0xFF, 0x99,0xFF,0xFF, 0x99,0xFF,0xFF, 0x99,0xFF,0xFF, 0x99,0xFF,0xFF, 0x99,0xFF,0xFF, 0x99,0xFF,0xFF, 0x99,0xFF,0xFF, 0x99,0xFF,0xFF, 0x99,0xFF,0xFF, 0x99,0xFF,0xFF, 0x99,0xFF,0xFF, 0x99,0xFF,0xFF, 
        0x99,0xFF,0xFF, 0x99,0xFF,0xFF, 0x99,0xFF,0xFF, 0x99,0xFF,0xFF, 0x99,0xFF,0xFF, 0x99,0xFF,0xFF, 0xB6,0xFF,0xFF,
  0x92,0xFF,0xFF, 0x7A,0xFF,0xFF, 0x7A,0xFF,0xFF, 0x7A,0xFF,0xFF, 0x7A,0xFF,0xFF, 0x7A,0xFF,0xFF, 0x7A,0xFF,0xFF, 0x7A,0xFF,0xFF, 0x7A,0xFF,0xFF, 0x7A,0xFF,0xFF, 0x7A,0xFF,0xFF, 0x7A,0xFF,0xFF, 0x7A,0xFF,0xFF, 0x7A,0xFF,0xFF, 0x7A,0xFF,0xFF, 
        0x7A,0xFF,0xFF, 0x7A,0xFF,0xFF, 0x7A,0xFF,0xFF, 0x7A,0xFF,0xFF, 0x7A,0xFF,0xFF, 0x7A,0xFF,0xFF, 0x92,0xFF,0xFF,
  0xFF,0x00,0x00, 0xFF,0x00,0x00, 0xFF,0x00,0x00, 0xFF,0x00,0x00, 0xFF,0x00,0x00, 0xFF,0x00,0x00, 0xFF,0x00,0x00, 0xFF,0x00,0x00, 0xFF,0x00,0x00, 0xFF,0x00,0x00, 0xFF,0x00,0x00, 0xFF,0x00,0x00, 0xFF,0x00,0x00, 0xFF,0x00,0x00, 0xFF,0x00,0x00, 
        0xFF,0x00,0x00, 0xFF,0x00,0x00, 0xFF,0x00,0x00, 0xFF,0x00,0x00, 0xFF,0x00,0x00, 0xFF,0x00,0x00, 0xFF,0x00,0x00,
  0xFF,0x00,0x00, 0xFF,0x00,0x00, 0xFF,0x00,0x00, 0xFF,0x00,0x00, 0xFF,0x00,0x00, 0xFF,0x00,0x00, 0xFF,0x00,0x00, 0xFF,0x00,0x00, 0xFF,0x00,0x00, 0xFF,0x00,0x00, 0xFF,0x00,0x00, 0xFF,0x00,0x00, 0xFF,0x00,0x00, 0xFF,0x00,0x00, 0xFF,0x00,0x00, 
        0xFF,0x00,0x00, 0xFF,0x00,0x00, 0xFF,0x00,0x00, 0xFF,0x00,0x00, 0xFF,0x00,0x00, 0xFF,0x00,0x00, 0xFF,0x00,0x00,
  0xFF,0x00,0x00, 0xFF,0x00,0x00, 0xFF,0x00,0x00, 0xFF,0x00,0x00, 0xFF,0x00,0x00, 0xFF,0x00,0x00, 0xFF,0x00,0x00, 0xFF,0x00,0x00, 0xFF,0x00,0x00, 0xFF,0x00,0x00, 0xFF,0x00,0x00, 0xFF,0x00,0x00, 0xFF,0x00,0x00, 0xFF,0x00,0x00, 0xFF,0x00,0x00, 
        0xFF,0x00,0x00, 0xFF,0x00,0x00, 0xFF,0x00,0x00, 0xFF,0x00,0x00, 0xFF,0x00,0x00, 0xFF,0x00,0x00, 0xFF,0x00,0x00,
  0xFF,0x00,0x00, 0xFF,0x00,0x00, 0xFF,0x00,0x00, 0xFF,0x00,0x00, 0xFF,0x00,0x00, 0xFF,0x00,0x00, 0xFF,0x00,0x00, 0xFF,0x00,0x00, 0xFF,0x00,0x00, 0xFF,0x00,0x00, 0xFF,0x00,0x00, 0xFF,0x00,0x00, 0xFF,0x00,0x00, 0xFF,0x00,0x00, 0xFF,0x00,0x00, 
        0xFF,0x00,0x00, 0xFF,0x00,0x00, 0xFF,0x00,0x00, 0xFF,0x00,0x00, 0xFF,0x00,0x00, 0xFF,0x00,0x00, 0xFF,0x00,0x00,
  0xFF,0x00,0x00, 0xFF,0x00,0x00, 0xFF,0x00,0x00, 0xFF,0x00,0x00, 0xFF,0x00,0x00, 0xFF,0x00,0x00, 0xFF,0x00,0x00, 0xFF,0x00,0x00, 0xFF,0x00,0x00, 0xFF,0x00,0x00, 0xFF,0x00,0x00, 0xFF,0x00,0x00, 0xFF,0x00,0x00, 0xFF,0x00,0x00, 0xFF,0x00,0x00, 
        0xFF,0x00,0x00, 0xFF,0x00,0x00, 0xFF,0x00,0x00, 0xFF,0x00,0x00, 0xFF,0x00,0x00, 0xFF,0x00,0x00, 0xFF,0x00,0x00,
  0xC6,0xFF,0xFF, 0xAD,0xFF,0xFF, 0xAD,0xFF,0xFF, 0xAD,0xFF,0xFF, 0xAD,0xFF,0xFF, 0xAD,0xFF,0xFF, 0xAD,0xFF,0xFF, 0xAD,0xFF,0xFF, 0xAD,0xFF,0xFF, 0xAD,0xFF,0xFF, 0xAD,0xFF,0xFF, 0xAD,0xFF,0xFF, 0xAD,0xFF,0xFF, 0xAD,0xFF,0xFF, 0xAD,0xFF,0xFF, 
        0xAD,0xFF,0xFF, 0xAD,0xFF,0xFF, 0xAD,0xFF,0xFF, 0xAD,0xFF,0xFF, 0xAD,0xFF,0xFF, 0xAD,0xFF,0xFF, 0xC6,0xFF,0xFF,
  0x81,0xFF,0xFF, 0x66,0xFF,0xFF, 0x66,0xFF,0xFF, 0x66,0xFF,0xFF, 0x66,0xFF,0xFF, 0x66,0xFF,0xFF, 0x66,0xFF,0xFF, 0x66,0xFF,0xFF, 0x66,0xFF,0xFF, 0x66,0xFF,0xFF, 0x66,0xFF,0xFF, 0x66,0xFF,0xFF, 0x66,0xFF,0xFF, 0x66,0xFF,0xFF, 0x66,0xFF,0xFF, 
        0x66,0xFF,0xFF, 0x66,0xFF,0xFF, 0x66,0xFF,0xFF, 0x66,0xFF,0xFF, 0x66,0xFF,0xFF, 0x66,0xFF,0xFF, 0x82,0xFF,0xFF
};

GUI_CONST_STORAGE GUI_BITMAP bmmain_menu = {
  22, // xSize
  16, // ySize
  66, // BytesPerLine
  24, // BitsPerPixel
  (unsigned char *)_acHomeButton,  // Pointer to picture data
  NULL,  // Pointer to palette
  GUI_DRAW_BMPAM565
};

/*************************** End of file ****************************/
