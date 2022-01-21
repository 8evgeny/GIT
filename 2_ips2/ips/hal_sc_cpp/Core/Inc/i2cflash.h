/*
 * i2cflash.h
 *
 *  Created on: Jun 17, 2020
 *      Author: Donskikh Dmitry
 */
#include "inttypes.h"

#ifndef INC_I2CFLASH_H_
#define INC_I2CFLASH_H_

#define IPS_BSP_MAXFILES	16
#define MAXFILENAME	12

#define FS_FLAG_OPENED 	(uint32_t)(1<<0)
#define FS_FLAG_RO	(uint32_t)(1<<1)

typedef struct
{
    uint32_t fflags;
    uint32_t fbegin;
    uint32_t fsize;
    char name[MAXFILENAME];
} __attribute__((packed)) fsfiledescription_t;
typedef struct
{
    uint32_t fscs;
    fsfiledescription_t fsnodes[IPS_BSP_MAXFILES];
} __attribute__((packed)) fstable;

typedef struct
{
    uint16_t node;
    fsfiledescription_t findex;
    uint32_t seek;
} FILE_t;

int extFlashRead (uint8_t *dest, uint32_t src, uint32_t size);
int extFlashWrite (uint32_t dest, uint8_t *src, uint32_t size);

int mkFStable (fstable *handle);
int fsFormat ();
int fsGetFileSize (uint16_t num);
int fsGetFileSizeN (const char *name);
int fsOpen (FILE_t *handler, uint16_t num);
int fsOpenN (FILE_t *handler, const char *name);
int fsRead (FILE_t *handler, uint32_t size, uint8_t *buffer);
int fsReadAround (FILE_t *handler, uint32_t size, uint8_t *buffer);
int fsWrite (FILE_t *handler, uint32_t size, uint8_t *buffer);
int fsClose (FILE_t *handler);
void fsCleanFileRecord (FILE_t *handler);
void fsNewOpen (const char *name, FILE_t *newfile);
int fsNewFile (const char *name);

#endif /* INC_I2CFLASH_H_ */
