/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   CRC-16_CCITT.c
 * Author: roman.ganaza
 * 
 * Created on November 27, 2020, 3:46 PM
 */

#include "CRC-16_CCITT.h"

// зеркалит bitnum бит в переменной crc

unsigned long reflect(unsigned long crc, int bitnum) {

	// reflects the lower 'bitnum' bits of 'crc'

	unsigned long i, j = 1, crcout = 0;

	for (i = (unsigned long) 1 << (bitnum - 1); i; i >>= 1) {
		if (crc & i) crcout |= j;
		j <<= 1;
	}
	return (crcout);
}

/*
	Name  : CRC-16 CCITT
	Poly  : 0x11021	x^16 + x^12 + x^5 + 1
	Init  : 0xFFFF
	Revert input (reflect): true (LSB first)
	XorOut: 0x0000
	Check : 0x29B1 ("123456789")
	MaxLen: 4095 байт (32767 бит) - обнаружение
		одинарных, двойных, тройных и всех нечетных ошибок
 */
unsigned short Crc16(unsigned char *pcBlock, unsigned short len) {
	unsigned short crc = 0xFFFF;
	unsigned char i;

	while (len--) {
		crc ^= reflect(*pcBlock++, 8) << 8;

		for (i = 0; i < 8; i++)
			crc = crc & 0x8000 ? (crc << 1) ^ 0x1021 : crc << 1;
	}

	return crc ^ 0xFFFF; // инвертируем, так приходит по HDLC
}
