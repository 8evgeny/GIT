
//---------------------------------
// rcc_delay.c
// Плата индикации (MDR32)
//---------------------------------
#include "MDR32Fx.h"
#include "MDR32F9Qx_rst_clk.h"
#include "MDR32F9Qx_port.h"
#include "MDR32F9Qx_timer.h"
#include "MDR32F9Qx_iwdg.h"
//---------------------------------
#include "gpio.h"
#include "main.h"
//---------------------------------
// Настройка тактирования микроконтроллера
//---------------------------------
void RCC_init() {
	RST_CLK_DeInit();	//Сброс настроек
	RST_CLK_HSEconfig(RST_CLK_HSE_ON);			//включение внешнего кварца 8 МГц (CPU_C1_SEL)
	while (RST_CLK_HSEstatus() != SUCCESS);	//ждем включения HSE
	RST_CLK_CPUclkSelectionC1(RST_CLK_CPU_C1srcHSEdiv1);	//мультиплексор CPU_C1: источник тактирования CPU
	RST_CLK_CPU_PLLconfig(RST_CLK_CPU_PLLsrcHSEdiv1, RST_CLK_CPU_PLLmul9); 	//CPU_PLL = 8MHz * 9 = 72 MHz
	RST_CLK_CPU_PLLuse(ENABLE);	//мультиплексор CPU_C2_SEL: используем CPU_PLL для входа CPU_C3_SEL
  RST_CLK_CPU_PLLcmd(ENABLE);	//включаем CPU_PLL
	while (RST_CLK_CPU_PLLstatus() != SUCCESS);	//ждем включения CPU_PLL
	RST_CLK_CPUclkPrescaler(RST_CLK_CPUclkDIV1);	//предделитель CPU_C3_SEL = 1
	RST_CLK_CPUclkSelection(RST_CLK_CPUclkCPU_C3);	//источник тактирования - выход предделителя
	RST_CLK_HSIcmd(DISABLE);// Выключаем HSI	
}
//---------------------------------
// Настройка IWDT
//---------------------------------
void IWDT_init() {
	RST_CLK_LSIcmd(ENABLE);	// включаем LSI
	while (RST_CLK_LSIstatus() != SUCCESS);	//ждем включения LSI
	RST_CLK_PCLKcmd(RST_CLK_PCLK_IWDG, ENABLE);	//включение тактирования
	IWDG_WriteAccessEnable();	//разрешаем доступ к регистрам IWDG
	IWDG_SetPrescaler(IWDG_Prescaler_256);	//устанавливаем предделитель 4, 8, 16 ... 256
	IWDG_SetReload(0x0FFF);	//значение для перезагрузки 0x0000...0x0FFF.
	IWDG_ReloadCounter();		//перезагружаем значение
	IWDG_WriteAccessDisable();	//запрещаем доступ к регистрам IWDG
	IWDG_Enable();	//включаем IWDT	
}
//---------------------------------
// Настройка TIM1
//---------------------------------
void TIM1_init() {
	TIMER_CntInitTypeDef TIM1_InitStructure;
	//Включение тактирования
	RST_CLK_PCLKcmd (RST_CLK_PCLK_TIMER1, ENABLE);
	//делитель входной частоты TIM_CLOCK = 2 (хз, но так работает нормально)
	TIMER_BRGInit(MDR_TIMER1, TIMER_HCLKdiv1);	
	//Настройка TIM1
	TIM1_InitStructure.TIMER_IniCounter = 0;
	TIM1_InitStructure.TIMER_Prescaler = 3600;	//предделитель 3600	
	TIM1_InitStructure.TIMER_Period = 10;	//сравнивается с 1000 (хз, но так работает нормально)
	TIM1_InitStructure.TIMER_CounterMode = TIMER_CntMode_ClkFixedDir;
	TIM1_InitStructure.TIMER_CounterDirection = TIMER_CntDir_Up;
	TIM1_InitStructure.TIMER_EventSource = TIMER_EvSrc_TM1;
	TIM1_InitStructure.TIMER_FilterSampling = TIMER_FDTS_TIMER_CLK_div_1;
	TIM1_InitStructure.TIMER_ARR_UpdateMode = TIMER_ARR_Update_On_CNT_Overflow;
	TIM1_InitStructure.TIMER_ETR_FilterConf = TIMER_Filter_1FF_at_TIMER_CLK;
	TIM1_InitStructure.TIMER_ETR_Prescaler = TIMER_ETR_Prescaler_None;
	TIM1_InitStructure.TIMER_ETR_Polarity = TIMER_ETRPolarity_NonInverted;
	TIM1_InitStructure.TIMER_BRK_Polarity = TIMER_BRKPolarity_NonInverted;
	TIMER_CntInit(MDR_TIMER1, &TIM1_InitStructure);
	TIMER_ITConfig(MDR_TIMER1, TIMER_STATUS_CNT_ARR, ENABLE);
//	NVIC_SetPriority (Timer1_IRQn, 1); // Приоритет прерывания
//	NVIC_EnableIRQ(Timer1_IRQn);
	TIMER_Cmd(MDR_TIMER1, ENABLE);
}
//---------------------------------
void Timer1_IRQHandler() {
	TIMER_ClearFlag(MDR_TIMER1, TIMER_STATUS_CNT_ARR);
	if (delay_dec) delay_dec--;
}
//---------------------------------
// Задержка в миллисекудах
//---------------------------------
void delay_ms(uint32_t ms) {
	delay_dec = ms;
	while (delay_dec) {};
}
/*
//---------------------------------
// Включение SysTick
//---------------------------------
void SysTick_init(void) {
	#define	CLKSOURCE		(1<<2)	//указываем источник синхросигнала: 0 - LSI, 1 - HCLK
	#define TCKINT			(1<<1)	//разрешаем запрос на прерывание от системного таймера
	#define ENABLE			(1<<0)	//разрешаем работу таймера
	SysTick->LOAD = (72000000/1000)-1;
	SysTick->CTRL |= CLKSOURCE|TCKINT|ENABLE;	
}
//---------------------------------
// Прерывание SysTick
//---------------------------------
void SysTick_Handler (void) {
	//if (delay_dec) delay_dec--;
}
*/
