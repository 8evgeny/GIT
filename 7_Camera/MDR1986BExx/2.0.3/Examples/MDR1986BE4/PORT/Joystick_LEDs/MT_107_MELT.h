/**
  ******************************************************************************
  * @file    MT_107_MELT.h
  * @author  Milandr Application Team
  * @version V2.0.2
  * @date    22/09/2021
  * @brief   This file contains the functions prototype for MT_107_MELT display.
  ******************************************************************************
  ******************************************************************************
  * <br><br>
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, MILANDR SHALL NOT BE HELD LIABLE FOR ANY DIRECT, INDIRECT
  * OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2021 Milandr </center></h2>
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef MT_107_MELT_H
#define MT_107_MELT_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "MDR32F9Qx_config.h"

// DB0(L)-g         DB0(H)- h
// DB1(L)-e         DB1(H)- b
// DB2(L)-d         DB2(H)- c
// DB3(l)-a         DB3(H)- f
//*****************************************
//         a
//      * *** *
//    f *  g  * b
//      * *** *
//    e *     * c
//      * *** *
//        d      *h

void Display_Init(void);
void Display_Clear(void);
void Display_PutChar(uint8_t char_position, uint8_t put_char);
void Display_Lock(void );
void Display_UnLock(void);
void Delay(void );
void Display_PutDigit(uint8_t char_position, uint8_t digit);

#ifdef __cplusplus
} // extern "C" block end
#endif

#endif /* MT_107_MELT_H */

/******************* (C) COPYRIGHT 2021 Milandr ********************************
*
* END OF FILE MT_107_MELT.h */
