/**
  ******************************************************************************
  * @file    MDR32F2C10_delay.h
  * @author  Milandr Application Team
  * @version V1.0.1
  * @date    28/01/2021
  * @brief   Delay functions Header file.
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
  * FILE MDR32F2C10_delay.h
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MDR32F2C10_DELAY_H
#define __MDR32F2C10_DELAY_H

/* Includes ------------------------------------------------------------------*/
#include "MDR32F2C10_config.h"

/** @addtogroup __MDR32F2C10_StdPeriph_Driver MDR32F2C10 Standard Peripherial Driver
  * @{
  */

/** @addtogroup Delay Delay
  * @{
  */

/** @defgroup Delay_Exported_Functions Delay Exported Functions
  * @{
  */

void DELAY_Config( void );
RAMFUNC void DELAY_us( uint32_t ulUs );
RAMFUNC void DELAY_ms( uint32_t ulMs );


/** @} */ /* End of group Delay_Exported_Functions */

/** @} */ /* End of group Delay */

/** @} */ /* End of group MDR32F2C10_StdPeriph_Driver */

#endif /* __MDR32F2C10_DELAY_H */

/******************* (C) COPYRIGHT 2021 Milandr *********
*
* END OF FILE MDR32F2C10_delay.h */

