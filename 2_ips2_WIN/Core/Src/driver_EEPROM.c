/**
  ******************************************************************************
  * @file    driver_EEPROM.c
  * @author  dmib@@ETS
  * @brief   This file contains EEPROM driver implementation
  ******************************************************************************
  */

#include "stm32h7xx_hal.h"
#include "string.h"
#include "CLI_io.h"
#include "CRC32.h"
#include "AES_codec.h"
#include "driver_EEPROM.h"

/// EEPROM driver print error level
#define EEPROM_CLI_ERROR_LEVEL	CLI_PRINTLEVEL_WORK

/// EEPROM maximum write delay time
#define EEPROM_MAX_WRDELAY	100

/// link to I2C1 hardware module control/status structure
extern I2C_HandleTypeDef hi2c1;

/// main system block
SystemBlockHdr eeprom_hdr;

/**
  * @brief  read bytes from EEPROM
  * @param  [in] address - EEPROM I2C address
  * @param  [inout] data - array of bytes
  * @param  [in] size - size of data
  * @retval result - HAL operation result
  */
uint8_t eeprom_read_bytes(uint32_t address, uint8_t *data, uint32_t size)
{
   return HAL_I2C_Mem_Read(&hi2c1, M24M01_Address | ((address >> 16) << 1), address,I2C_MEMADD_SIZE_16BIT, data, size , 100);
}

/**
  * @brief  write bytes to EEPROM
  * @param  [in] address - EEPROM I2C address
  * @param  [in] data - array of bytes
  * @param  [in] size - size of data
  * @retval result - HAL operation result
  */
uint8_t eeprom_write_bytes(uint32_t address, uint8_t *data, uint32_t size)
{
  return HAL_I2C_Mem_Write(&hi2c1, M24M01_Address | ((address >> 16) << 1), address,I2C_MEMADD_SIZE_16BIT, data, size , 100);
}

/**
  * @brief  polling wait for write operation
  * @param  [in] adr - EEPROM I2C address
  * @param  [inout] delay - actual delay
  * @retval result - 0 - operation done (<0 - error)
  */
uint8_t eeprom_waitwrite_polling(uint32_t adr, uint32_t *delay)
{
	uint8_t data;
	uint32_t wait_time;

	wait_time = HAL_GetTick();

	while ((HAL_GetTick() - wait_time)<EEPROM_MAX_WRDELAY) {
		if (eeprom_read_bytes(adr, &data, 1) == HAL_OK) {
			if (delay) *delay = HAL_GetTick() - wait_time;
			return HAL_OK;
		}
	}
	*delay = EEPROM_MAX_WRDELAY;
	return HAL_ERROR;
}

#define EEPROM_NUM_PAGES		512

int8_t eeprom_test_writedelay()
{
	uint32_t i, wait_time,delay;

	wait_time = HAL_GetTick();
	if (eeprom_write_bytes(0, (uint8_t *)&eeprom_hdr, 1) != HAL_OK)
	{
		CLI_print("Error EEPROM: write byte\r\n");
		return -1;
	}
	eeprom_waitwrite_polling(0, NULL);
	delay = HAL_GetTick() - wait_time;
	CLI_print("EEPROM byte write delay: %i\r\n", delay);
	
	wait_time = HAL_GetTick();
	if (eeprom_write_bytes(0, (uint8_t *)&eeprom_hdr, EEPROM_PAGE_SIZE) != HAL_OK)
	{
		CLI_print("Error EEPROM: write block\r\n");
		return -1;
	}
	eeprom_waitwrite_polling(0, NULL);
	delay = HAL_GetTick() - wait_time;
	CLI_print("EEPROM block write delay: %i\r\n", delay);
	
	wait_time = HAL_GetTick();
	for (i = 0; i < EEPROM_NUM_PAGES; i++) {
		if (eeprom_write_bytes(i * EEPROM_PAGE_SIZE, (uint8_t *)&eeprom_hdr, EEPROM_PAGE_SIZE) != HAL_OK)
		{
			CLI_print("Error EEPROM: write block %i\r\n", i);
			return -1;
		}
		eeprom_waitwrite_polling(0, NULL);
	}
	delay = HAL_GetTick() - wait_time;
	CLI_print("EEPROM full write delay: %i\r\n", delay);
	
	return 0;
}

void eeprom_update_block_CRC(SystemBlockHdr *sb, uint8_t block_num, uint8_t *block_ptr)
{
  sb->blocks[block_num].CRC32 = 0;
  sb->blocks[block_num].CRC32 = crc32_calc(0, block_ptr,sb->blocks[block_num].Size);
}

int8_t eeprom_read_SB(SystemBlockHdr *sb)
{
  uint32_t orig_crc, res;
    
  res = eeprom_read_bytes(0, (uint8_t *)sb, sizeof(SystemBlockHdr));
  if (res != HAL_OK) return -1;
  
  if (sb->blocks[0].Type != EEPROM_BLOCK_TYPE_SB) return -2;
  if (sb->blocks[0].Pos != 0) return -2;
  if (sb->blocks[0].Size != sizeof(SystemBlockHdr)) return -2;
  
  orig_crc = sb->blocks[0].CRC32;
  eeprom_update_block_CRC(sb, 0, (uint8_t *)sb);
  
  if (orig_crc != sb->blocks[0].CRC32) return -3;
  
  return 0;
}

int8_t eeprom_write_SB(SystemBlockHdr *sb)
{
  eeprom_update_block_CRC(sb, 0, (uint8_t *)sb);
  if (eeprom_write_bytes(0, (uint8_t *)sb, sizeof(SystemBlockHdr)) != HAL_OK) return -1;
  if (eeprom_waitwrite_polling(0,0)!=HAL_OK) return -1;

  return eeprom_read_SB(sb);
}

uint32_t eeprom_get_totalsize(SystemBlockHdr *sb)
{
	uint32_t tot_size;
	uint8_t i;

	tot_size = 0;
	for (i = 0; i < MAX_BLOCK_NUM; i++) {
		if (sb->blocks[i].Type == EEPROM_BLOCK_TYPE_UD) break;
		if (sb->blocks[i].Type == EEPROM_BLOCK_TYPE_FW) continue;
		tot_size += sb->blocks[i].Size;
	}

	return tot_size;
}

int8_t eeprom_read_block(SystemBlockHdr *sb, uint8_t blocknum, uint8_t *block_mem)
{
	uint32_t crc32, eeprom_adr;
	uint16_t i,blk_num;
	uint8_t *mem_ptr, res;

	//only if EEPROM_PAGE_SIZE=256
	blk_num = sb->blocks[blocknum].Size >> 8;
	if (sb->blocks[blocknum].Size & 0xFF) blk_num++;

	eeprom_adr = sb->blocks[blocknum].Pos;
	mem_ptr = block_mem;
	for (i=0 ; i < blk_num; i++ ) {
	 res = eeprom_read_bytes(eeprom_adr, mem_ptr, EEPROM_PAGE_SIZE);
     if (res != HAL_OK) return -1;
     eeprom_adr += EEPROM_PAGE_SIZE;
	 mem_ptr += EEPROM_PAGE_SIZE;
	}

    crc32 = crc32_calc(0, block_mem,sb->blocks[blocknum].Size);
	if (crc32 != sb->blocks[blocknum].CRC32) return -3;

    return 0;
}

int8_t eeprom_write_block(SystemBlockHdr *sb, uint8_t blocknum, uint8_t *block_mem)
{
	uint32_t eeprom_adr;
	uint16_t i,blk_num;
	uint8_t *mem_ptr, res;

	//only if EEPROM_PAGE_SIZE=256
	blk_num = sb->blocks[blocknum].Size >> 8;
	if (sb->blocks[blocknum].Size & 0xFF) blk_num++;

	eeprom_update_block_CRC(sb, blocknum, block_mem);

	eeprom_adr = sb->blocks[blocknum].Pos;
	mem_ptr = block_mem;
	for (i=0 ; i < blk_num; i++ ) {
	 res = eeprom_write_bytes(eeprom_adr, mem_ptr, EEPROM_PAGE_SIZE);
     if (res != HAL_OK) return -1;
     if (eeprom_waitwrite_polling(eeprom_adr,0)!=HAL_OK) return -1;
     eeprom_adr += EEPROM_PAGE_SIZE;
	 mem_ptr += EEPROM_PAGE_SIZE;
	}

    return 0;
}

void eeprom_default_SB(SystemBlockHdr *sb)
{
  memset(sb, 0, sizeof(SystemBlockHdr));

  sb->blocks[0].Type = EEPROM_BLOCK_TYPE_SB;
  sb->blocks[0].Pos = 0;
  sb->blocks[0].Size = sizeof(SystemBlockHdr);
  sb->blocks[0].Ver = SB_VERSION;
  sb->blocks[0].CRC32 = 0;

  sb->blocks[1].Type = EEPROM_BLOCK_TYPE_CNF;
  sb->blocks[1].Pos = EEPROM_CNF_POSITION;
  sb->blocks[1].Size = 0;
  sb->blocks[1].Ver = CNF_VERSION;
  sb->blocks[1].CRC32 = 0;

  sb->blocks[2].Type = EEPROM_BLOCK_TYPE_FW;
  sb->blocks[2].Pos = 0;
  sb->blocks[2].Size = 0;
  sb->blocks[2].Ver = 0;
  sb->blocks[2].CRC32 = 0;

  sb->blocks[3].Type = EEPROM_BLOCK_TYPE_TEL;
  sb->blocks[3].Pos = EEPROM_TEL_POSITION;
  sb->blocks[3].Size = 0;
  sb->blocks[3].Ver = TEL_VERSION;
  sb->blocks[3].CRC32 = 0;

  eeprom_update_block_CRC(sb, 0, (uint8_t *)sb);
}

int8_t eeprom_find_block(SystemBlockHdr *sb, uint8_t block_type)
{
  uint8_t i;
  for (i = 0; i<MAX_BLOCK_NUM; i++) 
   if (sb->blocks[i].Type == block_type) return i;
  return -1;
}

const char eeprom_err1[] = "Error: EEPROM read error\r\n";
const char eeprom_err2[] = "Error: EEPROM invalid filesystem\r\n";
const char eeprom_err3[] = "Error: EEPROM invalid CRC\r\n";
const char eeprom_err4[] = "Error: EEPROM error\r\n";

int8_t eeprom_init()
{
	int8_t res;
	const char *str;

	res = eeprom_read_SB(&eeprom_hdr);
	if (res < 0) {
		switch (res) {
		 case -1: str = eeprom_err1;break;
		 case -2: str = eeprom_err2;break;
		 case -3: str = eeprom_err3;break;
		 default: str = eeprom_err4;break;
		}
		CLI_print_lev(EEPROM_CLI_ERROR_LEVEL, (char *)str);
		eeprom_default_SB(&eeprom_hdr);
	}

	AES_key_init(AES_get_static_state(), &eeprom_hdr.KeyAES[0]);

	return res;
}

SystemBlockHdr *eeprom_get_system_SB()
{
	return &eeprom_hdr;
}

void eeprom_print_SB()
{
	int8_t i;
	char strAES[33];

	Array2Hex(eeprom_hdr.KeyAES, 16, strAES);
	strAES[32] = 0;

	CLI_print("----------\r\n");
	CLI_print("System block\r\n");

	CLI_print("- SN : %s\r\n", eeprom_hdr.SN);
	CLI_print("- KeyAES : %s\r\n", strAES);

	for (i = 0; i < MAX_BLOCK_NUM; i++) {
	  CLI_print("- Block %i:\r\n" , i);
	  CLI_print("-- Type : %i\r\n", eeprom_hdr.blocks[i].Type);
	  CLI_print("-- Pos : %i\r\n", eeprom_hdr.blocks[i].Pos);
	  CLI_print("-- Size : %i\r\n", eeprom_hdr.blocks[i].Size);
	  CLI_print("-- CRC32 : %.8X\r\n", eeprom_hdr.blocks[i].CRC32);
	  CLI_print("-- Ver : %i\r\n", eeprom_hdr.blocks[i].Ver);
	}

	CLI_print("----------");
}
