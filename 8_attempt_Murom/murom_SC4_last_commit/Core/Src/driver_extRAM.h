/**
  ******************************************************************************
  * @file    driver_extRAM.h
  * @author  dmib@@ETS
  * @brief   This file contains external SRAM driver module header
  ******************************************************************************
  */
#ifndef _DRIVER_EXTRAM_
#define _DRIVER_EXTRAM_

#include "stdint.h"

/** @defgroup driver_extRAM External SRAM driver module
  * @brief external SRAM driver implementation
  * @{
  */

/// address of external SRAM
#define EXT_SRAM_ADR  0x60000000
/// external SRAM size
#define EXT_SRAM_SIZE 0x100000

/**
  * @brief  external SRAM driver initialization
  * @retval result - SRAM status (0 - driver initializalized; -1 - SRAM initialization error)
  */
int8_t extram_init();

/**
  * @brief  get free SRAM size
  * @retval result - size of free SRAM
  */
uint32_t extram_get_freeRAM();

/**
  * @brief  allocate static block in SRAM
  * @param  [in] size - size of block
  * @retval result - pointer to allocated block (NULL - not enough free SRAM)
  */
void* extram_allocate_static(uint32_t size);

/**
  * @}
  */

#endif
