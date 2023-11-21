/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   uart.c
 * Author: roman.ganaza
 * 
 * Created on December 15, 2020
 */

#include "dbg_uart.h"
#include "FIFO.h"
#include <string.h>
#include <stdio.h>

#define USART_IRQ_HANDLER    USART1_IRQHandler // обработчик прерывания
#define uart	USART1
#define _PRINTF_
    
FIFO(4096) dbg_uart_tx_fifo;

unsigned char flag_tx_enabled = 1;


// Just rewrite this function
#ifdef _PRINTF_

int fputc(int ch, FILE *f) {
  dbgUartPutchar(ch);
  return ch;
}

#endif

void dbgUartReceiveStart(void) {
	volatile char dummy;

	dummy = uart->DR; // чистим флаг (для UART)
//	dummy = uart->RDR; // чистим флаг (для USART)
//	FIFO_FLUSH(uart_rx_fifo);
	LL_USART_EnableIT_RXNE(uart);
}

void dbgUartReceiveStop(void) {
	LL_USART_DisableIT_RXNE(uart);
}

unsigned char dbgUartIsTxEmpty(void) {
	return !LL_USART_IsEnabledIT_TXE(uart);
}

void dbgUartPauseTx(void) {
	flag_tx_enabled = 0;
}

void dbgUartResumeTx(void) {
	flag_tx_enabled = 1;
	LL_USART_EnableIT_TXE(uart);
}

void dbgUartPutchar(char c) {
	FIFO_PUSH(dbg_uart_tx_fifo, c);
	if (flag_tx_enabled) LL_USART_EnableIT_TXE(uart);
}

void dbgUartPuts(const char* str) {
	while (*str) {
		dbgUartPutchar(*str);
		str++;
	}
}

void dbgUartPutsArray(const char* str, uint16_t len) {
	while (len--) {
		dbgUartPutchar(*str);
		str++;
	}
}

// выводит массив пока не встретится заданный символ
void dbgUartPutsUntil(const char* str, char sym) {
	while (sym != *str) {
		dbgUartPutchar(*str);
		str++;
	}
}

uint16_t dbgUartGetFreeSpace(void) {
	return FIFO_SPACE(dbg_uart_tx_fifo);
}

void USART_IRQ_HANDLER(void) { // код обработчика прерывания

//	if ((LL_USART_IsActiveFlag_RXNE(uart))&&(LL_USART_IsEnabledIT_RXNE(uart))) {
//		if (!LL_USART_IsActiveFlag_FE(uart)) {
//			processDbgUartRx(LL_USART_ReceiveData8(uart));
//		}
//	}

	if ((LL_USART_IsActiveFlag_TXE(uart))&&(LL_USART_IsEnabledIT_TXE(uart))) {
		if (FIFO_COUNT(dbg_uart_tx_fifo)) {
			LL_USART_TransmitData8(uart, FIFO_POP(dbg_uart_tx_fifo));
		} else {
			LL_USART_DisableIT_TXE(uart);
		}
	}
	
	LL_USART_ClearFlag_ORE(uart); // сброс флага OverRun
	LL_USART_ClearFlag_FE(uart); // сброс флага ошибки фрейма
	LL_USART_ClearFlag_NE(uart); // Clear Noise Error detected Flag
	LL_USART_ClearFlag_PE(uart); // Clear Parity Error Flag
}

void dbgUartPutsBitArray(unsigned char* mem, unsigned short n) {
	unsigned short i, j;
	char str[16];
	
	dbgUartPuts("\r");
	for(i = 0; i < n; i+=8) {
		for(j = 0; (j < 8)&&((i+j)<n); j++) {
			if (mem[i+j]> 1) {
				dbgUartPuts("\rError in dbgUartPutsBitArray(): number greater then 1 in bit sequence!\r");
				return;
			}
			str[j] = mem[i+j] + '0';
		}
		str[j] = '\0';
		dbgUartPuts(str);
		dbgUartPuts(" ");
	}
	dbgUartPuts("\r");
}

void HexToChar(char *s, unsigned char b) {
	const char lut[] = "0123456789ABCDEF";
	
	s[0] = lut[b >> 4];
	s[1] = lut[b & 0x0F];
	s[2] = '\0';
}
