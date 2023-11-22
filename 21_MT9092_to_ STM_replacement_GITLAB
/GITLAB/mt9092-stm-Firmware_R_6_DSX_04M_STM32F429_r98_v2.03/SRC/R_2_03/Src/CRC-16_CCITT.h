/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   CRC-16_CCITT.h
 * Author: roman.ganaza
 *
 * Created on November 27, 2020, 3:46 PM
 */

#ifndef CRC_16_CCITT_H
#define CRC_16_CCITT_H

unsigned short Crc16(unsigned char *pcBlock, unsigned short len);
unsigned long reflect(unsigned long crc, int bitnum);

#endif /* CRC_16_CCITT_H */
