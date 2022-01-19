#include <sstream>
#include <bitset>
#include <iomanip>
#include <array>
#include "ArduinoJson.h"

#include "ff_gen_drv.h"
#include "../flash/flash.h"
#include <stm32f7xx_hal.h>

#include "CircularBuffer.h"
#include "json.h"

#include "../UDP_JSON/udp_multicast.h"
//#ifndef SC2BOARD
////#include "call_control.h"
//#else
//#include "../Call_control_for_SC2_board/call_control_sc2.h"
//#endif

const size_t SIZE_FIRMWARE_BASE = 512; /*! Firmware block */

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
    for (size_t i = 0; i < data.size(); i += 2) {
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

    osThreadDef(handelFirmwareThread, updateFirmwareThread, osPriorityNormal, 0, configMINIMAL_STACK_SIZE * 5);
    firmwareThreadId = osThreadCreate(osThread(handelFirmwareThread), NULL);
}

static uint32_t counterSize = 0; /*! A counter for size of data */
static int counterPackegs = 0; /*! A counter for size of packages */

[[ noreturn ]]void updateFirmwareThread(const void *arg)
{
    UNUSED(arg);
    uint32_t byteswritten{0};
    FRESULT res;

//    int versionFirmware;
//    int subVersionFirmware;
    int size;
//    int current;
//    int all;

    while (1) {
        osMutexWait(mutexFirmwareRingBufferId, osWaitForever);
        if (firmwareRingBuffer.size() != 0) {
            FirmwarePackage pack = firmwareRingBuffer.first();
            firmwareRingBuffer.shift();
            osMutexRelease(mutexFirmwareRingBufferId);

//            versionFirmware = pack.versionFirmware;
//            subVersionFirmware = pack.subVersionFirmware;
            size = pack.size;
//            all = pack.all;
//            current = pack.current;

            if (pack.current == counterPackegs) {
                if (pack.current == 0) {

                    //always erase
                    Flash::getInstance().erase();

                    counterSize = 0;
                    counterPackegs = 0;
                    /*##-1- Link the micro Flash I/O driver ##################################*/
                    FATFS_LinkDriver(&FLASH_Driver, FLASHPath);
                    /*##-2- Register the file system object to the FatFs module ##############*/
                    res = f_mount(&FLASHFatFs, (TCHAR const *)FLASHPath, 0);
                    /*##-3- Create a FAT file system (format) on the logical drive #########*/
                    /* WARNING: Formatting the Flash will delete all content on the device */
                    res = f_mkfs((TCHAR const *)FLASHPath, FS_FAT32, 0, workBuffer, sizeof(workBuffer));

                    /*##-4- Create and Open a new text file object with write access #####*/
                    res = f_open(&MyFile, "BIN", FA_CREATE_ALWAYS | FA_WRITE);
                    /*##-5- Write data to the text file ################################*/

                    res = f_write(&MyFile, pack.data.data(), pack.data.size(), (UINT *)&byteswritten);
                    counterSize += pack.data.size();
                    counterPackegs++;

                } else if (pack.current == pack.all) {

                    //this packeage can be less than 512 bytes
//                    res = f_write(&MyFile, pack.data.data(), pack.data.size(), (UINT *)&byteswritten);
                    res = f_write(&MyFile, pack.data.data(), size / 2, (UINT *)&byteswritten);
                    counterSize += pack.data.size();
                    counterPackegs++;

                    /*##-6- Close the open text file #################################*/
                    f_close(&MyFile);

                    Flash::getInstance().flush();
                    /*##-11- Unlink the Flash I/O driver ###############################*/
                    FATFS_UnLinkDriver(FLASHPath);
                    //resety
                    counterPackegs = 0;

                    //send msg firmware write OK
                    //send msg configuration write OK
                    const int capacity = JSON_OBJECT_SIZE(3);
                    StaticJsonDocument<capacity> firmwareDoc;

                    firmwareDoc["writeFirmwareId"] = Json::getInstance()->thisStation.id;
                    firmwareDoc["status"].set("ok");
//                    UdpJsonExch::getInstance()->callControl->sendJson(firmwareDoc, capacity/*, UDP_PORT*/);

                    std::fill(UdpJsonExch::getInstance()->callControl->messageData.txBuff, UdpJsonExch::getInstance()->callControl->messageData.txBuff + UdpJsonExch::getInstance()->callControl->messageData.txBuffSize, 0);
                    if (serializeJson(firmwareDoc, UdpJsonExch::getInstance()->callControl->messageData.txBuff, capacity) > 0) {
                        sendUdpMulticast(UdpJsonExch::getInstance()->callControl->messageData.txBuff, strlen(UdpJsonExch::getInstance()->callControl->messageData.txBuff));
                    }

                } else {
                    res = f_write(&MyFile,  pack.data.data(),  pack.data.size(), (UINT *)&byteswritten);
                    counterSize += pack.data.size();
                    counterPackegs++;
                }
            }
            osDelay(1);
        } else {
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

    if (cmd != nullptr) {

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
    if (Json::getInstance()->thisStation.id == rebootId) {
        HAL_NVIC_SystemReset();
    }
}

void deleteConfigMcuByJson(JsonDocument &doc)
{
    int deleteConfigId = doc["deleteConfigId"];
    if (Json::getInstance()->thisStation.id == deleteConfigId) {
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
    if (Json::getInstance()->thisStation.id == writeConfigId) {

        int number =  doc["number"];
        if (number == counterFrames) {

            int all = doc["all"];
            int size = doc["size"];

            counterFrames++;
            commonSizeAllFrames += size;
            const char *config  = doc["config"];

            std::fill(tmp, tmp + SIZE_DEF_BLOCK_UDP / 4, 0);
            std::memcpy(tmp, config, SIZE_DEF_BLOCK_UDP);
            SRAM::getInstance()->writeData(tmp, SIZE_DEF_BLOCK_UDP, reinterpret_cast<uint32_t *>(0x60020000 + number * SIZE_DEF_BLOCK_UDP));
            if ((all == number) && (counterFrames > 0)) {

                uint8_t readSramBuff[SIZE_WRITE_BLOCK] {0};

                lfs_remove(FsForEeprom::getInstance().lfsPtr, "boot_config");
                lfs_file_open(FsForEeprom::getInstance().lfsPtr, FsForEeprom::getInstance().filePtr, "boot_config", LFS_O_RDWR | LFS_O_CREAT);

                for (int32_t i = 0; i < commonSizeAllFrames; i += SIZE_WRITE_BLOCK) {
                    SRAM::getInstance()->readData(reinterpret_cast<uint32_t *>(readSramBuff), SIZE_WRITE_BLOCK, reinterpret_cast<uint32_t *>(0x60020000 + i));
                    lfs_file_write(FsForEeprom::getInstance().lfsPtr, FsForEeprom::getInstance().filePtr, readSramBuff,  commonSizeAllFrames - i < SIZE_WRITE_BLOCK ? static_cast<uint32_t>(commonSizeAllFrames - i) : SIZE_WRITE_BLOCK);
                }
                lfs_file_close(FsForEeprom::getInstance().lfsPtr, FsForEeprom::getInstance().filePtr);

                //send msg configuration write OK
                const int capacity = JSON_OBJECT_SIZE(3);
                StaticJsonDocument<capacity> configDoc;

                configDoc["writeConfigId"] = Json::getInstance()->thisStation.id;
                configDoc["status"].set("ok");
//                UdpJsonExch::getInstance()->callControl->sendJson(configDoc, capacity/*, UDP_PORT*/);

                std::fill(UdpJsonExch::getInstance()->callControl->messageData.txBuff, UdpJsonExch::getInstance()->callControl->messageData.txBuff + UdpJsonExch::getInstance()->callControl->messageData.txBuffSize, 0);
                if (serializeJson(configDoc, UdpJsonExch::getInstance()->callControl->messageData.txBuff, capacity) > 0) {
                    sendUdpMulticast(UdpJsonExch::getInstance()->callControl->messageData.txBuff, strlen(UdpJsonExch::getInstance()->callControl->messageData.txBuff));
                }

                commonSizeAllFrames = 0;
                counterFrames = 0;
            }
        }
    }
}

