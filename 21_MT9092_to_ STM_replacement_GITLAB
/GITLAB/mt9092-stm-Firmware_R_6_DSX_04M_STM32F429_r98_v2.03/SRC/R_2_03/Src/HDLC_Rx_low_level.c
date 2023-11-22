/*
 Здесь производится прием HDLC телеграмм с линии (нмжний уровень - битовый поток)
 */

/* 
 * File:   HDLC_bit_ring.c
 * Author: roman.ganaza
 * 
 * Created on September 21, 2020, 11:56 AM
 */

#include <string.h>

#include "HDLC_Rx_low_level.h"
#include "uart.h"
#include "HDLC_Rx.h"
#include "mt9092_registers.h"
#include "CRC-16_CCITT.h"
#include <stdio.h>

//#define DEBUG_INFO

static unsigned char bit_ring[BIT_RING_SIZE] = {0};
static unsigned char hdlc_frame_bit_sequence[BIT_RING_SIZE];
static unsigned char hdlc_frame_bytes[BIT_RING_SIZE/8]; // содержимое HDLC кадра (байты, не биты!)
static char telegram_string[TELEGRAM_LENGTH];
static uint16_t current_index = 0;
static unsigned char flag_string[8] = {0};
static const unsigned char flag_pattern[8] = {0, 1, 1, 1, 1, 1, 1, 0}; // 0x7E - flag sequence
static const unsigned char idle_pattern[8] = {1, 1, 1, 1, 1, 1, 1, 1}; // 0xFF - idle sequence
static const unsigned char abort_pattern[8] = {0, 1, 1, 1, 1, 1, 1, 1}; // 0x7F - abort sequence
static uint16_t start_flag_index = 0;
static unsigned char start_flag_received = 0;
static unsigned char flag_hdlc_frame_received = 0;
static unsigned short num_bits_in_frame = 0; // количество бит в витовом потоке HDLC кадра за вычетом 16 бит флагов 0x7E
#ifdef DEBUG_INFO
static char str[150];
#endif

void BitRingInit(void) {
	memset(bit_ring, 0, BIT_RING_SIZE);
	current_index = 0;
	start_flag_index = 0;
	start_flag_received = 0;
	uartPuts("\r\rBit ring init\r");
}
void debug(void) {
//	int i, j;
//	uint32_t t[28] = {0x7E, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x7E, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x7C, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x7E};
//
//	for(j = 0; j  < 28; j++) {
//		for (i = 0; i < 8; i++) {
//			BitRingProcessBit(t[j] & 0x01);
//			t[j] >>= 1;
//		}
//	}
}
uint16_t truncValue(uint16_t value) {
	return (value & (BIT_RING_SIZE - 1));
}

void Inc(uint16_t* c) {
	(*c)++;
	(*c) &= BIT_RING_SIZE - 1;
}

void Dec(uint16_t* c) {
	(*c)--;
	(*c) &= BIT_RING_SIZE - 1;
}

void readFromPosition(unsigned char *arr, uint16_t pos, uint16_t number) {
	uint16_t i;
	
	pos &= BIT_RING_SIZE - 1;
	
	for(i = 0; i < number; i++) {
		arr[i] = bit_ring[pos];
		Inc(&pos);
	}
}

void BitRingProcessBit(unsigned char bit) {
	uint16_t i, t, end;
	static uint16_t idle_state_counter = 0;
	
	if (bit) { // детектирование Idle State на линии (15 или более единиц подряд)
		if (idle_state_counter < 15) {
			idle_state_counter++;
		} else {
			SET_BIT(mt9092_reg[HDLC_STATUS_REGISTER], IDLE_CHAN);
		}
	} else {
		idle_state_counter = 0;
		CLEAR_BIT(mt9092_reg[HDLC_STATUS_REGISTER], IDLE_CHAN);
	}
	
	bit_ring[current_index] = bit; // запись в буфер
	Inc(&current_index);

	if (start_flag_received) {
		if (current_index == start_flag_index) { // переполнение
			start_flag_received = 0;

			return;
		}
	}
	
	readFromPosition(flag_string, current_index-8, 8);

	if (memcmp(flag_string, abort_pattern, 8) == 0) { // abort
		start_flag_received = 0;
	} else if (memcmp(flag_string, idle_pattern, 8) == 0) { // idle
		start_flag_received = 0;
	} else if (memcmp(flag_string, flag_pattern, 8) == 0) { // приняли флаг

		if (!start_flag_received) { // если стартовый флаг HDLC еще не принимали
			
			start_flag_index = truncValue(current_index - 8);
			start_flag_received = 1;
			CLEAR_BIT(mt9092_reg[HDLC_CONTROL_REGISTER_2], RXFRST); // выводим Rx FIFO из ресета
			
		} else { // стоповый флаг HDLC (может быть стартовым для следующего фрейма)
			
			num_bits_in_frame = truncValue(current_index - start_flag_index);
			if (num_bits_in_frame >= 48) { // 2 байта старт-стоп флаги, 2 байта адрес, 2 байта FCS

				num_bits_in_frame -= 2*8; // удаляем стартовый и стоповый флаги
				
				i = truncValue(start_flag_index + 8);
				t = 0;
				end = truncValue(current_index - 8);
				while (i != end) {
					hdlc_frame_bit_sequence[t] = bit_ring[i];
					t++;
					Inc(&i);
				}
				flag_hdlc_frame_received = 1; // флаг на обработку кадра HDLC

			}
			start_flag_index = truncValue(current_index - 8); // стоповый флаг может быть стартовым для следующего фрейма
			//start_flag_received = 1; // этот флаг здесь и так уже установлен
		}
	}
}

void processHdlcFrame(void) {
	uint16_t i, n_bits, calc_crc, rec_crc;
	uint8_t byte, bit_count, crc_ok = 0;
	char ones_count = 0; // счетчик единиц, идущих подряд в битовом потоке
	uint8_t tel_len = 0; // длина телеграммы в байтах

	if (flag_hdlc_frame_received) {
		n_bits = num_bits_in_frame;
		ones_count = 0;
		
		for(i = 0; i < n_bits; i++) { // удаляем бит-стаффинг (лишние нули после 5 единиц)
			if (ones_count >= 5) {
				ones_count = 0;
				if (hdlc_frame_bit_sequence[i] == 0) { // если после 5-ти единиц идет 0, удаляем его
					if (i != n_bits - 1) {
						memmove(&hdlc_frame_bit_sequence[i], &hdlc_frame_bit_sequence[i + 1], n_bits - i);
					}
					n_bits--; // суммарное количество элементов уменьшилось на 1
					i--; // чтобы учесть элемент, помещенный на место удаленного нуля
				} else { // больше 5 единиц подряд быть не должно
					uartPuts("\r\r> 5 ones contiguous!\r"); // по идее, такие ситуации фильтруются ещё в BitRingProcessBit()
					flag_hdlc_frame_received = 0; // сбрасываем флаг
					return;
				}
			} else {
				if (hdlc_frame_bit_sequence[i] == 1) {
					ones_count++;
				} else {
					ones_count = 0;
				}
			}
		}
		
		if ((n_bits % 8) != 0) { // содержимое кадра HDLC должно быть кратно 8 битам
#ifdef DEBUG_INFO
			uartPuts("\rNot / 8!\r");
#endif
			
		} else {

			byte = 0;
			bit_count = 0;
			
			for (i = 0; i < n_bits-16; i++) { // собираем биты в байты (исключая CRC), LSB first
				
				byte |= hdlc_frame_bit_sequence[i] << bit_count;

				bit_count++;
				if (bit_count == 8) {
					hdlc_frame_bytes[i/8] = byte;
					byte = 0;
					bit_count = 0;
				}
			}
			byte = 0;
			bit_count = 0;
			for (i = n_bits-16; i < n_bits; i++) { // собираем биты в байты (CRC), MSB first
				
				byte |= hdlc_frame_bit_sequence[i] << (7 - bit_count);

				bit_count++;
				if (bit_count == 8) {
					hdlc_frame_bytes[i/8] = byte;
					byte = 0;
					bit_count = 0;
				}
			}
			tel_len = n_bits/8-2; // длина телеграммы = длина фрейма за вычетом CRC
			
			rec_crc = ((uint16_t)hdlc_frame_bytes[tel_len] << 8) | hdlc_frame_bytes[tel_len+1]; // принятая CRC
			
			calc_crc = Crc16(hdlc_frame_bytes, tel_len);
			
			if (calc_crc == rec_crc) crc_ok = 1; else crc_ok = 0;
			
			if (crc_ok) {
				uartPuts("\r");
				if (tel_len < TELEGRAM_LENGTH - 1) {
					memcpy(telegram_string, hdlc_frame_bytes, tel_len); // без CRC
					telegram_string[tel_len] = '\0';
					uartPuts("->HDLC Rx: ");
					uartPutsN(telegram_string, tel_len);
					uartPuts("\r");
					processHdlcTelegram(telegram_string, tel_len);
				} else {
					uartPuts("Telegram is too long!\r");
				}
				#ifdef DEBUG_INFO
				uartPuts("   ");
				uartPuts("CRC OK\r");
				#endif
			} else {
				#ifdef DEBUG_INFO
				uartPuts("CRC Not OK\r");
				#endif
			}
		}
		
		flag_hdlc_frame_received = 0; // сбрасываем флаг
	}
}
