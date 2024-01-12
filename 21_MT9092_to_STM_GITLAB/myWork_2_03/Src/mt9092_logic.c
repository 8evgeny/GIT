/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   mt9092_logic.c
 * Author: roman.ganaza
 * 
 * Created on November 5, 2020, 10:59 AM
 */

#include "mt9092_logic.h"
#include "stm32f4xx.h"
#include "mt9092_registers.h"
#include "HDLC_Rx.h"
#include "uart.h"
#include "HDLC_Tx.h"
#include "ST-BUS_SAI_in_out.h"
#include "HDLC_Tx_low_level.h"
#include "codec.h"

unsigned char rx_fifo_treshold = 5; // порог для флага заполнения фифо RXSTAT2 регистра HDLC Status Register (м.б. 5 или 15 байт)
unsigned char tx_fifo_treshold = 5; // порог для флага опустошения фифо TXSTAT2 регистра HDLC Status Register (м.б. 5 или 15 байт)
unsigned char flag_start_hdlc_transmission = 0;
unsigned char flag_reset = 0;
uint8_t flag_new_TRANSDUCER_CONTROL_REGISTER = 0;

void mt9092LogicInit(void) {
	mt9092RegistersInit();
}

void mt9092Reset(void) {
	mt9092RegistersInit();
	resetHdlcRxFifo();
  HAL_GPIO_WritePin(LED3_GPIO_Port, LED3_Pin, GPIO_PIN_RESET);
	SAI_reset();
}

void mt9092IrqOn(void) { // активирует ногу прерывания IRQ (active Low)
	SET_BIT(mt9092_reg[HDLC_STATUS_REGISTER], INTGEN);
	IRQ_PIN_RESET();
    //uartPuts("IRQ On\r\n");
}

void mt9092IrqOff(void) { // деактивирует ногу прерывания IRQ
	CLEAR_BIT(mt9092_reg[HDLC_STATUS_REGISTER], INTGEN);
	IRQ_PIN_SET();
    //uartPuts("IRQ Off\r\n");
}

// обрабатывает команду на запись по SPI от Меги, возвращает 0, если команда корректна
int32_t mt9092ProcessWriteCommand(unsigned char reg_addr, unsigned char data) {
	if (reg_addr > MT9092_MAX_REG_ADDR) {
        uartPuts("\r\rError: attempt to write too big reg addr\r\n");
		return -1;
	}
	
	switch (reg_addr) {
		case GENERAL_CONTROL_REGISTER:
			if (data & 0x80) flag_reset = 1;
			mt9092_reg[reg_addr] = data;
			break;
		case HDLC_TX_RX_FIFO: // read/write separatly
			writeHdlcTxFifo(data);
			break;
		case C_CHANNEL_REGISTER: // read/write separatly
			C_channel_out = data;
			break;
		case HDLC_STATUS_REGISTER: case HDLC_INTERRUPT_STATUS_REGISTER:// read only
			
			break;
		case HDLC_CONTROL_REGISTER_1:
			// если передатчик HDLC был выключен, а теперь его включили
			if ((READ_BIT(data, HTXEN) != 0)&&(READ_BIT(mt9092_reg[reg_addr], HTXEN) == 0)) {
				if (!flag_HDLC_bitstream_ready) { // если какой-либо пакет уже не передается в данный момент, если уже передается, то следующий пойдет автоматически при включенном передатчике
					flag_start_hdlc_transmission = 1; // стартуем передачу пакета
				}
			}
			mt9092_reg[reg_addr] = data;
			break;
		case HDLC_CONTROL_REGISTER_2:
			if (READ_BIT(data, RXFRST)) {
				resetHdlcRxFifo();
			}
			if (READ_BIT(data, TXFRST)) {
				resetHdlcTxFifo();
			}
			mt9092_reg[reg_addr] = data;
			break;
    case TRANSDUCER_CONTROL_REGISTER:
      flag_new_TRANSDUCER_CONTROL_REGISTER = 1;
      mt9092_reg[reg_addr] = data;
      break;
		default: // write/verify or write only registers
			if ((reg_addr == 0x08) || (reg_addr == 0x09) || (reg_addr == 0x0c) || (reg_addr == 0x0d) ||
					(reg_addr == 0x10) || ((reg_addr >= 0x17)&&(reg_addr <= 0x1c)) || (reg_addr == 0x1F) ||
					(reg_addr == 0x22) || (reg_addr == 0x25) || ((reg_addr >= 0x27)&&(reg_addr <= 0x3f))) {
                uartPuts("\r\rError: attempt to write reserved registers\r\n");
				return -1;
			} else {
				mt9092_reg[reg_addr] = data;
			}
			break;			
	}
	return 0; // успешно
}

// обрабатывает команду на чтение по SPI от Меги
// передает байт ответа для SPI по указателю data
int32_t mt9092ProcessReadCommand(unsigned char reg_addr, unsigned char *data) {
	unsigned char t;
	
	if (reg_addr > MT9092_MAX_REG_ADDR) {
        uartPuts("\r\rError: attempt to read too big reg addr\r\n");
		*data = 0;
		return -1;
	}
	
	switch (reg_addr) {
		case HDLC_TX_RX_FIFO: // read/write separatly
			*data = readHdlcRxFifo();
			break;
		case C_CHANNEL_REGISTER: // read/write separatly
			*data = mt9092_reg[reg_addr];
			break;			
		case HDLC_STATUS_REGISTER: // read only
			*data = mt9092_reg[reg_addr];
			break;			
		case HDLC_INTERRUPT_STATUS_REGISTER: // read only
			t = mt9092_reg[reg_addr];
			mt9092_reg[HDLC_INTERRUPT_STATUS_REGISTER] = 0;
			mt9092IrqOff();
			*data = t;
			break;			
		default: // write/verify or read only
			if ((reg_addr == 0x08) || (reg_addr == 0x09) || (reg_addr == 0x0c) || (reg_addr == 0x0d) ||
					(reg_addr == 0x10) || ((reg_addr >= 0x17)&&(reg_addr <= 0x1c)) || (reg_addr == 0x1F) ||
					(reg_addr == 0x22) || (reg_addr == 0x25) || ((reg_addr >= 0x27)&&(reg_addr <= 0x3f))) {
                uartPuts("\r\rError: attempt to  read reserved registers\r\n");
				return -1;
			} else {
				*data = mt9092_reg[reg_addr];
			}
			break;
	}
	return 0; // успешно
}

void doMt9092Logic(void) {
 
	if (flag_start_hdlc_transmission) {
		flag_start_hdlc_transmission = 0;
		startHdlcTransmition();
	}
	if (flag_reset) {
		flag_reset = 0;
		mt9092Reset();
	}
  
  if (flag_new_TRANSDUCER_CONTROL_REGISTER) { // была запсиь в регистр управления звуком
    flag_new_TRANSDUCER_CONTROL_REGISTER = 0;

    if (mt9092_reg[TRANSDUCER_CONTROL_REGISTER] & MIC_EN) {
      micOn();
    } else {
      micOff();
    }
    if (mt9092_reg[TRANSDUCER_CONTROL_REGISTER] & MIC_nHNSTMIC) {
      selectMic(1);
    } else {
      selectMic(0);
    }
    if (mt9092_reg[TRANSDUCER_CONTROL_REGISTER] & SPKR_EN) {
      lineOutSpeakerOn();
    } else {
      lineOutSpeakerOff();
    }
    if (mt9092_reg[TRANSDUCER_CONTROL_REGISTER] & HSSPKR_EN) {
      headsetSpeakerOn();
    } else {
      headsetSpeakerOff();
    }
  }
}
