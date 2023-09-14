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

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define LED_E1_Pin GPIO_PIN_2
#define LED_E1_GPIO_Port GPIOE
#define LED_E2_Pin GPIO_PIN_3
#define LED_E2_GPIO_Port GPIOE
#define LED_E3_Pin GPIO_PIN_4
#define LED_E3_GPIO_Port GPIOE
#define LED_E4_Pin GPIO_PIN_5
#define LED_E4_GPIO_Port GPIOE
#define LED_E5_Pin GPIO_PIN_6
#define LED_E5_GPIO_Port GPIOE
#define LED_E6_Pin GPIO_PIN_0
#define LED_E6_GPIO_Port GPIOF
#define LED_ERR_Pin GPIO_PIN_1
#define LED_ERR_GPIO_Port GPIOF
#define LED_ON_Pin GPIO_PIN_2
#define LED_ON_GPIO_Port GPIOF
#define RMII_MDC_Pin GPIO_PIN_1
#define RMII_MDC_GPIO_Port GPIOC
#define RMII_REF_CLK_Pin GPIO_PIN_1
#define RMII_REF_CLK_GPIO_Port GPIOA
#define RMII_MDIO_Pin GPIO_PIN_2
#define RMII_MDIO_GPIO_Port GPIOA
#define RMII_CRS_DV_Pin GPIO_PIN_7
#define RMII_CRS_DV_GPIO_Port GPIOA
#define RMII_RXD0_Pin GPIO_PIN_4
#define RMII_RXD0_GPIO_Port GPIOC
#define RMII_RXD1_Pin GPIO_PIN_5
#define RMII_RXD1_GPIO_Port GPIOC
#define LD1_Pin GPIO_PIN_0
#define LD1_GPIO_Port GPIOB
#define START_Pin GPIO_PIN_11
#define START_GPIO_Port GPIOE
#define RMII_TXD1_Pin GPIO_PIN_13
#define RMII_TXD1_GPIO_Port GPIOB
#define RTS3_NAP_Pin GPIO_PIN_14
#define RTS3_NAP_GPIO_Port GPIOB
#define LED_0_Pin GPIO_PIN_6
#define LED_0_GPIO_Port GPIOG
#define SWDIO_Pin GPIO_PIN_13
#define SWDIO_GPIO_Port GPIOA
#define SWCLK_Pin GPIO_PIN_14
#define SWCLK_GPIO_Port GPIOA
#define PS1_Pin GPIO_PIN_1
#define PS1_GPIO_Port GPIOD
#define PS2_Pin GPIO_PIN_2
#define PS2_GPIO_Port GPIOD
#define PS3_Pin GPIO_PIN_3
#define PS3_GPIO_Port GPIOD
#define PS4_Pin GPIO_PIN_4
#define PS4_GPIO_Port GPIOD
#define PS5_Pin GPIO_PIN_5
#define PS5_GPIO_Port GPIOD
#define PS6_Pin GPIO_PIN_6
#define PS6_GPIO_Port GPIOD
#define BUT_Pin GPIO_PIN_7
#define BUT_GPIO_Port GPIOD
#define RMII_TX_EN_Pin GPIO_PIN_11
#define RMII_TX_EN_GPIO_Port GPIOG
#define RMII_TXD0_Pin GPIO_PIN_13
#define RMII_TXD0_GPIO_Port GPIOG
#define DBG_U1_TX_Pin GPIO_PIN_6
#define DBG_U1_TX_GPIO_Port GPIOB
#define DBG_U1_RX_Pin GPIO_PIN_7
#define DBG_U1_RX_GPIO_Port GPIOB
#define SCL_Pin GPIO_PIN_8
#define SCL_GPIO_Port GPIOB
#define SDA_Pin GPIO_PIN_9
#define SDA_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */
#define TX_485()            HAL_GPIO_WritePin(RTS3_NAP_GPIO_Port, RTS3_NAP_Pin, GPIO_PIN_SET)
#define RX_485()            HAL_GPIO_WritePin(RTS3_NAP_GPIO_Port, RTS3_NAP_Pin, GPIO_PIN_RESET)

#define LED_ON_ON()         HAL_GPIO_WritePin(LED_ON_GPIO_Port, LED_ON_Pin, GPIO_PIN_SET)
#define LED_ON_OFF()        HAL_GPIO_WritePin(LED_ON_GPIO_Port, LED_ON_Pin, GPIO_PIN_RESET)

#define LED_ERR_ON()        HAL_GPIO_WritePin(LED_ERR_GPIO_Port, LED_ERR_Pin, GPIO_PIN_SET)
#define LED_ERR_OFF()       HAL_GPIO_WritePin(LED_ERR_GPIO_Port, LED_ERR_Pin, GPIO_PIN_RESET)

#define LED_E1_ON()         HAL_GPIO_WritePin(LED_E1_GPIO_Port, LED_E1_Pin, GPIO_PIN_SET)
#define LED_E1_OFF()        HAL_GPIO_WritePin(LED_E1_GPIO_Port, LED_E1_Pin, GPIO_PIN_RESET)
#define LED_E2_ON()         HAL_GPIO_WritePin(LED_E2_GPIO_Port, LED_E2_Pin, GPIO_PIN_SET)
#define LED_E2_OFF()        HAL_GPIO_WritePin(LED_E2_GPIO_Port, LED_E2_Pin, GPIO_PIN_RESET)
#define LED_E3_ON()         HAL_GPIO_WritePin(LED_E3_GPIO_Port, LED_E3_Pin, GPIO_PIN_SET)
#define LED_E3_OFF()        HAL_GPIO_WritePin(LED_E3_GPIO_Port, LED_E3_Pin, GPIO_PIN_RESET)
#define LED_E4_ON()         HAL_GPIO_WritePin(LED_E4_GPIO_Port, LED_E4_Pin, GPIO_PIN_SET)
#define LED_E4_OFF()        HAL_GPIO_WritePin(LED_E4_GPIO_Port, LED_E4_Pin, GPIO_PIN_RESET)
#define LED_E5_ON()         HAL_GPIO_WritePin(LED_E5_GPIO_Port, LED_E5_Pin, GPIO_PIN_SET)
#define LED_E5_OFF()        HAL_GPIO_WritePin(LED_E5_GPIO_Port, LED_E5_Pin, GPIO_PIN_RESET)
#define LED_E6_ON()         HAL_GPIO_WritePin(LED_E6_GPIO_Port, LED_E6_Pin, GPIO_PIN_SET)
#define LED_E6_OFF()        HAL_GPIO_WritePin(LED_E6_GPIO_Port, LED_E6_Pin, GPIO_PIN_RESET)
/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
