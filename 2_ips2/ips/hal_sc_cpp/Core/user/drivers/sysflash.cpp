/*
 * sysflash.cpp
 *
 *  Created on: Aug 29, 2020
 *      Author: Dmitry Donskih
 */

/*
 * sysflash.cpp - a part of CMppCore -an object engine for Cortex-M
 * microcontrollers.
 *
 * Copyright (c) 2019-2020 by Dmitry Donskih
 * All rights reserved.
 *
 * Attention Software modification allowed only with
 * the permission of the author.
 */

#include <sysflash.hpp>
#include <cstring>
#include "main.h"
#include "stm32h7xx_hal_flash.h"
#include "stm32h7xx_hal_flash_ex.h"
#include "perf.hpp"
#include <ips_globals.hpp>
#include <ipssystem.hpp>

#define IFLASH_MEM2BLOCK(mem)	(void*)((uint32_t)mem&(~0x1F))
#define IFLASH_MEM2OFFSET(mem)	((uint32_t)mem&(0x1F))

#define IFLASH_ADDRESS(x)	IFLASH_MEM2BLOCK(x),IFLASH_MEM2OFFSET(x)

systemFlash::systemFlash ()
{
  i_buffer = (flashword_t*) system_cpp.allocateDmaBuffer (sizeof(flashword_t));
  o_buffer = (flashword_t*) system_cpp.allocateDmaBuffer (sizeof(flashword_t));
  blk_buffer = (flashword_t*) system_cpp.allocateDmaBuffer (sizeof(flashword_t));

  HAL_FLASHEx_Unlock_Bank2 ();

}

flashword_t systemFlash::readFlashword (void *address)
{
  read_block (address);
  flashword_t temp;
  __DMB ();
  memcpy (&temp, i_buffer, sizeof(flashword_t));
  return temp; //*(reinterpret_cast<flashword_t*>(address));
}

//int systemFlash::read_data (void *mem_dest, void *flash_src, size_t size)
//{
//  for (size_t i = 0; i < size; i++)
//    {
//      if(i%32==0)
//	{
//	  __ISB ();
//	  __DSB ();
//	}
//      ((uint8_t*) mem_dest)[i] = ((uint8_t*) flash_src)[i];
//    }
//  __ISB ();
//  __DSB ();
//  return 0;
//}

void systemFlash::read_block (void *adress)
{
  ipsSystem::debug->printfl ("%X alligned to %X\n", adress, IFLASH_MEM2BLOCK(adress));
  memcpy (i_buffer, IFLASH_MEM2BLOCK(adress), sizeof(flashword_t));
  __ISB ();
  __DSB ();
}

int systemFlash::write_block (void *flash_dest, void *mem_src)
{

  ipsSystem::debug->printfl ("write_block (void 0x%X, void 0x%X)\n", flash_dest, mem_src);
  __DMB ();
  if (IFLASH_MEM2OFFSET(flash_dest) != 0)
    {
      ipsSystem::debug->printfl ("ASSERT FAIL: Unalligned flash access@0x%X\n", flash_dest);
      return -1;
    }
  if (HAL_FLASH_Program (FLASH_TYPEPROGRAM_FLASHWORD, (uint32_t) flash_dest, (uint32_t) mem_src)
      != HAL_OK)
    {
      errno = HAL_FLASH_GetError ();
      ipsSystem::debug->printfl ("Error writing to internal FLASH from %x to %x\n",
				 (uint32_t) mem_src, (uint32_t) flash_dest);
      ipsSystem::debug->printfl ("Errno=%b \n", errno);
      return -1;
    }
  osDelay(10);
  errno = 0;
  __ISB ();
  __DSB ();
  return 0;
}

int systemFlash::write_bytes (void *flash_dest, void *mem_src, size_t size)
{
  ipsSystem::debug->printfl ("write_bytes(void 0x%X, void 0x%X, size_t %d)\n", flash_dest, mem_src,
			     size);
  read_block (IFLASH_MEM2BLOCK(flash_dest));
  memcpy (&(i_buffer->b[IFLASH_MEM2OFFSET(flash_dest)]), mem_src, size);
  return write_block (IFLASH_MEM2BLOCK(flash_dest), i_buffer);
}

int systemFlash::erase_sector (uint32_t sector)
{
  HAL_StatusTypeDef FlashStatus;
  uint32_t SectorError = 0;
  FLASH_EraseInitTypeDef pEraseInit;

  pEraseInit.TypeErase = TYPEERASE_SECTORS;
  pEraseInit.Banks = FLASH_BANK_2;
  pEraseInit.Sector = sector;
  pEraseInit.NbSectors = 1;
  pEraseInit.VoltageRange = VOLTAGE_RANGE_4;

  FlashStatus = HAL_FLASHEx_Erase (&pEraseInit, &SectorError);
  if (FlashStatus != HAL_OK)
    {
      errno = SectorError;
      return -1;
    }
  errno = 0;
  return 0;
}

systemFlash& systemFlash::getInstance ()
{
  static systemFlash instance;
  return instance;
}

int systemFlash::write (int32_t *dest, int value)
{
  return write_bytes (dest, &value, sizeof(int));
}

int systemFlash::write (void *dest, void *src, size_t size)
{
  unsigned int i = 0;
//	for(i=0;i<size/SYS_IC_FLASHWORD_SIZE;i+=SYS_IC_FLASHWORD_SIZE){
  for (i = 0; i < size / SYS_IC_FLASHWORD_SIZE; i++)
    {
      if (write_bytes ((uint8_t*) dest + i * SYS_IC_FLASHWORD_SIZE,
		       (uint8_t*) src + i * SYS_IC_FLASHWORD_SIZE, SYS_IC_FLASHWORD_SIZE) < 0)
	return -1;
    }
  write_bytes ((uint8_t*) dest + i * SYS_IC_FLASHWORD_SIZE,
	       (uint8_t*) src + i * SYS_IC_FLASHWORD_SIZE, size % SYS_IC_FLASHWORD_SIZE);
  return 0;
}

int systemFlash::fill (void *dest, int value, size_t size)
{
  return 0;
}

systemFlash &intFlashIO = systemFlash::getInstance ();

#ifdef TESTS
#include <ipcCLI.hpp>
#include <sysRNG.hpp>
char text_placeholder[] =
    "\nThis is flash IO operations test message!\n\
Text placeholder will write to internal flash memory to FS sectors.\n\n\
abcdefghijklmnopqrstuvwxyz ABCDEFGHIJKLMNOPQRSTUVWXYZ 0123456789\n\
Lorem ipsum dolor sit amet, consectetur adipiscing elit,\n\
sed do eiusmod tempor incididunt ut labore et dolore magna aliqua.\n\
Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris\n\
nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor in\n\
reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla\n\
pariatur. Excepteur sint occaecat cupidatat non proident, sunt in\n\
culpa qui officia deserunt mollit anim id est laborum.\n\
That's all folks ;-)\n";

static int32_t flash_test[256] __attribute__ ((section (".intflash")));

#define TEST_WORD_SIZE 1024

void testFlashFs (void *args)
{
  static flashword_t test_block =
    { 0, 0, 0, 0, 0, 0, 0, 0 };

  char *buffer = (char*) pvPortMalloc (TEST_WORD_SIZE);
  memset (buffer, 0, sizeof(TEST_WORD_SIZE));
  ttY *term = (reinterpret_cast<app_param_t*> (args)->terminal);

  perf_t proc_time;
  term->printfl ("\nTest allign macro\n");
  for (int i = 0; i < 69; i++)
    {
      if (i % 3 == 0) term->printfl ("\n");
      term->printfl ("%d: %X = A(%X),O(%d); ", i, &flash_test[i], IFLASH_ADDRESS(&flash_test[i]));
    }

  term->printfl ("\nRead current state\n");
  for (uint8_t i = 0; i < 125; i += 8)
    {
      test_block = intFlashIO.readFlashword (&flash_test[i]);
      for (uint8_t j = 0; j < 8; j++)
	{
	  term->printfl ("%d, ", test_block.i[j]);
	}
      term->printfl ("\n");
    }

  term->printfl ("Trying to read first 2KB Flash as chars..\n");

//  for (int i = 0; i < (TEST_WORD_SIZE); ++i)
//    {
//      term->printfl ("%c", ((char*) &flash_test)[i]);
//    }

  term->printfl ("\nLet's Flash IO test begins!!\n\n1.RNG test: char[%d] generated 10 times..\n",
		 2 * 1024);

//  intFlashIO.write(&flash_test, &text_placeholder, sizeof(text_placeholder));

  for (int j = 0; j < 10; ++j)
    {
      sysRNG::getInstance ().numbers (buffer, (TEST_WORD_SIZE));
//

      intFlashIO.write (&flash_test, buffer, TEST_WORD_SIZE);
      term->printfl("invalidate flash cache\n");
      SCB_InvalidateDCache_by_Addr((uint32_t*)IFLASH_MEM2BLOCK(&flash_test), TEST_WORD_SIZE);
      term->printfl("invalidate buffer cache\n");
      SCB_InvalidateDCache_by_Addr((uint32_t*)IFLASH_MEM2BLOCK(&buffer), TEST_WORD_SIZE);
      int errors = 0;
      char fl_w_temp=0;
      for (int i = 0; i < (TEST_WORD_SIZE); i++)
	{
	  fl_w_temp=*(((char*)&flash_test)+i);
	  if (buffer[i] != fl_w_temp)
	    {
	      //term->printfl ("Mismatch at %d\n", i);
	      //HAL_Delay (1);
	      term->printfl ("|");
	      errors++;
	    }
	  osDelay(10);
	}

      term->printfl ("Check %d:RW Error count: %d\n", j, errors);
      if (((errors * 100) / (TEST_WORD_SIZE)) > 20)
	{
	  term->printfl ("FATAL: Error raito:%d%%\n", (errors * 100) / (TEST_WORD_SIZE));
	  break;
	} else
	{
	  term->printfl ("Error raito:%d%%\n", (errors * 100) / (TEST_WORD_SIZE));
	}
    }
//  PERFCHECK_START;
//	for(uint8_t i=0;i<125;i++){
//		intFlashIO.write(&flash_test[i], (int)i);
//	}
//  PERFCHECK_STOP(proc_time);
  //if(intFlashIO.write_bytes(&flash_test[0], &test_block1,sizeof(test_block1))<0)ipsSystem::debug->printfl("fail write to %X errno(%d)\n",&flash_test[0],intFlashIO.errno);
  //if(intFlashIO.write_bytes(&flash_test[16], &test_block2,sizeof(test_block2))<0)ipsSystem::debug->printfl("fail write to %X errno(%d)\n",&flash_test[16],intFlashIO.errno);


  vPortFree (buffer);
}

void formatIntFlash (void *args)
{
  ttY *term = (reinterpret_cast<app_param_t*> (args)->terminal);
  if (intFlashIO.erase_sector (0) < 0)
    {
      term->printfl ("Error sector erasing.ERROR(%d)\n", intFlashIO.errno);
    } else
    {
      term->printfl ("Sector sucsessfully erased\n");
    }
}

#endif
