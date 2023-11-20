/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   tastature.h
 * Author: roman.ganaza
 *
 * Created on June 28, 2023, 1:13 PM
 */

#ifndef TASTATURE_H
#define TASTATURE_H

#define HANDSET_KEYPAD_NUMBER       'D' // 13
#define MAX_KEYPADS                  9 // максимальное число подключенных кейпадов

typedef enum {IND_MANUAL, IND_BLINK_500 = 500u, IND_BLINK_1000 = 1000u, IND_BLINK_UNPROGRAMMED} tIndMode;

#define BLINK_UNPROGRAMMED_PERIOD 500u

extern char key_event_str[];
extern uint8_t handset_is_off; // 1 - трубка снята, 0 - трубка повешена
extern uint8_t led_test_enabled; // флаг включения тестирования светодиодов

void initTastature(void);
uint8_t doTastature(void);
void tastCmdLeds(char *cmd);
void tastSetIndication(tIndMode mode);
void tastLedTestOn(void);
void tastLedTestOff(void);
void handsetButtonsService(void);

#endif /* TASTATURE_H */
