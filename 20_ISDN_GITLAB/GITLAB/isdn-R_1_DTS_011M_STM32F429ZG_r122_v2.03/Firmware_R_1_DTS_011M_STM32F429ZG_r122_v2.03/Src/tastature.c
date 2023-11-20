/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   tastature.c
 * Author: roman.ganaza
 * 
 * Created on June 28, 2023, 1:13 PM
 */

#include "main.h"
#include "tastature.h"
#include "uart.h"
#include "dbg_uart.h"
#include "FIFO.h"
#include "main_logic.h"

#define MAX_TAST_RESP_LEN       16

char key_event_str[MAX_TAST_RESP_LEN];
static char t_str[MAX_TAST_RESP_LEN];
static tIndMode ind_mode = IND_MANUAL;
static uint32_t key_tmr, ind_tmr;
static uint8_t ind_blink = 0; // для мигания светодиодов на тастатуре
static uint8_t keypad_present[MAX_KEYPADS]; // наличие (1) или отсутствие соответствующего кейпада

FIFO(512) tast_cmd_fifo; // FIFO для того, чтобы впихивать команды между опросами кнопок
FIFO(64) handset_evnt_fifo; // очередь событий от нажатия/отжатия кнопки и снятия/повешения трубки
uint8_t handset_is_off = 0; // 1 - трубка снята, 0 - трубка повешена
uint8_t led_test_enabled = 0; // флаг включения тестирования светодиодов

#define EVNT_HANDSET_BUT_PRESSED     0x82 // событие: кнопка на трубке нажата
#define EVNT_HANDSET_BUT_UNPRESSED   0x02 // событие: кнопка на трубке отжата
#define EVNT_HANDSET_HUNG_UP    0x03 // событие: повесили трубку
#define EVNT_HANDSET_IS_OFF  0x83 // событие: сняли трубку

uint8_t tastCheckPresence(char keypad);
uint8_t haveWorkingKeypad(void);

void initTastature(void) {
  uint8_t i;
  
  uartInit();
  
  HAL_Delay(1500); // клавиатура медленно стартует, 500 мс уже мало
  
  uartPuts("R1\r");
  HAL_Delay(50);
  uartPuts("C?z\r");
  HAL_Delay(50);
  uartPuts("P1\r");
  HAL_Delay(50);
  
  uartPuts("C?z\r");
  HAL_Delay(50);
  
  puts("Keypads Init...");
  
  for(i = 0; i < MAX_KEYPADS; i++) {
    keypad_present[i] = tastCheckPresence('1' + i);
    HAL_Delay(50);
  }
  
  if (!haveWorkingKeypad()) keypad_present[0] = 1; // если нет ни одного клавишного блока, будем опрашивать 1-й до посинения
}

uint8_t haveWorkingKeypad(void) { // возвращает 1, если есть хотя бы один кейпад
  uint8_t i;
  
  for(i = 0; i < MAX_KEYPADS; i++) {
    if (keypad_present[i] != 0) return 1;
  }
  return 0;
}

void tastCmdReadKeys(void) {
  static uint8_t current_keypad = 0;

  uartPutchar('P');
  uartPutchar('1' + current_keypad);
  uartPutchar('\r');
  
  do {
    current_keypad++;
    if (current_keypad >= MAX_KEYPADS) current_keypad = 0;
  } while(keypad_present[current_keypad] == 0);
}

void tastCmdLeds(char *cmd) {
  while (*cmd) {
    FIFO_PUSH(tast_cmd_fifo, *cmd);
    cmd++;
  }
  FIFO_PUSH(tast_cmd_fifo, '\r');
}

void tastSetIndication(tIndMode mode) {
  ind_mode = mode;
  
  if ((mode != IND_MANUAL)&&(mode != IND_BLINK_UNPROGRAMMED)) {
    tastCmdLeds("L?001");
    ind_blink = 1;
    ind_tmr = HAL_GetTick() + mode;
  } else if (mode == IND_BLINK_UNPROGRAMMED) {
    tastCmdLeds("L?000");
    tastCmdLeds("L1011");
    ind_blink = 1;
    ind_tmr = HAL_GetTick() + BLINK_UNPROGRAMMED_PERIOD;
  }
}
// включает тестирование светодиодов: зажигает все светодиоды на всех кейпадах,
// на последующие команды управления светодиодами после этого не реагируем
void tastLedTestOn(void) {
  
  led_test_enabled = 1;
  
  puts("LED TEST ON\r");
  
  // включаем все светодиоды на всех кейпадах
  tastSetIndication(IND_MANUAL);
  tastCmdLeds("L?001");
}

// выключает тестирование светодиодов: гасит все светодиоды на всех кейпадах,
void tastLedTestOff(void) {

  led_test_enabled = 0;

  puts("LED TEST OFF\r");

  // гасим все светодиоды на всех кейпадах
  tastSetIndication(IND_MANUAL);
  tastCmdLeds("L?000");
}
// опрос клавиатуры и индикация на клавиатуре,
// возвращает:
// 1, если было нажатие/отжатие клавиши
// 0 - в противном случае
uint8_t doTastature(void) { 
  static uint8_t j = 0;
  uint8_t result;
  
  result = 0;
  
  if (ind_mode != IND_BLINK_UNPROGRAMMED) { // если незапрограммированый порт, то не опрашиваем
    
    if (FIFO_COUNT(handset_evnt_fifo) > 0) { // событие от трубки, у событий трубки приоритет, просто для того, чтобы не было коллизий с опросом клавишных блоков
      char evnt;
      evnt = FIFO_POP(handset_evnt_fifo);
      
      if (flag_abon_registered) {
        if (evnt & 0x80) {
          key_event_str[0] = 'K';
          puts("Handset button pressed:");
        } else {
          key_event_str[0] = 'k';
          puts("Handset button unpressed:");
        }
        key_event_str[1] = HANDSET_KEYPAD_NUMBER; // номер кейпада для кнопки и рычага трубки

        sprintf(&key_event_str[2], "%02u", evnt & 0x7F);   
        puts(key_event_str);
        
        result = 1;
      }
    } else {
    
      if (HAL_GetTick() >= key_tmr) {
        if (FIFO_COUNT(tast_cmd_fifo) == 0) {
          if (!uartRS485TxBusy() && flag_abon_registered) {
            key_tmr = HAL_GetTick() + 50;
            tastCmdReadKeys();
          }
        } else { // если в буфере что-то есть, передаем
          while(FIFO_COUNT(tast_cmd_fifo)) {
            char c;
            c = FIFO_POP(tast_cmd_fifo);
            uartPutchar(c);
          }
        }
      }

      if (uartBytesReceived()) {
        t_str[j] = uartGetchar();
        if (t_str[j] == '\r') {
          t_str[j] = '\0';
          j = 0;
          if (t_str[0] == 'K') {
            puts("Key pressed:");
            puts(t_str);
            strcpy(key_event_str, t_str);
            result = 1;
          }
          if (t_str[0] == 'k') {
            puts("Key unpressed:");
            puts(t_str);
            strcpy(key_event_str, t_str);
            result = 1;
          }
          key_tmr = HAL_GetTick(); // сразу следующий запрос, не дожидаясь таймаута      
        } else {
          j++;
        }
        if (j >= MAX_TAST_RESP_LEN-1) j = 0;
      }
    } // end if (FIFO_COUNT(handset_evnt_fifo) > 0) {
  } else {
    if (FIFO_COUNT(tast_cmd_fifo) != 0) { // если в буфере что-то есть, передаем
      while (FIFO_COUNT(tast_cmd_fifo)) {
        char c;
        c = FIFO_POP(tast_cmd_fifo);
        uartPutchar(c);
      }
    }
    
    if (uartBytesReceived()) { // просто, чтобы вдруг не переполнился буфер
      uartGetchar();
    }
    
    // трубку тоже не обрабатываем
    if (FIFO_COUNT(handset_evnt_fifo) > 0) { // чистим буфер
        FIFO_POP(tast_cmd_fifo);
    }
  }
  
// индикация 
  switch(ind_mode) {
    case IND_MANUAL:
      
      break;
    case IND_BLINK_500: case IND_BLINK_1000:
      if (HAL_GetTick() >= ind_tmr) {
        ind_tmr = HAL_GetTick() + ind_mode; // в ind_mode как раз длительность импульса
        tastCmdLeds(ind_blink? "L?000" : "L?001"); // мигаем всеми светодиодами (адрес 00) на всех кейпадах ("?")
        ind_blink ^= 1;
      }
      break;
    case IND_BLINK_UNPROGRAMMED: // мигаем как ЖД семафор 1 и 2 светодиодом
      if (HAL_GetTick() >= ind_tmr) {
        ind_tmr = HAL_GetTick() + BLINK_UNPROGRAMMED_PERIOD;
        if (ind_blink) {
          tastCmdLeds("L?000");
          tastCmdLeds("L1021");
        } else {
          tastCmdLeds("L?000");
          tastCmdLeds("L1011");
        }
        ind_blink ^= 1;
      }
      break;
  }
  
  return result;
}

void handsetButtonsService(void) { // опрос кнопки и рычага трубки (вызывается в прерывании каждую 1 мс)
  uint8_t st_but, st_hook;
  static uint8_t prev_st_but = 0xFF, prev_st_hook = 0xFF;
  static uint8_t debounce1, debounce2;
  const uint8_t deb_coef = 5; // антидребезг, мс
  
  if (handset_support_en && handset_poll_en) {
    st_but = READ_TUBE_BUT();
    if (prev_st_but != st_but) {
      debounce1++;
      if (debounce1 >= deb_coef) {
        debounce1 = 0;
        prev_st_but = st_but;
        if (!FIFO_IS_FULL(handset_evnt_fifo)) {
          if (st_but == 0) FIFO_PUSH(handset_evnt_fifo, EVNT_HANDSET_BUT_PRESSED);
          else FIFO_PUSH(handset_evnt_fifo, EVNT_HANDSET_BUT_UNPRESSED);
        }
      }
    } else {
      debounce1 = 0;
    }

    if (handset_hook_inverse) {
      st_hook = !READ_TUBE_HOOK();
    } else {
      st_hook = READ_TUBE_HOOK();
    }
    if (prev_st_hook != st_hook) {
      debounce2++;
      if (debounce2 >= deb_coef) {
        debounce2 = 0;
        prev_st_hook = st_hook;
        if (!FIFO_IS_FULL(handset_evnt_fifo)) {
          if (st_hook == 0) FIFO_PUSH(handset_evnt_fifo, EVNT_HANDSET_HUNG_UP);
          else FIFO_PUSH(handset_evnt_fifo, EVNT_HANDSET_IS_OFF);
        }
        handset_is_off = st_hook;
      }
    } else {
      debounce2 = 0;
    }
  }
}
// проверяет, подключена ли клавиатура с номером keypad (в ASCII)
// возвращает 1, если подключена
uint8_t tastCheckPresence(char keypad) {
  uint32_t timeout;
  uint8_t j = 0;
  
  timeout = HAL_GetTick() + 100;
  
  uartRxFifoFlush();
  
  uartPutchar('P');
  uartPutchar(keypad);
  uartPutchar('\r');
  
  while (HAL_GetTick() <= timeout) {
    
    if (uartBytesReceived()) {
      t_str[j] = uartGetchar();
      if (t_str[j] == '\r') { // в принципе, этого достаточно, чтобы понять, что клавиатруа отвечает
        printf("keypad %c found!\r", keypad);
        return 1;
      } else {
        j++;
      }
      if (j >= MAX_TAST_RESP_LEN - 1) { // заглушка от переполнения
        j = 0;
        puts("Error: MAX_TAST_RESP_LEN overflow!");
        return 0;
      }
    }
  }

  printf("keypad %c not found!\r", keypad);
  return 0;
}
