/**
  ******************************************************************************
  * @file    MemTest.h
  * @author  dmib@@ETS
  * @brief   This file contains RAM tests module header
  ******************************************************************************
  */
#ifndef __MEM_TEST__
#define __MEM_TEST__

#include "stdint.h"

/** @defgroup MEMTEST_module RAM test module
  * @brief RAM test functions
  * @{
  */

/** @defgroup MEMTEST_ERR RAM test error codes
  * @brief RAM test error codes
  * @{
  */
#define TESTMEM_ERROR_BYTE			(1<<0)
#define TESTMEM_ERROR_DATA_R0		(1<<1)
#define TESTMEM_ERROR_DATA_R1		(1<<2)
#define TESTMEM_ERROR_ADDRESS		(1<<3)
#define TESTMEM_ERROR_ADDRESS_R0	(1<<4)
#define TESTMEM_ERROR_ADDRESS_R1	(1<<5)
#define TESTMEM_ERROR_ADDRESS_PTR	(1<<6)
#define TESTMEM_ERROR_ADDRESS_HASH	(1<<7)
#define TESTMEM_ERROR_ADDRESS_CNT	(1<<8)
/**
  * @}
  */

/**
  * @brief  RAM test result structure
  */
typedef struct 
{
	uint32_t error_flags;
	uint32_t error_address;
	uint32_t error_wdata;
	uint32_t error_rdata;
} SRam_Status;

/** @defgroup MEMTEST_FUNC RAM test functions
  * @brief RAM test functions
  * @{
  */
  
/**
  * @brief  full RAM tests
  * @param  [in] start_address - RAM start address
  * @param  [in] len - test length (range) in bytes
  * @param  [inout] ram_st - RAM test result structure
  * @param  [in] printout - print result to CLI (0 - no/1 - yes)
  * @retval error_flags - test RAM error flags
  */
uint8_t Test_RAM(uint32_t start_address, uint32_t len, SRam_Status* ram_st, uint8_t printout);

/**
  * @brief  fast RAM test
  * @param  [in] start_address - RAM start address
  * @param  [in] len - test length (range) in bytes
  * @param  [inout] ram_st - RAM test result structure
  * @retval result - 0 - error; 1 - test ok
  */
uint8_t Test_RAM_fast(uint32_t start_address, uint32_t len, SRam_Status* ram_st);

/**
  * @brief  print RAM test result
  * @param  [in] ram_st - RAM test result structure
  * @retval none
  */
void Test_RAM_printResult(SRam_Status* ram_st);

/**
  * @}
  */

/**
  * @}
  */

#endif
