#include "global_vars.h"
#include "eeprom_i2c.h"
#include <string.h>
#include <stdio.h>

char str[64]; // ������ ��� ����������� ������

uint8_t line_en[NUM_LINES] = {1,1,1,1,1,1}; // ���������� ��������� � ����� (1) ��� �� ���������� (0)

// ��� ������������� ��������� ����������� � EEPROM
// ������� ������ ������ ������ � ��������!
//--------------------------------------------------------------------- EEPROM Vars

//--------------------------------------------------------------------- END EEPROM Vars

void eepLoadVars(void) {
 
}

void eepWriteDefaults(void) {
  uint8_t test_byte;

  //dbgUartPuts("eepWriteDefaults\r");

  test_byte = TEST_BYTE;
  eepWriteSeq(EEP_ADDR_TEST_BYTE, (unsigned char*) &test_byte, sizeof (test_byte));
}
