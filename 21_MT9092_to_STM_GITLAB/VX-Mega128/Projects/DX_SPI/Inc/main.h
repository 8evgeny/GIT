#pragma once
#include <avr/io.h>
#include <avr/iom128.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdbool.h>
#include <avr/pgmspace.h>
#include "stdio.h"
#define F_CPU 4000000UL
#define BUAD  9600
#define BRC   ((F_CPU/16/BUAD) - 1)

static bool Buttons[6] = {
    true,   //1 рычаг существует
    true,   //2 рычаг существует
    true,   //3 рычаг существует
    true,   //4 рычаг существует
    true,   //5 рычаг существует
    true    //6 рычаг существует
};
static int uart_putchar(char c, FILE *stream);
static FILE mystdout = FDEV_SETUP_STREAM(
    uart_putchar,     // функция вывода символа
    NULL,           // функция ввода символа, нам сейчас не нужна
    _FDEV_SETUP_WRITE // флаги потока - только вывод
    );
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
static void checkButtons();
