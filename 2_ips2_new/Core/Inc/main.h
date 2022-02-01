/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
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

#define SC_2
//#define SC_4

#define WORK_MODE_UDEF		0
#define WORK_MODE_HWFAIL	1
#define WORK_MODE_NOCFG		2
#define WORK_MODE_OK		3

uint8_t get_pdo_work_mode();
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
#ifdef SC_4
#define UPR_PHONE_Pin GPIO_PIN_14
#endif
#ifdef SC_2
#define UPR_PHONE_Pin GPIO_PIN_6
#endif
#define UPR_PHONE_GPIO_Port GPIOC
#define UPR_MIC2_Pin GPIO_PIN_15
#define UPR_MIC2_GPIO_Port GPIOC
#define UPR1_SP_Pin GPIO_PIN_3
#define UPR1_SP_GPIO_Port GPIOA
#define UPR2_FAN_Pin GPIO_PIN_4
#define UPR2_FAN_GPIO_Port GPIOA
#define MKVKL_UPR_Pin GPIO_PIN_5
#define MKVKL_UPR_GPIO_Port GPIOA
#define RELE_UPR_Pin GPIO_PIN_6
#define RELE_UPR_GPIO_Port GPIOA
#define POW_DOWN_Pin GPIO_PIN_2
#define POW_DOWN_GPIO_Port GPIOB
#define CFG_UI0_Pin GPIO_PIN_6
#define CFG_UI0_GPIO_Port GPIOC
#define CFG_UI1_Pin GPIO_PIN_7
#define CFG_UI1_GPIO_Port GPIOC
#define CFG_UI2_Pin GPIO_PIN_8
#define CFG_UI2_GPIO_Port GPIOC
#define VOL_UP_Pin GPIO_PIN_9
#define VOL_UP_GPIO_Port GPIOA
#define VOL_DOWN_Pin GPIO_PIN_10
#define VOL_DOWN_GPIO_Port GPIOA
#define SENS_UP_Pin GPIO_PIN_11
#define SENS_UP_GPIO_Port GPIOA
#define SENS_DOWN_Pin GPIO_PIN_12
#define SENS_DOWN_GPIO_Port GPIOA
#define INT_BUT_Pin GPIO_PIN_4
#define INT_BUT_GPIO_Port GPIOA
#define NORMA_UPR_Pin GPIO_PIN_6
#define NORMA_UPR_GPIO_Port GPIOD
#define TEST_BUT_Pin GPIO_PIN_5
#define TEST_BUT_GPIO_Port GPIOB
#define TEST_LED_Pin GPIO_PIN_9
#define TEST_LED_GPIO_Port GPIOB
#define LED1 GPIO_PIN_10
#define LED2 GPIO_PIN_11
#define LED3 GPIO_PIN_12
#define LED4 GPIO_PIN_6
#define LED5 GPIO_PIN_7
#define LED6 GPIO_PIN_8
#define K1 GPIO_PIN_9
#define K2 GPIO_PIN_10
#define K3 GPIO_PIN_11
#define K4 GPIO_PIN_12
#define K5 GPIO_PIN_13
#define K6 GPIO_PIN_14

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
