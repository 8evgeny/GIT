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

#include "cmd_response.h"
#include "main_logic.h"
#include "global_vars.h"
#include "uart.h"
#include "eeprom_i2c.h"

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
#define CMD_LINE_CONTROL			0x04 // команда "Управление линиями" (подключаем/отключаем усилители от линии)

#define CMD_ANS_MASK        0x80 // маска (по или) на код команды в ответе

typedef enum {ST_REC_START0 = 0, ST_REC_START1, ST_REC_CMD, ST_REC_LEN, ST_REC_DATA} tRecStep;

static tRecStep step = ST_REC_START0;
static uint16_t data_bcount = 0;

unsigned int byte_timeout = BYTE_TIMEOUT; // таймаут прихода байта
unsigned char flag_byte_timeout = 0; // флаг таймаут прихода байта

uint32_t resp_timeout; // мс, таймаут прихода ответа на команду
uint32_t resp_timeout_tmr; // счетчик таймаута прихода ответа на команду
unsigned char flag_resp_timeout = 0; // флаг вышел таймаут ожидания ответа на команду
uint8_t estimated_cmd, estimated_data_len; // ожидаемые команда в ответе и длина данных

unsigned char flag_data_ready = 0;
uint32_t cmd, data_len;

uint8_t resp_data[MAX_DATA_LEN]; // используется для приема/передачи данных, которые следуют сразу за длиной пакета

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
			flag_byte_timeout = 1;
		}
  }
  if (resp_timeout_tmr < resp_timeout) {
    resp_timeout_tmr++;
    if (resp_timeout_tmr == resp_timeout) {
      flag_resp_timeout = 1;
    }
  }
}

void chargeCmdTimeout(void) {
	flag_byte_timeout = 0;
	byte_timeout = 0;
}

void stopCmdTimeout(void) {
	byte_timeout = BYTE_TIMEOUT;
	flag_byte_timeout = 0;
}

// Послать команду на плату MK
// cmd - команда
// data - данные
// размер data
void cmdSend(uint8_t cmd, uint8_t *data, uint8_t size) {
  static uint16_t i;

  uartPutchar(START_BYTE0);
  uartPutchar(START_BYTE1);

  uartPutchar(cmd);

  uartPutchar(size);

  for (i = 0; i < size; i++) {
    uartPutchar(data[i]);
  }
}
// посылает команду "проверка связи"
void cmdCheckComm(void) {
  cmdSend(CMD_CHECK_COMM, NULL, 0);
  startWaitResponse(CMD_CHECK_COMM, 2, 200);
  puts("cmdSendCheckComm\r");
}

// посылает команду "Измерить параметры линии" с номером line, на частоте с номером freq
void cmdMeasLine(uint8_t line, uint8_t freq) {
  uint8_t cmd_data[2];
  
  cmd_data[0] = line;
  cmd_data[1] = freq;
  
  cmdSend(CMD_MEAS_LINE, cmd_data, 2);
  startWaitResponse(CMD_MEAS_LINE, 8, 2000);
  printf("cmdMeasLine %u, freq = %u\r", (unsigned int)line, (unsigned int)freq);
}

// посылает команду "Управление выходами"
void cmdWriteOutput(uint8_t *output_arr) {
  int32_t i;
  
  cmdSend(CMD_WRITE_OUTPUT, output_arr, NUM_LINES);
  startWaitResponse(CMD_WRITE_OUTPUT, 0, 100);
  printf("cmdWriteOutput:");
  
  for(i = 0; i < NUM_LINES; i++) {
    printf(" E%i = ", i+1);
    putchar(output_arr[i] + '0');
  }
  putchar('\r');
}

// посылает команду "Управление линиями"
void cmdLineControl(uint8_t *output_arr) {
  int32_t i;
  
  cmdSend(CMD_LINE_CONTROL, output_arr, NUM_LINES);
  startWaitResponse(CMD_LINE_CONTROL, 0, 100);
  printf("cmdLineControl:");
  
  for(i = 0; i < NUM_LINES; i++) {
    printf(" AMP%i = ", i+1);
    putchar(output_arr[i] + '0');
  }
  putchar('\r');
}

// запускаем ожидание ответа на команду в течение времени timeout
// cmd - на какую команду ждем ответа
// len - ожидаемая длина  данных в ответе
void startWaitResponse(uint8_t cmd, uint8_t len, uint32_t timeout) {// TODO: допилить проверку на ожидаемую команду и длину, для cmdMeasLine похоже неверно задана длина ответного пакета (и впротоколе тоже)
  resp_timeout_tmr = 0;
  flag_resp_timeout = 0;
  resp_timeout = timeout;
}

// функция приема ответа на команду
// вызывается в цикле, пока не вернет ненулевое значение
// возвращает:
// 0 - ответ еще не принят
// 1 - ответ успешно принят
// 2 - вышел таймаут
uint8_t doReceiveResponse(void) {
	unsigned char rec_byte;
	static uint8_t rec_cmd; // тип пакета, команда

	if (flag_byte_timeout) { // вышел таймаут приема байта
		flag_byte_timeout = 0;
		cmdRecReset();
	}

  if (flag_resp_timeout) { // вышел таймаут ожидания ответа
    flag_resp_timeout = 0;
    cmdRecReset();
    err_no_conn_with_MK_board = 1;
    return 2;
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
				resp_data[data_bcount] = rec_byte;
				data_bcount++;
				if (data_bcount == data_len) {
          flag_data_ready = 1;
          cmdRecReset(); // принимаем следующий пакет
        }
				break;
		}
  }
  if (flag_data_ready) { // выполнение пришедшей команды

    flag_data_ready = 0;

    err_no_conn_with_MK_board = 0;
    return 1; // успешно приняли ответ
  }
  return 0;
}
