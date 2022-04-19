/**
  ******************************************************************************
  * @file    driver_EEPROM.h
  * @author  dmib@@ETS
  * @brief   This file contains EEPROM driver module header
  ******************************************************************************
  */

#ifndef __DRIVER_EEPROM__
#define __DRIVER_EEPROM__

#include "stdint.h"

/** @defgroup driver_EEPROM EEPROM driver module
  * @brief EEPROM driver implementation
  * @{
  */

/// EEPROM I2C address
#define	M24M01_Address		0xA0

/// EEPROM page size
#define EEPROM_PAGE_SIZE	256
 
/** @defgroup driver_EEPROM_btypes Block system block types
  * @brief Block system block types
  * @{
  */
#define EEPROM_BLOCK_TYPE_UD	0			///< empty/undefined block
#define EEPROM_BLOCK_TYPE_SB	1			///< system block
#define EEPROM_BLOCK_TYPE_CNF	2			///< configuration block
#define EEPROM_BLOCK_TYPE_FW	3			///< firmware description block
#define EEPROM_BLOCK_TYPE_TEL	4			///< abonents block
///@}

/// maximum block number
#define MAX_BLOCK_NUM		8

#define SB_VERSION		0		///< current system block version
#define CNF_VERSION		1		///< current configuration block version
#define TEL_VERSION		0		///< current abonents block version

/// maximum number of EEPROM pages for configuration block
#define EEPROM_CNF_BLOCKS		448		// *256 = 112 кб

/// position of configuration block
#define EEPROM_CNF_POSITION		256
/// position of abonent description block
#define EEPROM_TEL_POSITION		(EEPROM_CNF_POSITION + 256*EEPROM_CNF_BLOCKS)

/// EEPROM block description entry structure
typedef struct {
	uint16_t Type;
	uint32_t Pos;
	uint32_t Size;
	uint32_t CRC32;
	uint32_t Ver;
} BlockEntry;

/// EEPROM abonent block entry structure
typedef struct{
	uint16_t tel_number;
	uint16_t IP;
	uint8_t CID[8];
} TelBlock;

/// main EEPROM system block structure
typedef struct{
	BlockEntry blocks[MAX_BLOCK_NUM];
	uint8_t  KeyAES[16];
	uint8_t  SN[12];
} SystemBlockHdr;

/**
  * @brief  EEPROM driver initialization
  * @retval result - 0 - initialization done (<0 - error)
  */
int8_t eeprom_init();

/**
  * @brief  get link to main system block structure
  * @retval result - link to main system block structure
  */
SystemBlockHdr *eeprom_get_system_SB();

/**
  * @brief  read system block from EEPROM
  * @param  [inout] sb - system block structure
  * @retval result - 0 - operation done (<0 - error)
  */
int8_t eeprom_read_SB(SystemBlockHdr *sb);

/**
  * @brief  write system block to EEPROM
  * @param  [in] sb - system block structure
  * @retval result - 0 - operation done (<0 - error)
  */
int8_t eeprom_write_SB(SystemBlockHdr *sb);

/**
  * @brief  read block from EEPROM
  * @param  [inout] sb - system block structure
  * @param  [in] blocknum - block number in system block
  * @param  [inout] block_mem - pointer to block data
  * @retval result - 0 - operation done (<0 - error)
  */
int8_t eeprom_read_block(SystemBlockHdr *sb, uint8_t blocknum, uint8_t *block_mem);

/**
  * @brief  write block to EEPROM
  * @param  [in] sb - system block structure
  * @param  [in] blocknum - block number in system block
  * @param  [in] block_mem - pointer to block data
  * @retval result - 0 - operation done (<0 - error)
  */
int8_t eeprom_write_block(SystemBlockHdr *sb, uint8_t blocknum, uint8_t *block_mem);

/**
  * @brief  make default system block
  * @param  [inout] sb - system block structure
  * @retval none
  */
void eeprom_default_SB(SystemBlockHdr *sb);

/**
  * @brief  update block CRC
  * @param  [inout] sb - system block structure
  * @param  [in] block_num - block number in system block
  * @param  [in] block_ptr - pointer to block data
  * @retval none
  */
void eeprom_update_block_CRC(SystemBlockHdr *sb, uint8_t block_num, uint8_t *block_ptr);

/**
  * @brief  search entry of block with given type
  * @param  [in] sb - system block structure
  * @param  [in] block_type - search block type
  * @retval result - index of block (-1 - block not found)
  */
int8_t eeprom_find_block(SystemBlockHdr *sb, uint8_t block_type);

/**
  * @brief  print system block information
  * @retval none
  */
void eeprom_print_SB();

/**
  * @brief  get total size of used EEPROM space
  * @param  [in] sb - system block structure
  * @retval result - total size size of used EEPROM space
  */
uint32_t eeprom_get_totalsize(SystemBlockHdr *sb);

/**
  * @brief  test EEPROM write timings
  * @retval result 0 - test passed; -1 - test failed
  */
int8_t eeprom_test_writedelay();

/**
  * @}
  */

#endif


