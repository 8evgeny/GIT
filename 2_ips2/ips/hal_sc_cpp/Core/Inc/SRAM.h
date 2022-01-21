/*
 * SRAM.h
 *
 *  Created on: Jun 22, 2020
 *      Author: Donskikh Dmitry
 */

#ifndef INC_SRAM_H_
#define INC_SRAM_H_

#include "inttypes.h"

//#ifndef EXT_SRAM_SIZE
#define	EXT_SRAM_SIZE	1024*1024
#define EXT_SRAM_PTR	(uint8_t*)0x60000000
//#endif

uint32_t getFreeExtSRAM (void);
void* allocateExtSramStatic (uint32_t size);

#endif /* INC_SRAM_H_ */
