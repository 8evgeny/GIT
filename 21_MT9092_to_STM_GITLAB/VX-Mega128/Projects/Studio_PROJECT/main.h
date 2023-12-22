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
// ���� ��� ����������� ������ - ��������� � ���������� ������� ����������
#define special_output_port (*((volatile char *)0x24))


bool Buttons[6] = {
    true,   //1 ����� ����������
    true,   //2 ����� ����������
    true,   //3 ����� ����������
    true,   //4 ����� ����������
    true,   //5 ����� ����������
    true    //6 ����� ����������
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
void Printf(const char* fmt, ...);
