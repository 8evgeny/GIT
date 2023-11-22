/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   mt9092_logic.h
 * Author: roman.ganaza
 *
 * Created on November 5, 2020, 10:59 AM
 */

#ifndef MT9092_LOGIC_H
#define MT9092_LOGIC_H

#include <stdint.h>

// пин прерывания HDLC (выход)
#define IRQ_PIN_SET()       HAL_GPIO_WritePin(IRQ_GPIO_Port, IRQ_Pin, GPIO_PIN_SET)
#define IRQ_PIN_RESET()     HAL_GPIO_WritePin(IRQ_GPIO_Port, IRQ_Pin, GPIO_PIN_RESET)

#define MT9092_FIFO_SIZE 19

#define KEY_UNPRESS_DELAY   300 // мс - костыль - задержка отжатия кнопки

extern unsigned char rx_fifo_treshold;
extern unsigned char tx_fifo_treshold;
extern unsigned char flag_start_hdlc_transmission;

void mt9092LogicInit(void);
void mt9092Reset(void);
void mt9092IrqOn(void);
void mt9092IrqOff(void);
int32_t mt9092ProcessWriteCommand(unsigned char reg_addr, unsigned char data);
int32_t mt9092ProcessReadCommand(unsigned char reg_addr, unsigned char *data);
void doMt9092Logic(void);

#endif /* MT9092_LOGIC_H */
