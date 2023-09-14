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

// Формат: START0(1b), START1(1b) команда CMD(1b), длина данных LEN(1b), DATA (LEN b)
#define MAX_DATA_LEN        48 // байт, максимальная длина данных (поля DATA)
#define MAX_ANSWER_LEN      (MAX_DATA_LEN + 4) // байт, максимальная длина ответа на команду (всего пакета)

#define BYTE_TIMEOUT				100 // мс - максимальное время между байтами одной команды, если превышено, то сбрасываем прием команды (20 мало, 50 уже нормально)

#define START_BYTE0					0xAB // первый фиксированный байт пакета
#define START_BYTE1					0xCD // второй фиксированный байт пакета

// коды команд
#define CMD_CHECK_COMM        0x00 // команда "Проверка связи"
#define CMD_MEAS_LINE         0x01 // команда "Измерить параметры линии"
#define CMD_READ_INPUT				0x02 // команда "Чтение входов"
#define CMD_WRITE_OUTPUT			0x03 // команда "Управление выходами"
#define CMD_LINE_CONTROL			0x04 // команда "Управление линиями"

#define CMD_ANS_MASK        0x80 // маска (по или) на код команды в ответе

typedef enum {ST_REC_START0 = 0, ST_REC_START1, ST_REC_CMD, ST_REC_LEN, ST_REC_DATA} tRecStep;

static tRecStep step = ST_REC_START0;
static uint16_t data_bcount = 0;

unsigned int byte_timeout = BYTE_TIMEOUT; // таймаут прихода байта
unsigned char flag_timeout = 0;
unsigned char flag_data_ready = 0;
uint32_t cmd, data_len, mem_addr, mem_len;

static uint8_t data[MAX_DATA_LEN]; // используется для приема/передачи данных, которые следуют сразу за длиной пакета
static uint8_t ans[MAX_ANSWER_LEN]; // используется для передачи ответа

void stopCmdTimeout(void);

uint32_t parseInt32(unsigned char * buf) {
	return *((uint32_t*)buf); // из массива 4-х байт делаем uint32_t
}

void cmdRecReset(void) { // возвращает механизм приема команды в начальное состояние
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

// ответ на команду
// cmd - команда, на которую отвечаем
// data - данные
// размер data

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
	static uint8_t rec_cmd; // тип пакета, команда

	if (flag_timeout) { // вышел таймаут
		flag_timeout = 0;
		cmdRecReset();
		//dbgUartPuts("Cmd timeout!\r");
	}

	if (uartBytesReceived()) {
		rec_byte = uartGetchar();
		
		chargeCmdTimeout(); // заряжаем таймаут

		switch (step) {
			case ST_REC_START0:
				if (rec_byte == START_BYTE0) {
					step = ST_REC_START1;
				}
				break;
			case ST_REC_START1:
				if (rec_byte == START_BYTE1) {
					step = ST_REC_CMD;
				} else { // неверный байт
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
          cmdRecReset(); // принимаем следующий пакет
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
          cmdRecReset(); // принимаем следующий пакет
        }
				break;
		}
  }
  if (flag_data_ready) { // выполнение пришедшей команды
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
        HAL_GPIO_WritePin(TST_GPIO_Port, TST_Pin, (GPIO_PinState) 1); // пин "измерение в процессе" вкл
        leakage = measLeakage(data[0]);
        *((uint16_t*)&ans[5]) = leakage;
        if (leakage < GND_LEAKAGE_TRESHOLD) {
          *((float*) &ans[1]) = -1;
        } else {
          *((float*)&ans[1]) = measImpedance(data[0], data[1]);
        }
        HAL_GPIO_WritePin(TST_GPIO_Port, TST_Pin, (GPIO_PinState) 0); // пин "измерение в процессе" выкл
        uartRxFifoFlush(); // не выполняем команды, пришедшие во время измерения, а то их там много может набиться
        cmdSendAnswer(rec_cmd, ans, 7);
        
        //sprintf(str, "\r\rLine %u: impedance = %6.2f; leakage = %u units\r\r", data[0], *((float*)&ans[1]), *((uint16_t*)&ans[5]));
//        sprintf(str, "\r\rLine %u: Z = %6.2f; leak = %u units\r\r", data[0], *((float*)&ans[1]), *((uint16_t*)&ans[5]));
//        uartPuts(str);
        break;
      case CMD_READ_INPUT: // от этой команды отказались, вместо этого прокинули провод

        break;
      case CMD_WRITE_OUTPUT:
        writeOutputs(data);
        cmdSendAnswer(rec_cmd, ans, 0);
        break;
      case CMD_LINE_CONTROL:
        controlLines(data);
        cmdSendAnswer(rec_cmd, ans, 0);
        break;
      default: // неизвестная команда
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
