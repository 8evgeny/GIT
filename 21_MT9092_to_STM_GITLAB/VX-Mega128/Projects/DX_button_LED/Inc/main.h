#pragma once
#include <avr/io.h>
#include <avr/iom128.h>
#define F_CPU 4000000UL
#define BUAD  9600
#define BRC   ((F_CPU/16/BUAD) - 1)
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdbool.h>

bool Buttons[6] = {
    true,   //1 рычаг существует
    true,   //2 рычаг существует
    true,   //3 рычаг существует
    true,   //4 рычаг существует
    true,   //5 рычаг существует
    true    //6 рычаг существует
};

void GPIO_Init(void);
void USART0_Init(void);
bool checkButton(int num);
void setLed(int num);
void resetLed(int num);
void TIMER1_Init (void);
void TIMER3_Init (void);
unsigned char USART0_Receive(void);
void USART_sendChar(char character);
void USART_sendLine(char *string);
char USART_receiveChar(void);
