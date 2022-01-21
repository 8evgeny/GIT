/**
  ******************************************************************************
  * @file    system_MDR32F2C10.h
  * @author  Milandr Application Team
  * @version V1.0.1
  * @date    11/11/2020
  * @brief   CMSIS Cortex-M4F Device Peripheral Access Layer System Header File.
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
  * FILE system_MDR32F2C10.h
  */

/** @addtogroup __CMSIS CMSIS
  * @{
  */

/** @defgroup MDR32F2C10 MDR32F2C10
 *  @{
 */

/** @defgroup __MDR32F2C10 MDR32F2C10 System
  * @{
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __SYSTEM_MDR32F2C10_H
#define __SYSTEM_MDR32F2C10_H

/* Standard types inclusion --------------------------------------------------*/
#include <stdint.h>

extern uint32_t SystemCoreClock;          /*!< System Clock Frequency (Core Clock) */


/**
 * Initialize the system
 *
 * @param  none
 * @return none
 *
 * @brief  Setup the microcontroller system
 *         Initialise GPIO directions and values
 */
extern void SystemInit(void);
extern void SystemCoreClockUpdate(void);
#endif /* __SYSTEM_MDR32F2C10_H */

/** @} */ /* End of group __MDR32F2C10 */

/** @} */ /* End of group MDR32F2C10 */

/** @} */ /* End of group __CMSIS */

/******************* (C) COPYRIGHT 2021 Milandr ********************************
*
* END OF FILE system_MDR32F2C10.h */

