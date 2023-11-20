/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   HDLC_Tx.h
 * Author: roman.ganaza
 *
 * Created on November 25, 2020, 1:20 PM
 */

#ifndef HDLC_TX_H
#define HDLC_TX_H

#include <stdint.h>

#define MAX_TX_TEL_LEN         32 // максимальное количество символов в телеграмме на передачу по HDLC

void writeHdlcTxFifo(unsigned char);
void startHdlcTransmition(void);
void resetHdlcTxFifo(void);
unsigned char isHdlcFifoEmpty(void);
void HDLC_ProcessTx(void);

extern uint8_t flag_HDLC_transmitter_busy; // идёт передача телеграммы по HDLC (передатчик занят)

#endif /* HDLC_TX_H */
