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


#endif
