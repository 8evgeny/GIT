/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   HDLC_Tx_low_level.h
 * Author: roman.ganaza
 *
 * Created on November 26, 2020, 11:36 AM
 */

#ifndef HDLC_TX_LOW_LEVEL_H
#define HDLC_TX_LOW_LEVEL_H

extern unsigned char flag_HDLC_bitstream_ready;

void createHdlcBitStream(unsigned char* packet, unsigned char len);
unsigned char HdlcBitStreamGetBit(void);

#endif /* HDLC_TX_LOW_LEVEL_H */
