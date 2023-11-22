/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   mt9092_registers.c
 * Author: roman.ganaza
 * 
 * Created on November 5, 2020, 12:14 PM
 */

#include "mt9092_registers.h"

unsigned char mt9092_reg[MT9092_MAX_REG_ADDR+1];
unsigned char C_channel_out;

void mt9092RegistersInit(void) {
	C_channel_out = 0x14; // сразу стартуем MT9172, Мегу не ждем
	mt9092_reg[HDLC_ADDRESS_RECOGNITION_REGISTER_1] = 0;
	mt9092_reg[HDLC_ADDRESS_RECOGNITION_REGISTER_2] = 0;
	mt9092_reg[HDLC_TX_RX_FIFO] = 0;
	mt9092_reg[HDLC_CONTROL_REGISTER_1] = 0;
	mt9092_reg[HDLC_STATUS_REGISTER] = 0x78;
	mt9092_reg[HDLC_CONTROL_REGISTER_2] = 0;
	mt9092_reg[HDLC_INTERRUPT_ENABLE_REGISTER] = 0;
	mt9092_reg[HDLC_INTERRUPT_STATUS_REGISTER] = 0;
	mt9092_reg[FCODEC_GAIN_CONTROL_REGISTER_1] = 0;
	mt9092_reg[FCODEC_GAIN_CONTROL_REGISTER_2] = 0;
	mt9092_reg[TRANSDUCER_CONTROL_REGISTER] = 0;
	mt9092_reg[GENERAL_CONTROL_REGISTER] = 0;
	mt9092_reg[WATCHDOG_REGISTER] = 0x0A;
	mt9092_reg[LCD_SEGMENT_ENABLE_REGISTER_1] = 0;
	mt9092_reg[LCD_SEGMENT_ENABLE_REGISTER_2] = 0;
	mt9092_reg[C_CHANNEL_REGISTER] = 0;
	mt9092_reg[TIMING_CONTROL_REGISTER] = 0;
	mt9092_reg[LOOP_BACK_REGISTER_VERIFY] = 0;
	mt9092_reg[RECEIVE_GAIN_CONTROL_REGISTER] = 0;
	mt9092_reg[DSP_CONTROL_REGISTER] = 0;
	mt9092_reg[TRANSMIT_AUDIO_GAIN_REGISTER] = 0x30;
	mt9092_reg[TRANSMIT_DTMF_GAIN_REGISTER] = 0x2E;
	mt9092_reg[TONE_COEFFICIENT_REGISTER_1] = 0;
	mt9092_reg[TONE_COEFFICIENT_REGISTER_2] = 0;
	mt9092_reg[TONE_RINGER_WARBLE_RATE_REGISTER] = 0;  
}
