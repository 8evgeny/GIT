/**
  ******************************************************************************
  * File Name          : mdma.h
  * Description        : This file contains all the function prototypes for
  *                      the mdma.c file
  ******************************************************************************
  * @attention
  *
  *<h2><center>A part of GITComm IPS Project for Group of Industrial
  * Technologies LLC.</center></h2>
  * <h2><center>&copy; Copyright (c) 2020 Donskih Dmitriy.
  * All rights reserved.</center></h2>
  *
  ******************************************************************************
  */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __mdma_H
#define __mdma_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* MDMA transfer handles -----------------------------------------------------*/
extern MDMA_HandleTypeDef hmdma_mdma_channel40_dma1_stream0_tc_0;
extern MDMA_HandleTypeDef hmdma_mdma_channel41_dma1_stream2_tc_0;
extern MDMA_HandleTypeDef hmdma_mdma_channel42_dma1_stream3_tc_0;
extern MDMA_HandleTypeDef hmdma_mdma_channel43_dma1_stream4_tc_0;

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

void MX_MDMA_Init(void);

/* USER CODE BEGIN Prototypes */

/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif

#endif /* __mdma_H */

/**
  * @}
  */

