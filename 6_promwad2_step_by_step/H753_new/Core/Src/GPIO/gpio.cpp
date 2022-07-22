/*!
 \file gpio.cpp

 \authors Androshchuk A.V.

 \code
    GPIOInit();

    ...

 \endcode



*/
#include "gpio.h"

#include <algorithm>
#include <cstring>
#include "rs232.h"
#include "rs232_printf.h"

#ifdef __cplusplus
extern "C" {
#endif

osTimerDef(timer7, timerCallback); /*!< Define the attributes of the timer */

//osMessageQDef(message_q, 1, uint16_t); // Declare a message queue
osMutexDef (mutexRingBufferRx);

//extern osSemaphoreId Netif_LinkSemaphore;
static osTimerId timerId7; /*!< The thread ID of the timer */
constexpr static uint8_t timerDelay = 50;


static GPIO_InitTypeDef GPIO_InitStruct;

/*!
  \brief GPIO Initialization Function
  \param None
  \retval None
  */
void GPIOInit(void)
{

// timerCallback  каллбек
    timerId7 = osTimerCreate( osTimer(timer7), osTimerPeriodic, nullptr); // create timer thread
    if (timerId7)
    {
        osStatus status = osTimerStart (timerId7, timerDelay);   // start timer
        if (status != osOK)  {
            RS232::getInstance().term << "Failed to start [timer]" << "\n";
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

//ИНИЦИАЛИЗАЦИЯ светодиодов
#ifndef SC4
    initLEDS_SC2();
#endif
#ifdef SC4
    initLEDS_SC4();
#endif

    mutexRingBufferRx_id = osMutexCreate(osMutex(mutexRingBufferRx));
    if (mutexRingBufferRx_id == nullptr)
    {
        while(1)
            RS232::getInstance().term << "Failed to create [mutexRingBufferRx]" << "\n";
    }

//ИНИЦИАЛИЗАЦИЯ РЫЧАГОВ (кнопок)
//    message_q_id = osMessageCreate(osMessageQ(message_q), NULL);

#ifndef SC4
    initBUTTONS_SC2();
#endif
#ifdef SC4
    initBUTTONS_SC4();
#endif

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
term1("**** configLed ledNumber") term(ledNumber)
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

osDelay(200);
term("--- switchLEDsThread ---")

    while(true)
    {
        for(uint8_t i = 0; i < 6; ++i)
        {
            if (GPIO::getInstance()->aLeds[i].ledState) // Включаем пин
            {
                if(i == 0) HAL_GPIO_WritePin(GPIOG, GPIO_PIN_6, GPIO_PIN_SET);
                if(i == 1) HAL_GPIO_WritePin(GPIOC, GPIO_PIN_10, GPIO_PIN_SET);
                if(i == 2) HAL_GPIO_WritePin(GPIOG, GPIO_PIN_7, GPIO_PIN_SET);
                if(i == 3) HAL_GPIO_WritePin(GPIOC, GPIO_PIN_11, GPIO_PIN_SET);
                if(i == 4) HAL_GPIO_WritePin(GPIOG, GPIO_PIN_8, GPIO_PIN_SET);
                if(i == 5) HAL_GPIO_WritePin(GPIOC, GPIO_PIN_12, GPIO_PIN_SET);
//                if (i > 2) HAL_GPIO_WritePin(GPIOG, GPIO::getInstance()->aLeds[i].ledPin, GPIO_PIN_SET);
//                else HAL_GPIO_WritePin(GPIOC, GPIO::getInstance()->aLeds[i].ledPin, GPIO_PIN_SET);
            } else
            {
                if(i == 0) HAL_GPIO_WritePin(GPIOG, GPIO_PIN_6, GPIO_PIN_RESET);
                if(i == 1) HAL_GPIO_WritePin(GPIOC, GPIO_PIN_10, GPIO_PIN_RESET);
                if(i == 2) HAL_GPIO_WritePin(GPIOG, GPIO_PIN_7, GPIO_PIN_RESET);
                if(i == 3) HAL_GPIO_WritePin(GPIOC, GPIO_PIN_11, GPIO_PIN_RESET);
                if(i == 4) HAL_GPIO_WritePin(GPIOG, GPIO_PIN_8, GPIO_PIN_RESET);
                if(i == 5) HAL_GPIO_WritePin(GPIOC, GPIO_PIN_12, GPIO_PIN_RESET);
//                if (i > 2) HAL_GPIO_WritePin(GPIOG, GPIO::getInstance()->aLeds[i].ledPin, GPIO_PIN_RESET);
//                else HAL_GPIO_WritePin(GPIOC, GPIO::getInstance()->aLeds[i].ledPin, GPIO_PIN_RESET);
            }

        }
        osDelay(1);
    }
}


[[ noreturn ]]
void readButtonThread(void const *arg)
{
    (void)arg;
    PackageRx tempPack;
    tempPack.packetType = GPIO::getInstance()->button;

osDelay(4000);
term("--- readButtonThread ---")

    while(true)
    {
        for (uint8_t i = 0; i < GPIO::getInstance()->buttonArray.size() ; ++i)
        {
            uint16_t n = GPIO::getInstance()->buttonArray[i].n;
//            k = GPIO::getInstance()->buttonArray[i].i;

            if (HAL_GPIO_ReadPin(GPIOG, GPIO::getInstance()->buttonArray[i].n) == GPIO_PIN_SET)
            {

                osDelay(50);
                if (HAL_GPIO_ReadPin(GPIOG, GPIO::getInstance()->buttonArray[i].n)  == GPIO_PIN_SET)
                {

//term1("Pressed button: ") term(std::to_string(i + 1))

                //Включаю Led
//                if (i < 3 )  HAL_GPIO_WritePin(GPIOG, GPIO::getInstance()->aLeds[i].ledPin, GPIO_PIN_SET);
//                if (i >= 3 ) HAL_GPIO_WritePin(GPIOC, GPIO::getInstance()->aLeds[i ].ledPin, GPIO_PIN_SET);

                    n = GPIO::getInstance()->buttonArray[i].i;
                    tempPack.payloadData = n;

                    osMutexWait(GPIO::getInstance()->mutexRingBufferRx_id, osWaitForever);
                    GPIO::getInstance()->ringBufferRx.push(tempPack);
                    osMutexRelease(GPIO::getInstance()->mutexRingBufferRx_id);

//                    osMessagePut(GPIO::getInstance()->message_q_id, n, osWaitForever);
                }
            }
            else
            {
                //Гашу Led
//                if (i < 3 ) HAL_GPIO_WritePin(GPIOG, GPIO::getInstance()->aLeds[i].ledPin, GPIO_PIN_RESET);
//                if (i >= 3 ) HAL_GPIO_WritePin(GPIOC, GPIO::getInstance()->aLeds[i ].ledPin, GPIO_PIN_RESET);
            }

        }
        osDelay(1);
    }

}


#ifdef __cplusplus
}
#endif

void GPIO::initLEDS_SC2()
{
//    for (uint8_t i = 0, j = 6; i < 6; i++, j++) {
//        if (i == 3) j = 10;
//        aLeds[i].ledPin = aPin[j];
//    }
    aLeds[0].ledPin = GPIO_PIN_6;
    aLeds[1].ledPin = GPIO_PIN_10;
    aLeds[2].ledPin = GPIO_PIN_7;
    aLeds[3].ledPin = GPIO_PIN_11;
    aLeds[4].ledPin = GPIO_PIN_8;
    aLeds[5].ledPin = GPIO_PIN_12;
}

void GPIO::initLEDS_SC4()
{

}

void GPIO::initBUTTONS_SC2()
{
    buttonArray[0].i = 1;     buttonArray[0].n = GPIO_PIN_11;
    buttonArray[1].i = 2;     buttonArray[1].n = GPIO_PIN_12;
    buttonArray[2].i = 3;     buttonArray[2].n = GPIO_PIN_10;
    buttonArray[3].i = 4;     buttonArray[3].n = GPIO_PIN_13;
    buttonArray[4].i = 5;     buttonArray[4].n = GPIO_PIN_9;
    buttonArray[5].i = 6;     buttonArray[5].n = GPIO_PIN_14;
}

void GPIO::initBUTTONS_SC4()
{

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
        if (GPIO_Pin == GPIO_PIN_5)
        {
            HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9, GPIO_PIN_SET);
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

//        if (GPIO_Pin == GPIO_PIN_2) {
//            osSemaphoreRelease(Netif_LinkSemaphore);
//        }

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
