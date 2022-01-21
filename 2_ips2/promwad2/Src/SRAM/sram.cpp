
#include "sram.h"
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

SRAM_HandleTypeDef hsram1;
DMA_HandleTypeDef hdma_memtomem_dma2_stream0;
static void DMAInit(void);

/*!
  \brief FMC Initialization Function
  \param None
  \retval None
  */
/* FMC initialization function */
void SRAMInit(void)
{
    DMAInit();
    FMC_NORSRAM_TimingTypeDef Timing = {0};

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
    hsram1.Init.ContinuousClock = FMC_CONTINUOUS_CLOCK_SYNC_ASYNC;
    hsram1.Init.WriteFifo = FMC_WRITE_FIFO_DISABLE;
    hsram1.Init.PageSize = FMC_PAGE_SIZE_NONE;
    hsram1.hdma = &hdma_memtomem_dma2_stream0;
    /* Timing */
    Timing.AddressSetupTime = 2;
    Timing.AddressHoldTime = 1;
    Timing.DataSetupTime = 2;

    if (HAL_SRAM_Init(&hsram1, &Timing, NULL) != HAL_OK) {
        while (1) {
            Debug::getInstance().dbg << "SRAM Init Error!" << "\n";
        }
    }


    HAL_Delay(40);
}

/*!
  \brief SRAM DMA Initialization Function
  \param None
  \retval None
  */
static void DMAInit(void)
{
    /* DMA controller clock enable */
    __HAL_RCC_DMA2_CLK_ENABLE();

    /* Configure DMA request hdma_memtomem_dma2_stream0 on DMA2_Stream0 */
    hdma_memtomem_dma2_stream0.Instance = DMA2_Stream0;
    hdma_memtomem_dma2_stream0.Init.Channel = DMA_CHANNEL_0;
    hdma_memtomem_dma2_stream0.Init.Direction = DMA_MEMORY_TO_MEMORY;
    hdma_memtomem_dma2_stream0.Init.PeriphInc = DMA_PINC_ENABLE;
    hdma_memtomem_dma2_stream0.Init.MemInc = DMA_MINC_ENABLE;
    hdma_memtomem_dma2_stream0.Init.PeriphDataAlignment = DMA_PDATAALIGN_WORD;
    hdma_memtomem_dma2_stream0.Init.MemDataAlignment = DMA_MDATAALIGN_WORD;
    hdma_memtomem_dma2_stream0.Init.Mode = DMA_NORMAL;
    hdma_memtomem_dma2_stream0.Init.Priority = DMA_PRIORITY_HIGH;
    hdma_memtomem_dma2_stream0.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
    hdma_memtomem_dma2_stream0.Init.FIFOThreshold = DMA_FIFO_THRESHOLD_FULL;
    hdma_memtomem_dma2_stream0.Init.MemBurst = DMA_MBURST_SINGLE;
    hdma_memtomem_dma2_stream0.Init.PeriphBurst = DMA_PBURST_SINGLE;

    if (HAL_DMA_Init(&hdma_memtomem_dma2_stream0) != HAL_OK) {
        while (1) {
            Debug::getInstance().dbg << "SRAM DMA Init Error!" << "\n";
        }
    }

    HAL_NVIC_SetPriority(DMA2_Stream0_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(DMA2_Stream0_IRQn);

}

#ifdef __cplusplus
}
#endif

SRAM::SRAM()
{
    sram1Handle = &hsram1;
}
SRAM *SRAM::getInstance()
{
    if(!pInstance) pInstance = new SRAM();
    return pInstance;
}

SRAM *SRAM::pInstance = nullptr;

HAL_StatusTypeDef SRAM::readData(uint32_t *dataBuf, size_t bufSize, uint32_t *addr)
{
    HAL_StatusTypeDef status = HAL_SRAM_Read_DMA(sram1Handle, addr, dataBuf, bufSize);
    while (sramDataReady != SET);
    sramDataReady = RESET;
    return status;
}

HAL_StatusTypeDef SRAM::writeData(uint32_t *data, size_t dataSize, uint32_t *addr)
{
    HAL_StatusTypeDef status = HAL_SRAM_Write_DMA(sram1Handle, addr, data, dataSize);
    while (sramDataReady != SET);
    sramDataReady = RESET;
    return status;
}

#ifdef __cplusplus
extern "C" {
#endif

static uint32_t FMC_Initialized = 0;

static void HAL_FMC_MspInit(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    if (FMC_Initialized) {
        return;
    }
    FMC_Initialized = 1;
    /* Peripheral clock enable */
    __HAL_RCC_FMC_CLK_ENABLE();

    __HAL_RCC_GPIOD_CLK_ENABLE();
    __HAL_RCC_GPIOE_CLK_ENABLE();
    __HAL_RCC_GPIOF_CLK_ENABLE();
    __HAL_RCC_GPIOG_CLK_ENABLE();
    __HAL_RCC_GPIOH_CLK_ENABLE();
    __HAL_RCC_GPIOI_CLK_ENABLE();

    /** FMC GPIO Configuration
    PI9   ------> FMC_D30
    PI10   ------> FMC_D31
    PF0   ------> FMC_A0
    PF1   ------> FMC_A1
    PF2   ------> FMC_A2
    PF3   ------> FMC_A3
    PF4   ------> FMC_A4
    PF5   ------> FMC_A5
    PF12   ------> FMC_A6
    PF13   ------> FMC_A7
    PF14   ------> FMC_A8
    PF15   ------> FMC_A9
    PG0   ------> FMC_A10
    PG1   ------> FMC_A11
    PE7   ------> FMC_D4
    PE8   ------> FMC_D5
    PE9   ------> FMC_D6
    PE10   ------> FMC_D7
    PE11   ------> FMC_D8
    PE12   ------> FMC_D9
    PE13   ------> FMC_D10
    PE14   ------> FMC_D11
    PE15   ------> FMC_D12
    PH8   ------> FMC_D16
    PH9   ------> FMC_D17
    PH10   ------> FMC_D18
    PH11   ------> FMC_D19
    PH12   ------> FMC_D20
    PD8   ------> FMC_D13
    PD9   ------> FMC_D14
    PD10   ------> FMC_D15
    PD11   ------> FMC_A16
    PD12   ------> FMC_A17
    PD14   ------> FMC_D0
    PD15   ------> FMC_D1
    PG2   ------> FMC_A12
    PG3   ------> FMC_A13
    PG4   ------> FMC_A14
    PG5   ------> FMC_A15
    PH13   ------> FMC_D21
    PH14   ------> FMC_D22
    PH15   ------> FMC_D23
    PI0   ------> FMC_D24
    PI1   ------> FMC_D25
    PI2   ------> FMC_D26
    PI3   ------> FMC_D27
    PD0   ------> FMC_D2
    PD1   ------> FMC_D3
    PD4   ------> FMC_NOE
    PD5   ------> FMC_NWE
    PD7   ------> FMC_NE1
    PG9   ------> FMC_NE2
    PE0   ------> FMC_NBL0
    PE1   ------> FMC_NBL1
    PI4   ------> FMC_NBL2
    PI5   ------> FMC_NBL3
    PI6   ------> FMC_D28
    PI7   ------> FMC_D29
    */
    GPIO_InitStruct.Pin = GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_0|GPIO_PIN_1
                          |GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5
                          |GPIO_PIN_6|GPIO_PIN_7;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF12_FMC;
    HAL_GPIO_Init(GPIOI, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3
                          |GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_12|GPIO_PIN_13
                          |GPIO_PIN_14|GPIO_PIN_15;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF12_FMC;
    HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3
                          |GPIO_PIN_4|GPIO_PIN_5/*|GPIO_PIN_9*/;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF12_FMC;
    HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_7|GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10
                          |GPIO_PIN_11|GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14
                          |GPIO_PIN_15|GPIO_PIN_0|GPIO_PIN_1;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF12_FMC;
    HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_11
                          |GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF12_FMC;
    HAL_GPIO_Init(GPIOH, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_11
                          |GPIO_PIN_12|GPIO_PIN_14|GPIO_PIN_15|GPIO_PIN_0
                          |GPIO_PIN_1|GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_7;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF12_FMC;
    HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);
}

void HAL_SRAM_MspInit(SRAM_HandleTypeDef* hsram)
{
    HAL_FMC_MspInit();

}

static uint32_t FMC_DeInitialized = 0;

static void HAL_FMC_MspDeInit(void)
{
    if (FMC_DeInitialized) {
        return;
    }
    FMC_DeInitialized = 1;
    /* Peripheral clock enable */
    __HAL_RCC_FMC_CLK_DISABLE();

    __HAL_RCC_GPIOD_CLK_DISABLE();
    __HAL_RCC_GPIOE_CLK_DISABLE();
    __HAL_RCC_GPIOF_CLK_DISABLE();
    __HAL_RCC_GPIOG_CLK_DISABLE();
    __HAL_RCC_GPIOH_CLK_DISABLE();
    __HAL_RCC_GPIOI_CLK_DISABLE();

    /** FMC GPIO Configuration
    PI9   ------> FMC_D30
    PI10   ------> FMC_D31
    PF0   ------> FMC_A0
    PF1   ------> FMC_A1
    PF2   ------> FMC_A2
    PF3   ------> FMC_A3
    PF4   ------> FMC_A4
    PF5   ------> FMC_A5
    PF12   ------> FMC_A6
    PF13   ------> FMC_A7
    PF14   ------> FMC_A8
    PF15   ------> FMC_A9
    PG0   ------> FMC_A10
    PG1   ------> FMC_A11
    PE7   ------> FMC_D4
    PE8   ------> FMC_D5
    PE9   ------> FMC_D6
    PE10   ------> FMC_D7
    PE11   ------> FMC_D8
    PE12   ------> FMC_D9
    PE13   ------> FMC_D10
    PE14   ------> FMC_D11
    PE15   ------> FMC_D12
    PH8   ------> FMC_D16
    PH9   ------> FMC_D17
    PH10   ------> FMC_D18
    PH11   ------> FMC_D19
    PH12   ------> FMC_D20
    PD8   ------> FMC_D13
    PD9   ------> FMC_D14
    PD10   ------> FMC_D15
    PD11   ------> FMC_A16
    PD12   ------> FMC_A17
    PD14   ------> FMC_D0
    PD15   ------> FMC_D1
    PG2   ------> FMC_A12
    PG3   ------> FMC_A13
    PG4   ------> FMC_A14
    PG5   ------> FMC_A15
    PH13   ------> FMC_D21
    PH14   ------> FMC_D22
    PH15   ------> FMC_D23
    PI0   ------> FMC_D24
    PI1   ------> FMC_D25
    PI2   ------> FMC_D26
    PI3   ------> FMC_D27
    PD0   ------> FMC_D2
    PD1   ------> FMC_D3
    PD4   ------> FMC_NOE
    PD5   ------> FMC_NWE
    PD7   ------> FMC_NE1
    PG9   ------> FMC_NE2
    PE0   ------> FMC_NBL0
    PE1   ------> FMC_NBL1
    PI4   ------> FMC_NBL2
    PI5   ------> FMC_NBL3
    PI6   ------> FMC_D28
    PI7   ------> FMC_D29
    */
    HAL_GPIO_DeInit(GPIOI, GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_0|GPIO_PIN_1
                    |GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5
                    |GPIO_PIN_6|GPIO_PIN_7);

    HAL_GPIO_DeInit(GPIOF, GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3
                    |GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_12|GPIO_PIN_13
                    |GPIO_PIN_14|GPIO_PIN_15);

    HAL_GPIO_DeInit(GPIOG, GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3
                    |GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_9);

    HAL_GPIO_DeInit(GPIOE, GPIO_PIN_7|GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10
                    |GPIO_PIN_11|GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14
                    |GPIO_PIN_15|GPIO_PIN_0|GPIO_PIN_1);

    HAL_GPIO_DeInit(GPIOH, GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_11
                    |GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15);

    HAL_GPIO_DeInit(GPIOD, GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_11
                    |GPIO_PIN_12|GPIO_PIN_14|GPIO_PIN_15|GPIO_PIN_0
                    |GPIO_PIN_1|GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_7);
}

void HAL_SRAM_MspDeInit(SRAM_HandleTypeDef* hsram)
{
    HAL_FMC_MspDeInit();
}

void DMA2_Stream0_IRQHandler(void)
{
    HAL_DMA_IRQHandler(hsram1.hdma);
    SRAM::getInstance()->sramDataReady = SET;
}

#ifdef __cplusplus
}
#endif
