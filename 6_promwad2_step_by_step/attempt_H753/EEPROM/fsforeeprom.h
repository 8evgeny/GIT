#ifndef FSFOREEPROM_H
#define FSFOREEPROM_H

#include <stdint.h>
#include "lfs.h"

typedef struct FsForEeprom
{

    lfs_t *lfsPtr;
    lfs_file_t *filePtr;
} FsForEeprom_t;

void littleFsInit(void);

uint32_t Fs_init(void);
uint32_t Fs_read(uint8_t *pBuffer, uint16_t readAddr, uint16_t *numByteToRead);
uint32_t Fs_write(uint8_t *pBuffer, uint16_t writeAddr, uint16_t numByteToWrite);
void Fs_test();


#endif // FSFOREEPROM_H
