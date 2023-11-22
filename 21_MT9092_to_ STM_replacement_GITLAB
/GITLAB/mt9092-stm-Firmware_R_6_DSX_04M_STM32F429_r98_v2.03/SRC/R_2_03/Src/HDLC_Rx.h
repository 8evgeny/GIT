/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   HDLC_Rx.h
 * Author: roman.ganaza
 *
 * Created on November 11, 2020, 10:46 AM
 */

#ifndef HDLC_RX_H
#define HDLC_RX_H

void processHdlcTelegram(char * tel, unsigned char tel_len);
unsigned char readHdlcRxFifo(void);
void resetHdlcRxFifo(void);

#endif /* HDLC_RX_H */
