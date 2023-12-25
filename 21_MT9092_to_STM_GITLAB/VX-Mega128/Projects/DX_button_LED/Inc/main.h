#pragma once
#include <avr/io.h>
#include <avr/iom128.h>
#define F_CPU 4000000UL
#define BUAD  9600
#define BRC   ((F_CPU/16/BUAD) - 1)
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdbool.h>
#include <avr/pgmspace.h>
#include "stdio.h"
// порт для отладочного вывода - определен в параметрах запуска симулятора
#define special_output_port (*((volatile char *)0x24))


bool Buttons[6] = {
    true,   //1 рычаг существует
    true,   //2 рычаг существует
    true,   //3 рычаг существует
    true,   //4 рычаг существует
    true,   //5 рычаг существует
    true    //6 рычаг существует
};

static void GPIO_Init(void);
static void USART0_Init(void);
static bool checkButton(int num);
static void setLed(int num);
static void resetLed(int num);
static void TIMER1_Init (void);
static void TIMER3_Init (void);
static unsigned char USART0_Receive(void);
static void USART_sendChar(char character);
static void USART_sendLine(char *string);
static char USART_receiveChar(void);
static void Printf(const char* fmt, ...);
