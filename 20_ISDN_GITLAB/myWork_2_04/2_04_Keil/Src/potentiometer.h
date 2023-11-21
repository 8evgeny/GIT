/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   potentiometer.h
 * Author: roman.ganaza
 *
 * Created on May 23, 2023, 4:05 PM
 */

#ifndef POTENTIOMETER_H
#define POTENTIOMETER_H

#include <stdint.h>

#define POT_MIC     0 // громкость микрофона громкой связи
#define POT_VOL     1 // громкость динамика громкой связи
#define POT_HMIC    3 // громкость микрофона гарнитуры/трубки
#define POT_HLT     2 // громкость динамика гарнитуры/трубки

void potInit(void);
uint32_t getPotValue(uint8_t pot);

#endif /* POTENTIOMETER_H */
