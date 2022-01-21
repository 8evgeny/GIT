
//---------------------------------
// gpio.h
// Плата индикации (MDR32)
//----------------------------------
#include "MDR32Fx.h"
//---------------------------------
#ifndef __GPIO
#define __GPIO
//---------------------------------
// Настройка порта A
//---------------------------------
#define D0					PORT_Pin_0
#define D1					PORT_Pin_1
#define D2					PORT_Pin_2
#define D3					PORT_Pin_3
#define D4					PORT_Pin_4
#define D5					PORT_Pin_5
#define D6					PORT_Pin_6
#define D7					PORT_Pin_7
//---------------------------------
// Настройка порта В
//---------------------------------
#define LED1_ERROR	PORT_Pin_5
#define LED2_REC		PORT_Pin_6
#define BUZZER			PORT_Pin_7
#define RS				PORT_Pin_8
#define RW 				PORT_Pin_9
#define EN 				PORT_Pin_10
//---------------------------------
#endif
