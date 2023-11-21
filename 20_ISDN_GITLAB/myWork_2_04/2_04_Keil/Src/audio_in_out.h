/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   audio_in_out.h
 * Author: Роман
 *
 * Created on 7 января 2021 г., 22:39
 */

#ifndef AUDIO_IN_OUT_H
#define AUDIO_IN_OUT_H

#include <stdint.h>
#include <string.h>

// размер массива с 16-битными элементами: 2 канала (левый/правый)
#define AUDIO_IN_OUT_DMA_BUFFER_SIZE 2

// размеры FIFO на выход кодека и на вход
#define AUDIO_OUT_FIFO_SIZE     128 // степень 2!
#define AUDIO_IN_FIFO_SIZE      128 // степень 2!

extern int16_t sinebuf1[96];

void AudioInOutInit(void);
void AudioOutPush(int16_t sample);
int16_t AudioInPop(void);

#endif /* AUDIO_IN_OUT_H */
