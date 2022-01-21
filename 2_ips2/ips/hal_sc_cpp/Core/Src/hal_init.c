/*
 * hal_init.c
 *
 *  Created on: Jul 30, 2020
 *      Author: Dmitry Donskih
 */
/*
 * file - a part of CMppCore -an object engine for Cortex-M
 * microcontrollers.
 *
 * Copyright (c) 2019-2020 by Dmitry Donskih
 * All rights reserved.
 *
 * Attention Software modification allowed only with
 * the permission of the author.
 */

#include "hal_init.h"
#include "main.h"

//CRC_HandleTypeDef hcrc;
CRYP_HandleTypeDef hcryp;
__ALIGN_BEGIN static const uint32_t pKeyCRYP[4] __ALIGN_END =
  { 0x00000000, 0x00000000, 0x00000000, 0x00000000 };
//HASH_HandleTypeDef hhash;
RNG_HandleTypeDef hrng;

//I2C_HandleTypeDef hi2c1;
I2C_HandleTypeDef hi2c2;

SAI_HandleTypeDef hsai_BlockA1;
SAI_HandleTypeDef hsai_BlockB1;
DMA_HandleTypeDef hdma_sai1_a;
DMA_HandleTypeDef hdma_sai1_b;

RTC_HandleTypeDef hrtc;
TIM_HandleTypeDef htim5;

UART_HandleTypeDef huart7;
DMA_HandleTypeDef hdma_uart7_rx;
DMA_HandleTypeDef hdma_uart7_tx;

SRAM_HandleTypeDef hsram1;

#ifdef SC4BOPLA
I2C_HandleTypeDef hi2c3;
#endif

//static void SystemClock_Config (void);

//static void MPU_Config(void);
static void MX_GPIO_Init (void);
static void MX_DMA_Init (void);
//static void MX_CRC_Init (void);
static void MX_CRYP_Init (void);
//static void MX_HASH_Init (void);
static void MX_RNG_Init (void);
static void MX_FMC_Init (void);
//static void MX_I2C1_Init (void);
static void MX_I2C2_Init (void);
static void MX_SAI1_Init (void);
static void MX_UART7_Init (void);
static void MX_RTC_Init (void);
static void MX_TIM5_Init (void);

#ifdef SC4BOPLA
static void MX_I2C3_Init(void);
#endif

void HAL_BSP_Init (void)
{
//  SystemClock_Config ();
  MX_GPIO_Init ();
  MX_DMA_Init ();
//  MX_CRC_Init ();
  MX_CRYP_Init ();
//  MX_HASH_Init ();
  MX_RNG_Init ();
  MX_FMC_Init ();
//  MX_I2C1_Init ();
  MX_I2C2_Init ();
  MX_SAI1_Init ();
//  MX_UART7_Init (); //eat too much mem;
  MX_RTC_Init ();
  MX_TIM5_Init ();
#ifdef SC4BOPLA
    MX_I2C3_Init();
#endif
  /* Enable Ethernet PHY(MII) clock strobe 25 MHz */
  HAL_RCC_MCOConfig (RCC_MCO1, RCC_MCO1SOURCE_HSE, RCC_MCODIV_1);
}

/**
 * @brief CRC Initialization Function
 * @param None
 * @retval None
 */
//static void MX_CRC_Init (void)
//{
//
//  hcrc.Instance = CRC;
//  hcrc.Init.DefaultPolynomialUse = DEFAULT_POLYNOMIAL_ENABLE;
//  hcrc.Init.DefaultInitValueUse = DEFAULT_INIT_VALUE_ENABLE;
//  hcrc.Init.InputDataInversionMode = CRC_INPUTDATA_INVERSION_NONE;
//  hcrc.Init.OutputDataInversionMode = CRC_OUTPUTDATA_INVERSION_DISABLE;
//  hcrc.InputDataFormat = CRC_INPUTDATA_FORMAT_BYTES;
//  if (HAL_CRC_Init (&hcrc) != HAL_OK)
//    {
//      Error_Handler ();
//    }
//}

/**
 * @brief CRYP Initialization Function
 * @param None
 * @retval None
 */
static void MX_CRYP_Init (void)
{
  hcryp.Instance = CRYP;
  hcryp.Init.DataType = CRYP_DATATYPE_32B;
  hcryp.Init.KeySize = CRYP_KEYSIZE_128B;
  hcryp.Init.pKey = (uint32_t*) pKeyCRYP;
  hcryp.Init.Algorithm = CRYP_AES_ECB;
  if (HAL_CRYP_Init (&hcryp) != HAL_OK)
    {
      Error_Handler ();
    }
}

/**
 * @brief HASH Initialization Function
 * @param None
 * @retval None
 */
//static void MX_HASH_Init (void)
//{
//
//  hhash.Init.DataType = HASH_DATATYPE_32B;
//  if (HAL_HASH_Init (&hhash) != HAL_OK)
//    {
//      Error_Handler ();
//    }
//}

/**
 * @brief I2C1 Initialization Function
 * @param None
 * @retval None
 */
//static void MX_I2C1_Init (void)
//{
//  hi2c1.Instance = I2C1;
//  hi2c1.Init.Timing = 0x00602173; //0x00300B29;
//  hi2c1.Init.OwnAddress1 = 0;
//  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
//  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
//  hi2c1.Init.OwnAddress2 = 0;
//  hi2c1.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
//  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
//  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
//  if (HAL_I2C_Init (&hi2c1) != HAL_OK)
//    {
//      Error_Handler ();
//    }
//  /** Configure Analogue filter
//   */
//  if (HAL_I2CEx_ConfigAnalogFilter (&hi2c1, I2C_ANALOGFILTER_ENABLE) != HAL_OK)
//    {
//      Error_Handler ();
//    }
//  /** Configure Digital filter
//   */
//  if (HAL_I2CEx_ConfigDigitalFilter (&hi2c1, 0) != HAL_OK)
//    {
//      Error_Handler ();
//    }
//  /** I2C Enable Fast Mode Plus
//   */
//  //HAL_I2CEx_EnableFastModePlus(I2C_FASTMODEPLUS_I2C1);
//}

/**
 * @brief I2C2 Initialization Function
 * @param None
 * @retval None
 */
static void MX_I2C2_Init (void)
{
  hi2c2.Instance = I2C2;
  hi2c2.Init.Timing = 0x00602173;
  hi2c2.Init.OwnAddress1 = 0;
  hi2c2.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c2.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c2.Init.OwnAddress2 = 0;
  hi2c2.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
  hi2c2.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c2.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init (&hi2c2) != HAL_OK)
    {
      Error_Handler ();
    }
  /** Configure Analogue filter
   */
  if (HAL_I2CEx_ConfigAnalogFilter (&hi2c2, I2C_ANALOGFILTER_ENABLE) != HAL_OK)
    {
      Error_Handler ();
    }
  /** Configure Digital filter
   */
  if (HAL_I2CEx_ConfigDigitalFilter (&hi2c2, 0) != HAL_OK)
    {
      Error_Handler ();
    }
  /** I2C Enable Fast Mode Plus
   */
  //HAL_I2CEx_EnableFastModePlus(I2C_FASTMODEPLUS_I2C2);
}

/**
 * @brief I2C3 Initialization Function
 * @param None
 * @retval None
 */
#ifdef SC4BOPLA
static void MX_I2C3_Init(void)
{
  hi2c3.Instance = I2C3;
  hi2c3.Init.Timing = 0x00300B29;
  hi2c3.Init.OwnAddress1 = 0;
  hi2c3.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c3.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c3.Init.OwnAddress2 = 0;
  hi2c3.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
  hi2c3.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c3.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c3) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure Analogue filter
  */
  if (HAL_I2CEx_ConfigAnalogFilter(&hi2c3, I2C_ANALOGFILTER_ENABLE) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure Digital filter
  */
  if (HAL_I2CEx_ConfigDigitalFilter(&hi2c3, 0) != HAL_OK)
  {
    Error_Handler();
  }
  /** I2C Enable Fast Mode Plus
  */
  HAL_I2CEx_EnableFastModePlus(I2C_FASTMODEPLUS_I2C3);
}
#endif

/**
 * @brief RNG Initialization Function
 * @param None
 * @retval None
 */
static void MX_RNG_Init (void)
{

  hrng.Instance = RNG;
  hrng.Init.ClockErrorDetection = RNG_CED_ENABLE;
  if (HAL_RNG_Init (&hrng) != HAL_OK)
    {
      Error_Handler ();
    }
}

/**
 * @brief RTC Initialization Function
 * @param None
 * @retval None
 */
static void MX_RTC_Init (void)
{

  /* USER CODE BEGIN RTC_Init 0 */

  /* USER CODE END RTC_Init 0 */

  /* USER CODE BEGIN RTC_Init 1 */

  /* USER CODE END RTC_Init 1 */
  /** Initialize RTC Only
   */
  hrtc.Instance = RTC;
  hrtc.Init.HourFormat = RTC_HOURFORMAT_24;
  hrtc.Init.AsynchPrediv = 127;
  hrtc.Init.SynchPrediv = 255;
  hrtc.Init.OutPut = RTC_OUTPUT_DISABLE;
  hrtc.Init.OutPutPolarity = RTC_OUTPUT_POLARITY_HIGH;
  hrtc.Init.OutPutType = RTC_OUTPUT_TYPE_OPENDRAIN;
  hrtc.Init.OutPutRemap = RTC_OUTPUT_REMAP_NONE;
  if (HAL_RTC_Init (&hrtc) != HAL_OK)
    {
      Error_Handler ();
    }
  /* USER CODE BEGIN RTC_Init 2 */

  /* USER CODE END RTC_Init 2 */

}

/**
 * @brief SAI1 Initialization Function
 * @param None
 * @retval None
 */
static void MX_SAI1_Init (void)
{

  /* USER CODE BEGIN SAI1_Init 0 */

  /* USER CODE END SAI1_Init 0 */

  /* USER CODE BEGIN SAI1_Init 1 */

  /* USER CODE END SAI1_Init 1 */
  hsai_BlockA1.Instance = SAI1_Block_A;
  hsai_BlockA1.Init.AudioMode = SAI_MODEMASTER_TX;
  hsai_BlockA1.Init.Synchro = SAI_ASYNCHRONOUS;
  hsai_BlockA1.Init.OutputDrive = SAI_OUTPUTDRIVE_DISABLE;
  hsai_BlockA1.Init.NoDivider = SAI_MASTERDIVIDER_ENABLE;
  hsai_BlockA1.Init.FIFOThreshold = SAI_FIFOTHRESHOLD_EMPTY;
  hsai_BlockA1.Init.AudioFrequency = SAI_AUDIO_FREQUENCY_16K;
  hsai_BlockA1.Init.SynchroExt = SAI_SYNCEXT_DISABLE;
  hsai_BlockA1.Init.MonoStereoMode = SAI_STEREOMODE;
  hsai_BlockA1.Init.CompandingMode = SAI_NOCOMPANDING;
  hsai_BlockA1.Init.TriState = SAI_OUTPUT_NOTRELEASED;
  if (HAL_SAI_InitProtocol (&hsai_BlockA1, SAI_I2S_STANDARD, SAI_PROTOCOL_DATASIZE_16BIT, 2)
      != HAL_OK)
    {
      Error_Handler ();
    }
  hsai_BlockB1.Instance = SAI1_Block_B;
  hsai_BlockB1.Init.AudioMode = SAI_MODESLAVE_RX;
  hsai_BlockB1.Init.Synchro = SAI_SYNCHRONOUS;
  hsai_BlockB1.Init.OutputDrive = SAI_OUTPUTDRIVE_DISABLE;
  hsai_BlockB1.Init.FIFOThreshold = SAI_FIFOTHRESHOLD_EMPTY;
  hsai_BlockB1.Init.SynchroExt = SAI_SYNCEXT_DISABLE;
  hsai_BlockB1.Init.MonoStereoMode = SAI_STEREOMODE;
  hsai_BlockB1.Init.CompandingMode = SAI_NOCOMPANDING;
  hsai_BlockB1.Init.TriState = SAI_OUTPUT_NOTRELEASED;
  if (HAL_SAI_InitProtocol (&hsai_BlockB1, SAI_I2S_STANDARD, SAI_PROTOCOL_DATASIZE_16BIT, 2)
      != HAL_OK)
    {
      Error_Handler ();
    }
  /* USER CODE BEGIN SAI1_Init 2 */

  /* USER CODE END SAI1_Init 2 */

}

/**
 * @brief TIM5 Initialization Function
 * @param None
 * @retval None
 */
static void MX_TIM5_Init (void)
{

  /* USER CODE BEGIN TIM5_Init 0 */

  /* USER CODE END TIM5_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig =
    { 0 };
  TIM_MasterConfigTypeDef sMasterConfig =
    { 0 };

  /* USER CODE BEGIN TIM5_Init 1 */

  /* USER CODE END TIM5_Init 1 */
  htim5.Instance = TIM5;
  htim5.Init.Prescaler = 4800;
  htim5.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim5.Init.Period = 0xFFFFFF;
  htim5.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim5.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
  if (HAL_TIM_Base_Init (&htim5) != HAL_OK)
    {
      Error_Handler ();
    }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource (&htim5, &sClockSourceConfig) != HAL_OK)
    {
      Error_Handler ();
    }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization (&htim5, &sMasterConfig) != HAL_OK)
    {
      Error_Handler ();
    }
  /* USER CODE BEGIN TIM5_Init 2 */

  /* USER CODE END TIM5_Init 2 */

}

/**
 * @brief UART7 Initialization Function
 * @param None
 * @retval None
 */
/* HAL STD UART DRIVER EAT TOO MUCH MEM!!!
static void MX_UART7_Init (void)
{

  huart7.Instance = UART7;
  huart7.Init.BaudRate = 115200;
  huart7.Init.WordLength = UART_WORDLENGTH_8B;
  huart7.Init.StopBits = UART_STOPBITS_1;
  huart7.Init.Parity = UART_PARITY_NONE;
  huart7.Init.Mode = UART_MODE_TX_RX;
  huart7.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart7.Init.OverSampling = UART_OVERSAMPLING_16;
  huart7.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart7.Init.ClockPrescaler = UART_PRESCALER_DIV1;
  huart7.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init (&huart7) != HAL_OK)
    {
      Error_Handler ();
    }
  if (HAL_UARTEx_SetTxFifoThreshold (&huart7, UART_TXFIFO_THRESHOLD_1_8) != HAL_OK)
    {
      Error_Handler ();
    }
  if (HAL_UARTEx_SetRxFifoThreshold (&huart7, UART_RXFIFO_THRESHOLD_1_8) != HAL_OK)
    {
      Error_Handler ();
    }
  if (HAL_UARTEx_DisableFifoMode (&huart7) != HAL_OK)
    {
      Error_Handler ();
    }
}
*/
/**
 * Enable DMA controller clock
 */
static void MX_DMA_Init (void)
{

  /* DMA controller clock enable */
  __HAL_RCC_DMA1_CLK_ENABLE();
  __HAL_RCC_DMA2_CLK_ENABLE();

  /* DMA interrupt init */
  /* DMA1_Stream0_IRQn interrupt configuration */
  HAL_NVIC_SetPriority (DMA1_Stream0_IRQn, 6, 0);
  HAL_NVIC_EnableIRQ (DMA1_Stream0_IRQn);
  /* DMA1_Stream1_IRQn interrupt configuration */
  //HAL_NVIC_SetPriority(DMA1_Stream1_IRQn, 5, 0);
  //HAL_NVIC_EnableIRQ(DMA1_Stream1_IRQn);
  /* DMA1_Stream2_IRQn interrupt configuration */
  HAL_NVIC_SetPriority (DMA1_Stream2_IRQn, 6, 0);
  HAL_NVIC_EnableIRQ (DMA1_Stream2_IRQn);
  /* DMA1_Stream3_IRQn interrupt configuration */
  //HAL_NVIC_SetPriority(DMA1_Stream3_IRQn, 5, 0);
  //HAL_NVIC_EnableIRQ(DMA1_Stream3_IRQn);
  HAL_NVIC_SetPriority (DMA2_Stream0_IRQn, 6, 0);
  HAL_NVIC_EnableIRQ (DMA2_Stream0_IRQn);
  /* DMA2_Stream2_IRQn interrupt configuration */
  HAL_NVIC_SetPriority (DMA2_Stream2_IRQn, 6, 0);
  HAL_NVIC_EnableIRQ (DMA2_Stream2_IRQn);

}

/* FMC initialization function */
static void MX_FMC_Init (void)
{

  /* USER CODE BEGIN FMC_Init 0 */

  /* USER CODE END FMC_Init 0 */

  FMC_NORSRAM_TimingTypeDef Timing =
    { 0 };

  /* USER CODE BEGIN FMC_Init 1 */

  /* USER CODE END FMC_Init 1 */

  /** Perform the SRAM1 memory initialization sequence
   */
  hsram1.Instance = FMC_NORSRAM_DEVICE;
  hsram1.Extended = FMC_NORSRAM_EXTENDED_DEVICE;
  /* hsram1.Init */
  hsram1.Init.NSBank = FMC_NORSRAM_BANK1;
  hsram1.Init.DataAddressMux = FMC_DATA_ADDRESS_MUX_DISABLE;
  hsram1.Init.MemoryType = FMC_MEMORY_TYPE_SRAM;
  hsram1.Init.MemoryDataWidth = FMC_NORSRAM_MEM_BUS_WIDTH_32;
  hsram1.Init.BurstAccessMode = FMC_BURST_ACCESS_MODE_DISABLE;
  hsram1.Init.WaitSignalPolarity = FMC_WAIT_SIGNAL_POLARITY_LOW;
  hsram1.Init.WaitSignalActive = FMC_WAIT_TIMING_BEFORE_WS;
  hsram1.Init.WriteOperation = FMC_WRITE_OPERATION_ENABLE;
  hsram1.Init.WaitSignal = FMC_WAIT_SIGNAL_DISABLE;
  hsram1.Init.ExtendedMode = FMC_EXTENDED_MODE_DISABLE;
  hsram1.Init.AsynchronousWait = FMC_ASYNCHRONOUS_WAIT_DISABLE;
  hsram1.Init.WriteBurst = FMC_WRITE_BURST_DISABLE;
  hsram1.Init.ContinuousClock = FMC_CONTINUOUS_CLOCK_SYNC_ONLY;
  hsram1.Init.WriteFifo = FMC_WRITE_FIFO_ENABLE;
  hsram1.Init.PageSize = FMC_PAGE_SIZE_NONE;
  /* Timing */
  Timing.AddressSetupTime = 2;
  Timing.AddressHoldTime = 15;
  Timing.DataSetupTime = 2;
  Timing.BusTurnAroundDuration = 2;
  Timing.CLKDivision = 16;
  Timing.DataLatency = 17;
  Timing.AccessMode = FMC_ACCESS_MODE_A;
  /* ExtTiming */

  if (HAL_SRAM_Init (&hsram1, &Timing, NULL) != HAL_OK)
    {
      Error_Handler ();
    }

  /* USER CODE BEGIN FMC_Init 2 */

  /* USER CODE END FMC_Init 2 */
}

#ifdef SC2DIOBOARD
/**
 * @brief GPIO Initialization Function
 * @param None
 * @retval None
 */
static void MX_GPIO_Init (void)
{
  GPIO_InitTypeDef GPIO_InitStruct =
    { 0 };

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOI_CLK_ENABLE();
  __HAL_RCC_GPIOF_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOG_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin (GPIOG,
		     DO1_Pin | DO2_Pin | DO3_Pin | DO4_Pin | DO5_Pin | DO6_Pin | DO7_Pin | DO8_Pin,
		     GPIO_PIN_SET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin (LED_NORM_GPIO_Port, LED_NORM_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin (TEST_LED_GPIO_Port, TEST_LED_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : DO1_Pin DO2_Pin DO3_Pin DO4_Pin
   DO5_Pin DO6_Pin DO7_Pin DO8_Pin */
  GPIO_InitStruct.Pin = DO1_Pin | DO2_Pin | DO3_Pin | DO4_Pin | DO5_Pin | DO6_Pin | DO7_Pin
      | DO8_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init (GPIOG, &GPIO_InitStruct);

  /*Configure GPIO pins : DI4_Pin DI3_Pin DI2_Pin DI1_Pin */
  GPIO_InitStruct.Pin = DI4_Pin | DI3_Pin | DI2_Pin | DI1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init (GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pin : PHY_MASTER_CLOCK_Pin */
  GPIO_InitStruct.Pin = PHY_MASTER_CLOCK_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF0_MCO;
  HAL_GPIO_Init (PHY_MASTER_CLOCK_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : DI8_Pin DI7_Pin DI6_Pin DI5_Pin */
  GPIO_InitStruct.Pin = DI8_Pin | DI7_Pin | DI6_Pin | DI5_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init (GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : LED_NORM_Pin */
  GPIO_InitStruct.Pin = LED_NORM_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init (LED_NORM_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : TEST_LED_Pin */
  GPIO_InitStruct.Pin = TEST_LED_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init (TEST_LED_GPIO_Port, &GPIO_InitStruct);

}
#endif

#ifdef SC4BOPLA

static void MX_GPIO_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};

      /* GPIO Ports Clock Enable */
      __HAL_RCC_GPIOE_CLK_ENABLE();
      __HAL_RCC_GPIOI_CLK_ENABLE();
      __HAL_RCC_GPIOF_CLK_ENABLE();
      __HAL_RCC_GPIOH_CLK_ENABLE();
      __HAL_RCC_GPIOC_CLK_ENABLE();
      __HAL_RCC_GPIOA_CLK_ENABLE();
      __HAL_RCC_GPIOB_CLK_ENABLE();
      __HAL_RCC_GPIOG_CLK_ENABLE();
      __HAL_RCC_GPIOD_CLK_ENABLE();

      /*Configure GPIO pin Output Level */
      HAL_GPIO_WritePin(GPIOA, AMP_ON_Pin|LED_MIC_Pin, GPIO_PIN_RESET);

      /*Configure GPIO pin Output Level */
      HAL_GPIO_WritePin(LED_NORM_GPIO_Port, LED_NORM_Pin, GPIO_PIN_RESET);

      /*Configure GPIO pin Output Level */
      HAL_GPIO_WritePin(TEST_LED_GPIO_Port, TEST_LED_Pin, GPIO_PIN_RESET);

      /*Configure GPIO pins : AMP_ON_Pin LED_MIC_Pin */
      GPIO_InitStruct.Pin = AMP_ON_Pin|LED_MIC_Pin;
      GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
      GPIO_InitStruct.Pull = GPIO_NOPULL;
      GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
      HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

      /*Configure GPIO pin : PHY_MASTER_CLOCK_Pin */
      GPIO_InitStruct.Pin = PHY_MASTER_CLOCK_Pin;
      GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
      GPIO_InitStruct.Pull = GPIO_NOPULL;
      GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
      GPIO_InitStruct.Alternate = GPIO_AF0_MCO;
      HAL_GPIO_Init(PHY_MASTER_CLOCK_GPIO_Port, &GPIO_InitStruct);

      /*Configure GPIO pin : KB_INT_Pin */
      GPIO_InitStruct.Pin = KB_INT_Pin;
      GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
      GPIO_InitStruct.Pull = GPIO_NOPULL;
      HAL_GPIO_Init(KB_INT_GPIO_Port, &GPIO_InitStruct);

      /*Configure GPIO pin : LED_NORM_Pin */
      GPIO_InitStruct.Pin = LED_NORM_Pin;
      GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
      GPIO_InitStruct.Pull = GPIO_NOPULL;
      GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
      HAL_GPIO_Init(LED_NORM_GPIO_Port, &GPIO_InitStruct);

      /*Configure GPIO pin : TEST_LED_Pin */
      GPIO_InitStruct.Pin = TEST_LED_Pin;
      GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
      GPIO_InitStruct.Pull = GPIO_NOPULL;
      GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
      HAL_GPIO_Init(TEST_LED_GPIO_Port, &GPIO_InitStruct);

      HAL_NVIC_SetPriority(EXTI15_10_IRQn, 8, 0);
      HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);
}

#endif

/**
 * @brief System Clock Configuration
 * @retval None
 */
//static void SystemClock_Config (void)
//{
//  RCC_OscInitTypeDef RCC_OscInitStruct =
//    { 0 };
//  RCC_ClkInitTypeDef RCC_ClkInitStruct =
//    { 0 };
//  RCC_PeriphCLKInitTypeDef PeriphClkInitStruct =
//    { 0 };
//
//  /** Supply configuration update enable
//   */
//  HAL_PWREx_ConfigSupply (PWR_LDO_SUPPLY);
//  /** Configure the main internal regulator output voltage
//   */
//  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE0);
//
//  while (!__HAL_PWR_GET_FLAG(PWR_FLAG_VOSRDY))
//    {
//    }
//  /** Macro to configure the PLL clock source
//   */
//  __HAL_RCC_PLL_PLLSOURCE_CONFIG(RCC_PLLSOURCE_HSE);
//  /** Initializes the CPU, AHB and APB busses clocks
//   */
//  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_LSI | RCC_OSCILLATORTYPE_HSE;
//  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
//  RCC_OscInitStruct.LSIState = RCC_LSI_ON;
//  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
//  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
//  RCC_OscInitStruct.PLL.PLLM = 5;
//  RCC_OscInitStruct.PLL.PLLN = 192;
//  RCC_OscInitStruct.PLL.PLLP = 2;
//  RCC_OscInitStruct.PLL.PLLQ = 10;
//  RCC_OscInitStruct.PLL.PLLR = 2;  //60;//2;
//  RCC_OscInitStruct.PLL.PLLRGE = RCC_PLL1VCIRANGE_2;
//  RCC_OscInitStruct.PLL.PLLVCOSEL = RCC_PLL1VCOWIDE;
//  RCC_OscInitStruct.PLL.PLLFRACN = 0;
//  if (HAL_RCC_OscConfig (&RCC_OscInitStruct) != HAL_OK)
//    {
//      Error_Handler ();
//    }
//  /** Initializes the CPU, AHB and APB busses clocks
//   */
//  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1
//      | RCC_CLOCKTYPE_PCLK2 | RCC_CLOCKTYPE_D3PCLK1 | RCC_CLOCKTYPE_D1PCLK1;
//  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
//  RCC_ClkInitStruct.SYSCLKDivider = RCC_SYSCLK_DIV1;
//  RCC_ClkInitStruct.AHBCLKDivider = RCC_HCLK_DIV2;
//  RCC_ClkInitStruct.APB3CLKDivider = RCC_APB3_DIV2;
//  RCC_ClkInitStruct.APB1CLKDivider = RCC_APB1_DIV2;
//  RCC_ClkInitStruct.APB2CLKDivider = RCC_APB2_DIV2;
//  RCC_ClkInitStruct.APB4CLKDivider = RCC_APB4_DIV2;
//
//  if (HAL_RCC_ClockConfig (&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
//    {
//      Error_Handler ();
//    }
//  PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_RTC | RCC_PERIPHCLK_UART7
//      | RCC_PERIPHCLK_RNG | RCC_PERIPHCLK_SAI1 | RCC_PERIPHCLK_I2C2 | RCC_PERIPHCLK_I2C1
//      | RCC_PERIPHCLK_FMC;
//  PeriphClkInitStruct.PLL3.PLL3M = 2;
//  PeriphClkInitStruct.PLL3.PLL3N = 32;
//  PeriphClkInitStruct.PLL3.PLL3P = 49;
//  PeriphClkInitStruct.PLL3.PLL3Q = 2;
//  PeriphClkInitStruct.PLL3.PLL3R = 2;
//  PeriphClkInitStruct.PLL3.PLL3RGE = RCC_PLL3VCIRANGE_3;
//  PeriphClkInitStruct.PLL3.PLL3VCOSEL = RCC_PLL3VCOWIDE;
//  PeriphClkInitStruct.PLL3.PLL3FRACN = 923;
//  PeriphClkInitStruct.FmcClockSelection = RCC_FMCCLKSOURCE_PLL;
//  PeriphClkInitStruct.Sai1ClockSelection = RCC_SAI1CLKSOURCE_PLL3;
//  PeriphClkInitStruct.Usart234578ClockSelection = RCC_USART234578CLKSOURCE_D2PCLK1;
//  PeriphClkInitStruct.RngClockSelection = RCC_RNGCLKSOURCE_PLL;
//  PeriphClkInitStruct.I2c123ClockSelection = RCC_I2C123CLKSOURCE_HSI;			//D2PCLK1;
//  PeriphClkInitStruct.RTCClockSelection = RCC_RTCCLKSOURCE_LSI;
//  if (HAL_RCCEx_PeriphCLKConfig (&PeriphClkInitStruct) != HAL_OK)
//    {
//      Error_Handler ();
//    }
//  HAL_RCC_MCOConfig (RCC_MCO1, RCC_MCO1SOURCE_HSE, RCC_MCODIV_1);
//}

