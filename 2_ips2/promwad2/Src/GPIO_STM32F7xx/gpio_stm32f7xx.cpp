/*!
 \file gpio.cpp

 \authors Androshchuk A.V.

 \code
    GPIOInit();

    ...

 \endcode



*/
#include "gpio_stm32f7xx.h"
#include <algorithm>
#include <cstring>
#include "../Debug/debug.h"
#include "wdt.h"

#ifdef __cplusplus
extern "C" {
#endif
extern osSemaphoreId Netif_LinkSemaphore;
static osTimerId timerId4; /*!< The thread ID of the timer */
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

    /*##-3- Configure GPIO for volume buttons ##########################################*/
    /* GPIO pin configuration  */
    GPIO_InitStruct.Pin       = GPIO_PIN_14 | GPIO_PIN_15;
    GPIO_InitStruct.Mode      = GPIO_MODE_IT_FALLING;
    GPIO_InitStruct.Pull      = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

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


    /*Configure GPIO pin : PA5 */
    GPIO_InitStruct.Pin = GPIO_PIN_5;
    GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /* EXTI interrupt init*/
    HAL_NVIC_SetPriority(EXTI2_IRQn, 5, 0);
    HAL_NVIC_EnableIRQ(EXTI2_IRQn);

    HAL_NVIC_SetPriority(EXTI9_5_IRQn, 5, 0);
    HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);

    HAL_NVIC_SetPriority(EXTI15_10_IRQn, 5, 0);
    HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);

}

#ifdef __cplusplus
}
#endif


GPIO::GPIO()
{
    gpioInit = & GPIO_InitStruct;
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

void changeVolumeThread(void const *arg)
{
    (void)arg;
    int8_t volumeValue = GPIO::getInstance()->dacDriverGainValue;
    while(true) {
        if (volumeValue != GPIO::getInstance()->dacDriverGainValue) {
            I2C::getInstance()->writeRegister(TLV320AIC3254::I2C_ADDRESS, 0x00, 0x01);
            I2C::getInstance()->writeRegister(TLV320AIC3254::I2C_ADDRESS, 0x12, GPIO::getInstance()->dacDriverGainValue);
            I2C::getInstance()->writeRegister(TLV320AIC3254::I2C_ADDRESS, 0x13, GPIO::getInstance()->dacDriverGainValue);
        }
        osDelay(1);
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
        __HAL_RCC_WWDG_CLK_DISABLE();
        HAL_GPIO_WritePin(GPIOG, GPIO_PIN_13, GPIO_PIN_SET);
        GPIO::getInstance()->testFlag = true;
//        osSignalSet(GPIO::getInstance()->createTestTaskThreadId, 0x03);
    } else if (GPIO_Pin == GPIO_PIN_14) {
        if (GPIO::getInstance()->dacDriverGainValue < 29)
            ++GPIO::getInstance()->dacDriverGainValue;
    } else if (GPIO_Pin == GPIO_PIN_15) {
        if (GPIO::getInstance()->dacDriverGainValue > -6)
            --GPIO::getInstance()->dacDriverGainValue;
    }


    if (GPIO_Pin == GPIO_PIN_5) {
        osSemaphoreRelease(Netif_LinkSemaphore);
    }

}


/*!
  * @brief  This function handles external lines 9 to 5 interrupt request.
  * @param  None
  * @retval None
  */
void EXTI9_5_IRQHandler(void)
{
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_5);
}
/*!
  * @brief  This function handles external lines 15 to 10 interrupt request.
  * @param  None
  * @retval None
  */
void EXTI15_10_IRQHandler(void)
{
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_11);
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_14);
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_15);
}
#ifdef __cplusplus
}
#endif
