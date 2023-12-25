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
#define stdin (__iob[0])
#define stdout (__iob[1])
#define stderr (__iob[2])
#define EOF (-1)
#define fdev_set_udata(stream, u) do { (stream)->udata = u; } while(0)
#define fdev_get_udata(stream)   ((stream)->udata)
#define fdev_setup_stream(stream, put, get, rwflag)
//#define _FDEV_SETUP_READ __SRD
//#define _FDEV_SETUP_WRITE __SWR
//#define _FDEV_SETUP_RW (__SRD|__SWR)
//#define _FDEV_ERR (-1)
//#define _FDEV_EOF (-2)
//#define FDEV_SETUP_STREAM(put, get, rwflag)
//#define fdev_close()
//#define putc(__c, __stream) fputc(__c, __stream)
//#define putchar(__c) fputc(__c, stdout)
//#define getc(__stream) fgetc(__stream)
//#define getchar() fgetc(stdin)
//#define BUFSIZ 1024
//#define _IONBF 0
//#define fdev_get_udata ( stream )  ((stream)->udata)
//#define fdev_set_udata ( stream,
//                         u
//                       ) do { (stream)->udata = u; } while(0)
//#define fdev_setup_stream ( stream,
//                            put,
//                            get,
//                            rwflag
//                          )
//#define FDEV_SETUP_STREAM  ( put,
//                             get,
//                             rwflag
//                           )
//*#define getc ( __stream ) fgetc(__stream)
//#define getchar ( void ) fgetc(st*/din)
//#define putc ( __c,
//               __stream
//             ) fputc(__c, __stream)
//#define putchar ( __c ) fputc(__c, stdout)

static bool Buttons[6] = {
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
static void checkButtons();
