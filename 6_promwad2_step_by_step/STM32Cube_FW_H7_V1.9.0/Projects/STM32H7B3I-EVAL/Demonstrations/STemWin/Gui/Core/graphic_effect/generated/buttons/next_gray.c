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
* Source file: NextGray                                              *
* Dimensions:  40 * 42                                               *
* NumColors:   65536 colors + 8 bit alpha channel                    *
*                                                                    *
**********************************************************************
*/

#include <stdlib.h>

#include "GUI.h"

#ifndef GUI_CONST_STORAGE
  #define GUI_CONST_STORAGE const
#endif

extern GUI_CONST_STORAGE GUI_BITMAP bmNextGrayButton;

#if defined ( __ICCARM__ )
#pragma location="ExtQSPIFlashSection" 
#else
__attribute__((section(".ExtQSPIFlashSection")))  
#endif
static GUI_CONST_STORAGE unsigned char _acNextGray[] = {
  0xFF,0x00,0x00, 0xFF,0x00,0x00, 0xFF,0x00,0x00, 0xFF,0x00,0x00, 0xEE,0xFF,0xFF, 0x8B,0xFF,0xFF, 0x3C,0xFF,0xDF, 0x17,0xFF,0xFF, 0x01,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 
        0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 
        0x00,0xFF,0xFF, 0x01,0xFF,0xFF, 0x18,0xFF,0xFF, 0x3D,0xFF,0xDF, 0x8B,0xFF,0xFF, 0xEE,0xFF,0xFF, 0xFF,0x00,0x00, 0xFF,0x00,0x00, 0xFF,0x00,0x00, 0xFF,0x00,0x00,
  0xFF,0x00,0x00, 0xFF,0x00,0x00, 0xFF,0x00,0x00, 0x9B,0xFF,0xDF, 0x11,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 
        0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 
        0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x12,0xFF,0xFF, 0x9D,0xFF,0xDF, 0xFF,0x00,0x00, 0xFF,0x00,0x00, 0xFF,0x00,0x00,
  0xFF,0x00,0x00, 0xFF,0x00,0x00, 0x76,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xF7,0xBE, 0x00,0xD6,0x9A, 0x00,0xBD,0xD7, 0x00,0xAD,0x75, 0x00,0xAD,0x55, 0x00,0xAD,0x55, 0x00,0xAD,0x55, 0x00,0xA5,0x54, 0x00,0xA5,0x34, 0x00,0xA5,0x34, 
        0x00,0xA5,0x14, 0x00,0xA5,0x14, 0x00,0x9C,0xF3, 0x00,0x9C,0xF3, 0x00,0x9C,0xF3, 0x00,0x9C,0xD3, 0x00,0x9C,0xD3, 0x00,0x94,0xD2, 0x00,0x94,0xB2, 0x00,0x94,0xB2, 0x00,0x94,0x92, 0x00,0x94,0x92, 0x00,0x94,0x92, 0x00,0x8C,0x71, 0x00,0x8C,0x71, 
        0x00,0x8C,0x71, 0x00,0x8C,0x71, 0x00,0x9C,0xD3, 0x00,0xBD,0xF7, 0x00,0xF7,0x9E, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x77,0xFF,0xFF, 0xFF,0x00,0x00, 0xFF,0x00,0x00,
  0xFF,0x00,0x00, 0x9B,0xFF,0xDF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xDE,0xFB, 0x00,0xA5,0x14, 0x00,0x94,0xB2, 0x00,0x94,0x92, 0x00,0x94,0x92, 0x00,0x8C,0x71, 0x00,0x8C,0x71, 0x00,0x8C,0x51, 0x00,0x8C,0x51, 0x00,0x84,0x30, 0x00,0x84,0x30, 
        0x00,0x84,0x10, 0x00,0x84,0x10, 0x00,0x7B,0xEF, 0x00,0x7B,0xEF, 0x00,0x7B,0xCF, 0x00,0x7B,0xCF, 0x00,0x73,0xAE, 0x00,0x73,0xAE, 0x00,0x73,0x8E, 0x00,0x73,0x8E, 0x00,0x6B,0x6D, 0x00,0x6B,0x4D, 0x00,0x6B,0x4D, 0x00,0x6B,0x2D, 0x00,0x63,0x2C, 
        0x00,0x63,0x0C, 0x00,0x63,0x0C, 0x00,0x62,0xEC, 0x00,0x5A,0xEB, 0x00,0x6B,0x4D, 0x00,0xCE,0x79, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x9E,0xFF,0xDF, 0xFF,0x00,0x00,
  0xEE,0xFF,0xFF, 0x11,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xDF,0x1B, 0x00,0x9C,0xD3, 0x00,0x94,0xD2, 0x00,0x94,0xB2, 0x00,0x94,0x92, 0x00,0x94,0x92, 0x00,0x8C,0x71, 0x00,0x8C,0x71, 0x00,0x8C,0x51, 0x00,0x8C,0x51, 0x00,0x84,0x30, 0x00,0x84,0x30, 
        0x00,0x84,0x10, 0x00,0x84,0x10, 0x00,0x7B,0xEF, 0x00,0x7B,0xEF, 0x00,0x7B,0xCF, 0x00,0x7B,0xCF, 0x00,0x73,0xAE, 0x00,0x73,0xAE, 0x00,0x73,0x8E, 0x00,0x73,0x8E, 0x00,0x6B,0x6D, 0x00,0x6B,0x4D, 0x00,0x6B,0x4D, 0x00,0x6B,0x2D, 0x00,0x63,0x2C, 
        0x00,0x63,0x0C, 0x00,0x63,0x0C, 0x00,0x62,0xEC, 0x00,0x5A,0xEB, 0x00,0x5A,0xCB, 0x00,0x5A,0xCB, 0x00,0xCE,0x79, 0x00,0xFF,0xFF, 0x12,0xFF,0xFF, 0xEF,0xFF,0xFF,
  0x8B,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xF7,0xBE, 0x00,0xA5,0x34, 0x00,0x9C,0xD3, 0x00,0x94,0xD2, 0x00,0x94,0xB2, 0x00,0x94,0x92, 0x00,0x94,0x92, 0x00,0x8C,0x71, 0x00,0x8C,0x71, 0x00,0x8C,0x51, 0x00,0x8C,0x51, 0x00,0x84,0x30, 0x00,0x84,0x30, 
        0x00,0x84,0x10, 0x00,0x84,0x10, 0x00,0x7B,0xEF, 0x00,0x7B,0xEF, 0x00,0x7B,0xCF, 0x00,0x7B,0xCF, 0x00,0x73,0xAE, 0x00,0x73,0xAE, 0x00,0x73,0x8E, 0x00,0x73,0x8E, 0x00,0x6B,0x6D, 0x00,0x6B,0x4D, 0x00,0x6B,0x4D, 0x00,0x6B,0x2D, 0x00,0x63,0x2C, 
        0x00,0x63,0x0C, 0x00,0x63,0x0C, 0x00,0x62,0xEC, 0x00,0x5A,0xEB, 0x00,0x5A,0xCB, 0x00,0x5A,0xCB, 0x00,0x63,0x2C, 0x00,0xF7,0x9E, 0x00,0xFF,0xFF, 0x8D,0xFF,0xFF,
  0x3D,0xFF,0xDF, 0x00,0xFF,0xFF, 0x00,0xD6,0xBA, 0x00,0x9C,0xF3, 0x00,0x9C,0xD3, 0x00,0x94,0xD2, 0x00,0x94,0xB2, 0x00,0x94,0x92, 0x00,0x94,0x92, 0x00,0x8C,0x71, 0x00,0x8C,0x71, 0x00,0x8C,0x51, 0x00,0x8C,0x51, 0x00,0x84,0x30, 0x00,0x84,0x30, 
        0x00,0x84,0x10, 0x00,0x84,0x10, 0x00,0x7B,0xEF, 0x00,0x7B,0xEF, 0x00,0x7B,0xCF, 0x00,0x7B,0xCF, 0x00,0x73,0xAE, 0x00,0x73,0xAE, 0x00,0x73,0x8E, 0x00,0x73,0x8E, 0x00,0x6B,0x6D, 0x00,0x6B,0x4D, 0x00,0x6B,0x4D, 0x00,0x6B,0x2D, 0x00,0x63,0x2C, 
        0x00,0x63,0x0C, 0x00,0x63,0x0C, 0x00,0x62,0xEC, 0x00,0x5A,0xEB, 0x00,0x5A,0xCB, 0x00,0x5A,0xCB, 0x00,0x52,0xAA, 0x00,0xBD,0xD7, 0x00,0xFF,0xFF, 0x3F,0xFF,0xDF,
  0x17,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xBE,0x17, 0x00,0x9C,0xF3, 0x00,0x9C,0xD3, 0x00,0x94,0xD2, 0x00,0x94,0xB2, 0x00,0x94,0x92, 0x00,0x94,0x92, 0x00,0x8C,0x71, 0x00,0x8C,0x71, 0x00,0x8C,0x51, 0x00,0x8C,0x51, 0x00,0x84,0x30, 0x00,0x84,0x30, 
        0x00,0x84,0x10, 0x00,0x84,0x10, 0x00,0x7B,0xEF, 0x00,0x7B,0xEF, 0x00,0x7B,0xCF, 0x00,0x7B,0xCF, 0x00,0x73,0xAE, 0x00,0x73,0xAE, 0x00,0x73,0x8E, 0x00,0x73,0x8E, 0x00,0x6B,0x6D, 0x00,0x6B,0x4D, 0x00,0x6B,0x4D, 0x00,0x6B,0x2D, 0x00,0x63,0x2C, 
        0x00,0x63,0x0C, 0x00,0x63,0x0C, 0x00,0x62,0xEC, 0x00,0x5A,0xEB, 0x00,0x5A,0xCB, 0x00,0x5A,0xCB, 0x00,0x52,0xAA, 0x00,0x94,0x92, 0x00,0xFF,0xFF, 0x18,0xFF,0xFF,
  0x02,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xB5,0xD6, 0x00,0x9C,0xF3, 0x00,0x9C,0xD3, 0x00,0x94,0xD2, 0x00,0x94,0xB2, 0x00,0x94,0x92, 0x00,0x94,0x92, 0x00,0x8C,0x71, 0x00,0x8C,0x71, 0x00,0x8C,0x51, 0x00,0x8C,0x51, 0x00,0x84,0x30, 0x00,0x84,0x30, 
        0x00,0x84,0x10, 0x00,0x84,0x10, 0x00,0x7B,0xEF, 0x00,0x7B,0xEF, 0x00,0x7B,0xCF, 0x00,0x7B,0xCF, 0x00,0x73,0xAE, 0x00,0x73,0xAE, 0x00,0x73,0x8E, 0x00,0x73,0x8E, 0x00,0x6B,0x6D, 0x00,0x6B,0x4D, 0x00,0x6B,0x4D, 0x00,0x6B,0x2D, 0x00,0x63,0x2C, 
        0x00,0x63,0x0C, 0x00,0x63,0x0C, 0x00,0x62,0xEC, 0x00,0x5A,0xEB, 0x00,0x5A,0xCB, 0x00,0x5A,0xCB, 0x00,0x52,0xAA, 0x00,0x84,0x30, 0x00,0xFF,0xFF, 0x02,0xFF,0xFF,
  0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xB5,0xB6, 0x00,0x9C,0xF3, 0x00,0x9C,0xD3, 0x00,0x94,0xD2, 0x00,0x94,0xB2, 0x00,0x94,0x92, 0x00,0x94,0x92, 0x00,0x8C,0x71, 0x00,0x8C,0x71, 0x00,0x8C,0x51, 0x00,0x8C,0x51, 0x00,0x84,0x30, 0x00,0x84,0x30, 
        0x00,0x84,0x10, 0x00,0x84,0x10, 0x00,0x7B,0xEF, 0x00,0x7B,0xEF, 0x00,0x7B,0xCF, 0x00,0x7B,0xCF, 0x00,0x73,0xAE, 0x00,0x73,0xAE, 0x00,0x73,0x8E, 0x00,0x73,0x8E, 0x00,0x6B,0x6D, 0x00,0x6B,0x4D, 0x00,0x6B,0x4D, 0x00,0x6B,0x2D, 0x00,0x63,0x2C, 
        0x00,0x63,0x0C, 0x00,0x63,0x0C, 0x00,0x62,0xEC, 0x00,0x5A,0xEB, 0x00,0x5A,0xCB, 0x00,0x5A,0xCB, 0x00,0x52,0xAA, 0x00,0x84,0x10, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF,
  0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xB5,0xB6, 0x00,0x9C,0xF3, 0x00,0x9C,0xD3, 0x00,0x94,0xD2, 0x00,0x94,0xB2, 0x00,0x94,0x92, 0x00,0x94,0x92, 0x00,0x8C,0x71, 0x00,0x8C,0x71, 0x00,0x8C,0x51, 0x00,0x8C,0x51, 0x00,0x84,0x30, 0x00,0x84,0x30, 
        0x00,0x84,0x10, 0x00,0x84,0x10, 0x00,0x7B,0xEF, 0x00,0x7B,0xEF, 0x00,0x7B,0xCF, 0x00,0x7B,0xCF, 0x00,0x73,0xAE, 0x00,0x73,0xAE, 0x00,0x73,0x8E, 0x00,0x73,0x8E, 0x00,0x6B,0x6D, 0x00,0x6B,0x4D, 0x00,0x6B,0x4D, 0x00,0x6B,0x2D, 0x00,0x63,0x2C, 
        0x00,0x63,0x0C, 0x00,0x63,0x0C, 0x00,0x62,0xEC, 0x00,0x5A,0xEB, 0x00,0x5A,0xCB, 0x00,0x5A,0xCB, 0x00,0x52,0xAA, 0x00,0x84,0x10, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF,
  0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xB5,0xB6, 0x00,0x9C,0xF3, 0x00,0x9C,0xD3, 0x00,0x94,0xD2, 0x00,0x94,0xB2, 0x00,0x94,0x92, 0x00,0x94,0x92, 0x00,0x8C,0x71, 0x00,0x8C,0x71, 0x00,0x8C,0x51, 0x00,0x8C,0x51, 0x00,0x84,0x30, 0x00,0x84,0x30, 
        0x00,0x84,0x10, 0x00,0x84,0x10, 0x00,0x7B,0xEF, 0x00,0x7B,0xEF, 0x00,0x7B,0xCF, 0x00,0x7B,0xCF, 0x00,0x73,0xAE, 0x00,0x73,0xAE, 0x00,0x73,0x8E, 0x00,0x73,0x8E, 0x00,0x6B,0x6D, 0x00,0x6B,0x4D, 0x00,0x6B,0x4D, 0x00,0x6B,0x2D, 0x00,0x63,0x2C, 
        0x00,0x63,0x0C, 0x00,0x63,0x0C, 0x00,0x62,0xEC, 0x00,0x5A,0xEB, 0x00,0x5A,0xCB, 0x00,0x5A,0xCB, 0x00,0x52,0xAA, 0x00,0x84,0x10, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF,
  0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xB5,0xB6, 0x00,0x9C,0xF3, 0x00,0x9C,0xD3, 0x00,0x94,0xD2, 0x00,0x94,0xB2, 0x00,0x94,0x92, 0x00,0x94,0x92, 0x00,0x8C,0x71, 0x00,0x8C,0x71, 0x00,0x8C,0x51, 0x00,0x8C,0x51, 0x00,0x84,0x30, 0x00,0x84,0x30, 
        0x00,0x84,0x10, 0x00,0x84,0x10, 0x00,0x7B,0xEF, 0x00,0x7B,0xEF, 0x00,0x7B,0xCF, 0x00,0x7B,0xCF, 0x00,0x73,0xAE, 0x00,0x73,0xAE, 0x00,0x73,0x8E, 0x00,0x73,0x8E, 0x00,0x6B,0x6D, 0x00,0x6B,0x4D, 0x00,0x6B,0x4D, 0x00,0x6B,0x2D, 0x00,0x63,0x2C, 
        0x00,0x63,0x0C, 0x00,0x63,0x0C, 0x00,0x62,0xEC, 0x00,0x5A,0xEB, 0x00,0x5A,0xCB, 0x00,0x5A,0xCB, 0x00,0x52,0xAA, 0x00,0x84,0x10, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF,
  0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xB5,0xB6, 0x00,0x9C,0xF3, 0x00,0x9C,0xD3, 0x00,0x94,0xD2, 0x00,0x94,0xB2, 0x00,0x94,0x92, 0x00,0x94,0x92, 0x00,0x8C,0x71, 0x00,0x8C,0x71, 0x00,0x8C,0x51, 0x00,0x8C,0x51, 0x00,0x8C,0x51, 0x00,0xDE,0xFB, 
        0x00,0xB5,0xB6, 0x00,0x84,0x10, 0x00,0x7B,0xEF, 0x00,0x7B,0xEF, 0x00,0x7B,0xCF, 0x00,0x7B,0xCF, 0x00,0x73,0xAE, 0x00,0x73,0xAE, 0x00,0x73,0x8E, 0x00,0x73,0x8E, 0x00,0x6B,0x6D, 0x00,0x6B,0x4D, 0x00,0x6B,0x4D, 0x00,0x6B,0x2D, 0x00,0x63,0x2C, 
        0x00,0x63,0x0C, 0x00,0x63,0x0C, 0x00,0x62,0xEC, 0x00,0x5A,0xEB, 0x00,0x5A,0xCB, 0x00,0x5A,0xCB, 0x00,0x52,0xAA, 0x00,0x84,0x10, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF,
  0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xB5,0xB6, 0x00,0x9C,0xF3, 0x00,0x9C,0xD3, 0x00,0x94,0xD2, 0x00,0x94,0xB2, 0x00,0x94,0x92, 0x00,0x94,0x92, 0x00,0x8C,0x71, 0x00,0x8C,0x71, 0x00,0x8C,0x51, 0x00,0x8C,0x51, 0x00,0x94,0xB2, 0x00,0xFF,0xFF, 
        0x00,0xFF,0xFF, 0x00,0xDF,0x1B, 0x00,0x94,0x92, 0x00,0x7B,0xEF, 0x00,0x7B,0xCF, 0x00,0x7B,0xCF, 0x00,0x73,0xAE, 0x00,0x73,0xAE, 0x00,0x73,0x8E, 0x00,0x73,0x8E, 0x00,0x6B,0x6D, 0x00,0x6B,0x4D, 0x00,0x6B,0x4D, 0x00,0x6B,0x2D, 0x00,0x63,0x2C, 
        0x00,0x63,0x0C, 0x00,0x63,0x0C, 0x00,0x62,0xEC, 0x00,0x5A,0xEB, 0x00,0x5A,0xCB, 0x00,0x5A,0xCB, 0x00,0x52,0xAA, 0x00,0x84,0x10, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF,
  0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xB5,0xB6, 0x00,0x9C,0xF3, 0x00,0x9C,0xD3, 0x00,0x94,0xD2, 0x00,0x94,0xB2, 0x00,0x94,0x92, 0x00,0x94,0x92, 0x00,0x8C,0x71, 0x00,0x8C,0x71, 0x00,0x8C,0x51, 0x00,0x8C,0x51, 0x00,0x94,0xB2, 0x00,0xFF,0xFF, 
        0x00,0xDE,0xFB, 0x00,0xF7,0xBE, 0x00,0xF7,0xDE, 0x00,0xB5,0xB6, 0x00,0x7B,0xEF, 0x00,0x7B,0xCF, 0x00,0x73,0xAE, 0x00,0x73,0xAE, 0x00,0x73,0x8E, 0x00,0x73,0x8E, 0x00,0x6B,0x6D, 0x00,0x6B,0x4D, 0x00,0x6B,0x4D, 0x00,0x6B,0x2D, 0x00,0x63,0x2C, 
        0x00,0x63,0x0C, 0x00,0x63,0x0C, 0x00,0x62,0xEC, 0x00,0x5A,0xEB, 0x00,0x5A,0xCB, 0x00,0x5A,0xCB, 0x00,0x52,0xAA, 0x00,0x84,0x10, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF,
  0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xB5,0xB6, 0x00,0x9C,0xF3, 0x00,0x9C,0xD3, 0x00,0x94,0xD2, 0x00,0x94,0xB2, 0x00,0x94,0x92, 0x00,0x94,0x92, 0x00,0x8C,0x71, 0x00,0x8C,0x71, 0x00,0x8C,0x51, 0x00,0x8C,0x51, 0x00,0x94,0xB2, 0x00,0xFF,0xFF, 
        0x00,0xB5,0xB6, 0x00,0x94,0x92, 0x00,0xDE,0xDB, 0x00,0xFF,0xFF, 0x00,0xDF,0x1B, 0x00,0x8C,0x71, 0x00,0x73,0xAE, 0x00,0x73,0xAE, 0x00,0x73,0x8E, 0x00,0x73,0x8E, 0x00,0x6B,0x6D, 0x00,0x6B,0x4D, 0x00,0x6B,0x4D, 0x00,0x6B,0x2D, 0x00,0x63,0x2C, 
        0x00,0x63,0x0C, 0x00,0x63,0x0C, 0x00,0x62,0xEC, 0x00,0x5A,0xEB, 0x00,0x5A,0xCB, 0x00,0x5A,0xCB, 0x00,0x52,0xAA, 0x00,0x84,0x10, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF,
  0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xB5,0xB6, 0x00,0x9C,0xF3, 0x00,0x9C,0xD3, 0x00,0x94,0xD2, 0x00,0x94,0xB2, 0x00,0x94,0x92, 0x00,0x94,0x92, 0x00,0x8C,0x71, 0x00,0x8C,0x71, 0x00,0x8C,0x51, 0x00,0x8C,0x51, 0x00,0x94,0xB2, 0x00,0xFF,0xFF, 
        0x00,0xB5,0xB6, 0x00,0x84,0x10, 0x00,0x7B,0xEF, 0x00,0xA5,0x54, 0x00,0xF7,0xBE, 0x00,0xF7,0xDE, 0x00,0xB5,0xB6, 0x00,0x73,0xAE, 0x00,0x73,0x8E, 0x00,0x73,0x8E, 0x00,0x6B,0x6D, 0x00,0x6B,0x4D, 0x00,0x6B,0x4D, 0x00,0x6B,0x2D, 0x00,0x63,0x2C, 
        0x00,0x63,0x0C, 0x00,0x63,0x0C, 0x00,0x62,0xEC, 0x00,0x5A,0xEB, 0x00,0x5A,0xCB, 0x00,0x5A,0xCB, 0x00,0x52,0xAA, 0x00,0x84,0x10, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF,
  0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xB5,0xB6, 0x00,0x9C,0xF3, 0x00,0x9C,0xD3, 0x00,0x94,0xD2, 0x00,0x94,0xB2, 0x00,0x94,0x92, 0x00,0x94,0x92, 0x00,0x8C,0x71, 0x00,0x8C,0x71, 0x00,0x8C,0x51, 0x00,0x8C,0x51, 0x00,0x94,0xB2, 0x00,0xFF,0xFF, 
        0x00,0xB5,0xB6, 0x00,0x84,0x10, 0x00,0x7B,0xEF, 0x00,0x7B,0xEF, 0x00,0x84,0x30, 0x00,0xD6,0xBA, 0x00,0xFF,0xFF, 0x00,0xE7,0x3C, 0x00,0x8C,0x71, 0x00,0x73,0x8E, 0x00,0x6B,0x6D, 0x00,0x6B,0x4D, 0x00,0x6B,0x4D, 0x00,0x6B,0x2D, 0x00,0x63,0x2C, 
        0x00,0x63,0x0C, 0x00,0x63,0x0C, 0x00,0x62,0xEC, 0x00,0x5A,0xEB, 0x00,0x5A,0xCB, 0x00,0x5A,0xCB, 0x00,0x52,0xAA, 0x00,0x84,0x10, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF,
  0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xB5,0xB6, 0x00,0x9C,0xF3, 0x00,0x9C,0xD3, 0x00,0x94,0xD2, 0x00,0x94,0xB2, 0x00,0x94,0x92, 0x00,0x94,0x92, 0x00,0x8C,0x71, 0x00,0x8C,0x71, 0x00,0x8C,0x51, 0x00,0x8C,0x51, 0x00,0x94,0xB2, 0x00,0xFF,0xFF, 
        0x00,0xB5,0xB6, 0x00,0x84,0x10, 0x00,0x7B,0xEF, 0x00,0x7B,0xEF, 0x00,0x7B,0xCF, 0x00,0x7B,0xCF, 0x00,0xA5,0x14, 0x00,0xF7,0xBE, 0x00,0xFF,0xDF, 0x00,0xBD,0xD7, 0x00,0x73,0x8E, 0x00,0x6B,0x4D, 0x00,0x6B,0x4D, 0x00,0x6B,0x2D, 0x00,0x63,0x2C, 
        0x00,0x63,0x0C, 0x00,0x63,0x0C, 0x00,0x62,0xEC, 0x00,0x5A,0xEB, 0x00,0x5A,0xCB, 0x00,0x5A,0xCB, 0x00,0x52,0xAA, 0x00,0x84,0x10, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF,
  0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xB5,0xB6, 0x00,0x9C,0xF3, 0x00,0x9C,0xD3, 0x00,0x94,0xD2, 0x00,0x94,0xB2, 0x00,0x94,0x92, 0x00,0x94,0x92, 0x00,0x8C,0x71, 0x00,0x8C,0x71, 0x00,0x8C,0x51, 0x00,0x8C,0x51, 0x00,0x94,0xB2, 0x00,0xFF,0xFF, 
        0x00,0xB5,0xB6, 0x00,0x84,0x10, 0x00,0x7B,0xEF, 0x00,0x7B,0xEF, 0x00,0x7B,0xCF, 0x00,0x7B,0xCF, 0x00,0x73,0xAE, 0x00,0x84,0x10, 0x00,0xD6,0x9A, 0x00,0xFF,0xFF, 0x00,0xDE,0xFB, 0x00,0x6B,0x6D, 0x00,0x6B,0x4D, 0x00,0x6B,0x2D, 0x00,0x63,0x2C, 
        0x00,0x63,0x0C, 0x00,0x63,0x0C, 0x00,0x62,0xEC, 0x00,0x5A,0xEB, 0x00,0x5A,0xCB, 0x00,0x5A,0xCB, 0x00,0x52,0xAA, 0x00,0x84,0x10, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF,
  0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xB5,0xB6, 0x00,0x9C,0xF3, 0x00,0x9C,0xD3, 0x00,0x94,0xD2, 0x00,0x94,0xB2, 0x00,0x94,0x92, 0x00,0x94,0x92, 0x00,0x8C,0x71, 0x00,0x8C,0x71, 0x00,0x8C,0x51, 0x00,0x8C,0x51, 0x00,0x94,0xB2, 0x00,0xFF,0xFF, 
        0x00,0xB5,0xB6, 0x00,0x84,0x10, 0x00,0x7B,0xEF, 0x00,0x7B,0xEF, 0x00,0x7B,0xCF, 0x00,0x7B,0xCF, 0x00,0x73,0xAE, 0x00,0x84,0x10, 0x00,0xD6,0x9A, 0x00,0xFF,0xFF, 0x00,0xDE,0xFB, 0x00,0x6B,0x6D, 0x00,0x6B,0x4D, 0x00,0x6B,0x2D, 0x00,0x63,0x2C, 
        0x00,0x63,0x0C, 0x00,0x63,0x0C, 0x00,0x62,0xEC, 0x00,0x5A,0xEB, 0x00,0x5A,0xCB, 0x00,0x5A,0xCB, 0x00,0x52,0xAA, 0x00,0x84,0x10, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF,
  0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xB5,0xB6, 0x00,0x9C,0xF3, 0x00,0x9C,0xD3, 0x00,0x94,0xD2, 0x00,0x94,0xB2, 0x00,0x94,0x92, 0x00,0x94,0x92, 0x00,0x8C,0x71, 0x00,0x8C,0x71, 0x00,0x8C,0x51, 0x00,0x8C,0x51, 0x00,0x94,0xB2, 0x00,0xFF,0xFF, 
        0x00,0xB5,0xB6, 0x00,0x84,0x10, 0x00,0x7B,0xEF, 0x00,0x7B,0xEF, 0x00,0x7B,0xCF, 0x00,0x7B,0xCF, 0x00,0xA5,0x14, 0x00,0xF7,0xBE, 0x00,0xFF,0xDF, 0x00,0xBD,0xD7, 0x00,0x73,0x8E, 0x00,0x6B,0x4D, 0x00,0x6B,0x4D, 0x00,0x6B,0x2D, 0x00,0x63,0x2C, 
        0x00,0x63,0x0C, 0x00,0x63,0x0C, 0x00,0x62,0xEC, 0x00,0x5A,0xEB, 0x00,0x5A,0xCB, 0x00,0x5A,0xCB, 0x00,0x52,0xAA, 0x00,0x84,0x10, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF,
  0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xB5,0xB6, 0x00,0x9C,0xF3, 0x00,0x9C,0xD3, 0x00,0x94,0xD2, 0x00,0x94,0xB2, 0x00,0x94,0x92, 0x00,0x94,0x92, 0x00,0x8C,0x71, 0x00,0x8C,0x71, 0x00,0x8C,0x51, 0x00,0x8C,0x51, 0x00,0x94,0xB2, 0x00,0xFF,0xFF, 
        0x00,0xB5,0xB6, 0x00,0x84,0x10, 0x00,0x7B,0xEF, 0x00,0x7B,0xEF, 0x00,0x84,0x30, 0x00,0xD6,0xBA, 0x00,0xFF,0xFF, 0x00,0xDE,0xFB, 0x00,0x8C,0x51, 0x00,0x73,0x8E, 0x00,0x6B,0x6D, 0x00,0x6B,0x4D, 0x00,0x6B,0x4D, 0x00,0x6B,0x2D, 0x00,0x63,0x2C, 
        0x00,0x63,0x0C, 0x00,0x63,0x0C, 0x00,0x62,0xEC, 0x00,0x5A,0xEB, 0x00,0x5A,0xCB, 0x00,0x5A,0xCB, 0x00,0x52,0xAA, 0x00,0x84,0x10, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF,
  0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xB5,0xB6, 0x00,0x9C,0xF3, 0x00,0x9C,0xD3, 0x00,0x94,0xD2, 0x00,0x94,0xB2, 0x00,0x94,0x92, 0x00,0x94,0x92, 0x00,0x8C,0x71, 0x00,0x8C,0x71, 0x00,0x8C,0x51, 0x00,0x8C,0x51, 0x00,0x94,0xB2, 0x00,0xFF,0xFF, 
        0x00,0xB5,0xB6, 0x00,0x84,0x10, 0x00,0x7B,0xEF, 0x00,0xA5,0x54, 0x00,0xF7,0xBE, 0x00,0xF7,0xDE, 0x00,0xB5,0xB6, 0x00,0x73,0xAE, 0x00,0x73,0x8E, 0x00,0x73,0x8E, 0x00,0x6B,0x6D, 0x00,0x6B,0x4D, 0x00,0x6B,0x4D, 0x00,0x6B,0x2D, 0x00,0x63,0x2C, 
        0x00,0x63,0x0C, 0x00,0x63,0x0C, 0x00,0x62,0xEC, 0x00,0x5A,0xEB, 0x00,0x5A,0xCB, 0x00,0x5A,0xCB, 0x00,0x52,0xAA, 0x00,0x84,0x10, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF,
  0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xB5,0xB6, 0x00,0x9C,0xF3, 0x00,0x9C,0xD3, 0x00,0x94,0xD2, 0x00,0x94,0xB2, 0x00,0x94,0x92, 0x00,0x94,0x92, 0x00,0x8C,0x71, 0x00,0x8C,0x71, 0x00,0x8C,0x51, 0x00,0x8C,0x51, 0x00,0x94,0xB2, 0x00,0xFF,0xFF, 
        0x00,0xB5,0xB6, 0x00,0x94,0x92, 0x00,0xDE,0xDB, 0x00,0xFF,0xFF, 0x00,0xDF,0x1B, 0x00,0x8C,0x71, 0x00,0x73,0xAE, 0x00,0x73,0xAE, 0x00,0x73,0x8E, 0x00,0x73,0x8E, 0x00,0x6B,0x6D, 0x00,0x6B,0x4D, 0x00,0x6B,0x4D, 0x00,0x6B,0x2D, 0x00,0x63,0x2C, 
        0x00,0x63,0x0C, 0x00,0x63,0x0C, 0x00,0x62,0xEC, 0x00,0x5A,0xEB, 0x00,0x5A,0xCB, 0x00,0x5A,0xCB, 0x00,0x52,0xAA, 0x00,0x84,0x10, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF,
  0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xB5,0xB6, 0x00,0x9C,0xF3, 0x00,0x9C,0xD3, 0x00,0x94,0xD2, 0x00,0x94,0xB2, 0x00,0x94,0x92, 0x00,0x94,0x92, 0x00,0x8C,0x71, 0x00,0x8C,0x71, 0x00,0x8C,0x51, 0x00,0x8C,0x51, 0x00,0x94,0xB2, 0x00,0xFF,0xFF, 
        0x00,0xDE,0xFB, 0x00,0xF7,0xBE, 0x00,0xF7,0xDE, 0x00,0xB5,0xB6, 0x00,0x7B,0xEF, 0x00,0x7B,0xCF, 0x00,0x73,0xAE, 0x00,0x73,0xAE, 0x00,0x73,0x8E, 0x00,0x73,0x8E, 0x00,0x6B,0x6D, 0x00,0x6B,0x4D, 0x00,0x6B,0x4D, 0x00,0x6B,0x2D, 0x00,0x63,0x2C, 
        0x00,0x63,0x0C, 0x00,0x63,0x0C, 0x00,0x62,0xEC, 0x00,0x5A,0xEB, 0x00,0x5A,0xCB, 0x00,0x5A,0xCB, 0x00,0x52,0xAA, 0x00,0x84,0x10, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF,
  0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xB5,0xB6, 0x00,0x9C,0xF3, 0x00,0x9C,0xD3, 0x00,0x94,0xD2, 0x00,0x94,0xB2, 0x00,0x94,0x92, 0x00,0x94,0x92, 0x00,0x8C,0x71, 0x00,0x8C,0x71, 0x00,0x8C,0x51, 0x00,0x8C,0x51, 0x00,0x94,0xB2, 0x00,0xFF,0xFF, 
        0x00,0xFF,0xFF, 0x00,0xDF,0x1B, 0x00,0x94,0x92, 0x00,0x7B,0xEF, 0x00,0x7B,0xCF, 0x00,0x7B,0xCF, 0x00,0x73,0xAE, 0x00,0x73,0xAE, 0x00,0x73,0x8E, 0x00,0x73,0x8E, 0x00,0x6B,0x6D, 0x00,0x6B,0x4D, 0x00,0x6B,0x4D, 0x00,0x6B,0x2D, 0x00,0x63,0x2C, 
        0x00,0x63,0x0C, 0x00,0x63,0x0C, 0x00,0x62,0xEC, 0x00,0x5A,0xEB, 0x00,0x5A,0xCB, 0x00,0x5A,0xCB, 0x00,0x52,0xAA, 0x00,0x84,0x10, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF,
  0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xB5,0xB6, 0x00,0x9C,0xF3, 0x00,0x9C,0xD3, 0x00,0x94,0xD2, 0x00,0x94,0xB2, 0x00,0x94,0x92, 0x00,0x94,0x92, 0x00,0x8C,0x71, 0x00,0x8C,0x71, 0x00,0x8C,0x51, 0x00,0x8C,0x51, 0x00,0x8C,0x51, 0x00,0xDE,0xFB, 
        0x00,0xB5,0xB6, 0x00,0x84,0x10, 0x00,0x7B,0xEF, 0x00,0x7B,0xEF, 0x00,0x7B,0xCF, 0x00,0x7B,0xCF, 0x00,0x73,0xAE, 0x00,0x73,0xAE, 0x00,0x73,0x8E, 0x00,0x73,0x8E, 0x00,0x6B,0x6D, 0x00,0x6B,0x4D, 0x00,0x6B,0x4D, 0x00,0x6B,0x2D, 0x00,0x63,0x2C, 
        0x00,0x63,0x0C, 0x00,0x63,0x0C, 0x00,0x62,0xEC, 0x00,0x5A,0xEB, 0x00,0x5A,0xCB, 0x00,0x5A,0xCB, 0x00,0x52,0xAA, 0x00,0x84,0x10, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF,
  0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xB5,0xB6, 0x00,0x9C,0xF3, 0x00,0x9C,0xD3, 0x00,0x94,0xD2, 0x00,0x94,0xB2, 0x00,0x94,0x92, 0x00,0x94,0x92, 0x00,0x8C,0x71, 0x00,0x8C,0x71, 0x00,0x8C,0x51, 0x00,0x8C,0x51, 0x00,0x84,0x30, 0x00,0x84,0x30, 
        0x00,0x84,0x10, 0x00,0x84,0x10, 0x00,0x7B,0xEF, 0x00,0x7B,0xEF, 0x00,0x7B,0xCF, 0x00,0x7B,0xCF, 0x00,0x73,0xAE, 0x00,0x73,0xAE, 0x00,0x73,0x8E, 0x00,0x73,0x8E, 0x00,0x6B,0x6D, 0x00,0x6B,0x4D, 0x00,0x6B,0x4D, 0x00,0x6B,0x2D, 0x00,0x63,0x2C, 
        0x00,0x63,0x0C, 0x00,0x63,0x0C, 0x00,0x62,0xEC, 0x00,0x5A,0xEB, 0x00,0x5A,0xCB, 0x00,0x5A,0xCB, 0x00,0x52,0xAA, 0x00,0x84,0x10, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF,
  0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xB5,0xB6, 0x00,0x9C,0xF3, 0x00,0x9C,0xD3, 0x00,0x94,0xD2, 0x00,0x94,0xB2, 0x00,0x94,0x92, 0x00,0x94,0x92, 0x00,0x8C,0x71, 0x00,0x8C,0x71, 0x00,0x8C,0x51, 0x00,0x8C,0x51, 0x00,0x84,0x30, 0x00,0x84,0x30, 
        0x00,0x84,0x10, 0x00,0x84,0x10, 0x00,0x7B,0xEF, 0x00,0x7B,0xEF, 0x00,0x7B,0xCF, 0x00,0x7B,0xCF, 0x00,0x73,0xAE, 0x00,0x73,0xAE, 0x00,0x73,0x8E, 0x00,0x73,0x8E, 0x00,0x6B,0x6D, 0x00,0x6B,0x4D, 0x00,0x6B,0x4D, 0x00,0x6B,0x2D, 0x00,0x63,0x2C, 
        0x00,0x63,0x0C, 0x00,0x63,0x0C, 0x00,0x62,0xEC, 0x00,0x5A,0xEB, 0x00,0x5A,0xCB, 0x00,0x5A,0xCB, 0x00,0x52,0xAA, 0x00,0x84,0x10, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF,
  0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xB5,0xB6, 0x00,0x9C,0xF3, 0x00,0x9C,0xD3, 0x00,0x94,0xD2, 0x00,0x94,0xB2, 0x00,0x94,0x92, 0x00,0x94,0x92, 0x00,0x8C,0x71, 0x00,0x8C,0x71, 0x00,0x8C,0x51, 0x00,0x8C,0x51, 0x00,0x84,0x30, 0x00,0x84,0x30, 
        0x00,0x84,0x10, 0x00,0x84,0x10, 0x00,0x7B,0xEF, 0x00,0x7B,0xEF, 0x00,0x7B,0xCF, 0x00,0x7B,0xCF, 0x00,0x73,0xAE, 0x00,0x73,0xAE, 0x00,0x73,0x8E, 0x00,0x73,0x8E, 0x00,0x6B,0x6D, 0x00,0x6B,0x4D, 0x00,0x6B,0x4D, 0x00,0x6B,0x2D, 0x00,0x63,0x2C, 
        0x00,0x63,0x0C, 0x00,0x63,0x0C, 0x00,0x62,0xEC, 0x00,0x5A,0xEB, 0x00,0x5A,0xCB, 0x00,0x5A,0xCB, 0x00,0x52,0xAA, 0x00,0x84,0x10, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF,
  0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xB5,0xB6, 0x00,0x9C,0xF3, 0x00,0x9C,0xD3, 0x00,0x94,0xD2, 0x00,0x94,0xB2, 0x00,0x94,0x92, 0x00,0x94,0x92, 0x00,0x8C,0x71, 0x00,0x8C,0x71, 0x00,0x8C,0x51, 0x00,0x8C,0x51, 0x00,0x84,0x30, 0x00,0x84,0x30, 
        0x00,0x84,0x10, 0x00,0x84,0x10, 0x00,0x7B,0xEF, 0x00,0x7B,0xEF, 0x00,0x7B,0xCF, 0x00,0x7B,0xCF, 0x00,0x73,0xAE, 0x00,0x73,0xAE, 0x00,0x73,0x8E, 0x00,0x73,0x8E, 0x00,0x6B,0x6D, 0x00,0x6B,0x4D, 0x00,0x6B,0x4D, 0x00,0x6B,0x2D, 0x00,0x63,0x2C, 
        0x00,0x63,0x0C, 0x00,0x63,0x0C, 0x00,0x62,0xEC, 0x00,0x5A,0xEB, 0x00,0x5A,0xCB, 0x00,0x5A,0xCB, 0x00,0x52,0xAA, 0x00,0x84,0x10, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF,
  0x02,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xB5,0xD6, 0x00,0x9C,0xF3, 0x00,0x9C,0xD3, 0x00,0x94,0xD2, 0x00,0x94,0xB2, 0x00,0x94,0x92, 0x00,0x94,0x92, 0x00,0x8C,0x71, 0x00,0x8C,0x71, 0x00,0x8C,0x51, 0x00,0x8C,0x51, 0x00,0x84,0x30, 0x00,0x84,0x30, 
        0x00,0x84,0x10, 0x00,0x84,0x10, 0x00,0x7B,0xEF, 0x00,0x7B,0xEF, 0x00,0x7B,0xCF, 0x00,0x7B,0xCF, 0x00,0x73,0xAE, 0x00,0x73,0xAE, 0x00,0x73,0x8E, 0x00,0x73,0x8E, 0x00,0x6B,0x6D, 0x00,0x6B,0x4D, 0x00,0x6B,0x4D, 0x00,0x6B,0x2D, 0x00,0x63,0x2C, 
        0x00,0x63,0x0C, 0x00,0x63,0x0C, 0x00,0x62,0xEC, 0x00,0x5A,0xEB, 0x00,0x5A,0xCB, 0x00,0x5A,0xCB, 0x00,0x52,0xAA, 0x00,0x84,0x30, 0x00,0xFF,0xFF, 0x02,0xFF,0xFF,
  0x16,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xBE,0x17, 0x00,0x9C,0xF3, 0x00,0x9C,0xD3, 0x00,0x94,0xD2, 0x00,0x94,0xB2, 0x00,0x94,0x92, 0x00,0x94,0x92, 0x00,0x8C,0x71, 0x00,0x8C,0x71, 0x00,0x8C,0x51, 0x00,0x8C,0x51, 0x00,0x84,0x30, 0x00,0x84,0x30, 
        0x00,0x84,0x10, 0x00,0x84,0x10, 0x00,0x7B,0xEF, 0x00,0x7B,0xEF, 0x00,0x7B,0xCF, 0x00,0x7B,0xCF, 0x00,0x73,0xAE, 0x00,0x73,0xAE, 0x00,0x73,0x8E, 0x00,0x73,0x8E, 0x00,0x6B,0x6D, 0x00,0x6B,0x4D, 0x00,0x6B,0x4D, 0x00,0x6B,0x2D, 0x00,0x63,0x2C, 
        0x00,0x63,0x0C, 0x00,0x63,0x0C, 0x00,0x62,0xEC, 0x00,0x5A,0xEB, 0x00,0x5A,0xCB, 0x00,0x5A,0xCB, 0x00,0x52,0xAA, 0x00,0x94,0x92, 0x00,0xFF,0xFF, 0x18,0xFF,0xFF,
  0x3D,0xFF,0xDF, 0x00,0xFF,0xFF, 0x00,0xD6,0xBA, 0x00,0x9C,0xF3, 0x00,0x9C,0xD3, 0x00,0x94,0xD2, 0x00,0x94,0xB2, 0x00,0x94,0x92, 0x00,0x94,0x92, 0x00,0x8C,0x71, 0x00,0x8C,0x71, 0x00,0x8C,0x51, 0x00,0x8C,0x51, 0x00,0x84,0x30, 0x00,0x84,0x30, 
        0x00,0x84,0x10, 0x00,0x84,0x10, 0x00,0x7B,0xEF, 0x00,0x7B,0xEF, 0x00,0x7B,0xCF, 0x00,0x7B,0xCF, 0x00,0x73,0xAE, 0x00,0x73,0xAE, 0x00,0x73,0x8E, 0x00,0x73,0x8E, 0x00,0x6B,0x6D, 0x00,0x6B,0x4D, 0x00,0x6B,0x4D, 0x00,0x6B,0x2D, 0x00,0x63,0x2C, 
        0x00,0x63,0x0C, 0x00,0x63,0x0C, 0x00,0x62,0xEC, 0x00,0x5A,0xEB, 0x00,0x5A,0xCB, 0x00,0x5A,0xCB, 0x00,0x52,0xAA, 0x00,0xBD,0xF7, 0x00,0xFF,0xFF, 0x40,0xFF,0xDF,
  0x8B,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xF7,0xBE, 0x00,0xA5,0x34, 0x00,0x9C,0xD3, 0x00,0x94,0xD2, 0x00,0x94,0xB2, 0x00,0x94,0x92, 0x00,0x94,0x92, 0x00,0x8C,0x71, 0x00,0x8C,0x71, 0x00,0x8C,0x51, 0x00,0x8C,0x51, 0x00,0x84,0x30, 0x00,0x84,0x30, 
        0x00,0x84,0x10, 0x00,0x84,0x10, 0x00,0x7B,0xEF, 0x00,0x7B,0xEF, 0x00,0x7B,0xCF, 0x00,0x7B,0xCF, 0x00,0x73,0xAE, 0x00,0x73,0xAE, 0x00,0x73,0x8E, 0x00,0x73,0x8E, 0x00,0x6B,0x6D, 0x00,0x6B,0x4D, 0x00,0x6B,0x4D, 0x00,0x6B,0x2D, 0x00,0x63,0x2C, 
        0x00,0x63,0x0C, 0x00,0x63,0x0C, 0x00,0x62,0xEC, 0x00,0x5A,0xEB, 0x00,0x5A,0xCB, 0x00,0x5A,0xCB, 0x00,0x6B,0x2D, 0x00,0xF7,0xBE, 0x00,0xFF,0xFF, 0x8D,0xFF,0xFF,
  0xEE,0xFF,0xFF, 0x12,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xDF,0x1B, 0x00,0x9C,0xD3, 0x00,0x94,0xD2, 0x00,0x94,0xB2, 0x00,0x94,0x92, 0x00,0x94,0x92, 0x00,0x8C,0x71, 0x00,0x8C,0x71, 0x00,0x8C,0x51, 0x00,0x8C,0x51, 0x00,0x84,0x30, 0x00,0x84,0x30, 
        0x00,0x84,0x10, 0x00,0x84,0x10, 0x00,0x7B,0xEF, 0x00,0x7B,0xEF, 0x00,0x7B,0xCF, 0x00,0x7B,0xCF, 0x00,0x73,0xAE, 0x00,0x73,0xAE, 0x00,0x73,0x8E, 0x00,0x73,0x8E, 0x00,0x6B,0x6D, 0x00,0x6B,0x4D, 0x00,0x6B,0x4D, 0x00,0x6B,0x2D, 0x00,0x63,0x2C, 
        0x00,0x63,0x0C, 0x00,0x63,0x0C, 0x00,0x62,0xEC, 0x00,0x5A,0xEB, 0x00,0x5A,0xCB, 0x00,0x5A,0xCB, 0x00,0xCE,0x79, 0x00,0xFF,0xFF, 0x13,0xFF,0xFF, 0xEF,0xFF,0xFF,
  0xFF,0x00,0x00, 0x9D,0xFF,0xDF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xDF,0x1B, 0x00,0xA5,0x14, 0x00,0x94,0xB2, 0x00,0x94,0x92, 0x00,0x94,0x92, 0x00,0x8C,0x71, 0x00,0x8C,0x71, 0x00,0x8C,0x51, 0x00,0x8C,0x51, 0x00,0x84,0x30, 0x00,0x84,0x30, 
        0x00,0x84,0x10, 0x00,0x84,0x10, 0x00,0x7B,0xEF, 0x00,0x7B,0xEF, 0x00,0x7B,0xCF, 0x00,0x7B,0xCF, 0x00,0x73,0xAE, 0x00,0x73,0xAE, 0x00,0x73,0x8E, 0x00,0x73,0x8E, 0x00,0x6B,0x6D, 0x00,0x6B,0x4D, 0x00,0x6B,0x4D, 0x00,0x6B,0x2D, 0x00,0x63,0x2C, 
        0x00,0x63,0x0C, 0x00,0x63,0x0C, 0x00,0x62,0xEC, 0x00,0x5A,0xEB, 0x00,0x6B,0x4D, 0x00,0xCE,0x79, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0xA0,0xFF,0xDF, 0xFF,0x00,0x00,
  0xFF,0x00,0x00, 0xFF,0x00,0x00, 0x77,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xF7,0xBE, 0x00,0xD6,0xBA, 0x00,0xBD,0xD7, 0x00,0xAD,0x75, 0x00,0xAD,0x55, 0x00,0xAD,0x55, 0x00,0xAD,0x55, 0x00,0xA5,0x54, 0x00,0xA5,0x34, 0x00,0xA5,0x34, 
        0x00,0xA5,0x14, 0x00,0xA5,0x14, 0x00,0x9C,0xF3, 0x00,0x9C,0xF3, 0x00,0x9C,0xF3, 0x00,0x9C,0xD3, 0x00,0x9C,0xD3, 0x00,0x94,0xD2, 0x00,0x94,0xB2, 0x00,0x94,0xB2, 0x00,0x94,0x92, 0x00,0x94,0x92, 0x00,0x94,0x92, 0x00,0x8C,0x71, 0x00,0x8C,0x71, 
        0x00,0x8C,0x71, 0x00,0x8C,0x71, 0x00,0x9C,0xD3, 0x00,0xBD,0xF7, 0x00,0xF7,0xBE, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x79,0xFF,0xFF, 0xFF,0x00,0x00, 0xFF,0x00,0x00,
  0xFF,0x00,0x00, 0xFF,0x00,0x00, 0xFF,0x00,0x00, 0x9E,0xFF,0xDF, 0x12,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 
        0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 
        0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x13,0xFF,0xFF, 0x9F,0xFF,0xDF, 0xFF,0x00,0x00, 0xFF,0x00,0x00, 0xFF,0x00,0x00,
  0xFF,0x00,0x00, 0xFF,0x00,0x00, 0xFF,0x00,0x00, 0xFF,0x00,0x00, 0xEF,0xFF,0xFF, 0x8D,0xFF,0xFF, 0x3F,0xFF,0xDF, 0x18,0xFF,0xFF, 0x01,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 
        0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 
        0x00,0xFF,0xFF, 0x02,0xFF,0xFF, 0x18,0xFF,0xFF, 0x3F,0xFF,0xDF, 0x8D,0xFF,0xFF, 0xEF,0xFF,0xFF, 0xFF,0x00,0x00, 0xFF,0x00,0x00, 0xFF,0x00,0x00, 0xFF,0x00,0x00
};

GUI_CONST_STORAGE GUI_BITMAP bmNextGrayButton = {
  40, /* xSize */
  42, /* ySize */
  120, /* BytesPerLine */
  24, /* BitsPerPixel */
  (unsigned char *)_acNextGray,  /* Pointer to picture data */
  NULL,  /* Pointer to palette */
  GUI_DRAW_BMPA565
};

/*************************** End of file ****************************/
