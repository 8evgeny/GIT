//---------------------------------
// main.h
// Плата индикации (MDR32)
//---------------------------------
#ifdef __cplusplus
extern "C" {
#endif

#include "MDR32F9Qx_timer.h"
#include "MDR32F9Qx_iwdg.h"
//---------------------------------
#include "MDR32F9Qx_config.h"
#include "MDR32F9Qx_usb_handlers.h"
#include "MDR32Fx.h"
#include "MDR32F9Qx_rst_clk.h"
#include "MDR32F9Qx_port.h"
//---------------------------------
#include "gpio.h"

#ifndef _MAIN
    
#define _MAIN
//---------------------------------
// Прототипы функций
//---------------------------------
#define BUFFER_LENGTH    200
//static uint8_t BufferReceive[2];
static uint8_t Buffer[BUFFER_LENGTH];
//static uint8_t* pBuffer = Buffer;
static uint8_t BufferLCD[BUFFER_LENGTH];

/* Задание конфигурации последовательной линии связи которую может прочитать хост */

#ifdef USB_CDC_LINE_CODING_SUPPORTED
static USB_CDC_LineCoding_TypeDef LineCoding;
#endif /* USB_CDC_LINE_CODING_SUPPORTED */

static void VCom_Configuration(void)
{
#ifdef USB_CDC_LINE_CODING_SUPPORTED
    LineCoding.dwDTERate = 115200;
    LineCoding.bCharFormat = 0;
    LineCoding.bParityType = 0;
    LineCoding.bDataBits = 8;
#endif /* USB_CDC_LINE_CODING_SUPPORTED */
}

void firstScreen();
void parsingBuffer();
void checkLCD1();
void checkLCD2();
void checkBUZZER();

void RCC_init(void);
void IWDT_init(void);
void GPIO_init(void);
void Check(void);
void PrintString1(char* str);
void PrintString2(char* str);
void PrintString3(char* str);
void PrintString4(char* str);
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
