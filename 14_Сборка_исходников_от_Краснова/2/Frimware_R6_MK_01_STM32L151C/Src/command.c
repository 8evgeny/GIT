/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   command.c
 * Author: roman.ganaza
 * 
 * Created on January 12, 2021, 10:32 AM
 */

#include <string.h>

#include "command.h"
#include "main_logic.h"
#include "global_vars.h"
#include "uart.h"
#include "eeprom_i2c.h"
#include "a_main.h"

// ������: START0(1b), START1(1b) ������� CMD(1b), ����� ������ LEN(1b), DATA (LEN b)
#define MAX_DATA_LEN        48 // ����, ������������ ����� ������ (���� DATA)
#define MAX_ANSWER_LEN      (MAX_DATA_LEN + 4) // ����, ������������ ����� ������ �� ������� (����� ������)

#define BYTE_TIMEOUT				100 // �� - ������������ ����� ����� ������� ����� �������, ���� ���������, �� ���������� ����� ������� (20 ����, 50 ��� ���������)

#define START_BYTE0					0xAB // ������ ������������� ���� ������
#define START_BYTE1					0xCD // ������ ������������� ���� ������

// ���� ������
#define CMD_CHECK_COMM        0x00 // ������� "�������� �����"
#define CMD_MEAS_LINE         0x01 // ������� "�������� ��������� �����"
#define CMD_READ_INPUT				0x02 // ������� "������ ������"
#define CMD_WRITE_OUTPUT			0x03 // ������� "���������� ��������"
#define CMD_LINE_CONTROL			0x04 // ������� "���������� �������"

#define CMD_ANS_MASK        0x80 // ����� (�� ���) �� ��� ������� � ������

typedef enum {ST_REC_START0 = 0, ST_REC_START1, ST_REC_CMD, ST_REC_LEN, ST_REC_DATA} tRecStep;

static tRecStep step = ST_REC_START0;
static uint16_t data_bcount = 0;

unsigned int byte_timeout = BYTE_TIMEOUT; // ������� ������� �����
unsigned char flag_timeout = 0;
unsigned char flag_data_ready = 0;
uint32_t cmd, data_len, mem_addr, mem_len;

static uint8_t data[MAX_DATA_LEN]; // ������������ ��� ������/�������� ������, ������� ������� ����� �� ������ ������
static uint8_t ans[MAX_ANSWER_LEN]; // ������������ ��� �������� ������

void stopCmdTimeout(void);

uint32_t parseInt32(unsigned char * buf) {
	return *((uint32_t*)buf); // �� ������� 4-� ���� ������ uint32_t
}

void cmdRecReset(void) { // ���������� �������� ������ ������� � ��������� ���������
	step = ST_REC_START0;
	data_bcount = 0;
	stopCmdTimeout();
}

void doCmdTimeout(void) {
	if (byte_timeout < BYTE_TIMEOUT) {
		byte_timeout++;
		if (byte_timeout == BYTE_TIMEOUT) {
			flag_timeout = 1;
		}
	}
}

void chargeCmdTimeout(void) {
	flag_timeout = 0;
	byte_timeout = 0;
}

void stopCmdTimeout(void) {
	byte_timeout = BYTE_TIMEOUT;
	flag_timeout = 0;
}

// ����� �� �������
// cmd - �������, �� ������� ��������
// data - ������
// ������ data

void cmdSendAnswer(uint8_t cmd, uint8_t *data, uint8_t size) {
  static uint16_t i;

  uartPutchar(START_BYTE0);
  uartPutchar(START_BYTE1);

  uartPutchar(cmd | CMD_ANS_MASK);

  uartPutchar(size);

  for (i = 0; i < size; i++) {
    uartPutchar(data[i]);
  }
}

void doCommand(void) {
	unsigned char rec_byte;
	static uint8_t rec_cmd; // ��� ������, �������

	if (flag_timeout) { // ����� �������
		flag_timeout = 0;
		cmdRecReset();
		//dbgUartPuts("Cmd timeout!\r");
	}

	if (uartBytesReceived()) {
		rec_byte = uartGetchar();
		
		chargeCmdTimeout(); // �������� �������

		switch (step) {
			case ST_REC_START0:
				if (rec_byte == START_BYTE0) {
					step = ST_REC_START1;
				}
				break;
			case ST_REC_START1:
				if (rec_byte == START_BYTE1) {
					step = ST_REC_CMD;
				} else { // �������� ����
					cmdRecReset();
				}
				break;
			case ST_REC_CMD:
				rec_cmd = rec_byte;
				step = ST_REC_LEN;
				break;
			case ST_REC_LEN:
				data_len = rec_byte;

        if (data_len == 0) {
          flag_data_ready = 1;
          cmdRecReset(); // ��������� ��������� �����
        } else if (data_len <= MAX_DATA_LEN) {
					step = ST_REC_DATA;
				} else {
					//dbgUartPuts("Invalid data lenght\r");
					cmdRecReset();
				}
				break;
			case ST_REC_DATA:
				data[data_bcount] = rec_byte;
				data_bcount++;
				if (data_bcount == data_len) {
          flag_data_ready = 1;
          cmdRecReset(); // ��������� ��������� �����
        }
				break;
		}
  }
  if (flag_data_ready) { // ���������� ��������� �������
    uint16_t leakage; 
    flag_data_ready = 0;

    switch (rec_cmd) {
      case CMD_CHECK_COMM:
        ans[0] = FIRMWARE_VERSION_L;
        ans[1] = FIRMWARE_VERSION_H;
        cmdSendAnswer(rec_cmd, ans, 2);
        break;
      case CMD_MEAS_LINE:
        ans[0] = data[0];
        HAL_GPIO_WritePin(TST_GPIO_Port, TST_Pin, (GPIO_PinState) 1); // ��� "��������� � ��������" ���
        leakage = measLeakage(data[0]);
        *((uint16_t*)&ans[5]) = leakage;
        if (leakage < GND_LEAKAGE_TRESHOLD) {
          *((float*) &ans[1]) = -1;
        } else {
          *((float*)&ans[1]) = measImpedance(data[0], data[1]);
        }
        HAL_GPIO_WritePin(TST_GPIO_Port, TST_Pin, (GPIO_PinState) 0); // ��� "��������� � ��������" ����
        uartRxFifoFlush(); // �� ��������� �������, ��������� �� ����� ���������, � �� �� ��� ����� ����� ��������
        cmdSendAnswer(rec_cmd, ans, 7);
        
        //sprintf(str, "\r\rLine %u: impedance = %6.2f; leakage = %u units\r\r", data[0], *((float*)&ans[1]), *((uint16_t*)&ans[5]));
//        sprintf(str, "\r\rLine %u: Z = %6.2f; leak = %u units\r\r", data[0], *((float*)&ans[1]), *((uint16_t*)&ans[5]));
//        uartPuts(str);
        break;
      case CMD_READ_INPUT: // �� ���� ������� ����������, ������ ����� ��������� ������

        break;
      case CMD_WRITE_OUTPUT:
        writeOutputs(data);
        cmdSendAnswer(rec_cmd, ans, 0);
        break;
      case CMD_LINE_CONTROL:
        controlLines(data);
        cmdSendAnswer(rec_cmd, ans, 0);
        break;
      default: // ����������� �������
        ans[0] = 0xEE;
        ans[1] = 0xEE;
        ans[2] = 0xEE;
        ans[3] = 0xEE;
        ans[4] = 0xEE;
        cmdSendAnswer(0xFF, ans, 5);
        break;
    }
  }
}
