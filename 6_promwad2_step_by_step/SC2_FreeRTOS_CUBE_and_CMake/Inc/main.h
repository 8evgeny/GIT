/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32h7xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define DET_48V_Pin GPIO_PIN_13
#define DET_48V_GPIO_Port GPIOC
#define UPR_MIC2_Pin GPIO_PIN_15
#define UPR_MIC2_GPIO_Port GPIOC
#define POW_DOWN_Pin GPIO_PIN_2
#define POW_DOWN_GPIO_Port GPIOB
#define L4_Pin GPIO_PIN_6
#define L4_GPIO_Port GPIOG
#define L5_Pin GPIO_PIN_7
#define L5_GPIO_Port GPIOG
#define L6_Pin GPIO_PIN_8
#define L6_GPIO_Port GPIOG
#define CFG_UI0_Pin GPIO_PIN_6
#define CFG_UI0_GPIO_Port GPIOC
#define I2C3_INT_Pin GPIO_PIN_8
#define I2C3_INT_GPIO_Port GPIOC
#define L1_Pin GPIO_PIN_10
#define L1_GPIO_Port GPIOC
#define L2_Pin GPIO_PIN_11
#define L2_GPIO_Port GPIOC
#define L3_Pin GPIO_PIN_12
#define L3_GPIO_Port GPIOC
#define K1_Pin GPIO_PIN_9
#define K1_GPIO_Port GPIOG
#define K2_Pin GPIO_PIN_10
#define K2_GPIO_Port GPIOG
#define K3_Pin GPIO_PIN_11
#define K3_GPIO_Port GPIOG
#define K4_Pin GPIO_PIN_12
#define K4_GPIO_Port GPIOG
#define K5_Pin GPIO_PIN_13
#define K5_GPIO_Port GPIOG
#define K6_Pin GPIO_PIN_14
#define K6_GPIO_Port GPIOG
#define TEST_BUT_Pin GPIO_PIN_5
#define TEST_BUT_GPIO_Port GPIOB
#define TEST_LED_Pin GPIO_PIN_9
#define TEST_LED_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
