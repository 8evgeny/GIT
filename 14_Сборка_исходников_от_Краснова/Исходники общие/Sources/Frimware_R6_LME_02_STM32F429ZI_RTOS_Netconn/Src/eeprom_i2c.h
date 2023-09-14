#ifndef __EEPROM_I2C_H
#define __EEPROM_I2C_H

#include <stdint.h>
#include "main.h"

// Предопределения ------------------------------------------------------------
#define EEPROM_ADDR	0xA0
#define EEPROM24C64_MORE	// двухбайтовый адрес 24c64 и больше иначе однобайтовый

#ifdef EEPROM24C64_MORE
    #define ADDR_SIZE   2
#else 
    #define ADDR_SIZE   1
#endif

#define EEP_PAGE_SIZE       64

extern uint8_t flag_eeprom_error;
extern uint8_t flag_no_debug;

uint32_t eepInit(void);
void eepWriteSeq(uint32_t address, unsigned char * data, uint16_t size);
void eepReadSeq(uint32_t address, unsigned char * data, uint16_t size);

#endif /* __EEPROM_I2C_H */
