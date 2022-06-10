/*!
 \file gpio_stm32f1xx.cpp
*/
#include "gpio_stm32f1xx.h"
#include "can_stm32f1xx.h"
//#include "command.h"
#include <cstring>
#include "../Debug/debug.h"
#include "wdt.h"

#ifdef __cplusplus
extern "C" {
#endif

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

static uint8_t arrayButtons[8][4] = {

    {1, 2, 3, 4},       // r0
    {5, 6, 7, 8},       // r1
    {9, 10, 11, 12},    // r2
    {13, 14, 15, 16},   // r3
    {17, 18, 19, 20},   // r4
    {21, 22, 23, 24},   // r5
    {25, 26, 27, 28},   // r6
    {29, 30, 31, 32}    // r7

    // c0  c1  c2  c3
};

void GPIOInit(void)
{
    /*##-1- Enable GPIO Clocks #################################*/
    /* Enable GPIO clock */
    if (!READ_BIT(RCC->APB2ENR, RCC_APB2ENR_IOPCEN)) __HAL_RCC_GPIOC_CLK_ENABLE();
    if (!READ_BIT(RCC->APB2ENR, RCC_APB2ENR_IOPBEN)) __HAL_RCC_GPIOB_CLK_ENABLE();
    if (!READ_BIT(RCC->APB2ENR, RCC_APB2ENR_IOPAEN)) __HAL_RCC_GPIOA_CLK_ENABLE();

    /*##-2- Configure GPIO for CFG ##########################################*/
    /* GPIO pin configuration  */
    GPIO_InitStruct.Pin       = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2;
    GPIO_InitStruct.Mode      = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull      = GPIO_PULLUP;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    /*##-4- Configure GPIO for running zero ##########################################*/
    /* GPIO pin configuration  */
    GPIO_InitStruct.Pin       = GPIO_PIN_5 | GPIO_PIN_6 |
                                GPIO_PIN_7 | GPIO_PIN_8 |
                                GPIO_PIN_9;
    GPIO_InitStruct.Mode      = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull      = GPIO_NOPULL;
    GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

    /*##-5- Configure GPIO to turn on the LEDs ##########################################*/
    /* GPIO pin configuration  */
    GPIO_InitStruct.Pin       = GPIO_PIN_0 | GPIO_PIN_1 |
                                GPIO_PIN_2 | GPIO_PIN_3 |
                                GPIO_PIN_4 | GPIO_PIN_5 |
                                GPIO_PIN_6 | GPIO_PIN_7;
    GPIO_InitStruct.Mode      = GPIO_MODE_OUTPUT_OD;
    GPIO_InitStruct.Pull      = GPIO_NOPULL;
    GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    HAL_GPIO_WritePin( GPIOA, GPIO_PIN_0 | GPIO_PIN_1 |
                       GPIO_PIN_2 | GPIO_PIN_3 |
                       GPIO_PIN_4 | GPIO_PIN_5 |
                       GPIO_PIN_6 | GPIO_PIN_7, GPIO_PIN_SET);

    /*##-6- Configure GPIO to read button of keyboard ##########################################*/
    /* GPIO pin configuration  */
    GPIO_InitStruct.Pin       = GPIO_PIN_8 | GPIO_PIN_9 |
                                GPIO_PIN_10 | GPIO_PIN_11 |
                                GPIO_PIN_12 | GPIO_PIN_13 |
                                GPIO_PIN_14 | GPIO_PIN_15;
    GPIO_InitStruct.Mode      = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull      = GPIO_NOPULL;
    GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    /*##-7- Configure GPIO for mode maker ##########################################*/
    /* GPIO pin configuration  */
    GPIO_InitStruct.Pin       = GPIO_PIN_0 | GPIO_PIN_1;
    GPIO_InitStruct.Mode      = GPIO_MODE_OUTPUT_OD;
    GPIO_InitStruct.Pull      = GPIO_NOPULL;
    GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

    /*##-8- Configure GPIO for queue detector ##########################################*/
    /* GPIO pin configuration  */
    GPIO_InitStruct.Pin       = GPIO_PIN_13 | GPIO_PIN_14;
    GPIO_InitStruct.Mode      = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull      = GPIO_NOPULL;
    //GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_VERY_HIGH;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

    /*##-9- Configure GPIO for UPR pins ##########################################*/
    /* GPIO pin configuration  */
    GPIO_InitStruct.Pin       = GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7;
    GPIO_InitStruct.Mode      = GPIO_MODE_OUTPUT_OD;
    GPIO_InitStruct.Pull      = GPIO_NOPULL;
    GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

//    HAL_GPIO_WritePin(GPIOB,GPIO_PIN_7,GPIO_PIN_RESET); /*UPR1*/
    HAL_GPIO_WritePin(GPIOB,GPIO_PIN_5,GPIO_PIN_SET); /*UPR3*/
    HAL_GPIO_WritePin(GPIOB,GPIO_PIN_6,GPIO_PIN_SET); /*UPR2*/

    /*##-10- Configure GPIO for the test button ##########################################*/
    /* GPIO pin configuration  */
    GPIO_InitStruct.Pin       = GPIO_PIN_4;
    GPIO_InitStruct.Mode      = GPIO_MODE_IT_FALLING;
    GPIO_InitStruct.Pull      = GPIO_NOPULL;
    GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

    /* Enable and set EXTI line 4 Interrupt to the lowest priority */
    HAL_NVIC_SetPriority(EXTI4_IRQn, 5, 0);
    HAL_NVIC_EnableIRQ(EXTI4_IRQn);
}

#ifdef __cplusplus
}
#endif


GPIO::GPIO()
{
    gpioInit = & GPIO_InitStruct;
    initLEDsArray();
    osMessageQDef(queue, 1, int8_t);
    queue = osMessageCreate(osMessageQ(queue), nullptr);  // message queue creating

}

GPIO *GPIO::getInstance()
{
    if(!p_instance) p_instance = new GPIO();
    return p_instance;
}

GPIO *GPIO::p_instance = nullptr;

uint8_t GPIO::getCFG()
{
    uint8_t cfgData = 0;

    cfgData |= HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_0) << 2;
    cfgData |= HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_1) << 1;
    cfgData |= HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_2) << 0;

    return cfgData;
}

void GPIO::test()
{
    for (uint8_t i = 1; i < 33; ++i) {
        ledConfig(i, true);
    }
}

void GPIO::ledConfig(uint8_t ledNumber,
                     bool ledOn,
                     uint32_t timeOn,
                     uint32_t timeOff,
                     uint8_t repeatNum)
{
//    ledNumber -= 1;
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

void GPIO::readButton()
{
    osSignalWait(0x2, osWaitForever);
    for (int i = 8; i < 16; ++i) {
        if (HAL_GPIO_ReadPin(GPIOB, aPin[i])  == GPIO_PIN_RESET) {
            taskENTER_CRITICAL();
//            osDelay(300);
            osDelay(50);
            if (HAL_GPIO_ReadPin(GPIOB, aPin[i])  == GPIO_PIN_RESET) {
                GPIO::getInstance()->button.col = iter;
                GPIO::getInstance()->button.row = i - 8;
                buttonIsPressed = arrayButtons[button.row][button.col];
                CAN::getInstance()->createPackage(buttonIsPressed);
//                CAN::getInstance()->ringBufferTx.push(package
//                if (!testFlag) {
//                    if ( aLeds[buttonIsPressed - 1].ledState == false) {
//                        ledConfig(buttonIsPressed, true);
//                    } else {
//                        ledConfig(buttonIsPressed, false);
//                    }
//                }
                osSignalSet(CAN::getInstance()->sendToCanThreadId, 0x3);
            }
            taskEXIT_CRITICAL();


        }
    }
}


void GPIO::switchLEDs()
{
    uint32_t ledNumbers = 0;

    evt = osMessageGet(queue, osWaitForever); // wait for mail
    if(evt.status == osEventMessage) {

        for (auto var : aLeds) {
            if (var.ledState && var.col == (uint8_t)evt.value.v) ledNumbers |= var.row;
        }
        switch((uint8_t)evt.value.v) {
        case 0: {
            HAL_GPIO_WritePin(GPIOA, ledNumbers, GPIO_PIN_RESET);
            break;
        }
        case 1: {
            HAL_GPIO_WritePin(GPIOA, ledNumbers, GPIO_PIN_RESET);
            break;
        }
        case 2: {
            HAL_GPIO_WritePin(GPIOA, ledNumbers, GPIO_PIN_RESET);
            break;
        }
        case 3: {
            HAL_GPIO_WritePin(GPIOA, ledNumbers, GPIO_PIN_RESET);
            break;
        }
        default:
        {}
        }
    }
    osSignalSet(runZeroThreadId, 0x1);
}

void GPIO::runZero()
{
    iter = 0;
    for (uint8_t i = 6; i < 10; ++i) {

        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_All, GPIO_PIN_SET);
        if (i == 6) {
            HAL_GPIO_WritePin(GPIOC, aPin[i+3], GPIO_PIN_RESET);
            HAL_GPIO_WritePin(GPIOC, aPin[i], GPIO_PIN_SET);
        } else {
            HAL_GPIO_WritePin(GPIOC, aPin[i-1], GPIO_PIN_RESET);
            HAL_GPIO_WritePin(GPIOC, aPin[i], GPIO_PIN_SET);
        }
        osMessagePut(queue, iter, osWaitForever);
        osSignalWait(0x1, osWaitForever);
        osSignalSet(readButtonThreadId, 0x2);
        osDelay(1);
        iter++;

    }
}

void GPIO::initLEDsArray()
{
    for (uint8_t i = 0,
            j = 0,
            n = 0; i < 32; ++i, ++j) {
        if (j > 3) {
            j = 0;
            n++;
        }
        aLeds[i].row = aPin[n];
        aLeds[i].col = j;
    }
}

void GPIO::modeMaker()
{
    switch (mode) {
    case 1:
        HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET); //mode setting
        HAL_GPIO_WritePin(GPIOC, GPIO_PIN_14, GPIO_PIN_SET); // mode setting
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7, GPIO_PIN_SET); //power supply to the next board
        break;
    case 2:
        HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_SET); //mode setting
        HAL_GPIO_WritePin(GPIOC, GPIO_PIN_14, GPIO_PIN_SET); //mode setting
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7, GPIO_PIN_SET); //power supply to the next board
        break;
    default:
        break;
    }
}

void GPIO::detectMode()
{
    mode = HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_0) |
           HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_1) << 1;
    switch (mode) {
    case 1:
        modeMaker();
        break;
    case 2:
        modeMaker();
        break;
    case 3:
        break;
    default:
        Debug::getInstance().dbg << "UI32 detected incorrect mode!" << "\n";
        break;
    }
}

#ifdef __cplusplus
extern "C" {
#endif

/*!
  * \brief EXTI line detection callbacks
  * \param GPIO_Pin: Specifies the pins connected EXTI line
  * \retval None
  */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    if (GPIO_Pin == GPIO_PIN_4) {
        __HAL_RCC_WWDG_CLK_DISABLE();
        GPIO::getInstance()->testFlag = true;
        HAL_GPIO_WritePin(GPIOC, GPIO_PIN_5, GPIO_PIN_SET);
        osSignalSet (GPIO::getInstance()->createTestThreadId, 0x06);
    }
}

/*!
  * \brief  This function handles external lines 9 to 5 interrupt request.
  * \param  None
  * \retval None
  */
void EXTI4_IRQHandler(void)
{
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_4);

}

#ifdef __cplusplus
}
#endif
