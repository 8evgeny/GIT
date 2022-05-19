/*********************************************************************
*                SEGGER Microcontroller GmbH & Co. KG                *
*        Solutions for real time microcontroller applications        *
*                           www.segger.com                           *
**********************************************************************
*                                                                    *
* C-file generated by                                                *
*                                                                    *
*        Bitmap Converter (ST) for emWin V5.32.                      *
*        Compiled Oct  8 2015, 11:58:22                              *
*                                                                    *
*        (c) 1998 - 2015 Segger Microcontroller GmbH & Co. KG        *
*                                                                    *
**********************************************************************
*                                                                    *
* Source file: score                                                 *
* Dimensions:  69 * 17                                               *
* NumColors:   65536 colors + 8 bit alpha channel                    *
*                                                                    *
**********************************************************************
*/

#include <stdlib.h>

#include "GUI.h"

#ifndef GUI_CONST_STORAGE
  #define GUI_CONST_STORAGE const
#endif

extern GUI_CONST_STORAGE GUI_BITMAP bmscore;

#if defined ( __ICCARM__ )
#pragma location="ExtQSPIFlashSection" 
#else
__attribute__((section(".ExtQSPIFlashSection")))  
#endif
static GUI_CONST_STORAGE unsigned char _acscore[] = {
  0xFF,0x00,0x00, 0xFF,0x00,0x00, 0xD1,0xFF,0xFF, 0x7E,0xFF,0xFF, 0x4E,0xFF,0xFF, 0x36,0xFF,0xFF, 0x40,0xFF,0xFF, 0x73,0xFF,0xFF, 0xD5,0xFF,0xFF, 0xFF,0x00,0x00, 0xFF,0x00,0x00, 0xFF,0x00,0x00, 0xFF,0x00,0x00, 0xFF,0x00,0x00, 0xFF,0x00,0x00, 
        0xFF,0x00,0x00, 0xD5,0xFF,0xFF, 0x6E,0xFF,0xFF, 0x30,0xFF,0xFF, 0x10,0xFF,0xFF, 0x05,0xFF,0xFF, 0x20,0xFF,0xFF, 0x6F,0xFF,0xFF, 0xE9,0xFF,0xFF, 0xFF,0x00,0x00, 0xFF,0x00,0x00, 0xFF,0x00,0x00, 0xFF,0x00,0x00, 0xFF,0x00,0x00, 0xFF,0x00,0x00, 
        0xFF,0x00,0x00, 0xFC,0xFF,0xFF, 0xB7,0xFF,0xFF, 0x7A,0xFF,0xFF, 0x53,0xFF,0xFF, 0x44,0xFF,0xFF, 0x4F,0xFF,0xFF, 0x78,0xFF,0xFF, 0xC7,0xFF,0xFF, 0xFF,0x00,0x00, 0xFF,0x00,0x00, 0xFF,0x00,0x00, 0xFF,0x00,0x00, 0xFF,0x00,0x00, 0x8A,0xFF,0xFF, 
        0x29,0xFF,0xFF, 0x22,0xFF,0xFF, 0x35,0xFF,0xFF, 0xB9,0xFF,0xFF, 0x96,0xFF,0xFF, 0x56,0xFF,0xFF, 0x44,0xFF,0xFF, 0x5D,0xFF,0xFF, 0xB0,0xFF,0xFF, 0xFE,0xFF,0xFF, 0xFF,0x00,0x00, 0xFF,0x00,0x00, 0xEC,0xFF,0xFF, 0x8D,0xFF,0xFF, 0x67,0xFF,0xFF, 
        0x66,0xFF,0xFF, 0x66,0xFF,0xFF, 0x66,0xFF,0xFF, 0x66,0xFF,0xFF, 0x66,0xFF,0xFF, 0x74,0xFF,0xFF, 0x77,0xFF,0xFF, 0x77,0xFF,0xFF, 0xA0,0xFF,0xFF,
  0xFE,0xFF,0xFF, 0x6D,0xFF,0xFF, 0x02,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x03,0xFF,0xFF, 0x93,0xFF,0xFF, 0xFF,0x00,0x00, 0xFF,0x00,0x00, 0xFF,0x00,0x00, 0xFF,0x00,0x00, 0xFC,0xFF,0xFF, 
        0x72,0xFF,0xFF, 0x02,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x18,0xFF,0xFF, 0xD7,0xFF,0xFF, 0xFF,0x00,0x00, 0xFF,0x00,0x00, 0xFF,0x00,0x00, 0xFF,0x00,0x00, 0xFF,0x00,0x00, 
        0xB2,0xFF,0xFF, 0x1C,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x48,0xFF,0xFF, 0xEC,0xFF,0xFF, 0xFF,0x00,0x00, 0xFF,0x00,0x00, 0xFF,0x00,0x00, 0x3C,0xFF,0xFF, 
        0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x02,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x4E,0xFF,0xFF, 0xF8,0xFF,0xFF, 0xFF,0x00,0x00, 0x94,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 
        0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x10,0xFF,0xFF,
  0x88,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x02,0xFF,0xFF, 0xC2,0xFF,0xFF, 0xFF,0x00,0x00, 0xFF,0x00,0x00, 0xFF,0x00,0x00, 0x60,0xFF,0xFF, 
        0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x2B,0xFF,0xFF, 0xFB,0xFF,0xFF, 0xFF,0x00,0x00, 0xFF,0x00,0x00, 0xFF,0x00,0x00, 0x9B,0xFF,0xFF, 
        0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x2E,0xFF,0xFF, 0xF7,0xFF,0xFF, 0xFF,0x00,0x00, 0xFF,0x00,0x00, 0x37,0xFF,0xFF, 
        0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x81,0xFF,0xFF, 0xFF,0x00,0x00, 0x93,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 
        0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x09,0xFF,0xFF,
  0x1F,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x32,0xFF,0xFF, 0x7B,0xFF,0xFF, 0x25,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x53,0xFF,0xFF, 0xFF,0x00,0x00, 0xFF,0x00,0x00, 0xA7,0xFF,0xFF, 0x00,0xFF,0xFF, 
        0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0xAA,0xFF,0xFF, 0xFF,0x00,0x00, 0xFF,0x00,0x00, 0xD4,0xFF,0xFF, 0x04,0xFF,0xFF, 
        0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x07,0xFF,0xFF, 0x0D,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x7D,0xFF,0xFF, 0xFF,0x00,0x00, 0xFF,0x00,0x00, 0x31,0xFF,0xFF, 
        0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x1E,0xFF,0xFF, 0xFF,0x00,0x00, 0x95,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 
        0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x37,0xFF,0xFF,
  0x0A,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0xCD,0xFF,0xFF, 0xFF,0x00,0x00, 0xD3,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x21,0xFF,0xFF, 0xFF,0x00,0x00, 0xFE,0xFF,0xFF, 0x2A,0xFF,0xFF, 0x00,0xFF,0xFF, 
        0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x74,0xFF,0xFF, 0xEB,0xFF,0xFF, 0xD3,0xFF,0xFF, 0x17,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x5E,0xFF,0xFF, 0xFF,0x00,0x00, 0xFF,0x00,0x00, 0x54,0xFF,0xFF, 0x00,0xFF,0xFF, 
        0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x01,0xFF,0xFF, 0x80,0xFF,0xFF, 0xF4,0xFF,0xFF, 0xFE,0xFF,0xFF, 0xCC,0xFF,0xFF, 0x2A,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x14,0xFF,0xFF, 0xF7,0xFF,0xFF, 0xFF,0x00,0x00, 0x2C,0xFF,0xFF, 
        0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x09,0xFF,0xFF, 0x7D,0xFF,0xFF, 0xC0,0xFF,0xFF, 0xA6,0xFF,0xFF, 0x07,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x02,0xFF,0xFF, 0xFA,0xFF,0xFF, 0x96,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 
        0x00,0xFF,0xFF, 0x81,0xFF,0xFF, 0xEE,0xFF,0xFF, 0xEE,0xFF,0xFF, 0xEE,0xFF,0xFF, 0xFD,0xFF,0xFF, 0xFF,0x00,0x00, 0xF6,0xFF,0xFF, 0xF7,0xFF,0xFF,
  0x19,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x43,0xFF,0xFF, 0xDE,0xFF,0xFF, 0xFF,0x00,0x00, 0xC3,0xFF,0xFF, 0xAA,0xFF,0xFF, 0xB8,0xFF,0xFF, 0xE3,0xFF,0xFF, 0xFF,0x00,0x00, 0xCB,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 
        0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x66,0xFF,0xFF, 0xFF,0x00,0x00, 0xFF,0x00,0x00, 0xFF,0x00,0x00, 0x8A,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x2C,0xFF,0xFF, 0xFF,0x00,0x00, 0xF7,0xFF,0xFF, 0x09,0xFF,0xFF, 0x00,0xFF,0xFF, 
        0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x6A,0xFF,0xFF, 0xFF,0x00,0x00, 0xFF,0x00,0x00, 0xFF,0x00,0x00, 0xFF,0x00,0x00, 0xD4,0xFF,0xFF, 0x02,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0xBE,0xFF,0xFF, 0xFF,0x00,0x00, 0x27,0xFF,0xFF, 
        0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0xA9,0xFF,0xFF, 0xFF,0x00,0x00, 0xFF,0x00,0x00, 0xFF,0x00,0x00, 0x2B,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x01,0xFF,0xFF, 0xFD,0xFF,0xFF, 0x98,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 
        0x00,0xFF,0xFF, 0x8A,0xFF,0xFF, 0xFF,0x00,0x00, 0xFF,0x00,0x00, 0xFF,0x00,0x00, 0xFF,0x00,0x00, 0xFF,0x00,0x00, 0xFF,0x00,0x00, 0xFF,0x00,0x00,
  0x63,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x04,0xFF,0xFF, 0x5F,0xFF,0xFF, 0xE8,0xFF,0xFF, 0xFF,0x00,0x00, 0xFF,0x00,0x00, 0xFF,0x00,0x00, 0xFF,0x00,0x00, 0x8A,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 
        0x00,0xFF,0xFF, 0x01,0xFF,0xFF, 0xDC,0xFF,0xFF, 0xFF,0x00,0x00, 0xFF,0x00,0x00, 0xFF,0x00,0x00, 0xB3,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x45,0xFF,0xFF, 0xFF,0x00,0x00, 0xC1,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 
        0x00,0xFF,0xFF, 0x02,0xFF,0xFF, 0xE5,0xFF,0xFF, 0xFF,0x00,0x00, 0xFF,0x00,0x00, 0xFF,0x00,0x00, 0xFF,0x00,0x00, 0xFF,0x00,0x00, 0x3A,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x86,0xFF,0xFF, 0xFF,0x00,0x00, 0x22,0xFF,0xFF, 
        0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0xBA,0xFF,0xFF, 0xFF,0x00,0x00, 0xFF,0x00,0x00, 0xEF,0xFF,0xFF, 0x09,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x22,0xFF,0xFF, 0xFF,0x00,0x00, 0x99,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 
        0x00,0xFF,0xFF, 0x86,0xFF,0xFF, 0xFF,0x00,0x00, 0xFF,0x00,0x00, 0xFF,0x00,0x00, 0xFF,0x00,0x00, 0xFF,0x00,0x00, 0xFF,0x00,0x00, 0xFF,0x00,0x00,
  0xE5,0xFF,0xFF, 0x1E,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x10,0xFF,0xFF, 0x99,0xFF,0xFF, 0xFF,0x00,0x00, 0xFF,0x00,0x00, 0xFF,0x00,0x00, 0x61,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 
        0x00,0xFF,0xFF, 0x34,0xFF,0xFF, 0xFF,0x00,0x00, 0xFF,0x00,0x00, 0xFF,0x00,0x00, 0xFF,0x00,0x00, 0xFB,0xFF,0xFF, 0xBE,0xFF,0xFF, 0xB3,0xFF,0xFF, 0xD6,0xFF,0xFF, 0xFF,0x00,0x00, 0xFF,0x00,0x00, 0xA9,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 
        0x00,0xFF,0xFF, 0x2D,0xFF,0xFF, 0xFF,0x00,0x00, 0xFF,0x00,0x00, 0xFF,0x00,0x00, 0xFF,0x00,0x00, 0xFF,0x00,0x00, 0xFF,0x00,0x00, 0x60,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x6C,0xFF,0xFF, 0xFF,0x00,0x00, 0x1D,0xFF,0xFF, 
        0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0xB7,0xFF,0xFF, 0xFF,0x00,0x00, 0xFB,0xFF,0xFF, 0x51,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x71,0xFF,0xFF, 0xFF,0x00,0x00, 0x9B,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 
        0x00,0xFF,0xFF, 0x12,0xFF,0xFF, 0x22,0xFF,0xFF, 0x22,0xFF,0xFF, 0x2F,0xFF,0xFF, 0x33,0xFF,0xFF, 0x2B,0xFF,0xFF, 0xAC,0xFF,0xFF, 0xFF,0x00,0x00,
  0xFF,0x00,0x00, 0xEB,0xFF,0xFF, 0x6F,0xFF,0xFF, 0x0D,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x75,0xFF,0xFF, 0xFF,0x00,0x00, 0xFF,0x00,0x00, 0x4C,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 
        0x00,0xFF,0xFF, 0x51,0xFF,0xFF, 0xFF,0x00,0x00, 0xFF,0x00,0x00, 0xFF,0x00,0x00, 0xFF,0x00,0x00, 0xFF,0x00,0x00, 0xFF,0x00,0x00, 0xFF,0x00,0x00, 0xFF,0x00,0x00, 0xFF,0x00,0x00, 0xFF,0x00,0x00, 0x9D,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 
        0x00,0xFF,0xFF, 0x4A,0xFF,0xFF, 0xFF,0x00,0x00, 0xFF,0x00,0x00, 0xFF,0x00,0x00, 0xFF,0x00,0x00, 0xFF,0x00,0x00, 0xFF,0x00,0x00, 0x79,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x5C,0xFF,0xFF, 0xFF,0x00,0x00, 0x17,0xFF,0xFF, 
        0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0xB4,0xFF,0xFF, 0xC3,0xFF,0xFF, 0x30,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x11,0xFF,0xFF, 0xE5,0xFF,0xFF, 0xFF,0x00,0x00, 0x9C,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 
        0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x6B,0xFF,0xFF, 0xFF,0x00,0x00,
  0xFF,0x00,0x00, 0xFF,0x00,0x00, 0xFF,0x00,0x00, 0xF3,0xFF,0xFF, 0x93,0xFF,0xFF, 0x1D,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0xA8,0xFF,0xFF, 0xFF,0x00,0x00, 0x43,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 
        0x00,0xFF,0xFF, 0x5D,0xFF,0xFF, 0xFF,0x00,0x00, 0xFF,0x00,0x00, 0xFF,0x00,0x00, 0xFF,0x00,0x00, 0xFF,0x00,0x00, 0xFF,0x00,0x00, 0xFF,0x00,0x00, 0xFF,0x00,0x00, 0xFF,0x00,0x00, 0xFF,0x00,0x00, 0xA1,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 
        0x00,0xFF,0xFF, 0x41,0xFF,0xFF, 0xFF,0x00,0x00, 0xFF,0x00,0x00, 0xFF,0x00,0x00, 0xFF,0x00,0x00, 0xFF,0x00,0x00, 0xFF,0x00,0x00, 0x7D,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x5F,0xFF,0xFF, 0xFF,0x00,0x00, 0x12,0xFF,0xFF, 
        0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x19,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x0A,0xFF,0xFF, 0xBC,0xFF,0xFF, 0xFF,0x00,0x00, 0xFF,0x00,0x00, 0x9E,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 
        0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x6B,0xFF,0xFF, 0xFF,0x00,0x00,
  0xD5,0xFF,0xFF, 0xA2,0xFF,0xFF, 0x9F,0xFF,0xFF, 0xCF,0xFF,0xFF, 0xFF,0x00,0x00, 0xF4,0xFF,0xFF, 0x41,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x23,0xFF,0xFF, 0xFE,0xFF,0xFF, 0x5C,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 
        0x00,0xFF,0xFF, 0x33,0xFF,0xFF, 0xFF,0x00,0x00, 0xFF,0x00,0x00, 0xFF,0x00,0x00, 0xFF,0x00,0x00, 0xFF,0x00,0x00, 0xEE,0xFF,0xFF, 0x9B,0xFF,0xFF, 0xB2,0xFF,0xFF, 0xFB,0xFF,0xFF, 0xFF,0x00,0x00, 0xBB,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 
        0x00,0xFF,0xFF, 0x11,0xFF,0xFF, 0xFE,0xFF,0xFF, 0xFF,0x00,0x00, 0xFF,0x00,0x00, 0xFF,0x00,0x00, 0xFF,0x00,0x00, 0xFF,0x00,0x00, 0x5A,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x72,0xFF,0xFF, 0xFF,0x00,0x00, 0x0D,0xFF,0xFF, 
        0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x21,0xFF,0xFF, 0xC9,0xFF,0xFF, 0xFF,0x00,0x00, 0xFF,0x00,0x00, 0xFF,0x00,0x00, 0x9F,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 
        0x00,0xFF,0xFF, 0x12,0xFF,0xFF, 0x22,0xFF,0xFF, 0x22,0xFF,0xFF, 0x22,0xFF,0xFF, 0x22,0xFF,0xFF, 0x22,0xFF,0xFF, 0xB1,0xFF,0xFF, 0xFF,0x00,0x00,
  0x11,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x03,0xFF,0xFF, 0xEB,0xFF,0xFF, 0xFF,0x00,0x00, 0xD9,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0xE1,0xFF,0xFF, 0x8F,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 
        0x00,0xFF,0xFF, 0x01,0xFF,0xFF, 0xC0,0xFF,0xFF, 0xFF,0x00,0x00, 0xFF,0x00,0x00, 0xFF,0x00,0x00, 0xFD,0xFF,0xFF, 0x42,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x31,0xFF,0xFF, 0xF0,0xFF,0xFF, 0xE3,0xFF,0xFF, 0x01,0xFF,0xFF, 0x00,0xFF,0xFF, 
        0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0xA2,0xFF,0xFF, 0xFF,0x00,0x00, 0xFF,0x00,0x00, 0xFF,0x00,0x00, 0xFF,0x00,0x00, 0xF0,0xFF,0xFF, 0x11,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x9D,0xFF,0xFF, 0xFF,0x00,0x00, 0x08,0xFF,0xFF, 
        0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x60,0xFF,0xFF, 0xF3,0xFF,0xFF, 0xFF,0x00,0x00, 0xFF,0x00,0x00, 0xFF,0x00,0x00, 0xA0,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 
        0x00,0xFF,0xFF, 0x84,0xFF,0xFF, 0xFF,0x00,0x00, 0xFF,0x00,0x00, 0xFF,0x00,0x00, 0xFF,0x00,0x00, 0xFF,0x00,0x00, 0xFF,0x00,0x00, 0xFF,0x00,0x00,
  0x1E,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x7F,0xFF,0xFF, 0xFE,0xFF,0xFF, 0xE7,0xFF,0xFF, 0x01,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0xC3,0xFF,0xFF, 0xE0,0xFF,0xFF, 0x04,0xFF,0xFF, 0x00,0xFF,0xFF, 
        0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x13,0xFF,0xFF, 0xA5,0xFF,0xFF, 0xE6,0xFF,0xFF, 0xE0,0xFF,0xFF, 0x66,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0xBB,0xFF,0xFF, 0xFF,0x00,0x00, 0x36,0xFF,0xFF, 0x00,0xFF,0xFF, 
        0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x13,0xFF,0xFF, 0xC0,0xFF,0xFF, 0xFF,0x00,0x00, 0xFF,0x00,0x00, 0xEF,0xFF,0xFF, 0x4C,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x01,0xFF,0xFF, 0xE2,0xFF,0xFF, 0xFF,0x00,0x00, 0x03,0xFF,0xFF, 
        0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x12,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x22,0xFF,0xFF, 0xC5,0xFF,0xFF, 0xFF,0x00,0x00, 0xFF,0x00,0x00, 0xA2,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 
        0x00,0xFF,0xFF, 0x7E,0xFF,0xFF, 0xFF,0x00,0x00, 0xFF,0x00,0x00, 0xFF,0x00,0x00, 0xFF,0x00,0x00, 0xFF,0x00,0x00, 0xFF,0x00,0x00, 0xFF,0x00,0x00,
  0x58,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x17,0xFF,0xFF, 0x0E,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0xE4,0xFF,0xFF, 0xFF,0x00,0x00, 0x6A,0xFF,0xFF, 0x00,0xFF,0xFF, 
        0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x20,0xFF,0xFF, 0xFA,0xFF,0xFF, 0xFF,0x00,0x00, 0xAA,0xFF,0xFF, 0x00,0xFF,0xFF, 
        0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x03,0xFF,0xFF, 0x39,0xFF,0xFF, 0x4F,0xFF,0xFF, 0x15,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x4D,0xFF,0xFF, 0xFF,0x00,0x00, 0xFC,0xFF,0xFF, 0x00,0xFF,0xFF, 
        0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0xBA,0xFF,0xFF, 0x50,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x04,0xFF,0xFF, 0x82,0xFF,0xFF, 0xFD,0xFF,0xFF, 0xA3,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 
        0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x48,0xFF,0xFF,
  0xC5,0xFF,0xFF, 0x02,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x2D,0xFF,0xFF, 0xFF,0x00,0x00, 0xFF,0x00,0x00, 0xF5,0xFF,0xFF, 0x34,0xFF,0xFF, 
        0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x06,0xFF,0xFF, 0xC0,0xFF,0xFF, 0xFF,0x00,0x00, 0xFF,0x00,0x00, 0xFF,0x00,0x00, 0x5A,0xFF,0xFF, 
        0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x12,0xFF,0xFF, 0xDE,0xFF,0xFF, 0xFF,0x00,0x00, 0xF7,0xFF,0xFF, 0x00,0xFF,0xFF, 
        0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0xC7,0xFF,0xFF, 0xFE,0xFF,0xFF, 0x8D,0xFF,0xFF, 0x06,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x05,0xFF,0xFF, 0xF0,0xFF,0xFF, 0xA5,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 
        0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x13,0xFF,0xFF,
  0xFF,0x00,0x00, 0x90,0xFF,0xFF, 0x01,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x0E,0xFF,0xFF, 0xC8,0xFF,0xFF, 0xFF,0x00,0x00, 0xFF,0x00,0x00, 0xFF,0x00,0x00, 0xF2,0xFF,0xFF, 
        0x59,0xFF,0xFF, 0x01,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x20,0xFF,0xFF, 0xC0,0xFF,0xFF, 0xFF,0x00,0x00, 0xFF,0x00,0x00, 0xFF,0x00,0x00, 0xFF,0x00,0x00, 0xFB,0xFF,0xFF, 
        0x6E,0xFF,0xFF, 0x03,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x32,0xFF,0xFF, 0xD2,0xFF,0xFF, 0xFF,0x00,0x00, 0xFF,0x00,0x00, 0xF2,0xFF,0xFF, 0x00,0xFF,0xFF, 
        0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0xC4,0xFF,0xFF, 0xFF,0x00,0x00, 0xFF,0x00,0x00, 0xCC,0xFF,0xFF, 0x26,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x73,0xFF,0xFF, 0xFF,0x00,0x00, 0xA6,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 
        0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x00,0xFF,0xFF, 0x17,0xFF,0xFF,
  0xFF,0x00,0x00, 0xFF,0x00,0x00, 0xCD,0xFF,0xFF, 0x63,0xFF,0xFF, 0x25,0xFF,0xFF, 0x06,0xFF,0xFF, 0x03,0xFF,0xFF, 0x1F,0xFF,0xFF, 0x62,0xFF,0xFF, 0xDC,0xFF,0xFF, 0xFF,0x00,0x00, 0xFF,0x00,0x00, 0xFF,0x00,0x00, 0xFF,0x00,0x00, 0xFF,0x00,0x00, 
        0xFF,0x00,0x00, 0xCE,0xFF,0xFF, 0x79,0xFF,0xFF, 0x4A,0xFF,0xFF, 0x41,0xFF,0xFF, 0x47,0xFF,0xFF, 0x66,0xFF,0xFF, 0xA9,0xFF,0xFF, 0xF7,0xFF,0xFF, 0xFF,0x00,0x00, 0xFF,0x00,0x00, 0xFF,0x00,0x00, 0xFF,0x00,0x00, 0xFF,0x00,0x00, 0xFF,0x00,0x00, 
        0xFF,0x00,0x00, 0xD6,0xFF,0xFF, 0x79,0xFF,0xFF, 0x43,0xFF,0xFF, 0x25,0xFF,0xFF, 0x23,0xFF,0xFF, 0x38,0xFF,0xFF, 0x61,0xFF,0xFF, 0xAC,0xFF,0xFF, 0xFC,0xFF,0xFF, 0xFF,0x00,0x00, 0xFF,0x00,0x00, 0xFF,0x00,0x00, 0xFC,0xFF,0xFF, 0x6A,0xFF,0xFF, 
        0x34,0xFF,0xFF, 0x37,0xFF,0xFF, 0x69,0xFF,0xFF, 0xF1,0xFF,0xFF, 0xFF,0x00,0x00, 0xFF,0x00,0x00, 0xFF,0x00,0x00, 0xF3,0xFF,0xFF, 0x67,0xFF,0xFF, 0x68,0xFF,0xFF, 0xFA,0xFF,0xFF, 0xFF,0x00,0x00, 0xD3,0xFF,0xFF, 0x48,0xFF,0xFF, 0x33,0xFF,0xFF, 
        0x33,0xFF,0xFF, 0x33,0xFF,0xFF, 0x33,0xFF,0xFF, 0x33,0xFF,0xFF, 0x33,0xFF,0xFF, 0x33,0xFF,0xFF, 0x33,0xFF,0xFF, 0x33,0xFF,0xFF, 0x77,0xFF,0xFF
};

GUI_CONST_STORAGE GUI_BITMAP bmscore = {
  69, /* xSize */
  17, /* ySize */
  207, /* BytesPerLine */
  24, /* BitsPerPixel */
  (unsigned char *)_acscore,  /* Pointer to picture data */
  NULL,  /* Pointer to palette */
  GUI_DRAW_BMPAM565
};

/*************************** End of file ****************************/
