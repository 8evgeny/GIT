/**
  ******************************************************************************
  * @file    MDR32F9Qx_keypad.h
  * @author  Milandr Application Team
  * @version V2.0.0
  * @date    01/04/2021
  * @brief   This file contains all the functions prototypes for the KEYPAD
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
#ifndef MDR32F9QX_KEYPAD_H_
#define MDR32F9QX_KEYPAD_H_

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "MDR32F9Qx_config.h"

/** @addtogroup __MDR32Fx_StdPeriph_Driver MDR32Fx Standard Peripherial Driver
  * @{
  */

#if defined (USE_MDR1986VE3)
/** @addtogroup KEYPAD
  * @warning This module can be used only for MDR1986VE3 microcontrollers's.
  * @{
  */

/** @defgroup KEYPAD_Exported_Types KEYPAD Exported Types
  * @{
  */

/**
  * @brief The HCLK clock prescaler configuration bits.
  */
typedef enum
{
    KEYPAD_BRG_4    = ((uint32_t)0x00),
    KEYPAD_BRG_8    = ((uint32_t)0x01),
    KEYPAD_BRG_16   = ((uint32_t)0x02),
    KEYPAD_BRG_64   = ((uint32_t)0x03),
    KEYPAD_BRG_128  = ((uint32_t)0x04),
    KEYPAD_BRG_256  = ((uint32_t)0x05),
    KEYPAD_BRG_512  = ((uint32_t)0x06),
    KEYPAD_BRG_1024 = ((uint32_t)0x07)
} KEYPAD_BRG;

#define IS_KEYPAD_BRG(BRG)  (((BRG << KEYPAD_CR_KEY_PRE_Pos) & \
                             (~KEYPAD_CR_KEY_PRE_Msk)) == 0)

/**
  * @brief A parameter that sets the number of keys in a row of the matrix keyboard.
  *        Determines the number of controller read pins used.
  */
typedef enum
{
    KEYPAD_KEY_ROW_1 = ((uint32_t)0x00),
    KEYPAD_KEY_ROW_2 = ((uint32_t)0x01),
    KEYPAD_KEY_ROW_3 = ((uint32_t)0x02),
    KEYPAD_KEY_ROW_4 = ((uint32_t)0x03),
    KEYPAD_KEY_ROW_5 = ((uint32_t)0x04),
    KEYPAD_KEY_ROW_6 = ((uint32_t)0x05),
    KEYPAD_KEY_ROW_7 = ((uint32_t)0x06),
    KEYPAD_KEY_ROW_8 = ((uint32_t)0x07)
} KEYPAD_KEY_ROW;

#define IS_KEYPAD_KEY_ROW(KEY_ROW)  (((KEY_ROW << KEYPAD_CR_KEY_ROW_Pos) & \
                                     (~KEYPAD_CR_KEY_ROW_Msk)) == 0)

/**
  * @brief A parameter that sets the number of columns of matrix keyboard keys.
  *        Determines the number of controller scan pins involved.
  */
typedef enum
{
    KEYPAD_KEY_COLUMN_1  = ((uint32_t)0x00),
    KEYPAD_KEY_COLUMN_2  = ((uint32_t)0x01),
    KEYPAD_KEY_COLUMN_3  = ((uint32_t)0x02),
    KEYPAD_KEY_COLUMN_4  = ((uint32_t)0x03),
    KEYPAD_KEY_COLUMN_5  = ((uint32_t)0x04),
    KEYPAD_KEY_COLUMN_6  = ((uint32_t)0x05),
    KEYPAD_KEY_COLUMN_7  = ((uint32_t)0x06),
    KEYPAD_KEY_COLUMN_8  = ((uint32_t)0x07),
    KEYPAD_KEY_COLUMN_9  = ((uint32_t)0x08),
    KEYPAD_KEY_COLUMN_10 = ((uint32_t)0x09),
    KEYPAD_KEY_COLUMN_11 = ((uint32_t)0x0A),
    KEYPAD_KEY_COLUMN_12 = ((uint32_t)0x0B),
    KEYPAD_KEY_COLUMN_13 = ((uint32_t)0x0C),
    KEYPAD_KEY_COLUMN_14 = ((uint32_t)0x0D),
    KEYPAD_KEY_COLUMN_15 = ((uint32_t)0x0E),
    KEYPAD_KEY_COLUMN_16 = ((uint32_t)0x0F)
} KEYPAD_KEY_COLUMN;

#define IS_KEYPAD_KEY_COLUMN(KEY_COLUMN)    (((KEY_COLUMN << KEYPAD_CR_KEY_COL_Pos) & \
                                             (~KEYPAD_CR_KEY_COL_Msk)) == 0)

/**
  * @brief Contact bounce protection timer configuration bits. Determines the number
  *        of keyboard controller ticks during which scans and interrupts will be blocked.
  */
typedef enum
{
    KEYPAD_CONTACT_CHATTERING_TIMER_8_CYCLES    = ((uint32_t)0x00),
    KEYPAD_CONTACT_CHATTERING_TIMER_16_CYCLES   = ((uint32_t)0x01),
    KEYPAD_CONTACT_CHATTERING_TIMER_32_CYCLES   = ((uint32_t)0x02),
    KEYPAD_CONTACT_CHATTERING_TIMER_64_CYCLES   = ((uint32_t)0x03),
    KEYPAD_CONTACT_CHATTERING_TIMER_256_CYCLES  = ((uint32_t)0x04),
    KEYPAD_CONTACT_CHATTERING_TIMER_512_CYCLES  = ((uint32_t)0x05),
    KEYPAD_CONTACT_CHATTERING_TIMER_1024_CYCLES = ((uint32_t)0x06),
    KEYPAD_CONTACT_CHATTERING_TIMER_2048_CYCLES = ((uint32_t)0x07)
} KEYPAD_CONTACT_CHATTERING_TIMER;

#define IS_KEYPAD_CONTACT_CHATTERING_TIMER_CYCLES(CYCLES) (((CYCLES << KEYPAD_CR_KEY_LTIM_Pos) & \
                                                           (~KEYPAD_CR_KEY_LTIM_Msk)) == 0)

/**
  * @brief Column number, starting from which, when scanning,
  *        the scanning mode of the switch field is turned on.
  */
typedef enum
{
    KEYPAD_FIRST_COLUMN_NUMBER_SWITCHES_1  = ((uint32_t)0x00),
    KEYPAD_FIRST_COLUMN_NUMBER_SWITCHES_2  = ((uint32_t)0x01),
    KEYPAD_FIRST_COLUMN_NUMBER_SWITCHES_3  = ((uint32_t)0x02),
    KEYPAD_FIRST_COLUMN_NUMBER_SWITCHES_4  = ((uint32_t)0x03),
    KEYPAD_FIRST_COLUMN_NUMBER_SWITCHES_5  = ((uint32_t)0x04),
    KEYPAD_FIRST_COLUMN_NUMBER_SWITCHES_6  = ((uint32_t)0x05),
    KEYPAD_FIRST_COLUMN_NUMBER_SWITCHES_7  = ((uint32_t)0x06),
    KEYPAD_FIRST_COLUMN_NUMBER_SWITCHES_8  = ((uint32_t)0x07),
    KEYPAD_FIRST_COLUMN_NUMBER_SWITCHES_9  = ((uint32_t)0x08),
    KEYPAD_FIRST_COLUMN_NUMBER_SWITCHES_10 = ((uint32_t)0x09),
    KEYPAD_FIRST_COLUMN_NUMBER_SWITCHES_11 = ((uint32_t)0x0A),
    KEYPAD_FIRST_COLUMN_NUMBER_SWITCHES_12 = ((uint32_t)0x0B),
    KEYPAD_FIRST_COLUMN_NUMBER_SWITCHES_13 = ((uint32_t)0x0C),
    KEYPAD_FIRST_COLUMN_NUMBER_SWITCHES_14 = ((uint32_t)0x0D),
    KEYPAD_FIRST_COLUMN_NUMBER_SWITCHES_15 = ((uint32_t)0x0E),
    KEYPAD_FIRST_COLUMN_NUMBER_SWITCHES_16 = ((uint32_t)0x0F)
} KEYPAD_FIRST_COLUMN_NUMBER_SWITCHES;

#define IS_KEYPAD_FIRST_COLUMN_NUMBER_SWITCHES(SWITCHES) (((SWITCHES << KEYPAD_CR_KEY_SWCOL_Pos) & \
                                                          (~KEYPAD_CR_KEY_SWCOL_Msk)) == 0)

/**
  * @brief Delay timer configuration bits between scans. Determines the number
  *        of keyboard controller ticks during which the delay occurs.
  */
typedef enum
{
    KEYPAD_DELAY_BETWEEN_SCANS_8_CYCLES    = ((uint32_t)0x00),
    KEYPAD_DELAY_BETWEEN_SCANS_16_CYCLES   = ((uint32_t)0x01),
    KEYPAD_DELAY_BETWEEN_SCANS_32_CYCLES   = ((uint32_t)0x02),
    KEYPAD_DELAY_BETWEEN_SCANS_64_CYCLES   = ((uint32_t)0x03),
    KEYPAD_DELAY_BETWEEN_SCANS_256_CYCLES  = ((uint32_t)0x04),
    KEYPAD_DELAY_BETWEEN_SCANS_512_CYCLES  = ((uint32_t)0x05),
    KEYPAD_DELAY_BETWEEN_SCANS_1024_CYCLES = ((uint32_t)0x06),
    KEYPAD_DELAY_BETWEEN_SCANS_2048_CYCLES = ((uint32_t)0x07)
} KEYPAD_DELAY_BETWEEN_SCANS;

#define IS_KEYPAD_DELAY_BETWEEN_SCANS(CYCLES)   (((CYCLES << KEYPAD_CR_KEY_IVCONF_Pos) & \
                                                 (~KEYPAD_CR_KEY_IVCONF_Msk)) == 0)

/**
  * @brief Keypad state flags.
  */
typedef enum
{
    KEYPAD_FLAG_INTF   = ((uint32_t)0x01), /*!< Interrupt flag. */
    KEYPAD_FLAG_PT1F   = ((uint32_t)0x02), /*!< First key pressed flag. */
    KEYPAD_FLAG_PT2F   = ((uint32_t)0x04), /*!< Second key pressed flag. */
    KEYPAD_FLAG_ERRF   = ((uint32_t)0x08), /*!< Input error flag. */
    KEYPAD_FLAG_SCF    = ((uint32_t)0x10), /*!< Scanning end flag. */
    KEYPAD_FLAG_PCHATF = ((uint32_t)0x20)  /*!< Flag of the end of protection against bouncing contacts. */
} KEYPAD_Flags;

#define IS_KEYPAD_FLAG(FLAG)    ((FLAG == KEYPAD_FLAG_INTF) || \
                                 (FLAG == KEYPAD_FLAG_PT1F) || \
                                 (FLAG == KEYPAD_FLAG_PT2F) || \
                                 (FLAG == KEYPAD_FLAG_ERRF) || \
                                 (FLAG == KEYPAD_FLAG_SCF)  || \
                                 (FLAG == KEYPAD_FLAG_PCHATF))

/**
  * @brief KEYPAD Init Structure definition
  */
typedef struct
{
    KEYPAD_KEY_ROW                      KeyRow;                    /*!< Number of keys in a row matrix keyboard.
                                                                        Specifies the number of pins of the controller involved readers.
                                                                        This parameter can be a value of @ref KEYPAD_KEY_ROW. */
    KEYPAD_KEY_COLUMN                   KeyColumn;                 /*!< Number of columns of the matrix keyboard shortcuts.
                                                                        Specifies the number of involved scanning terminals of the controller.
                                                                        This parameter can be a value of @ref KEY_COLUMN. */
    FunctionalState                     InterruptState;            /*!< Enable or disable interrupt for any change in the registers KEYPAD_SWR1-KEYPAD_SWR14.
                                                                        This parameter can be a value of @ref FunctionalState. */
    FunctionalState                     ContactChatteringState;    /*!< Enable or disable contact chattering.
                                                                        This parameter can be a value of @ref FunctionalState. */
    KEYPAD_CONTACT_CHATTERING_TIMER     ContactChatteringTimer;    /*!< Config timer protection chattering. Specifies the number
                                                                        of clock cycles of the keyboard controller, during which
                                                                        will be blocked scanning and interrupt.
                                                                        This parameter can be a value of @ref KEYPAD_CONTACT_CHATTERING_TIMER. */
    FunctionalState                     SwitchesFieldScanState;    /*!< Enable or disable scan field switches.
                                                                        Scan Mode field switches.
                                                                        This parameter can be a value of @ref FunctionalState. */
    KEYPAD_FIRST_COLUMN_NUMBER_SWITCHES FirstColumnNumberSwitches; /*!< Specifies the column number from which the scan mode is
                                                                        activated scanning field switches.
                                                                        This parameter can be a value of @ref KEYPAD_FIRST_COLUMN_NUMBER_SWITCHES. */
    FunctionalState                     DelayBetweenScansState;    /*!< Enable or disable delay between the scans.
                                                                        This parameter can be a value of @ref FunctionalState. */
    KEYPAD_DELAY_BETWEEN_SCANS          DelayBetweenScansConfig;   /*!< Specifies the number of clock cycles of the keyboard controller,
                                                                        for which there is a delay between scans.
                                                                        This parameter can be a value of the @ref KEYPAD_DELAY_BETWEEN_SCANS. */
} KEYPAD_InitTypeDef;

/** @} */ /* End of group KEYPAD_Exported_Types */


/** @defgroup KEYPAD_Exported_Functions KEYPAD Exported Functions
  * @{
  */
void KEYPAD_DeInit(void);
void KEYPAD_BRGInit(KEYPAD_BRG KEYPAD_BRG_Value);
void KEYPAD_StructInit(KEYPAD_InitTypeDef* KEYPAD_InitStruct);
void KEYPAD_Init(KEYPAD_InitTypeDef* KEYPAD_InitStruct);
void KEYPAD_Cmd(FunctionalState NewState);
FlagStatus KEYPAD_GetFlagStatus(KEYPAD_Flags KEYPAD_FLAG);
void KEYPAD_ClearFlag(KEYPAD_Flags KEYPAD_FLAG);
uint32_t KEYPAD_GetPressedFirstKey(void);
uint32_t KEYPAD_GetPressedSecondKey(void);
BitStatus KEYPAD_GetSwitchStatus(uint32_t switch_number);
void KEYPAD_ITCmd(FunctionalState NewState);

/** @} */ /* End of group KEYPAD_Exported_Functions */

#endif /* #if defined (USE_MDR1986VE3) */

/** @} */ /* End of group KEYPAD */

/** @} */ /* End of group __MDR32Fx_StdPeriph_Driver */

#ifdef __cplusplus
} // extern "C" block end
#endif

#endif /* MDR32F9QX_KEYPAD_H_ */

/*********************** (C) COPYRIGHT 2021 Milandr ****************************
*
* END OF FILE MDR32F9Qx_keypad.h */
