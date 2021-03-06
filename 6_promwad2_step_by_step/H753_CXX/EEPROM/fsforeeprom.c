#include "fsforeeprom.h"
#include "eeprom.h"

#include <string.h>
extern UART_HandleTypeDef huart7;
// variables used by the filesystem
lfs_t lfs;
lfs_file_t file;
FsForEeprom_t fseeprom;

//Изменил параметры EEPROM
//uint8_t littlefs_mem[8192];
uint8_t littlefs_mem[131072];

uint8_t lfs_read_buf[256];
uint8_t lfs_prog_buf[256];
uint8_t lfs_lookahead_buf[16];  // 128/8=16
uint8_t lfs_file_buf[256];

// configuration of the filesystem is provided by this struct
struct lfs_config cfg;

int user_provided_block_device_read(const struct lfs_config *c,
                                    lfs_block_t block, lfs_off_t off,
                                    void *buffer, lfs_size_t size)
{
//    char msgUart7[] = "user_provided_block_device_read\r\n";
//    HAL_UART_Transmit (&huart7,(uint8_t*)msgUart7, sizeof (msgUart7), 1000);

    uint32_t addr = (uint32_t)(block * c->block_size + off);
    uint16_t NumByteToRead = (uint16_t)size;
    uint8_t *buffer_data = (uint8_t *)buffer;
    return BSP_EEPROM_ReadBuffer(buffer_data, addr, &NumByteToRead);
}

int user_provided_block_device_prog(const struct lfs_config *c,
                                    lfs_block_t block, lfs_off_t off,
                                    const void *buffer, lfs_size_t size)
{
    char msgUart7[] = "user_provided_block_device_prog\r\n";
    HAL_UART_Transmit (&huart7,(uint8_t*)msgUart7, sizeof (msgUart7),1000);
    uint32_t addr = (uint32_t)(block * c->block_size + off);
    uint8_t *buffer_data = (uint8_t *)buffer;
    //Изменил 4 * и вернул
    return BSP_EEPROM_WriteBuffer(buffer_data, addr, size);
}

int user_provided_block_device_erase(const struct lfs_config *c,
                                     lfs_block_t block)
{
    char msgUart7[] = "user_provided_block_device_erase\r\n";
    HAL_UART_Transmit (&huart7,(uint8_t*)msgUart7, sizeof (msgUart7),1000);
    uint32_t addr = (uint32_t)( block * c->block_size);
    memset((void *)littlefs_mem[addr], c->block_size, 0);
    for (int i = 0; i < c->block_size; i++) littlefs_mem[addr + i] = 0;
    return 0;
}

int user_provided_block_device_sync(const struct lfs_config *c)
{
    return 0;
}

void littleFsInit()
{
// release any resources we were using
    lfs_unmount(&lfs);
    const char str[] = "\r---- lfs_unmount ---- \n\r";
    HAL_UART_Transmit (&huart7,(uint8_t*)str, sizeof (str), 1000);

// example littlefs
    cfg.read = user_provided_block_device_read;
    cfg.prog = user_provided_block_device_prog;
    cfg.erase = user_provided_block_device_erase;
    cfg.sync = user_provided_block_device_sync;

//Параметры lfs похожи на правильные
    cfg.read_size = 256;
    cfg.prog_size = 256;
    cfg.block_size = 1024;
    cfg.block_count = 128;
    cfg.lookahead = 256;

    cfg.read_buffer = lfs_read_buf;
    cfg.prog_buffer = lfs_prog_buf;
    cfg.lookahead_buffer = lfs_lookahead_buf;
    cfg.file_buffer = lfs_file_buf;

// mount the filesystem

    int err = lfs_mount(&lfs, &cfg);
    const char str2[] = "---- lfs_mount ---- \n\r";
    HAL_UART_Transmit (&huart7,(uint8_t*)str2, sizeof (str2), 1000);
// reformat if we can't mount the filesystem
// this should only happen on the first boot
    if (err) {

        lfs_format(&lfs, &cfg);
        const char str[] = "---- lfs_format ---- \n\r";
        HAL_UART_Transmit (&huart7,(uint8_t*)str, sizeof (str), 1000);
        lfs_mount(&lfs, &cfg);
        const char str2[] = "---- lfs_mount ---- \n\r";
        HAL_UART_Transmit (&huart7,(uint8_t*)str2, sizeof (str2), 1000);
    }

}


void FS_test()
{
    char rhymeToEEPROM[] = "--Red on top, Green below. Red says Stop, Green says Go. Yellow says Wait Even if you re late.--";
    char rhymeFromEEPROM[sizeof (rhymeToEEPROM)];
    lfs_file_open(&lfs, &file, "boot_count", LFS_O_RDWR | LFS_O_CREAT);
    lfs_file_rewind(&lfs, &file);
    lfs_file_write(&lfs, &file, &rhymeToEEPROM, sizeof(rhymeToEEPROM));
    lfs_file_close(&lfs, &file);

    // read current count
    uint32_t boot_count = 0;
    lfs_file_open(&lfs, &file, "boot_count", LFS_O_RDWR | LFS_O_CREAT);

    memset(rhymeFromEEPROM,'*',sizeof(rhymeToEEPROM));
    HAL_UART_Transmit (&huart7,(uint8_t*)rhymeFromEEPROM, sizeof (rhymeFromEEPROM),1000);
    HAL_UART_Transmit (&huart7,(uint8_t*)"\r\n", sizeof ("\r\n"),1000);

    lfs_file_read(&lfs, &file, &rhymeFromEEPROM, sizeof(rhymeFromEEPROM));

    HAL_UART_Transmit (&huart7,(uint8_t*)rhymeFromEEPROM, sizeof (rhymeFromEEPROM),1000);
    HAL_UART_Transmit (&huart7,(uint8_t*)"\r\n", sizeof ("\r\n"),1000);

    lfs_file_close(&lfs, &file);

    // update boot count
//    boot_count += 1;
//    lfs_file_rewind(&lfs, &file);
//    lfs_file_write(&lfs, &file, &boot_count, sizeof(boot_count));

    // remember the storage is not updated until the file is closed successfully
    lfs_file_close(&lfs, &file);
}

uint32_t Fs_init(void)
{
    return BSP_EEPROM_Init();
}

uint32_t FS_read(uint8_t *pBuffer, uint16_t readAddr, uint16_t *numByteToRead)
{
    return BSP_EEPROM_ReadBuffer(pBuffer, readAddr, numByteToRead);
}

uint32_t FS_write(uint8_t *pBuffer, uint16_t writeAddr, uint16_t numByteToWrite)
{
    return BSP_EEPROM_WriteBuffer(pBuffer, writeAddr, numByteToWrite);
}

