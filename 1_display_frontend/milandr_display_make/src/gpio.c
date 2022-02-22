
//---------------------------------
// gpio.c
// Плата индикации (MDR32)
//---------------------------------
#include "MDR32Fx.h"
#include "MDR32F9Qx_rst_clk.h"
#include "MDR32F9Qx_port.h"
//---------------------------------
#include "gpio.h"
#include "main.h"
//---------------------------------
// Конфигурация выводов
//---------------------------------
void GPIO_init() {
	PORT_InitTypeDef GPIO_InitStructure;
	//Включение тактирования
	RST_CLK_PCLKcmd(RST_CLK_PCLK_PORTA, ENABLE);
	RST_CLK_PCLKcmd(RST_CLK_PCLK_PORTB, ENABLE);
	
	//Настройка выводов порта A
	GPIO_InitStructure.PORT_Pin = (D0|D1|D2|D3|D4|D5|D6|D7);
	GPIO_InitStructure.PORT_OE = PORT_OE_OUT; 
	GPIO_InitStructure.PORT_PULL_UP	= PORT_PULL_UP_ON;
	GPIO_InitStructure.PORT_PULL_DOWN = PORT_PULL_DOWN_OFF;
	GPIO_InitStructure.PORT_PD_SHM = PORT_PD_SHM_OFF; 
	GPIO_InitStructure.PORT_PD = PORT_PD_DRIVER;
	GPIO_InitStructure.PORT_GFEN = PORT_GFEN_OFF;
	GPIO_InitStructure.PORT_FUNC = PORT_FUNC_PORT;
	GPIO_InitStructure.PORT_SPEED = PORT_SPEED_MAXFAST;
	GPIO_InitStructure.PORT_MODE = PORT_MODE_DIGITAL;
	PORT_Init(MDR_PORTA, &GPIO_InitStructure);
	
	//Настройка выводов порта B
	GPIO_InitStructure.PORT_Pin = (LED1_ERROR|LED2_REC|BUZZER|RS|RW|EN);
	GPIO_InitStructure.PORT_OE = PORT_OE_OUT; 
	GPIO_InitStructure.PORT_PULL_UP	= PORT_PULL_UP_ON;
	GPIO_InitStructure.PORT_PULL_DOWN = PORT_PULL_DOWN_OFF;
	GPIO_InitStructure.PORT_PD_SHM = PORT_PD_SHM_OFF; 
	GPIO_InitStructure.PORT_PD = PORT_PD_DRIVER;
	GPIO_InitStructure.PORT_GFEN = PORT_GFEN_OFF;
	GPIO_InitStructure.PORT_FUNC = PORT_FUNC_PORT;
	GPIO_InitStructure.PORT_SPEED = PORT_SPEED_MAXFAST;
	GPIO_InitStructure.PORT_MODE = PORT_MODE_DIGITAL;
	PORT_Init(MDR_PORTB, &GPIO_InitStructure);
}
//---------------------------------
