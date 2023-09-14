/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   log.c
 * Author: roman.ganaza
 * 
 * Created on May 17, 2023, 9:50 AM
 */

#include "log.h"
#include <stdio.h>
#include <string.h>
#include "global_vars.h"
#include "eeprom_i2c.h"

uint32_t log_index = 0; // локальный адрес (без учета смещения) текущей записи лога (всегда указывает на ячейку из LOG_RECORD_SIZE байт с маркером конца)

char log_file[LOG_FILE_SIZE];

void initLog(void) {
//  uint32_t i;
  tLogRecord tmp = {0};
  
// находим конец лога
  puts("---------------------Find log_end---------------------------");
  for(log_index = 0; log_index < LOG_SIZE; log_index += LOG_RECORD_SIZE) {
    eepReadSeq(EEP_ADDR_LOG + log_index, (unsigned char*) &tmp, sizeof (tmp));
    if (tmp.end_marker) break;
  }
  if (log_index >= LOG_SIZE) {
    log_index = 0; // если не найден маркер, обнуляем индекс
    printf("ERROR: End of Log not found! log_index reset to 0\r");
  }
  
  printf("log_index = %u, record number(from 0) = %u\r", log_index, log_index/LOG_RECORD_SIZE);
  puts("---------------------Log_end found--------------------------");
  
//  puts("---------------------LOG BEGIN---------------------------");
  
//  memset(cal_err, 0, sizeof(cal_err));
//  memset(gnd_err, 0, sizeof(gnd_err));
//  memset(sht_err, 0, sizeof(sht_err));
//  memset(imp_err, 0, sizeof(imp_err));
//  
//  for(i = 0; i < LOG_NUM_RECORDS + 2; i++) {
//    
//    cal_err[i] = 1;
//    gnd_err[i] = 1;
//    imp_err[i] = 1;
//    sht_err[i] = 1;
//    
//    last_meas[i] = i;
//    
//    writeLogRecord(i % NUM_LINES);
//  }
  
//  for(i = 0; i < LOG_NUM_RECORDS; i++) {
//    eepReadSeq(EEP_ADDR_LOG + i * LOG_RECORD_SIZE, (unsigned char*)&tmp, sizeof(tmp));
//    printf("Record %u, line %u, cal_err = %u gnd_err = %u sht_err = %u imp_err = %u meas=%6.2f", i, tmp.line, tmp.cal_err, tmp.gnd_err, tmp.sht_err, tmp.imp_err, tmp.meas);
//    printf("\r");
//  }
//  
//  puts("---------------------LOG END---------------------------");
}

void makeLogFile(void) {
  uint32_t i;
  uint8_t flag_err;
  tLogRecord tmp = {0};
  uint32_t length = 0;

  i = log_index + LOG_RECORD_SIZE;

  length += snprintf(log_file+length, LOG_FILE_SIZE-length, "--------------------LOG FILE BEGIN------------------------\r");

  while (i != log_index) {
    eepReadSeq(EEP_ADDR_LOG + i, (unsigned char*) &tmp, sizeof (tmp)); // читаем запись

    if (!tmp.end_marker) { // пустые ячейки пропускаем
      
      length += snprintf(log_file+length, LOG_FILE_SIZE-length, "L%u:", tmp.line + 1);

      flag_err = 0;
      if (tmp.cal_err) {
        length += snprintf(log_file+length, LOG_FILE_SIZE-length, " CAL_ERR");
        flag_err = 1;
      }
      if (tmp.imp_err) {
        if (flag_err) length += snprintf(log_file+length, LOG_FILE_SIZE-length, ",");
        length += snprintf(log_file+length, LOG_FILE_SIZE-length, " IMP_ERR");
        flag_err = 1;
      }
      if (tmp.gnd_err) {
        if (flag_err) length += snprintf(log_file+length, LOG_FILE_SIZE-length, ",");
        length += snprintf(log_file+length, LOG_FILE_SIZE-length, " GND_ERR");
        flag_err = 1;
      }
      if (tmp.sht_err) {
        if (flag_err) length += snprintf(log_file+length, LOG_FILE_SIZE-length, ",");
        length += snprintf(log_file+length, LOG_FILE_SIZE-length, " SHT_ERR");
        flag_err = 1;
      }

      if (flag_err == 0) length += snprintf(log_file+length, LOG_FILE_SIZE-length, " OK");

      length += snprintf(log_file+length, LOG_FILE_SIZE-length, ", Z = %.2f\r", tmp.meas);
    }
    i += LOG_RECORD_SIZE;
    i %= LOG_SIZE;
  }

  length += snprintf(log_file+length, LOG_FILE_SIZE-length, "---------------------LOG FILE END-------------------------");
}

void writeLogRecord(uint8_t line) {
  tLogRecord log_rec = {0};

  log_rec.end_marker = 1;
  eepWriteSeq(EEP_ADDR_LOG + (log_index + LOG_RECORD_SIZE) % LOG_SIZE, (unsigned char*)&log_rec, sizeof(log_rec)); //  переставлем маркер конца
  
  log_rec.end_marker = 0;
  log_rec.line = line;
  log_rec.cal_err = cal_err[line];
  log_rec.imp_err = imp_err[line];
  log_rec.gnd_err = gnd_err[line];
  log_rec.sht_err = sht_err[line];
  log_rec.meas = last_meas[line];
  printf("Write LOG record %u, line %u\r", log_index/LOG_RECORD_SIZE, line);

  eepWriteSeq(EEP_ADDR_LOG + log_index, (unsigned char*)&log_rec, sizeof(log_rec)); // пишем запись в лог
  
  log_index += LOG_RECORD_SIZE;
  log_index %= LOG_SIZE;
}
