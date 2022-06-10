/**
  ******************************************************************************
  * @file    MDR32F9Qx_rst_clk.h
  * @author  Milandr Application Team
  * @version V2.0.2
  * @date    22/09/2021
  * @brief   This file contains all the functions prototypes for the RST_CLK
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
#ifndef __MDR32F9Qx_RST_CLK_H
#define __MDR32F9Qx_RST_CLK_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "MDR32F9Qx_config.h"

/** @addtogroup __MDR1986BE4_StdPeriph_Driver MDR1986BE4 Standard Peripherial Driver
  * @{
  */

/** @addtogroup RST_CLK
  * @{
  */

/** @defgroup RST_CLK_Exported_Types RST_CLK Exported Types
  * @{
  */

/**
  * @brief Structure type for modules clocks frequencies expressed in Hz
  */
typedef struct
{
    uint32_t CPU_CLK_Frequency;
    uint32_t UART_Frequency[2];  // UART1 and UART2 frequencies after prescaler
    uint32_t TIMER_Frequency[2]; // TIMER1 and TIMER2 frequencies after prescaler
    uint32_t SSP_Frequency;      // SSP1 frequency after prescaler
    uint32_t ADC_CLK_Frequency;
    uint32_t ADCIU_CLK_Frequency;
    uint32_t RTCHSI_Frequency;
    uint32_t RTCHSE_Frequency;
} RST_CLK_FreqTypeDef;

/**
  * @brief RST_CLK unit non-volatile settings init structure
  */
typedef struct
{
    uint32_t REG_0F;
} Init_NonVolatile_RST_CLK_TypeDef;

/**
  * @brief RST_CLK HSE (High Speed External) clock mode and source selection constants
  */
typedef enum
{
    RST_CLK_HSE_OFF    = ((uint32_t)0x00), /*!< Switch off HSE clock generator. */
    RST_CLK_HSE_ON     = ((uint32_t)0x01), /*!< Switch on HSE clock generator. */
    RST_CLK_HSE_Bypass = ((uint32_t)0x02)  /*!< Use external clock source. */
} RST_CLK_HSE_Mode;

#define IS_RST_CLK_HSE(HSE)     (((HSE) == RST_CLK_HSE_OFF) || \
                                 ((HSE) == RST_CLK_HSE_ON)  || \
                                 ((HSE) == RST_CLK_HSE_Bypass))

/**
  * @brief RST_CLK LSE (Low Speed External) clock mode and source selection constants
  */
typedef enum
{
    RST_CLK_LSE_OFF    = ((uint32_t)0x00), /*!< Switch off LSE clock generator. */
    RST_CLK_LSE_ON     = ((uint32_t)0x01), /*!< Switch on LSE clock generator. */
    RST_CLK_LSE_Bypass = ((uint32_t)0x02)  /*!< Use external clock source. */
} RST_CLK_LSE_Mode;

#define IS_RST_CLK_LSE(LSE)     (((LSE) == RST_CLK_LSE_OFF) || \
                                 ((LSE) == RST_CLK_LSE_ON)  || \
                                 ((LSE) == RST_CLK_LSE_Bypass))

/**
  * @brief RST_CLK CPU_PLL clock mode and source selection constants
  */
typedef enum
{
    RST_CLK_CPU_PLLsrcHSIdiv1 = ((uint32_t)0x00), /*!< HSI oscillator clock is selected as CPU_PLL clock entry. */
    RST_CLK_CPU_PLLsrcHSIdiv2 = ((uint32_t)0x01), /*!< HSI oscillator clock divided by 2 is selected as CPU_PLL clock entry. */
    RST_CLK_CPU_PLLsrcHSEdiv1 = ((uint32_t)0x02), /*!< HSE oscillator clock is selected as CPU_PLL clock entry. */
    RST_CLK_CPU_PLLsrcHSEdiv2 = ((uint32_t)0x03)  /*!< HSE oscillator clock divided by 2 is selected as CPU_PLL clock entry. */
} RST_CLK_CPU_PLL_Source;

#define IS_RST_CLK_CPU_PLL_SOURCE(SRC)  (((SRC) == RST_CLK_CPU_PLLsrcHSIdiv1) || \
                                         ((SRC) == RST_CLK_CPU_PLLsrcHSIdiv2) || \
                                         ((SRC) == RST_CLK_CPU_PLLsrcHSEdiv1) || \
                                         ((SRC) == RST_CLK_CPU_PLLsrcHSEdiv2))

/**
  * @brief RST_CLK CPU_C1 clock mode and source selection constants
  */
typedef enum
{
    RST_CLK_CPU_C1srcHSIdiv1 = ((uint32_t)0x00),
    RST_CLK_CPU_C1srcHSIdiv2 = ((uint32_t)0x01),
    RST_CLK_CPU_C1srcHSEdiv1 = ((uint32_t)0x02),
    RST_CLK_CPU_C1srcHSEdiv2 = ((uint32_t)0x03)
} RST_CLK_CPU_C1_Source;

#define IS_RST_CLK_CPU_C1_SOURCE(SRC)   (((SRC) == RST_CLK_CPU_C1srcHSIdiv1) || \
                                         ((SRC) == RST_CLK_CPU_C1srcHSIdiv2) || \
                                         ((SRC) == RST_CLK_CPU_C1srcHSEdiv1) || \
                                         ((SRC) == RST_CLK_CPU_C1srcHSEdiv2))

/**
  * @brief RST_CLK PLL_CPU_MUL multiplier of the CPU_C1 clock constants
  */
typedef enum
{
    RST_CLK_CPU_PLLmul1  = ((uint32_t)0x00),
    RST_CLK_CPU_PLLmul2  = ((uint32_t)0x01),
    RST_CLK_CPU_PLLmul3  = ((uint32_t)0x02),
    RST_CLK_CPU_PLLmul4  = ((uint32_t)0x03),
    RST_CLK_CPU_PLLmul5  = ((uint32_t)0x04),
    RST_CLK_CPU_PLLmul6  = ((uint32_t)0x05),
    RST_CLK_CPU_PLLmul7  = ((uint32_t)0x06),
    RST_CLK_CPU_PLLmul8  = ((uint32_t)0x07),
    RST_CLK_CPU_PLLmul9  = ((uint32_t)0x08),
    RST_CLK_CPU_PLLmul10 = ((uint32_t)0x09),
    RST_CLK_CPU_PLLmul11 = ((uint32_t)0x0A),
    RST_CLK_CPU_PLLmul12 = ((uint32_t)0x0B),
    RST_CLK_CPU_PLLmul13 = ((uint32_t)0x0C),
    RST_CLK_CPU_PLLmul14 = ((uint32_t)0x0D),
    RST_CLK_CPU_PLLmul15 = ((uint32_t)0x0E),
    RST_CLK_CPU_PLLmul16 = ((uint32_t)0x0F)
} RST_CLK_CPU_PLL_Multiplier;

#define IS_RST_CLK_CPU_PLL_MUL(SRC)     (((SRC) >= 0) && ((SRC) < 16))

/**
  * @brief RST_CLK CPU_PLL output clock CPU_CLK_DIV divider constants
  */
typedef enum
{
    RST_CLK_CPUclkDIV1   = ((uint32_t)0x00),
    RST_CLK_CPUclkDIV2   = ((uint32_t)0x08),
    RST_CLK_CPUclkDIV4   = ((uint32_t)0x09),
    RST_CLK_CPUclkDIV8   = ((uint32_t)0x0A),
    RST_CLK_CPUclkDIV16  = ((uint32_t)0x0B),
    RST_CLK_CPUclkDIV32  = ((uint32_t)0x0C),
    RST_CLK_CPUclkDIV64  = ((uint32_t)0x0D),
    RST_CLK_CPUclkDIV128 = ((uint32_t)0x0E),
    RST_CLK_CPUclkDIV256 = ((uint32_t)0x0F)
} RST_CLK_CPU_C3_Divisor;

#define IS_RST_CLK_CPUclkDIV(DIV)   (((DIV) == RST_CLK_CPUclkDIV1)   || \
                                     ((DIV) == RST_CLK_CPUclkDIV2)   || \
                                     ((DIV) == RST_CLK_CPUclkDIV4)   || \
                                     ((DIV) == RST_CLK_CPUclkDIV8)   || \
                                     ((DIV) == RST_CLK_CPUclkDIV16)  || \
                                     ((DIV) == RST_CLK_CPUclkDIV32)  || \
                                     ((DIV) == RST_CLK_CPUclkDIV64)  || \
                                     ((DIV) == RST_CLK_CPUclkDIV128) || \
                                     ((DIV) == RST_CLK_CPUclkDIV256))

/**
  * @brief RST_CLK CPU_CLK source CPU_CLK selector constants
  */
typedef enum
{
    RST_CLK_CPUclkHSI    = ((uint32_t)0x0000),
    RST_CLK_CPUclkCPU_C3 = ((uint32_t)0x0100),
    RST_CLK_CPUclkLSE    = ((uint32_t)0x0200),
    RST_CLK_CPUclkLSI    = ((uint32_t)0x0300)
} RST_CLK_HCLK_Source;

#define IS_RST_CPU_CLK(SRC)     (((SRC) == RST_CLK_CPUclkHSI)    || \
                                 ((SRC) == RST_CLK_CPUclkCPU_C3) || \
                                 ((SRC) == RST_CLK_CPUclkLSE)    || \
                                 ((SRC) == RST_CLK_CPUclkLSI))

/**
  * @brief RST_CLK ADC_CLK source selector constants
  */
typedef enum
{
    RST_CLK_ADCclkCPU_C1  = ((uint32_t)0x00),
    RST_CLK_ADCclkPER1_C1 = ((uint32_t)0x01),
    RST_CLK_ADCclkPLLCPU  = ((uint32_t)0x02),
    RST_CLK_ADCclkHSI_CLK = ((uint32_t)0x03)
} RST_CLK_ADC_Source;

#define IS_RST_CLK_ADCclk(SRC)  (((SRC) == RST_CLK_ADCclkCPU_C1)  || \
                                 ((SRC) == RST_CLK_ADCclkPER1_C1) || \
                                 ((SRC) == RST_CLK_ADCclkPLLCPU)  || \
                                 ((SRC) == RST_CLK_ADCclkHSI_CLK))

/**
  * @brief RST_CLK ADC_CS2_SEL output clock ADC_CS3_SEL divider constants
  */
typedef enum
{
    RST_CLK_ADCclkDIV1   = ((uint32_t)0x00),
    RST_CLK_ADCclkDIV2   = ((uint32_t)0x08),
    RST_CLK_ADCclkDIV4   = ((uint32_t)0x09),
    RST_CLK_ADCclkDIV8   = ((uint32_t)0x0A),
    RST_CLK_ADCclkDIV16  = ((uint32_t)0x0B),
    RST_CLK_ADCclkDIV32  = ((uint32_t)0x0C),
    RST_CLK_ADCclkDIV64  = ((uint32_t)0x0D),
    RST_CLK_ADCclkDIV128 = ((uint32_t)0x0E),
    RST_CLK_ADCclkDIV256 = ((uint32_t)0x0F)
} RST_CLK_ADC_C3_Divisor;

#define IS_RST_CLK_ADCclkDivValue(SRC)  (((SRC) == RST_CLK_ADCclkDIV1)   || \
                                         ((SRC) == RST_CLK_ADCclkDIV2)   || \
                                         ((SRC) == RST_CLK_ADCclkDIV4)   || \
                                         ((SRC) == RST_CLK_ADCclkDIV8)   || \
                                         ((SRC) == RST_CLK_ADCclkDIV16)  || \
                                         ((SRC) == RST_CLK_ADCclkDIV32)  || \
                                         ((SRC) == RST_CLK_ADCclkDIV64)  || \
                                         ((SRC) == RST_CLK_ADCclkDIV128) || \
                                         ((SRC) == RST_CLK_ADCclkDIV256))

/**
 * @brief RST_CLK ADCIU_CLK selector constants
 */
typedef enum
{
    RST_CLK_ADCIUclkCPU_C1  = ((uint32_t)0x00),
    RST_CLK_ADCIUclkPER1_C1 = ((uint32_t)0x04),
    RST_CLK_ADCIUclkPLLCPU  = ((uint32_t)0x08),
    RST_CLK_ADCIUclkHSI_CLK = ((uint32_t)0x0C)
} RST_CLK_ADCIU_Source;

#define IS_RST_CLK_ADCIUclk(SRC)    (((SRC) == RST_CLK_ADCIUclkCPU_C1)  || \
                                     ((SRC) == RST_CLK_ADCIUclkPER1_C1) || \
                                     ((SRC) == RST_CLK_ADCIUclkPLLCPU)  || \
                                     ((SRC) == RST_CLK_ADCIUclkHSI_CLK))

/**
  * @brief RST_CLK ADCIU_CLK output clock ADCIU_CLK_DIV divider constants
  */
typedef enum
{
    RST_CLK_ADCIUclkDIV1   = ((uint32_t)0x00),
    RST_CLK_ADCIUclkDIV2   = ((uint32_t)0x80),
    RST_CLK_ADCIUclkDIV4   = ((uint32_t)0x90),
    RST_CLK_ADCIUclkDIV8   = ((uint32_t)0xA0),
    RST_CLK_ADCIUclkDIV16  = ((uint32_t)0xB0),
    RST_CLK_ADCIUclkDIV32  = ((uint32_t)0xC0),
    RST_CLK_ADCIUclkDIV64  = ((uint32_t)0xD0),
    RST_CLK_ADCIUclkDIV128 = ((uint32_t)0xE0),
    RST_CLK_ADCIUclkDIV256 = ((uint32_t)0xF0)
} RST_CLK_ADCIU_C3_Divisor;

#define IS_RST_CLK_ADCIUclkDIV(DIV)     (((DIV) == RST_CLK_ADCIUclkDIV1)   || \
                                         ((DIV) == RST_CLK_ADCIUclkDIV2)   || \
                                         ((DIV) == RST_CLK_ADCIUclkDIV4)   || \
                                         ((DIV) == RST_CLK_ADCIUclkDIV8)   || \
                                         ((DIV) == RST_CLK_ADCIUclkDIV16)  || \
                                         ((DIV) == RST_CLK_ADCIUclkDIV32)  || \
                                         ((DIV) == RST_CLK_ADCIUclkDIV64)  || \
                                         ((DIV) == RST_CLK_ADCIUclkDIV128) || \
                                         ((DIV) == RST_CLK_ADCIUclkDIV256))

#define PCLK_BIT(BASE)      ((uint32_t)(1 << ((((uint32_t)BASE) >> 15) & 0x1F)))

/**
  * @brief RST_CLK peripheral modules clock constants
  */
typedef enum
{
    RST_CLK_PCLK_SSP1    = PCLK_BIT(MDR_SSP1_BASE),
    RST_CLK_PCLK_UART1   = PCLK_BIT(MDR_UART1_BASE),
    RST_CLK_PCLK_UART2   = PCLK_BIT(MDR_UART2_BASE),
    RST_CLK_PCLK_EEPROM  = PCLK_BIT(MDR_EEPROM_BASE),
    RST_CLK_PCLK_RST_CLK = PCLK_BIT(MDR_RST_CLK_BASE),
    RST_CLK_PCLK_DMA     = PCLK_BIT(MDR_DMA_BASE),
    RST_CLK_PCLK_COMP    = PCLK_BIT(MDR_COMP_BASE),
    RST_CLK_PCLK_DAC     = PCLK_BIT(MDR_DAC_BASE),
    RST_CLK_PCLK_ADC     = PCLK_BIT(MDR_ADC_BASE),
    RST_CLK_PCLK_WWDG    = PCLK_BIT(MDR_WWDG_BASE),
    RST_CLK_PCLK_IWDG    = PCLK_BIT(MDR_IWDG_BASE),
    RST_CLK_PCLK_POWER   = PCLK_BIT(MDR_POWER_BASE),
    RST_CLK_PCLK_BKP     = PCLK_BIT(MDR_BKP_BASE),
    RST_CLK_PCLK_ADCUI   = PCLK_BIT(MDR_ADCIU_BASE),
    RST_CLK_PCLK_TIMER1  = PCLK_BIT(MDR_TIMER1_BASE),
    RST_CLK_PCLK_TIMER2  = PCLK_BIT(MDR_TIMER2_BASE),
    RST_CLK_PCLK_PORTA   = PCLK_BIT(MDR_PORTA_BASE),
    RST_CLK_PCLK_PORTB   = PCLK_BIT(MDR_PORTB_BASE),
    RST_CLK_PCLK_PORTC   = PCLK_BIT(MDR_PORTC_BASE),
    RST_CLK_PCLK_CRC     = PCLK_BIT(MDR_CRC_BASE)
} RST_CLK_PCLK_TypeDef;

#define RST_CLK_PCLK_MASK   (RST_CLK_PCLK_SSP1    | RST_CLK_PCLK_UART1  | \
                             RST_CLK_PCLK_UART2   | RST_CLK_PCLK_EEPROM | \
                             RST_CLK_PCLK_RST_CLK | RST_CLK_PCLK_DMA    | \
                             RST_CLK_PCLK_COMP    | RST_CLK_PCLK_DAC    | \
                             RST_CLK_PCLK_ADC     | RST_CLK_PCLK_WWDG   | \
                             RST_CLK_PCLK_IWDG    | RST_CLK_PCLK_POWER  | \
                             RST_CLK_PCLK_BKP     | RST_CLK_PCLK_ADCUI  | \
                             RST_CLK_PCLK_TIMER1  | RST_CLK_PCLK_TIMER2 | \
                             RST_CLK_PCLK_PORTA   | RST_CLK_PCLK_PORTB  | \
                             RST_CLK_PCLK_PORTC   | RST_CLK_PCLK_CRC)

#define IS_RST_CLK_PCLK(PCLK)   ((PCLK & RST_CLK_PCLK_MASK) == PCLK)

/**
  * @brief RST_CLK PER1_C1 clock source constants
  */
typedef enum
{
    RST_CLK_PER1_C1_CLK_SRC_LSI   = ((uint32_t)0x00),
    RST_CLK_PER1_C1_CLK_SRC_LSI_2 = ((uint32_t)0x01),
    RST_CLK_PER1_C1_CLK_SRC_LSE   = ((uint32_t)0x02),
    RST_CLK_PER1_C1_CLK_SRC_LSE_2 = ((uint32_t)0x03)
} RST_CLK_PER1_C1_TypeDef;

#define IS_RST_CLK_PER1_C1_CLK_SRC(CLK_SRC)     ((CLK_SRC == RST_CLK_PER1_C1_CLK_SRC_LSI)   || \
                                                 (CLK_SRC == RST_CLK_PER1_C1_CLK_SRC_LSI_2) || \
                                                 (CLK_SRC == RST_CLK_PER1_C1_CLK_SRC_LSE)   || \
                                                 (CLK_SRC == RST_CLK_PER1_C1_CLK_SRC_LSE_2))

/**
  * @brief RST_CLK PER1_C2 clock source constants
  */
typedef enum
{
    RST_CLK_PER1_C2_CLK_SRC_CPU_C1  = ((uint32_t)0x00),
    RST_CLK_PER1_C2_CLK_SRC_PER1_C1 = ((uint32_t)0x04),
    RST_CLK_PER1_C2_CLK_SRC_PLLCPU  = ((uint32_t)0x08),
    RST_CLK_PER1_C2_CLK_SRC_HSI_CLK = ((uint32_t)0x0C)
} RST_CLK_PER1_C2_TypeDef;

#define IS_RST_CLK_PER1_C2_CLK_SRC(CLK_SRC)     ((CLK_SRC == RST_CLK_PER1_C2_CLK_SRC_CPU_C1) || \
                                                 (CLK_SRC == RST_CLK_PER1_C2_CLK_SRC_PER1_C1)|| \
                                                 (CLK_SRC == RST_CLK_PER1_C2_CLK_SRC_PLLCPU) || \
                                                 (CLK_SRC == RST_CLK_PER1_C2_CLK_SRC_HSI_CLK))

/**
  * @brief RST_CLK flag identifiers
  */
typedef enum
{
    RST_CLK_FLAG_HSIRDY    = ((uint32_t)(0x00 | 23)),
    RST_CLK_FLAG_LSIRDY    = ((uint32_t)(0x00 | 21)),
    RST_CLK_FLAG_HSERDY    = ((uint32_t)(0x20 |  2)),
    RST_CLK_FLAG_HSE2RDY   = ((uint32_t)(0x20 |  3)),
    RST_CLK_FLAG_LSERDY    = ((uint32_t)(0x00 | 13)),
    RST_CLK_FLAG_PLLCPURDY = ((uint32_t)(0x20 |  1))
} RST_CLK_Flags;

#define IS_RST_CLK_FLAG(FLAG)   (((FLAG) == RST_CLK_FLAG_HSIRDY)    || \
                                 ((FLAG) == RST_CLK_FLAG_LSIRDY)    || \
                                 ((FLAG) == RST_CLK_FLAG_HSERDY)    || \
                                 ((FLAG) == RST_CLK_FLAG_HSE2RDY)   || \
                                 ((FLAG) == RST_CLK_FLAG_LSERDY)    || \
                                 ((FLAG) == RST_CLK_FLAG_PLLCPURDY))

/**
  * @brief RST_CLK HSI clock HSI_C1_SEL divider constants
  */
typedef enum
{
    RST_CLK_HSIclkDIV1   = ((uint32_t)0x00),
    RST_CLK_HSIclkDIV2   = ((uint32_t)0x08),
    RST_CLK_HSIclkDIV4   = ((uint32_t)0x09),
    RST_CLK_HSIclkDIV8   = ((uint32_t)0x0A),
    RST_CLK_HSIclkDIV16  = ((uint32_t)0x0B),
    RST_CLK_HSIclkDIV32  = ((uint32_t)0x0C),
    RST_CLK_HSIclkDIV64  = ((uint32_t)0x0D),
    RST_CLK_HSIclkDIV128 = ((uint32_t)0x0E),
    RST_CLK_HSIclkDIV256 = ((uint32_t)0x0F)
} RST_CLK_HSI_C1_Divisor;

#define IS_RST_CLK_HSIclkDivValue(SRC)  (((SRC) == RST_CLK_HSIclkDIV1)   || \
                                         ((SRC) == RST_CLK_HSIclkDIV2)   || \
                                         ((SRC) == RST_CLK_HSIclkDIV4)   || \
                                         ((SRC) == RST_CLK_HSIclkDIV8)   || \
                                         ((SRC) == RST_CLK_HSIclkDIV16)  || \
                                         ((SRC) == RST_CLK_HSIclkDIV32)  || \
                                         ((SRC) == RST_CLK_HSIclkDIV64)  || \
                                         ((SRC) == RST_CLK_HSIclkDIV128) || \
                                         ((SRC) == RST_CLK_HSIclkDIV256))

/**
  * @brief RST_CLK HSE clock HSE_C1_SEL divider constants for RTCHSE
  */
typedef enum
{
    RST_CLK_HSEclkDIV1   = ((uint32_t)0x00),
    RST_CLK_HSEclkDIV2   = ((uint32_t)0x08),
    RST_CLK_HSEclkDIV4   = ((uint32_t)0x09),
    RST_CLK_HSEclkDIV8   = ((uint32_t)0x0A),
    RST_CLK_HSEclkDIV16  = ((uint32_t)0x0B),
    RST_CLK_HSEclkDIV32  = ((uint32_t)0x0C),
    RST_CLK_HSEclkDIV64  = ((uint32_t)0x0D),
    RST_CLK_HSEclkDIV128 = ((uint32_t)0x0E),
    RST_CLK_HSEclkDIV256 = ((uint32_t)0x0F)
} RST_CLK_HSE_C1_Divisor;

#define IS_RST_CLK_HSEclkDivValue(SRC)  (((SRC) == RST_CLK_HSEclkDIV1)   || \
                                         ((SRC) == RST_CLK_HSEclkDIV2)   || \
                                         ((SRC) == RST_CLK_HSEclkDIV4)   || \
                                         ((SRC) == RST_CLK_HSEclkDIV8)   || \
                                         ((SRC) == RST_CLK_HSEclkDIV16)  || \
                                         ((SRC) == RST_CLK_HSEclkDIV32)  || \
                                         ((SRC) == RST_CLK_HSEclkDIV64)  || \
                                         ((SRC) == RST_CLK_HSEclkDIV128) || \
                                         ((SRC) == RST_CLK_HSEclkDIV256))

/** @} */ /* End of group RST_CLK_Exported_Types */


/** @defgroup RST_CLK_Exported_Constants RST_CLK Exported Constants
  * @{
  */

#define IS_RCC_CLK_HSI_TRIM_VALUE(TRIM)     ((TRIM) <= 0x3F)
#define IS_RCC_CLK_LSI_TRIM_VALUE(TRIM)     ((TRIM) <= 0x1F)

/** @} */ /* End of group RST_CLK_Exported_Constants */


/** @defgroup RST_CLK_Exported_Functions RST_CLK Exported Functions
  * @{
  */

void RST_CLK_DeInit(void);
void RST_CLK_WarmDeInit(void);

void RST_CLK_HSEconfig(RST_CLK_HSE_Mode RST_CLK_HSE);
ErrorStatus RST_CLK_HSEstatus(void);

void RST_CLK_LSEconfig(RST_CLK_LSE_Mode RST_CLK_LSE);
ErrorStatus RST_CLK_LSEstatus(void);

void RST_CLK_HSIcmd(FunctionalState NewState);
void RST_CLK_HSIadjust(uint32_t HSItrimValue);
ErrorStatus RST_CLK_HSIstatus(void);

void RST_CLK_LSIcmd(FunctionalState NewState);
void RST_CLK_LSIadjust(uint32_t LSItrimValue);
ErrorStatus RST_CLK_LSIstatus(void);

void RST_CLK_CPU_PLLconfig(RST_CLK_CPU_PLL_Source RST_CLK_CPU_PLLsource, uint32_t RST_CLK_CPU_PLLmul);
void RST_CLK_CPU_PLLuse(FunctionalState UsePLL);
void RST_CLK_CPU_PLLcmd(FunctionalState NewState);
ErrorStatus RST_CLK_CPU_PLLstatus(void);

void RST_CLK_CPUclkPrescaler(RST_CLK_CPU_C3_Divisor CPUclkDivValue);
void RST_CLK_CPUclkSelection(RST_CLK_HCLK_Source CPU_CLK);

void RST_CLK_ADCclkSelection(RST_CLK_ADC_Source ADC_CLK);
void RST_CLK_ADCclkPrescaler(RST_CLK_ADC_C3_Divisor ADCclkDivValue);
void RST_CLK_ADCclkEnable(FunctionalState NewState);

void RST_CLK_ADCIUclkDeInit(void);
void RST_CLK_ADCIUclkSelection(RST_CLK_ADCIU_Source ADCIU_CLK_C1);
void RST_CLK_ADCIUclkPrescaler(RST_CLK_ADCIU_C3_Divisor ADCIUclkDIVValue);
void RST_CLK_ADCIUclkCMD(FunctionalState NewState);

void RST_CLK_HSIclkPrescaler(RST_CLK_HSI_C1_Divisor HSIclkDivValue);
void RST_CLK_RTC_HSIclkEnable(FunctionalState NewState);

void RST_CLK_HSEclkPrescaler(RST_CLK_HSE_C1_Divisor HSEclkDivValue);
void RST_CLK_RTC_HSEclkEnable(FunctionalState NewState);

void RST_CLK_PCLKPer1_C1_CLKSelection(RST_CLK_PER1_C1_TypeDef CLK_SRC);
void RST_CLK_PCLKPer1_C2_CLKSelection(RST_CLK_PER1_C2_TypeDef CLK_SRC);

void RST_CLK_CPUclkSelectionC1(RST_CLK_CPU_C1_Source CPU_CLK);

void RST_CLK_DMACmd(FunctionalState NewState);
FunctionalState RST_CLK_DMAState(void);

void RST_CLK_DEBUGCmd(FunctionalState NewState);
FunctionalState RST_CLK_DEBUGState(void);

void RST_CLK_PCLKcmd(uint32_t RST_CLK_PCLK, FunctionalState NewState);

void RST_CLK_GetClocksFreq(RST_CLK_FreqTypeDef* RST_CLK_Clocks);

FlagStatus RST_CLK_GetFlagStatus(RST_CLK_Flags RST_CLK_FLAG);

/** @} */ /* End of group RST_CLK_Exported_Functions */

/** @} */ /* End of group RST_CLK */

/** @} */ /* End of group __MDR1986BE4_StdPeriph_Driver */

#ifdef __cplusplus
} // extern "C" block end
#endif

#endif /* __MDR32F9Qx_RST_CLK_H */

/*********************** (C) COPYRIGHT 2021 Milandr ****************************
*
* END OF FILE MDR32F9Qx_rst_clk.h */
