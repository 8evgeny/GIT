/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   measurement.h
 * Author: roman.ganaza
 *
 * Created on May 2, 2023, 2:00 PM
 */

#ifndef MEASUREMENT_H
#define MEASUREMENT_H

#include <stdint.h>

#define GND_LEAKAGE_TRESHOLD            730 // ниже этого значения АЦП считаем, что утечка на землю ЕСТЬ
#define SHORT_CIRCUIT_TRESHOLD          30 // Ом, ниже этого импеданса считаем, что КЗ

#define NOT_FORCED                      0
#define FORCED                          1

void measurementInit(void);
uint8_t measLine(uint8_t line);
uint8_t measAllLines(uint8_t forced);
uint8_t calibrLine(uint8_t line);

#endif /* MEASUREMENT_H */
