#include "rs232.h"
#include "rs232_printf.h"
//#include <algorithm>
#include <string.h>
//#include "../Debug/debug.h"
//#include "eeprom.h"
//#include "fsforeeprom.h"

//#include "json.h"

extern UART_HandleTypeDef huart7;
static DMA_HandleTypeDef hdma_uart7_rx;
static DMA_HandleTypeDef hdma_uart7_tx;
//static unsigned char readByte();
//static void writeByte(unsigned char byte);

struct RS232 rs232;

HAL_StatusTypeDef RS232_write_u(uint8_t *buf, uint16_t size)
{
    while (HAL_UART_GetState(&huart7) != HAL_UART_STATE_READY);
    HAL_StatusTypeDef status = HAL_UART_Transmit_IT(&huart7, buf, size);
    while (rs232.uartWriteReady != SET);
    rs232.uartWriteReady = RESET;
    return status;
}

HAL_StatusTypeDef RS232_write_c(char *buf, uint16_t size)
{
    HAL_StatusTypeDef status = HAL_UART_Transmit_IT(&huart7, (uint8_t *)(buf), size);
    while (rs232.uartWriteReady != SET);
    rs232.uartWriteReady = RESET;
    return status;
}

HAL_StatusTypeDef RS232_read_u(uint8_t *buf, uint16_t size)
{
    HAL_StatusTypeDef status;
    while (HAL_UART_GetState(&huart7) != HAL_UART_STATE_READY);
    status = HAL_UART_Receive_IT(&huart7, buf, size);

    while (rs232.uartReadReady != SET);
    rs232.uartReadReady = RESET;
    return status;
}

HAL_StatusTypeDef RS232_read_c(char *buf, uint16_t size)
{
    HAL_StatusTypeDef status = HAL_UART_Receive_IT(&huart7, (uint8_t *)(buf), size);
    while (rs232.uartReadReady != SET);
    rs232.uartReadReady = RESET;
    return status;
}


//HAL_StatusTypeDef RS232::read(uint8_t *buf, uint16_t size, uint32_t timeout, bool saveDataToRingBuffer)
//{
//    std::fill(buf, buf + size, size);
//    HAL_StatusTypeDef status = HAL_UART_Receive(uartHandle, buf, size, timeout);
//    //get size of string
//    if (saveDataToRingBuffer) {
//        uint32_t lenString = std::strlen(reinterpret_cast<const char *>(buf));
//        //write string to ringBuffer
//        for (uint32_t i = 0; i < lenString; i++) {
//            ringBuffer.push(buf[i]);
//        }
//    }
//    return status;
//}

//HAL_StatusTypeDef RS232::read(char *buf, uint16_t size, uint32_t timeout, bool saveDataToRingBuffer)
//{
//    std::fill(buf, buf + size, 0);
//    HAL_StatusTypeDef status;
//    uint8_t tmp = 0;
//    do {
//        while (HAL_UART_GetState(uartHandle) != HAL_UART_STATE_READY);
//        status = HAL_UART_Receive(uartHandle, &tmp, 1, 1);
//    } while (tmp == 0);

//    buf[0] = tmp;

//    while (HAL_UART_GetState(uartHandle) != HAL_UART_STATE_READY);
//    status = HAL_UART_Receive(uartHandle, reinterpret_cast<uint8_t *>(buf + 1), size - 1, timeout);
//    //get size of string
//    if (saveDataToRingBuffer) {
//        uint32_t lenString = std::strlen(reinterpret_cast<const char *>(buf));
//        //write string to ringBuffer
//        for (uint32_t i = 0; i < lenString; i++) {
//            ringBuffer.push(buf[i]);
//        }
//    }
//    return status;
//}

//HAL_StatusTypeDef RS232::read(std::string &str, uint32_t timeout, bool saveDataToRingBuffer)
//{
//    std::array<uint8_t, sizeCircularBuffer> buf;
//    uint8_t tmp = 0;
//    HAL_StatusTypeDef status;
//    str.clear();
//    buf.fill(0);

//    do {
//        while (HAL_UART_GetState(uartHandle) != HAL_UART_STATE_READY);
//        status = HAL_UART_Receive(uartHandle, &tmp, 1, 1);
//    } while (tmp == 0);

//    str.push_back(tmp);

//    while (HAL_UART_GetState(uartHandle) != HAL_UART_STATE_READY);
//    status = HAL_UART_Receive(uartHandle, buf.begin(), buf.size(), timeout);
//    for (auto i : buf) {
//        str.push_back(i);
//    }

//    if (saveDataToRingBuffer) {
//        //write string to ringBuffer
//        for (auto i : buf) {
//            ringBuffer.push(i);
//        }
//    }
//    return status;
//}


//static unsigned char readByte()
//{
//    uint8_t byte;
//    rs232.read(&byte, sizeof(byte));
//    return (unsigned char)(byte);
//}

//static void writeByte(unsigned char byte)
//{
//    RS232::getInstance().write(&byte, sizeof(byte));
//}


void HAL_UART_TxCpltCallback(UART_HandleTypeDef *UartHandle)
{
    if (UartHandle->Instance == UART7)
    {
        /* Set transmission flag: trasfer complete*/
        rs232.uartWriteReady = SET;
    }
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *UartHandle)
{
    if (UartHandle->Instance == UART7)
    {
        /* Set transmission flag: trasfer complete*/
        rs232.uartReadReady = SET;

    }
}

void HAL_UART_ErrorCallback(UART_HandleTypeDef *UartHandle)
{
    if (UartHandle->Instance == UART7) {
        rs232.uartErrorCallback = 1;
    }
}


//void DMA1_Stream1_IRQHandler(void)
//{
//    /* USER CODE BEGIN DMA1_Stream1_IRQn 0 */

//    /* USER CODE END DMA1_Stream1_IRQn 0 */
//    HAL_DMA_IRQHandler(huart7.hdmatx);
//    /* USER CODE BEGIN DMA1_Stream1_IRQn 1 */

//    /* USER CODE END DMA1_Stream1_IRQn 1 */
//}

//void UART7_IRQHandler(void)
//{
//    HAL_UART_IRQHandler(RS232::getInstance().uartHandle);
//}


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
