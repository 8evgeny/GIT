/**
  ******************************************************************************
  * File Name          : HASH.c
  * Description        : This file provides code for the configuration
  *                      of the HASH instances.
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

/* Includes ------------------------------------------------------------------*/
#include "hash.h"

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

HASH_HandleTypeDef hhash;
DMA_HandleTypeDef hdma_hash_in;

/* HASH init function */
void MX_HASH_Init(void)
{

  hhash.Init.DataType = HASH_DATATYPE_8B;
  if (HAL_HASH_Init(&hhash) != HAL_OK)
  {
    Error_Handler();
  }

}

void HAL_HASH_MspInit(HASH_HandleTypeDef* hashHandle)
{

  /* USER CODE BEGIN HASH_MspInit 0 */

  /* USER CODE END HASH_MspInit 0 */
    /* HASH clock enable */
    __HAL_RCC_HASH_CLK_ENABLE();
  
    /* HASH DMA Init */
    /* HASH_IN Init */
    hdma_hash_in.Instance = DMA1_Stream1;
    hdma_hash_in.Init.Request = DMA_REQUEST_HASH_IN;
    hdma_hash_in.Init.Direction = DMA_MEMORY_TO_PERIPH;
    hdma_hash_in.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_hash_in.Init.MemInc = DMA_MINC_ENABLE;
    hdma_hash_in.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    hdma_hash_in.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
    hdma_hash_in.Init.Mode = DMA_NORMAL;
    hdma_hash_in.Init.Priority = DMA_PRIORITY_LOW;
    hdma_hash_in.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
    if (HAL_DMA_Init(&hdma_hash_in) != HAL_OK)
    {
      Error_Handler();
    }

    __HAL_LINKDMA(hashHandle,hdmain,hdma_hash_in);

  /* USER CODE BEGIN HASH_MspInit 1 */

  /* USER CODE END HASH_MspInit 1 */
}

void HAL_HASH_MspDeInit(HASH_HandleTypeDef* hashHandle)
{

  /* USER CODE BEGIN HASH_MspDeInit 0 */

  /* USER CODE END HASH_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_HASH_CLK_DISABLE();

    /* HASH DMA DeInit */
    HAL_DMA_DeInit(hashHandle->hdmain);
  /* USER CODE BEGIN HASH_MspDeInit 1 */

  /* USER CODE END HASH_MspDeInit 1 */
} 

/* USER CODE BEGIN 1 */

/* USER CODE END 1 */
