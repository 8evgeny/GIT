/*********************************************************************
*          Portions COPYRIGHT 2017 STMicroelectronics                *
*          Portions SEGGER Microcontroller GmbH & Co. KG             *
*        Solutions for real time microcontroller applications        *
**********************************************************************
*                                                                    *
*        (c) 1996 - 2017  SEGGER Microcontroller GmbH & Co. KG       *
*                                                                    *
*        Internet: www.segger.com    Support:  support@segger.com    *
*                                                                    *
**********************************************************************

** emWin V5.40 - Graphical user interface for embedded applications **
All  Intellectual Property rights  in the Software belongs to  SEGGER.
emWin is protected by  international copyright laws.  Knowledge of the
source code may not be used to write a similar product.  This file may
only be used in accordance with the following terms:

The  software has  been licensed  to STMicroelectronics International
N.V. a Dutch company with a Swiss branch and its headquarters in Plan-
les-Ouates, Geneva, 39 Chemin du Champ des Filles, Switzerland for the
purposes of creating libraries for ARM Cortex-M-based 32-bit microcon_
troller products commercialized by Licensee only, sublicensed and dis_
tributed under the terms and conditions of the End User License Agreement
supplied by STMicroelectronics International N.V.
Full source code is available at: www.segger.com

We appreciate your understanding and fairness.
----------------------------------------------------------------------
File        : GUIConf.c
Purpose     : Display controller initialization
---------------------------END-OF-HEADER------------------------------
*/

#include "GUI.h"

/*********************************************************************
*
*       Defines
*
**********************************************************************
*/

/* Define the available number of bytes available for the GUI */

//#define GUI_NUMBYTES  (210) *  1024
#define GUI_NUMBYTES  (2 * 1024 * 1024)

/* Define the memory table to use by STemWin */
#if defined ( __ICCARM__ )
#pragma data_alignment=32
#pragma location="STemWinMemPool"
#elif defined(__CC_ARM)
__attribute__((section(".STemWinMemPool"), zero_init)) __attribute__ ((aligned (32)))
#elif defined(__GNUC__)
__attribute__((section(".STemWinMemPool"))) __attribute__ ((aligned (32)))
#endif
U32 aMemory[GUI_NUMBYTES / 4];


/*********************************************************************
*
*       Public code
*
**********************************************************************
*/
/*********************************************************************
*
*       GUI_X_Config
*
* Purpose:
*   Called during the initialization process in order to set up the
*   available memory for the GUI.
*/
void GUI_X_Config(void) 
{
  GUI_ALLOC_AssignMemory(aMemory, GUI_NUMBYTES);
}

/*************************** End of file ****************************/
