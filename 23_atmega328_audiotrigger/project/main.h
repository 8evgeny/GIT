#ifndef __MAIN_H
#define __MAIN_H
#include <avr/io.h>
#define F_CPU 16000000UL
#include <util/delay.h>
#include <avr/interrupt.h>
#include "adc.h"
#define led1 PORTC4
#define led2 PORTC5

void GPIO_Init(void);
void USART0_Init(void);
#if 0
char USART_receiveChar(void);
void USART0_Transmit( unsigned char data );
unsigned char USART0_Receive(void);
#endif
void USART_sendChar(char character);
void USART_sendLine(char *string);

void pinON(int);
void pinOFF(int);
void blink(void);
void readEncoderDel1(void);
void readEncoderDel2(void);
void readEncoderHold1(void);
void readEncoderHold2(void);
void readEncoders(void);
void printEncoders(void);
void printADC(void);
enum adc{
    ADC0,
    ADC1,
    ADC2,
    ADC3
};

#endif
