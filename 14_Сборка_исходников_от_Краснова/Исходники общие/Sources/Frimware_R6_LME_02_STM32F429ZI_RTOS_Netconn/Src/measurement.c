/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   measurement.c
 * Author: roman.ganaza
 * 
 * Created on May 2, 2023, 2:00 PM
 */

#include <stdio.h>

#include "measurement.h"
#include "cmsis_os.h"
#include "cmd_response.h"
#include "global_vars.h"
#include "dbg_uart.h"
#include "eeprom_i2c.h"
#include "indication.h"

osMutexDef(obj_meas_mutex);
osMutexId meas_mutex;

typedef enum {GND_ERR = 0, SHT_ERR, IMP_ERR} lineErr_t;

uint8_t lineHasErrors(uint8_t line);
uint8_t writeOutput(uint8_t* out_data);

void measurementInit(void) {
  uint8_t out_data[NUM_LINES], line;
  
  meas_mutex = osMutexCreate(osMutex(obj_meas_mutex));

  for(line = 0; line < NUM_LINES; line++) {
    if (lineHasErrors(line)) {
      indicationSet(LED_E1 + line, IND_ON);
      out_data[line] = 1;
    } else {
      indicationSet(LED_E1 + line, IND_OFF);
      out_data[line] = 0;
    }
  }
  if (writeOutput(out_data)) puts("Write output OK");
  else puts("Write output ERR");
}

// устанавливает нужную ошибку по линии, инкрементирует счетчик ошибок
void setErrOnLine(uint8_t line, lineErr_t err_type) {
  uint32_t prev;
  prev = err_counter[line];
  
  switch(err_type) {
    case GND_ERR:
      if (gnd_err[line] == 0) {
        gnd_err[line] = 1;
        err_counter[line]++;
      }
      break;
    case SHT_ERR:
      if (sht_err[line] == 0) {
        sht_err[line] = 1;
        err_counter[line]++;
      }
      break;
    case IMP_ERR:
      if (imp_err[line] == 0) {
        imp_err[line] = 1;
        err_counter[line]++;
      }
      break;
  }
  
  if (prev != err_counter[line]) { // сохраняем, если изменился
    eepWriteSeq(EEP_ADDR_err_counter + line*sizeof(err_counter[0]), (unsigned char*) &err_counter[line], sizeof(err_counter[0]));
  }
}

// сбрасывает нужную ошибку по линии
void clrErrOnLine(uint8_t line, lineErr_t err_type) {
  switch (err_type) {
    case GND_ERR:
      gnd_err[line] = 0;
      break;
    case SHT_ERR:
      sht_err[line] = 0;
      break;
    case IMP_ERR:
      imp_err[line] = 0;
      break;
  }
}

// показывает, есть ли ошибки по линии
uint8_t lineHasErrors(uint8_t line) {
  if (gnd_err[line]) return 1;
  if (sht_err[line]) return 1;
  if (imp_err[line]) return 1;
  if (cal_err[line]) return 1;
  return 0;
}

// показывает, остановлено ли тестирование заданной линии с помощью внешнего входа запрета тестирования
uint8_t isLineStopped(uint8_t line) {
  switch(line) {
    case 0:
      return !HAL_GPIO_ReadPin(PS1_GPIO_Port, PS1_Pin);
    case 1:
      return !HAL_GPIO_ReadPin(PS2_GPIO_Port, PS2_Pin);
    case 2:
      return !HAL_GPIO_ReadPin(PS3_GPIO_Port, PS3_Pin);
    case 3:
      return !HAL_GPIO_ReadPin(PS4_GPIO_Port, PS4_Pin);
    case 4:
      return !HAL_GPIO_ReadPin(PS5_GPIO_Port, PS5_Pin);
    case 5:
      return !HAL_GPIO_ReadPin(PS6_GPIO_Port, PS6_Pin);
  }
  return 0;
}

// запускает с помощью команды процедуру измерения в блоке MK, принимает ответ
// функция "честно" измеряет импеданс на заданной чатоте без всякого пересчета,
// а также утечку
// возвращает 1 в случае успеха, 0 - в противном случае
uint8_t measLineRaw(uint8_t line, uint8_t freq, float *imp, uint16_t *leak ) {
  uint32_t tickcount = osKernelSysTick();
  uint8_t res = 0;
  uint8_t ret_value = 0;
  
  if (line >= NUM_LINES) return 0;
  
  cmdMeasLine(line, freq);
  osDelayUntil(&tickcount, 500); // пауза 500 мс - даем поработать ОС, все равно раньше ответ не придет
  
  while (res == 0) {
    res = doReceiveResponse();
    if (res != 0) { // прием ответа в процессе
      
      if (res == 1) { // успех
        
        memcpy((void*) imp, (void*) &resp_data[1], sizeof (imp));
        memcpy((void*) leak, (void*) &resp_data[5], sizeof (leak));

        puts("Answer successfully received!\r");
        ret_value = 1;
        break;
      } else if (res == 2) { // таймаут
        
        puts("Answer TIMEOUT!\r");
        ret_value = 0;
        break;
      }
    }
  }
  return ret_value;
}
// переключает состояние выходов на плате MK
uint8_t writeOutput(uint8_t* out_data) {
  uint8_t res = 0;
  uint8_t ret_value = 0;

//  if (memcmp(MK_out_data, out_data, NUM_LINES) == 0) return 1;
  
  cmdWriteOutput(out_data);

  while (res == 0) {
    res = doReceiveResponse();
    if (res != 0) { // прием ответа в процессе

      if (res == 1) { // успех

        memcpy(MK_out_data, out_data, NUM_LINES);
        puts("Answer successfully received!\r");
        ret_value = 1;
        break;
      } else if (res == 2) { // таймаут

        puts("Answer TIMEOUT!\r");
        ret_value = 0;
        break;
      }
    }
  }
  return ret_value;
}

// подключает/отключает усилители к линиям (например, в случае КЗ)
uint8_t lineControl(uint8_t* out_data) {
  uint8_t res = 0;
  uint8_t ret_value = 0;

  //  if (memcmp(MK_out_data, out_data, NUM_LINES) == 0) return 1;

  cmdLineControl(out_data);

  while (res == 0) {
    res = doReceiveResponse();
    if (res != 0) { // прием ответа в процессе

      if (res == 1) { // успех

        memcpy(line_control_data, out_data, NUM_LINES);
        puts("Answer successfully received!\r");
        ret_value = 1;
        break;
      } else if (res == 2) { // таймаут

        puts("Answer TIMEOUT!\r");
        ret_value = 0;
        break;
      }
    }
  }
  return ret_value;
}
// запускает с помощью команды процедуру измерения в блоке MK, на 80 герцах,
// принимает ответ, потом пересчитывает на 1 кГц
// сохраняет полученный импеданс и утечку в соотв. переменных
// выставляет, если нужно, флаги ошибок, осуществляет индикацию и включение нужных выходов
// возвращает 1 в случае успеха, 0 - в противном случае
uint8_t measLine(uint8_t line) {
  float imp;
  uint16_t leakage;
  uint8_t out_data[NUM_LINES];
  
  indicationSet(LED_E1 + line, IND_BLINK);

  if (osMutexWait(meas_mutex, 4000) != 0) {
    puts("osMutexWait(meas_mutex, 4000) Timeout");
    return 0; // ждем освобождения ресурса, если не дождались, выходим с ошибкой
  } else {
    puts("Mutex aquired");
  }
  
  if (measLineRaw(line, FREQ_NUM_80Hz, &imp, &leakage) == 0) {
    indicationSet(LED_E1 + line, IND_PREV);
    puts("Mutex released");
    osMutexRelease(meas_mutex);
    return 0;
  }
  
  //last_meas[line] = imp * recalc_coef[line]; // пересчитываем на 1 кГц
  last_meas[line] = imp;
  
  printf("Meas. line = %u: ", (uint32_t)line);
  printf("impedance = %6.2f; ", last_meas[line]);
  printf("leakage = %u\r", (uint32_t)leakage);
  
  // обрабатываем результаты
  if (leakage < GND_LEAKAGE_TRESHOLD) { // утечка на землю есть
    setErrOnLine(line, GND_ERR);
    
    // эти ошибки не определяем
    sht_err[line] = ERR_NO_DATA;
    imp_err[line] = ERR_NO_DATA;
  } else { // утечки нет
    clrErrOnLine(line, GND_ERR);
    
    // вычисляем отклонение импеданса
    //discrepancy[line] = (last_meas[line] - ref_impedance_1khz[line]) * 100 / ref_impedance_1khz[line]; // сравниваем с эталоном на 1 кГц
    discrepancy[line] = (last_meas[line] - ref_impedance_80hz[line]) * 100 / ref_impedance_80hz[line];
    
    if (last_meas[line] < SHORT_CIRCUIT_TRESHOLD) { // КЗ есть
      setErrOnLine(line, SHT_ERR);
    } else { // КЗ нету
      clrErrOnLine(line, SHT_ERR);
      
      if ((discrepancy[line] < bot_limit[line]) || (discrepancy[line] > top_limit[line])) { // смотрим отклонение
        setErrOnLine(line, IMP_ERR);
      } else {
        clrErrOnLine(line, IMP_ERR);
      }
    }  
  }


  
  // индикация и управление выходами ошибок на плате коммутации (MK)
  memcpy(out_data, MK_out_data, NUM_LINES);

  if (lineHasErrors(line)) {
    indicationSet(LED_E1 + line, IND_ON);
    out_data[line] = 1;
  } else {
    indicationSet(LED_E1 + line, IND_OFF);
    out_data[line] = 0;
  }
  if (writeOutput(out_data)) puts("Write output OK");
  else puts("Write output ERR");

  // отключаем усилитель от линий, на которых КЗ
  memcpy(out_data, line_control_data, NUM_LINES);
  
  if (sht_err[line]) {
    out_data[line] = 0;
  } else {
    out_data[line] = 1;
  }
  if (lineControl(out_data)) puts("Line control OK");
  else puts("Line control ERR");
  
  writeLogRecord(line);

  puts("Mutex released");
  osMutexRelease(meas_mutex);
  
  return 1;
}
// аналогично measLine, но для всех линий по очереди
// возвращает 1 в случае успеха, 0 - в противном случае
uint8_t measAllLines(uint8_t forced) {
  uint8_t i;
  
  for(i = 0; i < NUM_LINES; i++) {
    if (line_enabled[i]){ // если мониторинг включен
      if (!isLineStopped(i) || forced) { // мониторинг линии не остановлен по внешнему сигналу или, наоборот, имеем внешний сигнал запуска
        if (measLine(i) == 0) return 0; // измеряем
      }
    }
  }
  return 1;
}

// запускает процедуру калибровки, а именно:
// мерит импеданс на 1 килогерце, затем на 80 герцах,
// вычисляет коэффициент пересчета между этими двумя измерениями,
// измерение на 80 Гц просто запоминаем,
// измерение на 1 кГц запоминает как эталонное.
// При последующих измерениях на 80 герцах, будем пересчитывать на 1 кГц и 
// сравнивать с этим эталонным значением.
// возвращает 1 в случае успеха, 0 - в противном случае
uint8_t calibrLine(uint8_t line) {
  float imp80Hz, imp1kHz;
  uint16_t leakage;
  uint8_t out_data[NUM_LINES];

  indicationSet(LED_E1 + line, IND_BLINK);
  
  if (measLineRaw(line, FREQ_NUM_80Hz, &imp80Hz, &leakage) == 0) {
    indicationSet(LED_E1 + line, IND_PREV);
    return 0;
  }
  
  if (measLineRaw(line, FREQ_NUM_1kHz, &imp1kHz, &leakage) == 0) {
    indicationSet(LED_E1 + line, IND_PREV);
    return 0;
  }
  
  //recalc_coef[line] = imp1kHz/imp80Hz;
  //eepWriteSeq(EEP_ADDR_recalc_coef + line * sizeof (recalc_coef[0]), (unsigned char*) &recalc_coef[line], sizeof (recalc_coef[0]));
  
  ref_impedance_80hz[line] = imp80Hz;
  eepWriteSeq(EEP_ADDR_ref_impedance_80hz + line*sizeof(ref_impedance_80hz[0]), (unsigned char*) &ref_impedance_80hz[line], sizeof(ref_impedance_80hz[0]));
  
  ref_impedance_1khz[line] = imp1kHz;
  eepWriteSeq(EEP_ADDR_ref_impedance_1khz + line*sizeof(ref_impedance_1khz[0]), (unsigned char*) &ref_impedance_1khz[line], sizeof(ref_impedance_1khz[0]));
  
  cal_err[line] = 0; // сбрасываем ошибку калибровки
  imp_err[line] = 0; // и импеданса
  eepWriteSeq(EEP_ADDR_cal_err + line*sizeof(cal_err[0]), (unsigned char*) &cal_err[line], sizeof(cal_err[0]));

  if (lineHasErrors(line)) {
    indicationSet(LED_E1 + line, IND_ON);
    out_data[line] = 1;
  } else {
    indicationSet(LED_E1 + line, IND_OFF);
    out_data[line] = 0;
  }
  if (writeOutput(out_data)) puts("Write output OK");
  else puts("Write output ERR");
  return 1;
}
