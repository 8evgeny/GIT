#include "i2c.h"
#include "stm32h7xx_hal.h"
#include "stm32h7xx_hal_i2c.h"
#include "rs232.h"

#ifdef __cplusplus
extern "C" {
#endif
static I2C_HandleTypeDef hI2cAudioHandler;
static I2C_HandleTypeDef hI2cBoardHandler;

void i2cInitBoard(void)
{
    /* DMA controller clock enable */
    __HAL_RCC_DMA1_CLK_ENABLE();
    /* DMA interrupt init */

    /* DMA1_Stream2_IRQn interrupt configuration */
    HAL_NVIC_SetPriority(DMA1_Stream0_IRQn, 5, 0);
    HAL_NVIC_EnableIRQ(DMA1_Stream0_IRQn);

    /* DMA1_Stream4_IRQn interrupt configuration */
    HAL_NVIC_SetPriority(DMA1_Stream7_IRQn, 5, 0);
    HAL_NVIC_EnableIRQ(DMA1_Stream7_IRQn);

    hI2cBoardHandler.Instance = I2C3;
    hI2cBoardHandler.Init.Timing = 0x20404768;
    hI2cBoardHandler.Init.OwnAddress1 = 0x00;
    hI2cBoardHandler.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
    hI2cBoardHandler.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
    hI2cBoardHandler.Init.OwnAddress2 = 0xFF;
    hI2cBoardHandler.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
    hI2cBoardHandler.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
    HAL_I2C_Init(&hI2cBoardHandler);

    /** Configure Analogue filter
    */
    HAL_I2CEx_ConfigAnalogFilter(&hI2cBoardHandler, I2C_ANALOGFILTER_ENABLE);
    /** Configure Digital filter
    */
    HAL_I2CEx_ConfigDigitalFilter(&hI2cBoardHandler, 0);
}

void i2cInitAudio(void)
{
    /* DMA controller clock enable */
    __HAL_RCC_DMA1_CLK_ENABLE();
    /* DMA interrupt init */

    /* DMA1_Stream2_IRQn interrupt configuration */
    HAL_NVIC_SetPriority(DMA1_Stream2_IRQn, 5, 0);
    HAL_NVIC_EnableIRQ(DMA1_Stream2_IRQn);

    /* DMA1_Stream4_IRQn interrupt configuration */
    HAL_NVIC_SetPriority(DMA1_Stream4_IRQn, 5, 0);
    HAL_NVIC_EnableIRQ(DMA1_Stream4_IRQn);

    hI2cAudioHandler.Instance = I2C2;
    hI2cAudioHandler.Init.Timing = 0x20404768;
    hI2cAudioHandler.Init.OwnAddress1 = 0x00;
    hI2cAudioHandler.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
    hI2cAudioHandler.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
    hI2cAudioHandler.Init.OwnAddress2 = 0xFF;
    hI2cAudioHandler.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
    hI2cAudioHandler.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
    HAL_I2C_Init(&hI2cAudioHandler);

    /** Configure Analogue filter
    */
    HAL_I2CEx_ConfigAnalogFilter(&hI2cAudioHandler, I2C_ANALOGFILTER_ENABLE);
    /** Configure Digital filter
    */
    HAL_I2CEx_ConfigDigitalFilter(&hI2cAudioHandler, 0);
}

#ifdef __cplusplus
}
#endif

I2C *I2C::pInstance = nullptr;

I2C::I2C()
{
    hI2cHandlerAudio = &hI2cAudioHandler;
    hI2cHandlerBoard = &hI2cBoardHandler;
}

I2C *I2C::getInstance()
{
    if (!pInstance) pInstance = new I2C();
    return pInstance;
}

uint8_t I2C::readRegister(uint8_t addr, uint8_t reg, bool audio)
{
    uint8_t readData = 0;
    HAL_StatusTypeDef status = HAL_OK;

    if(audio)
    {
        while (HAL_I2C_GetState(hI2cHandlerAudio) != HAL_I2C_STATE_READY);
        do {
            status = HAL_I2C_Master_Transmit_DMA(hI2cHandlerAudio, addr, &reg, 1);
            while (I2C::getInstance()->i2c2WriteReady == RESET);
            I2C::getInstance()->i2c2WriteReady = RESET;
            errorI2CAudio(status);
            }
        while (status != HAL_OK);

        while (HAL_I2C_GetState(hI2cHandlerAudio) != HAL_I2C_STATE_READY);
        do {
            status = HAL_I2C_Master_Receive_DMA(hI2cHandlerAudio, addr, &readData, 1);
            while (I2C::getInstance()->i2c2ReadReady == RESET);
            I2C::getInstance()->i2c2ReadReady = RESET;
            errorI2CAudio(status);
            }
        while (status != HAL_OK);
    }
    else
    {
        while (HAL_I2C_GetState(hI2cHandlerBoard) != HAL_I2C_STATE_READY);
        do {
            status = HAL_I2C_Master_Transmit_DMA(hI2cHandlerBoard, addr, &reg, 1);
            while (I2C::getInstance()->i2c3WriteReady == RESET);
            I2C::getInstance()->i2c3WriteReady = RESET;
            errorI2CBoard(status);
            }
        while (status != HAL_OK);

        while (HAL_I2C_GetState(hI2cHandlerBoard) != HAL_I2C_STATE_READY);
        do {
            status = HAL_I2C_Master_Receive_DMA(hI2cHandlerBoard, addr, &readData, 1);
            while (I2C::getInstance()->i2c3ReadReady == RESET);
            I2C::getInstance()->i2c3ReadReady = RESET;
            errorI2CBoard(status);
            }
        while (status != HAL_OK);
    }
    return readData;
}

void I2C::writeRegister(uint8_t addr, uint8_t reg, uint8_t val, bool audio)
{
    HAL_StatusTypeDef status = HAL_OK;
    uint8_t request[2];

    request[0] = reg;
    request[1] = val;

    if(audio) //Кодек I2C2
    {
        while (HAL_I2C_GetState(hI2cHandlerAudio) != HAL_I2C_STATE_READY);
        do {
            status = HAL_I2C_Master_Transmit_DMA(hI2cHandlerAudio, addr, request, sizeof(request) / sizeof(uint8_t));
            while (I2C::getInstance()->i2c2WriteReady == RESET);
            I2C::getInstance()->i2c2WriteReady = RESET;
            errorI2CAudio(status);
           }
        while (status != HAL_OK);
    }
    else //Плата клавиатуры I2C3
    {
        while (HAL_I2C_GetState(hI2cHandlerBoard) != HAL_I2C_STATE_READY);
        do {
            status = HAL_I2C_Master_Transmit_DMA(hI2cHandlerBoard, addr, request, sizeof(request) / sizeof(uint8_t));
            while (I2C::getInstance()->i2c3WriteReady == RESET);
            I2C::getInstance()->i2c3WriteReady = RESET;
            errorI2CBoard(status);
           }
        while (status != HAL_OK);
    }


}

void I2C::errorI2CAudio(HAL_StatusTypeDef status)
{
    if (status != HAL_OK) {
        HAL_I2C_DeInit(hI2cHandlerAudio);
        i2cInitAudio();
    }

    if (I2C::getInstance()->i2c2Error == SET) {
        HAL_I2C_DeInit(hI2cHandlerAudio);
        i2cInitAudio();
        I2C::getInstance()->i2c2Error =  RESET;
    }
}

void I2C::errorI2CBoard(HAL_StatusTypeDef status)
{
    if (status != HAL_OK) {
        HAL_I2C_DeInit(hI2cHandlerBoard);
        i2cInitBoard();
    }

    if (I2C::getInstance()->i2c3Error == SET) {
        HAL_I2C_DeInit(hI2cHandlerBoard);
        i2cInitBoard();
        I2C::getInstance()->i2c3Error =  RESET;
    }
}

#ifdef __cplusplus
extern "C" {
#endif

/**
  * @brief This function handles DMA1 stream2 global interrupt.
  */
//void DMA1_Stream2_IRQHandler(void)
//{
//    HAL_DMA_IRQHandler(hI2cAudioHandler.hdmarx);
//}

/**
  * @brief This function handles DMA1 stream4 global interrupt.
  */
//void DMA1_Stream4_IRQHandler(void)
//{
//    HAL_DMA_IRQHandler(hI2cAudioHandler.hdmatx);
//}

/**
  * @brief This function handles I2C2 event interrupt.
  */
void I2C2_EV_IRQHandler(void)
{
    HAL_I2C_EV_IRQHandler(&hI2cAudioHandler);
}

void I2C3_EV_IRQHandler(void)
{
    HAL_I2C_EV_IRQHandler(&hI2cBoardHandler);
}

/**
  * @brief This function handles I2C2 error interrupt.
  */
void I2C2_ER_IRQHandler(void)
{
    HAL_I2C_ER_IRQHandler(&hI2cAudioHandler);
    I2C::getInstance()->i2c2Error = SET;
}

void I2C3_ER_IRQHandler(void)
{
    HAL_I2C_ER_IRQHandler(&hI2cBoardHandler);
    I2C::getInstance()->i2c3Error = SET;
}

void HAL_I2C_MasterTxCpltCallback(I2C_HandleTypeDef *hi2c)
{
    if (hi2c->Instance == I2C2) {
        I2C::getInstance()->i2c2WriteReady = SET;
    }
    if (hi2c->Instance == I2C3) {
        I2C::getInstance()->i2c3WriteReady = SET;
    }
}

void HAL_I2C_MasterRxCpltCallback(I2C_HandleTypeDef *hi2c)
{
    if (hi2c->Instance == I2C2) {
        I2C::getInstance()->i2c2ReadReady = SET;
    }
    if (hi2c->Instance == I2C3) {
        I2C::getInstance()->i2c3ReadReady = SET;
    }
}

#ifdef __cplusplus
}
#endif

