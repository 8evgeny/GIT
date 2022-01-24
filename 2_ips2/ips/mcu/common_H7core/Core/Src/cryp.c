/**
  ******************************************************************************
  * File Name          : CRYP.c
  * Description        : This file provides code for the configuration
  *                      of the CRYP instances.
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
#include "cryp.h"

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

CRYP_HandleTypeDef hcryp;
__ALIGN_BEGIN static const uint32_t pKeyCRYP[4] __ALIGN_END = {
                            0x00000000,0x00000000,0x00000000,0x00000000};
DMA_HandleTypeDef hdma_cryp_in;
DMA_HandleTypeDef hdma_cryp_out;

/* CRYP init function */
void MX_CRYP_Init(void)
{

  hcryp.Instance = CRYP;
  hcryp.Init.DataType = CRYP_DATATYPE_8B;
  hcryp.Init.KeySize = CRYP_KEYSIZE_128B;
  hcryp.Init.pKey = (uint32_t *)pKeyCRYP;
  hcryp.Init.Algorithm = CRYP_AES_ECB;
  if (HAL_CRYP_Init(&hcryp) != HAL_OK)
  {
    Error_Handler();
  }

}

void HAL_CRYP_MspInit(CRYP_HandleTypeDef* crypHandle)
{

  if(crypHandle->Instance==CRYP)
  {
  /* USER CODE BEGIN CRYP_MspInit 0 */

  /* USER CODE END CRYP_MspInit 0 */
    /* CRYP clock enable */
    __HAL_RCC_CRYP_CLK_ENABLE();
  
    /* CRYP DMA Init */
    /* CRYP_IN Init */
    hdma_cryp_in.Instance = DMA1_Stream0;
    hdma_cryp_in.Init.Request = DMA_REQUEST_CRYP_IN;
    hdma_cryp_in.Init.Direction = DMA_MEMORY_TO_PERIPH;
    hdma_cryp_in.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_cryp_in.Init.MemInc = DMA_MINC_ENABLE;
    hdma_cryp_in.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    hdma_cryp_in.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
    hdma_cryp_in.Init.Mode = DMA_NORMAL;
    hdma_cryp_in.Init.Priority = DMA_PRIORITY_HIGH;
    hdma_cryp_in.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
    if (HAL_DMA_Init(&hdma_cryp_in) != HAL_OK)
    {
      Error_Handler();
    }

    __HAL_LINKDMA(crypHandle,hdmain,hdma_cryp_in);

    /* CRYP_OUT Init */
    hdma_cryp_out.Instance = DMA1_Stream2;
    hdma_cryp_out.Init.Request = DMA_REQUEST_CRYP_OUT;
    hdma_cryp_out.Init.Direction = DMA_PERIPH_TO_MEMORY;
    hdma_cryp_out.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_cryp_out.Init.MemInc = DMA_MINC_ENABLE;
    hdma_cryp_out.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    hdma_cryp_out.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
    hdma_cryp_out.Init.Mode = DMA_NORMAL;
    hdma_cryp_out.Init.Priority = DMA_PRIORITY_LOW;
    hdma_cryp_out.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
    if (HAL_DMA_Init(&hdma_cryp_out) != HAL_OK)
    {
      Error_Handler();
    }

    __HAL_LINKDMA(crypHandle,hdmaout,hdma_cryp_out);

  /* USER CODE BEGIN CRYP_MspInit 1 */

  /* USER CODE END CRYP_MspInit 1 */
  }
}

void HAL_CRYP_MspDeInit(CRYP_HandleTypeDef* crypHandle)
{

  if(crypHandle->Instance==CRYP)
  {
  /* USER CODE BEGIN CRYP_MspDeInit 0 */

  /* USER CODE END CRYP_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_CRYP_CLK_DISABLE();

    /* CRYP DMA DeInit */
    HAL_DMA_DeInit(crypHandle->hdmain);
    HAL_DMA_DeInit(crypHandle->hdmaout);
  /* USER CODE BEGIN CRYP_MspDeInit 1 */

  /* USER CODE END CRYP_MspDeInit 1 */
  }
} 

/* USER CODE BEGIN 1 */

/* USER CODE END 1 */

