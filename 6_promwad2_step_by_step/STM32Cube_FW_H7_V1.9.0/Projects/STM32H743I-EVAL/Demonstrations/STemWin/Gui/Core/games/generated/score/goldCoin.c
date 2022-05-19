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
* Source file: goldCoin                                              *
* Dimensions:  30 * 30                                               *
* NumColors:   65536 colors + 8 bit alpha channel                    *
*                                                                    *
**********************************************************************
*/

#include <stdlib.h>

#include "GUI.h"

#ifndef GUI_CONST_STORAGE
  #define GUI_CONST_STORAGE const
#endif

extern GUI_CONST_STORAGE GUI_BITMAP bmgoldCoin;

#if defined ( __ICCARM__ )
#pragma location="ExtQSPIFlashSection" 
#else
__attribute__((section(".ExtQSPIFlashSection")))  
#endif
static GUI_CONST_STORAGE unsigned long _acgoldCoin[] = {
  0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x03FFFF00, 0x3EFBDA04, 0x8DFBDB04, 0xC7FCDA04, 0xE9FCDA04, 0xFAFCDA04, 0xFAFCDA04, 0xE9FCDA04, 0xC7FCDA04, 0x8CFBDB04, 0x3DFBD904, 0x03FFFF00, 
        0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
  0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x09FFE300, 0x78FDDB04, 0xEAFCDA04, 0xFFFCDA04, 0xFFFCDA04, 0xFFFCDA04, 0xFFFCDA04, 0xFFFCDA04, 0xFFFCDA04, 0xFFFCDA04, 0xFFFCDA04, 0xFFFCDA04, 0xFFFCDA04, 0xE9FCDA04, 
        0x76FDDA04, 0x09FFE300, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
  0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x56FCDB03, 0xE4FCDA04, 0xFFFCDA04, 0xFFFCDA04, 0xFFFCDA04, 0xFFFCDA04, 0xFFFCDA04, 0xFFFCDA04, 0xFFFCDA04, 0xFFFCDA04, 0xFFFCDA04, 0xFFFCDA04, 0xFFFCDA04, 0xFFFCDA04, 0xFFFCDA04, 
        0xFFFCDA04, 0xE2FCDA05, 0x54FCDB03, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
  0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x01FFFF00, 0x80FBD904, 0xFEFCDA04, 0xFFFCDA04, 0xFFFCDA04, 0xFFFCDA04, 0xFFFCDA04, 0xFFFCD604, 0xFFFCCA04, 0xFFFCC404, 0xFFFCBF04, 0xFFFCBF04, 0xFFFCC404, 0xFFFCCA04, 0xFFFCD704, 0xFFFCDA04, 0xFFFCDA04, 
        0xFFFCDA04, 0xFFFCDA04, 0xFEFCDA04, 0x88FBDA04, 0x02FFFF00, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
  0x00000000, 0x00000000, 0x00000000, 0x02FFFF00, 0xA9FCD905, 0xFFFCDA04, 0xFFFCDA04, 0xFFFCDA04, 0xFFFCDA04, 0xFFFCD004, 0xFFFCBA04, 0xFFFCAF04, 0xFFFCAE04, 0xFFFCAE04, 0xFFFCAE04, 0xFFFCAE04, 0xFFFCAE04, 0xFFFCAE04, 0xFFFCAF04, 0xFFFCBA04, 0xFFFCD004, 
        0xFFFCDA04, 0xFFFCDA04, 0xFFFCDA04, 0xFFFCDA04, 0xA8FCDB05, 0x02FFFF00, 0x00000000, 0x00000000, 0x00000000,
  0x00000000, 0x00000000, 0x00000000, 0x87FBD904, 0xFFFCDA04, 0xFFFCDA04, 0xFFFCDA04, 0xFFFCD804, 0xFFFCBD04, 0xFFFCAE04, 0xFFFCAE04, 0xFFFCAE04, 0xFFFCAE04, 0xFFFCAE04, 0xFFFCAE04, 0xFFFCAE04, 0xFFFCAE04, 0xFFFCAE04, 0xFFFCAE04, 0xFFFCAE04, 0xFFFCAE04, 
        0xFFFCBE04, 0xFFFCD904, 0xFFFCDA04, 0xFFFCDA04, 0xFFFCDA04, 0x84FBDA04, 0x00000000, 0x00000000, 0x00000000,
  0x00000000, 0x00000000, 0x56FCDB03, 0xFFFCDA04, 0xFFFCDA04, 0xFFFCDA04, 0xFFFCD504, 0xFFFCB604, 0xFFFCAE04, 0xFFFCAE04, 0xFFFCAE04, 0xFFFCAE04, 0xFFFCAE04, 0xFFFCAE04, 0xFFFCAE04, 0xFFFCAE04, 0xFFFCAE04, 0xFFFCAE04, 0xFFFCAE04, 0xFFFCAE04, 0xFFFCAE04, 
        0xFFFCAE04, 0xFFFCB604, 0xFFFCD504, 0xFFFCDA04, 0xFFFCDA04, 0xFEFCDA04, 0x53FCDA03, 0x00000000, 0x00000000,
  0x00000000, 0x09FFE300, 0xE2FCDA05, 0xFFFCDA04, 0xFFFCDA04, 0xFFFCD804, 0xFFFCB604, 0xFFFCAE04, 0xFFFCAE04, 0xFFFCAE04, 0xFFFCAE04, 0xFFFCAE04, 0xFFFCAE04, 0xFFFCAE04, 0xFFFCB204, 0xFFFCB204, 0xFFFCAE04, 0xFFFCAE04, 0xFFFCAE04, 0xFFFCAE04, 0xFFFCAE04, 
        0xFFFCAE04, 0xFFFCAE04, 0xFFFCB604, 0xFFFCD904, 0xFFFCDA04, 0xFFFCDA04, 0xE4FCDA04, 0x0AFFE600, 0x00000000,
  0x00000000, 0x79FDD904, 0xFFFCDA04, 0xFFFCDA04, 0xFFFCDA04, 0xFFFCBD04, 0xFFFCAE04, 0xFFFCAE04, 0xFFFCAE04, 0xFFFCAE04, 0xFFFCAE04, 0xFFFCAE04, 0xFFFCAE04, 0xFFFCAE04, 0xFFFCC704, 0xFFFCC604, 0xFFFCAE04, 0xFFFCAE04, 0xFFFCAE04, 0xFFFCAE04, 0xFFFCAE04, 
        0xFFFCAE04, 0xFFFCAE04, 0xFFFCAE04, 0xFFFCBE04, 0xFFFCDA04, 0xFFFCDA04, 0xFFFCDA04, 0x76FDDA04, 0x00000000,
  0x03FFFF00, 0xEAFCDA04, 0xFFFCDA04, 0xFFFCDA04, 0xFFFCD104, 0xFFFCAE04, 0xFFFCAE04, 0xFFFCAE04, 0xFFFCAE04, 0xFFFCAE04, 0xFFFCAE04, 0xFFFCAE04, 0xFFFCAE04, 0xFFFCB304, 0xFFFCD804, 0xFFFCD804, 0xFFFCB204, 0xFFFCAE04, 0xFFFCAE04, 0xFFFCAE04, 0xFFFCAE04, 
        0xFFFCAE04, 0xFFFCAE04, 0xFFFCAE04, 0xFFFCAE04, 0xFFFCD004, 0xFFFCDA04, 0xFFFCDA04, 0xE8FCDA04, 0x03FFFF00,
  0x3FFBDB04, 0xFFFCDA04, 0xFFFCDA04, 0xFFFCDA04, 0xFFFCBA04, 0xFFFCAE04, 0xFFFCAE04, 0xFFFCAE04, 0xFFFCAE04, 0xFFFCAE04, 0xFFFCAE04, 0xFFFCAE04, 0xFFFCAE04, 0xFFFCC704, 0xFFFCDA04, 0xFFFCDA04, 0xFFFCC604, 0xFFFCAE04, 0xFFFCAE04, 0xFFFCAE04, 0xFFFCAE04, 
        0xFFFCAE04, 0xFFFCAE04, 0xFFFCAE04, 0xFFFCAE04, 0xFFFCBA04, 0xFFFCDA04, 0xFFFCDA04, 0xFFFCDA04, 0x3CFBD904,
  0x8EFBD904, 0xFFFCDA04, 0xFFFCDA04, 0xFFFCD604, 0xFFFCAF04, 0xFFFCAE04, 0xFFFCAE04, 0xFFFCAE04, 0xFFFCAE04, 0xFFFCAE04, 0xFFFCAE04, 0xFFFCAE04, 0xFFFCB104, 0xFFFCD804, 0xFFFCDA04, 0xFFFCDA04, 0xFFFCD804, 0xFFFCB204, 0xFFFCAE04, 0xFFFCAE04, 0xFFFCAE04, 
        0xFFFCAE04, 0xFFFCAE04, 0xFFFCAE04, 0xFFFCAE04, 0xFFFCAF04, 0xFFFCD704, 0xFFFCDA04, 0xFFFCDA04, 0x8BFBDA04,
  0xC8FCDA04, 0xFFFCDA04, 0xFFFCDA04, 0xFFFCCA04, 0xFFFCAE04, 0xFFFCAE04, 0xFFFCAE04, 0xFFFCB304, 0xFFFCBC04, 0xFFFCC204, 0xFFFCC804, 0xFFFCCF04, 0xFFFCD504, 0xFFFCDA04, 0xFFFCDA04, 0xFFFCDA04, 0xFFFCDA04, 0xFFFCD504, 0xFFFCCF04, 0xFFFCC804, 0xFFFCC204, 
        0xFFFCBC04, 0xFFFCB304, 0xFFFCAE04, 0xFFFCAE04, 0xFFFCAE04, 0xFFFCC904, 0xFFFCDA04, 0xFFFCDA04, 0xC5FCD904,
  0xEAFCDA04, 0xFFFCDA04, 0xFFFCDA04, 0xFFFCC304, 0xFFFCAE04, 0xFFFCAE04, 0xFFFCAE04, 0xFFFCB104, 0xFFFCD104, 0xFFFCDA04, 0xFFFCDA04, 0xFFFCDA04, 0xFFFCDA04, 0xFFFCDA04, 0xFFFCDA04, 0xFFFCDA04, 0xFFFCDA04, 0xFFFCDA04, 0xFFFCDA04, 0xFFFCDA04, 0xFFFCDA04, 
        0xFFFCD204, 0xFFFCB204, 0xFFFCAE04, 0xFFFCAE04, 0xFFFCAE04, 0xFFFCC404, 0xFFFCDA04, 0xFFFCDA04, 0xE8FCDA04,
  0xF9FCDA04, 0xFFFCDA04, 0xFFFCDA04, 0xFFFCBF04, 0xFFFCAE04, 0xFFFCAE04, 0xFFFCAE04, 0xFFFCAE04, 0xFFFCB104, 0xFFFCD104, 0xFFFCDA04, 0xFFFCDA04, 0xFFFCDA04, 0xFFFCDA04, 0xFFFCDA04, 0xFFFCDA04, 0xFFFCDA04, 0xFFFCDA04, 0xFFFCDA04, 0xFFFCDA04, 0xFFFCD104, 
        0xFFFCB104, 0xFFFCAE04, 0xFFFCAE04, 0xFFFCAE04, 0xFFFCAE04, 0xFFFCBF04, 0xFFFCDA04, 0xFFFCDA04, 0xF8FCDA04,
  0xF9FCDA04, 0xFFFCDA04, 0xFFFCDA04, 0xFFFCBF04, 0xFFFCAE04, 0xFFFCAE04, 0xFFFCAE04, 0xFFFCAE04, 0xFFFCAE04, 0xFFFCB104, 0xFFFCD004, 0xFFFCDA04, 0xFFFCDA04, 0xFFFCDA04, 0xFFFCDA04, 0xFFFCDA04, 0xFFFCDA04, 0xFFFCDA04, 0xFFFCDA04, 0xFFFCD104, 0xFFFCB104, 
        0xFFFCAE04, 0xFFFCAE04, 0xFFFCAE04, 0xFFFCAE04, 0xFFFCAE04, 0xFFFCBF04, 0xFFFCDA04, 0xFFFCDA04, 0xF8FCDA04,
  0xEAFCDA04, 0xFFFCDA04, 0xFFFCDA04, 0xFFFCC304, 0xFFFCAE04, 0xFFFCAE04, 0xFFFCAE04, 0xFFFCAE04, 0xFFFCAE04, 0xFFFCAE04, 0xFFFCB004, 0xFFFCD504, 0xFFFCDA04, 0xFFFCDA04, 0xFFFCDA04, 0xFFFCDA04, 0xFFFCDA04, 0xFFFCDA04, 0xFFFCD504, 0xFFFCB004, 0xFFFCAE04, 
        0xFFFCAE04, 0xFFFCAE04, 0xFFFCAE04, 0xFFFCAE04, 0xFFFCAE04, 0xFFFCC404, 0xFFFCDA04, 0xFFFCDA04, 0xE8FCDA04,
  0xC7FCDA04, 0xFFFCDA04, 0xFFFCDA04, 0xFFFCCA04, 0xFFFCAE04, 0xFFFCAE04, 0xFFFCAE04, 0xFFFCAE04, 0xFFFCAE04, 0xFFFCAE04, 0xFFFCAE04, 0xFFFCD704, 0xFFFCDA04, 0xFFFCDA04, 0xFFFCDA04, 0xFFFCDA04, 0xFFFCDA04, 0xFFFCDA04, 0xFFFCD804, 0xFFFCAE04, 0xFFFCAE04, 
        0xFFFCAE04, 0xFFFCAE04, 0xFFFCAE04, 0xFFFCAE04, 0xFFFCAE04, 0xFFFCCA04, 0xFFFCDA04, 0xFFFCDA04, 0xC4FCDB04,
  0x8CFBDB04, 0xFFFCDA04, 0xFFFCDA04, 0xFFFCD704, 0xFFFCAE04, 0xFFFCAE04, 0xFFFCAE04, 0xFFFCAE04, 0xFFFCAE04, 0xFFFCAE04, 0xFFFCB304, 0xFFFCDA04, 0xFFFCDA04, 0xFFFCDA04, 0xFFFCDA04, 0xFFFCDA04, 0xFFFCDA04, 0xFFFCDA04, 0xFFFCDA04, 0xFFFCB404, 0xFFFCAE04, 
        0xFFFCAE04, 0xFFFCAE04, 0xFFFCAE04, 0xFFFCAE04, 0xFFFCAF04, 0xFFFCD704, 0xFFFCDA04, 0xFFFCDA04, 0x89FBDA04,
  0x3DFBD904, 0xFFFCDA04, 0xFFFCDA04, 0xFFFCDA04, 0xFFFCBA04, 0xFFFCAE04, 0xFFFCAE04, 0xFFFCAE04, 0xFFFCAE04, 0xFFFCAE04, 0xFFFCBB04, 0xFFFCDA04, 0xFFFCDA04, 0xFFFCD804, 0xFFFCC604, 0xFFFCC604, 0xFFFCD804, 0xFFFCDA04, 0xFFFCDA04, 0xFFFCBB04, 0xFFFCAE04, 
        0xFFFCAE04, 0xFFFCAE04, 0xFFFCAE04, 0xFFFCAE04, 0xFFFCBB04, 0xFFFCDA04, 0xFFFCDA04, 0xFFFCDA04, 0x3AFBDC04,
  0x03FFFF00, 0xE9FCDA04, 0xFFFCDA04, 0xFFFCDA04, 0xFFFCD104, 0xFFFCAE04, 0xFFFCAE04, 0xFFFCAE04, 0xFFFCAE04, 0xFFFCAE04, 0xFFFCC304, 0xFFFCD904, 0xFFFCC904, 0xFFFCB303, 0xFFFCAE04, 0xFFFCAE04, 0xFFFCB304, 0xFFFCC704, 0xFFFCD904, 0xFFFCC204, 0xFFFCAE04, 
        0xFFFCAE04, 0xFFFCAE04, 0xFFFCAE04, 0xFFFCAE04, 0xFFFCD004, 0xFFFCDA04, 0xFFFCDA04, 0xE8FCDA04, 0x02FFFF00,
  0x00000000, 0x76FDDA04, 0xFFFCDA04, 0xFFFCDA04, 0xFFFCDA04, 0xFFFCBE04, 0xFFFCAE04, 0xFFFCAE04, 0xFFFCAE04, 0xFFFCAE04, 0xFFFCBD04, 0xFFFCB404, 0xFFFCAE04, 0xFFFCAE04, 0xFFFCAE04, 0xFFFCAE04, 0xFFFCAE04, 0xFFFCAE04, 0xFFFCB404, 0xFFFCBD04, 0xFFFCAE04, 
        0xFFFCAE04, 0xFFFCAE04, 0xFFFCAE04, 0xFFFCBE04, 0xFFFCDA04, 0xFFFCDA04, 0xFFFCDA04, 0x73FDD904, 0x00000000,
  0x00000000, 0x08FFDF00, 0xE1FCDA05, 0xFFFCDA04, 0xFFFCDA04, 0xFFFCD804, 0xFFFCB704, 0xFFFCAE04, 0xFFFCAE04, 0xFFFCAE04, 0xFFFCAE04, 0xFFFCAE04, 0xFFFCAE04, 0xFFFCAE04, 0xFFFCAE04, 0xFFFCAE04, 0xFFFCAE04, 0xFFFCAE04, 0xFFFCAE04, 0xFFFCAE04, 0xFFFCAE04, 
        0xFFFCAE04, 0xFFFCAE04, 0xFFFCB604, 0xFFFCD904, 0xFFFCDA04, 0xFFFCDA04, 0xE3FCDA04, 0x09FFE300, 0x00000000,
  0x00000000, 0x00000000, 0x54FCDB03, 0xFEFCDA04, 0xFFFCDA04, 0xFFFCDA04, 0xFFFCD504, 0xFFFCB604, 0xFFFCAE04, 0xFFFCAE04, 0xFFFCAE04, 0xFFFCAE04, 0xFFFCAE04, 0xFFFCAE04, 0xFFFCAE04, 0xFFFCAE04, 0xFFFCAE04, 0xFFFCAE04, 0xFFFCAE04, 0xFFFCAE04, 0xFFFCAE04, 
        0xFFFCAE04, 0xFFFCB704, 0xFFFCD604, 0xFFFCDA04, 0xFFFCDA04, 0xFEFCDA04, 0x51FCD903, 0x00000000, 0x00000000,
  0x00000000, 0x00000000, 0x00000000, 0x85FBDB04, 0xFFFCDA04, 0xFFFCDA04, 0xFFFCDA04, 0xFFFCD904, 0xFFFCBE04, 0xFFFCAE04, 0xFFFCAE04, 0xFFFCAE04, 0xFFFCAE04, 0xFFFCAE04, 0xFFFCAE04, 0xFFFCAE04, 0xFFFCAE04, 0xFFFCAE04, 0xFFFCAE04, 0xFFFCAE04, 0xFFFCAE04, 
        0xFFFCBF04, 0xFFFCD804, 0xFFFCDA04, 0xFFFCDA04, 0xFFFCDA04, 0x82FBDA04, 0x00000000, 0x00000000, 0x00000000,
  0x00000000, 0x00000000, 0x00000000, 0x02FFFF00, 0xA8FCDB05, 0xFFFCDA04, 0xFFFCDA04, 0xFFFCDA04, 0xFFFCDA04, 0xFFFCD004, 0xFFFCBA04, 0xFFFCAF04, 0xFFFCAE04, 0xFFFCAE04, 0xFFFCAE04, 0xFFFCAE04, 0xFFFCAE04, 0xFFFCAE04, 0xFFFCAF04, 0xFFFCBB04, 0xFFFCD104, 
        0xFFFCDA04, 0xFFFCDA04, 0xFFFCDA04, 0xFFFCDA04, 0xA7FCDA05, 0x02FFFF00, 0x00000000, 0x00000000, 0x00000000,
  0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x01FFFF00, 0x80FBD904, 0xFEFCDA04, 0xFFFCDA04, 0xFFFCDA04, 0xFFFCDA04, 0xFFFCDA04, 0xFFFCD704, 0xFFFCC904, 0xFFFCC404, 0xFFFCBF04, 0xFFFCBF04, 0xFFFCC404, 0xFFFCCA04, 0xFFFCD704, 0xFFFCDA04, 0xFFFCDA04, 
        0xFFFCDA04, 0xFFFCDA04, 0xFEFCDA04, 0x88FBDA04, 0x02FFFF00, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
  0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x53FCDA03, 0xE2FCDA05, 0xFFFCDA04, 0xFFFCDA04, 0xFFFCDA04, 0xFFFCDA04, 0xFFFCDA04, 0xFFFCDA04, 0xFFFCDA04, 0xFFFCDA04, 0xFFFCDA04, 0xFFFCDA04, 0xFFFCDA04, 0xFFFCDA04, 0xFFFCDA04, 
        0xFFFCDA04, 0xE1FCDA05, 0x51FCD903, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
  0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x08FFDF00, 0x75FDDA04, 0xE8FCDA04, 0xFFFCDA04, 0xFFFCDA04, 0xFFFCDA04, 0xFFFCDA04, 0xFFFCDA04, 0xFFFCDA04, 0xFFFCDA04, 0xFFFCDA04, 0xFFFCDA04, 0xFFFCDA04, 0xE8FCDA04, 
        0x74FDDA04, 0x08FFDF00, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
  0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x02FFFF00, 0x3CFBD904, 0x8BFBDA04, 0xC5FCD904, 0xE8FCDA04, 0xF9FCDA04, 0xF9FCDA04, 0xE8FCDA04, 0xC5FCD904, 0x8AFBDA04, 0x3BFBD804, 0x02FFFF00, 
        0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000
};

GUI_CONST_STORAGE GUI_BITMAP bmgoldCoin = {
  30, /* xSize*/
  30, /* ySize */
  120, /* BytesPerLine */
  32, /* BitsPerPixel */
  (unsigned char *)_acgoldCoin,  /* Pointer to picture data */
  NULL,  /* Pointer to palette */
  GUI_DRAW_BMPM8888I
};

/*************************** End of file ****************************/
