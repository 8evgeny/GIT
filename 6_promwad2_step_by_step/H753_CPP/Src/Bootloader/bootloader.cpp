#include "bootloader.h"
#include "stm32f7xx_hal.h"
#include "../flash/flash.h"
#include "../Debug/debug.h"
#include <stdlib.h>
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif

#include "AES128/aes.h"

#ifdef __cplusplus
}
#endif

#define START_AREA_NEW_FIRMWARE 0x0810ca00
#define START_AREA_CURRENT_FIRMWARE 0x08040000
#define FIRMWARE_NEW_VERSION 1
#define FIRMWARE_NEW_SUBVERSION 1
#define FIRMWARE_NEW_SIZE 4
#define FIRMWARE_NEW_MD5 16
#define FIRMWARE_NEW_TIME 30
#define FIRMWARE_STEP_HEAD 512
#define APPLICATION_ADDRESS    (START_AREA_CURRENT_FIRMWARE + FIRMWARE_STEP_HEAD)

bool bootloader::checkStart(void)
{
    uint8_t *currentFirmware = reinterpret_cast<uint8_t *>(START_AREA_CURRENT_FIRMWARE);
    uint8_t *newFirmware = reinterpret_cast<uint8_t *>(START_AREA_NEW_FIRMWARE);

    uint32_t error = 0;

    for (int i = 0; i < FIRMWARE_STEP_HEAD; i++) {
        if (((*(currentFirmware + i)) == (*(newFirmware + i)))
                && ((*(currentFirmware + i)) == 0xFF)) {
            error++;
        }
    }

    if (error == FIRMWARE_STEP_HEAD) {
        return false;
    }

    return true;
}

bool bootloader::checkHeader(void)
{
    uint8_t *currentFirmware = reinterpret_cast<uint8_t *>(START_AREA_CURRENT_FIRMWARE);
    uint8_t *newFirmware = reinterpret_cast<uint8_t *>(START_AREA_NEW_FIRMWARE);

    for (int i = 0; i < FIRMWARE_STEP_HEAD; i++) {
        if ((*(currentFirmware + i)) == (*(newFirmware + i))) {
            continue;
        } else {
            return 0;
        }
    }
    return 1;
}

bool bootloader::checkMD5(void)
{
    uint8_t md5NewFirmware[FIRMWARE_NEW_MD5];
    uint8_t md5NewFirmwareFromFlash[FIRMWARE_NEW_MD5];

    std::memset(md5NewFirmware, 0x00, sizeof(md5NewFirmware));

    uint8_t byte1 = *reinterpret_cast<uint8_t *>(START_AREA_NEW_FIRMWARE + FIRMWARE_NEW_VERSION + FIRMWARE_NEW_SUBVERSION);
    uint8_t byte2 = *reinterpret_cast<uint8_t *>(START_AREA_NEW_FIRMWARE + FIRMWARE_NEW_VERSION + FIRMWARE_NEW_SUBVERSION + 1);
    uint8_t byte3 = *reinterpret_cast<uint8_t *>(START_AREA_NEW_FIRMWARE + FIRMWARE_NEW_VERSION + FIRMWARE_NEW_SUBVERSION + 2);
    uint8_t byte4 = *reinterpret_cast<uint8_t *>(START_AREA_NEW_FIRMWARE + FIRMWARE_NEW_VERSION + FIRMWARE_NEW_SUBVERSION + 3);

    uint32_t sizeNewFirmware = static_cast<uint32_t>((byte1 << 24) | (byte2 << 16) | (byte3 << 8) | (byte4));
    uint8_t *pMD5NewFirmware = MD5::make_hash2(reinterpret_cast<void const *>(START_AREA_NEW_FIRMWARE + FIRMWARE_STEP_HEAD), sizeNewFirmware);

    if (pMD5NewFirmware != nullptr) {
        for (int  i = 0; i < FIRMWARE_NEW_MD5; i++) {
            md5NewFirmware[i] = pMD5NewFirmware[i];
            md5NewFirmwareFromFlash[i] = *reinterpret_cast<uint8_t *>(START_AREA_NEW_FIRMWARE + FIRMWARE_NEW_VERSION + FIRMWARE_NEW_SUBVERSION + FIRMWARE_NEW_SIZE + i);
        }
        free(pMD5NewFirmware);
    }

    if (std::memcmp(md5NewFirmwareFromFlash, md5NewFirmware, FIRMWARE_NEW_MD5) == 0) {
        return true;
    }
    return false;
}

bool bootloader::checkMD5Bare(void)
{
    uint8_t md5NewFirmware[FIRMWARE_NEW_MD5];
    uint8_t md5NewFirmwareFromFlash[FIRMWARE_NEW_MD5];

    std::memset(md5NewFirmware, 0x00, sizeof(md5NewFirmware));

    uint32_t addr =  START_AREA_NEW_FIRMWARE + FIRMWARE_NEW_VERSION + FIRMWARE_NEW_SUBVERSION + FIRMWARE_NEW_SIZE + FIRMWARE_NEW_MD5 + FIRMWARE_NEW_TIME;

    uint8_t byte1Bare = *reinterpret_cast<uint8_t *>(addr);
    uint8_t byte2Bare = *reinterpret_cast<uint8_t *>(addr + 1);
    uint8_t byte3Bare = *reinterpret_cast<uint8_t *>(addr + 2);
    uint8_t byte4Bare = *reinterpret_cast<uint8_t *>(addr + 3);

    uint32_t sizeNewFirmwareBare = static_cast<uint32_t>((byte1Bare << 24) | (byte2Bare << 16) | (byte3Bare << 8) | (byte4Bare));

    uint8_t *pMD5NewFirmware = MD5::make_hash2(reinterpret_cast<void const *>(START_AREA_CURRENT_FIRMWARE + FIRMWARE_STEP_HEAD), sizeNewFirmwareBare);

    if (pMD5NewFirmware != nullptr) {
        for (int  i = 0; i < FIRMWARE_NEW_MD5; i++) {
            md5NewFirmware[i] = pMD5NewFirmware[i];
            md5NewFirmwareFromFlash[i] = *reinterpret_cast<uint8_t *>(addr + FIRMWARE_NEW_SIZE + i);
        }
        free(pMD5NewFirmware);
    }

    if (std::memcmp(md5NewFirmwareFromFlash, md5NewFirmware, FIRMWARE_NEW_MD5) == 0) {
        return true;
    }
    return false;
}

void bootloader::copyFirmware(void)
{
    Flash::getInstance().eraseForUpdate();

    uint8_t byte1 = *reinterpret_cast<uint8_t *>(START_AREA_NEW_FIRMWARE + FIRMWARE_NEW_VERSION + FIRMWARE_NEW_SUBVERSION);
    uint8_t byte2 = *reinterpret_cast<uint8_t *>(START_AREA_NEW_FIRMWARE + FIRMWARE_NEW_VERSION + FIRMWARE_NEW_SUBVERSION + 1);
    uint8_t byte3 = *reinterpret_cast<uint8_t *>(START_AREA_NEW_FIRMWARE + FIRMWARE_NEW_VERSION + FIRMWARE_NEW_SUBVERSION + 2);
    uint8_t byte4 = *reinterpret_cast<uint8_t *>(START_AREA_NEW_FIRMWARE + FIRMWARE_NEW_VERSION + FIRMWARE_NEW_SUBVERSION + 3);

    __ALIGN_BEGIN static const uint8_t key[16] __ALIGN_END = { '1', '2', '3', '4', '5', '6', '7', '8', '1', '2', '3', '4', '5', '6', '7', '8'};

    const uint32_t stepSize = 16;

    uint8_t buff[stepSize];
    uint32_t sizeNewFirmware = static_cast<uint32_t>((byte1 << 24) | (byte2 << 16) | (byte3 << 8) | (byte4));

    Flash::getInstance().unlock();
    for (uint32_t i = FIRMWARE_STEP_HEAD; i < sizeNewFirmware + FIRMWARE_STEP_HEAD; i += stepSize) {
        AES128_ECB_decrypt(reinterpret_cast<uint8_t *>(START_AREA_NEW_FIRMWARE + i), key, buff);
        if ((sizeNewFirmware + FIRMWARE_STEP_HEAD - i) <= stepSize) {

            uint32_t addr =  START_AREA_NEW_FIRMWARE + FIRMWARE_NEW_VERSION + FIRMWARE_NEW_SUBVERSION + FIRMWARE_NEW_SIZE + FIRMWARE_NEW_MD5 + FIRMWARE_NEW_TIME;

            uint8_t byte1Bare = *reinterpret_cast<uint8_t *>(addr);
            uint8_t byte2Bare = *reinterpret_cast<uint8_t *>(addr + 1);
            uint8_t byte3Bare = *reinterpret_cast<uint8_t *>(addr + 2);
            uint8_t byte4Bare = *reinterpret_cast<uint8_t *>(addr + 3);
            uint32_t sizeNewFirmwareBare = static_cast<uint32_t>((byte1Bare << 24) | (byte2Bare << 16) | (byte3Bare << 8) | (byte4Bare));

            uint8_t buffTmp[stepSize];
            uint32_t mod = sizeNewFirmwareBare % stepSize;
            std::memset(buffTmp, 0xff, stepSize);
            std::memcpy(buffTmp, buff, mod);

            Flash::getInstance().write(START_AREA_CURRENT_FIRMWARE + i, reinterpret_cast<const char *>(buffTmp), stepSize);
        } else {
            Flash::getInstance().write(START_AREA_CURRENT_FIRMWARE + i, reinterpret_cast<const char *>(buff), stepSize);
        }
    }
    Flash::getInstance().lock();

    Flash::getInstance().unlock();
    Flash::getInstance().write(START_AREA_CURRENT_FIRMWARE, reinterpret_cast<const char *>(START_AREA_NEW_FIRMWARE), FIRMWARE_STEP_HEAD);
    Flash::getInstance().lock();
}

bool bootloader::updateFirmware(void)
{
    if (!checkStart()) {
        return false;
    }

    if (checkHeader()) {
        return true;
    }

    if (!checkMD5()) {
        return true;
    }

    copyFirmware();
    checkMD5Bare();
    return true;
}

void goToUserApp(void)
{
    uint32_t appJumpAddress;
    void (*GoToApp)(void);

    appJumpAddress = *((volatile uint32_t *)(APPLICATION_ADDRESS + 4));
    GoToApp = (void (*)(void))appJumpAddress;
    SCB->VTOR = APPLICATION_ADDRESS;
    __set_MSP(*((volatile uint32_t *) APPLICATION_ADDRESS));
    GoToApp();
}

void bootloader::jumpToFirmware()
{
    goToUserApp();
}
