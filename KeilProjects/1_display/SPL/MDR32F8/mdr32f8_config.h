/**
  ******************************************************************************
  * @file    mdr32f8_config.h
  * @author  Milandr Application Team
  * @version V0.4.1
  * @date    12/10/2020
  * @brief   Library configuration file.
  ******************************************************************************
  * <br><br>
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, MILANDR SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2021 Milandr</center></h2>
  */

/**
 * @mainpage MDR32F8 Standard Peripherals Library.
 * MDR32F8 Standard Peripherals Library is a package consisting of
 * all standard peripheral device drivers for 1986VE8 microcontrollers.
 * This library is a firmware package which contains a collection of routines,
 * data structures and macros covering the features of Milandr MDR32F8
 * peripherals. It includes a description of the device drivers plus a set of
 * examples for each peripheral. The firmware library allows any device to be
 * used in the user application without the need for in-depth study of each
 * peripherals specifications. Using the Standard Peripherals Library has two
 * advantages: it saves significant time that would otherwise be spent in
 * coding, while simultaneously reducing application development and
 * integration costs.
 *
 * The MDR32F8 Standard Peripherals Library is compatible with Milandr
 * 1986VE8 evaluation boards.
 *
 * The MDR32F8 Standard Peripherals Library is full CMSIS compliant.
 */
 
 /* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MDR32F8_CONFIG_H
#define __MDR32F8_CONFIG_H

#include "mdr32f8_lib.h"
#include "stdio.h"
#include "stdint.h"

 #define USE_ASSERT_INFO    0
 //#define USE_ASSERT_INFO    1 
//#define USE_ASSERT_INFO    2 

/* Target system parameters */
/* RST_CLK generators frequencies in HZ */
#define HSI_Value       	((uint32_t)8000000)
#define HSE0_Value       	((uint32_t)10000000)
#define HSE1_Value      	((uint32_t)25000000)
#define LSI_Value       	((uint32_t)40000)
#define LSE_Value       	((uint32_t)32768)
#define FREQ_FAULT_HZ  0
/* DMA configuration parameters */
/* Number of DMA channels to use */
#define DMA_Channels_Number   32          /* This parameter is in range 1..32 */

/* Alternate Control Data Structure Usage */
/* This parameter can be a value of:
    0 = DMA_ALternateDataDisabled;
    1 = DMA_ALternateDataEnabled; */
/*#define DMA_AlternateData   0 */          /* DMA_AlternateDataDisabled */
#define DMA_AlternateData   1             /* DMA_AlternateDataEnabled */


#if (USE_ASSERT_INFO == 0)
  #define assert_param(expr) ((void)0)
#elif (USE_ASSERT_INFO == 1)
  #define assert_param(expr) ((expr) ? (void)0 : assert_failed(ASSERT_INFO_FILE_ID, __LINE__))
  void assert_failed(uint32_t file_id, uint32_t line);
#elif (USE_ASSERT_INFO == 2)
  #define assert_param(expr) ((expr) ? (void)0 : assert_failed(ASSERT_INFO_FILE_ID, __LINE__, #expr))
  void assert_failed(uint32_t file_id, uint32_t line, const uint8_t* expr);
#else
  #error "Unsupported USE_ASSERT_INFO level"
#endif /* USE_ASSERT_INFO */

#endif /* __MDR32F8_CONFIG_H */

/******************* (C) COPYRIGHT 2021 Milandr *********************************
*
* END OF FILE mdr32f8_config.h */
