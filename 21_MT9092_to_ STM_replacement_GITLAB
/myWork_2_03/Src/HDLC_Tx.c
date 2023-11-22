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
#include "uart.h"
#include "mt9092_registers.h"
#include "HDLC_Tx_low_level.h"
#include "mt9092_logic.h"

FIFO(32) tx_fifo;
unsigned char hdlc_packet_data[32];
unsigned char delayed_hdlc_packet_data[32];
unsigned char delayed_hdlc_packet_len;
unsigned char flag_delay_hdlc_packet = 0;
uint32_t delay_tmr = 0;

void writeHdlcTxFifo(unsigned char data) {
//	uartPuts("Write HDLC Tx FIFO: ");
//	uartPutchar(data);
//	uartPutchar('\r');
	if (!(FIFO_IS_FULL(tx_fifo))) {
		FIFO_PUSH(tx_fifo, data);
		
		
		if (FIFO_COUNT(tx_fifo) == 0) { // TX FIFO EMPTY
			SET_BIT(mt9092_reg[HDLC_STATUS_REGISTER], TXSTAT2);
			CLEAR_BIT(mt9092_reg[HDLC_STATUS_REGISTER], TXSTAT1);
		} else if (FIFO_COUNT(tx_fifo) < tx_fifo_treshold) { // < 5 (15) байт в TX FIFO
			SET_BIT(mt9092_reg[HDLC_STATUS_REGISTER], TXSTAT2);
			SET_BIT(mt9092_reg[HDLC_STATUS_REGISTER], TXSTAT1);
		} else if (FIFO_COUNT(tx_fifo) < MT9092_FIFO_SIZE) {
			CLEAR_BIT(mt9092_reg[HDLC_STATUS_REGISTER], TXSTAT2);
			SET_BIT(mt9092_reg[HDLC_STATUS_REGISTER], TXSTAT1);
		} else { // TX FIFO FULL
			CLEAR_BIT(mt9092_reg[HDLC_STATUS_REGISTER], TXSTAT2);
			CLEAR_BIT(mt9092_reg[HDLC_STATUS_REGISTER], TXSTAT1);
		}
	}	else uartPuts("\rError: HDLC Tx FIFO overflow!\r");
}

void startHdlcTransmition(void) {
	unsigned char len = 0, i = 0;
	
	if (FIFO_COUNT(tx_fifo) == 0) { // нечего передавать
		CLEAR_BIT(mt9092_reg[HDLC_CONTROL_REGISTER_1], HTXEN); // отключаем передачу
		return;
	}
	
	while(FIFO_COUNT(tx_fifo) > 0) {
		hdlc_packet_data[len] = FIFO_POP(tx_fifo);
		len++;
	}

	// FIFO EMPTY bits
	SET_BIT(mt9092_reg[HDLC_STATUS_REGISTER], TXSTAT2);
	CLEAR_BIT(mt9092_reg[HDLC_STATUS_REGISTER], TXSTAT1);
	
	uartPuts("\r<-HDLC Tx: ");
	for (i = 0; i < len; i++) uartPutchar(hdlc_packet_data[i]);
	uartPuts("\r");
	
	createHdlcBitStream(hdlc_packet_data, i);
}

void resetHdlcTxFifo(void) {
	SET_BIT(mt9092_reg[HDLC_STATUS_REGISTER], TXSTAT2);
	CLEAR_BIT(mt9092_reg[HDLC_STATUS_REGISTER], TXSTAT1);
	FIFO_FLUSH(tx_fifo);
}

unsigned char isHdlcFifoEmpty(void) {
	if (FIFO_COUNT(tx_fifo) == 0) return 1;
	else return 0;
}
