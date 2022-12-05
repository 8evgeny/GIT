/*!
 \file fsforeeprom.cpp

 \authors Shulenkov R.A.
*/
#include "fsforeeprom.h"
#include "eeprom.h"
#include "rs232.h"

#include <cstring>

#ifdef __cplusplus
extern "C" {
#endif

// variables used by the filesystem
lfs_t lfs;
lfs_file_t file;

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

    uint32_t addr = (uint32_t)(block * c->block_size + off);
    //Изменил 4 * и вернул
    uint16_t NumByteToRead = (uint16_t)size;
    uint8_t *buffer_data = (uint8_t *)buffer;
    return BSP_EEPROM_ReadBuffer(buffer_data, addr, &NumByteToRead);
}

int user_provided_block_device_prog(const struct lfs_config *c,
                                    lfs_block_t block, lfs_off_t off,
                                    const void *buffer, lfs_size_t size)
{
term("*****  user_provided_block_device_prog  ******")

    uint32_t addr = (uint32_t)(block * c->block_size + off);
    uint8_t *buffer_data = (uint8_t *)buffer;
    //Изменил 4 * и вернул
    return BSP_EEPROM_WriteBuffer(buffer_data, addr, size);
}

int user_provided_block_device_erase(const struct lfs_config *c,
                                     lfs_block_t block)
{
term("*****  user_provided_block_device_erase  ******")

    //Изменил 4 * и вернул
    uint32_t addr = (uint32_t)( block * c->block_size);
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
term("lfs_mount")

// reformat if we can't mount the filesystem
// this should only happen on the first boot
    if (err) {

term("error - FORMAT lfs")

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

void FsForEeprom::numReboots()
{
    int numBoot = 0;
    lfs_file_open(FsForEeprom::getInstance().lfsPtr, FsForEeprom::getInstance().filePtr, "numBoot", LFS_O_RDWR | LFS_O_CREAT);
    lfs_file_read(FsForEeprom::getInstance().lfsPtr, FsForEeprom::getInstance().filePtr, &numBoot, sizeof(numBoot));
    lfs_file_close(FsForEeprom::getInstance().lfsPtr, FsForEeprom::getInstance().filePtr);
    ++numBoot;
    lfs_file_open(FsForEeprom::getInstance().lfsPtr, FsForEeprom::getInstance().filePtr, "numBoot", LFS_O_WRONLY );
    lfs_file_write(FsForEeprom::getInstance().lfsPtr, FsForEeprom::getInstance().filePtr, &numBoot, sizeof(numBoot));
    lfs_file_close(FsForEeprom::getInstance().lfsPtr, FsForEeprom::getInstance().filePtr);
    RS232::getInstance().term << "number of reboot = " << numBoot << "\r\n";
}

void FsForEeprom::test()
{
    char fileToEEPROM[] = "Red on top, Green below. Red says “Stop”, Green says “Go”. Yellow says “Wait” Even if you’re late.";
    char fileFromEEPROM[sizeof (fileToEEPROM)];
    lfs_file_open(FsForEeprom::getInstance().lfsPtr, FsForEeprom::getInstance().filePtr, "testEEPROM", LFS_O_RDWR | LFS_O_CREAT);
    lfs_file_rewind(FsForEeprom::getInstance().lfsPtr, FsForEeprom::getInstance().filePtr);
    lfs_file_write(FsForEeprom::getInstance().lfsPtr, FsForEeprom::getInstance().filePtr, &fileToEEPROM, sizeof(fileToEEPROM));
    lfs_file_close(FsForEeprom::getInstance().lfsPtr, FsForEeprom::getInstance().filePtr);

    lfs_file_open(FsForEeprom::getInstance().lfsPtr, FsForEeprom::getInstance().filePtr, "testEEPROM", LFS_O_RDWR | LFS_O_CREAT);
    lfs_file_read(FsForEeprom::getInstance().lfsPtr, FsForEeprom::getInstance().filePtr, &fileFromEEPROM, sizeof(fileFromEEPROM));
    lfs_file_close(FsForEeprom::getInstance().lfsPtr, FsForEeprom::getInstance().filePtr); // remember the storage is not updated until the file is closed successfully
    bool error = false;
    for (unsigned int i = 0; i < sizeof(fileToEEPROM); ++i)
    {
        if (fileToEEPROM[i] != fileFromEEPROM[i]) error = true;
    }
    if(error)
        term2("test EEPROM FAILED")
    if(!error)
        term2("test EEPROM passed")
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

