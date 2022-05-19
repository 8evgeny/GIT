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
static GUI_CONST_STORAGE unsigned long _acscore[] = {
#if (GUI_USE_ARGB == 1)
  0x1AFFFFFF, 0x89FFFFFF, 0xCCFFFFFF, 0xDAFFFFFF, 0x9FFFFFFF, 0x24FFFFFF, 0x00000000, 0x00000000, 0x1FFFFFFF, 0x95FFFFFF, 0xE1FFFFFF, 0xF8FFFFFF, 0xD2FFFFFF, 0x44FFFFFF, 0x02FFFFFF, 0x00000000, 0x01FFFFFF, 0x3EFFFFFF, 0x9BFFFFFF, 0xCBFFFFFF, 0xD1FFFFFF, 
        0xA0FFFFFF, 0x34FFFFFF, 0x01FFFFFF, 0x13FFFFFF, 0xC3FFFFFF, 0xE6FFFFFF, 0xA2FFFFFF, 0xC4FFFFFF, 0xCCFFFFFF, 0x72FFFFFF, 0x08FFFFFF, 0x44FFFFFF, 0xBAFFFFFF, 0xC3FFFFFF, 0xC3FFFFFF, 0xC1FFFFFF, 0xB9FFFFFF, 0xA7FFFFFF,
  0xACFFFFFF, 0xFCFFFFFF, 0xF3FFFFFF, 0xEFFFFFFF, 0xFFFFFFFF, 0xB1FFFFFF, 0x11FFFFFF, 0x19FFFFFF, 0xC4FFFFFF, 0xFEFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xE1FFFFFF, 0x26FFFFFF, 0x02FFFFFF, 0x50FFFFFF, 0xECFFFFFF, 0xFFFFFFFF, 0xFDFFFFFF, 0xFEFFFFFF, 
        0xFFFFFFFF, 0xDDFFFFFF, 0x34FFFFFF, 0x1AFFFFFF, 0xE7FFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xF8FFFFFF, 0x5DFFFFFF, 0x74FFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xF3FFFFFF,
  0xF5FFFFFF, 0xFFFFFFFF, 0x8BFFFFFF, 0x58FFFFFF, 0xF4FFFFFF, 0xE2FFFFFF, 0x2BFFFFFF, 0x78FFFFFF, 0xFFFFFFFF, 0xFDFFFFFF, 0xA7FFFFFF, 0x7FFFFFFF, 0xF5FFFFFF, 0xFFFFFFFF, 0x6FFFFFFF, 0x17FFFFFF, 0xCDFFFFFF, 0xFFFFFFFF, 0xDCFFFFFF, 0x6CFFFFFF, 0x75FFFFFF, 
        0xEDFFFFFF, 0xFFFFFFFF, 0x9FFFFFFF, 0x1EFFFFFF, 0xEBFFFFFF, 0xFFFFFFFF, 0xDBFFFFFF, 0x8CFFFFFF, 0xCFFFFFFF, 0xFFFFFFFF, 0xA1FFFFFF, 0x74FFFFFF, 0xFFFFFFFF, 0xD3FFFFFF, 0x67FFFFFF, 0x62FFFFFF, 0x5BFFFFFF, 0x54FFFFFF,
  0xDDFFFFFF, 0xFFFFFFFF, 0xD6FFFFFF, 0x61FFFFFF, 0x2CFFFFFF, 0x1EFFFFFF, 0x15FFFFFF, 0xC3FFFFFF, 0xFFFFFFFF, 0xC1FFFFFF, 0x11FFFFFF, 0x0DFFFFFF, 0xBAFFFFFF, 0xFFFFFFFF, 0x92FFFFFF, 0x3EFFFFFF, 0xFDFFFFFF, 0xFFFFFFFF, 0x49FFFFFF, 0x00000000, 0x02FFFFFF, 
        0x81FFFFFF, 0xFEFFFFFF, 0xD4FFFFFF, 0x38FFFFFF, 0xEFFFFFFF, 0xFFFFFFFF, 0x4DFFFFFF, 0x01FFFFFF, 0x8FFFFFFF, 0xFFFFFFFF, 0x9DFFFFFF, 0x72FFFFFF, 0xFFFFFFFF, 0xB8FFFFFF, 0x05FFFFFF, 0x00000000, 0x00000000, 0x00000000,
  0x5CFFFFFF, 0xE0FFFFFF, 0xFEFFFFFF, 0xF7FFFFFF, 0xB2FFFFFF, 0x22FFFFFF, 0x23FFFFFF, 0xDEFFFFFF, 0xFFFFFFFF, 0x7EFFFFFF, 0x01FFFFFF, 0x01FFFFFF, 0x2BFFFFFF, 0x42FFFFFF, 0x13FFFFFF, 0x5DFFFFFF, 0xFFFFFFFF, 0xE8FFFFFF, 0x13FFFFFF, 0x00000000, 0x00000000, 
        0x51FFFFFF, 0xFBFFFFFF, 0xE3FFFFFF, 0x4BFFFFFF, 0xF3FFFFFF, 0xFFFFFFFF, 0x50FFFFFF, 0x47FFFFFF, 0xE1FFFFFF, 0xFDFFFFFF, 0x58FFFFFF, 0x70FFFFFF, 0xFFFFFFFF, 0xF1FFFFFF, 0xCEFFFFFF, 0xC6FFFFFF, 0xB8FFFFFF, 0x28FFFFFF,
  0x0AFFFFFF, 0x3AFFFFFF, 0x89FFFFFF, 0xE9FFFFFF, 0xFFFFFFFF, 0xAEFFFFFF, 0x38FFFFFF, 0xE6FFFFFF, 0xFFFFFFFF, 0x6CFFFFFF, 0x00000000, 0x00000000, 0x01FFFFFF, 0x0BFFFFFF, 0x01FFFFFF, 0x60FFFFFF, 0xFFFFFFFF, 0xE1FFFFFF, 0x11FFFFFF, 0x00000000, 0x00000000, 
        0x46FFFFFF, 0xFAFFFFFF, 0xE6FFFFFF, 0x4FFFFFFF, 0xF7FFFFFF, 0xFFFFFFFF, 0xC7FFFFFF, 0xE8FFFFFF, 0xFBFFFFFF, 0xAAFFFFFF, 0x0EFFFFFF, 0x6EFFFFFF, 0xFFFFFFFF, 0xFEFFFFFF, 0xFBFFFFFF, 0xFBFFFFFF, 0xECFFFFFF, 0x3DFFFFFF,
  0xA0FFFFFF, 0xACFFFFFF, 0x30FFFFFF, 0x4BFFFFFF, 0xFFFFFFFF, 0xF9FFFFFF, 0x5AFFFFFF, 0xD5FFFFFF, 0xFFFFFFFF, 0x9AFFFFFF, 0x06FFFFFF, 0x00000000, 0x3DFFFFFF, 0xB1FFFFFF, 0x57FFFFFF, 0x49FFFFFF, 0xFFFFFFFF, 0xFBFFFFFF, 0x31FFFFFF, 0x00000000, 0x01FFFFFF, 
        0x6EFFFFFF, 0xFDFFFFFF, 0xDBFFFFFF, 0x44FFFFFF, 0xFAFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xD0FFFFFF, 0x11FFFFFF, 0x00000000, 0x6DFFFFFF, 0xFFFFFFFF, 0xD6FFFFFF, 0x6CFFFFFF, 0x68FFFFFF, 0x60FFFFFF, 0x10FFFFFF,
  0xE3FFFFFF, 0xFFFFFFFF, 0xAFFFFFFF, 0x65FFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x57FFFFFF, 0x98FFFFFF, 0xFFFFFFFF, 0xF6FFFFFF, 0x95FFFFFF, 0x75FFFFFF, 0xDCFFFFFF, 0xFFFFFFFF, 0xB6FFFFFF, 0x28FFFFFF, 0xDBFFFFFF, 0xFFFFFFFF, 0xC6FFFFFF, 0x50FFFFFF, 0x5BFFFFFF, 
        0xE2FFFFFF, 0xFFFFFFFF, 0xB3FFFFFF, 0x29FFFFFF, 0xFEFFFFFF, 0xFFFFFFFF, 0xCBFFFFFF, 0xF8FFFFFF, 0xFAFFFFFF, 0xAEFFFFFF, 0x24FFFFFF, 0x6CFFFFFF, 0xFFFFFFFF, 0xD4FFFFFF, 0x5EFFFFFF, 0x5AFFFFFF, 0x5AFFFFFF, 0x4CFFFFFF,
  0x8DFFFFFF, 0xFBFFFFFF, 0xFEFFFFFF, 0xFBFFFFFF, 0xFFFFFFFF, 0xE8FFFFFF, 0x2DFFFFFF, 0x27FFFFFF, 0xD4FFFFFF, 0xFEFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xECFFFFFF, 0x4FFFFFFF, 0x04FFFFFF, 0x6BFFFFFF, 0xF5FFFFFF, 0xFFFFFFFF, 0xF3FFFFFF, 0xF6FFFFFF, 
        0xFFFFFFFF, 0xE7FFFFFF, 0x47FFFFFF, 0x26FFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x50FFFFFF, 0x78FFFFFF, 0xECFFFFFF, 0xFFFFFFFF, 0x8BFFFFFF, 0x6CFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xEDFFFFFF,
  0x14FFFFFF, 0x8EFFFFFF, 0xE3FFFFFF, 0xFCFFFFFF, 0xDAFFFFFF, 0x5DFFFFFF, 0x05FFFFFF, 0x00000000, 0x26FFFFFF, 0x96FFFFFF, 0xD0FFFFFF, 0xD4FFFFFF, 0xADFFFFFF, 0x42FFFFFF, 0x03FFFFFF, 0x00000000, 0x04FFFFFF, 0x5CFFFFFF, 0xBEFFFFFF, 0xE6FFFFFF, 0xE0FFFFFF, 
        0xAFFFFFFF, 0x3FFFFFFF, 0x03FFFFFF, 0x1DFFFFFF, 0xD3FFFFFF, 0xD2FFFFFF, 0x2CFFFFFF, 0x04FFFFFF, 0x47FFFFFF, 0xC2FFFFFF, 0x33FFFFFF, 0x52FFFFFF, 0xDDFFFFFF, 0xE1FFFFFF, 0xE1FFFFFF, 0xE1FFFFFF, 0xE1FFFFFF, 0xC5FFFFFF
#else
  0xE5FFFFFF, 0x76FFFFFF, 0x33FFFFFF, 0x25FFFFFF, 0x60FFFFFF, 0xDBFFFFFF, 0xFF000000, 0xFF000000, 0xE0FFFFFF, 0x6AFFFFFF, 0x1EFFFFFF, 0x07FFFFFF, 0x2DFFFFFF, 0xBBFFFFFF, 0xFDFFFFFF, 0xFF000000, 0xFEFFFFFF, 0xC1FFFFFF, 0x64FFFFFF, 0x34FFFFFF, 0x2EFFFFFF, 
        0x5FFFFFFF, 0xCBFFFFFF, 0xFEFFFFFF, 0xECFFFFFF, 0x3CFFFFFF, 0x19FFFFFF, 0x5DFFFFFF, 0x3BFFFFFF, 0x33FFFFFF, 0x8DFFFFFF, 0xF7FFFFFF, 0xBBFFFFFF, 0x45FFFFFF, 0x3CFFFFFF, 0x3CFFFFFF, 0x3EFFFFFF, 0x46FFFFFF, 0x58FFFFFF,
  0x53FFFFFF, 0x03FFFFFF, 0x0CFFFFFF, 0x10FFFFFF, 0x00FFFFFF, 0x4EFFFFFF, 0xEEFFFFFF, 0xE6FFFFFF, 0x3BFFFFFF, 0x01FFFFFF, 0x00FFFFFF, 0x00FFFFFF, 0x00FFFFFF, 0x1EFFFFFF, 0xD9FFFFFF, 0xFDFFFFFF, 0xAFFFFFFF, 0x13FFFFFF, 0x00FFFFFF, 0x02FFFFFF, 0x01FFFFFF, 
        0x00FFFFFF, 0x22FFFFFF, 0xCBFFFFFF, 0xE5FFFFFF, 0x18FFFFFF, 0x00FFFFFF, 0x00FFFFFF, 0x00FFFFFF, 0x00FFFFFF, 0x07FFFFFF, 0xA2FFFFFF, 0x8BFFFFFF, 0x00FFFFFF, 0x00FFFFFF, 0x00FFFFFF, 0x00FFFFFF, 0x00FFFFFF, 0x0CFFFFFF,
  0x0AFFFFFF, 0x00FFFFFF, 0x74FFFFFF, 0xA7FFFFFF, 0x0BFFFFFF, 0x1DFFFFFF, 0xD4FFFFFF, 0x87FFFFFF, 0x00FFFFFF, 0x02FFFFFF, 0x58FFFFFF, 0x80FFFFFF, 0x0AFFFFFF, 0x00FFFFFF, 0x90FFFFFF, 0xE8FFFFFF, 0x32FFFFFF, 0x00FFFFFF, 0x23FFFFFF, 0x93FFFFFF, 0x8AFFFFFF, 
        0x12FFFFFF, 0x00FFFFFF, 0x60FFFFFF, 0xE1FFFFFF, 0x14FFFFFF, 0x00FFFFFF, 0x24FFFFFF, 0x73FFFFFF, 0x30FFFFFF, 0x00FFFFFF, 0x5EFFFFFF, 0x8BFFFFFF, 0x00FFFFFF, 0x2CFFFFFF, 0x98FFFFFF, 0x9DFFFFFF, 0xA4FFFFFF, 0xABFFFFFF,
  0x22FFFFFF, 0x00FFFFFF, 0x29FFFFFF, 0x9EFFFFFF, 0xD3FFFFFF, 0xE1FFFFFF, 0xEAFFFFFF, 0x3CFFFFFF, 0x00FFFFFF, 0x3EFFFFFF, 0xEEFFFFFF, 0xF2FFFFFF, 0x45FFFFFF, 0x00FFFFFF, 0x6DFFFFFF, 0xC1FFFFFF, 0x02FFFFFF, 0x00FFFFFF, 0xB6FFFFFF, 0xFF000000, 0xFDFFFFFF, 
        0x7EFFFFFF, 0x01FFFFFF, 0x2BFFFFFF, 0xC7FFFFFF, 0x10FFFFFF, 0x00FFFFFF, 0xB2FFFFFF, 0xFEFFFFFF, 0x70FFFFFF, 0x00FFFFFF, 0x62FFFFFF, 0x8DFFFFFF, 0x00FFFFFF, 0x47FFFFFF, 0xFAFFFFFF, 0xFF000000, 0xFF000000, 0xFF000000,
  0xA3FFFFFF, 0x1FFFFFFF, 0x01FFFFFF, 0x08FFFFFF, 0x4DFFFFFF, 0xDDFFFFFF, 0xDCFFFFFF, 0x21FFFFFF, 0x00FFFFFF, 0x81FFFFFF, 0xFEFFFFFF, 0xFEFFFFFF, 0xD4FFFFFF, 0xBDFFFFFF, 0xECFFFFFF, 0xA2FFFFFF, 0x00FFFFFF, 0x17FFFFFF, 0xECFFFFFF, 0xFF000000, 0xFF000000, 
        0xAEFFFFFF, 0x04FFFFFF, 0x1CFFFFFF, 0xB4FFFFFF, 0x0CFFFFFF, 0x00FFFFFF, 0xAFFFFFFF, 0xB8FFFFFF, 0x1EFFFFFF, 0x02FFFFFF, 0xA7FFFFFF, 0x8FFFFFFF, 0x00FFFFFF, 0x0EFFFFFF, 0x31FFFFFF, 0x39FFFFFF, 0x47FFFFFF, 0xD7FFFFFF,
  0xF5FFFFFF, 0xC5FFFFFF, 0x76FFFFFF, 0x16FFFFFF, 0x00FFFFFF, 0x51FFFFFF, 0xC7FFFFFF, 0x19FFFFFF, 0x00FFFFFF, 0x93FFFFFF, 0xFF000000, 0xFF000000, 0xFEFFFFFF, 0xF4FFFFFF, 0xFEFFFFFF, 0x9FFFFFFF, 0x00FFFFFF, 0x1EFFFFFF, 0xEEFFFFFF, 0xFF000000, 0xFF000000, 
        0xB9FFFFFF, 0x05FFFFFF, 0x19FFFFFF, 0xB0FFFFFF, 0x08FFFFFF, 0x00FFFFFF, 0x38FFFFFF, 0x17FFFFFF, 0x04FFFFFF, 0x55FFFFFF, 0xF1FFFFFF, 0x91FFFFFF, 0x00FFFFFF, 0x01FFFFFF, 0x04FFFFFF, 0x04FFFFFF, 0x13FFFFFF, 0xC2FFFFFF,
  0x5FFFFFFF, 0x53FFFFFF, 0xCFFFFFFF, 0xB4FFFFFF, 0x00FFFFFF, 0x06FFFFFF, 0xA5FFFFFF, 0x2AFFFFFF, 0x00FFFFFF, 0x65FFFFFF, 0xF9FFFFFF, 0xFF000000, 0xC2FFFFFF, 0x4EFFFFFF, 0xA8FFFFFF, 0xB6FFFFFF, 0x00FFFFFF, 0x04FFFFFF, 0xCEFFFFFF, 0xFF000000, 0xFEFFFFFF, 
        0x91FFFFFF, 0x02FFFFFF, 0x24FFFFFF, 0xBBFFFFFF, 0x05FFFFFF, 0x00FFFFFF, 0x00FFFFFF, 0x00FFFFFF, 0x2FFFFFFF, 0xEEFFFFFF, 0xFF000000, 0x92FFFFFF, 0x00FFFFFF, 0x29FFFFFF, 0x93FFFFFF, 0x97FFFFFF, 0x9FFFFFFF, 0xEFFFFFFF,
  0x1CFFFFFF, 0x00FFFFFF, 0x50FFFFFF, 0x9AFFFFFF, 0x00FFFFFF, 0x00FFFFFF, 0xA8FFFFFF, 0x67FFFFFF, 0x00FFFFFF, 0x09FFFFFF, 0x6AFFFFFF, 0x8AFFFFFF, 0x23FFFFFF, 0x00FFFFFF, 0x49FFFFFF, 0xD7FFFFFF, 0x24FFFFFF, 0x00FFFFFF, 0x39FFFFFF, 0xAFFFFFFF, 0xA4FFFFFF, 
        0x1DFFFFFF, 0x00FFFFFF, 0x4CFFFFFF, 0xD6FFFFFF, 0x01FFFFFF, 0x00FFFFFF, 0x34FFFFFF, 0x07FFFFFF, 0x05FFFFFF, 0x51FFFFFF, 0xDBFFFFFF, 0x93FFFFFF, 0x00FFFFFF, 0x2BFFFFFF, 0xA1FFFFFF, 0xA5FFFFFF, 0xA5FFFFFF, 0xB3FFFFFF,
  0x72FFFFFF, 0x04FFFFFF, 0x01FFFFFF, 0x04FFFFFF, 0x00FFFFFF, 0x17FFFFFF, 0xD2FFFFFF, 0xD8FFFFFF, 0x2BFFFFFF, 0x01FFFFFF, 0x00FFFFFF, 0x00FFFFFF, 0x00FFFFFF, 0x13FFFFFF, 0xB0FFFFFF, 0xFBFFFFFF, 0x94FFFFFF, 0x0AFFFFFF, 0x00FFFFFF, 0x0CFFFFFF, 0x09FFFFFF, 
        0x00FFFFFF, 0x18FFFFFF, 0xB8FFFFFF, 0xD9FFFFFF, 0x00FFFFFF, 0x00FFFFFF, 0xAFFFFFFF, 0x87FFFFFF, 0x13FFFFFF, 0x00FFFFFF, 0x74FFFFFF, 0x93FFFFFF, 0x00FFFFFF, 0x00FFFFFF, 0x00FFFFFF, 0x00FFFFFF, 0x00FFFFFF, 0x12FFFFFF,
  0xEBFFFFFF, 0x71FFFFFF, 0x1CFFFFFF, 0x03FFFFFF, 0x25FFFFFF, 0xA2FFFFFF, 0xFAFFFFFF, 0xFF000000, 0xD9FFFFFF, 0x69FFFFFF, 0x2FFFFFFF, 0x2BFFFFFF, 0x52FFFFFF, 0xBDFFFFFF, 0xFCFFFFFF, 0xFF000000, 0xFBFFFFFF, 0xA3FFFFFF, 0x41FFFFFF, 0x19FFFFFF, 0x1FFFFFFF, 
        0x50FFFFFF, 0xC0FFFFFF, 0xFCFFFFFF, 0xE2FFFFFF, 0x2CFFFFFF, 0x2DFFFFFF, 0xD3FFFFFF, 0xFBFFFFFF, 0xB8FFFFFF, 0x3DFFFFFF, 0xCCFFFFFF, 0xADFFFFFF, 0x22FFFFFF, 0x1EFFFFFF, 0x1EFFFFFF, 0x1EFFFFFF, 0x1EFFFFFF, 0x3AFFFFFF
#endif
};

GUI_CONST_STORAGE GUI_BITMAP bmscore = {
  39, // xSize
  10, // ySize
  156, // BytesPerLine
  32, // BitsPerPixel
  (unsigned char *)_acscore,  // Pointer to picture data
  NULL,  // Pointer to palette
#if (GUI_USE_ARGB == 1)
  GUI_DRAW_BMPM8888I
#else
  GUI_DRAW_BMP8888
#endif
};

/*************************** End of file ****************************/
