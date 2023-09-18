#ifndef __EEPROM_I2C_H
#define __EEPROM_I2C_H

#include <stdint.h>
#include "main.h"

// Предопределения ------------------------------------------------------------
#define EEPROM_ADDR	0xA0
//#define EEPROM24C64_MORE	// двухбайтовый адрес 24c64 и больше

#ifdef EEPROM24C64_MORE
    #define ADDR_SIZE   2
#else 
    #define ADDR_SIZE   1
#endif

uint32_t eepInit(void);
void eepWriteSeq(uint32_t address, unsigned char * data, uint32_t size);
void eepReadSeq(uint32_t address, unsigned char * data, uint32_t size);

#endif /* __EEPROM_I2C_H */
