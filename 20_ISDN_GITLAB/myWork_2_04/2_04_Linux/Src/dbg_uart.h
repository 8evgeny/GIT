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

#ifndef DBG_UART_H
#define DBG_UART_H

#include "main.h"
#include <string.h>
#include <stdio.h>

void dbgUartReceiveStart(void);
void dbgUartReceiveStop(void);
unsigned char dbgUartIsTxEmpty(void);
void dbgUartPauseTx(void);
void dbgUartResumeTx(void);
void dbgUartPutchar(char c);
void dbgUartPuts(const char* str);
void dbgUartPutsArray(const char* str, uint16_t len);
void dbgUartPutsUntil(const char* str, char sym);
uint16_t dbgUartGetFreeSpace(void);
void dbgUartPutsBitArray(unsigned char* mem, unsigned short n);
void dbgUartISR(void);
void HexToChar(char *s, unsigned char b);

#endif /* DBG_UART_H */
