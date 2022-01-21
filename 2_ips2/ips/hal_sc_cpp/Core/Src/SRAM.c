/*
 * SRAM.c
 *
 *  Created on: Jun 22, 2020
 *      Author: Donskikh Dmitry
 */
#include "main.h"
#include "SRAM.h"
#include "string.h"

extern SRAM_HandleTypeDef hsram1;

static uint8_t *esram_begin_ptr = 0x60000000U;
static uint8_t *sram_pos_ptr = 0x60000000U;

uint32_t getFreeExtSRAM ()
{
  return (EXT_SRAM_SIZE - (sram_pos_ptr - esram_begin_ptr));
}

void* allocateExtSramStatic (uint32_t size)
{
  if (HAL_SRAM_GetState (&hsram1) == HAL_SRAM_STATE_READY)
    {
      if (getFreeExtSRAM () < size)
	{
	  return NULL;
	} else
	{
	  uint8_t *l_buffer = sram_pos_ptr;
	  sram_pos_ptr += size + size % 4;
	  return memset (l_buffer, 0, size);
	}
    } else
    {
      return NULL;
    }

}
