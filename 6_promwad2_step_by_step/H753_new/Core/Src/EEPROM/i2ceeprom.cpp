#include "i2ceeprom.h"
#include "rs232.h"
#include <cstring>

#ifdef __cplusplus
extern "C" {
#endif

extern I2C_HandleTypeDef hi2c1;
DMA_HandleTypeDef hdma_i2c1_rx,
                  hdma_i2c1_tx;


static void I2C1Init(void)
{

    hi2c1.Instance = I2C1;
    hi2c1.Init.Timing = 0x307075B1; //Код формирует CUBE
//    hi2c1.Init.Timing = FAST_MODE_PLUS;
    hi2c1.Init.OwnAddress1 = 0;
    hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
    hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
    hi2c1.Init.OwnAddress2 = 0;
    hi2c1.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
    hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
    hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
    if (HAL_I2C_Init(&hi2c1) != HAL_OK)
    {
        while(1) {
            RS232::getInstance().term << "I2C1 Init Error!" << "\n";
        }
    }
    /** Configure Analogue filter
    */
    if (HAL_I2CEx_ConfigAnalogFilter(&hi2c1, I2C_ANALOGFILTER_ENABLE) != HAL_OK) {
        while(1) {
            RS232::getInstance().term << "I2C1 Configure Analogue filter Error!" << "\n";
        }
    }
    /** Configure Digital filter
    */
    if (HAL_I2CEx_ConfigDigitalFilter(&hi2c1, 0) != HAL_OK) {
        while(1) {
            RS232::getInstance().term << "I2C1 Configure Digital filter Error!" << "\n";
        }
    }

}

void printNumReboot()
{
    FsForEeprom::getInstance().numReboots();
}


void test_EEPROM()
{
    const char wmsg[] = "Some data";
        char rmsg[sizeof(wmsg)];
        // HAL expects address to be shifted one bit to the left
        uint16_t devAddr = 0xA0;
//        uint16_t memAddr = 0x0100;
        uint16_t memAddr = 0x0000;   // Адресуется максимум 64 кб   У нас 128 кб
//        Чипы, имеющие более 64 Кбайт памяти, начинают использовать для адресации младшие биты I2С-адреса устройства.
//        То есть, с точки зрения нашего кода, они будут выглядеть, как 2 или более отдельных I2C-устройства

        HAL_StatusTypeDef status;

        // Hint: try to comment this line
        HAL_I2C_Mem_Write(&hi2c1, devAddr, memAddr, I2C_MEMADD_SIZE_16BIT, (uint8_t*)wmsg, sizeof(wmsg), HAL_MAX_DELAY);
//        HAL_I2C_Mem_Write(&hi2c1, devAddr, memAddr, I2C_MEMADD_SIZE_16BIT, (uint8_t*)wmsg, sizeof(wmsg), 10);

        for(;;)
        { // wait...
            status = HAL_I2C_IsDeviceReady(&hi2c1, devAddr, 1, HAL_MAX_DELAY);
            if(status == HAL_OK)
                break;
        }

        HAL_I2C_Mem_Read(&hi2c1, devAddr, memAddr, I2C_MEMADD_SIZE_16BIT, (uint8_t*)rmsg, sizeof(rmsg), HAL_MAX_DELAY);
//        HAL_I2C_Mem_Write(&hi2c1, devAddr, memAddr, I2C_MEMADD_SIZE_16BIT, (uint8_t*)wmsg, sizeof(wmsg), 10);

        if(memcmp(rmsg, wmsg, sizeof(rmsg)) == 0)
        {
            FsForEeprom::getInstance().test();
        }
        else
        {
            const char result[] = "Test EEPROM failed :(";
            term2(result)
        }
}

void test_EEPROM_2()
{
    const char wmsg[] = "1234567890";
        char rmsg[128];
        uint16_t devAddr = 0xA0;
        uint16_t memAddr = 0x0000;
        HAL_StatusTypeDef status;
        HAL_I2C_Mem_Read(&hi2c1, devAddr, memAddr, I2C_MEMADD_SIZE_16BIT, (uint8_t*)rmsg, sizeof(rmsg), HAL_MAX_DELAY);
        RS232::getInstance().term << "old EEPROM Data:  " << rmsg << "\r\n";

        HAL_I2C_Mem_Write(&hi2c1, devAddr, memAddr, I2C_MEMADD_SIZE_16BIT, (uint8_t*)wmsg, sizeof(wmsg), HAL_MAX_DELAY);
        for(;;)
        { // wait...
            status = HAL_I2C_IsDeviceReady(&hi2c1, devAddr, 1, HAL_MAX_DELAY);
            if(status == HAL_OK)
                break;
        }

        HAL_I2C_Mem_Read(&hi2c1, devAddr, memAddr, I2C_MEMADD_SIZE_16BIT, (uint8_t*)rmsg, sizeof(rmsg), HAL_MAX_DELAY);
        RS232::getInstance().term << "new EEPROM Data: " << rmsg << "\r\n";
}


void EEPROM_IO_Init(void)
{
    I2C1Init();
}

HAL_StatusTypeDef EEPROM_IO_WriteData(uint16_t DevAddress, uint16_t MemAddress, uint8_t *pBuffer, uint32_t BufferSize)
{
    HAL_StatusTypeDef status = I2c1::getInstance()->writeData(DevAddress, MemAddress, pBuffer, BufferSize);
    return status;
}

HAL_StatusTypeDef EEPROM_IO_ReadData(uint16_t DevAddress, uint16_t MemAddress, uint8_t *pBuffer, uint32_t BufferSize)
{
    HAL_StatusTypeDef status = I2c1::getInstance()->readData(DevAddress, MemAddress, pBuffer, BufferSize);
    return status;
}

HAL_StatusTypeDef EEPROM_IO_IsDeviceReady(uint16_t DevAddress, uint32_t Trials)
{
    HAL_StatusTypeDef status = HAL_I2C_IsDeviceReady(&hi2c1, DevAddress, Trials, I2c1::getInstance()->timeOutMax);
    return status;
}

#ifdef __cplusplus
}
#endif

I2c1::I2c1()
{
    i2c1Handle = &hi2c1;
}
I2c1 *I2c1::getInstance()
{
    if(!pInstance) pInstance = new I2c1();
    return pInstance;
}

I2c1 *I2c1::pInstance = nullptr;

#include "cmsis_os.h"

HAL_StatusTypeDef I2c1::writeData(uint16_t DevAddress, uint16_t MemAddress, uint8_t *pData, uint16_t Size)
{

    while (HAL_I2C_GetState(i2c1Handle) != HAL_I2C_STATE_READY);

//    HAL_StatusTypeDef status = HAL_I2C_Mem_Write_IT(i2c1Handle, DevAddress, MemAddress, I2C_MEMADD_SIZE_16BIT, pData, Size); //Выпилил
    HAL_StatusTypeDef status = HAL_I2C_Mem_Write(&hi2c1, DevAddress, MemAddress, I2C_MEMADD_SIZE_16BIT, pData, Size, HAL_MAX_DELAY);
    for(;;)
    { // wait...
        status = HAL_I2C_IsDeviceReady(&hi2c1, DevAddress, 1, HAL_MAX_DELAY);
        if(status == HAL_OK)
            break;
    }

//    while( i2cWriteReady != SET); //Выпилил
//    i2cWriteReady = RESET;

    return status;
}

HAL_StatusTypeDef I2c1::readData(uint16_t DevAddress, uint16_t MemAddress, uint8_t *pData, uint16_t Size)
{
    while (HAL_I2C_GetState(i2c1Handle) != HAL_I2C_STATE_READY)
        HAL_Delay(10);

//term("--------- HAL_I2C_Mem_Read__begin ------------")

//    HAL_StatusTypeDef status = HAL_I2C_Mem_Read_IT(i2c1Handle, DevAddress, MemAddress, I2C_MEMADD_SIZE_16BIT, pData, Size); //Выпилил
    HAL_StatusTypeDef status = HAL_I2C_Mem_Read(&hi2c1, DevAddress, MemAddress, I2C_MEMADD_SIZE_16BIT, pData, Size, HAL_MAX_DELAY);

//    while(i2cReadReady != SET); //Выпилил
//    i2cReadReady = RESET;

//term("--------- HAL_I2C_Mem_Read__end ------------")
    return status;
}

#ifdef __cplusplus
extern "C" {
#endif

/**
  * @brief  Tx Transfer completed callback.
  * @param  I2cHandle: I2C handle
  * @note   This example shows a simple way to report end of DMA Tx transfer, and
  *         you can add your own implementation.
  * @retval None
  */
void HAL_I2C_MemTxCpltCallback(I2C_HandleTypeDef *I2cHandle)
{
term("HAL_I2C_MemTxCpltCallback")
    I2c1::getInstance()->i2cWriteReady = SET;
}

/**
  * @brief  Rx Transfer completed callback.
  * @param  I2cHandle: I2C handle
  * @note   This example shows a simple way to report end of DMA Rx transfer, and
  *         you can add your own implementation.
  * @retval None
  */
void HAL_I2C_MemRxCpltCallback(I2C_HandleTypeDef *I2cHandle)
{
term("HAL_I2C_MemRxCpltCallback")
    I2c1::getInstance()->i2cReadReady = SET;
}
/**
  * @brief  This function handles DMA interrupt request.
  * @param  None
  * @retval None
  * @Note   This function is redefined in "main.h" and related to DMA handle
  *         used for I2C data transmission
  */
void DMA1_Stream5_IRQHandler(void)
{
    HAL_DMA_IRQHandler(hi2c1.hdmarx);

}

/**
  * @brief  This function handles DMA interrupt request.
  * @param  None
  * @retval None
  * @Note   This function is redefined in "main.h" and related to DMA handle
  *         used for I2C data reception
  */
void DMA1_Stream6_IRQHandler(void)
{
    HAL_DMA_IRQHandler(hi2c1.hdmatx);

}

/**
  * @brief  This function handles I2C event interrupt request.
  * @param  None
  * @retval None
  * @Note   This function is redefined in "main.h" and related to I2C data transmission
  */
void I2C1_EV_IRQHandler(void)
{
    HAL_I2C_EV_IRQHandler(&hi2c1);
}

/**
  * @brief  This function handles I2C error interrupt request.
  * @param  None
  * @retval None
  * @Note   This function is redefined in "main.h" and related to I2C error
  */
void I2C1_ER_IRQHandler(void)
{
    HAL_I2C_ER_IRQHandler(&hi2c1);
}

#ifdef __cplusplus
}
#endif
