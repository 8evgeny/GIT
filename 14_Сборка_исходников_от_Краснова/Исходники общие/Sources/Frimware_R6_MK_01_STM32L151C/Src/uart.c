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

#include "uart.h"
#include "FIFO.h"
#include <string.h>
#include <stdio.h>
#include "main.h"
#include "main_logic.h"
#include "a_main.h"

#define USE_RS485

#ifdef USE_RS485
  #define RS485_TX()      TX_485()
  #define RS485_RX()      RX_485()
#endif

#define USART_IRQ_HANDLER    USART1_IRQHandler // обработчик прерывания
#define uart	USART1

#define RX_BLINK_PERIOD		100 // мс - подмигивание желтым по приему байт

FIFO(64) uart_tx_fifo;
FIFO(64) uart_rx_fifo;

static uint16_t blink_counter = 1;

void blink(void) {
  HAL_GPIO_WritePin(LED0_GPIO_Port, LED0_Pin, GPIO_PIN_RESET);
	blink_counter = RX_BLINK_PERIOD;
}

void doRxBlink(void) { // поместить в 1 мс прерывание
	if (blink_counter > 0) {
		blink_counter--;
		if (blink_counter == 0) {
      HAL_GPIO_WritePin(LED0_GPIO_Port, LED0_Pin, GPIO_PIN_SET);
		}
	}
}

void uartInit(void) {
#ifdef USE_RS485
  RS485_RX();
  LL_USART_EnableIT_TC(uart);
#endif
  uartReceiveStart();
}

void uartReceiveStart(void) {
	volatile char dummy;
//	dummy = uart->RDR; // чистим флаг
	dummy = uart->DR; // чистим флаг
	FIFO_FLUSH(uart_rx_fifo);
	LL_USART_EnableIT_RXNE(uart);
}

void uartReceiveStop(void) {
	LL_USART_DisableIT_RXNE(uart);
}

void uartRxFifoFlush(void) {
	FIFO_FLUSH(uart_rx_fifo);
}

void uartPutchar(char c) {
#ifdef USE_RS485
  RS485_TX();
#endif
	FIFO_PUSH(uart_tx_fifo, c);
	LL_USART_EnableIT_TXE(uart);
}

void uartPuts(char* str) {
	while (*str) {
		uartPutchar(*str);
		str++;
	}
}

void uartPutsN(unsigned char* data, unsigned short n) {
	while (n) {
		uartPutchar(*data);
		data++;
		n--;
	}
}

unsigned short uartBytesReceived(void) {
	return FIFO_COUNT(uart_rx_fifo);
}

char uartGetchar(void) {
	return FIFO_POP(uart_rx_fifo);
}
void USART_IRQ_HANDLER(void) { // прерывание по USART
	unsigned char c;
	if ((LL_USART_IsActiveFlag_RXNE(uart))&&(LL_USART_IsEnabledIT_RXNE(uart))) {

		if (!LL_USART_IsActiveFlag_FE(uart)) {
			c = LL_USART_ReceiveData8(uart);
			FIFO_PUSH(uart_rx_fifo, c);
			blink();
		}
	}
	if ((LL_USART_IsActiveFlag_TXE(uart))&&(LL_USART_IsEnabledIT_TXE(uart))) {
		if (FIFO_COUNT(uart_tx_fifo)) {
			LL_USART_TransmitData8(uart, FIFO_POP(uart_tx_fifo));
		} else {
			LL_USART_DisableIT_TXE(uart);
		}
  }
#ifdef USE_RS485
  if ((LL_USART_IsActiveFlag_TC(uart))&&(LL_USART_IsEnabledIT_TC(uart))) {
    RS485_RX(); // переключаем обратно на прием призавершении передачи
    LL_USART_ClearFlag_TC(uart);
  }
#endif
	LL_USART_ClearFlag_ORE(uart); // сброс флага OverRun
	LL_USART_ClearFlag_FE(uart); // сброс флага ошибки фрейма
	LL_USART_ClearFlag_NE(uart); // Clear Noise Error detected Flag
	LL_USART_ClearFlag_PE(uart); // Clear Parity Error Flag
}

/*
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
}*/
