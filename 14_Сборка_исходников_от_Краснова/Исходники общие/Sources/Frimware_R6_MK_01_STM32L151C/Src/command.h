/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   command.h
 * Author: roman.ganaza
 *
 * Created on January 12, 2021, 10:32 AM
 */

#ifndef COMMAND_H
#define COMMAND_H

#include "FIFO.h"

void cmdProcessInputByte(unsigned char b);
void doCmdTimeout(void);
void doCommand(void);
void respPutsN(void * data, unsigned char n);
unsigned short respGetCount(void); // возвращает число оставшихся (неотправленных) байт ответа на команду
unsigned char respGetByte(void); // возвращает очередной байт ответа на команду

#endif /* COMMAND_H */
