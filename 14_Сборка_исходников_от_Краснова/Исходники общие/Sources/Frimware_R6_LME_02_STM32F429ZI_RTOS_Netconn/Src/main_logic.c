/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   main_logic.c
 * Author: roman.ganaza
 * 
 * Created on February 22, 2023, 12:56 PM
 */

#include <string.h>
#include "cmsis_os.h"
#include "main_logic.h"
#include "dbg_uart.h"
#include "encryption.h"
#include "eeprom_i2c.h"
#include "global_vars.h"
#include "measurement.h"
#include "cmd_response.h"
#include "indication.h"
#include "log.h"

extern osTimerId timer1secHandle;

void makeLoginPassword(char *login_password, char *login, char* password) {
  	// делаем строку вида "login:password"
	strcpy(login_password, login);
	strcat(login_password, ":");
	strcat(login_password, password);
}

void initMainLogic(void) {

  makeLoginPassword(login_password, login, password); // делаем пару логин-пароль
  encode(login_password_hash, login_password); // кодируем её
	
  printf(login_password);
	printf(" -> ");
	
  printf("%s\r", login_password_hash);
  
  initLog();
}

void doForceStartPolling(void) { // опрос вывода принудительного старта измерения (в прерывании 1 мс)
  uint8_t pin_val;
  static uint8_t i = 0, st = 0;
  
  pin_val = !HAL_GPIO_ReadPin(START_GPIO_Port, START_Pin);
  
  if (pin_val == 1) {
    if (st != 1) {
      i++;
      if (i == 30) {
        st = 1;
        flag_force_start = 1;
      }
    }
  } else {
    i = 0;
    st = 0;
  }
  
}

void ThreadMainLogic(void const * argument) {
  uint32_t i;
  
	dbgUartPuts("Start ThreadMainLogic.\r");
	initMainLogic();
  
  for(i = 0; i < NUMBER_OF_LEDS; i++) indicationSet(i, IND_ON);
  osDelay(1000);
  for (i = 0; i < NUMBER_OF_LEDS; i++) indicationSet(i, IND_OFF);
  indicationSet(LED_ON, IND_ON);
  
  measurementInit();
  osTimerStart(timer1secHandle, 1000);
  
  for(;;)
  {
    if (flag_force_start) { // принудительный запуск измерения
      measAllLines(FORCED);
      flag_force_start = 0;
    }
    osDelay(10);
  }
}

void tick1sec(void const * argument) { // вызывается RTOS в системной задаче osTimerThread() с периодичностью в 1 секунду
  static uint32_t meas_interval_cnt = 0; // счетчик для отсчета интервалов измерения
  static uint32_t cnt15sec = 0;
  
  HAL_GPIO_TogglePin(LED_0_GPIO_Port, LED_0_Pin);
  
  meas_interval_cnt++;
  cnt15sec++;
  
  if (cnt15sec >= 15) {
    cnt15sec = 0;
    printf("\rTimer 1 sec: <-- %u -->\r", meas_interval_cnt);
  }

  if (meas_interval_cnt >= meas_interval) {
    meas_interval_cnt = 0;
    
    measAllLines(NOT_FORCED);
  }  
}

void defaultTaskMainLoop(void) { // крутится в цикле задачи default task, с периодичностью ~10 мс
  static int32_t but_cnt;
  static uint8_t err, prev_err = 0xFF;
  uint32_t tickcount = osKernelSysTick();
  
  if (HAL_GPIO_ReadPin(BUT_GPIO_Port, BUT_Pin) == 0) { // опрос кнопки сброса
    but_cnt++;
    if (but_cnt >= 1000u) {
      eepWriteDefaults();
      HAL_Delay(100);
      NVIC_SystemReset();
    }
  } else {
    but_cnt = 0;
  }
  
  if (flag_reset_to_defaults) {
    flag_reset_to_defaults = 0;
    eepWriteDefaults();
    osDelay(200);
    NVIC_SystemReset();
  }
  
  err = err_no_conn_with_MK_board + err_eeprom_fail;
  if (err != prev_err) {
    if (err) indicationSet(LED_ERR, IND_ON); else indicationSet(LED_ERR, IND_OFF);
  }
  prev_err = err;
  
  osDelayUntil(&tickcount, 10); // пауза 10 мс
}

void initEepVars(void) {
  uint32_t e;
  
  dbgUartPuts("\r---->EEPROM Init Result: ");
  e = eepInit();
  if (e == HAL_I2C_ERROR_NONE) {
    uint8_t test_byte;
    dbgUartPuts("OK\r");
    printf("EEPROM bytes used for variables: %u\r", EEP_ADDR_TEST_BYTE + 1);
    printf("Log size: %u\r", LOG_SIZE);
    eepReadSeq(EEP_ADDR_TEST_BYTE, (unsigned char*) &test_byte, sizeof (test_byte));
    if (test_byte != TEST_BYTE) {
      eepWriteDefaults();
    }

    eepLoadVars();
  } else {
    dbgUartPuts("ERROR ");
    HexToChar(str, e);
    dbgUartPuts(str);
    dbgUartPuts("\r");
    err_eeprom_fail = 1;
  }
}

