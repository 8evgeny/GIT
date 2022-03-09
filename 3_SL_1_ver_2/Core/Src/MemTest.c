/**
  ******************************************************************************
  * @file    MemTest.c
  * @author  dmib@@ETS
  * @brief   This file contains RAM tests implementation
  ******************************************************************************
  */
#include "stm32h7xx_hal.h"
#include "stdio.h"
#include "CRC32.h"
#include "CLI_io.h"
#include "MemTest.h"

/** @addtogroup MEMTEST_module
  * @{
  */
/** @defgroup MEMTEST_STRR RAM test strings
  * @brief RAM test strings
  * @{
  */
#define MEM_ERROR_MSG_BYTEIO		"Test SRAM : Error byteio A %.8lX , W %.8lX, R %.8lX\r\n"
#define MEM_ERROR_MSG_DATAR1		"Test SRAM : Error data run1 A %.8lX , W %.8lX, R %.8lX\r\n"
#define MEM_ERROR_MSG_DATAR0		"Test SRAM : Error data run0 A %.8lX , W %.8lX, R %.8lX\r\n"
#define MEM_ERROR_MSG_ADDRESS_R1	"Test SRAM : Error address run1 A %.8lX , W %.8lX, R %.8lX\r\n"
#define MEM_ERROR_MSG_ADDRESS_R0	"Test SRAM : Error address run0 A %.8lX , W %.8lX, R %.8lX\r\n"
#define MEM_ERROR_MSG_ADDRESS_HASH	"Test SRAM : Error address hash A %.8lX , W %.8lX, R %.8lX\r\n"
#define MEM_ERROR_MSG_ADDRESS_CNT	"Test SRAM : Error address cnt A %.8lX , W %.8lX, R %.8lX\r\n"
#define MEM_ERROR_MSG_ADDRESS_PTR	"Test SRAM : Error address ptr A %.8lX , W %.8lX, R %.8lX\r\n"
/**
  * @}
  */
/**
  * @}
  */

/**
  * @brief  print RAM test result
  * @param  [in] ram_st - RAM test result structure
  * @retval none
  */
void Test_RAM_printResult(SRam_Status* ram_st)
{
	if (ram_st->error_flags & TESTMEM_ERROR_BYTE)
		CLI_print(MEM_ERROR_MSG_BYTEIO,ram_st->error_address,ram_st->error_wdata,ram_st->error_rdata);
	else
	if (ram_st->error_flags & TESTMEM_ERROR_DATA_R1)
		CLI_print(MEM_ERROR_MSG_DATAR1,ram_st->error_address,ram_st->error_wdata,ram_st->error_rdata);
	else
	if (ram_st->error_flags & TESTMEM_ERROR_DATA_R0)
		CLI_print(MEM_ERROR_MSG_DATAR0,ram_st->error_address,ram_st->error_wdata,ram_st->error_rdata);
	else
	if (ram_st->error_flags & TESTMEM_ERROR_ADDRESS_R1)
		CLI_print(MEM_ERROR_MSG_ADDRESS_R1,ram_st->error_address,ram_st->error_wdata,ram_st->error_rdata);
	else
	if (ram_st->error_flags & TESTMEM_ERROR_ADDRESS_R0)
		CLI_print(MEM_ERROR_MSG_ADDRESS_R0,ram_st->error_address,ram_st->error_wdata,ram_st->error_rdata);
	else
	if (ram_st->error_flags & TESTMEM_ERROR_ADDRESS_HASH)
		CLI_print(MEM_ERROR_MSG_ADDRESS_HASH,ram_st->error_address,ram_st->error_wdata,ram_st->error_rdata);
	else
	if (ram_st->error_flags & TESTMEM_ERROR_ADDRESS_CNT)
		CLI_print(MEM_ERROR_MSG_ADDRESS_CNT,ram_st->error_address,ram_st->error_wdata,ram_st->error_rdata);
	else
	if (ram_st->error_flags & TESTMEM_ERROR_ADDRESS_PTR)
		CLI_print(MEM_ERROR_MSG_ADDRESS_PTR,ram_st->error_address,ram_st->error_wdata,ram_st->error_rdata);
	else
		CLI_print("Test SRAM : OK\r\n");
}

/**
  * @brief  fast RAM test
  * @param  [in] start_address - RAM start address
  * @param  [in] len - test length (range) in bytes
  * @param  [inout] ram_st - RAM test result structure
  * @retval result - 0 - error; 1 - test ok
  */
uint8_t Test_RAM_fast(uint32_t start_address, uint32_t len, SRam_Status* ram_st){
	uint8_t *ptr8;
	__IO uint8_t tmp8, r_data8;
	__IO uint32_t ptr32, tmp32, r_data32, addrofs;
	uint32_t i;

	ram_st->error_flags=0;

	// run 1 at start_address
	ptr32 = start_address;
	tmp32 = 1;
	for (i = 0; i < 32; i++){
	 	*((uint32_t *)ptr32) = tmp32;
	 	__DSB();
	 	r_data32 = *((uint32_t *)ptr32);
	 	if (r_data32 != tmp32){
	 		ram_st->error_address = (uint32_t)ptr32;
	 		ram_st->error_wdata = tmp32;
	 		ram_st->error_rdata = r_data32;
	 		ram_st->error_flags = ram_st->error_flags | TESTMEM_ERROR_DATA_R1;
	 		return 0;
	 	}
		tmp32 = tmp32 << 1;
	}
	// run 0 at start_address
	tmp32 = 0xFFFFFFFE;
	for (i = 0; i < 32; i++){
	 	*((uint32_t *)ptr32) = tmp32;
	 	__DSB();
	 	r_data32 = *((uint32_t *)ptr32);
	 	if (r_data32 != tmp32){
	 		ram_st->error_address = (uint32_t)ptr32;
	 		ram_st->error_wdata = tmp32;
	 		ram_st->error_rdata = r_data32;
	 		ram_st->error_flags = ram_st->error_flags | TESTMEM_ERROR_DATA_R0;
	 		return 0;
	 	}
		tmp32 = (tmp32 << 1) | 1;
	}

	// write 01 02 03 04, check 01 02 03 04
	ptr8 = (uint8_t*)start_address;
	tmp8 = 0x01;
	for (i = 0; i < 4; i++){
	 	*ptr8 = tmp8;
		ptr8++;
		tmp8++;
	}
	__DSB();
	ptr8 = (uint8_t*)start_address;
	tmp8 = 0x01;
	for (i = 0; i < 4; i++){
		  r_data8 = *ptr8;
		  if (r_data8 != tmp8){
			ram_st->error_address = (uint32_t)ptr8;
			ram_st->error_wdata = tmp8;
			ram_st->error_rdata = r_data8;
            ram_st->error_flags = ram_st->error_flags | TESTMEM_ERROR_BYTE;
            return 0;
		}
		ptr8++;
		tmp8++;
	}

	// write 16 bit counter at (start_address + run 1), then check
	ptr32 = start_address;
	tmp32 = 0x00020002;
	addrofs = 4;
	while (addrofs < len){
	 	*((uint32_t *)ptr32) = tmp32;
		ptr32 = start_address + addrofs;
		tmp32 += 0x00010001;
		addrofs <<= 1;
	}
	__DSB();
	ptr32 = start_address;
	tmp32 = 0x00020002;
	addrofs = 4;
	while (addrofs < len){
		  r_data32 = *((uint32_t *)ptr32);
		  if (r_data32 != tmp32){
			ram_st->error_address = (uint32_t)ptr32;
			ram_st->error_wdata = tmp32;
			ram_st->error_rdata = r_data32;
            ram_st->error_flags = ram_st->error_flags | TESTMEM_ERROR_ADDRESS_R1;
            return 0;
		}
		ptr32 = start_address + addrofs;
		tmp32 += 0x00010001;
		addrofs <<= 1;
	}

	// write 16 bit counter at (start_address + run 0), then check
	ptr32 = start_address;
	tmp32 = 0x00020002;
	addrofs = 4;
	while (addrofs < len){
	 	*((uint32_t *)ptr32) = tmp32;
		ptr32 = start_address + ((~addrofs)&(len-1)&(~3));
		tmp32 += 0x00010001;
		addrofs <<= 1;
	}
	__DSB();
	ptr32 = start_address;
	tmp32 = 0x00020002;
	addrofs = 4;
	while (addrofs < len){
		  r_data32 = *((uint32_t *)ptr32);
		  if (r_data32 != tmp32){
			ram_st->error_address = (uint32_t)ptr32;
			ram_st->error_wdata = tmp32;
			ram_st->error_rdata = r_data32;
            ram_st->error_flags = ram_st->error_flags | TESTMEM_ERROR_ADDRESS_R0;
            return 0;
		}
		ptr32 = start_address + ((~addrofs)&(len-1)&(~3));
		tmp32 += 0x00010001;
		addrofs <<= 1;
	}
	
	return 1;
}

/**
  * @brief  byte IO RAM test
  * @param  [in] start_address - RAM start address
  * @param  [in] len - test length (range) in bytes
  * @param  [inout] ram_st - RAM test result structure
  * @retval result - 0 - error; 1 - test ok
  */
uint8_t TestRAM_byteio(uint32_t start_address, uint32_t len, SRam_Status* ram_st){
	uint8_t *ptr8;
	uint8_t tmp8, r_data;
	uint32_t i;
	
	ptr8 = (uint8_t*)start_address;
	tmp8 = 0;	
	for (i = 0; i < len; i++){
	 	*ptr8 = tmp8;
		ptr8++;
		tmp8++;
	}	
	__DSB();
	ptr8 = (uint8_t*)start_address;
	tmp8 = 0;	
	for (i = 0; i < len; i++){
		  r_data = *ptr8;
		  if (r_data != tmp8){	
			ram_st->error_address = (uint32_t)ptr8;
			ram_st->error_wdata = tmp8;
			ram_st->error_rdata = r_data;
            ram_st->error_flags = ram_st->error_flags | TESTMEM_ERROR_BYTE;				
			break;
		}
		ptr8++;
		tmp8++;
	}
	
	return ram_st->error_flags;
}

/**
  * @brief  data running 0 full RAM test
  * @param  [in] start_address - RAM start address
  * @param  [in] len - test length (range) in bytes
  * @param  [inout] ram_st - RAM test result structure
  * @retval result - 0 - error; 1 - test ok
  */
uint8_t TestRAM_runzeros(uint32_t start_address, uint32_t len, SRam_Status* ram_st){
	uint32_t *ptr32;
	uint32_t tmp32,r_data;
	uint32_t i;

	len = len >> 2;

	ptr32 = (uint32_t*)start_address;
	tmp32 = 1;	
	for (i = 0; i < len; i++){
	 	*ptr32++ = ~tmp32;
		tmp32 = tmp32<<1;
		if (!tmp32) tmp32=1;
	}	
	__DSB();
	ptr32 = (uint32_t*)start_address;
	tmp32 = 1;	
	for (i = 0; i < len; i++){
		r_data = *ptr32;
		if (r_data != ~tmp32){	
			ram_st->error_address = (uint32_t)ptr32;
			ram_st->error_wdata = ~tmp32;
			ram_st->error_rdata = r_data;
			ram_st->error_flags = ram_st->error_flags | TESTMEM_ERROR_DATA_R0;
			break;
		}
		ptr32++;
		tmp32 = tmp32<<1;
		if (!tmp32) tmp32=1;
	}

	return ram_st->error_flags;	
}

/**
  * @brief  data running 1 full RAM test
  * @param  [in] start_address - RAM start address
  * @param  [in] len - test length (range) in bytes
  * @param  [inout] ram_st - RAM test result structure
  * @retval result - 0 - error; 1 - test ok
  */
uint8_t TestRAM_runones(uint32_t start_address, uint32_t len, SRam_Status* ram_st){
	uint32_t *ptr32;
	uint32_t tmp32,r_data;
	uint32_t i;

	len = len >> 2;
	
	ptr32 = (uint32_t*)start_address;
	tmp32 = 1;	
	for (i = 0; i < len; i++){
	 	*ptr32++ = tmp32;
		tmp32 = tmp32<<1;
		if (!tmp32) tmp32=1;
	}	
	__DSB();
	ptr32 = (uint32_t*)start_address;
	tmp32 = 1;	
	for (i = 0; i < len; i++){
		r_data = *ptr32;
		if (r_data != tmp32){	
			ram_st->error_address = (uint32_t)ptr32;
			ram_st->error_wdata = tmp32;
			ram_st->error_rdata = r_data;
			ram_st->error_flags = ram_st->error_flags | TESTMEM_ERROR_DATA_R1;
			break;
		}
		ptr32++;
		tmp32 = tmp32<<1;
		if (!tmp32) tmp32=1;
	}
	
	return ram_st->error_flags;	
}

/**
  * @brief  address ptr full RAM test
  * @param  [in] start_address - RAM start address
  * @param  [in] len - test length (range) in bytes
  * @param  [inout] ram_st - RAM test result structure
  * @retval result - 0 - error; 1 - test ok
  */
uint8_t TestRAM_address_ptr(uint32_t start_address, uint32_t len, SRam_Status* ram_st){
	uint32_t *ptr32;
	uint32_t tmp32,r_data;
	uint32_t i;

	len = len >> 2;

	ptr32=(uint32_t*)start_address;	
	for (i = 0; i < len; i++){
		*ptr32 = (uint32_t)ptr32; 
		ptr32++;
	}
	__DSB();
	ptr32 = (uint32_t*)start_address;
	for (i = 0; i < len; i++){
		tmp32 = (uint32_t)ptr32;
		r_data = *ptr32;
		if(tmp32 != r_data){  
			ram_st->error_address = (uint32_t)ptr32;
			ram_st->error_wdata = tmp32;
			ram_st->error_rdata = r_data;
			ram_st->error_flags = ram_st->error_flags | TESTMEM_ERROR_ADDRESS_PTR;
			break;
		}
		ptr32++;
	}

	return ram_st->error_flags;
}

/**
  * @brief  address counter full RAM test
  * @param  [in] start_address - RAM start address
  * @param  [in] len - test length (range) in bytes
  * @param  [inout] ram_st - RAM test result structure
  * @retval result - 0 - error; 1 - test ok
  */
uint8_t TestRAM_address_cnt(uint32_t start_address, uint32_t len, SRam_Status* ram_st){
	uint32_t *ptr32;
	uint32_t tmp32,r_data;
	uint32_t i;

	len = len >> 2;

	ptr32=(uint32_t*)start_address;
	tmp32 = 0;
	for (i = 0; i < len; i++){
		*ptr32 = tmp32;
		ptr32++;
		tmp32 += 0x00040004;
	}
	__DSB();
	ptr32 = (uint32_t*)start_address;
	tmp32 = 0;
	for (i = 0; i < len; i++){
		r_data = *ptr32;
		if(tmp32 != r_data){
			ram_st->error_address = (uint32_t)ptr32;
			ram_st->error_wdata = tmp32;
			ram_st->error_rdata = r_data;
			ram_st->error_flags = ram_st->error_flags | TESTMEM_ERROR_ADDRESS_CNT;
			break;
		}
		ptr32++;
		tmp32 += 0x00040004;
	}

	return ram_st->error_flags;
}

/**
  * @brief  address ptr hash full RAM test
  * @param  [in] start_address - RAM start address
  * @param  [in] len - test length (range) in bytes
  * @param  [inout] ram_st - RAM test result structure
  * @retval result - 0 - error; 1 - test ok
  */
uint8_t TestRAM_hashaddress(uint32_t start_address, uint32_t len, SRam_Status* ram_st){
	uint32_t *ptr32;
	uint32_t tmp32,r_data;
	uint32_t i;

	len = len >> 2;

	ptr32=(uint32_t*)start_address;	
	for (i = 0; i < len; i++){
		*ptr32 = crc32NT(0,(uint8_t *)&ptr32,4); 
		ptr32++;
	}
	__DSB();
	ptr32 = (uint32_t*)start_address;
	for (i = 0; i < len; i++){
		tmp32 = crc32NT(0,(uint8_t *)&ptr32,4);
		r_data = *ptr32;
		if(tmp32 != r_data){  
			ram_st->error_address = (uint32_t)ptr32;
			ram_st->error_wdata = tmp32;
			ram_st->error_rdata = r_data;
			ram_st->error_flags = ram_st->error_flags | TESTMEM_ERROR_ADDRESS_HASH;
			break;
		}
		ptr32++;
	}

	return ram_st->error_flags;
}

/**
  * @brief  full RAM tests
  * @param  [in] start_address - RAM start address
  * @param  [in] len - test length (range) in bytes
  * @param  [inout] ram_st - RAM test result structure
  * @param  [in] printout - print result to CLI (0 - no/1 - yes)
  * @retval error_flags - test RAM error flags
  */
uint8_t Test_RAM(uint32_t start_address, uint32_t len, SRam_Status* ram_st, uint8_t printout){

	ram_st->error_flags=0;

    TestRAM_byteio(start_address, len, ram_st);
    if (printout) {
     if (ram_st->error_flags & TESTMEM_ERROR_BYTE)
 		CLI_print(MEM_ERROR_MSG_BYTEIO,ram_st->error_address,ram_st->error_wdata,ram_st->error_rdata);
     else
      CLI_print("Test SRAM : byteio OK\r\n");
    }

    TestRAM_runones(start_address, len, ram_st);
    if (printout) {
	if (ram_st->error_flags & TESTMEM_ERROR_DATA_R0)
	  CLI_print(MEM_ERROR_MSG_DATAR0,ram_st->error_address,ram_st->error_wdata,ram_st->error_rdata);
     else
      CLI_print("Test SRAM : data ones OK\r\n");
    }

    TestRAM_runzeros(start_address, len, ram_st);
    if (printout) {
	if (ram_st->error_flags & TESTMEM_ERROR_DATA_R1)
	  CLI_print(MEM_ERROR_MSG_DATAR1,ram_st->error_address,ram_st->error_wdata,ram_st->error_rdata);
     else
      CLI_print("Test SRAM : data zeros OK\r\n");
    }

    TestRAM_hashaddress(start_address, len, ram_st);
    if (ram_st->error_flags & TESTMEM_ERROR_ADDRESS_HASH) {
    	if (printout) CLI_print(MEM_ERROR_MSG_ADDRESS_HASH,ram_st->error_address,ram_st->error_wdata,ram_st->error_rdata);
    	TestRAM_address_ptr(start_address, len, ram_st);
    	if (printout) {
         if (ram_st->error_flags & TESTMEM_ERROR_ADDRESS_PTR)
  	      CLI_print(MEM_ERROR_MSG_ADDRESS_PTR,ram_st->error_address,ram_st->error_wdata,ram_st->error_rdata);
         else
          CLI_print("Test SRAM : address ptr OK\r\n");
    	}
    	TestRAM_address_cnt(start_address, len, ram_st);
    	if (printout) {
  	     if (ram_st->error_flags)
  	      CLI_print(MEM_ERROR_MSG_ADDRESS_CNT,ram_st->error_address,ram_st->error_wdata,ram_st->error_rdata);
         else
          CLI_print("Test SRAM : address cnt OK\r\n");
    	}
    } else {
    	if (printout) CLI_print("Test SRAM : address hash OK\r\n");
    }

    return ram_st->error_flags;	
}
