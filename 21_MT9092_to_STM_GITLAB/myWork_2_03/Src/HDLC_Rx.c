/*
 * здесь организован механизм, имитирующий приемник HDLC MT9092,
 * т.е. Rx FIFO, прерывания, выдачу по SPI и т.д.
 */

/* 
 * File:   HDLC_Rx.c
 * Author: roman.ganaza
 * 
 * Created on November 11, 2020, 10:46 AM
 */

#include "HDLC_Rx.h"
#include "mt9092_logic.h"
#include "FIFO.h"
#include "uart.h"
#include "mt9092_registers.h"

FIFO(32) rx_fifo;

void processHdlcTelegram(char * tel, unsigned char tel_len) {
	uint8_t need_irq = 0;
	
	CLEAR_BIT(mt9092_reg[HDLC_CONTROL_REGISTER_1], HRXEN); // вырубаем приемник HDLC (такова логика MT9092)
	
	while(tel_len != 0) {
		if (!(FIFO_IS_FULL(rx_fifo))) FIFO_PUSH(rx_fifo, *tel);
		else { // таких длинных пакетов быть не должно - игнорируем
            uartPuts("\rError: HDLC Rx FIFO overflow!\r\n");
			resetHdlcRxFifo();
			return;
		}
		tel++;
		tel_len--;
	}
	if (FIFO_COUNT(rx_fifo) >= rx_fifo_treshold) {
		if (READ_BIT(mt9092_reg[HDLC_INTERRUPT_ENABLE_REGISTER], RX_FF)) {
			SET_BIT(mt9092_reg[HDLC_INTERRUPT_STATUS_REGISTER], RX_FF); // Rx FIFO has 5 or 15 more bytes
			need_irq = 1;
		}
		SET_BIT(mt9092_reg[HDLC_STATUS_REGISTER], RXSTAT2);
		SET_BIT(mt9092_reg[HDLC_STATUS_REGISTER], RXSTAT1);
	} else {
		CLEAR_BIT(mt9092_reg[HDLC_STATUS_REGISTER], RXSTAT2);
		SET_BIT(mt9092_reg[HDLC_STATUS_REGISTER], RXSTAT1);
	}
	
	if (READ_BIT(mt9092_reg[HDLC_INTERRUPT_ENABLE_REGISTER], EOPD)) {
		SET_BIT(mt9092_reg[HDLC_INTERRUPT_STATUS_REGISTER], EOPD); // end of packet
		need_irq = 1;
	}
	// first byte
	CLEAR_BIT(mt9092_reg[HDLC_STATUS_REGISTER], RXBS2);
	SET_BIT(mt9092_reg[HDLC_STATUS_REGISTER], RXBS1);
	
	if (need_irq) mt9092IrqOn();
}

unsigned char readHdlcRxFifo(void) {
	unsigned char t = 0;
	
	if (FIFO_COUNT(rx_fifo) > 0) t = FIFO_POP(rx_fifo);

	if (FIFO_COUNT(rx_fifo) > 1) {
		// packet byte
		CLEAR_BIT(mt9092_reg[HDLC_STATUS_REGISTER], RXBS2);
		CLEAR_BIT(mt9092_reg[HDLC_STATUS_REGISTER], RXBS1);
	} else if (FIFO_COUNT(rx_fifo) == 1) {
		// last byte (good packet)
		SET_BIT(mt9092_reg[HDLC_STATUS_REGISTER], RXBS2);
		CLEAR_BIT(mt9092_reg[HDLC_STATUS_REGISTER], RXBS1);
	}
	
	if (FIFO_COUNT(rx_fifo) >= rx_fifo_treshold) {
		// 5 (15) or more bytes on Rx FIFO
		SET_BIT(mt9092_reg[HDLC_STATUS_REGISTER], RXSTAT2);
		SET_BIT(mt9092_reg[HDLC_STATUS_REGISTER], RXSTAT1);
	} else if (FIFO_COUNT(rx_fifo) > 0) {
		// 4 (14) or less bytes on Rx FIFO
		CLEAR_BIT(mt9092_reg[HDLC_STATUS_REGISTER], RXSTAT2);
		SET_BIT(mt9092_reg[HDLC_STATUS_REGISTER], RXSTAT1);
	} else { // == 0
		// Rx FIFO Empty
		CLEAR_BIT(mt9092_reg[HDLC_STATUS_REGISTER], RXSTAT2);
		CLEAR_BIT(mt9092_reg[HDLC_STATUS_REGISTER], RXSTAT1);
	}
	return t;
}

void resetHdlcRxFifo(void) {
	SET_BIT(mt9092_reg[HDLC_STATUS_REGISTER], RXBS1 | RXBS2);
	CLEAR_BIT(mt9092_reg[HDLC_STATUS_REGISTER], RXSTAT1 | RXSTAT2);
	FIFO_FLUSH(rx_fifo);
}
