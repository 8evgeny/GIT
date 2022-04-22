/*!
 * @file wdt.cpp
 *
 */
#include "wdt.h"

#ifdef __cplusplus
extern "C" {
#endif

static WWDG_HandleTypeDef hwwdg;

void WDTInit(void)
{
    /*Check if the system has resumed from WWDG reset ####################*/
    if (__HAL_RCC_GET_FLAG(RCC_FLAG_WWDGRST) != RESET) {

#ifdef STM32F777xx
        /* Insert 4s delay */
        HAL_Delay(4000);
#endif

        /* Clear reset flags */
        __HAL_RCC_CLEAR_RESET_FLAGS();
    }

    hwwdg.Instance = WWDG;

#ifdef STM32F777xx
    hwwdg.Init.Window = 127;
    hwwdg.Init.Prescaler = WWDG_PRESCALER_8;
#else
    hwwdg.Init.Window = 0x7E;
    hwwdg.Init.Prescaler = WWDG_PRESCALER_8;
#endif
    hwwdg.Init.Counter = 0x7E;
    hwwdg.Init.EWIMode = WWDG_EWI_ENABLE;
    if (HAL_WWDG_Init(&hwwdg) != HAL_OK) {
        while (1) {
            Debug::getInstance().dbg << "WWDG Init Error!" << "\n";
        }
    }
}

#ifdef __cplusplus
}
#endif

WDT *WDT::pInstance = nullptr;

void WDT::SetDelayTime(uint32_t time)
{
    time_.store(time);
}

void WDT::Loop()
{
    while (running_) {
        refresh();

#ifdef STM32F777xx
        osDelay(time_);
#else
        osDelay(time_);
#endif

    }
}

void WDT::Stop()
{
    running_ = false;
}

WDT::WDT()
{
    wwdgtHandle = &hwwdg;
    running_ = true;

#ifdef STM32F777xx
    SetDelayTime(22);
#else
    SetDelayTime(44);
#endif
}

void WDT::refresh(void)
{
    if (HAL_WWDG_Refresh(wwdgtHandle) != HAL_OK) {
        Debug::getInstance().dbg << "WWDG Refresh Error!" << "\n";
        while(1);
    }
}

WDT *WDT::getInstance()
{
    if(!pInstance) pInstance = new WDT();
    return pInstance;
}


/**
  * @brief  Start WWDG task
  * @param  pvParameters not used
  * @retval None
  */
void StartWdtThread(void const *argument)
{
    (void)argument;
//    while(1) {
    WDT::getInstance()->Loop();
//        osDelay(15);
//    }
}

#ifdef __cplusplus
extern "C" {
#endif

/*!
* \brief WWDG MSP Initialization
*  This function configures the hardware resources used in this example
* \param hwwdg: WWDG handle pointer
* \retval None
*/
void HAL_WWDG_MspInit(WWDG_HandleTypeDef* hwwdg)
{
    if(hwwdg->Instance==WWDG) {

        /* Peripheral clock enable */
        __HAL_RCC_WWDG_CLK_ENABLE();

        /* WWDG interrupt Init */
        HAL_NVIC_SetPriority(WWDG_IRQn, 4, 0);
        HAL_NVIC_EnableIRQ(WWDG_IRQn);
    }
}

/*!
* \brief WWDG MSP Initialization
*  This function configures the hardware resources used in this example
* \param hwwdg: WWDG handle pointer
* \retval None
*/
void HAL_WWDG_MspDeInit(WWDG_HandleTypeDef* hwwdg)
{
    if(hwwdg->Instance==WWDG) {

        /* Peripheral clock enable */
        __HAL_RCC_WWDG_CLK_DISABLE();

        /* WWDG interrupt Init */
        HAL_NVIC_DisableIRQ(WWDG_IRQn);
    }
}


/*!
  \brief  WWDG early wake up callback
  \param  WwdgHandle: WWDG handle
  \retval None
*/
void HAL_WWDG_EarlyWakeupCallback(WWDG_HandleTypeDef *hwwdg)
{
    if (hwwdg->Instance == WWDG) {
        //Empty
        WDT::getInstance()->refresh();
    }
}

/**
  * \brief This function handles Window watchdog interrupt.
  */
void WWDG_IRQHandler(void)
{
    HAL_WWDG_IRQHandler(&hwwdg);
}

#ifdef __cplusplus
}
#endif
