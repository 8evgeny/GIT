/*!
 \file flash.cpp

 \authors Shulenkov R.A.

 \code
    char bufWrite[512] = "hello world";
    char bufRead[512];

    std::fill(bufRead, bufRead + sizeof(bufRead), 0);

    static FLASH_EraseInitTypeDef EraseInitStruct;
    static FLASH_OBProgramInitTypeDef OBInit;

    uint32_t SECTORError = 0;

    EraseInitStruct.TypeErase     = FLASH_TYPEERASE_SECTORS;
    EraseInitStruct.VoltageRange  = FLASH_VOLTAGE_RANGE_3;
    EraseInitStruct.Sector        = FLASH_SECTOR_7;
    EraseInitStruct.NbSectors     = 1;

    HAL_FLASH_OB_Unlock();

    HAL_FLASHEx_OBGetConfig(&OBInit);

    HAL_FLASH_OB_Lock();

    Flash::getInstance().unlock();

    __HAL_FLASH_ART_DISABLE();
    __HAL_FLASH_ART_RESET();
    __HAL_FLASH_ART_ENABLE();

    while (HAL_FLASHEx_Erase(&EraseInitStruct, &SECTORError) != HAL_OK) {
        Debug::getInstance().dbg << "Error occurred while sector erase.\n";
    }

    SCB_CleanInvalidateDCache_by_Addr((uint32_t *)FLASH_SECTOR_7, 256 * 1024);
    SCB_InvalidateICache();

    Flash::getInstance().lock();

    unlock();

    __HAL_FLASH_ART_DISABLE();
    __HAL_FLASH_ART_RESET();
    __HAL_FLASH_ART_ENABLE();

    write(ADDR_FLASH_SECTOR_7, reinterpret_cast<const char *>(bufWrite), sizeof(bufWrite));

    SCB_CleanInvalidateDCache_by_Addr((uint32_t *)ADDR_FLASH_SECTOR_7, sizeof(bufWrite));
    SCB_InvalidateICache();

    read(ADDR_FLASH_SECTOR_7, reinterpret_cast<char *>(bufRead), sizeof(bufRead));
    lock();

 \endcode
*/

#include "flash.h"
#include <algorithm>
#include "stm32f7xx_hal.h"
#include <cstring>
#include <../Debug/debug.h>

Flash *Flash::pInstance = nullptr;
FlashDestroyer Flash::destroyer;

FlashDestroyer::~FlashDestroyer()
{
    delete pInstance;
}

void FlashDestroyer::initialize(Flash *p)
{
    pInstance = p;
}

Flash &Flash::getInstance()
{
    if (!pInstance)     {
        pInstance = new Flash();
        destroyer.initialize(pInstance);
    }
    return *pInstance;
}

void Flash::read(uint32_t addr, char *buf, uint32_t size)
{
//    std::copy(reinterpret_cast<uint8_t *>(addr), reinterpret_cast<uint8_t *>(addr + size), buf);
    std::memcpy(buf, (char *)addr, size);
}

void Flash::write(uint32_t addr, const char *buf, uint32_t size)
{
    uint32_t alignOffset;
    uint32_t alignedFlashAddress;
    uint32_t tmp;
    uint32_t chunkSize;

    /* Wait for any busy flags */
    while (FLASH_WaitForLastOperation(10) != HAL_OK);

    /* Check if the flash address is correctly aligned */
    alignOffset = addr % sizeof(uint32_t);

    if (alignOffset != 0) {
        /* Not aligned, thus we have to read the data in flash already present
        * and update them with buffer's data */

        /* Align the flash address correctly */
        alignedFlashAddress = addr - alignOffset;

        /* Read already present data */
        tmp = *(volatile uint32_t *)alignedFlashAddress;

        /* Compute how much bytes one must update in the data read */
        chunkSize = sizeof(uint32_t) - alignOffset;
        if (chunkSize > size)
            chunkSize = size; // this happens when both address and address + size are not aligned

        /* Update the read data with buffer's data */

        std::memcpy((uint8_t *)&tmp + alignOffset, buf, chunkSize);
        //std::copy(buf, buf + chunkSize, reinterpret_cast<uint8_t *>(&tmp + alignOffset));

        /* Write the new data in flash */
        HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, alignedFlashAddress, tmp);

        /* Advance */
        addr += chunkSize;
        buf += chunkSize;
        size -= chunkSize;
    }

    /* Now, address is correctly aligned. One can copy data directly from
     * buffer's data to flash memory until the size of the data remaining to be
     * copied requires special treatment. */

    while (size >= sizeof(uint32_t)) {

        HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, addr, *(const uint32_t *)buf);
        addr += sizeof(uint32_t);
        buf += sizeof(uint32_t);
        size -= sizeof(uint32_t);
    }

    /* Now, address is correctly aligned, but the remaining data are to
     * small to fill a entier flashdata_t. Thus, one must read data already
     * in flash and update them with buffer's data before writing an entire
     * flashdata_t to flash memory. */
    if (size > 0) {
        uint32_t tmp = *(volatile uint32_t *)addr;
        std::memcpy(&tmp, buf, size);
        //std::copy(buf, buf + size, &tmp);

        HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, addr, tmp);
    }
}

void Flash::write(void *addr, void *data)
{
    write(reinterpret_cast<uint32_t>(addr), reinterpret_cast<const char *>(data), IFLASH_PAGE_SIZE);
}

void Flash::unlock()
{
    while (HAL_FLASH_Unlock() != HAL_OK);
}

void Flash::lock()
{
    while (HAL_FLASH_Lock() != HAL_OK);
}

bool Flash::compare(uint32_t address, const uint8_t *buffer, uint32_t size)
{
    /* For efficiency, compare flashdata_t values as much as possible,
     * then, fallback to byte per byte comparison. */
    while (size >= sizeof(uint32_t)) {
        if (*(volatile uint32_t *)address != *(uint32_t *)buffer)
            return false;
        address += sizeof(uint32_t);
        buffer += sizeof(uint32_t);
        size -= sizeof(uint32_t);
    }
    while (size > 0) {
        if (*(volatile uint8_t *)address != *buffer)
            return false;
        ++address;
        ++buffer;
        --size;
    }

    return true;
}

void Flash::test()
{
    char bufWrite[512] = "hello world";
    char bufRead[512];

    std::fill(bufRead, bufRead + sizeof(bufRead), 0);

    /*Variable used for Erase procedure*/
    static FLASH_EraseInitTypeDef EraseInitStruct;
    static FLASH_OBProgramInitTypeDef OBInit;

    uint32_t SECTORError = 0;

    /* Get the 1st sector to erase */
    EraseInitStruct.TypeErase     = FLASH_TYPEERASE_SECTORS;
    EraseInitStruct.VoltageRange  = FLASH_VOLTAGE_RANGE_3;
    EraseInitStruct.Sector        = FLASH_SECTOR_7;
    EraseInitStruct.NbSectors     = 1;


    //if you need to write, you erase
    /* Note: If an erase operation in Flash memory also concerns data in the data or instruction cache,
     you have to make sure that these data are rewritten before they are accessed during code
     execution. If this cannot be done safely, it is recommended to flush the caches by setting the
     DCRST and ICRST bits in the FLASH_CR register. */

    /* Allow Access to option bytes sector */
    HAL_FLASH_OB_Unlock();
    /* Get the Dual bank configuration status */
    HAL_FLASHEx_OBGetConfig(&OBInit);
    /* Allow Access to option bytes sector */
    HAL_FLASH_OB_Lock();

    Flash::getInstance().unlock();

    __HAL_FLASH_ART_DISABLE();
    __HAL_FLASH_ART_RESET();
    __HAL_FLASH_ART_ENABLE();

    while (HAL_FLASHEx_Erase(&EraseInitStruct, &SECTORError) != HAL_OK) {
        /*
        Error occurred while sector erase.
        User can add here some code to deal with this error.
        SECTORError will contain the faulty sector and then to know the code error on this sector,
        user can call function 'HAL_FLASH_GetError()'
        */
        /* Infinite loop */
        Debug::getInstance().dbg << "Error occurred while sector erase.\n";
    }

    SCB_CleanInvalidateDCache_by_Addr((uint32_t *)FLASH_SECTOR_7, 256 * 1024);
    SCB_InvalidateICache();

    Flash::getInstance().lock();

    unlock();

    __HAL_FLASH_ART_DISABLE();
    __HAL_FLASH_ART_RESET();
    __HAL_FLASH_ART_ENABLE();

    write(ADDR_FLASH_SECTOR_7, reinterpret_cast<const char *>(bufWrite), sizeof(bufWrite));

    SCB_CleanInvalidateDCache_by_Addr((uint32_t *)ADDR_FLASH_SECTOR_7, sizeof(bufWrite));
    SCB_InvalidateICache();

    read(ADDR_FLASH_SECTOR_7, reinterpret_cast<char *>(bufRead), sizeof(bufRead));
    lock();
}

void Flash::erase(){
    /*Variable used for Erase procedure*/
    static FLASH_EraseInitTypeDef EraseInitStruct;
    static FLASH_OBProgramInitTypeDef OBInit;

    uint32_t SECTORError = 0;

    /* Get the 1st sector to erase */
    EraseInitStruct.TypeErase     = FLASH_TYPEERASE_SECTORS;
    EraseInitStruct.VoltageRange  = FLASH_VOLTAGE_RANGE_3;
    EraseInitStruct.Sector        = FLASH_SECTOR_8;
    EraseInitStruct.NbSectors     = 4;

    //if you need to write, you erase
    /* Note: If an erase operation in Flash memory also concerns data in the data or instruction cache,
               you have to make sure that these data are rewritten before they are accessed during code
               execution. If this cannot be done safely, it is recommended to flush the caches by setting the
               DCRST and ICRST bits in the FLASH_CR register. */


    /* Allow Access to option bytes sector */
    HAL_FLASH_OB_Unlock();
    /* Get the Dual bank configuration status */
    HAL_FLASHEx_OBGetConfig(&OBInit);
    /* Allow Access to option bytes sector */
    HAL_FLASH_OB_Lock();

    Flash::getInstance().unlock();

    __HAL_FLASH_ART_DISABLE();
    __HAL_FLASH_ART_RESET();
    __HAL_FLASH_ART_ENABLE();

    while (HAL_FLASHEx_Erase(&EraseInitStruct, &SECTORError) != HAL_OK) {
        /*
                Error occurred while sector erase.
                User can add here some code to deal with this error.
                SECTORError will contain the faulty sector and then to know the code error on this sector,
                user can call function 'HAL_FLASH_GetError()'
                */
        /* Infinite loop */
    }

    SCB_CleanInvalidateDCache_by_Addr((uint32_t *)FLASH_SECTOR_8, 4 * 256 * 1024);
    SCB_InvalidateICache();

    Flash::getInstance().lock();
}

void Flash::eraseForUpdate(){
    /*Variable used for Erase procedure*/
    static FLASH_EraseInitTypeDef EraseInitStruct;
    static FLASH_OBProgramInitTypeDef OBInit;

    uint32_t SECTORError = 0;

    /* Get the 1st sector to erase */
    EraseInitStruct.TypeErase     = FLASH_TYPEERASE_SECTORS;
    EraseInitStruct.VoltageRange  = FLASH_VOLTAGE_RANGE_3;
    EraseInitStruct.Sector        = FLASH_SECTOR_5;
    EraseInitStruct.NbSectors     = 3;

    //if you need to write, you erase
    /* Note: If an erase operation in Flash memory also concerns data in the data or instruction cache,
               you have to make sure that these data are rewritten before they are accessed during code
               execution. If this cannot be done safely, it is recommended to flush the caches by setting the
               DCRST and ICRST bits in the FLASH_CR register. */


    /* Allow Access to option bytes sector */
    HAL_FLASH_OB_Unlock();
    /* Get the Dual bank configuration status */
    HAL_FLASHEx_OBGetConfig(&OBInit);
    /* Allow Access to option bytes sector */
    HAL_FLASH_OB_Lock();

    Flash::getInstance().unlock();

    __HAL_FLASH_ART_DISABLE();
    __HAL_FLASH_ART_RESET();
    __HAL_FLASH_ART_ENABLE();

    while (HAL_FLASHEx_Erase(&EraseInitStruct, &SECTORError) != HAL_OK) {
        /*
                Error occurred while sector erase.
                User can add here some code to deal with this error.
                SECTORError will contain the faulty sector and then to know the code error on this sector,
                user can call function 'HAL_FLASH_GetError()'
                */
        /* Infinite loop */
    }

    SCB_CleanInvalidateDCache_by_Addr((uint32_t *)FLASH_SECTOR_5, 3 * 256 * 1024);
    SCB_InvalidateICache();

    Flash::getInstance().lock();
}
