/**
  ******************************************************************************
  * @file    driver_extRAM.c
  * @author  dmib@@ETS
  * @brief   This file contains external SRAM driver implementation
  ******************************************************************************
  */
#include "string.h"
#include "MemTest.h"
#include "CLI_io.h"
#include "driver_extRAM.h"

/**
  * @brief end address of SRAM used by startup loader (from *.s file)
  */

uint32_t _ext_ram_ld_end;
extern uint32_t _ext_ram_ld_end;

/**
  * @brief current pointer of free SRAM
  */
uint8_t *extram_pos_ptr;

/**
  * @brief current size of free SRAM
  */
uint32_t extram_full_size;

uint32_t extram_get_freeRAM ()
{
  return (extram_full_size - ((uint32_t)extram_pos_ptr - _ext_ram_ld_end));
}

void* extram_allocate_static(uint32_t size)
{
    if (extram_get_freeRAM () < size)
	{
	  return 0;
	} else
	{
	  uint8_t *l_buffer = extram_pos_ptr;
	  extram_pos_ptr += size & (~0x3);
	  if (size & 0x03) extram_pos_ptr++;
	  return memset (l_buffer, 0, size);
	}
}

int8_t extram_init()
{
	SRam_Status ram_st;

	extram_pos_ptr = (uint8_t *)_ext_ram_ld_end;

	Test_RAM_fast(EXT_SRAM_ADR, EXT_SRAM_SIZE, &ram_st);
	Test_RAM_printResult(&ram_st);

	if (!ram_st.error_flags) {
		extram_full_size = EXT_SRAM_SIZE - _ext_ram_ld_end;
		return 0;
	} else {
		extram_full_size = 0;
		return -1;
	}
}
