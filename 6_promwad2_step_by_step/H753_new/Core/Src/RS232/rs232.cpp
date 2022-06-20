/*!
 \file rs232.cpp

 \authors Shulenkov R.A.

 \code
    RS232Init();

    ...

    uint32_t i  = 0;

    while (1) {
        RS232::getInstance().term << 123 <<  " " << i << " " << "1234567890\n";
        RS232::getInstance().term << 456 <<  " " << i << " " << "1234567890\n";
        RS232::getInstance().term << 789 <<  " " << i << " " << "1234567890\n";
    }

    ...

    std::array <char, 10> buf;

    while (1) {

        RS232::getInstance().read(buf.begin(), buf.size(), 100); //without save data to RingBuffer
        //RS232::getInstance().read(buf.begin(), buf.size(), 100, false) = RS232::getInstance().read(buf.begin(), buf.size(), 100)
        RS232::getInstance().term << buf.begin();

        RS232::getInstance().read(buf.begin(), buf.size(), 100, true); //with save data to RingBuffer
        RS232::getInstance().term << buf.begin();
    }

 \endcode



*/
#include "rs232.h"
#include "rs232_printf.h"
#include <algorithm>
#include <cstring>
#include "../Debug/debug.h"
#include "eeprom.h"
#include "fsforeeprom.h"
//#ifndef SC2BOARD
////#include "call_control.h"
//#else
////#include "../Call_control_for_SC2_board/call_control_sc2.h"
//#endif


#include "json.h"




#ifdef __cplusplus
extern "C" {
#endif
extern UART_HandleTypeDef huart7;
extern DMA_HandleTypeDef hdma_uart7_rx;
extern DMA_HandleTypeDef hdma_uart7_tx;
static unsigned char readByte();
static void writeByte(unsigned char byte);

void RS232Init(void)
{


    RS232DevOut(writeByte);
    RS232DevIn(readByte);

    /* UART7 DMA Init */
    /* UART7_TX Init */
//    huart7.Instance = UART7;
//    huart7.Init.BaudRate = 115200;
//    huart7.Init.WordLength = UART_WORDLENGTH_8B;
//    huart7.Init.StopBits = UART_STOPBITS_1;
//    huart7.Init.Parity = UART_PARITY_NONE;
//    huart7.Init.Mode = UART_MODE_TX_RX;
//    huart7.Init.HwFlowCtl = UART_HWCONTROL_NONE;
//    huart7.Init.OverSampling = UART_OVERSAMPLING_16;
//    huart7.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
//    huart7.Init.ClockPrescaler = UART_PRESCALER_DIV1;
//    huart7.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;

//    huart7.hdmarx = &hdma_uart7_rx;
//    huart7.hdmatx = &hdma_uart7_tx;

//    if (HAL_UART_Init(&huart7) != HAL_OK) {
//      while (1) {
//          Debug::getInstance().dbg << "UART Init Error!" << "\n";
//      }
//    }

}

#ifdef __cplusplus
}
#endif


RS232 *RS232::pInstance = nullptr;
RS232Destroyer RS232::destroyer;

RS232::RS232()
{
    uartHandle = &huart7;

}

RS232Destroyer::~RS232Destroyer()
{
    delete pInstance;
}

void RS232Destroyer::initialize(RS232 *p)
{
    pInstance = p;
}

RS232 &RS232::getInstance()
{
    if (!pInstance) {
        pInstance = new RS232();
        destroyer.initialize(pInstance);
    }
    return *pInstance;
}

extern "C" RS232& RS232::C_getInstance(RS232* p)
{
   return p->RS232::getInstance();
}


HAL_StatusTypeDef RS232::write(uint8_t *buf, uint16_t size)
{
    while (HAL_UART_GetState(uartHandle) != HAL_UART_STATE_READY);
    HAL_StatusTypeDef status = HAL_UART_Transmit_DMA(uartHandle, buf, size);
    while (uartWriteReady != SET);
    uartWriteReady = RESET;
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


#ifdef __cplusplus
extern "C" {
#endif

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

#ifdef __cplusplus
}
#endif

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

void readFromUartThread(void const *arg)
{
    (void)arg;
    constexpr int32_t SIZE_DEF_BLOCK_UDP = 256;
    constexpr int32_t SIZE_WRITE_BLOCK = 128;

    uint8_t buffUart[SIZE_DEF_BLOCK_UDP] = {0};
    char buf[SIZE_DEF_BLOCK_UDP] {0};
    uint32_t tmp[SIZE_DEF_BLOCK_UDP / 4];

HAL_Delay(10000);
term("****  readFromUartThread  start  ****")

    while (true)
    {
term("****  readFromUartThread  working  ****")
        const int capacityJson = 2000;
        StaticJsonDocument <capacityJson> doc;

        const int capacity = JSON_OBJECT_SIZE(3);
        StaticJsonDocument<capacity> configDoc;

        while (1)
        {
term("****  read ****")
            RS232::getInstance().read(buffUart, sizeof(buffUart));
term("****  receive block ****")
            if (buffUart[0] == 0)
            {
                memcpy(buf, buffUart + 1, SIZE_DEF_BLOCK_UDP - 1);
            }
            else
            {
                memcpy(buf, buffUart, SIZE_DEF_BLOCK_UDP);
            }
term(buf)
            DeserializationError err = deserializeJson(doc, buf);
            if (DeserializationError::Ok == err)
            {
term("Deserialization no Error")
                //int writeConfigId = doc["writeConfigId"];
                int number =  doc["number"];
term1("number")
term(number)
                int all = doc["all"];
term1("all")
term(all)
                int size = doc["size"];
term1("size")
term(size)
                const char *config  = doc["config"];
term1("config")
term(config)
                if (number == 0)
                {
                    commonSizeAllFrames = 0;
                    counterFrames = 0;
                }
                if (number == counterFrames)
                {
                    counterFrames++;
                    commonSizeAllFrames += size;
                    std::fill(tmp, tmp + SIZE_DEF_BLOCK_UDP / 4, 0);
                    std::memcpy(tmp, config, SIZE_WRITE_BLOCK);
term("1")
                    SRAM::getInstance()->writeData(reinterpret_cast<uint32_t *>(tmp), sizeof(tmp) / sizeof(uint8_t), reinterpret_cast<uint32_t *>(0x60000000  + number * SIZE_WRITE_BLOCK));
term("2")
                    if (number == all)
                    {
                        break;
                    }
                }
            }
        }//while

        uint8_t readSramBuff[SIZE_WRITE_BLOCK] {0};

        lfs_remove(FsForEeprom::getInstance().lfsPtr, "boot_config");
        lfs_file_open(FsForEeprom::getInstance().lfsPtr, FsForEeprom::getInstance().filePtr, "boot_config", LFS_O_RDWR | LFS_O_CREAT);

        for (int32_t i = 0; i < commonSizeAllFrames; i += SIZE_WRITE_BLOCK)
        {
            SRAM::getInstance()->readData(reinterpret_cast<uint32_t *>(readSramBuff), SIZE_WRITE_BLOCK,
                                          reinterpret_cast<uint32_t *>(0x60000000 + i));
            lfs_file_write(FsForEeprom::getInstance().lfsPtr, FsForEeprom::getInstance().filePtr, readSramBuff,
                           commonSizeAllFrames - i < SIZE_WRITE_BLOCK ? static_cast<uint32_t>(commonSizeAllFrames - i) : SIZE_WRITE_BLOCK);
        }
        lfs_file_close(FsForEeprom::getInstance().lfsPtr, FsForEeprom::getInstance().filePtr);

        commonSizeAllFrames = 0;
        counterFrames = 0;


        //send msg configuration write OK

        char tmpWriteBuf[SIZE_DEF_BLOCK_UDP] {0};
        std::fill(tmpWriteBuf, tmpWriteBuf + SIZE_DEF_BLOCK_UDP, 0);

        configDoc["writeConfigId"] = Json::getInstance()->thisStation.id;
        configDoc["status"].set("ok");
        serializeJson(configDoc, tmpWriteBuf, capacity);
        RS232::getInstance().write(reinterpret_cast<uint8_t *>(tmpWriteBuf), static_cast<uint16_t>(std::strlen(tmpWriteBuf)));

    osDelay(1000);
    } //end while (true)

}
