/**
  ******************************************************************************
  * @file    mdr32f8_clkctrl.h
  * @author  Milandr Application Team
  * @version V1.0.0
  * @date    20/07/2015
  * @brief   This file contains all the functions prototypes for the CLKCTRL
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
  * FILE mdr32f8_clkctrl.h
  */
	
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MDR32F8_CLKCTRL_H
#define __MDR32F8_CLKCTRL_H	

/* Includes ------------------------------------------------------------------*/
#include "stdint.h"
#include "MDR1986VE8T.h"
#include "mdr32f8_config.h"

/** @addtogroup __MDR32F8_StdPeriph_Driver MDR32F8 Standard Peripherial Driver
  * @{
  */

/** @addtogroup CLKCTRL CLKCTRL
  * @{
  */

/** @defgroup CLKCTRL_Exported_Types CLKCTRL Exported Types
  * @{
  */

/* Exported types ------------------------------------------------------------*/
/**
  * @brief Structure type for modules clocks frequencies expressed in Hz
  */

typedef struct
{
  uint32_t CPU_CLK_Frequency;

}CLKCTRL_FreqTypeDef;

/**
  * @}
  */ 

/* Exported constants --------------------------------------------------------*/

/** @defgroup CLKCTRL_Exported_Constants CLKCTRL Exported Constants
  * @{
  */

/** @defgroup CLKCTRL_HSE0_configuration
  * @{
  */
#define CLKCTRL_HSE0_CLK_OFF                 ((uint32_t)0x00000000)
#define CLKCTRL_HSE0_CLK_ON                  ((uint32_t)0x08000000)
#define CLKCTRL_HSE0_CLK_BYP                 ((uint32_t)0x10000000)

#define IS_CLKCTRL_HSE0_CLK(HSE)             (((HSE) == CLKCTRL_HSE0_CLK_OFF) || \
                                              ((HSE) == CLKCTRL_HSE0_CLK_ON)  || \
                                              ((HSE) == CLKCTRL_HSE0_CLK_BYP))
/**
  * @}
  */ 

/** @defgroup CLKCTRL_HSE1_configuration
  * @{
  */
#define CLKCTRL_HSE1_CLK_OFF                 ((uint32_t)0x00000001)//+1 for case, but write CLKCTRL_HSE0_CLK_OFF
#define CLKCTRL_HSE1_CLK_ON                  ((uint32_t)0x08000001)//
#define CLKCTRL_HSE1_CLK_BYP                 ((uint32_t)0x10000001)//

#define IS_CLKCTRL_HSE1_CLK(HSE)             (((HSE) == CLKCTRL_HSE1_CLK_OFF) || \
                                              ((HSE) == CLKCTRL_HSE1_CLK_ON)  || \
                                              ((HSE) == CLKCTRL_HSE1_CLK_BYP))
/**
  * @}
  */ 

/** @defgroup BKPCTRL_REG_63_TMR0_LSE_configuration
  * @{
  */
#define BKPCTRL_REG_63_TMR0_LSE_OFF          ((uint32_t)0x00000000)
#define BKPCTRL_REG_63_TMR0_LSE_ON           ((uint32_t)0x00000001)
#define BKPCTRL_REG_63_TMR0_LSE_BYP       	 ((uint32_t)0x00000002)

#define IS_BKPCTRL_REG_63_TMR0_LSE(LSE)      (((LSE) == BKPCTRL_REG_63_TMR0_LSE_OFF) || \
                                              ((LSE) == BKPCTRL_REG_63_TMR0_LSE_ON)  || \
                                              ((LSE) == BKPCTRL_REG_63_TMR0_LSE_BYP))
/**
  * @}
  */ 
	
/** @defgroup BKPCTRL_REG_63_TMR1_LSE_configuration
  * @{
  */
#define BKPCTRL_REG_63_TMR1_LSE_OFF          ((uint32_t)0x00000000)
#define BKPCTRL_REG_63_TMR1_LSE_ON           ((uint32_t)0x00000001)
#define BKPCTRL_REG_63_TMR1_LSE_BYP       	 ((uint32_t)0x00000002)

#define IS_BKPCTRL_REG_63_TMR1_LSE(LSE)      (((LSE) == BKPCTRL_REG_63_TMR1_LSE_OFF) || \
                                              ((LSE) == BKPCTRL_REG_63_TMR1_LSE_ON)  || \
                                              ((LSE) == BKPCTRL_REG_63_TMR1_LSE_BYP))
/**
  * @}
  */ 

/** @defgroup BKPCTRL_REG_63_TMR2_LSE_configuration
  * @{
  */
#define BKPCTRL_REG_63_TMR2_LSE_OFF          ((uint32_t)0x00000000)
#define BKPCTRL_REG_63_TMR2_LSE_ON           ((uint32_t)0x00000001)
#define BKPCTRL_REG_63_TMR2_LSE_BYP       	 ((uint32_t)0x00000002)

#define IS_BKPCTRL_REG_63_TMR2_LSE(LSE)      (((LSE) == BKPCTRL_REG_63_TMR2_LSE_OFF) || \
                                              ((LSE) == BKPCTRL_REG_63_TMR2_LSE_ON)  || \
                                              ((LSE) == BKPCTRL_REG_63_TMR2_LSE_BYP))
/**
  * @}
  */ 

/** @defgroup BKPCTRL_REG_63_TMR0_HSI_configuration
  * @{
  */
#define BKPCTRL_REG_63_TMR0_HSI_OFF          ((uint32_t)0x00000000)
#define BKPCTRL_REG_63_TMR0_HSI_ON           ((uint32_t)0x00400000)

#define IS_BKPCTRL_REG_63_TMR0_HSI(HSI)      (((HSI) == BKPCTRL_REG_63_TMR0_HSI_OFF) || \
                                              ((HSI) == BKPCTRL_REG_63_TMR0_HSI_ON)
/**
  * @}
  */ 

/** @defgroup BKPCTRL_REG_63_TMR0_LSI_configuration
  * @{
  */
#define BKPCTRL_REG_63_TMR0_LSI_OFF          ((uint32_t)0x00000000)
#define BKPCTRL_REG_63_TMR0_LSI_ON           ((uint32_t)0x00008000)

#define IS_BKPCTRL_REG_63_TMR0_LSI(LSI)      (((LSI) == BKPCTRL_REG_63_TMR0_LSI_OFF) || \
                                              ((LSI) == BKPCTRL_REG_63_TMR0_LSI_ON)
/**
  * @}
  */ 

/** @defgroup CLKCTRL_PLLn_CLK_SELECT
  * @{
  */
#define PLL0			0	
#define PLL1			1	
#define PLL2			2	
/**
  * @}
  */ 	
	

/** @defgroup CLKCTRL_PLLn_CLK_SELECT
  * @{
  */
#define CLKCTRL_PLLn_CLK_SELECT_HSIdiv1      ((uint32_t)0x00000000)
#define CLKCTRL_PLLn_CLK_SELECT_HSIdiv2      ((uint32_t)0x00000001)
#define CLKCTRL_PLLn_CLK_SELECT_HSE0div1     ((uint32_t)0x00000002)
#define CLKCTRL_PLLn_CLK_SELECT_HSE0div2     ((uint32_t)0x00000003)
#define CLKCTRL_PLLn_CLK_SELECT_HSE1div1     ((uint32_t)0x00000004)
#define CLKCTRL_PLLn_CLK_SELECT_HSE1div2     ((uint32_t)0x00000005)

#define IS_CLKCTRL_PLLn_CLK_SELECT(SRC)      (((SRC) == CLKCTRL_PLLn_CLK_SELECT_HSIdiv1) 	|| \
																							((SRC) == CLKCTRL_PLLn_CLK_SELECT_HSIdiv2)  || \
																							((SRC) == CLKCTRL_PLLn_CLK_SELECT_HSE0div1) || \
																							((SRC) == CLKCTRL_PLLn_CLK_SELECT_HSE0div2) || \
																							((SRC) == CLKCTRL_PLLn_CLK_SELECT_HSE1div1) || \
																							((SRC) == CLKCTRL_PLLn_CLK_SELECT_HSE1div2))
/**
  * @}
  */ 

/** @defgroup CLKCTRL_MAX_CLK_SELECT
  * @{
  */
#define CLKCTRL_MAX_CLK_HSIdiv1       ((uint32_t)0x00000000)
#define CLKCTRL_MAX_CLK_HSIdiv2       ((uint32_t)0x00000001)
#define CLKCTRL_MAX_CLK_HSE0div1      ((uint32_t)0x00000002)
#define CLKCTRL_MAX_CLK_HSE0div2      ((uint32_t)0x00000003)
#define CLKCTRL_MAX_CLK_HSE1div1      ((uint32_t)0x00000004)
#define CLKCTRL_MAX_CLK_HSE1div2      ((uint32_t)0x00000005)
#define CLKCTRL_MAX_CLK_LSI           ((uint32_t)0x00000006)
#define CLKCTRL_MAX_CLK_LSE           ((uint32_t)0x00000007)
#define CLKCTRL_MAX_CLK_PLL0          ((uint32_t)0x00000008)
#define CLKCTRL_MAX_CLK_PLL1          ((uint32_t)0x00000009)
#define CLKCTRL_MAX_CLK_PLL2          ((uint32_t)0x0000000A)

#define IS_CLKCTRL_MAX_CLK(SRC)      (((SRC) == CLKCTRL_MAX_CLK_HSIdiv1) || \
                                      ((SRC) == CLKCTRL_MAX_CLK_HSIdiv2) || \
                                      ((SRC) == CLKCTRL_MAX_CLK_HSE0div1)|| \
                                      ((SRC) == CLKCTRL_MAX_CLK_HSE0div2)|| \
                                      ((SRC) == CLKCTRL_MAX_CLK_HSE1div1)|| \
                                      ((SRC) == CLKCTRL_MAX_CLK_HSE1div2)|| \
                                      ((SRC) == CLKCTRL_MAX_CLK_LSI)     || \
                                      ((SRC) == CLKCTRL_MAX_CLK_LSE)     || \
                                      ((SRC) == CLKCTRL_MAX_CLK_PLL0)    || \
                                      ((SRC) == CLKCTRL_MAX_CLK_PLL1)    || \
                                      ((SRC) == CLKCTRL_MAX_CLK_PLL2))
/**
  * @}
  */ 
	
/** @defgroup CLKCTRL_CPU_CLK_DIV
  * @{
  */
#define CLKCTRL_CPU_CLK_CPUclkDIV1                    ((uint32_t)0x00000000)
#define CLKCTRL_CPU_CLK_CPUclkDIV2                    ((uint32_t)0x00000001)
#define CLKCTRL_CPU_CLK_CPUclkDIV4                    ((uint32_t)0x00000003)
#define CLKCTRL_CPU_CLK_CPUclkDIV8                    ((uint32_t)0x00000007)
#define CLKCTRL_CPU_CLK_CPUclkDIV16                   ((uint32_t)0x0000000F)
#define CLKCTRL_CPU_CLK_CPUclkDIV32                   ((uint32_t)0x0000001F)
#define CLKCTRL_CPU_CLK_CPUclkDIV64                   ((uint32_t)0x0000003F)
#define CLKCTRL_CPU_CLK_CPUclkDIV128                  ((uint32_t)0x0000007E)
#define CLKCTRL_CPU_CLK_CPUclkDIV256                  ((uint32_t)0x000000FF)

#define IS_CLKCTRL_CPU_CLK_CPUclkDIV(DIV)     (((DIV) == CLKCTRL_CPU_CLK_CPUclkDIV1)   || \
                                               ((DIV) == CLKCTRL_CPU_CLK_CPUclkDIV2)   || \
                                               ((DIV) == CLKCTRL_CPU_CLK_CPUclkDIV4)   || \
                                               ((DIV) == CLKCTRL_CPU_CLK_CPUclkDIV8)   || \
                                               ((DIV) == CLKCTRL_CPU_CLK_CPUclkDIV16)  || \
                                               ((DIV) == CLKCTRL_CPU_CLK_CPUclkDIV32)  || \
                                               ((DIV) == CLKCTRL_CPU_CLK_CPUclkDIV64)  || \
                                               ((DIV) == CLKCTRL_CPU_CLK_CPUclkDIV128) || \
                                               ((DIV) == CLKCTRL_CPU_CLK_CPUclkDIV256))
/**
  * @}
  */ 
	
/** @defgroup CLKCTRL_PLL_Multipliers Q, N and Q+DV
  * @{
  */
#define PLL_DIVQ_Q_1					((uint32_t)0x00000000)
#define PLL_DIVQ_Q_2					((uint32_t)0x00000001)
#define PLL_DIVQ_Q_3					((uint32_t)0x00000002)
#define PLL_DIVQ_Q_4					((uint32_t)0x00000003)
#define PLL_DIVQ_Q_5					((uint32_t)0x00000004)
#define PLL_DIVQ_Q_6					((uint32_t)0x00000005)
#define PLL_DIVQ_Q_7					((uint32_t)0x00000006)
#define PLL_DIVQ_Q_8					((uint32_t)0x00000007)
#define PLL_DIVQ_Q_9					((uint32_t)0x00000008)
#define PLL_DIVQ_Q_10					((uint32_t)0x00000009)
#define PLL_DIVQ_Q_11					((uint32_t)0x0000000A)
#define PLL_DIVQ_Q_12					((uint32_t)0x0000000B)
#define PLL_DIVQ_Q_13					((uint32_t)0x0000000C)
#define PLL_DIVQ_Q_14					((uint32_t)0x0000000D)
#define PLL_DIVQ_Q_15					((uint32_t)0x0000000E)
#define PLL_DIVQ_Q_16					((uint32_t)0x0000000F)


#define PLL_DIVQ_Q_1_DV						((uint32_t)0x00000010)
#define PLL_DIVQ_Q_2_DV						((uint32_t)0x00000011)
#define PLL_DIVQ_Q_3_DV						((uint32_t)0x00000012)
#define PLL_DIVQ_Q_4_DV						((uint32_t)0x00000013)
#define PLL_DIVQ_Q_5_DV						((uint32_t)0x00000014)
#define PLL_DIVQ_Q_6_DV						((uint32_t)0x00000015)
#define PLL_DIVQ_Q_7_DV						((uint32_t)0x00000016)
#define PLL_DIVQ_Q_8_DV						((uint32_t)0x00000017)
#define PLL_DIVQ_Q_9_DV						((uint32_t)0x00000018)
#define PLL_DIVQ_Q_10_DV					((uint32_t)0x00000019)
#define PLL_DIVQ_Q_11_DV					((uint32_t)0x0000001A)
#define PLL_DIVQ_Q_12_DV					((uint32_t)0x0000001B)
#define PLL_DIVQ_Q_13_DV					((uint32_t)0x0000001C)
#define PLL_DIVQ_Q_14_DV					((uint32_t)0x0000001D)
#define PLL_DIVQ_Q_15_DV					((uint32_t)0x0000001E)
#define PLL_DIVQ_Q_16_DV					((uint32_t)0x0000001F)



/**
  * @}
  */

/**
  * @defgroup CLKCTRL_BKPCTRL_FLAG
  * @{
  */	
#define BKPCTRL_REG_63_TMR0_LSE_RDY            ((uint32_t)0x080D)//tmp_I=1	
#define BKPCTRL_REG_63_TMR1_LSE_RDY            ((uint32_t)0x100D)//tmp_I=2	
#define BKPCTRL_REG_63_TMR2_LSE_RDY            ((uint32_t)0x180D)//tmp_I=3	

#define BKPCTRL_REG_63_TMR0_LSI_RDY            ((uint32_t)0x0815)//tmp_I=1	
#define BKPCTRL_REG_63_TMR1_LSI_RDY            ((uint32_t)0x1015)//tmp_I=2	
#define BKPCTRL_REG_63_TMR2_LSI_RDY            ((uint32_t)0x1815)//tmp_I=3	

#define BKPCTRL_REG_63_TMR0_HSI_RDY            ((uint32_t)0x0817)//tmp_I=1	
#define BKPCTRL_REG_63_TMR1_HSI_RDY            ((uint32_t)0x1017)//tmp_I=2	
#define BKPCTRL_REG_63_TMR2_HSI_RDY            ((uint32_t)0x1817)//tmp_I=3	

#define CLKCTRL_HSEn_STAT_HSE0_RDY           	 ((uint32_t)0x54)	 //tmp_E=2	
#define CLKCTRL_HSEn_STAT_HSE1_RDY         		 ((uint32_t)0x74)	 //tmp_E=3	

#define CLKCTRL_PLLn_STAT_PLL0_RDY         		 ((uint32_t)0x094) //tmp_P=1
#define CLKCTRL_PLLn_STAT_PLL1_RDY         		 ((uint32_t)0x114) //tmp_P=2
#define CLKCTRL_PLLn_STAT_PLL2_RDY         		 ((uint32_t)0x194) //tmp_P=3
#define CLKCTRL_PLLn_STAT_PLL3_RDY         		 ((uint32_t)0x214) //tmp_P=4
#define CLKCTRL_PLLn_STAT_PLL4_RDY         		 ((uint32_t)0x294) //tmp_P=5
#define CLKCTRL_PLLn_STAT_PLL5_RDY         		 ((uint32_t)0x314) //tmp_P=6
#define CLKCTRL_PLLn_STAT_PLL6_RDY         		 ((uint32_t)0x394) //tmp_P=7
#define CLKCTRL_PLLn_STAT_PLL7_RDY         		 ((uint32_t)0x414) //tmp_P=8

#define IS_CLKCTRL_BKPCTRL_FLAG(FLAG)         (((FLAG) == BKPCTRL_REG_63_TMR0_LSE_RDY)    || \
                                               ((FLAG) == BKPCTRL_REG_63_TMR0_LSI_RDY)    || \
                                               ((FLAG) == BKPCTRL_REG_63_TMR0_HSI_RDY)    || \
                                               ((FLAG) == BKPCTRL_REG_63_TMR1_LSE_RDY)    || \
                                               ((FLAG) == BKPCTRL_REG_63_TMR1_LSI_RDY)    || \
                                               ((FLAG) == BKPCTRL_REG_63_TMR1_HSI_RDY)    || \
                                               ((FLAG) == BKPCTRL_REG_63_TMR2_LSE_RDY)    || \
                                               ((FLAG) == BKPCTRL_REG_63_TMR2_LSI_RDY)    || \
																							 ((FLAG) == BKPCTRL_REG_63_TMR2_HSI_RDY)    || \
																							 ((FLAG) == CLKCTRL_HSEn_STAT_HSE0_RDY)	  	|| \
                                               ((FLAG) == CLKCTRL_HSEn_STAT_HSE1_RDY)  		|| \
                                               ((FLAG) == CLKCTRL_PLLn_STAT_PLL0_RDY)  		|| \
                                               ((FLAG) == CLKCTRL_PLLn_STAT_PLL1_RDY)  		|| \
                                               ((FLAG) == CLKCTRL_PLLn_STAT_PLL2_RDY)  		|| \
                                               ((FLAG) == CLKCTRL_PLLn_STAT_PLL3_RDY)  		|| \
                                               ((FLAG) == CLKCTRL_PLLn_STAT_PLL4_RDY)  		|| \
                                               ((FLAG) == CLKCTRL_PLLn_STAT_PLL5_RDY)  		|| \
                                               ((FLAG) == CLKCTRL_PLLn_STAT_PLL6_RDY)  		|| \
                                               ((FLAG) == CLKCTRL_PLLn_STAT_PLL7_RDY))																						 
/**
  * @}
  */ 

#define IS_BKPCTRL_REG_63_HSI_TRIM_VALUE(TRIM)       ((TRIM) <= 0x3F)
#define IS_BKPCTRL_REG_63_LSI_TRIM_VALUE(TRIM)       ((TRIM) <= 0x1F)

/**
  * @defgroup CLKCTRL_PER0_CLK
  * @{
  */
#define	CLKCTRL_PER0_CLK_RESERVED00						((uint32_t)0x00000001)//RESERVED00
#define	CLKCTRL_PER0_CLK_RESERVED01						((uint32_t)0x00000002)//RESERVED01
#define	CLKCTRL_PER0_CLK_RESERVED02						((uint32_t)0x00000004)//RESERVED02
#define	CLKCTRL_PER0_CLK_RESERVED03						((uint32_t)0x00000008)//RESERVED03
#define	CLKCTRL_PER0_CLK_RESERVED04						((uint32_t)0x000000010)//RESERVED04
#define	CLKCTRL_PER0_CLK_RESERVED05						((uint32_t)0x000000020)//RESERVED05
#define	CLKCTRL_PER0_CLK_RESERVED06						((uint32_t)0x000000040)//RESERVED06
#define	CLKCTRL_PER0_CLK_RESERVED07						((uint32_t)0x000000080)//RESERVED07
#define	CLKCTRL_PER0_CLK_RESERVED08						((uint32_t)0x000000100)//RESERVED08
#define	CLKCTRL_PER0_CLK_RESERVED09						((uint32_t)0x00000200)//RESERVED09
#define	CLKCTRL_PER0_CLK_RESERVED10						((uint32_t)0x00000400)//RESERVED10
#define	CLKCTRL_PER0_CLK_RESERVED11						((uint32_t)0x00000800)//RESERVED11
#define	CLKCTRL_PER0_CLK_RESERVED12						((uint32_t)0x00001000)//RESERVED12
#define CLKCTRL_PER0_CLK_MDR_PORTA_EN					 ((uint32_t)0x00002000)
#define CLKCTRL_PER0_CLK_MDR_PORTB_EN					 ((uint32_t)0x00004000)
#define CLKCTRL_PER0_CLK_MDR_PORTC_EN					 ((uint32_t)0x00008000)
#define CLKCTRL_PER0_CLK_MDR_PORTD_EN					 ((uint32_t)0x00010000)
#define CLKCTRL_PER0_CLK_MDR_PORTE_EN					 ((uint32_t)0x00020000)
#define CLKCTRL_PER0_CLK_RESERVED18					   ((uint32_t)0x00040000)//RESERVED18
#define CLKCTRL_PER0_CLK_RESERVED19 					 ((uint32_t)0x00080000)//RESERVED19
#define CLKCTRL_PER0_CLK_RESERVED20					   ((uint32_t)0x00100000)//RESERVED20
#define CLKCTRL_PER0_CLK_MDR_SPW0_EN					 ((uint32_t)0x00200000)
#define CLKCTRL_PER0_CLK_RESERVED22					   ((uint32_t)0x00400000)//RESERVED22
#define CLKCTRL_PER0_CLK_MDR_TMR0_EN					 ((uint32_t)0x00800000)
#define CLKCTRL_PER0_CLK_MDR_TMR1_EN					 ((uint32_t)0x01000000)
#define CLKCTRL_PER0_CLK_MDR_TMR2_EN					 ((uint32_t)0x02000000)
#define CLKCTRL_PER0_CLK_MDR_TMR3_EN					 ((uint32_t)0x04000000)
#define CLKCTRL_PER0_CLK_RESERVED27					    ((uint32_t)0x08000000)//RESERVED27
#define CLKCTRL_PER0_CLK_RESERVED28				        ((uint32_t)0x10000000)//RESERVED28
#define CLKCTRL_PER0_CLK_MDR_CAN0_EN					 ((uint32_t)0x20000000)
#define CLKCTRL_PER0_CLK_RESERVED30					 	 ((uint32_t)0x40000000)//RESERVED30
#define CLKCTRL_PER0_CLK_RESERVED31					 	 ((uint32_t)0x80000000)//RESERVED31

#define IS_CLKCTRL_PER0_CLK(PER0_CLK)                                                   	((((PER0_CLK) & CLKCTRL_PER0_CLK_RESERVED00) == 0x00) && \
                                                                                            (((PER0_CLK) & CLKCTRL_PER0_CLK_RESERVED01) == 0x00) && \
                                                                                            (((PER0_CLK) & CLKCTRL_PER0_CLK_RESERVED02) == 0x00) && \
                                                                                            (((PER0_CLK) & CLKCTRL_PER0_CLK_RESERVED03) == 0x00) && \
                                                                                            (((PER0_CLK) & CLKCTRL_PER0_CLK_RESERVED04) == 0x00) && \
                                                                                            (((PER0_CLK) & CLKCTRL_PER0_CLK_RESERVED05) == 0x00) && \
                                                                                            (((PER0_CLK) & CLKCTRL_PER0_CLK_RESERVED06) == 0x00) && \
                                                                                            (((PER0_CLK) & CLKCTRL_PER0_CLK_RESERVED07) == 0x00) && \
                                                                                            (((PER0_CLK) & CLKCTRL_PER0_CLK_RESERVED08) == 0x00) && \
                                                                                            (((PER0_CLK) & CLKCTRL_PER0_CLK_RESERVED09) == 0x00) && \
                                                                                            (((PER0_CLK) & CLKCTRL_PER0_CLK_RESERVED10) == 0x00) && \
                                                                                            (((PER0_CLK) & CLKCTRL_PER0_CLK_RESERVED11) == 0x00) && \
                                                                                            (((PER0_CLK) & CLKCTRL_PER0_CLK_RESERVED12) == 0x00) && \
																							(((PER0_CLK) & CLKCTRL_PER0_CLK_RESERVED19) == 0x00) && \
                                                                                            (((PER0_CLK) & CLKCTRL_PER0_CLK_RESERVED20) == 0x00) && \
																							(((PER0_CLK) & CLKCTRL_PER0_CLK_RESERVED22) == 0x00) && \
																							(((PER0_CLK) & CLKCTRL_PER0_CLK_RESERVED27) == 0x00) && \
																							(((PER0_CLK) & CLKCTRL_PER0_CLK_RESERVED28) == 0x00) && \
																							(((PER0_CLK) & CLKCTRL_PER0_CLK_RESERVED30) == 0x00) && \
																							(((PER0_CLK) & CLKCTRL_PER0_CLK_RESERVED31) == 0x00))
/**
  * @}
  */ 	

/**
  * @defgroup CLKCTRL_PER1_CLK
  * @{
  */
#define	CLKCTRL_PER1_CLK_RESERVED00							((uint32_t)0x00000001)//RESERVED00
#define	CLKCTRL_PER1_CLK_RESERVED01							((uint32_t)0x00000002)//RESERVED01
#define	CLKCTRL_PER1_CLK_MDR_SSP0_EN						((uint32_t)0x00000004)
#define	CLKCTRL_PER1_CLK_RESERVED03							((uint32_t)0x00000008)//RESERVED03
#define	CLKCTRL_PER1_CLK_RESERVED04							((uint32_t)0x00000010)//RESERVED04
#define	CLKCTRL_PER1_CLK_RESERVED05							((uint32_t)0x00000020)//RESERVED05
#define CLKCTRL_PER1_CLK_MDR_UART0_EN						((uint32_t)0x00000040)
#define	CLKCTRL_PER1_CLK_MDR_UART1_EN						((uint32_t)0x00000080)
#define	CLKCTRL_PER1_CLK_RESERVED08							((uint32_t)0x00000100)//RESERVED08
#define	CLKCTRL_PER1_CLK_RESERVED09							((uint32_t)0x00000200)//RESERVED09
#define	CLKCTRL_PER1_CLK_RESERVED10							((uint32_t)0x00000400)//RESERVED10
#define	CLKCTRL_PER1_CLK_MDR_ARCRX0_EN					    ((uint32_t)0x00000800)
#define	CLKCTRL_PER1_CLK_RESERVED12							((uint32_t)0x00001000)//RESERVED12
#define	CLKCTRL_PER1_CLK_MDR_ARCTX0_EN				        ((uint32_t)0x00002000)
#define	CLKCTRL_PER1_CLK_RESERVED14							((uint32_t)0x00004000)//RESERVED14
#define	CLKCTRL_PER1_CLK_RESERVED15							((uint32_t)0x00008000)//RESERVED15
#define	CLKCTRL_PER1_CLK_RESERVED16							((uint32_t)0x00010000)//RESERVED16
#define	CLKCTRL_PER1_CLK_RESERVED17							((uint32_t)0x00020000)//RESERVED17
#define	CLKCTRL_PER1_CLK_RESERVED18							((uint32_t)0x00030000)//RESERVED18
#define CLKCTRL_PER1_CLK_MDR_MIL0_EN						((uint32_t)0x00080000)
#define	CLKCTRL_PER1_CLK_RESERVED20							((uint32_t)0x00100000)//RESERVED20
#define CLKCTRL_PER1_CLK_MDR_MIL1_EN						((uint32_t)0x00200000)
#define	CLKCTRL_PER1_CLK_RESERVED22							((uint32_t)0x00400000)//RESERVED22
#define CLKCTRL_PER1_CLK_MDR_ADC0_EN						((uint32_t)0x00800000)
#define CLKCTRL_PER1_CLK_MDR_ADC1_EN					    ((uint32_t)0x01000000)
#define CLKCTRL_PER1_CLK_MDR_DAC0_EN						((uint32_t)0x02000000)
#define CLKCTRL_PER1_CLK_MDR_DAC1_EN			            ((uint32_t)0x04000000)
#define	CLKCTRL_PER1_CLK_RESERVED27							((uint32_t)0x08000000)//RESERVED27
#define	CLKCTRL_PER1_CLK_RESERVED28							((uint32_t)0x10000000)//RESERVED28
#define	CLKCTRL_PER1_CLK_RESERVED29							((uint32_t)0x20000000)//RESERVED29
#define	CLKCTRL_PER1_CLK_RESERVED30							((uint32_t)0x40000000)//RESERVED30
#define	CLKCTRL_PER1_CLK_RESERVED31							((uint32_t)0x80000000)//RESERVED31

#define IS_CLKCTRL_PER1_CLK(PER1_CLK)        ((((PER1_CLK) & CLKCTRL_PER1_CLK_RESERVED00) == 0x00) && \
											 (((PER1_CLK) & CLKCTRL_PER1_CLK_RESERVED01) == 0x00) && \
											 (((PER1_CLK) & CLKCTRL_PER1_CLK_RESERVED03) == 0x00) && \
											 (((PER1_CLK) & CLKCTRL_PER1_CLK_RESERVED04) == 0x00) && \
											 (((PER1_CLK) & CLKCTRL_PER1_CLK_RESERVED05) == 0x00) && \
											 (((PER1_CLK) & CLKCTRL_PER1_CLK_RESERVED08) == 0x00) && \
											 (((PER1_CLK) & CLKCTRL_PER1_CLK_RESERVED09) == 0x00) && \
											 (((PER1_CLK) & CLKCTRL_PER1_CLK_RESERVED10) == 0x00) && \
									         (((PER1_CLK) & CLKCTRL_PER1_CLK_RESERVED14) == 0x00) && \
											 (((PER1_CLK) & CLKCTRL_PER1_CLK_RESERVED15) == 0x00) && \
											 (((PER1_CLK) & CLKCTRL_PER1_CLK_RESERVED16) == 0x00) && \
											 (((PER1_CLK) & CLKCTRL_PER1_CLK_RESERVED17) == 0x00) && \
										     (((PER1_CLK) & CLKCTRL_PER1_CLK_RESERVED18) == 0x00) && \
											 (((PER1_CLK) & CLKCTRL_PER1_CLK_RESERVED20) == 0x00) && \
											 (((PER1_CLK) & CLKCTRL_PER1_CLK_RESERVED22) == 0x00) && \
											 (((PER1_CLK) & CLKCTRL_PER1_CLK_RESERVED27) == 0x00) && \
											 (((PER1_CLK) & CLKCTRL_PER1_CLK_RESERVED28) == 0x00) && \
											 (((PER1_CLK) & CLKCTRL_PER1_CLK_RESERVED29) == 0x00) && \
											 (((PER1_CLK) & CLKCTRL_PER1_CLK_RESERVED30) == 0x00) && \
											 (((PER1_CLK) & CLKCTRL_PER1_CLK_RESERVED31) == 0x00))
/**
  * @}
  */ 
  
/** @defgroup BKPCTRL_SRILOWconfig
  * @{
  */
#define BKPCNTR_SRILOW_upto_10MHz  ((uint32_t)0x00000000)
#define BKPCNTR_SRILOW_upto_30MHz  ((uint32_t)0x00000001)
#define BKPCNTR_SRILOW_upto_50MHz  ((uint32_t)0x00000002)
#define BKPCNTR_SRILOW_upto_100MHz ((uint32_t)0x00000003)
#define BKPCNTR_SRILOW_upto_1MHz   ((uint32_t)0x00000004)
#define BKPCNTR_SRILOW_upto_500kHz ((uint32_t)0x00000005)
#define BKPCNTR_SRILOW_upto_100kHz ((uint32_t)0x00000006)
#define BKPCNTR_SRILOW_over_10kHz  ((uint32_t)0x00000007)  
  
/* Bit field positions: */
#define BKPCTRL_REG_61_TMRx_SRILOW0_Pos  ((uint32_t)  0)
#define BKPCTRL_REG_61_TMRx_SRILOW1_Pos  ((uint32_t)  8)
#define BKPCTRL_REG_61_TMRx_SRILOW2_Pos  ((uint32_t) 16)
#define BKPCTRL_REG_61_TMRx_SRILOW3_Pos  ((uint32_t) 24)

#define BKPCTRL_REG_62_TMRx_SRILOW4_Pos  ((uint32_t)  0)
#define BKPCTRL_REG_62_TMRx_SRILOW5_Pos  ((uint32_t)  8)

/* Bit field masks: */
#define BKPCTRL_REG_61_TMRx_SRILOW0_Msk  ((uint32_t)0x00000007)
#define BKPCTRL_REG_61_TMRx_SRILOW1_Msk  ((uint32_t)0x00000700)
#define BKPCTRL_REG_61_TMRx_SRILOW2_Msk  ((uint32_t)0x00070000)
#define BKPCTRL_REG_61_TMRx_SRILOW3_Msk  ((uint32_t)0x07000000)
#define BKPCTRL_REG_61_TMRx_SRILOWx_Msk  ((uint32_t)0x07070707)

#define BKPCTRL_REG_62_TMRx_SRILOW4_Msk  ((uint32_t)0x00000007)
#define BKPCTRL_REG_62_TMRx_SRILOW5_Msk  ((uint32_t)0x00000700)
#define BKPCTRL_REG_62_TMRx_SRILOWx_Msk  ((uint32_t)0x00000707)

#define IS_BKPCNTR_SRILOW(MODE) (((MODE) == BKPCNTR_SRILOW_upto_10MHz) || \
                                ((MODE) == BKPCNTR_SRILOW_upto_30MHz)  || \
                                ((MODE) == BKPCNTR_SRILOW_upto_50MHz)  || \
                                ((MODE) == BKPCNTR_SRILOW_upto_100MHz) || \
                                ((MODE) == BKPCNTR_SRILOW_upto_1MHz)   || \
                                ((MODE) == BKPCNTR_SRILOW_upto_500kHz) || \
                                ((MODE) == BKPCNTR_SRILOW_upto_100kHz) || \
                                ((MODE) == BKPCNTR_SRILOW_over_10kHz))
/**
  * @}
  */   

/** @defgroup CLKCTRL_Exported_Functions CLKCTRL Exported Functions
  * @{
  */
void CLKCTRL_DeInit(void);

void CLKCTRL_HSEconfig(uint32_t CLKCTRL_HSE);
void BKPCTRL_REG_63_LSEconfig(uint32_t CLKCTRL_BKPCTRL_LSE);

void BKPCNTR_REG_63_TMRx_HSIcmd(FunctionalState NewState);

ErrorStatus CLKCTRL_HSEstatus(uint32_t CLKCTRL_HSE);
ErrorStatus CLKCTRL_CPU_PLLstatus(uint32_t PLLn);
ErrorStatus BKPCNTR_REG_63_TMRx_HSIstatus(void);
ErrorStatus BKPCTRL_REG_63_LSIstatus(void);
ErrorStatus BKPCTRL_REG_63_LSEstatus(void);

FlagStatus CLKCTRL_BKPCTRL_GetFlagStatus(uint32_t CLKCTRL_BKPCTRL_FLAG);

void CLKCTRL_CPU_PLLconfig (uint32_t PLLn, uint32_t CLKCTRL_CPU_PLLsource,
	uint32_t CLKCTRL_PLLn_CLK_PLLn_Q, uint32_t CLKCTRL_PLLn_CLK_PLLn_N);

void CLKCTRL_CPUclkPrescaler(uint32_t CPUclkDivValue);

void CLKCTRL_MAX_CLKSelection(uint32_t MAX_CLK);

void CLKCTRL_PER0_CLKcmd(uint32_t CLKCTRL_PER0_CLK, FunctionalState NewState);
void CLKCTRL_PER1_CLKcmd(uint32_t CLKCTRL_PER1_CLK, FunctionalState NewState);

void BKPCNTR_REG_63_TMRx_HSIcmd(FunctionalState NewState);
void BKPCNTR_REG_63_TMRx_LSIcmd(FunctionalState NewState);

void BKPCNTR_SRILOWconfig(uint32_t SRILOW);


/**
  * @}
  */

/**
  * @}
  */


#endif /* __MDR32F8_CLKCTRL_H */

/******************* (C) COPYRIGHT 2021 Milandr *********************************
*
* END OF FILE mdr32f8_clkctrl.h */
