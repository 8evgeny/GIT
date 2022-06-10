/**
  ******************************************************************************
  * @file    MDR32F9Qx_led.h
  * @author  Milandr Application Team
  * @version V2.0.0
  * @date    06/05/2021
  * @brief   This file contains all the functions prototypes for the LED Display
  *          firmware library.
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
  * <h2><center>&copy; COPYRIGHT 2021 Milandr</center></h2>
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef MDR32F9QX_LED_H_
#define MDR32F9QX_LED_H_

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "MDR32F9Qx_config.h"

/** @addtogroup __MDR32Fx_StdPeriph_Driver MDR32Fx Standard Peripherial Driver
  * @{
  */

#if defined (USE_MDR1986VE3)
/** @addtogroup LED
  * @warning This module can be used only for MDR1986VE3 microcontroller's.
  * @{
  */

/** @defgroup LED_Exported_Types LED Exported Types
  * @{
  */

/**
  * @brief LED Frame Number
  */
typedef enum
{
    LED_FRAME0 = ((uint32_t)0x00),
    LED_FRAME1 = ((uint32_t)0x01)
} LED_Frame_Number;

#define IS_LED_FRAME(LED_FRMAME)        ((LED_FRMAME == LED_FRAME0) ||\
                                         (LED_FRMAME == LED_FRAME1))

/**
  * @brief LED Row Length
  */
typedef enum
{
    LED_ROW1 = ((uint32_t)0x00),
    LED_ROW2 = ((uint32_t)0x10),
    LED_ROW3 = ((uint32_t)0x20),
    LED_ROW4 = ((uint32_t)0x30),
    LED_ROW5 = ((uint32_t)0x40),
    LED_ROW6 = ((uint32_t)0x50),
    LED_ROW7 = ((uint32_t)0x60),
    LED_ROW8 = ((uint32_t)0x70)
} LED_Row_Length;

#define IS_LED_ROW_LEN(ROW_LEN)         ((ROW_LEN & (~LED_CR_LEN_STR_Msk)) == 0)

/**
  * @brief LED Column Length
  */
typedef enum
{
    LED_COLUMN1 =  ((uint32_t)0x0000),
    LED_COLUMN2 =  ((uint32_t)0x0100),
    LED_COLUMN3 =  ((uint32_t)0x0200),
    LED_COLUMN4 =  ((uint32_t)0x0300),
    LED_COLUMN5 =  ((uint32_t)0x0400),
    LED_COLUMN6 =  ((uint32_t)0x0500),
    LED_COLUMN7 =  ((uint32_t)0x0600),
    LED_COLUMN8 =  ((uint32_t)0x0700)
} LED_Column_Length;

#define IS_LED_COLUMN_LEN(COLUMN_LEN)   ((COLUMN_LEN & ~ LED_CR_LEN_STOLB_Msk) == 0)

/**
  * @brief LED Clock BRG
  */
typedef enum
{
    LED_BRG4 = ((uint32_t)0x03),
    LED_BRG5 = ((uint32_t)0x04),
    LED_BRG6 = ((uint32_t)0x05),
    LED_BRG7 = ((uint32_t)0x06),
    LED_BRG8 = ((uint32_t)0x07)
} LED_BRG;

#define IS_LED_BRG(BRG)                 ((BRG >= LED_BRG4) && (BRG <= LED_BRG8))

/**
  * @brief LED Output Inversion
  */
typedef enum
{
    LED_INVERSION_Y  = ((uint32_t)0x1000), /*!< Invert the Y outputs. */
    LED_INVERSION_EN = ((uint32_t)0x2000), /*!< Output EN inversion is used to compensate
                                                for inversion of the matching circuit. */
    LED_INVERSION_C  = ((uint32_t)0x4000), /*!< Output C inversion is used to compensate
                                                for inversion of the matching circuit. */
    LED_INVERSION_DO = ((uint32_t)0x8000)  /*!< Output DO inversion is used to compensate
                                                for inversion of the matching circuit. */
} LED_Inversion_Output;

#define IS_LED_INVERSION_OUTPUT(OUTPUT) ((OUTPUT == LED_INVERSION_Y)  || \
                                         (OUTPUT == LED_INVERSION_EN) || \
                                         (OUTPUT == LED_INVERSION_C)  || \
                                         (OUTPUT == LED_INVERSION_DO))

/**
 * @brief LED Init Structure definition
 */
typedef struct
{
    FunctionalState   TwinkleState;      /*!< Enable or disable flickering.
                                              This parameter can be a value of @ref FunctionalState. */
    uint32_t          TwinkleDIV;        /*!< Division ratio of the frequency of blinking display controller.
                                              This parameter can be a value from 0 to 0xFF. */
    LED_Frame_Number  FrameNumber;       /*!< Number of used frame if the flicker off FlickeringState=DISABLE.
                                              This parameter can be a value of @ref LED_Frame_Number. */
    LED_Row_Length    RowLen;            /*!< String length indicators IPV72A-4/5h7L - 1.
                                              Specifies the length of consistent data on circuits DO.
                                              This parameter can be a value of @ref LED_Row_Length. */
    LED_Column_Length ColumnLen;         /*!< Number of lines DO [7:0] - 1, connected to the indicators.
                                              This parameter can be a value of @ref LED_Column_Length. */
    FunctionalState   RAMState;          /*!< Enable or disable access to TextBuf and DecodeBbuf, when LED off.
                                              This parameter can be: ENABLE or DISABLE. */
    FunctionalState   EN_InversionState; /*!< Enable or disable inversion on output EN.
                                              This parameter can be a value of @ref FunctionalState. */
    FunctionalState   Y_InversionState;  /*!< Enable or disable inversion on output Y.
                                              This parameter can be a value of @ref FunctionalState. */
    FunctionalState   C_InversionState;  /*!< Enable or disable inversion on output C.
                                              This parameter can be a value of @ref FunctionalState. */
    FunctionalState   DO_InversionState; /*!< Enable or disable inversion on output DO.
                                              This parameter can be a value of @ref FunctionalState. */
} LED_InitTypeDef;

/** @} */ /* End of group LED_Exported_Types */


/** @defgroup LED_Exported_Functions LED Exported Functions
  * @{
  */
void LED_DeInit(void);
void LED_BRGInit(LED_BRG LED_BRG_Value);
void LED_StructInit(LED_InitTypeDef* LED_InitStruct);
void LED_Init(LED_InitTypeDef* LED_InitStruct);
void LED_CMD(FunctionalState NewState);
void LED_InversionOutputCMD(LED_Inversion_Output output, FunctionalState NewState);
void LED_ITCMD(FunctionalState NewState);
void LED_TwinkleCMD(FunctionalState NewState);
FlagStatus LED_GetReadyFlag(void);
void LED_InitDecodeBuf(void);
void LED_ClearFrame(LED_Frame_Number led_frame);
void LED_PutString(uint32_t position, const char * ptr_char);

/** @} */ /* End of group LED_Exported_Functions */

/** @} */ /* End of group LED */
#endif /* #if defined (USE_MDR1986VE3) */

/** @} */ /* End of group __MDR32Fx_StdPeriph_Driver */

#ifdef __cplusplus
} // extern "C" block end
#endif

#endif /* MDR32F9QX_LED_H_ */

/*********************** (C) COPYRIGHT 2021 Milandr ****************************
*
* END OF FILE MDR32F9Qx_led.h */

