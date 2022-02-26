/*!
 \file gpio.cpp

 \authors Androshchuk A.V.

 \code
    GPIOInit();

    ...

 \endcode



*/
#include "gpio_sc2_board.h"
//#include "gpio_stm32f7xx.h"
#include <algorithm>
#include <cstring>
#include "../Debug/debug.h"

#ifdef __cplusplus
extern "C" {
#endif

osThreadDef(switchLEDsThread, switchLEDsThread, osPriorityNormal, 0, configMINIMAL_STACK_SIZE * 2);
osThreadDef(readButtonThread, readButtonThread, osPriorityNormal, 0, configMINIMAL_STACK_SIZE * 5);
osTimerDef(timer7, timerCallback); /*!< Define the attributes of the timer */
//osMessageQDef(message_q, 1, uint16_t); // Declare a message queue
osMutexDef (mutexRingBufferRx);

extern osSemaphoreId Netif_LinkSemaphore;
static osTimerId timerId7; /*!< The thread ID of the timer */
constexpr static uint8_t timerDelay = 50;


static GPIO_InitTypeDef GPIO_InitStruct;
static uint16_t aPin[16] = { GPIO_PIN_0, GPIO_PIN_1,
                             GPIO_PIN_2, GPIO_PIN_3,
                             GPIO_PIN_4, GPIO_PIN_5,
                             GPIO_PIN_6, GPIO_PIN_7,
                             GPIO_PIN_8, GPIO_PIN_9,
                             GPIO_PIN_10, GPIO_PIN_11,
                             GPIO_PIN_12, GPIO_PIN_13,
                             GPIO_PIN_14, GPIO_PIN_15
                           };

/*!
  \brief GPIO Initialization Function
  \param None
  \retval None
  */
void GPIOInit(void)
{
    /*##-1- Enable GPIO Clocks #################################*/
    /* Enable GPIO clock */
    if (!READ_BIT(RCC->AHB1ENR, RCC_AHB1ENR_GPIOCEN)) __HAL_RCC_GPIOC_CLK_ENABLE();
    if (!READ_BIT(RCC->AHB1ENR, RCC_AHB1ENR_GPIOBEN)) __HAL_RCC_GPIOB_CLK_ENABLE();
    if (!READ_BIT(RCC->AHB1ENR, RCC_AHB1ENR_GPIOAEN)) __HAL_RCC_GPIOA_CLK_ENABLE();
    if (!READ_BIT(RCC->AHB1ENR, RCC_AHB1ENR_GPIOGEN)) __HAL_RCC_GPIOG_CLK_ENABLE();
    if (!READ_BIT(RCC->AHB1ENR, RCC_AHB1ENR_GPIOFEN)) __HAL_RCC_GPIOF_CLK_ENABLE();

    /*##-2- Configure GPIO for CFG ##########################################*/
    /* GPIO pin configuration  */
    GPIO_InitStruct.Pin       = GPIO_PIN_10 | GPIO_PIN_11 | GPIO_PIN_12 | //CFG resistors
                                GPIO_PIN_6 | GPIO_PIN_7 | GPIO_PIN_8 | GPIO_PIN_9 ; //DETEDCTION
    GPIO_InitStruct.Mode      = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull      = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

//    /*##-3- Configure GPIO for volume buttons ##########################################*/
//    /* GPIO pin configuration  */
//    GPIO_InitStruct.Pin       = GPIO_PIN_14 | GPIO_PIN_15;
//    GPIO_InitStruct.Mode      = GPIO_MODE_IT_FALLING;
//    GPIO_InitStruct.Pull      = GPIO_NOPULL;
//    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    /*##-4- Configure GPIO for test button ##########################################*/
    /* GPIO pin configuration  */
    GPIO_InitStruct.Pin       = GPIO_PIN_11;
    GPIO_InitStruct.Mode      = GPIO_MODE_IT_FALLING;
    GPIO_InitStruct.Pull      = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);

    /*##-5- Configure GPIO for test LED ##########################################*/
    /* GPIO pin configuration  */
    GPIO_InitStruct.Pin       = GPIO_PIN_13;
    GPIO_InitStruct.Mode      = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull      = GPIO_NOPULL;
    GPIO_InitStruct.Speed     = GPIO_SPEED_LOW;
    HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);

    HAL_GPIO_WritePin(GPIOG, GPIO_PIN_13, GPIO_PIN_RESET);

    /*##-5- Configure GPIO for test LED ##########################################*/
    /* GPIO pin configuration  */
    GPIO_InitStruct.Pin       = GPIO_PIN_6 |  GPIO_PIN_7 | GPIO_PIN_8;
    GPIO_InitStruct.Mode      = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull      = GPIO_NOPULL;
    GPIO_InitStruct.Speed     = GPIO_SPEED_LOW;
    HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);

    /*##-5- Configure GPIO for test LED ##########################################*/
    /* GPIO pin configuration  */
    GPIO_InitStruct.Pin       = GPIO_PIN_10 |  GPIO_PIN_11 | GPIO_PIN_12 |   // Buttons
                                GPIO_PIN_9; // BASE_DET
    GPIO_InitStruct.Mode      = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull      = GPIO_NOPULL;
    GPIO_InitStruct.Speed     = GPIO_SPEED_LOW;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /*##-5- Configure GPIO for test LED ##########################################*/
    /* GPIO pin configuration  */
    GPIO_InitStruct.Pin       = GPIO_PIN_4 |  GPIO_PIN_5 | GPIO_PIN_6;    // LEDs
    GPIO_InitStruct.Mode      = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull      = GPIO_NOPULL;
    GPIO_InitStruct.Speed     = GPIO_SPEED_LOW;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6, GPIO_PIN_SET);

    /*##-5- Configure GPIO for test LED ##########################################*/
    /* GPIO pin configuration  */
    GPIO_InitStruct.Pin       = GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10; //LEDs
    GPIO_InitStruct.Mode      = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull      = GPIO_NOPULL;
    GPIO_InitStruct.Speed     = GPIO_SPEED_LOW;
    HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);

    HAL_GPIO_WritePin(GPIOF, GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10, GPIO_PIN_SET);

    GPIO_InitStruct.Pin = GPIO_PIN_2;
    GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    /* EXTI interrupt init*/
    HAL_NVIC_SetPriority(EXTI2_IRQn, 5, 0);
    HAL_NVIC_EnableIRQ(EXTI2_IRQn);

    HAL_NVIC_SetPriority(EXTI9_5_IRQn, 5, 0);
    HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);

    HAL_NVIC_SetPriority(EXTI15_10_IRQn, 5, 0);
    HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);

    if ((osThreadCreate(osThread(switchLEDsThread), nullptr)) == nullptr) {
        Debug::getInstance().dbg << "Failed to create [switchLEDsThread]" << "\n";
    }

    if ((osThreadCreate(osThread(readButtonThread), nullptr)) == nullptr) {
        Debug::getInstance().dbg << "Failed to create [readButtonThread]" << "\n";
    }

    timerId7 = osTimerCreate( osTimer(timer7), osTimerPeriodic, nullptr); // create timer thread
    if (timerId7) {
        osStatus status = osTimerStart (timerId7, timerDelay);   // start timer
        if (status != osOK)  {
            Debug::getInstance().dbg << "Failed to start [timer]" << "\n";
            while(1);
        }
    }

}

#ifdef __cplusplus
}
#endif


GPIO::GPIO()
{
    gpioInit = & GPIO_InitStruct;

    initLEDs();

    mutexRingBufferRx_id = osMutexCreate(osMutex(mutexRingBufferRx));
    if (mutexRingBufferRx_id == nullptr) {
        while(1)
            Debug::getInstance().dbg << "Failed to create [mutexRingBufferRx]" << "\n";
    }

//    message_q_id = osMessageCreate(osMessageQ(message_q), NULL);
    for (uint8_t var = 6, j = 0; var < 13; ++var, ++j) {

        if (var == 9) {
            --j;
            continue;
        }
        sPinArray[j].i = j + 1;
        sPinArray[j].n = aPin[var];
    }

    uint16_t temp = sPinArray.at(0).n;
    sPinArray.at(0).n = sPinArray.at(2).n;
    sPinArray.at(2).n = temp;

}


GPIO *GPIO::getInstance()
{
    if (!p_instance) p_instance = new GPIO();
    return p_instance;
}

GPIO *GPIO::p_instance = nullptr;

/*!
  \brief Function of getting board configuration
  \param None
  \retval None
  */
uint16_t GPIO::getCFG(void)
{
    uint16_t dataCFG = 0;

    dataCFG |= HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_10) << 2;
    dataCFG |= HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_11) << 1;
    dataCFG |= HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_12) << 0;

    return dataCFG;
}

/*!
 * \brief Volume up function
 * \param none
 * \return none
 */
void GPIO::upVolume(void)
{
    //Empty
}

/*!
 * \brief Volume down function
 * \param none
 * \return none
 */
void GPIO::downVolume(void)
{
    //Empty
}

/*!
  \brief GPIO Test Function
  \param None
  \retval None
  */
void GPIO::test(void)
{

}

void GPIO::configLed(uint8_t ledNumber,
                     bool ledOn,
                     uint32_t timeOn,
                     uint32_t timeOff,
                     uint8_t repeatNum)
{
    --ledNumber;
    if (timeOn < 50 && timeOff < 50) {
        aLeds[ledNumber].ledState = ledOn;
        aLeds[ledNumber].timeStart = false;
        aLeds[ledNumber].timeOn = timeOn;
        aLeds[ledNumber].timeOff = timeOff;
    } else {
        aLeds[ledNumber].timeOn = timeOn;
        aLeds[ledNumber].timeOff = timeOff;
        aLeds[ledNumber].timeStart = ledOn;
        aLeds[ledNumber].ledState = ledOn;
        if (repeatNum > 0)
            aLeds[ledNumber].reiterationNum = repeatNum * 2;
    }
}

#ifdef __cplusplus
extern "C" {
#endif

void timerCallback(void const *arg)
{
    (void)arg;
    for (uint8_t i = 0; i < 6; i++) {
        if (GPIO::getInstance()->aLeds[i].timeStart) {
            GPIO::getInstance()->aLeds[i].count += 1;
            if(GPIO::getInstance()->aLeds[i].ledState == false && GPIO::getInstance()->aLeds[i].count >= GPIO::getInstance()->aLeds[i].timeOff/timerDelay) {
                GPIO::getInstance()->aLeds[i].ledState = true;
                GPIO::getInstance()->aLeds[i].count = 0;
                if (GPIO::getInstance()->aLeds[i].reiterationNum > 0)
                    GPIO::getInstance()->aLeds[i].reiterationNum -= 1;
                if (GPIO::getInstance()->aLeds[i].reiterationNum == 0) {
                    GPIO::getInstance()->aLeds[i].timeStart = false;
                    GPIO::getInstance()->aLeds[i].reiterationNum -= 1;
                }
            } else if (GPIO::getInstance()->aLeds[i].ledState == true && GPIO::getInstance()->aLeds[i].count >= GPIO::getInstance()->aLeds[i].timeOn/timerDelay) {
                GPIO::getInstance()->aLeds[i].ledState = false;
                GPIO::getInstance()->aLeds[i].count = 0;
                if (GPIO::getInstance()->aLeds[i].reiterationNum > 0)
                    GPIO::getInstance()->aLeds[i].reiterationNum -= 1;
                if (GPIO::getInstance()->aLeds[i].reiterationNum == 0) {
                    GPIO::getInstance()->aLeds[i].timeStart = false;
                    GPIO::getInstance()->aLeds[i].reiterationNum -= 1;
                }
            }
        }
    }
}

[[ noreturn ]]
void switchLEDsThread(void const *arg)
{
    (void)arg;
    while(true) {
        for(uint8_t i = 0; i < 6; ++i) {

            if (GPIO::getInstance()->aLeds[i].ledState) {
                if (i > 2) HAL_GPIO_WritePin(GPIOF, GPIO::getInstance()->aLeds[i].ledPin, GPIO_PIN_RESET);
                else HAL_GPIO_WritePin(GPIOA, GPIO::getInstance()->aLeds[i].ledPin, GPIO_PIN_RESET);
            } else {
                if (i > 2) HAL_GPIO_WritePin(GPIOF, GPIO::getInstance()->aLeds[i].ledPin, GPIO_PIN_SET);
                else HAL_GPIO_WritePin(GPIOA, GPIO::getInstance()->aLeds[i].ledPin, GPIO_PIN_SET);
            }
        }
        osDelay(1);
    }
}

[[ noreturn ]]
void readButtonThread(void const *arg)
{
    (void)arg;
    uint8_t a = 1;
    PackageRx tempPack;
    tempPack.packetType = GPIO::getInstance()->button;

    GPIO_TypeDef * arrPortId[2] = {GPIOG, GPIOA};
    while(true) {
        uint8_t portId = 0;
        for (uint8_t i = 0; i < GPIO::getInstance()->sPinArray.size() ; ++i) {

            if (i > 2)  portId = 1;
            uint16_t n = GPIO::getInstance()->sPinArray[i].n,
                     k = GPIO::getInstance()->sPinArray[i].i;

            if (HAL_GPIO_ReadPin(arrPortId[portId], GPIO::getInstance()->sPinArray[i].n) == GPIO_PIN_SET) {
                osDelay(50);
                if (HAL_GPIO_ReadPin(arrPortId[portId], GPIO::getInstance()->sPinArray[i].n)  == GPIO_PIN_SET) {

                    n = GPIO::getInstance()->sPinArray[i].i;

                    tempPack.payloadData = n;

                    osMutexWait(GPIO::getInstance()->mutexRingBufferRx_id, osWaitForever);
                    GPIO::getInstance()->ringBufferRx.push(tempPack);
                    osMutexRelease(GPIO::getInstance()->mutexRingBufferRx_id);

//                    osMessagePut(GPIO::getInstance()->message_q_id, n, osWaitForever);
                }
            }

        }
        osDelay(1);
    }

}


#ifdef __cplusplus
}
#endif

void GPIO::initLEDs()
{
    for (uint8_t i = 0, j = 4; i < 6; i++, j++) {
        if (i == 3) j++;
        aLeds[i].ledPin = aPin[j];
    }
}

#ifdef __cplusplus
extern "C" {
#endif
/*!
  * @brief EXTI line detection callbacks
  * @param GPIO_Pin: Specifies the pins connected EXTI line
  * @retval None
  */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    if (GPIO_Pin == GPIO_PIN_11) {
        HAL_GPIO_WritePin(GPIOG, GPIO_PIN_13, GPIO_PIN_SET);
        GPIO::getInstance()->testFlag = true;
        osSignalSet(GPIO::getInstance()->createTestTaskThreadId, 0x03);
    }

//    else if (GPIO_Pin == GPIO_PIN_14) {
//        if (GPIO::getInstance()->dacDriverGainValue < 29)
//            ++GPIO::getInstance()->dacDriverGainValue;
//    } else if (GPIO_Pin == GPIO_PIN_15) {
//        if (GPIO::getInstance()->dacDriverGainValue > -6)
//            --GPIO::getInstance()->dacDriverGainValue;
//    }

    if (GPIO_Pin == GPIO_PIN_2) {
        osSemaphoreRelease(Netif_LinkSemaphore);
    }

}

void EXTI2_IRQHandler(void)
{
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_2);
}
/*!
  * @brief  This function handles external lines 15 to 10 interrupt request.
  * @param  None
  * @retval None
  */
void EXTI15_10_IRQHandler(void)
{
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_11);
//    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_14);
//    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_15);
}
#ifdef __cplusplus
}
#endif
