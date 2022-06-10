
#include "stm32f7xx_hal.h"

#include <algorithm>
#include <cstring>

#ifndef BOOTLOADER_FEZ
#include "RS232/rs232.h"
#include "flash/flash.h"
#endif

#ifdef BOOTLOADER_FEZ
#include "../flash/flash.h"
#endif



/* Includes ------------------------------------------------------------------*/
#include "ff_gen_drv.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Block Size in Bytes */
#define BLOCK_SIZE                512
#define COUNT_BLOCKS              34

/* Private variables ---------------------------------------------------------*/
/* Disk status */
static volatile DSTATUS Stat = STA_NOINIT;

/* Private function prototypes -----------------------------------------------*/
DSTATUS FLASH_initialize(BYTE);
DSTATUS FLASH_status(BYTE);
DRESULT FLASH_read(BYTE, BYTE *, DWORD, UINT);
#if _USE_WRITE == 1
DRESULT FLASH_write(BYTE, const BYTE *, DWORD, UINT);
#endif /* _USE_WRITE == 1 */
#if _USE_IOCTL == 1
DRESULT FLASH_ioctl(BYTE, BYTE, void *);
#endif /* _USE_IOCTL == 1 */

Diskio_drvTypeDef  FLASH_Driver = {
    FLASH_initialize,
    FLASH_status,
    FLASH_read,
#if  _USE_WRITE
    FLASH_write,
#endif  /* _USE_WRITE == 1 */
#if  _USE_IOCTL == 1
    FLASH_ioctl,
#endif /* _USE_IOCTL == 1 */
};

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Initializes a Drive
  * @param  lun : not used
  * @retval DSTATUS: Operation status
  */
DSTATUS FLASH_initialize(BYTE lun)
{
    (void) lun;
    Stat = STA_NOINIT;
    Stat &= ~STA_NOINIT;
    return Stat;
}

/**
  * @brief  Gets Disk Status
  * @param  lun : not used
  * @retval DSTATUS: Operation status
  */
DSTATUS FLASH_status(BYTE lun)
{
    (void) lun;
    return Stat;
}

static uint8_t bufCacheSector0[BLOCK_SIZE];
static uint8_t bufCacheSector1[COUNT_BLOCKS * BLOCK_SIZE];

/**
  * @brief  Reads Sector(s)
  * @param  lun : not used
  * @param  *buff: Data buffer to store read data
  * @param  sector: Sector address (LBA)
  * @param  count: Number of sectors to read (1..128)
  * @retval DRESULT: Operation result
  */
DRESULT FLASH_read(BYTE lun, BYTE *buff, DWORD sector, UINT count)
{
    (void) lun;

    uint32_t bufferSize = (BLOCK_SIZE * count);
    uint32_t flashAddress = Flash::getInstance().ADDR_FLASH_BANK_2 + (sector * BLOCK_SIZE);
    uint32_t flashBaseAddress = Flash::getInstance().ADDR_FLASH_BANK_2;

    //max size for reading 512byte
    //static constexpr uint32_t ADDR_FLASH_SECTOR_7 = 0x080C0000; /*! Base address of Sector 7, 256 Kbytes */

    //1 block
    if ((flashAddress >= flashBaseAddress) && (flashAddress <= flashBaseAddress + 0x1ff)) {
        std::memcpy(buff, bufCacheSector0, BLOCK_SIZE);
        return RES_OK;
    }

    //34 blocks
    if ((flashAddress >= flashBaseAddress + 0x7e00) && (flashAddress <= flashBaseAddress + 0xc1ff)) {
        uint32_t addr = flashAddress - (flashBaseAddress + 0x7e00);
        std::memcpy(buff, bufCacheSector1 + addr, BLOCK_SIZE);
        return RES_OK;
    }

    //Flash::getInstance().unlock();
    Flash::getInstance().read(flashAddress, (char *)buff, bufferSize);
    //Flash::getInstance().lock();

    return RES_OK;
}

/**
  * @brief  Writes Sector(s)
  * @param  lun : not used
  * @param  *buff: Data to be written
  * @param  sector: Sector address (LBA)
  * @param  count: Number of sectors to write (1..128)
  * @retval DRESULT: Operation result
  */
#if _USE_WRITE == 1
DRESULT FLASH_write(BYTE lun, const BYTE *buff, DWORD sector, UINT count)
{
    uint32_t bufferSize = (BLOCK_SIZE * count);
    uint32_t flashAddress = Flash::getInstance().ADDR_FLASH_BANK_2 + (sector * BLOCK_SIZE);
    uint32_t flashBaseAddress = Flash::getInstance().ADDR_FLASH_BANK_2;

    //max size for writing 512byte

    //1 block
    if ((flashAddress >= flashBaseAddress ) && (flashAddress <= flashBaseAddress + 0x1ff)) {
        std::memcpy(bufCacheSector0, buff, BLOCK_SIZE);
        return RES_OK;
    }

    //34 blocks
    if ((flashAddress >= flashBaseAddress + 0x7e00) && (flashAddress <= flashBaseAddress + 0xc1ff)) {
        uint32_t addr = flashAddress - (flashBaseAddress + 0x7e00);

        std::memcpy(bufCacheSector1 + addr, buff, BLOCK_SIZE);
        return RES_OK;
    }

    Flash::getInstance().unlock();

    __HAL_FLASH_ART_DISABLE();
    __HAL_FLASH_ART_RESET();
    __HAL_FLASH_ART_ENABLE();

    Flash::getInstance().write(flashAddress, (const char *)buff, bufferSize);

    SCB_CleanInvalidateDCache_by_Addr((uint32_t *)flashAddress, bufferSize);
    SCB_InvalidateICache();

    Flash::getInstance().lock();

    return RES_OK;
}
#endif /* _USE_WRITE == 1 */

/**
  * @brief  I/O control operation
  * @param  lun : not used
  * @param  cmd: Control code
  * @param  *buff: Buffer to send/receive control data
  * @retval DRESULT: Operation result
  */
#if _USE_IOCTL == 1
DRESULT FLASH_ioctl(BYTE lun, BYTE cmd, void *buff)
{
    DRESULT res = RES_ERROR;

    if (Stat & STA_NOINIT) return RES_NOTRDY;

    switch (cmd) {
    /* Make sure that no pending write process */
    case CTRL_SYNC :
        res = RES_OK;
        break;

    /* Get number of sectors on the disk (DWORD) */
    case GET_SECTOR_COUNT :
        *(DWORD *)buff = Flash::getInstance().FATFS_SIZE / BLOCK_SIZE;
        res = RES_OK;
        break;

    /* Get R/W sector size (WORD) */
    case GET_SECTOR_SIZE :
        *(WORD *)buff = BLOCK_SIZE;
        res = RES_OK;
        break;

    /* Get erase block size in unit of sector (DWORD) */
    case GET_BLOCK_SIZE :
        *(DWORD *)buff = 1;
        res = RES_OK;
        break;

    default:
        res = RES_PARERR;
    }

    return res;
}
#endif /* _USE_IOCTL == 1 */


#ifdef __cplusplus
}
#endif

void Flash::flush()
{
    //1 block
    unlock();

    __HAL_FLASH_ART_DISABLE();
    __HAL_FLASH_ART_RESET();
    __HAL_FLASH_ART_ENABLE();

    uint32_t flashBaseAddress = Flash::getInstance().ADDR_FLASH_BANK_2;

    write(flashBaseAddress, (const char *)bufCacheSector0, BLOCK_SIZE);

    SCB_CleanInvalidateDCache_by_Addr((uint32_t *)Flash::getInstance().ADDR_FLASH_BANK_2, BLOCK_SIZE);
    SCB_InvalidateICache();

    lock();

    //34 blocks
    for (uint32_t i = 0; i < COUNT_BLOCKS; i++) {
        unlock();

        __HAL_FLASH_ART_DISABLE();
        __HAL_FLASH_ART_RESET();
        __HAL_FLASH_ART_ENABLE();

        write(flashBaseAddress + 0x7e00 + i * BLOCK_SIZE, (const char *)(bufCacheSector1 + i * BLOCK_SIZE), BLOCK_SIZE);

        SCB_CleanInvalidateDCache_by_Addr((uint32_t *)(flashBaseAddress + 0x7e00) + i * BLOCK_SIZE, BLOCK_SIZE);
        SCB_InvalidateICache();

        lock();
    }
}

