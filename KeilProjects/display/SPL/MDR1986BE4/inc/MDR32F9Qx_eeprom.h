/**
  ******************************************************************************
  * @file    MDR32F9Qx_eeprom.h
  * @author  Milandr Application Team
  * @version V2.0.2
  * @date    22/09/2021
  * @brief   This file contains all the functions prototypes for the EEPROM
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
#ifndef __MDR32F9Qx_EEPROM_H
#define __MDR32F9Qx_EEPROM_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "MDR32F9Qx_config.h"

/** @addtogroup __MDR1986BE4_StdPeriph_Driver MDR1986BE4 Standard Peripherial Driver
  * @{
  */

/** @addtogroup EEPROM
  * @{
  */

/** @defgroup EEPROM_Exported_Types EEPROM Exported Types
  * @{
  */

/**
  * @brief EEPROM Latency
  */
typedef enum
{
    EEPROM_Latency_0 = ((uint32_t)0x00), /*!< EEPROM 0 Latency cycle (MCU Clock up to 18 MHz) */
    EEPROM_Latency_1 = ((uint32_t)0x08)  /*!< EEPROM 1 Latency cycle (MCU Clock up to 36 MHz) */
} EEPROM_Latency_Cycles;

#define EEPROM_Latency_MSK      ((uint32_t)0x00000038)

#define IS_EEPROM_LATENCY(EEPROM_Latency) (((EEPROM_Latency) & (~EEPROM_Latency_MSK)) == 0x0)


/**
  * @brief EEPROM Bank Selector
  */
typedef enum
{
    EEPROM_Main_Bank_Select = ((uint32_t)0x00), /*!< EEPROM Main Bank Selector */
    EEPROM_Info_Bank_Select = ((uint32_t)0x01)  /*!< EEPROM Information Bank Selector */
} EEPROM_Mem_Bank;

#define IS_EEPROM_BANK_SELECTOR(SEL)    (((SEL) == EEPROM_Main_Bank_Select) || \
                                         ((SEL) == EEPROM_Info_Bank_Select))

/** @} */ /* End of group EEPROM_Exported_Types */


/** @defgroup EEPROM_Private_Defines EEPROM Private Defines
  * @{
  */
#if defined (__ICCARM__) /* IAR Compiler */
    #define __RAMFUNC    __ramfunc
#elif defined (__CMCARM__) /* Phyton CMC-ARM Compiler */
    #define __RAMFUNC    __ramfunc
#elif defined (__ARMCC_VERSION) /* ARM Compiler */
    #define __RAMFUNC
#endif
/** @} */ /* End of group EEPROM_Private_Defines */


/** @defgroup EEPROM_Exported_Functions EEPROM Exported Functions
  * @{
  */

/** @defgroup EEPROM_Functions_allowed_to_be_located_in_Flash Functions allowed to be located in Flash
  * @{
  */

void EEPROM_SetLatency(EEPROM_Latency_Cycles EEPROM_Latency);
EEPROM_Latency_Cycles EEPROM_GetLatency(void);

/** @} */ /* End of group EEPROM_Functions_allowed_to_be_located_in_Flash */

/** @defgroup EEPROM_Functions_not_allowed_to_be_located_in_Flash Functions not allowed to be located in Flash
  * @{
  */

__RAMFUNC uint8_t EEPROM_ReadByte(uint32_t Address, EEPROM_Mem_Bank BankSelector) __attribute__((section("EXECUTABLE_MEMORY_SECTION")));
__RAMFUNC uint16_t EEPROM_ReadHalfWord(uint32_t Address, EEPROM_Mem_Bank BankSelector) __attribute__((section("EXECUTABLE_MEMORY_SECTION")));
__RAMFUNC uint32_t EEPROM_ReadWord(uint32_t Address, EEPROM_Mem_Bank BankSelector) __attribute__((section("EXECUTABLE_MEMORY_SECTION")));
__RAMFUNC void EEPROM_ErasePage(uint32_t Address, EEPROM_Mem_Bank BankSelector) __attribute__((section("EXECUTABLE_MEMORY_SECTION")));
__RAMFUNC void EEPROM_EraseBlock(uint32_t Address, EEPROM_Mem_Bank BankSelector) __attribute__((section("EXECUTABLE_MEMORY_SECTION")));
__RAMFUNC void EEPROM_ProgramByte(uint32_t Address, EEPROM_Mem_Bank BankSelector, uint32_t Data) __attribute__((section("EXECUTABLE_MEMORY_SECTION")));
__RAMFUNC void EEPROM_ProgramHalfWord(uint32_t Address, EEPROM_Mem_Bank BankSelector, uint32_t Data) __attribute__((section("EXECUTABLE_MEMORY_SECTION")));
__RAMFUNC void EEPROM_ProgramWord(uint32_t Address, EEPROM_Mem_Bank BankSelector, uint32_t Data) __attribute__((section("EXECUTABLE_MEMORY_SECTION")));
__RAMFUNC void EEPROM_UpdateDCache(void) __attribute__((section("EXECUTABLE_MEMORY_SECTION")));

/** @} */ /* End of group EEPROM_Functions_not_allowed_to_be_located_in_Flash */

/** @} */ /* End of group EEPROM_Exported_Functions */

/** @} */ /* End of group EEPROM */

/** @} */ /* End of group __MDR1986BE4_StdPeriph_Driver */

#ifdef __cplusplus
} // extern "C" block end
#endif

#endif /* __MDR32F9Qx_EEPROM_H */

/*********************** (C) COPYRIGHT 2021 Milandr ****************************
*
* END OF FILE MDR32F9Qx_eeprom.h */
