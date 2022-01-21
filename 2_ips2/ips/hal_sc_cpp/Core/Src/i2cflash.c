/*
 * i2cflash.c
 *
 *  Created on: Jun 17, 2020
 *      Author: Donskikh Dmitry
 */
#include <system_c.h>
#include "stm32h7xx_hal.h"
#include "i2cflash.h"

#include "string.h"
#include "telnet.h"

#define EXT_FLASH_BUSY		0xFF
#define EXT_FLASH_CPLT		0x00
#define EXT_FLASH_MAXCHUNK	256
#define MAX_NBYTE_SIZE		16*1024

extern I2C_HandleTypeDef hi2c1;

static uint8_t tx_state = EXT_FLASH_BUSY;
static uint8_t rx_state = EXT_FLASH_BUSY;

fstable g_fs;

static int fsGetNumByName (const char *name, fstable *handle);
static void fstableFlush (fstable *handle);
static void fsfileFlagSet (uint16_t num, uint8_t flags);
static void fsfileFlagReset (uint16_t num, uint8_t flags);
static int fsSearchFreeNode ();
static int fsSearchLastNode ();
void fsGetFsTable (fstable *fstab);

/*						HW IO Operations						*/
/*==============================================================*/
int extFlashRead (uint8_t *dest, uint32_t src, uint32_t size)
{
  while (HAL_I2C_GetState (&hi2c1) != HAL_I2C_STATE_READY);
  sysLedTestON ();
  uint32_t bytewriten = 0;
  printTelnet ("Read %d bytes from %d \r\n", size, src);
  while (size - bytewriten > EXT_FLASH_MAXCHUNK)
    {
      rx_state = EXT_FLASH_BUSY;
      if (HAL_I2C_Mem_Read_IT (&hi2c1, (0xA0) | (((src + bytewriten) & 0x10000) >> 15),
			       (uint16_t) ((src + bytewriten) & 0xFFFF), I2C_MEMADD_SIZE_16BIT,
			       (uint8_t*) (dest + (uint32_t) bytewriten), EXT_FLASH_MAXCHUNK)
	  != HAL_ERROR)
	{
	  while (rx_state == EXT_FLASH_BUSY);
	  //while (HAL_I2C_IsDeviceReady(&hi2c1, 0xA0, 3000, 30000)!=HAL_OK);
	  bytewriten += EXT_FLASH_MAXCHUNK;
	} else
	{
	  sysLedTestOFF ();
	  return HAL_I2C_GetError (&hi2c1);
	}
    }
  if (size - bytewriten > 0)
    {
      rx_state = EXT_FLASH_BUSY;
      if (HAL_I2C_Mem_Read_IT (&hi2c1, (0xA0) | (((src + bytewriten) & 0x10000) >> 15),
			       (uint16_t) ((src + bytewriten) & 0xFFFF), I2C_MEMADD_SIZE_16BIT,
			       (uint8_t*) (dest + (uint32_t) bytewriten), size - bytewriten)
	  != HAL_ERROR)
	{
	  while (rx_state == EXT_FLASH_BUSY);
	  //while (HAL_I2C_IsDeviceReady(&hi2c1, 0xA0, 3000, 30000)!=HAL_OK);
	  bytewriten += (size - bytewriten);
	} else
	{
	  sysLedTestOFF ();
	  return HAL_I2C_GetError (&hi2c1);
	}
    }

  sysLedTestOFF ();
  return 0;
}

int extFlashWrite (uint32_t dest, uint8_t *src, uint32_t size)
{
  while (HAL_I2C_GetState (&hi2c1) != HAL_I2C_STATE_READY);
  sysLedTestON ();
  uint32_t bytewriten = 0;
  printTelnet ("Write %d bytes to %d \r\n", size, dest);
  while (size - bytewriten > EXT_FLASH_MAXCHUNK)
    {
      tx_state = EXT_FLASH_BUSY;
      if (HAL_I2C_Mem_Write_IT (&hi2c1, (0xA0) | (((dest + bytewriten) & 0x10000) >> 15),
				(uint16_t) ((dest + bytewriten) & 0xFFFF), I2C_MEMADD_SIZE_16BIT,
				src + bytewriten, EXT_FLASH_MAXCHUNK) != HAL_ERROR)
	{
	  while (tx_state == EXT_FLASH_BUSY);
	  while (HAL_I2C_IsDeviceReady (&hi2c1, 0xA0, 3000, 30000) != HAL_OK);
	  bytewriten += EXT_FLASH_MAXCHUNK;
	} else
	{
	  sysLedTestOFF ();
	  return HAL_I2C_GetError (&hi2c1);
	}
    }
  if (size - bytewriten > 0)
    {
      tx_state = EXT_FLASH_BUSY;
      if (HAL_I2C_Mem_Write_IT (&hi2c1, (0xA0) | (((dest + bytewriten) & 0x10000) >> 15),
				(uint16_t) ((dest + bytewriten) & 0xFFFF), I2C_MEMADD_SIZE_16BIT,
				src + bytewriten, size - bytewriten) != HAL_ERROR)
	{
	  while (tx_state == EXT_FLASH_BUSY);
	  while (HAL_I2C_IsDeviceReady (&hi2c1, 0xA0, 3000, 30000) != HAL_OK);
	  bytewriten += size - bytewriten;
	} else
	{
	  sysLedTestOFF ();
	  return HAL_I2C_GetError (&hi2c1);
	}
    }

  sysLedTestOFF ();
  return size;
}

void HAL_I2C_MemTxCpltCallback (I2C_HandleTypeDef *hi2c)
{
  tx_state = EXT_FLASH_CPLT;
}

void HAL_I2C_MemRxCpltCallback (I2C_HandleTypeDef *hi2c)
{
  rx_state = EXT_FLASH_CPLT;
}

/*					Simple filesystem Operations				*/
/*==============================================================*/

int mkFStable (fstable *handle)
{
  uint32_t i_fs_checksum = 0x00;
  extFlashRead ((uint8_t*) (&i_fs_checksum), 0x0000, sizeof(i_fs_checksum));
  if (i_fs_checksum != 0xAAAAAAAA)
    {
      fsFormat ();
    }
  extFlashRead ((uint8_t*) handle, 0x0000, sizeof(fstable));
  return 0;
}

int fsFormat ()
{
  fstable newfs;
  memset (&newfs, 0x00, sizeof(fstable));
  newfs.fscs = 0xAAAAAAAA;
  extFlashWrite (0x0, (uint8_t*) (&newfs), sizeof(fstable));
  return 0;
}

void fsGetFsTable (fstable *fstab)
{
  extFlashRead ((uint8_t*) fstab, 0, sizeof(fstable));
  return;
}

int fsGetFileSize (uint16_t num)
{
  fsGetFsTable (&g_fs);
  return (g_fs.fsnodes[num].fbegin == 0) ? (-1) : g_fs.fsnodes[num].fsize;
}

int fsGetFileSizeN (const char *name)
{
  fsGetFsTable (&g_fs);
  int fs_node = fsGetNumByName (name, &g_fs);
  return (fs_node < 0) ? fs_node : g_fs.fsnodes[fs_node].fsize;
}

int fsOpen (FILE_t *handler, uint16_t num)
{
  fsGetFsTable (&g_fs);
  if ((g_fs.fsnodes[num].fbegin == 0) || (g_fs.fsnodes[num].fflags & FS_FLAG_OPENED ))
    {
      return -1;
    }
  memcpy (&(handler->findex), &(g_fs.fsnodes[num]), sizeof(fsfiledescription_t)); //TODO:COPY
  handler->seek = 0;
  handler->findex.fflags |= FS_FLAG_OPENED;
  handler->node = num;
  fsfileFlagSet (num, FS_FLAG_OPENED);
  return 0;
}

int fsOpenN (FILE_t *handler, const char *name)
{
  fsGetFsTable (&g_fs);
  int fs_node = fsGetNumByName (name, &g_fs);
  if (fs_node < 0)
    {
      return fs_node;
    }
  return fsOpen (handler, fs_node);
}

int fsRead (FILE_t *handler, uint32_t size, uint8_t *buffer)
{
  if (!(handler->findex.fflags & FS_FLAG_OPENED ))
    {
      return -1;
    }
  int read_sz =
      ((handler->seek + size) < handler->findex.fsize) ?
	  size : (handler->findex.fsize - handler->seek);
  extFlashRead (buffer, handler->findex.fbegin + handler->seek, read_sz);
  handler->seek += read_sz;
  return read_sz;
}

int fsReadAround (FILE_t *handler, uint32_t size, uint8_t *buffer)
{
  if (!(handler->findex.fflags & FS_FLAG_OPENED ))
    {
      return -1;
    }
  if ((handler->seek + size) < handler->findex.fsize)
    {
      extFlashRead (buffer, handler->findex.fbegin + handler->seek, size);
      return size;
    } else
    {
      uint32_t first_part = handler->findex.fsize - handler->seek;
      extFlashRead (buffer, handler->findex.fbegin + handler->seek, first_part);
      extFlashRead (buffer + first_part, handler->findex.fbegin, size - first_part);
      handler->seek = size - first_part;
      return size;
    }
}

int fsWrite (FILE_t *handler, uint32_t size, uint8_t *buffer)
{
  if ((!(handler->findex.fflags & FS_FLAG_OPENED )) || (handler->findex.fflags & FS_FLAG_RO ))
    {
      return -1;
    }
  printTelnet ("extFlashWrite(%d,%d)\r\n", handler->findex.fbegin + handler->seek, size);
  extFlashWrite (handler->findex.fbegin + handler->seek, buffer, size);
  ////	some kinda checks
  uint8_t *check_buf = pvPortMalloc (size);
  extFlashRead (check_buf, handler->findex.fbegin + handler->seek, size);
  for (uint32_t i = 0; i < size; i++)
    {
      if (*(buffer + i) != *(check_buf + i))
	{
	  printTelnet ("Data verify mismatch @ %d :EEPROM value: %x Buffer value: %x \r\n",
		       i + handler->findex.fbegin + handler->seek, *(check_buf + i), *(buffer + i));
	}
    }
  fstable table;
  fsGetFsTable (&table);
  printTelnet ("\r\nNode table:\r\n");
  for (int i = 0; i < IPS_BSP_MAXFILES; i++)
    {
      printTelnet ("%d: start:%d size:%d bytes name: %s \n", i, table.fsnodes[i].fbegin,
		   table.fsnodes[i].fsize, table.fsnodes[i].name);
    }
  vPortFree (check_buf);
  /////	end checks
  handler->seek += size;
  handler->findex.fsize += size; //(handler->seek<handler->findex.fsize)?handler->findex.fsize:handler->seek;
  return size;
}

int fsClose (FILE_t *handler)
{
  if (!(handler->findex.fflags & FS_FLAG_OPENED ))
    {
      return -1;
    }
  fsGetFsTable (&g_fs);
  handler->findex.fflags &= (~FS_FLAG_OPENED );
  memcpy (&(g_fs.fsnodes[handler->node]), &(handler->findex), sizeof(fsfiledescription_t));
  fstableFlush (&g_fs);
  memset (handler, 0x00, sizeof(FILE_t));
  return 0;
}

void fsCleanFileRecord (FILE_t *handler)
{
  fsGetFsTable (&g_fs);
  memset (&(g_fs.fsnodes[handler->node]), 0x00, sizeof(fsfiledescription_t));
  memset (handler, 0x00, sizeof(FILE_t));
  fstableFlush (&g_fs);
}

int fsNewFile (const char *name)
{
  fsGetFsTable (&g_fs);
  int free_node = fsSearchFreeNode ();
  if (free_node < 0)
    {
      return -1;
    }
  int name_len = strlen (name);
  name_len = (name_len < MAXFILENAME) ? name_len : MAXFILENAME;
  int last_node = fsSearchLastNode ();
  if (g_fs.fsnodes[last_node].fbegin == 0)
    {
      g_fs.fsnodes[last_node].fbegin = 0 + sizeof(fstable) + (256 - sizeof(fstable) % 256);
      g_fs.fsnodes[last_node].fsize = 0;
      g_fs.fsnodes[last_node].fflags = 0;
      memcpy (&(g_fs.fsnodes[last_node].name), name, name_len);
      fstableFlush (&g_fs);
      return last_node;
    }
  fsfileFlagSet (last_node, FS_FLAG_RO);
  g_fs.fsnodes[free_node].fbegin = (g_fs.fsnodes[last_node].fbegin + g_fs.fsnodes[last_node].fsize)
      + 256 - (g_fs.fsnodes[last_node].fbegin + g_fs.fsnodes[last_node].fsize) % 256;
  g_fs.fsnodes[free_node].fflags = 0;
  g_fs.fsnodes[free_node].fsize = 0;
  memcpy (&(g_fs.fsnodes[free_node].name), name, name_len);
  fstableFlush (&g_fs);
  return free_node;
}

void fsNewOpen (const char *name, FILE_t *newfile)
{
  fsOpen (newfile, fsNewFile (name));
  return;
}

/*				Simple filesystem static functions				*/
/*==============================================================*/

int fsGetNumByName (const char *name, fstable *handle)
{
  uint16_t cnt = 0;
  uint16_t pos = 0;
  for (int i = 0; i < IPS_BSP_MAXFILES; i++)
    {
      if (strstr (&(handle->fsnodes[i].name), name) != NULL)
	{
	  cnt++;
	  pos = i;
	}
    }
  if (cnt != 1)
    {
      return -1;
    }
  return pos;
}

void fstableFlush (fstable *handle)
{
  extFlashWrite (0, (uint8_t*) handle, sizeof(fstable));
}

void fsfileFlagSet (uint16_t num, uint8_t flags)
{
  fsGetFsTable (&g_fs);
  g_fs.fsnodes[num].fflags |= flags;
  fstableFlush (&g_fs);
}

void fsfileFlagReset (uint16_t num, uint8_t flags)
{
  fsGetFsTable (&g_fs);
  g_fs.fsnodes[num].fflags &= (~flags);
  fstableFlush (&g_fs);
}
int fsSearchFreeNode ()
{
  fsGetFsTable (&g_fs);
  uint16_t index = 0;
  while ((g_fs.fsnodes[index].fbegin != 0) && (index < IPS_BSP_MAXFILES))
    {
      index++;
    }
  return (index < IPS_BSP_MAXFILES) ? index : (-1);
}

int fsSearchLastNode ()
{
  fsGetFsTable (&g_fs);
  uint32_t fbegin_max_ptr = 0;
  int index = 0;
  for (int i = 0; i < IPS_BSP_MAXFILES; i++)
    {
      index = (g_fs.fsnodes[i].fbegin > g_fs.fsnodes[index].fbegin) ? i : index;
    }
  return index;
}
