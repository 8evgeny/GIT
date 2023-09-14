/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   uart.h
 * Author: roman.ganaza
 *
 * Created on September 14, 2020, 12:36 PM
 */

#ifndef UART_H
#define UART_H

#include "main.h"
#include <stdio.h>
#include <string.h>

void uartInit(void);
void uartReceiveStart(void);
void uartReceiveStop(void);
void uartRxFifoFlush(void);
void uartPutchar(char c);
void uartPuts(char* str);
void uartPutsN(unsigned char* data, unsigned short n);
unsigned short uartBytesReceived(void);
char uartGetchar(void);
void uartPutsBitArray(unsigned char* mem, unsigned short n);
void uartISR(void);
void doRxBlink(void);

#endif /* UART_H */
