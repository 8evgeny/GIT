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

        #if (GUI_USE_ARGB == 1)
  0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x1AFDDC04, 0x70FCDA04, 0xBBFCDA04, 0xECFCDA04, 0xFCFCDA04, 0xECFCDA04, 0xBAFCDA04, 0x70FCDA04, 0x1AFDDB04, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
  0x00000000, 0x00000000, 0x07FBDA04, 0x5DFCDA04, 0xDDFCDA04, 0xFCFCDA04, 0xFFFCD804, 0xFFFCD404, 0xFFFCD204, 0xFFFCD404, 0xFFFCD904, 0xFCFCDA04, 0xDCFCDA04, 0x5DFCDA04, 0x07FBDC04, 0x00000000, 0x00000000,
  0x00000000, 0x07FBDB04, 0x79FCDA04, 0xF1FCDA04, 0xFFFCD804, 0xFFFCCA04, 0xFFFCBA04, 0xFFFCB404, 0xFFFCB304, 0xFFFCB404, 0xFFFCBA04, 0xFFFCCA04, 0xFFFCD804, 0xF2FCDA04, 0x7AFCDB04, 0x07FBDC04, 0x00000000,
  0x00000000, 0x5EFCDA04, 0xF2FCDA04, 0xFFFCD704, 0xFFFCBC04, 0xFFFCAF04, 0xFFFCAE04, 0xFFFCAE04, 0xFFFCAE04, 0xFFFCAE04, 0xFFFCAE04, 0xFFFCAF04, 0xFFFCBD04, 0xFFFCD704, 0xF2FCDA04, 0x5DFCDA04, 0x00000000,
  0x1BFDDA04, 0xDDFCDA04, 0xFFFCD804, 0xFFFCBC04, 0xFFFCAE04, 0xFFFCAE04, 0xFFFCAE04, 0xFFFCAF04, 0xFFFCBC04, 0xFFFCAF04, 0xFFFCAE04, 0xFFFCAE04, 0xFFFCAE04, 0xFFFCBD04, 0xFFFCD804, 0xDDFCDA04, 0x1AFDDB04,
  0x71FCDA04, 0xFCFCDA04, 0xFFFCCA04, 0xFFFCAF04, 0xFFFCAE04, 0xFFFCAE04, 0xFFFCAE04, 0xFFFCB704, 0xFFFCD704, 0xFFFCB704, 0xFFFCAE04, 0xFFFCAE04, 0xFFFCAE04, 0xFFFCAF04, 0xFFFCCA04, 0xFBFCDA04, 0x6FFCDA04,
  0xBBFCDA04, 0xFFFCD804, 0xFFFCBA04, 0xFFFCAE04, 0xFFFCB004, 0xFFFCB204, 0xFFFCB504, 0xFFFCCA04, 0xFFFCDA04, 0xFFFCCA04, 0xFFFCB504, 0xFFFCB204, 0xFFFCB004, 0xFFFCAE04, 0xFFFCBA04, 0xFFFCD804, 0xB9FCDA04,
  0xEDFCDA04, 0xFFFCD404, 0xFFFCB404, 0xFFFCAE04, 0xFFFCBC04, 0xFFFCD104, 0xFFFCD604, 0xFFFCD904, 0xFFFCDA04, 0xFFFCD904, 0xFFFCD604, 0xFFFCD104, 0xFFFCBC04, 0xFFFCAE04, 0xFFFCB404, 0xFFFCD404, 0xEBFCDA04,
  0xFCFCDA04, 0xFFFCD204, 0xFFFCB304, 0xFFFCAE04, 0xFFFCAF04, 0xFFFCC604, 0xFFFCD904, 0xFFFCDA04, 0xFFFCDA04, 0xFFFCDA04, 0xFFFCD904, 0xFFFCC604, 0xFFFCAF04, 0xFFFCAE04, 0xFFFCB304, 0xFFFCD204, 0xFBFCDA04,
  0xECFCDA04, 0xFFFCD404, 0xFFFCB404, 0xFFFCAE04, 0xFFFCAE04, 0xFFFCAF04, 0xFFFCCE04, 0xFFFCDA04, 0xFFFCDA04, 0xFFFCDA04, 0xFFFCCF04, 0xFFFCAF04, 0xFFFCAE04, 0xFFFCAE04, 0xFFFCB404, 0xFFFCD404, 0xEBFCDA04,
  0xBAFCDA04, 0xFFFCD904, 0xFFFCBA04, 0xFFFCAE04, 0xFFFCAE04, 0xFFFCB004, 0xFFFCD104, 0xFFFCD904, 0xFFFCD504, 0xFFFCD904, 0xFFFCD104, 0xFFFCB004, 0xFFFCAE04, 0xFFFCAE04, 0xFFFCBA04, 0xFFFCD904, 0xB8FCDA04,
  0x70FCDA04, 0xFCFCDA04, 0xFFFCCA04, 0xFFFCAF04, 0xFFFCAE04, 0xFFFCB404, 0xFFFCCF04, 0xFFFCC304, 0xFFFCB604, 0xFFFCC304, 0xFFFCCE04, 0xFFFCB404, 0xFFFCAE04, 0xFFFCAF04, 0xFFFCCA04, 0xFBFCDA04, 0x6EFCDA04,
  0x1AFDDA04, 0xDCFCDA04, 0xFFFCD804, 0xFFFCBD04, 0xFFFCAE04, 0xFFFCB004, 0xFFFCB104, 0xFFFCAE04, 0xFFFCAE04, 0xFFFCAE04, 0xFFFCB104, 0xFFFCB004, 0xFFFCAE04, 0xFFFCBD04, 0xFFFCD804, 0xDCFCDA04, 0x19FDDA04,
  0x00000000, 0x5DFCDA04, 0xF2FCDA04, 0xFFFCD704, 0xFFFCBD04, 0xFFFCAF04, 0xFFFCAE04, 0xFFFCAE04, 0xFFFCAE04, 0xFFFCAE04, 0xFFFCAE04, 0xFFFCAF04, 0xFFFCBD04, 0xFFFCD704, 0xF2FCDA04, 0x5CFCDA04, 0x00000000,
  0x00000000, 0x07FBDD04, 0x79FCDB04, 0xF1FCDA04, 0xFFFCD804, 0xFFFCCA04, 0xFFFCBA04, 0xFFFCB404, 0xFFFCB304, 0xFFFCB404, 0xFFFCBA04, 0xFFFCCA04, 0xFFFCD804, 0xF2FCDA04, 0x79FCDA04, 0x07FBDC04, 0x00000000,
  0x00000000, 0x00000000, 0x07FBDA04, 0x5CFCDA04, 0xDCFCDA04, 0xFBFCDA04, 0xFFFCD804, 0xFFFCD404, 0xFFFCD204, 0xFFFCD404, 0xFFFCD904, 0xFBFCDA04, 0xDCFCDA04, 0x5CFCDA04, 0x07FBDC04, 0x00000000, 0x00000000,
  0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x1AFDDA04, 0x6FFCDA04, 0xB9FCDA04, 0xEBFCDA04, 0xFCFCDA04, 0xEBFCDA04, 0xB9FCDA04, 0x6FFCDA04, 0x19FDDA04, 0x00000000, 0x00000000, 0x00000000, 0x00000000
#else
  0xFF000000, 0xFF000000, 0xFF000000, 0xFF000000, 0xE504DCFD, 0x8F04DAFC, 0x4404DAFC, 0x1304DAFC, 0x0304DAFC, 0x1304DAFC, 0x4504DAFC, 0x8F04DAFC, 0xE504DBFD, 0xFF000000, 0xFF000000, 0xFF000000, 0xFF000000,
  0xFF000000, 0xFF000000, 0xF804DAFB, 0xA204DAFC, 0x2204DAFC, 0x0304DAFC, 0x0004D8FC, 0x0004D4FC, 0x0004D2FC, 0x0004D4FC, 0x0004D9FC, 0x0304DAFC, 0x2304DAFC, 0xA204DAFC, 0xF804DCFB, 0xFF000000, 0xFF000000,
  0xFF000000, 0xF804DBFB, 0x8604DAFC, 0x0E04DAFC, 0x0004D8FC, 0x0004CAFC, 0x0004BAFC, 0x0004B4FC, 0x0004B3FC, 0x0004B4FC, 0x0004BAFC, 0x0004CAFC, 0x0004D8FC, 0x0D04DAFC, 0x8504DBFC, 0xF804DCFB, 0xFF000000,
  0xFF000000, 0xA104DAFC, 0x0D04DAFC, 0x0004D7FC, 0x0004BCFC, 0x0004AFFC, 0x0004AEFC, 0x0004AEFC, 0x0004AEFC, 0x0004AEFC, 0x0004AEFC, 0x0004AFFC, 0x0004BDFC, 0x0004D7FC, 0x0D04DAFC, 0xA204DAFC, 0xFF000000,
  0xE404DAFD, 0x2204DAFC, 0x0004D8FC, 0x0004BCFC, 0x0004AEFC, 0x0004AEFC, 0x0004AEFC, 0x0004AFFC, 0x0004BCFC, 0x0004AFFC, 0x0004AEFC, 0x0004AEFC, 0x0004AEFC, 0x0004BDFC, 0x0004D8FC, 0x2204DAFC, 0xE504DBFD,
  0x8E04DAFC, 0x0304DAFC, 0x0004CAFC, 0x0004AFFC, 0x0004AEFC, 0x0004AEFC, 0x0004AEFC, 0x0004B7FC, 0x0004D7FC, 0x0004B7FC, 0x0004AEFC, 0x0004AEFC, 0x0004AEFC, 0x0004AFFC, 0x0004CAFC, 0x0404DAFC, 0x9004DAFC,
  0x4404DAFC, 0x0004D8FC, 0x0004BAFC, 0x0004AEFC, 0x0004B0FC, 0x0004B2FC, 0x0004B5FC, 0x0004CAFC, 0x0004DAFC, 0x0004CAFC, 0x0004B5FC, 0x0004B2FC, 0x0004B0FC, 0x0004AEFC, 0x0004BAFC, 0x0004D8FC, 0x4604DAFC,
  0x1204DAFC, 0x0004D4FC, 0x0004B4FC, 0x0004AEFC, 0x0004BCFC, 0x0004D1FC, 0x0004D6FC, 0x0004D9FC, 0x0004DAFC, 0x0004D9FC, 0x0004D6FC, 0x0004D1FC, 0x0004BCFC, 0x0004AEFC, 0x0004B4FC, 0x0004D4FC, 0x1404DAFC,
  0x0304DAFC, 0x0004D2FC, 0x0004B3FC, 0x0004AEFC, 0x0004AFFC, 0x0004C6FC, 0x0004D9FC, 0x0004DAFC, 0x0004DAFC, 0x0004DAFC, 0x0004D9FC, 0x0004C6FC, 0x0004AFFC, 0x0004AEFC, 0x0004B3FC, 0x0004D2FC, 0x0404DAFC,
  0x1304DAFC, 0x0004D4FC, 0x0004B4FC, 0x0004AEFC, 0x0004AEFC, 0x0004AFFC, 0x0004CEFC, 0x0004DAFC, 0x0004DAFC, 0x0004DAFC, 0x0004CFFC, 0x0004AFFC, 0x0004AEFC, 0x0004AEFC, 0x0004B4FC, 0x0004D4FC, 0x1404DAFC,
  0x4504DAFC, 0x0004D9FC, 0x0004BAFC, 0x0004AEFC, 0x0004AEFC, 0x0004B0FC, 0x0004D1FC, 0x0004D9FC, 0x0004D5FC, 0x0004D9FC, 0x0004D1FC, 0x0004B0FC, 0x0004AEFC, 0x0004AEFC, 0x0004BAFC, 0x0004D9FC, 0x4704DAFC,
  0x8F04DAFC, 0x0304DAFC, 0x0004CAFC, 0x0004AFFC, 0x0004AEFC, 0x0004B4FC, 0x0004CFFC, 0x0004C3FC, 0x0004B6FC, 0x0004C3FC, 0x0004CEFC, 0x0004B4FC, 0x0004AEFC, 0x0004AFFC, 0x0004CAFC, 0x0404DAFC, 0x9104DAFC,
  0xE504DAFD, 0x2304DAFC, 0x0004D8FC, 0x0004BDFC, 0x0004AEFC, 0x0004B0FC, 0x0004B1FC, 0x0004AEFC, 0x0004AEFC, 0x0004AEFC, 0x0004B1FC, 0x0004B0FC, 0x0004AEFC, 0x0004BDFC, 0x0004D8FC, 0x2304DAFC, 0xE604DAFD,
  0xFF000000, 0xA204DAFC, 0x0D04DAFC, 0x0004D7FC, 0x0004BDFC, 0x0004AFFC, 0x0004AEFC, 0x0004AEFC, 0x0004AEFC, 0x0004AEFC, 0x0004AEFC, 0x0004AFFC, 0x0004BDFC, 0x0004D7FC, 0x0D04DAFC, 0xA304DAFC, 0xFF000000,
  0xFF000000, 0xF804DDFB, 0x8604DBFC, 0x0E04DAFC, 0x0004D8FC, 0x0004CAFC, 0x0004BAFC, 0x0004B4FC, 0x0004B3FC, 0x0004B4FC, 0x0004BAFC, 0x0004CAFC, 0x0004D8FC, 0x0D04DAFC, 0x8604DAFC, 0xF804DCFB, 0xFF000000,
  0xFF000000, 0xFF000000, 0xF804DAFB, 0xA304DAFC, 0x2304DAFC, 0x0404DAFC, 0x0004D8FC, 0x0004D4FC, 0x0004D2FC, 0x0004D4FC, 0x0004D9FC, 0x0404DAFC, 0x2304DAFC, 0xA304DAFC, 0xF804DCFB, 0xFF000000, 0xFF000000,
  0xFF000000, 0xFF000000, 0xFF000000, 0xFF000000, 0xE504DAFD, 0x9004DAFC, 0x4604DAFC, 0x1404DAFC, 0x0304DAFC, 0x1404DAFC, 0x4604DAFC, 0x9004DAFC, 0xE604DAFD, 0xFF000000, 0xFF000000, 0xFF000000, 0xFF000000
#endif
};

GUI_CONST_STORAGE GUI_BITMAP bmgoldCoin = {
  17, // xSize
  17, // ySize
  68, // BytesPerLine
  32, // BitsPerPixel
  (unsigned char *)_acgoldCoin,  // Pointer to picture data
  NULL,  // Pointer to palette
#if (GUI_USE_ARGB == 1)
  GUI_DRAW_BMPM8888I
#else
  GUI_DRAW_BMP8888
#endif
};

/*************************** End of file ****************************/
