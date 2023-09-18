/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   log.h
 * Author: roman.ganaza
 *
 * Created on May 17, 2023, 9:50 AM
 */

#ifndef LOG_H
#define LOG_H

#include <stdint.h>

#define LOG_FILE_SIZE 50000ul // если сделать 100k, то web-сервер уже не запускается

extern char log_file[LOG_FILE_SIZE];

#pragma pack(push, 1) // выравнивание структуры 1 байт (вместо 4-х), так sizeof будет меньше

typedef struct {
    uint8_t line;
    unsigned cal_err:1;
    unsigned imp_err:1;
    unsigned gnd_err:1;
    unsigned sht_err:1;
    unsigned :1;
    unsigned :1;
    unsigned :1;
    unsigned end_marker:1; // маркер конца лога
    float meas;
} tLogRecord;

#pragma pack(pop) // возвращаем назад

void initLog(void);
void makeLogFile(void);
void writeLogRecord(uint8_t line);

#endif /* LOG_H */
