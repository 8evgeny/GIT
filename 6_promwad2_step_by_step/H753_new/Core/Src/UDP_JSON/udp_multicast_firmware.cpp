#include <sstream>
#include <bitset>
#include <iomanip>
#include <array>
#include "ArduinoJson.h"
#include "json.h"
#include "ff_gen_drv.h"
#include <stm32h7xx_hal.h>
#include "flash.h"
#include "CircularBuffer.h"
#include "json.h"
#include "main.h"
#include "rs232.h"
#include "rs232_printf.h"
#include "../UDP_JSON/udp_multicast.h"
#include "aes.h"
#include "stm32h7xx_hal_cryp.h"
#include <inttypes.h>
#include "testTasks.h"

void printFlashOptions(FLASH_OBProgramInitTypeDef &OBInit);
void newFirmwareWrite(uint32_t firmwareSize);
extern HASH_HandleTypeDef hhash;
extern CRC_HandleTypeDef hcrc;
extern uint8_t DataFirmware[NUM_FIRMWARE_PACKET][SIZE_FIRMWARE_BASE] __attribute__((section(".ExtRamData")));
//extern uint8_t DataFirmware2[NUM_FIRMWARE_PACKET][SIZE_FIRMWARE_BASE] __attribute__((section(".ExtRamData")));

//extern CRYP_HandleTypeDef hcrypFIRMWARE;
extern char *allConfig;
extern int sizeConfig;
extern uint8_t pinNormaState;
extern uint8_t pinMkState;
extern lfs_t lfs;
extern lfs_file_t file;
/*!
 \brief Function translate binary data to a string

 \fn hexStr
 \param const std::array<char, SIZE_FIRMWARE_BASE> &data Binary data
 \return std::string Created string for binary data
*/
std::string hexStr(const std::array<char, SIZE_FIRMWARE_BASE> &data)
{
    std::stringstream ss;
    ss << std::hex;
    for (char i : data) {
        auto c = static_cast<unsigned char>(i);
        int x = c;
        ss << std::hex;
        ss << std::setw(2) << std::setfill('0') << x;
    }
    return ss.str();
}

/*!
 \brief unction translate a string to binary data

 \fn strHex
 \param data A string
 \return std::array<char, SIZE_FIRMWARE_BASE> Binary data
*/
std::array<char, SIZE_FIRMWARE_BASE> strHex(const std::string &data)
{
    std::array<char, SIZE_FIRMWARE_BASE> tmp;
    size_t j = 0;
    for (size_t i = 0; i < data.size(); i += 2)
    {
        std::stringstream ss;
        ss << std::hex;
        std::string hexConv = std::string("") + data.at(i) + std::string("") + data.at(i + 1);
        ss << hexConv;
        int x = 0;
        ss >> x;
        auto c = static_cast<char>(x);
        tmp[j] = c;
        j++;
    }
    return tmp;
}

using FirmwarePackage = struct {
    int versionFirmware;
    int subVersionFirmware;
    int size;
    int current;
    int all;
    std::array<char, SIZE_FIRMWARE_BASE> data;
};

static CircularBuffer <FirmwarePackage, 20> firmwareRingBuffer; /*! Ring buffer for JSON packages */
static osMutexId mutexFirmwareRingBufferId; /*! Mutex ID */
static osThreadId firmwareThreadId; /*! Thread ID */

[[ noreturn ]]void updateFirmwareThread(const void *arg);

void firmwareInitThread()
{
    osMutexDef(mutexFirmware);
    mutexFirmwareRingBufferId = osMutexCreate(osMutex(mutexFirmware));

    osThreadDef(handelFirmwareThread, updateFirmwareThread, osPriorityHigh, 0, configMINIMAL_STACK_SIZE * 3);
    firmwareThreadId = osThreadCreate(osThread(handelFirmwareThread), NULL);
}

static FATFS FLASHFatFs;  /*! File system object for Flash logical drive */
static char FLASHPath[4]; /*! FLASH logical drive path */
[[ noreturn ]]void updateFirmwareThread(const void *arg)
{
    char tmp[256];
    UNUSED(arg);

    bool lastPacket = false;
    bool beginFirmware = false;
    uint8_t receivedHashKeyBin[16];
    uint8_t calculatedMd5[16];
    uint32_t indexFirwareArray = 0;
    uint8_t indexTempArray16byte = 0;
    uint32_t sizeDecoded = 0;
    uint32_t counterPackegs = 0; /*! A counter for size of packages */
    uint32_t counterSize = 0; /*! A counter for size of data */
//    int versionFirmware;
//    int subVersionFirmware;
//    int current;
//    int all;

    while (1)
    {
        osMutexWait(mutexFirmwareRingBufferId, osWaitForever);
        if (firmwareRingBuffer.size() != 0)
        {
            FirmwarePackage pack = firmwareRingBuffer.first();
            firmwareRingBuffer.shift();
            osMutexRelease(mutexFirmwareRingBufferId);

//            versionFirmware = pack.versionFirmware;
//            subVersionFirmware = pack.subVersionFirmware;

//            all = pack.all;
//            current = pack.current;

            if (pack.current == counterPackegs)
            {
                beginFirmware = true;
                if (pack.current == 0) //Первый пакет
                {
                    pinNormaState = pinNormaBlink;
                    pinMkState = pinMkReset;

                    f_mount(&FLASHFatFs, (TCHAR const *)FLASHPath, 0); //Прерывистось если нет этого

                    //Очищаем массив под прошивку (Можно и не очищать)
                    for (size_t k = 0; k < NUM_FIRMWARE_PACKET; ++k)
                    {
                        for (size_t i = 0; i < SIZE_FIRMWARE_BASE; ++i)
                        {
                           DataFirmware[k][i] = 0x00;
                        }
                    }
                    counterSize = 0;
                    counterPackegs = 0;
                }
                else if (pack.current == pack.all) //Последний пакет
                {
                    lastPacket = true;
                }
                else
                { //Все пакеты кроме первого и последнего
                }
            }

            if(beginFirmware && (pack.current != counterPackegs)) //Ошибка получения пакета
            {
                term2("error receiving package...")
                //Обработка неудачной прошивки
                pinNormaState = pinNormaSet;
                pinMkState = pinMkBlinkFast;
                counterSize = 0;
                counterPackegs = 0;
                indexFirwareArray = 0;
                sizeDecoded = 0;
                indexTempArray16byte = 0;
                indexFirwareArray = 0;
                beginFirmware = false;
            }
            uint8_t temp[16];
            if((!lastPacket) && (pack.current == counterPackegs) && beginFirmware)
            {
                counterSize += pack.data.size();
                counterPackegs++;
                for (size_t i = 0; i < SIZE_FIRMWARE_BASE; ++i)
                {
                    temp[indexTempArray16byte] = pack.data.at(i);
                    ++indexTempArray16byte;
                    if (indexTempArray16byte == 16)
                    {
                        indexTempArray16byte = 0;
                        AES128_ECB_decrypt(temp, key, (uint8_t *)DataFirmware + indexFirwareArray * 16 );
                        ++indexFirwareArray;
                        sizeDecoded +=16;
                    }
                }
                sprintf(tmp,"packet %d of %d size_packet = %d received_size = %d" , (uint32_t)pack.current, (uint32_t)pack.all, (uint32_t)pack.data.size(), (uint32_t)counterSize);
                term2(tmp)
            }

            if((lastPacket) && (pack.current == counterPackegs) && beginFirmware)
            {
                counterSize += pack.size / 2;   //Последний пакет меньше
                for (size_t i = 0; i < pack.size/2 ; ++i)
                {
                    temp[indexTempArray16byte] = pack.data.at(i);
                    ++indexTempArray16byte;
                    if ((indexTempArray16byte == 16) && (i < pack.size/2 - 16 ))
                    {
                        indexTempArray16byte = 0;
                        AES128_ECB_decrypt(temp, key, (uint8_t *)DataFirmware + indexFirwareArray * 16 );
                        ++indexFirwareArray;
                        sizeDecoded +=16;
                    }
                    if ((indexTempArray16byte == 16) && (i >= pack.size/2 - 16 )) //В indexTempArray16byte находится Хеш
                    {
                        //Копируем полученный hashKeyBin
                        strncpy ((char*)receivedHashKeyBin,(const char*)temp , 16);
                    }
                }
                sprintf(tmp,"packet %d of %d size_packet = %d received_size = %d", (uint32_t)pack.current, (uint32_t)pack.all, (uint32_t)pack.size /2, (uint32_t)counterSize);
                term2(tmp)

                sprintf(tmp,"sizeDecoded = %d", sizeDecoded);
                term2(tmp)

                lastPacket = false;
                //Размер полученного файла
                uint32_t firmwareSize = counterSize - 16; //Последние 16 байт  - Хеш
                sprintf(tmp,"firmware size = %d", (uint32_t)firmwareSize);
                term2 (tmp)

                //Выводим полученный hashKeyBin
                RS232::getInstance().term <<"Received hashKeyBin:\t";
                for (auto i=0; i < 16; ++i) { sprintf(tmp,"%1.1x", receivedHashKeyBin[i]); RS232::getInstance().term <<tmp;}
                RS232::getInstance().term <<"\r\n";

                HAL_HASH_MD5_Start(&hhash, (uint8_t *)DataFirmware, firmwareSize, calculatedMd5, 1000);
                RS232::getInstance().term <<"Calculated Md5:\t\t";
                for (auto i=0; i < 16; ++i) { sprintf(tmp,"%1.1x",calculatedMd5[i]); RS232::getInstance().term <<tmp;}
                RS232::getInstance().term <<"\r\n";

                //Для тестов - выводим DataFirmware
//                for (uint32_t i = 0; i < firmwareSize; ++i)
//                {
//                    RS232::getInstance().term << DataFirmware[i];
//                }
//                RS232::getInstance().term << "\r\n";

                if(strncmp((char*)receivedHashKeyBin, (char*)calculatedMd5, 16) == 0)
                {
                    term2("MD5 OK")
                    newFirmwareWrite(firmwareSize);   //md5 совпали - пишем прошивку
                }
                else
                {
                    term2("MD5 error")
                    //Обработка неудачной прошивки
                    pinNormaState = pinNormaSet;
                    pinMkState = pinMkBlinkFast;
                    counterSize = 0;
                    counterPackegs = 0;
                    indexFirwareArray = 0;
                    sizeDecoded = 0;
                    indexTempArray16byte = 0;
                    indexFirwareArray = 0;
                    beginFirmware = false;
                }
            }

            osDelay(1);
        }
        else
        {
            osMutexRelease(mutexFirmwareRingBufferId);
            osDelay(10);
        }
        osDelay(1);
    }
}

/*!
 \brief Parsing Firmware from Json package

 \fn parsingFirmwareFromJson
 \param doc Json package
*/

void newFirmwareWrite(uint32_t firmwareSize)
{
    term2("md5 received == md5 calculated")
        pinNormaState = pinNormaBlinkFast;
    term2("Start erasing flash")
        eraseFlashBank(1);
    term2("Start writing flash")
        writeFlashFromExtRam(1);
        printMd5(1, firmwareSize);
        //Запоминаем в EEPROM firmwareSize
        lfs_file_open(&lfs, &file, "firmwareSize", LFS_O_RDWR | LFS_O_CREAT);
        lfs_file_write(&lfs, &file, &firmwareSize, sizeof(firmwareSize));
        lfs_file_close(&lfs, &file);

    //Нужно переключить банк памяти для новой загрузки
    static FLASH_OBProgramInitTypeDef OBInit;
    /* Get FLASH_WRP_SECTORS write protection status */
    HAL_FLASH_OB_Unlock();
    HAL_FLASHEx_OBGetConfig(&OBInit);
    //                   printFlashOptions(OBInit);
    /* Check Swap Flash banks  status */
    if ((OBInit.USERConfig & OB_SWAP_BANK_ENABLE) == OB_SWAP_BANK_DISABLE)
    {
        term2("swap bank")
            /*Swap to bank2 */
            /*Set OB SWAP_BANK_OPT to swap Bank2*/
            OBInit.OptionType = OPTIONBYTE_USER;
        OBInit.USERType   = OB_USER_SWAP_BANK;
        OBInit.USERConfig = OB_SWAP_BANK_ENABLE;
        HAL_FLASHEx_OBProgram(&OBInit);
        /* Launch Option bytes loading */
        HAL_FLASH_OB_Launch();
    }
    else
    {
        term2("swap bank")
            /* Swap to bank1 */
            /*Set OB SWAP_BANK_OPT to swap Bank1*/
            OBInit.OptionType = OPTIONBYTE_USER;
        OBInit.USERType = OB_USER_SWAP_BANK;
        OBInit.USERConfig = OB_SWAP_BANK_DISABLE;
        HAL_FLASHEx_OBProgram(&OBInit);
        /* Launch Option bytes loading */
        HAL_FLASH_OB_Launch();
    }
    //                   printFlashOptions(OBInit);
    //                   HAL_FLASH_OB_Lock();
    term2("reboot...\r\n")
        HAL_NVIC_SystemReset();
}

void parsingFirmwareFromJson(JsonDocument &doc)
{
    /* FatFs function common result code */
//    const char *cmd = doc["cmd"];
//    const char *station = doc["station"];
    if ((doc["cmd"] == "update")
        && (((uint8_t)doc["station"] == (uint8_t)ThisStation_.id)
            ||(doc["station"] == "all"))) //Заливаем на все устройства
    {
        int versionFirmware = doc["versionFirmware"];
        int subVersionFirmware = doc["subVersionFirmware"];
        int size = doc["size"];
        int current = doc["current"];
        int all = doc["all"];
        const char *data =  doc["data"];

        std::string dataStr(data);
        FirmwarePackage pack;

        pack.data = strHex(dataStr);
        pack.versionFirmware = versionFirmware;
        pack.subVersionFirmware = subVersionFirmware;
        pack.size = size;
        pack.current = current;
        pack.all = all;

        osMutexWait(mutexFirmwareRingBufferId, osWaitForever);
        firmwareRingBuffer.push(pack);
        osMutexRelease(mutexFirmwareRingBufferId);
    }
}

void rebootMcuByJson(JsonDocument &doc)
{
    int rebootId = doc["rebootId"];
    if (ThisStation_.id == rebootId)
    {
        HAL_NVIC_SystemReset();
    }
}

void deleteConfigMcuByJson(JsonDocument &doc)
{
    int deleteConfigId = doc["deleteConfigId"];
    if (ThisStation_.id == deleteConfigId)
    {
        lfs_remove(FsForEeprom::getInstance().lfsPtr, "boot_config");
    }
}

static int32_t commonSizeAllFrames = 0;
static int32_t counterFrames = 0;

void writeConfigMcuByJson(JsonDocument &doc)
{
    constexpr int32_t SIZE_DEF_BLOCK_UDP = 256;
    constexpr int32_t SIZE_WRITE_BLOCK = 128;

    uint32_t tmp[SIZE_DEF_BLOCK_UDP / 4];

    int writeConfigId = doc["writeConfigId"];
    if (ThisStation_.id == writeConfigId)
    {
if(counterFrames == 0)
{
    std::fill(allConfig, allConfig + sizeConfig, 0);
}
        int number =  doc["number"];
        if (number == counterFrames)
        {

            int all = doc["all"];
            int size = doc["size"];
RS232Puts("number\r\n");
term2(number)
            counterFrames++;
            commonSizeAllFrames += size;
            const char *config  = doc["config"];
            std::fill(tmp, tmp + SIZE_DEF_BLOCK_UDP / 4, 0);
            std::memcpy(tmp, config, SIZE_DEF_BLOCK_UDP);
//RS232Puts("tmp\r\n");
//term2((char*)tmp)

//            Это нерабочий метод который я меняю на memcpy
//            SRAM::getInstance()->writeData(
//            tmp,
//            SIZE_DEF_BLOCK_UDP,
//            reinterpret_cast<uint32_t *>(0x60020000 + number * SIZE_DEF_BLOCK_UDP));

            std::memcpy(allConfig + number * sizeof (tmp), tmp, sizeof (tmp));
//RS232Puts("config part\r\n");
//term2(config)
//RS232Puts("config All\r\n");
//term2(allConfig)
            if ((all == number) && (counterFrames > 0))
            {
RS232Puts("config size = ");
term2((uint8_t)commonSizeAllFrames)

                uint8_t readSramBuff[SIZE_WRITE_BLOCK] {0};
                lfs_remove(FsForEeprom::getInstance().lfsPtr, "boot_config");
                lfs_file_open(FsForEeprom::getInstance().lfsPtr, FsForEeprom::getInstance().filePtr, "boot_config", LFS_O_RDWR | LFS_O_CREAT);
                for (int32_t i = 0; i < commonSizeAllFrames; i += SIZE_WRITE_BLOCK)
                {
//                    Это нерабочий метод который я меняю на memcpy
//                    SRAM::getInstance()->readData(
//                    reinterpret_cast<uint32_t *>(readSramBuff),
//                    SIZE_WRITE_BLOCK,
//                    reinterpret_cast<uint32_t *>(0x60020000 + i));

                    std::memcpy(readSramBuff,
                    allConfig + i,
                    commonSizeAllFrames - i < SIZE_WRITE_BLOCK ? static_cast<uint32_t>(commonSizeAllFrames - i) : SIZE_WRITE_BLOCK);

//term2(readSramBuff)

                    lfs_file_write(FsForEeprom::getInstance().lfsPtr, FsForEeprom::getInstance().filePtr, readSramBuff,  commonSizeAllFrames - i < SIZE_WRITE_BLOCK ? static_cast<uint32_t>(commonSizeAllFrames - i) : SIZE_WRITE_BLOCK);
                }

                lfs_file_close(FsForEeprom::getInstance().lfsPtr, FsForEeprom::getInstance().filePtr);

                //send msg configuration write OK
                const int capacity = JSON_OBJECT_SIZE(3);
                StaticJsonDocument<capacity> configDoc;

                configDoc["writeConfigId"] = ThisStation_.id;
                configDoc["status"].set("ok");
//                CallControl_->sendJson(configDoc, capacity/*, UDP_PORT*/);

            term2("write config OK")

                std::fill(CallControl_->messageData.txBuff, CallControl_->messageData.txBuff + CallControl_->messageData.txBuffSize, 0);
                if (serializeJson(configDoc, CallControl_->messageData.txBuff, capacity) > 0)
                {
                    sendUdpMulticast(CallControl_->messageData.txBuff, strlen(CallControl_->messageData.txBuff));
                }

                commonSizeAllFrames = 0;
                counterFrames = 0;

                osDelay(1000);
                HAL_NVIC_SystemReset();

            }
        }
    }
}

void eraseFlashBank(int numBank)
{
//    taskENTER_CRITICAL();
    uint32_t SECTORError = 0;
    static FLASH_EraseInitTypeDef EraseInitStruct;
    EraseInitStruct.TypeErase     = FLASH_TYPEERASE_MASSERASE;
    if(numBank == 0)
        EraseInitStruct.Banks         = FLASH_BANK_1;
    if(numBank == 1)
        EraseInitStruct.Banks         = FLASH_BANK_2;
    if(numBank > 1)
        EraseInitStruct.Banks         = FLASH_BANK_BOTH;
    //    EraseInitStruct.VoltageRange  = FLASH_VOLTAGE_RANGE_3;
    //    EraseInitStruct.VoltageRange  = FLASH_VOLTAGE_RANGE_1;
    //    EraseInitStruct.Sector        = FLASH_SECTOR_0;
    //    EraseInitStruct.NbSectors     = 4;

    Flash::getInstance().unlock();
    while (HAL_FLASHEx_Erase(&EraseInitStruct, &SECTORError) != HAL_OK)
    {
        RS232::getInstance().term << "Error erase.\r\n";
    }
    while (FLASH_WaitForLastOperation(100000, FLASH_BANK_BOTH) != HAL_OK);
    Flash::getInstance().lock();

//    taskEXIT_CRITICAL();
}

void writeFlashFromExtRam(int numBank)
{
//    taskENTER_CRITICAL();
    uint32_t writeADDR;
    if(numBank == 0)
        writeADDR = Flash::getInstance().ADDR_FLASH_BANK_1;
    else
        writeADDR = Flash::getInstance().ADDR_FLASH_BANK_2;

    static FLASH_OBProgramInitTypeDef OBInit;

    HAL_FLASH_OB_Unlock();
    HAL_FLASHEx_OBGetConfig(&OBInit);
    HAL_FLASH_OB_Lock();

    Flash::getInstance().unlock();
    while (FLASH_WaitForLastOperation(100000, FLASH_BANK_BOTH) != HAL_OK);
    Flash::getInstance().write(writeADDR, reinterpret_cast<const char *>(DataFirmware), 1024 * 128 * 4);
    while (FLASH_WaitForLastOperation(100000, FLASH_BANK_BOTH) != HAL_OK);
    Flash::getInstance().lock();

//    taskEXIT_CRITICAL();
}

void writeFirmwareFromBank0ToBank1()
{
    taskENTER_CRITICAL();

    Flash::getInstance().unlock();
    while (FLASH_WaitForLastOperation(100000, FLASH_BANK_BOTH) != HAL_OK);
    Flash::getInstance().write(Flash::getInstance().ADDR_FLASH_BANK_2, reinterpret_cast<const char *>(Flash::getInstance().ADDR_FLASH_BANK_1), 1024 * 128 * 4);
    while (FLASH_WaitForLastOperation(100000, FLASH_BANK_BOTH) != HAL_OK);
    Flash::getInstance().lock();

    taskEXIT_CRITICAL();
}

void printFlashOptions(FLASH_OBProgramInitTypeDef &OBInit)
{
    char tmp[128];
    sprintf(tmp, "OptionType=%X "
                 "WRPState=%X "
                 "WRPSector=%X "
                 "BORLevel=%X "
                 "USERType=%X "
                 "USERConfig=%X "
                 "Banks=%X"
                 ,
                 (unsigned)OBInit.OptionType,
                 (unsigned)OBInit.WRPState,
                 (unsigned)OBInit.WRPSector,
                 (unsigned)OBInit.BORLevel,
                 (unsigned)OBInit.USERType,
                 (unsigned)OBInit.USERConfig,
                 (unsigned)OBInit.Banks
                 );
    term2(tmp);
}

//Перенес сюда aes.c
/*****************************************************************************/
/* Includes:                                                                 */
/*****************************************************************************/
#include <stdint.h>
#include <string.h> // CBC mode, for memset
#include "aes.h"


/*****************************************************************************/
/* Defines:                                                                  */
/*****************************************************************************/
// The number of columns comprising a state in AES. This is a constant in AES. Value=4
#define Nb 4
// The number of 32 bit words in a key.
#define Nk 4
// Key length in bytes [128 bit]
#define KEYLEN 16
// The number of rounds in AES Cipher.
#define Nr 10

// jcallan@github points out that declaring Multiply as a function
// reduces code size considerably with the Keil ARM compiler.
// See this link for more information: https://github.com/kokke/tiny-AES128-C/pull/3
#ifndef MULTIPLY_AS_A_FUNCTION
#define MULTIPLY_AS_A_FUNCTION 0
#endif


/*****************************************************************************/
/* Private variables:                                                        */
/*****************************************************************************/
// state - array holding the intermediate results during decryption.
typedef uint8_t state_t[4][4];
static state_t *state;

// The array that stores the round keys.
static uint8_t RoundKey[176];

// The Key input to the AES Program
static const uint8_t *Key;

#if defined(CBC) && CBC
// Initial Vector used only for CBC mode
static uint8_t *Iv;
#endif

// The lookup-tables are marked const so they can be placed in read-only storage instead of RAM
// The numbers below can be computed dynamically trading ROM for RAM -
// This can be useful in (embedded) bootloader applications, where ROM is often limited.
static const uint8_t sbox[256] =   {
    //0     1    2      3     4    5     6     7      8    9     A      B    C     D     E     F
    0x63, 0x7c, 0x77, 0x7b, 0xf2, 0x6b, 0x6f, 0xc5, 0x30, 0x01, 0x67, 0x2b, 0xfe, 0xd7, 0xab, 0x76,
    0xca, 0x82, 0xc9, 0x7d, 0xfa, 0x59, 0x47, 0xf0, 0xad, 0xd4, 0xa2, 0xaf, 0x9c, 0xa4, 0x72, 0xc0,
    0xb7, 0xfd, 0x93, 0x26, 0x36, 0x3f, 0xf7, 0xcc, 0x34, 0xa5, 0xe5, 0xf1, 0x71, 0xd8, 0x31, 0x15,
    0x04, 0xc7, 0x23, 0xc3, 0x18, 0x96, 0x05, 0x9a, 0x07, 0x12, 0x80, 0xe2, 0xeb, 0x27, 0xb2, 0x75,
    0x09, 0x83, 0x2c, 0x1a, 0x1b, 0x6e, 0x5a, 0xa0, 0x52, 0x3b, 0xd6, 0xb3, 0x29, 0xe3, 0x2f, 0x84,
    0x53, 0xd1, 0x00, 0xed, 0x20, 0xfc, 0xb1, 0x5b, 0x6a, 0xcb, 0xbe, 0x39, 0x4a, 0x4c, 0x58, 0xcf,
    0xd0, 0xef, 0xaa, 0xfb, 0x43, 0x4d, 0x33, 0x85, 0x45, 0xf9, 0x02, 0x7f, 0x50, 0x3c, 0x9f, 0xa8,
    0x51, 0xa3, 0x40, 0x8f, 0x92, 0x9d, 0x38, 0xf5, 0xbc, 0xb6, 0xda, 0x21, 0x10, 0xff, 0xf3, 0xd2,
    0xcd, 0x0c, 0x13, 0xec, 0x5f, 0x97, 0x44, 0x17, 0xc4, 0xa7, 0x7e, 0x3d, 0x64, 0x5d, 0x19, 0x73,
    0x60, 0x81, 0x4f, 0xdc, 0x22, 0x2a, 0x90, 0x88, 0x46, 0xee, 0xb8, 0x14, 0xde, 0x5e, 0x0b, 0xdb,
    0xe0, 0x32, 0x3a, 0x0a, 0x49, 0x06, 0x24, 0x5c, 0xc2, 0xd3, 0xac, 0x62, 0x91, 0x95, 0xe4, 0x79,
    0xe7, 0xc8, 0x37, 0x6d, 0x8d, 0xd5, 0x4e, 0xa9, 0x6c, 0x56, 0xf4, 0xea, 0x65, 0x7a, 0xae, 0x08,
    0xba, 0x78, 0x25, 0x2e, 0x1c, 0xa6, 0xb4, 0xc6, 0xe8, 0xdd, 0x74, 0x1f, 0x4b, 0xbd, 0x8b, 0x8a,
    0x70, 0x3e, 0xb5, 0x66, 0x48, 0x03, 0xf6, 0x0e, 0x61, 0x35, 0x57, 0xb9, 0x86, 0xc1, 0x1d, 0x9e,
    0xe1, 0xf8, 0x98, 0x11, 0x69, 0xd9, 0x8e, 0x94, 0x9b, 0x1e, 0x87, 0xe9, 0xce, 0x55, 0x28, 0xdf,
    0x8c, 0xa1, 0x89, 0x0d, 0xbf, 0xe6, 0x42, 0x68, 0x41, 0x99, 0x2d, 0x0f, 0xb0, 0x54, 0xbb, 0x16
};

static const uint8_t rsbox[256] = {
    0x52, 0x09, 0x6a, 0xd5, 0x30, 0x36, 0xa5, 0x38, 0xbf, 0x40, 0xa3, 0x9e, 0x81, 0xf3, 0xd7, 0xfb,
    0x7c, 0xe3, 0x39, 0x82, 0x9b, 0x2f, 0xff, 0x87, 0x34, 0x8e, 0x43, 0x44, 0xc4, 0xde, 0xe9, 0xcb,
    0x54, 0x7b, 0x94, 0x32, 0xa6, 0xc2, 0x23, 0x3d, 0xee, 0x4c, 0x95, 0x0b, 0x42, 0xfa, 0xc3, 0x4e,
    0x08, 0x2e, 0xa1, 0x66, 0x28, 0xd9, 0x24, 0xb2, 0x76, 0x5b, 0xa2, 0x49, 0x6d, 0x8b, 0xd1, 0x25,
    0x72, 0xf8, 0xf6, 0x64, 0x86, 0x68, 0x98, 0x16, 0xd4, 0xa4, 0x5c, 0xcc, 0x5d, 0x65, 0xb6, 0x92,
    0x6c, 0x70, 0x48, 0x50, 0xfd, 0xed, 0xb9, 0xda, 0x5e, 0x15, 0x46, 0x57, 0xa7, 0x8d, 0x9d, 0x84,
    0x90, 0xd8, 0xab, 0x00, 0x8c, 0xbc, 0xd3, 0x0a, 0xf7, 0xe4, 0x58, 0x05, 0xb8, 0xb3, 0x45, 0x06,
    0xd0, 0x2c, 0x1e, 0x8f, 0xca, 0x3f, 0x0f, 0x02, 0xc1, 0xaf, 0xbd, 0x03, 0x01, 0x13, 0x8a, 0x6b,
    0x3a, 0x91, 0x11, 0x41, 0x4f, 0x67, 0xdc, 0xea, 0x97, 0xf2, 0xcf, 0xce, 0xf0, 0xb4, 0xe6, 0x73,
    0x96, 0xac, 0x74, 0x22, 0xe7, 0xad, 0x35, 0x85, 0xe2, 0xf9, 0x37, 0xe8, 0x1c, 0x75, 0xdf, 0x6e,
    0x47, 0xf1, 0x1a, 0x71, 0x1d, 0x29, 0xc5, 0x89, 0x6f, 0xb7, 0x62, 0x0e, 0xaa, 0x18, 0xbe, 0x1b,
    0xfc, 0x56, 0x3e, 0x4b, 0xc6, 0xd2, 0x79, 0x20, 0x9a, 0xdb, 0xc0, 0xfe, 0x78, 0xcd, 0x5a, 0xf4,
    0x1f, 0xdd, 0xa8, 0x33, 0x88, 0x07, 0xc7, 0x31, 0xb1, 0x12, 0x10, 0x59, 0x27, 0x80, 0xec, 0x5f,
    0x60, 0x51, 0x7f, 0xa9, 0x19, 0xb5, 0x4a, 0x0d, 0x2d, 0xe5, 0x7a, 0x9f, 0x93, 0xc9, 0x9c, 0xef,
    0xa0, 0xe0, 0x3b, 0x4d, 0xae, 0x2a, 0xf5, 0xb0, 0xc8, 0xeb, 0xbb, 0x3c, 0x83, 0x53, 0x99, 0x61,
    0x17, 0x2b, 0x04, 0x7e, 0xba, 0x77, 0xd6, 0x26, 0xe1, 0x69, 0x14, 0x63, 0x55, 0x21, 0x0c, 0x7d
};


// The round constant word array, Rcon[i], contains the values given by
// x to th e power (i-1) being powers of x (x is denoted as {02}) in the field GF(2^8)
// Note that i starts at 1, not 0).
static const uint8_t Rcon[255] = {
    0x8d, 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1b, 0x36, 0x6c, 0xd8, 0xab, 0x4d, 0x9a,
    0x2f, 0x5e, 0xbc, 0x63, 0xc6, 0x97, 0x35, 0x6a, 0xd4, 0xb3, 0x7d, 0xfa, 0xef, 0xc5, 0x91, 0x39,
    0x72, 0xe4, 0xd3, 0xbd, 0x61, 0xc2, 0x9f, 0x25, 0x4a, 0x94, 0x33, 0x66, 0xcc, 0x83, 0x1d, 0x3a,
    0x74, 0xe8, 0xcb, 0x8d, 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1b, 0x36, 0x6c, 0xd8,
    0xab, 0x4d, 0x9a, 0x2f, 0x5e, 0xbc, 0x63, 0xc6, 0x97, 0x35, 0x6a, 0xd4, 0xb3, 0x7d, 0xfa, 0xef,
    0xc5, 0x91, 0x39, 0x72, 0xe4, 0xd3, 0xbd, 0x61, 0xc2, 0x9f, 0x25, 0x4a, 0x94, 0x33, 0x66, 0xcc,
    0x83, 0x1d, 0x3a, 0x74, 0xe8, 0xcb, 0x8d, 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1b,
    0x36, 0x6c, 0xd8, 0xab, 0x4d, 0x9a, 0x2f, 0x5e, 0xbc, 0x63, 0xc6, 0x97, 0x35, 0x6a, 0xd4, 0xb3,
    0x7d, 0xfa, 0xef, 0xc5, 0x91, 0x39, 0x72, 0xe4, 0xd3, 0xbd, 0x61, 0xc2, 0x9f, 0x25, 0x4a, 0x94,
    0x33, 0x66, 0xcc, 0x83, 0x1d, 0x3a, 0x74, 0xe8, 0xcb, 0x8d, 0x01, 0x02, 0x04, 0x08, 0x10, 0x20,
    0x40, 0x80, 0x1b, 0x36, 0x6c, 0xd8, 0xab, 0x4d, 0x9a, 0x2f, 0x5e, 0xbc, 0x63, 0xc6, 0x97, 0x35,
    0x6a, 0xd4, 0xb3, 0x7d, 0xfa, 0xef, 0xc5, 0x91, 0x39, 0x72, 0xe4, 0xd3, 0xbd, 0x61, 0xc2, 0x9f,
    0x25, 0x4a, 0x94, 0x33, 0x66, 0xcc, 0x83, 0x1d, 0x3a, 0x74, 0xe8, 0xcb, 0x8d, 0x01, 0x02, 0x04,
    0x08, 0x10, 0x20, 0x40, 0x80, 0x1b, 0x36, 0x6c, 0xd8, 0xab, 0x4d, 0x9a, 0x2f, 0x5e, 0xbc, 0x63,
    0xc6, 0x97, 0x35, 0x6a, 0xd4, 0xb3, 0x7d, 0xfa, 0xef, 0xc5, 0x91, 0x39, 0x72, 0xe4, 0xd3, 0xbd,
    0x61, 0xc2, 0x9f, 0x25, 0x4a, 0x94, 0x33, 0x66, 0xcc, 0x83, 0x1d, 0x3a, 0x74, 0xe8, 0xcb
};


/*****************************************************************************/
/* Private functions:                                                        */
/*****************************************************************************/
static uint8_t getSBoxValue(uint8_t num)
{
    return sbox[num];
}

static uint8_t getSBoxInvert(uint8_t num)
{
    return rsbox[num];
}

// This function produces Nb(Nr+1) round keys. The round keys are used in each round to decrypt the states.
static void KeyExpansion(void)
{
    uint32_t i, j, k;
    uint8_t tempa[4]; // Used for the column/row operations

    // The first round key is the key itself.
    for (i = 0; i < Nk; ++i) {
        RoundKey[(i * 4) + 0] = Key[(i * 4) + 0];
        RoundKey[(i * 4) + 1] = Key[(i * 4) + 1];
        RoundKey[(i * 4) + 2] = Key[(i * 4) + 2];
        RoundKey[(i * 4) + 3] = Key[(i * 4) + 3];
    }

    // All other round keys are found from the previous round keys.
    for (; (i < (Nb * (Nr + 1))); ++i) {
        for (j = 0; j < 4; ++j) {
            tempa[j] = RoundKey[(i - 1) * 4 + j];
        }
        if (i % Nk == 0) {
            // This function rotates the 4 bytes in a word to the left once.
            // [a0,a1,a2,a3] becomes [a1,a2,a3,a0]

            // Function RotWord()
            {
                k = tempa[0];
                tempa[0] = tempa[1];
                tempa[1] = tempa[2];
                tempa[2] = tempa[3];
                tempa[3] = k;
            }

            // SubWord() is a function that takes a four-byte input word and
            // applies the S-box to each of the four bytes to produce an output word.

            // Function Subword()
            {
                tempa[0] = getSBoxValue(tempa[0]);
                tempa[1] = getSBoxValue(tempa[1]);
                tempa[2] = getSBoxValue(tempa[2]);
                tempa[3] = getSBoxValue(tempa[3]);
            }

            tempa[0] =  tempa[0] ^ Rcon[i / Nk];
        } else if (Nk > 6 && i % Nk == 4) {
            // Function Subword()
            {
                tempa[0] = getSBoxValue(tempa[0]);
                tempa[1] = getSBoxValue(tempa[1]);
                tempa[2] = getSBoxValue(tempa[2]);
                tempa[3] = getSBoxValue(tempa[3]);
            }
        }
        RoundKey[i * 4 + 0] = RoundKey[(i - Nk) * 4 + 0] ^ tempa[0];
        RoundKey[i * 4 + 1] = RoundKey[(i - Nk) * 4 + 1] ^ tempa[1];
        RoundKey[i * 4 + 2] = RoundKey[(i - Nk) * 4 + 2] ^ tempa[2];
        RoundKey[i * 4 + 3] = RoundKey[(i - Nk) * 4 + 3] ^ tempa[3];
    }
}

// This function adds the round key to state.
// The round key is added to the state by an XOR function.
static void AddRoundKey(uint8_t round)
{
    uint8_t i, j;
    for (i = 0; i < 4; ++i) {
        for (j = 0; j < 4; ++j) {
            (*state)[i][j] ^= RoundKey[round * Nb * 4 + i * Nb + j];
        }
    }
}

// The SubBytes Function Substitutes the values in the
// state matrix with values in an S-box.
static void SubBytes(void)
{
    uint8_t i, j;
    for (i = 0; i < 4; ++i) {
        for (j = 0; j < 4; ++j) {
            (*state)[j][i] = getSBoxValue((*state)[j][i]);
        }
    }
}

// The ShiftRows() function shifts the rows in the state to the left.
// Each row is shifted with different offset.
// Offset = Row number. So the first row is not shifted.
static void ShiftRows(void)
{
    uint8_t temp;

    // Rotate first row 1 columns to left
    temp           = (*state)[0][1];
    (*state)[0][1] = (*state)[1][1];
    (*state)[1][1] = (*state)[2][1];
    (*state)[2][1] = (*state)[3][1];
    (*state)[3][1] = temp;

    // Rotate second row 2 columns to left
    temp           = (*state)[0][2];
    (*state)[0][2] = (*state)[2][2];
    (*state)[2][2] = temp;

    temp       = (*state)[1][2];
    (*state)[1][2] = (*state)[3][2];
    (*state)[3][2] = temp;

    // Rotate third row 3 columns to left
    temp       = (*state)[0][3];
    (*state)[0][3] = (*state)[3][3];
    (*state)[3][3] = (*state)[2][3];
    (*state)[2][3] = (*state)[1][3];
    (*state)[1][3] = temp;
}

static uint8_t xtime(uint8_t x)
{
    return ((x << 1) ^ (((x >> 7) & 1) * 0x1b));
}

// MixColumns function mixes the columns of the state matrix
static void MixColumns(void)
{
    uint8_t i;
    uint8_t Tmp, Tm, t;
    for (i = 0; i < 4; ++i) {
        t   = (*state)[i][0];
        Tmp = (*state)[i][0] ^ (*state)[i][1] ^ (*state)[i][2] ^ (*state)[i][3] ;
        Tm  = (*state)[i][0] ^ (*state)[i][1] ;
        Tm = xtime(Tm);
        (*state)[i][0] ^= Tm ^ Tmp ;
        Tm  = (*state)[i][1] ^ (*state)[i][2] ;
        Tm = xtime(Tm);
        (*state)[i][1] ^= Tm ^ Tmp ;
        Tm  = (*state)[i][2] ^ (*state)[i][3] ;
        Tm = xtime(Tm);
        (*state)[i][2] ^= Tm ^ Tmp ;
        Tm  = (*state)[i][3] ^ t ;
        Tm = xtime(Tm);
        (*state)[i][3] ^= Tm ^ Tmp ;
    }
}

// Multiply is used to multiply numbers in the field GF(2^8)
#if MULTIPLY_AS_A_FUNCTION
static uint8_t Multiply(uint8_t x, uint8_t y)
{
    return (((y & 1) * x) ^
            ((y >> 1 & 1) * xtime(x)) ^
            ((y >> 2 & 1) * xtime(xtime(x))) ^
            ((y >> 3 & 1) * xtime(xtime(xtime(x)))) ^
            ((y >> 4 & 1) * xtime(xtime(xtime(xtime(x))))));
}
#else
#define Multiply(x, y)                                \
      (  ((y & 1) * x) ^                              \
      ((y>>1 & 1) * xtime(x)) ^                       \
      ((y>>2 & 1) * xtime(xtime(x))) ^                \
      ((y>>3 & 1) * xtime(xtime(xtime(x)))) ^         \
      ((y>>4 & 1) * xtime(xtime(xtime(xtime(x))))))   \

#endif

// MixColumns function mixes the columns of the state matrix.
// The method used to multiply may be difficult to understand for the inexperienced.
// Please use the references to gain more information.
static void InvMixColumns(void)
{
    int i;
    uint8_t a, b, c, d;
    for (i = 0; i < 4; ++i) {
        a = (*state)[i][0];
        b = (*state)[i][1];
        c = (*state)[i][2];
        d = (*state)[i][3];

        (*state)[i][0] = Multiply(a, 0x0e) ^ Multiply(b, 0x0b) ^ Multiply(c, 0x0d) ^ Multiply(d, 0x09);
        (*state)[i][1] = Multiply(a, 0x09) ^ Multiply(b, 0x0e) ^ Multiply(c, 0x0b) ^ Multiply(d, 0x0d);
        (*state)[i][2] = Multiply(a, 0x0d) ^ Multiply(b, 0x09) ^ Multiply(c, 0x0e) ^ Multiply(d, 0x0b);
        (*state)[i][3] = Multiply(a, 0x0b) ^ Multiply(b, 0x0d) ^ Multiply(c, 0x09) ^ Multiply(d, 0x0e);
    }
}


// The SubBytes Function Substitutes the values in the
// state matrix with values in an S-box.
static void InvSubBytes(void)
{
    uint8_t i, j;
    for (i = 0; i < 4; ++i) {
        for (j = 0; j < 4; ++j) {
            (*state)[j][i] = getSBoxInvert((*state)[j][i]);
        }
    }
}

static void InvShiftRows(void)
{
    uint8_t temp;

    // Rotate first row 1 columns to right
    temp = (*state)[3][1];
    (*state)[3][1] = (*state)[2][1];
    (*state)[2][1] = (*state)[1][1];
    (*state)[1][1] = (*state)[0][1];
    (*state)[0][1] = temp;

    // Rotate second row 2 columns to right
    temp = (*state)[0][2];
    (*state)[0][2] = (*state)[2][2];
    (*state)[2][2] = temp;

    temp = (*state)[1][2];
    (*state)[1][2] = (*state)[3][2];
    (*state)[3][2] = temp;

    // Rotate third row 3 columns to right
    temp = (*state)[0][3];
    (*state)[0][3] = (*state)[1][3];
    (*state)[1][3] = (*state)[2][3];
    (*state)[2][3] = (*state)[3][3];
    (*state)[3][3] = temp;
}


// Cipher is the main function that encrypts the PlainText.
static void Cipher(void)
{
    uint8_t round = 0;

    // Add the First round key to the state before starting the rounds.
    AddRoundKey(0);

    // There will be Nr rounds.
    // The first Nr-1 rounds are identical.
    // These Nr-1 rounds are executed in the loop below.
    for (round = 1; round < Nr; ++round) {
        SubBytes();
        ShiftRows();
        MixColumns();
        AddRoundKey(round);
    }

    // The last round is given below.
    // The MixColumns function is not here in the last round.
    SubBytes();
    ShiftRows();
    AddRoundKey(Nr);
}

static void InvCipher(void)
{
    uint8_t round = 0;

    // Add the First round key to the state before starting the rounds.
    AddRoundKey(Nr);

    // There will be Nr rounds.
    // The first Nr-1 rounds are identical.
    // These Nr-1 rounds are executed in the loop below.
    for (round = Nr - 1; round > 0; round--) {
        InvShiftRows();
        InvSubBytes();
        AddRoundKey(round);
        InvMixColumns();
    }

    // The last round is given below.
    // The MixColumns function is not here in the last round.
    InvShiftRows();
    InvSubBytes();
    AddRoundKey(0);
}

static void BlockCopy(uint8_t *output, uint8_t *input)
{
    uint8_t i;
    for (i = 0; i < KEYLEN; ++i) {
        output[i] = input[i];
    }
}



/*****************************************************************************/
/* Public functions:                                                         */
/*****************************************************************************/
#if defined(ECB) && ECB


void AES128_ECB_encrypt(uint8_t *input, const uint8_t *key, uint8_t *output)
{
    // Copy input to output, and work in-memory on output
    BlockCopy(output, input);
    state = (state_t *)output;

    Key = key;
    KeyExpansion();

    // The next function call encrypts the PlainText with the Key using AES algorithm.
    Cipher();
}

void AES128_ECB_decrypt(uint8_t *input, const uint8_t *key, uint8_t *output)
{
    // Copy input to output, and work in-memory on output
    BlockCopy(output, input);
    state = (state_t *)output;

    // The KeyExpansion routine must be called before encryption.
    Key = key;
    KeyExpansion();

    InvCipher();
}


#endif // #if defined(ECB) && ECB





#if defined(CBC) && CBC


static void XorWithIv(uint8_t *buf)
{
    uint8_t i;
    for (i = 0; i < KEYLEN; ++i) {
        buf[i] ^= Iv[i];
    }
}

void AES128_CBC_encrypt_buffer(uint8_t *output, uint8_t *input, uint32_t length, const uint8_t *key, const uint8_t *iv)
{
    uintptr_t i;
    uint8_t remainders = length % KEYLEN; /* Remaining bytes in the last non-full block */

    BlockCopy(output, input);
    state = (state_t *)output;

    // Skip the key expansion if key is passed as 0
    if (0 != key) {
        Key = key;
        KeyExpansion();
    }

    if (iv != 0) {
        Iv = (uint8_t *)iv;
    }

    for (i = 0; i < length; i += KEYLEN) {
        XorWithIv(input);
        BlockCopy(output, input);
        state = (state_t *)output;
        Cipher();
        Iv = output;
        input += KEYLEN;
        output += KEYLEN;
    }

    if (remainders) {
        BlockCopy(output, input);
        memset(output + remainders, 0, KEYLEN - remainders); /* add 0-padding */
        state = (state_t *)output;
        Cipher();
    }
}

void AES128_CBC_decrypt_buffer(uint8_t *output, uint8_t *input, uint32_t length, const uint8_t *key, const uint8_t *iv)
{
    uintptr_t i;
    uint8_t remainders = length % KEYLEN; /* Remaining bytes in the last non-full block */

    BlockCopy(output, input);
    state = (state_t *)output;

    // Skip the key expansion if key is passed as 0
    if (0 != key) {
        Key = key;
        KeyExpansion();
    }

    // If iv is passed as 0, we continue to encrypt without re-setting the Iv
    if (iv != 0) {
        Iv = (uint8_t *)iv;
    }

    for (i = 0; i < length; i += KEYLEN) {
        BlockCopy(output, input);
        state = (state_t *)output;
        InvCipher();
        XorWithIv(output);
        Iv = input;
        input += KEYLEN;
        output += KEYLEN;
    }

    if (remainders) {
        BlockCopy(output, input);
        memset(output + remainders, 0, KEYLEN - remainders); /* add 0-padding */
        state = (state_t *)output;
        InvCipher();
    }
}


#endif // #if defined(CBC) && CBC
