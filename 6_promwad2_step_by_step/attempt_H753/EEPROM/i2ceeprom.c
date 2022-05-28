#include "i2ceeprom.h"
#include <string.h>

extern UART_HandleTypeDef huart7;
extern I2C_HandleTypeDef hi2c1;
//DMA_HandleTypeDef hdma_i2c1_rx,
//                  hdma_i2c1_tx;

uint32_t timeOutMax = 300;
FlagStatus i2cReadReady = RESET ;
FlagStatus i2cWriteReady = RESET ;


void simpleEEPROM_test()
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
            const char result[] = "\n\r##### Test EEPROM passed! #####\r\n\n";
            HAL_UART_Transmit (&huart7,(uint8_t*)result, sizeof (result),1000);

        } else
        {
            const char result[] = "\n\r##### Test EEPROM failed :( #####\r\n";
            HAL_UART_Transmit (&huart7,(uint8_t*)result, sizeof (result),1000);
        }
        HAL_Delay(10);
}

void simpleEEPROM_test2()
{
    const char wmsg[] = "1234567890";
        char rmsg[128];
        uint16_t devAddr = 0xA0;
        uint16_t memAddr = 0x0000;
        HAL_StatusTypeDef status;

//        HAL_I2C_Mem_Read_IT(&hi2c1, devAddr, memAddr, I2C_MEMADD_SIZE_16BIT, (uint8_t*)rmsg, sizeof(rmsg));
        HAL_I2C_Mem_Read(&hi2c1, devAddr, memAddr, I2C_MEMADD_SIZE_16BIT, (uint8_t*)rmsg, sizeof(rmsg), 100);


//        HAL_I2C_Mem_Write_IT(&hi2c1, devAddr, memAddr, I2C_MEMADD_SIZE_16BIT, (uint8_t*)wmsg, sizeof(wmsg));
        HAL_I2C_Mem_Write(&hi2c1, devAddr, memAddr, I2C_MEMADD_SIZE_16BIT, (uint8_t*)wmsg, sizeof(wmsg), 100);
        for(;;)
        { // wait...
            status = HAL_I2C_IsDeviceReady(&hi2c1, devAddr, 1, HAL_MAX_DELAY);
            if(status == HAL_OK)
                break;
        }

        HAL_I2C_Mem_Read(&hi2c1, devAddr, memAddr, I2C_MEMADD_SIZE_16BIT, (uint8_t*)rmsg, sizeof(rmsg), HAL_MAX_DELAY);
        const char result[] = "\n\r---- Read Data: ---- ";
        HAL_UART_Transmit (&huart7,(uint8_t*)result, sizeof (result), 1000);
        HAL_UART_Transmit (&huart7,(uint8_t*)rmsg, 10, 1000);
        HAL_UART_Transmit (&huart7,(uint8_t*)" --- \n\r", sizeof (" --- \n\r"), 1000);
}

HAL_StatusTypeDef i2c1_writeData(uint16_t DevAddress, uint16_t MemAddress, uint8_t *pData, uint16_t Size)
{
    while (HAL_I2C_GetState(&hi2c1) != HAL_I2C_STATE_READY)
    {
       HAL_Delay(10);
    }

//    HAL_StatusTypeDef status = HAL_I2C_Mem_Write_DMA(&hi2c1, DevAddress, MemAddress, I2C_MEMADD_SIZE_16BIT, pData, Size); //Выпилил
    HAL_StatusTypeDef status = HAL_I2C_Mem_Write(&hi2c1, DevAddress, MemAddress, I2C_MEMADD_SIZE_16BIT, pData, Size, HAL_MAX_DELAY);
    for(;;)
    { // wait...
        status = HAL_I2C_IsDeviceReady(&hi2c1, DevAddress, 1, HAL_MAX_DELAY);
        if(status == HAL_OK)
            break;
    }

//    while( i2cWriteReady != SET);
//    i2cWriteReady = RESET;

    return status;
}

HAL_StatusTypeDef i2c1_readData(uint16_t DevAddress, uint16_t MemAddress, uint8_t *pData, uint16_t Size)
{
    while (HAL_I2C_GetState(&hi2c1) != HAL_I2C_STATE_READY)
    {
        HAL_Delay(10);
    }

//    HAL_StatusTypeDef status = HAL_I2C_Mem_Read_DMA(&hi2c1, DevAddress, MemAddress, I2C_MEMADD_SIZE_16BIT, pData, Size);
//    while(i2cReadReady != SET);
//    i2cReadReady = RESET;

HAL_StatusTypeDef status = HAL_I2C_Mem_Read(&hi2c1, DevAddress, MemAddress, I2C_MEMADD_SIZE_16BIT, pData, Size, HAL_MAX_DELAY);

return status;
}

void EEPROM_IO_Init(void)
{
//Уже в main

}

HAL_StatusTypeDef EEPROM_IO_WriteData(uint16_t DevAddress, uint16_t MemAddress, uint8_t *pBuffer, uint32_t BufferSize)
{
    HAL_StatusTypeDef status = i2c1_writeData(DevAddress, MemAddress, pBuffer, BufferSize);
    return status;
}

HAL_StatusTypeDef EEPROM_IO_ReadData(uint16_t DevAddress, uint16_t MemAddress, uint8_t *pBuffer, uint32_t BufferSize)
{
    HAL_StatusTypeDef status = i2c1_readData(DevAddress, MemAddress, pBuffer, BufferSize);
    return status;
}

HAL_StatusTypeDef EEPROM_IO_IsDeviceReady(uint16_t DevAddress, uint32_t Trials)
{
    HAL_StatusTypeDef status = HAL_I2C_IsDeviceReady(&hi2c1, DevAddress, Trials, 300);
    return status;
}


#include "cmsis_os.h"

void HAL_I2C_MemTxCpltCallback(I2C_HandleTypeDef *I2cHandle)
{
    i2cWriteReady = SET;
}

void HAL_I2C_MemRxCpltCallback(I2C_HandleTypeDef *I2cHandle)
{
    i2cReadReady = SET;
}

void I2C1_EV_IRQHandler(void)
{
    HAL_I2C_EV_IRQHandler(&hi2c1);
}

void I2C1_ER_IRQHandler(void)
{
    HAL_I2C_ER_IRQHandler(&hi2c1);
}

