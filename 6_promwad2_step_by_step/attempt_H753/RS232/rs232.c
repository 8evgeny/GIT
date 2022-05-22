#include "rs232.h"
#include "rs232_printf.h"
//#include <algorithm>
#include <string.h>
//#include "../Debug/debug.h"
#include "eeprom.h"
#include "fsforeeprom.h"

//#include "json.h"

extern UART_HandleTypeDef huart7;
static DMA_HandleTypeDef hdma_uart7_rx;
static DMA_HandleTypeDef hdma_uart7_tx;
static unsigned char readByte();
static void writeByte(unsigned char byte);

struct RS232 rs232;

void RS232Init(void)
{
    RS232DevOut(writeByte);
    RS232DevIn(readByte);

    GPIO_InitTypeDef GPIO_InitStruct = {0};
    __HAL_RCC_UART7_CLK_ENABLE();

    __HAL_RCC_DMA1_CLK_ENABLE();
    __HAL_RCC_GPIOF_CLK_ENABLE();
    /**UART7 GPIO Configuration
    PF6     ------> UART7_RX
    PF7     ------> UART7_TX
    */
    GPIO_InitStruct.Pin = GPIO_PIN_6 | GPIO_PIN_7;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF7_UART7;
    HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);

    /* UART7 DMA Init */
    huart7.hdmarx = &hdma_uart7_rx;
    huart7.hdmatx = &hdma_uart7_tx;

}

HAL_StatusTypeDef RS232_write(uint8_t *buf, uint16_t size)
{
    while (HAL_UART_GetState(&huart7) != HAL_UART_STATE_READY);
    HAL_StatusTypeDef status = HAL_UART_Transmit_DMA(&huart7, buf, size);
    while (rs232.uartWriteReady != SET);
    rs232.uartWriteReady = RESET;
    return status;
}

//HAL_StatusTypeDef RS232::write(char *buf, uint16_t size)
//{
//    HAL_StatusTypeDef status = HAL_UART_Transmit_DMA(uartHandle, uartHandle, reinterpret_cast<uint8_t *>(buf), size);
//    while (uartWriteReady != SET);
//    uartWriteReady = RESET;
//    return status;
//}

HAL_StatusTypeDef RS232::read(uint8_t *buf, uint16_t size)
{
    HAL_StatusTypeDef status;
    while (HAL_UART_GetState(uartHandle) != HAL_UART_STATE_READY);
    status = HAL_UART_Receive_DMA(uartHandle, buf, size);


    while (uartReadReady != SET);
    uartReadReady = RESET;
    return status;
}

HAL_StatusTypeDef RS232::read(char *buf, uint16_t size)
{
    HAL_StatusTypeDef status = HAL_UART_Receive_DMA(uartHandle, reinterpret_cast<uint8_t *>(buf), size);
    while (uartReadReady != SET);
    uartReadReady = RESET;
    return status;
}


HAL_StatusTypeDef RS232::read(uint8_t *buf, uint16_t size, uint32_t timeout, bool saveDataToRingBuffer)
{
    std::fill(buf, buf + size, size);
    HAL_StatusTypeDef status = HAL_UART_Receive(uartHandle, buf, size, timeout);
    //get size of string
    if (saveDataToRingBuffer) {
        uint32_t lenString = std::strlen(reinterpret_cast<const char *>(buf));
        //write string to ringBuffer
        for (uint32_t i = 0; i < lenString; i++) {
            ringBuffer.push(buf[i]);
        }
    }
    return status;
}

HAL_StatusTypeDef RS232::read(char *buf, uint16_t size, uint32_t timeout, bool saveDataToRingBuffer)
{
    std::fill(buf, buf + size, 0);
    HAL_StatusTypeDef status;
    uint8_t tmp = 0;
    do {
        while (HAL_UART_GetState(uartHandle) != HAL_UART_STATE_READY);
        status = HAL_UART_Receive(uartHandle, &tmp, 1, 1);
    } while (tmp == 0);

    buf[0] = tmp;

    while (HAL_UART_GetState(uartHandle) != HAL_UART_STATE_READY);
    status = HAL_UART_Receive(uartHandle, reinterpret_cast<uint8_t *>(buf + 1), size - 1, timeout);
    //get size of string
    if (saveDataToRingBuffer) {
        uint32_t lenString = std::strlen(reinterpret_cast<const char *>(buf));
        //write string to ringBuffer
        for (uint32_t i = 0; i < lenString; i++) {
            ringBuffer.push(buf[i]);
        }
    }
    return status;
}

HAL_StatusTypeDef RS232::read(std::string &str, uint32_t timeout, bool saveDataToRingBuffer)
{
    std::array<uint8_t, sizeCircularBuffer> buf;
    uint8_t tmp = 0;
    HAL_StatusTypeDef status;
    str.clear();
    buf.fill(0);

    do {
        while (HAL_UART_GetState(uartHandle) != HAL_UART_STATE_READY);
        status = HAL_UART_Receive(uartHandle, &tmp, 1, 1);
    } while (tmp == 0);

    str.push_back(tmp);

    while (HAL_UART_GetState(uartHandle) != HAL_UART_STATE_READY);
    status = HAL_UART_Receive(uartHandle, buf.begin(), buf.size(), timeout);
    for (auto i : buf) {
        str.push_back(i);
    }

    if (saveDataToRingBuffer) {
        //write string to ringBuffer
        for (auto i : buf) {
            ringBuffer.push(i);
        }
    }
    return status;
}


static unsigned char readByte()
{
    uint8_t byte;
    RS232::getInstance().read(&byte, sizeof(byte));
    return static_cast<unsigned char>(byte);
}

static void writeByte(unsigned char byte)
{
    RS232::getInstance().write(&byte, sizeof(byte));
}

/*!
  \brief UART MSP Initialization


          This function configures the hardware resources used in this example:
             - Peripheral's clock enable
             - Peripheral's GPIO Configuration
             - DMA configuration for transmission request by peripheral
             - NVIC configuration for DMA interrupt request enable


  \param huart: UART handle pointer
  \retval None
  */
void HAL_UART_MspInit(UART_HandleTypeDef *huart)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    if (huart->Instance == UART7) {

        /* Peripheral clock enable */
        __HAL_RCC_UART7_CLK_ENABLE();

        __HAL_RCC_DMA1_CLK_ENABLE();
        __HAL_RCC_GPIOF_CLK_ENABLE();
        /**UART7 GPIO Configuration
        PF6     ------> UART7_RX
        PF7     ------> UART7_TX
        */
        GPIO_InitStruct.Pin = GPIO_PIN_6 | GPIO_PIN_7;
        GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull = GPIO_PULLUP;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
        GPIO_InitStruct.Alternate = GPIO_AF7_UART7;
        HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);

        /* UART7 DMA Init */
        /* UART7_TX Init */
        hdma_uart7_tx.Instance = DMA1_Stream1;
        hdma_uart7_tx.Init.Request = DMA_REQUEST_UART7_TX;
        hdma_uart7_tx.Init.Direction = DMA_MEMORY_TO_PERIPH;
        hdma_uart7_tx.Init.PeriphInc = DMA_PINC_DISABLE;
        hdma_uart7_tx.Init.MemInc = DMA_MINC_ENABLE;
        hdma_uart7_tx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
        hdma_uart7_tx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
        hdma_uart7_tx.Init.Mode = DMA_NORMAL;
        hdma_uart7_tx.Init.Priority = DMA_PRIORITY_LOW;
        hdma_uart7_tx.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
        HAL_DMA_Init(&hdma_uart7_tx);

        __HAL_LINKDMA(huart, hdmatx, hdma_uart7_tx);

        /* UART7_RX Init */
        hdma_uart7_rx.Instance = DMA1_Stream3;
        hdma_uart7_rx.Init.Request = DMA_REQUEST_UART7_RX;
        hdma_uart7_rx.Init.Direction = DMA_PERIPH_TO_MEMORY;
        hdma_uart7_rx.Init.PeriphInc = DMA_PINC_DISABLE;
        hdma_uart7_rx.Init.MemInc = DMA_MINC_ENABLE;
        hdma_uart7_rx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
        hdma_uart7_rx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
        hdma_uart7_rx.Init.Mode = DMA_NORMAL;
        hdma_uart7_rx.Init.Priority = DMA_PRIORITY_HIGH;
        hdma_uart7_rx.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
        HAL_DMA_Init(&hdma_uart7_rx);

        __HAL_LINKDMA(huart, hdmarx, hdma_uart7_rx);

            /* DMA interrupt init */
            /* DMA1_Stream1_IRQn interrupt configuration */
            HAL_NVIC_SetPriority(DMA1_Stream1_IRQn, 5, 0);
            HAL_NVIC_EnableIRQ(DMA1_Stream1_IRQn);
            /* DMA1_Stream2_IRQn interrupt configuration */
            HAL_NVIC_SetPriority(DMA1_Stream2_IRQn, 5, 0);
            HAL_NVIC_EnableIRQ(DMA1_Stream2_IRQn);
            /* DMA1_Stream3_IRQn interrupt configuration */
            HAL_NVIC_SetPriority(DMA1_Stream3_IRQn, 5, 0);
            HAL_NVIC_EnableIRQ(DMA1_Stream3_IRQn);
            /* DMA1_Stream4_IRQn interrupt configuration */
            HAL_NVIC_SetPriority(DMA1_Stream4_IRQn, 5, 0);
            HAL_NVIC_EnableIRQ(DMA1_Stream4_IRQn);
            /* DMA2_Stream3_IRQn interrupt configuration */
            HAL_NVIC_SetPriority(DMA2_Stream3_IRQn, 5, 0);
            HAL_NVIC_EnableIRQ(DMA2_Stream3_IRQn);
            /* DMA2_Stream4_IRQn interrupt configuration */
            HAL_NVIC_SetPriority(DMA2_Stream4_IRQn, 5, 0);
            HAL_NVIC_EnableIRQ(DMA2_Stream4_IRQn);

            /* UART7 interrupt Init */
            HAL_NVIC_SetPriority(UART7_IRQn, 0, 1);
            HAL_NVIC_EnableIRQ(UART7_IRQn);

            /* DMA1_Stream1_IRQn interrupt configuration */
            HAL_NVIC_SetPriority(DMA1_Stream1_IRQn, 0, 1);
            HAL_NVIC_EnableIRQ(DMA1_Stream1_IRQn);

            /* DMA1_Stream3_IRQn interrupt configuration */
            HAL_NVIC_SetPriority(DMA1_Stream3_IRQn, 0, 0);
            HAL_NVIC_EnableIRQ(DMA1_Stream3_IRQn);

            __HAL_RCC_DMA1_CLK_ENABLE();
            __HAL_RCC_DMA2_CLK_ENABLE();
    }

}

/**
* @brief UART MSP De-Initialization
* This function freeze the hardware resources used in this example
* @param huart: UART handle pointer
* @retval None
*/
void HAL_UART_MspDeInit(UART_HandleTypeDef *huart)
{
    if (huart->Instance == UART7) {
        /* USER CODE BEGIN UART7_MspDeInit 0 */

        /* USER CODE END UART7_MspDeInit 0 */
        /* Peripheral clock disable */
        __HAL_RCC_UART7_CLK_DISABLE();

        /**UART7 GPIO Configuration
        PF6     ------> UART7_RX
        PF7     ------> UART7_TX
        */
        HAL_GPIO_DeInit(GPIOF, GPIO_PIN_6 | GPIO_PIN_7);

        /* UART7 DMA DeInit */
        HAL_DMA_DeInit(huart->hdmatx);
        HAL_DMA_DeInit(huart->hdmarx);

        /* UART7 interrupt DeInit */
        HAL_NVIC_DisableIRQ(UART7_IRQn);
        /* USER CODE BEGIN UART7_MspDeInit 1 */

        /* USER CODE END UART7_MspDeInit 1 */
    }
}

/*!
  \brief  Tx Transfer completed callback
  \param  UartHandle: UART handle.
  \retval None
*/
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *UartHandle)
{
    if (UartHandle->Instance == UART7) {
        /* Set transmission flag: trasfer complete*/
        RS232::getInstance().uartWriteReady = SET;

    }
}

/*!
  \brief  Rx Transfer completed callback
  \param  UartHandle: UART handle
  \retval None
*/
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *UartHandle)
{
    if (UartHandle->Instance == UART7) {
        /* Set transmission flag: trasfer complete*/
        RS232::getInstance().uartReadReady = SET;

    }
}

/*!
  \brief  UART error callbacks
  \param  UartHandle: UART handle
  \retval None
*/
void HAL_UART_ErrorCallback(UART_HandleTypeDef *UartHandle)
{
    if (UartHandle->Instance == UART7) {
        RS232::getInstance().uartErrorCallback = true;
    }
}

/**
  * @brief This function handles DMA1 stream3 global interrupt.
  */
/**
  * @brief This function handles DMA1 stream3 global interrupt.
  */
void DMA1_Stream3_IRQHandler(void)
{
    /* USER CODE BEGIN DMA1_Stream3_IRQn 0 */

    /* USER CODE END DMA1_Stream3_IRQn 0 */
    HAL_DMA_IRQHandler(RS232::getInstance().uartHandle->hdmarx);
    /* USER CODE BEGIN DMA1_Stream3_IRQn 1 */

    /* USER CODE END DMA1_Stream3_IRQn 1 */
}
/**
  * @brief This function handles DMA1 stream1 global interrupt.
  */
/**
  * @brief This function handles DMA1 stream1 global interrupt.
  */
void DMA1_Stream1_IRQHandler(void)
{
    /* USER CODE BEGIN DMA1_Stream1_IRQn 0 */

    /* USER CODE END DMA1_Stream1_IRQn 0 */
    HAL_DMA_IRQHandler(RS232::getInstance().uartHandle->hdmatx);
    /* USER CODE BEGIN DMA1_Stream1_IRQn 1 */

    /* USER CODE END DMA1_Stream1_IRQn 1 */
}
/**
  * @brief This function handles UART7 global interrupt.
  */

//void UART7_IRQHandler(void)
//{
//    HAL_UART_IRQHandler(RS232::getInstance().uartHandle);
//}

/*!
 \brief Custom output stream for RS232 [const char *]
*/
Console &operator<<(Console &console, const char *data)
{
    RS232Printf("%s", data);
    return console;
}

/*!
 \brief Custom output stream for RS232 [const unsigned char *]
*/
Console &operator<<(Console &console, const unsigned char *data)
{
    RS232Printf("%s", data);
    return console;
}

/*!
 \brief Custom output stream for RS232 [unsigned char]
*/
Console &operator<<(Console &console, unsigned char data)
{
    RS232Printf("%u", data);
    return console;
}

/*!
 \brief Custom output stream for RS232 [char]
*/
Console &operator<<(Console &console, char data)
{
    RS232Printf("%c", data);
    return console;
}

/*!
 \brief Custom output stream for RS232 [short]
*/
Console &operator<<(Console &console, short data)
{
    RS232Printf("%h", data);
    return console;
}

/*!
 \brief Custom output stream for RS232 [unsigned short]
*/
Console &operator<<(Console &console, unsigned short data)
{
    RS232Printf("%h", data);
    return console;
}

/*!
 \brief Custom output stream for RS232 [int]
*/
Console &operator<<(Console &console, int data)
{
    RS232Printf("%d", data);
    return console;
}

/*!
 \brief Custom output stream for RS232 [unsigned int]
*/
Console &operator<<(Console &console, unsigned int data)
{
    RS232Printf("%u", data);
    return console;
}

/*!
 \brief Custom output stream for RS232 [long int]
*/
Console &operator<<(Console &console, long int data)
{
    RS232Printf("%l", data);
    return console;
}

/*!
 \brief Custom output stream for RS232 [unsigned long int]
*/
Console &operator<<(Console &console, unsigned long int data)
{
    RS232Printf("%ul", data);
    return console;
}

/*!
 \brief Custom output stream for RS232
*/
Console &Console::operator<<(Console & (*data)(Console &))
{
    return (*data)(*this);
}

void RS232::test()
{
    uint32_t i  = 0;
    std::array <char, 100> buf;

    RS232::getInstance().term << 123 <<  " " << i << " " << "1234567890\n";
    RS232::getInstance().term << 456 <<  " " << i << " " << "1234567890\n";
    RS232::getInstance().term << 789 <<  " " << i << " " << "1234567890\n";


    RS232::getInstance().read(buf.begin(), buf.size(), 100); //without save data to RingBuffer
    RS232::getInstance().term << buf.begin();

    RS232::getInstance().read(buf.begin(), buf.size(), 100, true); //with save data to RingBuffer
    RS232::getInstance().term << buf.begin();
}

extern "C" void  RS232::C_test(RS232* p)
{
   return p->RS232::test();
}



Console &operator>>(Console &console, std::string &data)
{
    RS232::getInstance().read(data, 10);
    return console;
}

Console &operator<<(Console &console, const std::string &data)
{
    RS232Printf("%s", data.c_str());
    return console;
}


static int32_t commonSizeAllFrames = 0;
static int32_t counterFrames = 0;

//void readFromUartThread(void const *arg)
//{
//    (void)arg;
//    constexpr int32_t SIZE_DEF_BLOCK_UDP = 256;
//    constexpr int32_t SIZE_WRITE_BLOCK = 128;

//    uint8_t buffUart[SIZE_DEF_BLOCK_UDP] = {0};
//    char buf[SIZE_DEF_BLOCK_UDP] {0};
//    uint32_t tmp[SIZE_DEF_BLOCK_UDP / 4];

//    while (true) {

//        const int capacityJson = 2000;
//        StaticJsonDocument <capacityJson> doc;

//        const int capacity = JSON_OBJECT_SIZE(3);
//        StaticJsonDocument<capacity> configDoc;

//        while (1) {

//            RS232::getInstance().read(buffUart, sizeof(buffUart));
//            if (buffUart[0] == 0) {
//                memcpy(buf, buffUart + 1, SIZE_DEF_BLOCK_UDP - 1);
//            } else {
//                memcpy(buf, buffUart, SIZE_DEF_BLOCK_UDP);
//            }

//            DeserializationError err = deserializeJson(doc, buf);
//            if (DeserializationError::Ok == err) {

//                //int writeConfigId = doc["writeConfigId"];
//                int number =  doc["number"];
//                int all = doc["all"];
//                int size = doc["size"];
//                const char *config  = doc["config"];

//                if (number == 0) {
//                    commonSizeAllFrames = 0;
//                    counterFrames = 0;
//                }

//                if (number == counterFrames) {
//                    counterFrames++;
//                    commonSizeAllFrames += size;
//                    std::fill(tmp, tmp + SIZE_DEF_BLOCK_UDP / 4, 0);
//                    std::memcpy(tmp, config, SIZE_WRITE_BLOCK);
//                    SRAM::getInstance()->writeData(reinterpret_cast<uint32_t *>(tmp), sizeof(tmp) / sizeof(uint8_t), reinterpret_cast<uint32_t *>(0x60000000  + number * SIZE_WRITE_BLOCK));
//                    if (number == all) {
//                        break;
//                    }
//                }
//            }
//        }

//        uint8_t readSramBuff[SIZE_WRITE_BLOCK] {0};

//        lfs_remove(FsForEeprom::getInstance().lfsPtr, "boot_config");
//        lfs_file_open(FsForEeprom::getInstance().lfsPtr, FsForEeprom::getInstance().filePtr, "boot_config", LFS_O_RDWR | LFS_O_CREAT);

//        for (int32_t i = 0; i < commonSizeAllFrames; i += SIZE_WRITE_BLOCK) {
//            SRAM::getInstance()->readData(reinterpret_cast<uint32_t *>(readSramBuff), SIZE_WRITE_BLOCK, reinterpret_cast<uint32_t *>(0x60000000 + i));
//            lfs_file_write(FsForEeprom::getInstance().lfsPtr, FsForEeprom::getInstance().filePtr, readSramBuff,  commonSizeAllFrames - i < SIZE_WRITE_BLOCK ? static_cast<uint32_t>(commonSizeAllFrames - i) : SIZE_WRITE_BLOCK);
//        }
//        lfs_file_close(FsForEeprom::getInstance().lfsPtr, FsForEeprom::getInstance().filePtr);

//        commonSizeAllFrames = 0;
//        counterFrames = 0;


//        //send msg configuration write OK

//        char tmpWriteBuf[SIZE_DEF_BLOCK_UDP] {0};
//        std::fill(tmpWriteBuf, tmpWriteBuf + SIZE_DEF_BLOCK_UDP, 0);

//        configDoc["writeConfigId"] = Json::getInstance()->thisStation.id;
//        configDoc["status"].set("ok");
//        serializeJson(configDoc, tmpWriteBuf, capacity);
//        RS232::getInstance().write(reinterpret_cast<uint8_t *>(tmpWriteBuf), static_cast<uint16_t>(std::strlen(tmpWriteBuf)));

//    }
//}
