/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   command.h
 * Author: roman.ganaza
 *
 * Created on January 12, 2021, 10:32 AM
 */

#ifndef COMM_H
#define COMM_H

#include <stdint.h>
#include "FIFO.h"

#define FREQ_NUM_80Hz       0 // номер элемента массива для частоты 80 Гц
#define FREQ_NUM_1kHz       1 // номер элемента массива для частоты 1 кГц
#define FREQ_NUM_4kHz       2
#define FREQ_NUM_160Hz      3
#define FREQ_NUM_400Hz      4
#define FREQ_NUM_40Hz       5

extern uint8_t resp_data[]; // используется для приема/передачи данных, которые следуют сразу за длиной пакета

void cmdProcessInputByte(unsigned char b);
void doCmdTimeout(void);
uint8_t doReceiveResponse(void);
void startWaitResponse(uint8_t cmd, uint8_t len, uint32_t timeout);

void cmdCheckComm(void);
void cmdMeasLine(uint8_t line, uint8_t freq);
void cmdWriteOutput(uint8_t *output_arr);
void cmdLineControl(uint8_t *output_arr);

void respPutsN(void * data, unsigned char n);
unsigned short respGetCount(void); // возвращает число оставшихся (неотправленных) байт ответа на команду
unsigned char respGetByte(void); // возвращает очередной байт ответа на команду

#endif /* COMM_H */
