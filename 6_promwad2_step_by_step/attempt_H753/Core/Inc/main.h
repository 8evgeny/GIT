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
#define MII_MOC_Pin GPIO_PIN_1
#define MII_MOC_GPIO_Port GPIOC
#define MII_TXD2_Pin GPIO_PIN_2
#define MII_TXD2_GPIO_Port GPIOC
#define MII_TX_CLK_Pin GPIO_PIN_3
#define MII_TX_CLK_GPIO_Port GPIOC
#define MII_CRS_Pin GPIO_PIN_0
#define MII_CRS_GPIO_Port GPIOA
#define MII_RX_CLK_Pin GPIO_PIN_1
#define MII_RX_CLK_GPIO_Port GPIOA
#define MII_MDIO_Pin GPIO_PIN_2
#define MII_MDIO_GPIO_Port GPIOA
#define MII_COL_Pin GPIO_PIN_3
#define MII_COL_GPIO_Port GPIOH
#define MII_RX_DV_Pin GPIO_PIN_7
#define MII_RX_DV_GPIO_Port GPIOA
#define MII_RXD0_Pin GPIO_PIN_4
#define MII_RXD0_GPIO_Port GPIOC
#define MII_RXD1_Pin GPIO_PIN_5
#define MII_RXD1_GPIO_Port GPIOC
#define MII_RXD2_Pin GPIO_PIN_0
#define MII_RXD2_GPIO_Port GPIOB
#define MII_RXD3_Pin GPIO_PIN_1
#define MII_RXD3_GPIO_Port GPIOB
#define POW_DOWN_Pin GPIO_PIN_2
#define POW_DOWN_GPIO_Port GPIOB
#define MII_RX_ER_Pin GPIO_PIN_10
#define MII_RX_ER_GPIO_Port GPIOB
#define MII_TX_EN_Pin GPIO_PIN_11
#define MII_TX_EN_GPIO_Port GPIOB
#define MII_TXD0_Pin GPIO_PIN_12
#define MII_TXD0_GPIO_Port GPIOB
#define MII_TXD1_Pin GPIO_PIN_13
#define MII_TXD1_GPIO_Port GPIOB
#define L4_Pin GPIO_PIN_6
#define L4_GPIO_Port GPIOG
#define L5_Pin GPIO_PIN_7
#define L5_GPIO_Port GPIOG
#define L6_Pin GPIO_PIN_8
#define L6_GPIO_Port GPIOG
#define TANG_UPR_Pin GPIO_PIN_9
#define TANG_UPR_GPIO_Port GPIOA
#define PEDAL_UPR_Pin GPIO_PIN_10
#define PEDAL_UPR_GPIO_Port GPIOA
#define RELE1_UPR_Pin GPIO_PIN_11
#define RELE1_UPR_GPIO_Port GPIOA
#define AMP_UPR_Pin GPIO_PIN_12
#define AMP_UPR_GPIO_Port GPIOA
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
#define MII_TXD3_Pin GPIO_PIN_8
#define MII_TXD3_GPIO_Port GPIOB
#define TEST_LED_Pin GPIO_PIN_9
#define TEST_LED_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
