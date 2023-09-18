#ifndef FLAGS_H
#define FLAGS_H

#include <stdint.h>

#define FIRMWARE_VERSION_H      0x01
#define FIRMWARE_VERSION_L      0x00

#define NUM_LINES               6

#define GND_LEAKAGE_TRESHOLD            730 // ниже этого значения АЦП считаем, что утечка на землю ЕСТЬ

extern char str[]; // строка для отладочного вывода
extern uint8_t line_en[NUM_LINES]; // подключать усилитель к линии (1) или не подключать (0)

// все нижеследующие параметры дублируются в EEPROM
// размеры менять только вместе с адресами!
// размеры массивов прописывать даже для extern, иначе не будет работать sizeof
//--------------------------------------------------------------------- EEPROM Vars

//--------------------------------------------------------------------- END EEPROM Vars

// лежат в EEPROM по адресам:

#define EEP_ADDR_TEST_BYTE                      255

#define TEST_BYTE           0xAB

void eepLoadVars(void);
void eepWriteDefaults(void);

#endif
