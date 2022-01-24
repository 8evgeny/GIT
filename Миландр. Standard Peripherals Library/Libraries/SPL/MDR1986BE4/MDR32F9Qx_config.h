/**
  ******************************************************************************
  * @file    MDR32F9Qx_config.h
  * @author  Milandr Application Team
  * @version V2.0.2
  * @date    23/09/2021
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
  */

/**
  * @mainpage MDR1986BE4 Standard Peripherals Library.
  * MDR1986BE4 Standard Peripherals Library is a package consisting of
  * all standard peripheral device drivers for 1986BE4 microcontrollers.
  * This library is a firmware package which contains a collection of routines,
  * data structures and macros covering the features of Milandr 1986BE4
  * peripherals. It includes a description of the device drivers plus a set of
  * examples for each peripheral. The firmware library allows any device to be
  * used in the user application without the need for in-depth study of each
  * peripherals specifications. Using the Standard Peripherals Library has two
  * advantages: it saves significant time that would otherwise be spent in
  * coding, while simultaneously reducing application development and
  * integration costs.
  *
  * The MDR1986BE4 Standard Peripherals Library is compatible with Milandr
  * 1986BE4x evaluation boards.
  *
  * The MDR1986BE4 Standard Peripherals Library is full CMSIS compliant.
  */

// <<< Use Configuration Wizard in Context Menu >>>


/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MDR32F9Qx_CONFIG_H
#define __MDR32F9Qx_CONFIG_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
#include "MDR1986BE4.h"

// <c3> SWD pins protection
// <i> Prevents RTXT and OE bits setting for SWD pins when doing GPIO read-modify-write.
// <i> Uncomment the definition below to define used SWD port.
// <i> Leave all commented/unchecked if there is no GPIO pins combined with SWD.
#define USE_SWD
// </c>

// <o> Legacy support definition
// <i> Specifies level of legacy support. If legacy support is not needed, outcomment this definition or change default value.
// <i> Support for the legacy version is removed after 2 updates.
// <i> Default value for version 2.0.x: 153 - to support previous version (1.5.3)
#define MDR_LEGACY_SUPPORT 153

// <h> Target system parameters
// <h> RST_CLK generators frequencies
// <o> HSI clock value [Hz]
// <i> Default: 8000000 (8MHz)
#define HSI_Value       ((uint32_t)8000000)
// <o> HSE clock value [Hz]
// <i> Default: 8000000 (8MHz)
#define HSE_Value       ((uint32_t)8000000)
// <o> LSI clock value [Hz]
// <i> Default: 40000 (40kHz)
#define LSI_Value       ((uint32_t)40000)
// <o> LSE clock value [Hz]
// <i> Default: 32768 (32.768kHz)
#define LSE_Value       ((uint32_t)32768)
// </h>

// <h> RST_CLK frequencies startup timeouts settings
// <o> HSE timeout startup value
// <i> Default: 0x0600
#define HSEonTimeOut    ((uint16_t)0x0600)
// <o> LSE timeout startup value
// <i> Default: 0x0600
#define LSEonTimeOut    ((uint16_t)0x0600)
// <o> HSI timeout startup value
// <i> Default: 0x0600
#define HSIonTimeOut    ((uint16_t)0x0600)
// <o> LSI timeout startup value
// <i> Default: 0x0600
#define LSIonTimeOut    ((uint16_t)0x0600)
// <o> PLLCPU timeout startup value
// <i> Default: 0x0600
#define PLLCPUonTimeOut ((uint16_t)0x0600)
// </h>

// <o> EEPROM controller freq [MHz]
 //<i> Default: 8MHz
#define FLASH_PROG_FREQ_MHZ     (8.0)
// </h>


// <h> RTC configuration parameters
// <o> RTC calibration value
// <i> Default: 0
#define RTC_CalibratorValue       0
// <o> RTC prescaler value
// <i> Default: 32768
#define RTC_PRESCALER_VALUE       32768
// </h>

// <h> Controller blocks settings
// <h> DMA configuration parameters
// <o.0..5> Number of DMA channels to use
// <i> This parameter is in range 1..32
// <i> Default: 32
#define DMA_Channels_Number   32          /* This parameter is in range 1..32 */

// <o> Alternate Control Data Structure Usage
//   <0=> 0: DMA_ALternateDataDisabled
//   <1=> 1: DMA_ALternateDataEnabled
// <i> Default: 1 (DMA_ALternateDataEnabled)
#define DMA_AlternateData     1             /* DMA_AlternateDataEnabled */
// </h>

// </h>

#if defined (__ICCARM__)
    #define __attribute__(name_section)
#endif
#if defined (__CMCARM__)
    #define __attribute__(name_section)
    #define IAR_SECTION(section)
#endif
#if defined (__ARMCC_VERSION)
    #define IAR_SECTION(section)
#endif

// <h> Parameter run-time check support

// <o> Paramater checking level
//   <0=> 0: no parameter checks ("assert_param" macro is disabled)
//   <1=> 1: check enabled, source file ID and line number are available
//   <2=> 2: check enabled, source file ID, line number and checking expression (as string) are available (increased code size)
// <i> Default: 0 ("assert_param" macro is disabled)
#define USE_ASSERT_INFO    0

/**
  * @brief  The assert_param macro is used for function's parameters check.
  * @param  expr: If expr is false, it calls assert_failed user's function which 
  *         reports the name of the source file, source line number
  *         and expression text (if USE_ASSERT_INFO == 2) of the call that failed.
  *         That function should not return. If expr is true, nothing is done.
  * @retval None
  */
#if (USE_ASSERT_INFO == 0)
    #define assert_param(expr) ((void)0)
#elif (USE_ASSERT_INFO == 1)
    #define assert_param(expr) ((expr) ? (void)0 : assert_failed((uint8_t *)__FILE__, __LINE__))
    void assert_failed(uint8_t* file, uint32_t line);
#elif (USE_ASSERT_INFO == 2)
    #define assert_param(expr) ((expr) ? (void)0 : assert_failed((uint8_t *)__FILE__, __LINE__, (const uint8_t*) #expr))
    void assert_failed(uint8_t* file, uint32_t line, const uint8_t* expr);
#else
    #error "Unsupported USE_ASSERT_INFO level"
#endif /* USE_ASSERT_INFO */

// </h>

#ifdef __cplusplus
} // extern "C" block end
#endif

#endif /* __MDR32F9Qx_CONFIG_H */

/*********************** (C) COPYRIGHT 2021 Milandr ****************************
*
* END OF FILE MDR32F9Qx_config.h */
