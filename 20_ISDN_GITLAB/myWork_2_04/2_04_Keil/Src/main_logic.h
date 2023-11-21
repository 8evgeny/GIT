/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   main_logic.h
 * Author: roman.ganaza
 *
 * Created on June 22, 2023, 2:06 PM
 */

#ifndef MAIN_LOGIC_H
#define MAIN_LOGIC_H

#include <stdint.h>

typedef enum {SOUND_CH_B1 = 1, SOUND_CH_B2} tSoundCh;
typedef enum {ABON_ST_NOT_CONNECTED = 0, ABON_ST_CONNECTED = 2, ABON_ST_UNPROGRAMMED = 4} tAbonSt;

extern uint8_t flag_abon_registered;
extern tSoundCh sound_channel; // в каком канале ЗВУК (B1 или B2)
extern uint8_t loud_speaker_en; // вкл/выкл динамик громкой связи
extern uint8_t handset_speaker_en; // вкл/выкл динамик трубки
extern uint8_t handset_support_en; // поддержка трубки/гарнитуры (включается соответствующей телеграммой)
extern uint8_t handset_poll_en; // включает опрос кнопки и рычага трубки
extern uint8_t handset_hook_inverse; // инверсия сигнала с рычага трубки
extern uint8_t flag_sound_en; // если установлен в 0, то входной звук с линии заменяется на 0

void initMainLogic(void);
void doMainLogic(void);

#endif /* MAIN_LOGIC_H */
