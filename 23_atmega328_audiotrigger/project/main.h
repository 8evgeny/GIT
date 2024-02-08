#ifndef __MAIN_H
#define __MAIN_H
#include <avr/io.h>
//#include <avr/iom128.h>
#define F_CPU 16000000UL
#include <util/delay.h>
#include <avr/interrupt.h>
#define led1 PORTC4
#define led2 PORTC5

static void GPIO_Init(void);
static void pinON(int);
static void pinOFF(int);
static void blink(void);
typedef struct encoder{
    int in;
    int out0;
    int out1;
    int out2;
    int out3
}ENCODER;
static void initEncoder(ENCODER * enc, int in, int out0, int out1, int out2, int out3);
static void init(void);
static uint8_t readEncoder(ENCODER * enc);
static void read(void);
static void initComparator(void);
void USART0_Init(void);
void USART0_Transmit( unsigned char data );
unsigned char USART0_Receive(void);
void USART_sendChar(char character);
void USART_sendLine(char *string);
char USART_receiveChar(void);
static void testBlink(void);
#endif
