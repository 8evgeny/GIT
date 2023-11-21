/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   HDLC_bit_ring.h
 * Author: roman.ganaza
 *
 * Created on September 21, 2020, 11:56 AM
 */

#ifndef HDLC_BIT_RING_H
#define HDLC_BIT_RING_H

#include <stdint.h>

#define BIT_RING_SIZE 1024u // power of 2 !!!

#define TELEGRAM_LENGTH      64

extern char telegram_string[TELEGRAM_LENGTH];
extern uint8_t telegram_len;
extern uint8_t flag_telegram_received;

void BitRingInit(void);
void BitRingProcessBit(unsigned char bit);
void BitRingProcessBit(unsigned char bit);
void processHdlcFrame(void);
void debug(void);

#endif /* HDLC_BIT_RING_H */
