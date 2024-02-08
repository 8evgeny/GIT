#ifndef __MAIN_H
#define __MAIN_H
#include <avr/io.h>
#define F_CPU 16000000UL
#include <util/delay.h>
#include <avr/interrupt.h>
#include "adc.h"
#define led1 PORTC4
#define led2 PORTC5

static void GPIO_Init(void);
static void USART0_Init(void);
static void USART0_Transmit( unsigned char data );
static unsigned char USART0_Receive(void);
static void USART_sendChar(char character);
static void USART_sendLine(char *string);
static char USART_receiveChar(void);
static void pinON(int);
static void pinOFF(int);
static void blink(void);
static void readEncoderDel1(void);
static void readEncoderDel2(void);
static void readEncoderHold1(void);
static void readEncoderHold2(void);
static void readEncoders(void);
static void printEncoders(void);
static void printADC(void);



#endif
