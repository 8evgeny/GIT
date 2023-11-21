/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   DV_port_SAI_input.h
 * Author: roman.ganaza
 *
 * Created on September 11, 2020, 4:34 PM
 */

#ifndef DV_PORT_SAI_INPUT_H
#define DV_PORT_SAI_INPUT_H

extern unsigned char sync_bit;
extern unsigned char C_channel_out;

#define DV_INPUT_BUFFER_SIZE 4

// комментарий по поводу нумерации байт см в ST-BUS_SAI_in_out.c
#define D_CHANNEL       1
#define C_CHANNEL       0
#define B1_CHANNEL      3
#define B2_CHANNEL      2

void DV_port_input_Init(void);
void SAI_reset(void);

#endif /* DV_PORT_SAI_INPUT_H */

