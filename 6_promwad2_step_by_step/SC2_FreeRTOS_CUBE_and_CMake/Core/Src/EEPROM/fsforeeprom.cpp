/*!
 \file fsforeeprom.cpp

 \authors Shulenkov R.A.
*/
#include "fsforeeprom.h"
#include "eeprom.h"

#include <cstring>

#ifdef __cplusplus
extern "C" {
#endif

// variables used by the filesystem
lfs_t lfs;
lfs_file_t file;

uint8_t littlefs_mem[8192];

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
    uint32_t addr = (uint32_t)(block * c->block_size + off);
    uint16_t NumByteToRead = (uint16_t)size;
    uint8_t *buffer_data = (uint8_t *)buffer;
    return BSP_EEPROM_ReadBuffer(buffer_data, addr, &NumByteToRead);
}

int user_provided_block_device_prog(const struct lfs_config *c,
                                    lfs_block_t block, lfs_off_t off,
                                    const void *buffer, lfs_size_t size)
{
    uint32_t addr = (uint32_t)(block * c->block_size + off);
    uint8_t *buffer_data = (uint8_t *)buffer;
    return BSP_EEPROM_WriteBuffer(buffer_data, addr, size);
}

int user_provided_block_device_erase(const struct lfs_config *c,
                                     lfs_block_t block)
{
    uint32_t addr = (uint32_t)(block * c->block_size);
//    std::memset((void *)littlefs_mem[addr], c->block_size, 0);
//    for (int i = 0; i < c->block_size; i++) littlefs_mem[addr + i] = 0;
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

// example littlefs
    cfg.read = user_provided_block_device_read;
    cfg.prog = user_provided_block_device_prog;
    cfg.erase = user_provided_block_device_erase;
    cfg.sync = user_provided_block_device_sync;

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

// reformat if we can't mount the filesystem
// this should only happen on the first boot
    if (err) {
        lfs_format(&lfs, &cfg);
        lfs_mount(&lfs, &cfg);
    }

}
#ifdef __cplusplus
}
#endif


FsForEeprom *FsForEeprom::pInstance = nullptr;
FsForEepromDestroyer FsForEeprom::destroyer;

FsForEeprom::FsForEeprom()
{
    lfsPtr = &lfs;
    filePtr = &file;

}

FsForEepromDestroyer::~FsForEepromDestroyer()
{
    delete pInstance;
}
void FsForEepromDestroyer::initialize(FsForEeprom *p)
{
    pInstance = p;
}

FsForEeprom &FsForEeprom::getInstance()
{
    if (!pInstance)     {
        pInstance = new FsForEeprom();
        destroyer.initialize(pInstance);



    }
    return *pInstance;
}

void FsForEeprom::test()
{
    char rhymeToEEPROM[] = "Red on top, Green below. Red says “Stop”, Green says “Go”. Yellow says “Wait” Even if you’re late.";
    char rhymeFromEEPROM[sizeof (rhymeToEEPROM)];
    lfs_file_open(&lfs, &file, "boot_count", LFS_O_RDWR | LFS_O_CREAT);
    lfs_file_rewind(&lfs, &file);
    lfs_file_write(&lfs, &file, &rhymeToEEPROM, sizeof(rhymeToEEPROM));
    lfs_file_close(&lfs, &file);

    // read current count
    uint32_t boot_count = 0;
    lfs_file_open(&lfs, &file, "boot_count", LFS_O_RDWR | LFS_O_CREAT);
    lfs_file_read(&lfs, &file, &rhymeFromEEPROM, sizeof(rhymeFromEEPROM));
    lfs_file_close(&lfs, &file);

    // update boot count
//    boot_count += 1;
//    lfs_file_rewind(&lfs, &file);
//    lfs_file_write(&lfs, &file, &boot_count, sizeof(boot_count));

    // remember the storage is not updated until the file is closed successfully
    lfs_file_close(&lfs, &file);
}

uint32_t FsForEeprom::init(void)
{
    return BSP_EEPROM_Init();
}

uint32_t FsForEeprom::read(uint8_t *pBuffer, uint16_t readAddr, uint16_t *numByteToRead)
{
    return BSP_EEPROM_ReadBuffer(pBuffer, readAddr, numByteToRead);
}

uint32_t FsForEeprom::write(uint8_t *pBuffer, uint16_t writeAddr, uint16_t numByteToWrite)
{
    return BSP_EEPROM_WriteBuffer(pBuffer, writeAddr, numByteToWrite);
}

