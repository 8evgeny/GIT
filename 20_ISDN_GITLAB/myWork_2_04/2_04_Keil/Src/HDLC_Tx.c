/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   HDLC_Tx.c
 * Author: roman.ganaza
 * 
 * Created on November 25, 2020, 1:20 PM
 */

#include <string.h>

#include "HDLC_Tx.h"
#include "FIFO.h"
#include "dbg_uart.h"
#include "HDLC_Tx_low_level.h"

FIFO(MAX_TX_TEL_LEN) tx_fifo;
unsigned char hdlc_packet_data[MAX_TX_TEL_LEN];
uint8_t flag_HDLC_transmitter_busy = 0; // идёт передача телеграммы по HDLC (передатчик занят)

void writeHdlcTxFifo(unsigned char data) {
//	uartPuts("Write HDLC Tx FIFO: ");
//	uartPutchar(data);
//	uartPutchar('\r');
	if (!(FIFO_IS_FULL(tx_fifo))) {
		FIFO_PUSH(tx_fifo, data);
		
	}	else puts("\rError: HDLC Tx FIFO overflow!\r");
}

void startHdlcTransmition(void) {
	unsigned char len = 0, i = 0;
	
	if (FIFO_COUNT(tx_fifo) == 0) { // нечего передавать
    flag_HDLC_transmitter_busy = 0;
		return;
	}
	
	while(FIFO_COUNT(tx_fifo) > 0) {
		hdlc_packet_data[len] = FIFO_POP(tx_fifo);
		len++;
	}

	flag_HDLC_transmitter_busy = 1;
	puts("\r<-HDLC Tx: ");
	for (i = 0; i < len; i++) dbgUartPutchar(hdlc_packet_data[i]);
	puts("\r");
	
	createHdlcBitStream(hdlc_packet_data, i);
}

void resetHdlcTxFifo(void) {
	FIFO_FLUSH(tx_fifo);
}

unsigned char isHdlcFifoEmpty(void) {
	if (FIFO_COUNT(tx_fifo) == 0) return 1;
	else return 0;
}

void HDLC_ProcessTx(void) {
  if (flag_start_hdlc_transmission) {
    flag_start_hdlc_transmission = 0;
    
    startHdlcTransmition();
  }
}

