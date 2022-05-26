/**
  ******************************************************************************
  * @file    MDR32F9Qx_bkp.h
  * @author  Milandr Application Team
  * @version V2.0.2
  * @date    22/09/2021
  * @brief   This file contains all the functions prototypes for the BKP
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
#ifndef __MDR32F9Qx_BKP_H
#define __MDR32F9Qx_BKP_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "MDR32F9Qx_config.h"

/** @addtogroup __MDR1986BE4_StdPeriph_Driver MDR1986BE4 Standard Peripherial Driver
  * @{
  */

/** @addtogroup BKP
  * @{
  */


/** @defgroup BKP_Exported_Types BKP Exported Types
  * @{
  */

/**
  * @brief Data Backup Registers
  */
typedef enum
{
    BKP_REG00 = ((uint32_t)0x00),
    BKP_REG01 = ((uint32_t)0x04),
    BKP_REG02 = ((uint32_t)0x08),
    BKP_REG03 = ((uint32_t)0x0C),
    BKP_REG04 = ((uint32_t)0x10),
    BKP_REG05 = ((uint32_t)0x14),
    BKP_REG06 = ((uint32_t)0x18),
    BKP_REG07 = ((uint32_t)0x1C),
    BKP_REG08 = ((uint32_t)0x20),
    BKP_REG09 = ((uint32_t)0x24),
    BKP_REG0A = ((uint32_t)0x28),
    BKP_REG0B = ((uint32_t)0x2C),
    BKP_REG0C = ((uint32_t)0x30),
    BKP_REG0D = ((uint32_t)0x34),
    BKP_REG0E = ((uint32_t)0x38),
    BKP_REG0F = ((uint32_t)0x3C)
} Data_Backup_Registers;

#define IS_BKP_REG(REG)       (((REG) == BKP_REG00) || ((REG) == BKP_REG01) || \
                               ((REG) == BKP_REG02) || ((REG) == BKP_REG03) || \
                               ((REG) == BKP_REG04) || ((REG) == BKP_REG05) || \
                               ((REG) == BKP_REG06) || ((REG) == BKP_REG07) || \
                               ((REG) == BKP_REG08) || ((REG) == BKP_REG09) || \
                               ((REG) == BKP_REG0A) || ((REG) == BKP_REG0B) || \
                               ((REG) == BKP_REG0C) || ((REG) == BKP_REG0D) || \
                               ((REG) == BKP_REG0E) || ((REG) == BKP_REG0F))

#define IS_USER_BKP_REG(REG)  (((REG) == BKP_REG00) || ((REG) == BKP_REG01) || \
                               ((REG) == BKP_REG02) || ((REG) == BKP_REG03) || \
                               ((REG) == BKP_REG04) || ((REG) == BKP_REG05) || \
                               ((REG) == BKP_REG06) || ((REG) == BKP_REG07) || \
                               ((REG) == BKP_REG08) || ((REG) == BKP_REG09) || \
                               ((REG) == BKP_REG0A) || ((REG) == BKP_REG0B) || \
                               ((REG) == BKP_REG0C) || ((REG) == BKP_REG0D))

/**
  * @brief BKP RTC CLK
  */
typedef enum
{
    BKP_RTC_LSIclk = ((uint32_t)0x00), /*!< LSI oscillator clock is selected as RTC clock source. */
    BKP_RTC_LSEclk = ((uint32_t)0x04), /*!< LSE oscillator clock is selected as RTC clock source. */
    BKP_RTC_HSIclk = ((uint32_t)0x08), /*!< HSI oscillator clock is selected as RTC clock source. */
    BKP_RTC_HSEclk = ((uint32_t)0x0C)  /*!< HSE oscillator clock is selected as RTC clock source. */
} BKP_RTC_CLK_TypeDef;

#define IS_RTC_CLK_SOURCE(CLK) (((CLK) == BKP_RTC_LSIclk) || \
                                ((CLK) == BKP_RTC_LSEclk) || \
                                ((CLK) == BKP_RTC_HSIclk) || \
                                ((CLK) == BKP_RTC_HSEclk))

/**
  * @brief BKP RTC CLK
  */
typedef enum
{
    BKP_RTC_IT_ALRF = ((uint32_t)BKP_RTC_CS_ALRF_IE), /*!< Alarm interrupt. */
    BKP_RTC_IT_SECF = ((uint32_t)BKP_RTC_CS_SECF_IE), /*!< Second interrupt. */
    BKP_RTC_IT_OWF  = ((uint32_t)BKP_RTC_CS_OWF_IE)   /*!< Overflow interrupt. */
} BKP_RTC_IT;

#define IS_RTC_CLK_IT(IE) (((IE) & ~(BKP_RTC_IT_ALRF | BKP_RTC_IT_SECF | BKP_RTC_IT_OWF)) == 0)

/**
  * @brief BKP RTC FLAG
  */
typedef enum
{
    BKP_RTC_FLAG_WEC  = ((uint32_t)BKP_RTC_CS_WEC),  /*!< RTC Write operation ended flag */
    BKP_RTC_FLAG_OWF  = ((uint32_t)BKP_RTC_CS_OWF),  /*!< Overflow flag */
    BKP_RTC_FLAG_ALRF = ((uint32_t)BKP_RTC_CS_ALRF), /*!< Alarm flag */
    BKP_RTC_FLAG_SECF = ((uint32_t)BKP_RTC_CS_SECF)  /*!< Second flag */
} BKP_RTC_FLAGS;

#define IS_BKP_RTC_GET_FLAG(F) (((F) == BKP_RTC_CS_WEC)  || \
                                ((F) == BKP_RTC_CS_OWF)  || \
                                ((F) == BKP_RTC_CS_ALRF) || \
                                ((F) == BKP_RTC_CS_SECF))

#define IS_BKP_RTC_CLAER_FLAG(F) (((F) == BKP_RTC_CS_OWF)  || \
                                  ((F) == BKP_RTC_CS_ALRF) || \
                                  ((F) == BKP_RTC_CS_SECF))

/**
  * @brief BKP DUcc Mode
  */
typedef enum
{
    BKP_DUcc_upto_10MHz  = (uint32_t)(0x00),                                     /*!< system clock is up to 10 MHz */
    BKP_DUcc_upto_200kHz = (uint32_t)(0x01 | (0x01 << BKP_REG_0E_SELECTRI_Pos)), /*!< system clock is up to 200 kHz */
    BKP_DUcc_upto_500kHz = (uint32_t)(0x02 | (0x02 << BKP_REG_0E_SELECTRI_Pos)), /*!< system clock is up to 500 kHz */
    BKP_DUcc_upto_1MHz   = (uint32_t)(0x03 | (0x03 << BKP_REG_0E_SELECTRI_Pos)), /*!< system clock is up to 1 MHz */
    BKP_DUcc_clk_off     = (uint32_t)(0x04 | (0x04 << BKP_REG_0E_SELECTRI_Pos)), /*!< all clocks are switched off */
    BKP_DUcc_upto_40MHz  = (uint32_t)(0x05 | (0x05 << BKP_REG_0E_SELECTRI_Pos)), /*!< system clock is up to 40 MHz */
    BKP_DUcc_upto_80MHz  = (uint32_t)(0x06 | (0x06 << BKP_REG_0E_SELECTRI_Pos)), /*!< system clock is up to 80 MHz */
    BKP_DUcc_over_80MHz  = (uint32_t)(0x07 | (0x07 << BKP_REG_0E_SELECTRI_Pos))  /*!< system clock is over 80 MHz */
} BKP_DUcc_Mode;

#define IS_BKP_DUCC_MODE(MODE) (((MODE) == BKP_DUcc_upto_10MHz) || \
                                ((MODE) == BKP_DUcc_upto_200kHz)|| \
                                ((MODE) == BKP_DUcc_upto_500kHz)|| \
                                ((MODE) == BKP_DUcc_clk_off)    || \
                                ((MODE) == BKP_DUcc_upto_1MHz)  || \
                                ((MODE) == BKP_DUcc_upto_40MHz) || \
                                ((MODE) == BKP_DUcc_upto_80MHz) || \
                                ((MODE) == BKP_DUcc_over_80MHz))

/**
  * @brief BKP BKP Trim
  */
typedef enum
{
    BKP_TRIM_1_8_V = (0 << BKP_REG_0E_TRIM43_Pos),
    BKP_TRIM_1_6_V = (1 << BKP_REG_0E_TRIM43_Pos),
    BKP_TRIM_1_4_V = (2 << BKP_REG_0E_TRIM43_Pos)
} BKP_TRIM;

#define IS_BKP_TRIM(TRIM)   (((TRIM) == BKP_TRIM_1_4_V) ||\
                             ((TRIM) == BKP_TRIM_1_6_V) ||\
                             ((TRIM) == BKP_TRIM_1_8_V))

/**
  * @brief BKP DUcc Trim
  */
typedef enum
{
    BKP_DUcc_plus_100mV  = ((uint32_t)(0x0000 << 8)), /*!< trim DUcc with + 100mV */
    BKP_DUcc_plus_060mV  = ((uint32_t)(0x0001 << 8)), /*!< trim DUcc with + 060mV */
    BKP_DUcc_plus_040mV  = ((uint32_t)(0x0002 << 8)), /*!< trim DUcc with + 040mV */
    BKP_DUcc_plus_010mV  = ((uint32_t)(0x0003 << 8)), /*!< trim DUcc with + 010mV */
    BKP_DUcc_minus_010mV = ((uint32_t)(0x0004 << 8)), /*!< trim DUcc with - 010mV */
    BKP_DUcc_minus_040mV = ((uint32_t)(0x0005 << 8)), /*!< trim DUcc with - 040mV */
    BKP_DUcc_minus_060mV = ((uint32_t)(0x0006 << 8)), /*!< trim DUcc with - 060mV */
    BKP_DUcc_minus_100mV = ((uint32_t)(0x0007 << 8))  /*!< trim DUcc with - 100mV */
} BKP_DUcc_Trim;

#define IS_BKP_DUCC_TRIM(TRIM) (((TRIM) == BKP_DUcc_plus_100mV) || \
                                ((TRIM) == BKP_DUcc_plus_060mV) || \
                                ((TRIM) == BKP_DUcc_plus_040mV) || \
                                ((TRIM) == BKP_DUcc_plus_010mV) || \
                                ((TRIM) == BKP_DUcc_minus_010mV)|| \
                                ((TRIM) == BKP_DUcc_minus_040mV)|| \
                                ((TRIM) == BKP_DUcc_minus_060mV)|| \
                                ((TRIM) == BKP_DUcc_minus_100mV))

/**
  * @brief BKP Stop Entry Mode
  */
typedef enum
{
    BKP_STOPentry_WFE = ((uint32_t)0x00), /*!< enter STOP mode with WFE instruction */
    BKP_STOPentry_WFI = ((uint32_t)0x01)  /*!< enter STOP mode with WFI instruction */
} BKP_Stop_Entry_Mode;

#define IS_BKP_STOP_ENTRY(F) (((F) == BKP_STOPentry_WFI) || ((F) == BKP_STOPentry_WFE))

/** @} */ /* End of group BKP_Exported_Types */


/** @defgroup BKP_Exported_Constants BKP Exported Constants
  * @{
  */

/** @defgroup BKP_RTC_CALIBRATION BKP RTC CALIBRATION
  * @{
  */

#define IS_RTC_CALIBRATION(CAL) ((CAL >= 0) && (CAL <= 255))

/** @} */ /* End of group BKP_RTC_CALIBRATION */

/** @defgroup BKP_PRESCALER_VALUE BKP PRESCALER VALUE
  * @{
  */

#define IS_BKP_PRESCALER_VALUE(VAL) ((uint32_t)(VAL) <= 0x000FFFFF)

/** @} */ /* End of group BKP_PRESCALER_VALUE */

/** @} */ /* End of group BKP_Exported_Constants */


/** @defgroup BKP_Exported_Functions BKP Exported Functions
  * @{
  */
void BKP_DeInit(void);
void BKP_SWD_CMD(FunctionalState NewState);
void BKP_RTCclkSource(BKP_RTC_CLK_TypeDef RTC_CLK);
void BKP_RTC_WorkPermit(FunctionalState NewState);
#if defined (MDR_LEGACY_SUPPORT) && (MDR_LEGACY_SUPPORT) == 153
    #warning "Legacy v1.5.3: BKP_RTC_Enable() will be removed and replaced with BKP_RTC_WorkPermit()"
    #define BKP_RTC_Enable(NewState) BKP_RTC_WorkPermit(NewState)
#endif /* MDR_LEGACY_SUPPORT 153 */
void BKP_RTC_Calibration(uint32_t RTC_Calibration);
void BKP_RTC_Reset(FunctionalState NewState);
void BKP_RTC_ITConfig(BKP_RTC_IT BKP_RTC_IT_Source, FunctionalState NewState);
uint32_t BKP_RTC_GetCounter(void);
void BKP_RTC_SetCounter(uint32_t CounterValue);
void BKP_RTC_SetAlarm(uint32_t AlarmValue);
uint32_t BKP_RTC_GetDivider(void);
void BKP_RTC_SetPrescaler(uint32_t PrescalerValue);
void BKP_RTC_WaitForUpdate(void);
FlagStatus BKP_RTC_GetFlagStatus(BKP_RTC_FLAGS BKP_RTC_Flag_Value);
void BKP_RTC_ClearFlagStatus(uint32_t BKP_RTC_FLAG);
void BKP_DUccMode(BKP_DUcc_Mode DUccMode);
void BKP_SetTrim(BKP_TRIM trim);
void BKP_DUccTrim(BKP_DUcc_Trim DUccTrim);
void BKP_SetFlagPOR(void);
ErrorStatus BKP_FlagPORstatus(void);
void BKP_EnterSTOPMode(FunctionalState BKP_Regulator_state, BKP_Stop_Entry_Mode BKP_STOPEntry);
void BKP_CurrentLimitProtection150mA(FunctionalState NewState);
void BKP_EnterSTANDBYMode(void);
FunctionalState BKP_COVDET_Status(void);

/** @} */ /* End of group BKP_Exported_Functions */

/** @} */ /* End of group BKP */

/** @} */ /* End of group __MDR1986BE4_StdPeriph_Driver */

#ifdef __cplusplus
} // extern "C" block end
#endif

#endif /* __MDR32F9Qx_BKP_H */

/*********************** (C) COPYRIGHT 2021 Milandr ****************************
*
* END OF FILE MDR32F9Qx_bkp.h */
