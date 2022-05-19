/**
  ******************************************************************************
  * @file    TIM/TIM_DMA/Inc/main.h
  * @author  MCD Application Team
  * @brief   Header for main.c module
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2019 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

/* Includes ------------------------------------------------------------------*/
#include "stm32h7xx_hal.h"
#include "stm32h7b3i_discovery.h"


/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Definition of TIM instance */
#define TIMx                             TIM8

/* Definition for TIMx clock resources */
#define TIMx_CLK_ENABLE                  __HAL_RCC_TIM8_CLK_ENABLE
#define DMAx_CLK_ENABLE                  __HAL_RCC_DMA1_CLK_ENABLE

/* Definition for TIMx Pins */
#define TIMx_CHANNEL3_GPIO_CLK_ENABLE    __HAL_RCC_GPIOI_CLK_ENABLE

#define TIMx_GPIO_CHANNEL3_PORT          GPIOI
#define GPIO_PIN_CHANNEL3                GPIO_PIN_7


#define GPIO_AF_TIMx                     GPIO_AF3_TIM8

/* Definition for TIMx's DMA */
#define DMA_CHANNEL_CC3                  DMA_CHANNEL_3
#define TIMx_CC3_DMA_REQUEST             DMA_REQUEST_TIM8_CH3
#define TIMx_CC3_DMA_INST                DMA1_Stream7

/* Definition for ADCx's NVIC */
#define TIMx_DMA_IRQn                    DMA1_Stream7_IRQn
#define TIMx_DMA_IRQHandler              DMA1_Stream7_IRQHandler

/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */


#endif /* __MAIN_H */

