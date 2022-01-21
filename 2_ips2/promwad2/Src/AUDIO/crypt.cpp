#include "sai.h"
#include "../Debug/debug.h"

#ifdef __cplusplus
extern "C" {
#endif

extern DMA_HandleTypeDef hdmaCrypIn;
extern DMA_HandleTypeDef hdmaCrypOut;
extern CRYP_HandleTypeDef hcryp;

/**
* @brief CRYP MSP Initialization
* This function configures the hardware resources used in this example
* @param hcryp: CRYP handle pointer
* @retval None
*/
void HAL_CRYP_MspInit(CRYP_HandleTypeDef *hcryp)
{
    if (hcryp->Instance == CRYP) {
        /* Peripheral clock enable */
        __HAL_RCC_CRYP_CLK_ENABLE();

        /* Enable DMA2 clocks */
        __HAL_RCC_DMA2_CLK_ENABLE();

        /* CRYP DMA Init */
        /* CRYP_IN Init */
        hdmaCrypIn.Instance = DMA2_Stream6;
        hdmaCrypIn.Init.Channel = DMA_CHANNEL_2;
        hdmaCrypIn.Init.Direction = DMA_MEMORY_TO_PERIPH;
        hdmaCrypIn.Init.PeriphInc = DMA_PINC_DISABLE;
        hdmaCrypIn.Init.MemInc = DMA_MINC_ENABLE;
        hdmaCrypIn.Init.PeriphDataAlignment = DMA_PDATAALIGN_WORD;
        hdmaCrypIn.Init.MemDataAlignment = DMA_MDATAALIGN_WORD;
        hdmaCrypIn.Init.Mode = DMA_NORMAL;
        hdmaCrypIn.Init.Priority = DMA_PRIORITY_LOW;
        hdmaCrypIn.Init.FIFOMode = DMA_FIFOMODE_DISABLE;

//        hdmaCrypIn.Init.FIFOThreshold = DMA_FIFO_THRESHOLD_FULL;
//        hdmaCrypIn.Init.MemBurst = DMA_MBURST_INC16;
//        hdmaCrypIn.Init.PeriphBurst  = DMA_MBURST_INC16;
        if (HAL_DMA_Init(&hdmaCrypIn) != HAL_OK) {
            Debug::getInstance().dbg << __FUNCTION__ << " " << __LINE__ << "\n";
        }

        __HAL_LINKDMA(hcryp, hdmain, hdmaCrypIn);

        /* CRYP_OUT Init */
        hdmaCrypOut.Instance = DMA2_Stream5;
        hdmaCrypOut.Init.Channel = DMA_CHANNEL_2;
        hdmaCrypOut.Init.Direction = DMA_PERIPH_TO_MEMORY;
        hdmaCrypOut.Init.PeriphInc = DMA_PINC_DISABLE;
        hdmaCrypOut.Init.MemInc = DMA_MINC_ENABLE;
        hdmaCrypOut.Init.PeriphDataAlignment = DMA_PDATAALIGN_WORD;
        hdmaCrypOut.Init.MemDataAlignment = DMA_MDATAALIGN_WORD;
        hdmaCrypOut.Init.Mode = DMA_NORMAL;
        hdmaCrypOut.Init.Priority = DMA_PRIORITY_LOW;
        hdmaCrypOut.Init.FIFOMode = DMA_FIFOMODE_DISABLE;

//        hdmaCrypOut.Init.FIFOThreshold = DMA_FIFO_THRESHOLD_FULL;
//        hdmaCrypOut.Init.MemBurst = DMA_MBURST_INC16;
//        hdmaCrypOut.Init.PeriphBurst  = DMA_MBURST_INC16;
        if (HAL_DMA_Init(&hdmaCrypOut) != HAL_OK) {
            Debug::getInstance().dbg << __FUNCTION__ << " " << __LINE__ << "\n";
        }

        __HAL_LINKDMA(hcryp, hdmaout, hdmaCrypOut);

        /* CRYP interrupt Init */
        HAL_NVIC_SetPriority(CRYP_IRQn, 5, 0);
        HAL_NVIC_EnableIRQ(CRYP_IRQn);
    }
}

/**
* @brief CRYP MSP De-Initialization
* This function freeze the hardware resources used in this example
* @param hcryp: CRYP handle pointer
* @retval None
*/
void HAL_CRYP_MspDeInit(CRYP_HandleTypeDef *hcryp)
{
    if (hcryp->Instance == CRYP) {
        /* Peripheral clock disable */
        __HAL_RCC_CRYP_CLK_DISABLE();

        /* CRYP DMA DeInit */
        HAL_DMA_DeInit(hcryp->hdmain);
        HAL_DMA_DeInit(hcryp->hdmaout);

        /* CRYP interrupt DeInit */
        HAL_NVIC_DisableIRQ(CRYP_IRQn);
    }
}

/**
  * @brief This function handles DMA2 stream5 global interrupt.
  */
void DMA2_Stream5_IRQHandler(void)
{
    HAL_DMA_IRQHandler(hcryp.hdmaout);
}

/**
  * @brief This function handles DMA2 stream6 global interrupt.
  */
void DMA2_Stream6_IRQHandler(void)
{
    HAL_DMA_IRQHandler(hcryp.hdmain);
}

/**
  * @brief This function handles CRYP global interrupt.
  */
void CRYP_IRQHandler(void)
{
    HAL_CRYP_IRQHandler(&hcryp);
}

void HAL_CRYP_InCpltCallback(CRYP_HandleTypeDef *hcryp)
{
    if (hcryp->Instance == CRYP) {
        SAI::getInstance()->getInstance()->cryptRxComplete = true;
    }
}

void HAL_CRYP_OutCpltCallback(CRYP_HandleTypeDef *hcryp)
{
    if (hcryp->Instance == CRYP) {
        SAI::getInstance()->getInstance()->cryptTxComplete = true;
    }
}

#ifdef __cplusplus
}
#endif

