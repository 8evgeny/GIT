/**
  ******************************************************************************
  * @file    MDR1986BE4_IT.h
  * @author  Milandr Application Team
  * @version V2.0.2
  * @date    22/09/2021
  * @brief   This file contains all the functions prototypes
  *          for the interrupt handlers for MPU MDR1986BE4.
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
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MDR1986BE4_IT_H
#define __MDR1986BE4_IT_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "MDR32F9Qx_config.h"

/* Function prototypes -------------------------------------------------------*/
void HardFault_Handler(void);
void SVC_Handler(void);
void PendSV_Handler(void);
void SysTick_Handler(void);

void DMA_IRQHandler(void);
void UART1_IRQHandler(void);
void UART2_IRQHandler(void);
void SSP1_IRQHandler(void);
void POWER_IRQHandler(void);
void WWDG_IRQHandler(void);
void TIMER1_IRQHandler(void);
void TIMER2_IRQHandler(void);
void ADC_IRQHandler(void);
void COMPARATOR_IRQHandler(void);
void BACKUP_IRQHandler(void);
void EXT_INT0_IRQHandler(void);
void EXT_INT1_IRQHandler(void);
void EXT_INT2_IRQHandler(void);
void ADCIU_CH1_IRQHandler(void);
void ADCIU_CH2_IRQHandler(void);
void ADCIU_CH3_IRQHandler(void);
void ADCIU_CH4_IRQHandler(void);
void ADCIU_CH5_IRQHandler(void);
void ADCIU_CH6_IRQHandler(void);
void ADCIU_CH7_IRQHandler(void);
void ADCIU_CH8_IRQHandler(void);
void ADCIU_IRQHandler(void);

#ifdef __cplusplus
} // extern "C" block end
#endif

#endif /* __MDR1986BE4_IT_H */

/*********************** (C) COPYRIGHT 2021 Milandr ****************************
*
* END OF FILE MDR1986BE4_IT.h */
