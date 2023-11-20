/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2023 STMicroelectronics.
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
#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal.h"
#include "stm32f4xx_ll_usart.h"
#include "stm32f4xx_ll_rcc.h"
#include "stm32f4xx_ll_bus.h"
#include "stm32f4xx_ll_cortex.h"
#include "stm32f4xx_ll_system.h"
#include "stm32f4xx_ll_utils.h"
#include "stm32f4xx_ll_pwr.h"
#include "stm32f4xx_ll_gpio.h"
#include "stm32f4xx_ll_dma.h"

#include "stm32f4xx_ll_exti.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */
extern ADC_HandleTypeDef hadc1;
/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define EXTIN1_Pin GPIO_PIN_2
#define EXTIN1_GPIO_Port GPIOF
#define EXTIN2_Pin GPIO_PIN_3
#define EXTIN2_GPIO_Port GPIOF
#define EXTIN3_Pin GPIO_PIN_4
#define EXTIN3_GPIO_Port GPIOF
#define EXTIN4_Pin GPIO_PIN_5
#define EXTIN4_GPIO_Port GPIOF
#define EXTIN5_Pin GPIO_PIN_6
#define EXTIN5_GPIO_Port GPIOF
#define EN_25W_Pin GPIO_PIN_13
#define EN_25W_GPIO_Port GPIOF
#define nEN_HLT_Pin GPIO_PIN_14
#define nEN_HLT_GPIO_Port GPIOF
#define USART3_485_DE_Pin GPIO_PIN_7
#define USART3_485_DE_GPIO_Port GPIOE
#define ST_LOUTDIS_Pin GPIO_PIN_12
#define ST_LOUTDIS_GPIO_Port GPIOG
#define REL_Pin GPIO_PIN_13
#define REL_GPIO_Port GPIOG
#define LED_ON_Pin GPIO_PIN_14
#define LED_ON_GPIO_Port GPIOG
#define LED_ERR_Pin GPIO_PIN_15
#define LED_ERR_GPIO_Port GPIOG
#define LED_S1_Pin GPIO_PIN_8
#define LED_S1_GPIO_Port GPIOB
#define LED_S2_Pin GPIO_PIN_9
#define LED_S2_GPIO_Port GPIOB
#define nAIC_RES_Pin GPIO_PIN_0
#define nAIC_RES_GPIO_Port GPIOE
/* USER CODE BEGIN Private defines */

#define FIRMWARE_VERSION        "2.03"

#define READ_TUBE_BUT() HAL_GPIO_ReadPin(EXTIN2_GPIO_Port, EXTIN2_Pin)
#define READ_TUBE_HOOK() !HAL_GPIO_ReadPin(EXTIN3_GPIO_Port, EXTIN3_Pin)
/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
