/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   HDLC_Tx_low_level.c
 * Author: roman.ganaza
 * 
 * Created on November 26, 2020, 11:36 AM
 */

#include <string.h>

#include "HDLC_Tx_low_level.h"
#include "uart.h"
#include "mt9092_registers.h"
#include "CRC-16_CCITT.h"
#include "FIFO.h"
#include "HDLC_Tx.h"
#include "mt9092_logic.h"

volatile unsigned char tx_hdlc_packet_bit_stream[512]; // последовательность бит для передачи HDLC пакета (битовый поток)
volatile uint16_t bit_stream_length = 0; // счетчик бит битового потока Tx HDLC
volatile uint16_t current_bit = 0;
volatile uint16_t ones_counter = 0;
unsigned char flag_HDLC_bitstream_ready = 0; // устраняет ситуацию, когда HTXEN установлен, а передавать ещё нечего и из-за этого ломается алгоритм

void addFlagToBitStream(void) {
	unsigned char i;
	unsigned char b = 0x7E; // флаг начала/конца пакета HDLC
	
	for(i = 0; i < 8; i++) {
		tx_hdlc_packet_bit_stream[bit_stream_length] = b & 0x01;
		
		bit_stream_length++;
		
		b >>= 1; // следующий бит входного байта
	}
}

void addByteToBitStream(unsigned char b) {
	unsigned char i;
	
	for(i = 0; i < 8; i++) {
		tx_hdlc_packet_bit_stream[bit_stream_length] = b & 0x01;
		
		if (tx_hdlc_packet_bit_stream[bit_stream_length] == 1) ones_counter++;
		else ones_counter = 0;
		
		bit_stream_length++;
		
		if (ones_counter == 5) { // если встретилось 5 единиц подряд, принудительно вставляем 0
			ones_counter = 0;
			tx_hdlc_packet_bit_stream[bit_stream_length] = 0;
			bit_stream_length++;
		}
		
		b >>= 1; // следующий бит входного байта
	}
}
void createHdlcBitStream(unsigned char* packet, unsigned char len) {
	unsigned char i;
	unsigned short crc;
	
	bit_stream_length = 0;
	ones_counter = 0;
	current_bit = 0;
	
	addFlagToBitStream(); // стартовый флаг
	
	for(i = 0; i < len; i++) {
		addByteToBitStream(packet[i]);
	}
	
	crc = Crc16(packet, len);
	
	addByteToBitStream(reflect((crc >> 8), 8)); // отражаем биты, т.к. CRC отправляется MSB first 
	addByteToBitStream(reflect((crc & 0xFF), 8));
	
	addFlagToBitStream(); // стоповый флаг
	
	flag_HDLC_bitstream_ready = 1;
}

unsigned char HdlcBitStreamGetBit(void) {
	unsigned char result;
	
	result = tx_hdlc_packet_bit_stream[current_bit];
	
	current_bit++;
	if (current_bit >= bit_stream_length) {
		if (isHdlcFifoEmpty()) { // нечего больше передавать
			CLEAR_BIT(mt9092_reg[HDLC_CONTROL_REGISTER_1], HTXEN); // отключаем передачу		
		} else {
			if (READ_BIT(mt9092_reg[HDLC_CONTROL_REGISTER_1], HTXEN)) { // если включен передатчик
				flag_start_hdlc_transmission = 1; // запускаем следующую посылку
			}
		}
		flag_HDLC_bitstream_ready = 0; // всё передали
	}
	
	return result;
}
