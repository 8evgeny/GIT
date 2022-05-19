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
* Source file: ANIM6                                                 *
* Dimensions:  28 * 26                                               *
* NumColors:   65536 colors + 8 bit alpha channel                    *
*                                                                    *
**********************************************************************
*/

#include <stdlib.h>

#include "GUI.h"

#ifndef GUI_CONST_STORAGE
  #define GUI_CONST_STORAGE const
#endif

extern GUI_CONST_STORAGE GUI_BITMAP bmCoin6;

#if defined ( __ICCARM__ )
#pragma location="ExtQSPIFlashSection" 
#else
__attribute__((section(".ExtQSPIFlashSection")))  
#endif
static GUI_CONST_STORAGE unsigned long _accoin6[] = {
#if (GUI_USE_ARGB == 1)
  0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
  0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
  0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x02FCC404, 0x12FCD704, 0x36FCDA04, 0x42FCDA04, 0x1EFCDA04, 0x07FCDA04, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
  0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x0FFCAE04, 0x6FFCC404, 0xCDFCD704, 0xFFFCD104, 0xFFFCCE04, 0xEEFCD604, 0x96FCDA04, 0x16FCDA04, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
  0x00000000, 0x00000000, 0x00000000, 0x15FCAE04, 0xC2FCB704, 0xFDFCD004, 0xFFFCBB04, 0xFFFCAF04, 0xFFFCB004, 0xFFFCB204, 0xFEFCC604, 0xC2FCD904, 0x0BFCDA04, 0x00000000, 0x00000000, 0x00000000,
  0x00000000, 0x00000000, 0x04FCAE04, 0x89FCAE04, 0xFFFCC404, 0xFFFCC604, 0xFFFCB104, 0xFFFCB104, 0xFFFCC304, 0xFFFCB604, 0xFFFCB004, 0xFFFCCA04, 0x6BFCDA04, 0x02FCDA04, 0x00000000, 0x00000000,
  0x00000000, 0x00000000, 0x15FCAE04, 0xD2FCAE04, 0xFFFCCC04, 0xFFFCBC04, 0xFFFCB804, 0xFFFCCF04, 0xFFFCD804, 0xFFFCC604, 0xFFFCB104, 0xFFFCB604, 0xBFFCDA04, 0x0FFCDA04, 0x00000000, 0x00000000,
  0x00000000, 0x00000000, 0x1BFCAE04, 0xE4FCAE04, 0xFFFCCD04, 0xFFFCBB04, 0xFFFCB004, 0xFFFCCE04, 0xFFFCDA04, 0xFFFCD904, 0xFFFCC704, 0xFFFCAF04, 0xE4FCD704, 0x1BFCDA04, 0x00000000, 0x00000000,
  0x00000000, 0x00000000, 0x1AFCAE04, 0xDFFCAE04, 0xFFFCC504, 0xFFFCC304, 0xFFFCB104, 0xFFFCCD04, 0xFFFCD304, 0xFFFCCE04, 0xFFFCB004, 0xFFFCAE04, 0xE4FCD704, 0x1BFCDA04, 0x00000000, 0x00000000,
  0x00000000, 0x00000000, 0x0CFCAE04, 0xB2FCAE04, 0xFFFCB904, 0xFFFCCF04, 0xFFFCB504, 0xFFFCB404, 0xFFFCB504, 0xFFFCC104, 0xFFFCAE04, 0xFFFCB704, 0xC7FCDA04, 0x12FCDA04, 0x00000000, 0x00000000,
  0x00000000, 0x00000000, 0x00000000, 0x4BFCAE04, 0xFDFCAE04, 0xFFFCC804, 0xFFFCCB04, 0xFFFCB204, 0xFFFCAF04, 0xFFFCAF04, 0xFFFCB204, 0xFEFCCD04, 0x6DFCDA04, 0x02FCDA04, 0x00000000, 0x00000000,
  0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x8BFCAE04, 0xF9FCB104, 0xFFFCC404, 0xFFFCD404, 0xFFFCCA04, 0xFFFCC904, 0xF2FCD404, 0x94FCDA04, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
  0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x08FCAE04, 0x5BFCAE04, 0xBCFCAF04, 0xD8FCB604, 0xD1FCC204, 0x9AFCC904, 0x35FCD404, 0x06FCDA04, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
  0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x0FFCAE04, 0x23FCAE04, 0x1DFCAE04, 0x03FCAE04, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
  0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000
#else
  0xFF000000, 0xFF000000, 0xFF000000, 0xFF000000, 0xFF000000, 0xFF000000, 0xFF000000, 0xFF000000, 0xFF000000, 0xFF000000, 0xFF000000, 0xFF000000, 0xFF000000, 0xFF000000, 0xFF000000, 0xFF000000,
  0xFF000000, 0xFF000000, 0xFF000000, 0xFF000000, 0xFF000000, 0xFF000000, 0xFF000000, 0xFF000000, 0xFF000000, 0xFF000000, 0xFF000000, 0xFF000000, 0xFF000000, 0xFF000000, 0xFF000000, 0xFF000000,
  0xFF000000, 0xFF000000, 0xFF000000, 0xFF000000, 0xFF000000, 0xFD04C4FC, 0xED04D7FC, 0xC904DAFC, 0xBD04DAFC, 0xE104DAFC, 0xF804DAFC, 0xFF000000, 0xFF000000, 0xFF000000, 0xFF000000, 0xFF000000,
  0xFF000000, 0xFF000000, 0xFF000000, 0xFF000000, 0xF004AEFC, 0x9004C4FC, 0x3204D7FC, 0x0004D1FC, 0x0004CEFC, 0x1104D6FC, 0x6904DAFC, 0xE904DAFC, 0xFF000000, 0xFF000000, 0xFF000000, 0xFF000000,
  0xFF000000, 0xFF000000, 0xFF000000, 0xEA04AEFC, 0x3D04B7FC, 0x0204D0FC, 0x0004BBFC, 0x0004AFFC, 0x0004B0FC, 0x0004B2FC, 0x0104C6FC, 0x3D04D9FC, 0xF404DAFC, 0xFF000000, 0xFF000000, 0xFF000000,
  0xFF000000, 0xFF000000, 0xFB04AEFC, 0x7604AEFC, 0x0004C4FC, 0x0004C6FC, 0x0004B1FC, 0x0004B1FC, 0x0004C3FC, 0x0004B6FC, 0x0004B0FC, 0x0004CAFC, 0x9404DAFC, 0xFD04DAFC, 0xFF000000, 0xFF000000,
  0xFF000000, 0xFF000000, 0xEA04AEFC, 0x2D04AEFC, 0x0004CCFC, 0x0004BCFC, 0x0004B8FC, 0x0004CFFC, 0x0004D8FC, 0x0004C6FC, 0x0004B1FC, 0x0004B6FC, 0x4004DAFC, 0xF004DAFC, 0xFF000000, 0xFF000000,
  0xFF000000, 0xFF000000, 0xE404AEFC, 0x1B04AEFC, 0x0004CDFC, 0x0004BBFC, 0x0004B0FC, 0x0004CEFC, 0x0004DAFC, 0x0004D9FC, 0x0004C7FC, 0x0004AFFC, 0x1B04D7FC, 0xE404DAFC, 0xFF000000, 0xFF000000,
  0xFF000000, 0xFF000000, 0xE504AEFC, 0x2004AEFC, 0x0004C5FC, 0x0004C3FC, 0x0004B1FC, 0x0004CDFC, 0x0004D3FC, 0x0004CEFC, 0x0004B0FC, 0x0004AEFC, 0x1B04D7FC, 0xE404DAFC, 0xFF000000, 0xFF000000,
  0xFF000000, 0xFF000000, 0xF304AEFC, 0x4D04AEFC, 0x0004B9FC, 0x0004CFFC, 0x0004B5FC, 0x0004B4FC, 0x0004B5FC, 0x0004C1FC, 0x0004AEFC, 0x0004B7FC, 0x3804DAFC, 0xED04DAFC, 0xFF000000, 0xFF000000,
  0xFF000000, 0xFF000000, 0xFF000000, 0xB404AEFC, 0x0204AEFC, 0x0004C8FC, 0x0004CBFC, 0x0004B2FC, 0x0004AFFC, 0x0004AFFC, 0x0004B2FC, 0x0104CDFC, 0x9204DAFC, 0xFD04DAFC, 0xFF000000, 0xFF000000,
  0xFF000000, 0xFF000000, 0xFF000000, 0xFF000000, 0x7404AEFC, 0x0604B1FC, 0x0004C4FC, 0x0004D4FC, 0x0004CAFC, 0x0004C9FC, 0x0D04D4FC, 0x6B04DAFC, 0xFF000000, 0xFF000000, 0xFF000000, 0xFF000000,
  0xFF000000, 0xFF000000, 0xFF000000, 0xFF000000, 0xF704AEFC, 0xA404AEFC, 0x4304AFFC, 0x2704B6FC, 0x2E04C2FC, 0x6504C9FC, 0xCA04D4FC, 0xF904DAFC, 0xFF000000, 0xFF000000, 0xFF000000, 0xFF000000,
  0xFF000000, 0xFF000000, 0xFF000000, 0xFF000000, 0xFF000000, 0xFF000000, 0xF004AEFC, 0xDC04AEFC, 0xE204AEFC, 0xFC04AEFC, 0xFF000000, 0xFF000000, 0xFF000000, 0xFF000000, 0xFF000000, 0xFF000000,
  0xFF000000, 0xFF000000, 0xFF000000, 0xFF000000, 0xFF000000, 0xFF000000, 0xFF000000, 0xFF000000, 0xFF000000, 0xFF000000, 0xFF000000, 0xFF000000, 0xFF000000, 0xFF000000, 0xFF000000, 0xFF000000
#endif
};

GUI_CONST_STORAGE GUI_BITMAP bmCoin6 = {
  16, // xSize
  15, // ySize
  64, // BytesPerLine
  32, // BitsPerPixel
  (unsigned char *)_accoin6,  // Pointer to picture data
  NULL,  // Pointer to palette
#if (GUI_USE_ARGB == 1)
  GUI_DRAW_BMPM8888I
#else
  GUI_DRAW_BMP8888
#endif
};
/*************************** End of file ****************************/
