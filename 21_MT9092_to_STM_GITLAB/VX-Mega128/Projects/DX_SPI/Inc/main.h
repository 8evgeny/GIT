#pragma once
#define F_CPU 4000000UL //должно быть раньше #include <util/delay.h>
#include <avr/io.h>
#include <avr/iom128.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdbool.h>
#include <avr/pgmspace.h>
#include "stdio.h"

#define BUAD  9600
#define BRC   ((F_CPU/16/BUAD) - 1)

#define PIN_INT0 PD0 //SPI
#define SPI_MISO 3
#define SPI_MOSI 2
#define SPI_SCK 1
#define SPI_SS 0

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
void SPI_Init(void);
void SPI_WriteByte(uint8_t data);
void SPI_WriteString(char *string);
void SPI_WriteChar(char data);
uint8_t SPI_WriteReadByte(uint8_t writeData);
int main(void);
static void USART_sendChar(char character);
static void USART_sendLine(char *string);
static char USART_receiveChar(void);
//static void Printf(const char* fmt, ...);
static void checkButtons(void);
