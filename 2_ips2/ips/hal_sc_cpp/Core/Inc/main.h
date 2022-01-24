/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.h
 * @brief          : Header for main.c file.
 *                   This file contains the common defines of the application.
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
 * All rights reserved.</center></h2>
 *
 * This software component is licensed by ST under Ultimate Liberty license
 * SLA0044, the "License"; You may not use this file except in compliance with
 * the License. You may obtain a copy of the License at:
 *                             www.st.com/SLA0044
 *
 ******************************************************************************
 */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C"
{
#endif

  /* Device type selector-------------------------------------------------------*/

//#define SC2DIOBOARD
//#define SC4BOPLA
  /* Includes ------------------------------------------------------------------*/
#include "stm32h7xx_hal.h"

  /* Private includes ----------------------------------------------------------*/
  /*
   #ifdef SC2DIOBOARD

   #endif

   #ifdef SC4BOPLA

   #endif
   */
  /* Exported types ------------------------------------------------------------*/

  /* Exported constants --------------------------------------------------------*/

  /* Exported macro ------------------------------------------------------------*/

#define FASTMEM 	__attribute__ ((section (".fastdata")))
#define FASTCODE 	__attribute__ ((section (".fastest_i")))
#define EXTMEM	 	__attribute__ ((section (".extsram")))

  /* Exported functions prototypes ---------------------------------------------*/
  void Error_Handler (void);

/* Private defines -----------------------------------------------------------*/
#ifdef SC2DIOBOARD

#define DO1_Pin GPIO_PIN_7
#define DO1_GPIO_Port GPIOG
#define DO2_Pin GPIO_PIN_8
#define DO2_GPIO_Port GPIOG
#define DI4_Pin GPIO_PIN_6
#define DI4_GPIO_Port GPIOC
#define DI3_Pin GPIO_PIN_7
#define DI3_GPIO_Port GPIOC
#define DI2_Pin GPIO_PIN_8
#define DI2_GPIO_Port GPIOC
#define DI1_Pin GPIO_PIN_9
#define DI1_GPIO_Port GPIOC
#define PHY_MASTER_CLOCK_Pin GPIO_PIN_8
#define PHY_MASTER_CLOCK_GPIO_Port GPIOA
#define DI8_Pin GPIO_PIN_9
#define DI8_GPIO_Port GPIOA
#define DI7_Pin GPIO_PIN_10
#define DI7_GPIO_Port GPIOA
#define DI6_Pin GPIO_PIN_11
#define DI6_GPIO_Port GPIOA
#define DI5_Pin GPIO_PIN_12
#define DI5_GPIO_Port GPIOA
#define LED_NORM_Pin GPIO_PIN_6
#define LED_NORM_GPIO_Port GPIOD
#define DO3_Pin GPIO_PIN_9
#define DO3_GPIO_Port GPIOG
#define DO4_Pin GPIO_PIN_10
#define DO4_GPIO_Port GPIOG
#define DO5_Pin GPIO_PIN_11
#define DO5_GPIO_Port GPIOG
#define DO6_Pin GPIO_PIN_12
#define DO6_GPIO_Port GPIOG
#define DO7_Pin GPIO_PIN_13
#define DO7_GPIO_Port GPIOG
#define DO8_Pin GPIO_PIN_14
#define DO8_GPIO_Port GPIOG
#define TEST_LED_Pin GPIO_PIN_9
#define TEST_LED_GPIO_Port GPIOB

#endif

#ifdef SC4BOPLA
#define AMP_ON_Pin GPIO_PIN_3
#define AMP_ON_GPIO_Port GPIOA
#define LED_MIC_Pin GPIO_PIN_5
#define LED_MIC_GPIO_Port GPIOA
#define PHY_MASTER_CLOCK_Pin GPIO_PIN_8
#define PHY_MASTER_CLOCK_GPIO_Port GPIOA
#define KB_INT_Pin GPIO_PIN_12
#define KB_INT_GPIO_Port GPIOC
#define KB_INT_EXTI_IRQn EXTI15_10_IRQn
#define LED_NORM_Pin GPIO_PIN_6
#define LED_NORM_GPIO_Port GPIOD
#define TEST_LED_Pin GPIO_PIN_9
#define TEST_LED_GPIO_Port GPIOB
#endif
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
