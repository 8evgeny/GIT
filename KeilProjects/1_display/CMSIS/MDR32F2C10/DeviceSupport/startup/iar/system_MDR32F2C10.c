/**
  ******************************************************************************
  * @file    system_MDR32F2C10.c
  * @author  Milandr Application Team
  * @version V1.0.1
  * @date    13/08/2021
  * @brief   CMSIS Cortex-M4F Device Peripheral Access Layer System Source File.
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
  * FILE system_MDR32F2C10.c
  */


/** @addtogroup __CMSIS CMSIS
  * @{
  */

/** @defgroup MDR32F2C10 MDR32F2C10
  *  @{
  */

/** @addtogroup __MDR32F2C10 MDR32F2C10 System
  * @{
  */

/** @addtogroup System_Private_Includes System Private Includes
  * @{
  */

#include "MDR1986VK01.h"

/** @} */ /* End of group System_Private_Includes */

/** @addtogroup __MDR32F2C10_System_Private_Variables MDR32F2C10 System Private Variables
  * @{
  */

/*******************************************************************************
*  Clock Definitions
*******************************************************************************/
uint32_t SystemCoreClock = 8000000UL;         /*!< System Clock Frequency (Core Clock)
                                                         *   default value */

/** @} */ /* End of group __MDR32F2C10_System_Private_Variables */

/** @addtogroup __MDR32F2C10_System_Private_Functions MDR32F2C10 System Private Functions
  * @{
  */

/**
  * @brief  Update SystemCoreClock according to Clock Register Values
  * @note   None
  * @param  None
  * @retval None
  */
void SystemCoreClockUpdate (void)
{
    
}

/**
  * @brief  Setup the microcontroller system
  *         RST clock configuration to the default reset state
  *         Setup SystemCoreClock variable.
  * @note   This function should be used only after reset.
  * @param  None
  * @retval None
  */
void SystemInit (void)
{
//    SCB->VTOR = 0x20000000;
//    MDR_BKP->KEY = 0;

    MDR_ICACHE->KEY = 0x8555AAA1;
    MDR_ICACHE->CNTL = 5;
    MDR_ICACHE->KEY = 0x00000000;

    MDR_DCACHE->KEY = 0x8555AAA1;
    MDR_DCACHE->CNTL = 5;
    MDR_DCACHE->KEY = 0x00000000;

    MDR_FLASH->KEY = 0x8555AAA1;
    MDR_FLASH->CNTR = 4;
    MDR_FLASH->KEY  = 0x00000000;

//    SCB->VTOR = 0x01000000;
//    __NOP();
//    __NOP();
//    __NOP();
//    MDR_BKP->KEY = 0x8555AAA1;
//    MDR_BKP->REG_60_TMR0 |= 1<<9;
//    __NOP();
//    __NOP();
//    __NOP();

    SystemCoreClockUpdate();
}

/** @} */ /* End of group __MDR32F2C10_System_Private_Functions */

/** @} */ /* End of group __MDR32F2C10 */

/** @} */ /* End of group MDR32F2C10 */

/** @} */ /* End of group __CMSIS */

/******************* (C) COPYRIGHT 2021 Milandr ********************************
*
* END OF FILE system_MDR32F2C10.c */
