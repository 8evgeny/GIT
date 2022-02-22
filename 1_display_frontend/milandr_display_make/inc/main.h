//---------------------------------
// main.h
// Плата индикации (MDR32)
//---------------------------------

#ifdef __cplusplus
extern "C" {
#endif

#include "MDR32Fx.h"
#include "MDR32F9Qx_rst_clk.h"
#include "MDR32F9Qx_port.h"
//---------------------------------
#include "gpio.h"
//---------------------------------
#ifndef _MAIN
#define _MAIN
//---------------------------------
// Прототипы функций
//---------------------------------
void RCC_init(void);
void IWDT_init(void);
void GPIO_init(void);
void MAIN_logic(void);
void START_logic(void);
//---------------------------------
//void SysTick_init(void);
//void SysTick_Handler(void);
void delay_ms(uint32_t TEMP);
void TIM1_init(void);
//---------------------------------
void Setup_CPU_Clock(void);
void Setup_USB(void);
void VCom_Configuration(void);
//---------------------------------
void LCD_init(void);
void LCD_clear_display(void);
void LCD_return_home(void);
void LCD_entry_mode(uint8_t ID, uint8_t SH);
void LCD_control(uint8_t D, uint8_t C, uint8_t B);
void LCD_cursor(uint8_t SC, uint8_t RL);
void LCD_function(uint8_t DL, uint8_t P);
void LCD_set_CGRAM(uint8_t ACG);
void LCD_set_DDRAM(uint8_t ADD);
uint8_t LCD_read_flag_adress(void);
void LCD_write_data(uint8_t BYTE);
uint8_t LCD_read_data(void);
//---------------------------------
void LCD_write_string(char *DATA);
void LCD_set_line(uint8_t BYTE);
void LCD_set_XY(uint8_t X, uint8_t Y);
//---------------------------------
// Задержка SysTick
//---------------------------------
extern volatile uint32_t delay_dec;
//---------------------------------
#endif

#ifdef __cplusplus
}
#endif
