#ifndef FLAGS_H
#define FLAGS_H

#include <stdint.h>

#define FIRMWARE_VERSION_H      0x01
#define FIRMWARE_VERSION_L      0x00

#define NUM_LINES               6

#define GND_LEAKAGE_TRESHOLD            730 // ���� ����� �������� ��� �������, ��� ������ �� ����� ����

extern char str[]; // ������ ��� ����������� ������
extern uint8_t line_en[NUM_LINES]; // ���������� ��������� � ����� (1) ��� �� ���������� (0)

// ��� ������������� ��������� ����������� � EEPROM
// ������� ������ ������ ������ � ��������!
// ������� �������� ����������� ���� ��� extern, ����� �� ����� �������� sizeof
//--------------------------------------------------------------------- EEPROM Vars

//--------------------------------------------------------------------- END EEPROM Vars

// ����� � EEPROM �� �������:

#define EEP_ADDR_TEST_BYTE                      255

#define TEST_BYTE           0xAB

void eepLoadVars(void);
void eepWriteDefaults(void);

#endif
