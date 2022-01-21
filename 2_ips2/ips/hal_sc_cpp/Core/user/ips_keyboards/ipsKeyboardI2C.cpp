/*
 * ipsKeyboardI2C.cpp
 *
 *  Created on: Jul 22, 2020
 *      Author: Dmitry Donskih
 */
#include "main.h"
#include "ipsKeyboardI2C.h"

//void HAL_I2C_MspInit(I2C_HandleTypeDef* hi2c)
//{
//  GPIO_InitTypeDef GPIO_InitStruct = {0};
//  if(hi2c->Instance==I2C1)
//  {
//  /* USER CODE BEGIN I2C1_MspInit 0 */
//
//  /* USER CODE END I2C1_MspInit 0 */
//
//    __HAL_RCC_GPIOB_CLK_ENABLE();
//    /**I2C1 GPIO Configuration
//    PB8     ------> I2C1_SCL
//    PB9     ------> I2C1_SDA
//    */
//    GPIO_InitStruct.Pin = GPIO_PIN_8|GPIO_PIN_9;
//    GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
//    GPIO_InitStruct.Pull = GPIO_NOPULL;
//    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
//    GPIO_InitStruct.Alternate = GPIO_AF4_I2C1;
//    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
//
//    /* Peripheral clock enable */
//    __HAL_RCC_I2C1_CLK_ENABLE();
//    /* I2C1 interrupt Init */
//    HAL_NVIC_SetPriority(I2C1_EV_IRQn, 0, 0);
//    HAL_NVIC_EnableIRQ(I2C1_EV_IRQn);
//  /* USER CODE BEGIN I2C1_MspInit 1 */
//
//  /* USER CODE END I2C1_MspInit 1 */
//  }else{
//	  Error_Handler();
//  }
//
//}



ipsKeyboardI2C::ipsKeyboardI2C(I2C_HandleTypeDef *hI2Cport)
{
    hkeyboard=*hI2Cport;
    uFW_I2CPort_Init();
}

void ipsKeyboardI2C::uFW_I2CPort_Init() {

}


