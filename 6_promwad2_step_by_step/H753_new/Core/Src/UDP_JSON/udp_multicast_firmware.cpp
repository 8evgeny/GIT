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
//#ifndef SC2BOARD
////#include "call_control.h"
//#else
//#include "../Call_control_for_SC2_board/call_control_sc2.h"
//#endif
void printFlashOptions(FLASH_OBProgramInitTypeDef &OBInit);
extern CRC_HandleTypeDef hcrc;
extern uint8_t DataFirmware[NUM_FIRMWARE_PACKET][SIZE_FIRMWARE_BASE] __attribute__((section(".ExtRamData")));
extern char *allConfig;
extern int sizeConfig;
extern uint8_t pinNormaState;
//char allConfigExtRam[1024 * 100] __attribute__((section(".ExtRamData")));

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

static FATFS FLASHFatFs;  /*! File system object for Flash logical drive */
static FIL MyFile;     /*! File object */
static char FLASHPath[4]; /*! FLASH logical drive path */
static uint8_t workBuffer[1024]; /*! Work buffer */

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

/*!
 \brief The thread for updating the firmware

 \fn updateFirmwareThread
 \param arg Doesn't need
*/
[[ noreturn ]]void updateFirmwareThread(const void *arg);

/*!
 \brief Create a thread for updating the firmware

 \fn firmwareInitThread
*/
void firmwareInitThread()
{
    osMutexDef(mutexFirmware);
    mutexFirmwareRingBufferId = osMutexCreate(osMutex(mutexFirmware));

    osThreadDef(handelFirmwareThread, updateFirmwareThread, osPriorityNormal, 0, configMINIMAL_STACK_SIZE * 3);
    firmwareThreadId = osThreadCreate(osThread(handelFirmwareThread), NULL);
}

static uint32_t counterSize = 0; /*! A counter for size of data */
static int counterPackegs = 0; /*! A counter for size of packages */

[[ noreturn ]]void updateFirmwareThread(const void *arg)
{
    char tmp[256];
    UNUSED(arg);
    uint32_t byteswritten{0};
    FRESULT res;
    bool calculateCRC = false;
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
                if (pack.current == 0) //Первый пакет
                {
                    pinNormaState = pinNormaBlink;
                    //always erase
//                    Flash::getInstance().erase();

                    //Очищаем массив под прошивку
                    for (int k = 0; k < NUM_FIRMWARE_PACKET; ++k)
                    {
                        for (int i = 0; i < SIZE_FIRMWARE_BASE; ++i)
                        {
                           DataFirmware[k][i] = 0x00;
                        }
                    }
                    counterSize = 0;
                    counterPackegs = 0;
                    uint32_t CRCVal = HAL_CRC_Calculate(&hcrc, (uint32_t *)DataFirmware, SIZE_FIRMWARE_BASE * NUM_FIRMWARE_PACKET /4);
                    sprintf(tmp,"DataFirmwareStart CRC =  %X", CRCVal);
                    term2(tmp)
//                    /*##-1- Link the micro Flash I/O driver ##################################*/
//                    FATFS_LinkDriver(&FLASH_Driver, FLASHPath);
//                    /*##-2- Register the file system object to the FatFs module ##############*/
//                    res = f_mount(&FLASHFatFs, (TCHAR const *)FLASHPath, 0);
//                    /*##-3- Create a FAT file system (format) on the logical drive #########*/
//                    /* WARNING: Formatting the Flash will delete all content on the device */
//                    res = f_mkfs((TCHAR const *)FLASHPath, FS_FAT32, 0, workBuffer, sizeof(workBuffer));

//                    /*##-4- Create and Open a new text file object with write access #####*/
//                    res = f_open(&MyFile, "BIN", FA_CREATE_ALWAYS | FA_WRITE);
//                    /*##-5- Write data to the text file ################################*/

//                    res = f_write(&MyFile, pack.data.data(), pack.data.size(), (UINT *)&byteswritten);

                }
                else if (pack.current == pack.all) //Последний пакет
                {
                    calculateCRC = true;

//                    //this packeage can be less than 512 bytes
////                    res = f_write(&MyFile, pack.data.data(), pack.data.size(), (UINT *)&byteswritten);
//                    res = f_write(&MyFile, pack.data.data(), size / 2, (UINT *)&byteswritten);
//                    counterSize += pack.data.size();
//                    counterPackegs++;

//                    /*##-6- Close the open text file #################################*/
//                    f_close(&MyFile);

//                    Flash::getInstance().flush();
//                    /*##-11- Unlink the Flash I/O driver ###############################*/
//                    FATFS_UnLinkDriver(FLASHPath);
//                    //resety
//                    counterPackegs = 0;

//                    //send msg firmware write OK
//                    //send msg configuration write OK
//                    const int capacity = JSON_OBJECT_SIZE(3);
//                    StaticJsonDocument<capacity> firmwareDoc;

//                    firmwareDoc["writeFirmwareId"] = Json::getInstance()->thisStation.id;
//                    firmwareDoc["status"].set("ok");
////                    UdpJsonExch::getInstance()->callControl->sendJson(firmwareDoc, capacity/*, UDP_PORT*/);

//                    std::fill(UdpJsonExch::getInstance()->callControl->messageData.txBuff, UdpJsonExch::getInstance()->callControl->messageData.txBuff + UdpJsonExch::getInstance()->callControl->messageData.txBuffSize, 0);
//                    if (serializeJson(firmwareDoc, UdpJsonExch::getInstance()->callControl->messageData.txBuff, capacity) > 0) {
//                        sendUdpMulticast(UdpJsonExch::getInstance()->callControl->messageData.txBuff, strlen(UdpJsonExch::getInstance()->callControl->messageData.txBuff));
//                    }

                } else
                {
//                    res = f_write(&MyFile,  pack.data.data(),  pack.data.size(), (UINT *)&byteswritten);
                }
                counterSize += pack.data.size();
                counterPackegs++;

            }

            sprintf(tmp,"packet %d of %d size packet = %d data size = %d", pack.current, pack.all, pack.size, counterSize);

            for (int i = 0; i < SIZE_FIRMWARE_BASE; ++i)
            {
                DataFirmware[pack.current][i] = pack.data.at(i);
            }
            term2(tmp)

            if (calculateCRC)
            {//Прошивка вся в SRAM - считать CRC BUFFER_SIZE указывается не в байтах, а в количестве 32-разрядных слов.
                uint32_t CRCVal = HAL_CRC_Calculate(&hcrc, (uint32_t *)DataFirmware, 1024 * 128);
                sprintf(tmp,"DataFirmwareEnd CRC =  %X", CRCVal);
                term2(tmp)

            term2("Start erasing flash")
            eraseFlashBank(1);
            term2("Start writing flash")
            writeFlashFromExtRam(1);

            //Нужно переключить банк памяти для новой загрузки
            static FLASH_OBProgramInitTypeDef OBInit;




            /* Get FLASH_WRP_SECTORS write protection status */
            HAL_FLASH_OB_Unlock();
//            OBInit.Banks     = FLASH_BANK_1;
            HAL_FLASHEx_OBGetConfig(&OBInit);
            printFlashOptions(OBInit);
            /* Check Swap Flash banks  status */
            if ((OBInit.USERConfig & OB_SWAP_BANK_ENABLE) == OB_SWAP_BANK_DISABLE)
            {
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
              /* Swap to bank1 */
              /*Set OB SWAP_BANK_OPT to swap Bank1*/
              OBInit.OptionType = OPTIONBYTE_USER;
              OBInit.USERType = OB_USER_SWAP_BANK;
              OBInit.USERConfig = OB_SWAP_BANK_DISABLE;
              HAL_FLASHEx_OBProgram(&OBInit);

              /* Launch Option bytes loading */
              HAL_FLASH_OB_Launch();
            }
            printFlashOptions(OBInit);
            HAL_FLASH_OB_Lock();

            term2("reboot...")
            //Перезагрузка
            HAL_NVIC_SystemReset();
            }


            osDelay(1);
        }
        else
        {
            osMutexRelease(mutexFirmwareRingBufferId);
            osDelay(10);
        }
    }
}

/*!
 \brief Parsing Firmware from Json package

 \fn parsingFirmwareFromJson
 \param doc Json package
*/
void parsingFirmwareFromJson(JsonDocument &doc)
{
    /* FatFs function common result code */
    const char *cmd = doc["cmd"];
    const char *station = doc["station"];
    if ((doc["cmd"] == "update") && ((uint8_t)doc["station"] == (uint8_t)Json::getInstance()->thisStation.id))
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
    if (Json::getInstance()->thisStation.id == rebootId)
    {
        HAL_NVIC_SystemReset();
    }
}

void deleteConfigMcuByJson(JsonDocument &doc)
{
    int deleteConfigId = doc["deleteConfigId"];
    if (Json::getInstance()->thisStation.id == deleteConfigId)
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
    if (Json::getInstance()->thisStation.id == writeConfigId)
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
term2((int)commonSizeAllFrames)

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

                configDoc["writeConfigId"] = Json::getInstance()->thisStation.id;
                configDoc["status"].set("ok");
//                UdpJsonExch::getInstance()->callControl->sendJson(configDoc, capacity/*, UDP_PORT*/);

            term2("write config OK")

                std::fill(UdpJsonExch::getInstance()->callControl->messageData.txBuff, UdpJsonExch::getInstance()->callControl->messageData.txBuff + UdpJsonExch::getInstance()->callControl->messageData.txBuffSize, 0);
                if (serializeJson(configDoc, UdpJsonExch::getInstance()->callControl->messageData.txBuff, capacity) > 0)
                {
                    sendUdpMulticast(UdpJsonExch::getInstance()->callControl->messageData.txBuff, strlen(UdpJsonExch::getInstance()->callControl->messageData.txBuff));
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
                 OBInit.OptionType,
                 OBInit.WRPState,
                 OBInit.WRPSector,
                 OBInit.BORLevel,
                 OBInit.USERType,
                 OBInit.USERConfig,
                 OBInit.Banks
                 );
    term2(tmp);
}
