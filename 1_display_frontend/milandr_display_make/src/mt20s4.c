
//---------------------------------
// mt20s4.h
// Плата индикации (MDR32)
//----------------------------------
#include "MDR32Fx.h"
#include "MDR32F9Qx_rst_clk.h"
#include "MDR32F9Qx_port.h"
//---------------------------------
#include "main.h"
#include "gpio.h"
//---------------------------------
void LCD_init() {
	//установить разрядность интерфейса
	delay_ms(25);	//ждать не менее 20мс
	PORT_SetBits(MDR_PORTB, EN);	//EN set
	PORT_ResetBits(MDR_PORTB, RW|RS); 
	PORT_ResetBits(MDR_PORTA, D7|D6);
	PORT_SetBits(MDR_PORTA, D5|D4);
	PORT_ResetBits(MDR_PORTA, D3|D2|D1|D0);
	PORT_ResetBits(MDR_PORTB, EN);	//EN reset
	//установить разрядность интерфейса
	delay_ms(1);	//ждать не менее 40мкс
	PORT_SetBits(MDR_PORTB, EN);	//EN set
	PORT_ResetBits(MDR_PORTB, RW|RS); 
	PORT_ResetBits(MDR_PORTA, D7|D6);
	PORT_SetBits(MDR_PORTA, D5|D4);
	PORT_ResetBits(MDR_PORTA, D3|D2|D1|D0);
	PORT_ResetBits(MDR_PORTB, EN);	//EN reset
	//установить разрядность интерфейса
	delay_ms(1);	//ждать не менее 40мкс
	PORT_SetBits(MDR_PORTB, EN);	//EN set
	PORT_ResetBits(MDR_PORTB, RW|RS); 
	PORT_ResetBits(MDR_PORTA, D7|D6);
	PORT_SetBits(MDR_PORTA, D5|D4);
	PORT_ResetBits(MDR_PORTA, D3|D2|D1|D0);
	PORT_ResetBits(MDR_PORTB, EN);	//EN reset
	//установка параметров
	delay_ms(1);	//ждать не менее 40мкс
	PORT_SetBits(MDR_PORTB, EN);	//EN set
	PORT_ResetBits(MDR_PORTB, RW|RS); 
	PORT_ResetBits(MDR_PORTA, D7|D6);
	PORT_SetBits(MDR_PORTA, D5|D4|D3);
	PORT_ResetBits(MDR_PORTA, D2|D1|D0);
	PORT_ResetBits(MDR_PORTB, EN);	//EN reset
	//выключение дисплея
	PORT_SetBits(MDR_PORTB, EN);	//EN set
	PORT_ResetBits(MDR_PORTB, RW|RS); 
	PORT_ResetBits(MDR_PORTA, D7|D6|D5|D4);
	PORT_SetBits(MDR_PORTA, D3);
	PORT_ResetBits(MDR_PORTA, D2|D1|D0);
	PORT_ResetBits(MDR_PORTB, EN);	//EN reset
	//очистка дисплея
	PORT_SetBits(MDR_PORTB, EN);	//EN set
	PORT_ResetBits(MDR_PORTB, RW|RS); 
	PORT_ResetBits(MDR_PORTA, D7|D6|D5|D4|D3|D2|D1);
	PORT_SetBits(MDR_PORTA, D0);
	PORT_ResetBits(MDR_PORTB, EN);	//EN reset
	//установка режима ввода данных
	PORT_SetBits(MDR_PORTB, EN);	//EN set
	PORT_ResetBits(MDR_PORTB, RW|RS); 
	PORT_ResetBits(MDR_PORTA, D7|D6|D5|D4|D3);
	PORT_SetBits(MDR_PORTA, D2|D1);
	PORT_ResetBits(MDR_PORTA, D0);
	PORT_ResetBits(MDR_PORTB, EN);	//EN reset
}
//---------------------------------
void LCD_clear_display() {
	//очистка дисплея и перемещение курсор в самую левую позицию
	PORT_ResetBits(MDR_PORTB, RS|RW);
	delay_ms(1);
	PORT_SetBits(MDR_PORTB, EN);		//EN set
	delay_ms(1);
	PORT_Write(MDR_PORTA, 0x01);
	delay_ms(1);
	PORT_ResetBits(MDR_PORTB, EN);	//EN reset
}
//---------------------------------
void LCD_return_home() {
	//перемещение курсора в левую позицию
	PORT_ResetBits(MDR_PORTB, RS|RW);
	delay_ms(1);
	PORT_SetBits(MDR_PORTB, EN);		//EN set
	delay_ms(1);
	PORT_Write(MDR_PORTA, 0x02);
	delay_ms(1);
	PORT_ResetBits(MDR_PORTB, EN);	//EN reset
}
//---------------------------------
void LCD_entry_mode(uint8_t ID, uint8_t SH) {
	//установка направления сдвига: ID=0/1 - влево/вправо
	//разрешение сдвига SH=1 при записи в DDRAM
	uint8_t temp = 0x04;
	if (ID) temp |= 0x02; else temp &= (~0x02);
	if (SH) temp |= 0x01; else temp &= (~0x01);
	PORT_ResetBits(MDR_PORTB, RS|RW);
	delay_ms(1);
	PORT_SetBits(MDR_PORTB, EN);		//EN set
	delay_ms(1);
	PORT_Write(MDR_PORTA, temp);
	delay_ms(1);
	PORT_ResetBits(MDR_PORTB, EN);	//EN reset
}
//---------------------------------
void LCD_control(uint8_t D, uint8_t C, uint8_t B) {
	//включение модуля D=1
	//выбор типа курсора (C, B)
	//C=0, B=0 — курсора нет, ничего не мигает;
	//C=0, B=1 — курсора нет, мигает весь символ в позиции курсора;
	//C=1, B=0 — курсор есть (подчёркивание), ничего не мигает;
	//C=1, B=1 — курсор есть (подчёркивание) и только он и мигает
	uint8_t temp = 0x08;
	if (D) temp |= 0x04; else temp &= (~0x04);
	if (C) temp |= 0x02; else temp &= (~0x02);
	if (B) temp |= 0x01; else temp &= (~0x01);
	PORT_ResetBits(MDR_PORTB, RS|RW);
	delay_ms(1);
	PORT_SetBits(MDR_PORTB, EN);		//EN set
	delay_ms(1);
	PORT_Write(MDR_PORTA, temp);
	delay_ms(1);
	PORT_ResetBits(MDR_PORTB, EN);	//EN reset
}
//---------------------------------
void LCD_cursor(uint8_t SC, uint8_t RL) {
	//сдвиг дисплея или курсора
	//SC=0/1 - курсор/дисплей
	//RL=0/1 - влево/вправо
	uint8_t temp = 0x10;
	if (SC) temp |= 0x08; else temp &= (~0x08);
	if (RL) temp |= 0x04; else temp &= (~0x04);
	PORT_ResetBits(MDR_PORTB, RS|RW);
	delay_ms(1);
	PORT_SetBits(MDR_PORTB, EN);		//EN set
	delay_ms(1);
	PORT_Write(MDR_PORTA, temp);
	delay_ms(1);
	PORT_ResetBits(MDR_PORTB, EN);	//EN reset
}
//---------------------------------
void LCD_function(uint8_t DL, uint8_t P) {
	//разрядность интерфейса и страницы знакогенератора Р
	//DL=0/1 - 4/8 бит
	uint8_t temp = 0x28;
	if (DL) temp |= 0x10; else temp &= (~0x10);
	if (P) temp |= 0x02; else temp &= (~0x02);
	PORT_ResetBits(MDR_PORTB, RS|RW);
	delay_ms(1);
	PORT_SetBits(MDR_PORTB, EN);		//EN set
	delay_ms(1);
	PORT_Write(MDR_PORTA, temp);
	delay_ms(1);
	PORT_ResetBits(MDR_PORTB, EN);	//EN reset
}
//---------------------------------
void LCD_set_CGRAM(uint8_t ACG) {
	//установка адреса последующей операции и выбор области CGRAM
	uint8_t temp = 0x40;
	temp |= ACG & 0x3F;
	PORT_ResetBits(MDR_PORTB, RS|RW);
	delay_ms(1);
	PORT_SetBits(MDR_PORTB, EN);		//EN set
	delay_ms(1);
	PORT_Write(MDR_PORTA, temp);
	delay_ms(1);
	PORT_ResetBits(MDR_PORTB, EN);	//EN reset
}
//---------------------------------
void LCD_set_DDRAM(uint8_t ADD) {
	//установка адреса последующей операции и выбор области DDRAM
	uint8_t temp = 0x80;
	temp |= ADD & 0x7F;
	PORT_ResetBits(MDR_PORTB, RS|RW);
	delay_ms(1);
	PORT_SetBits(MDR_PORTB, EN);		//EN set
	delay_ms(1);
	PORT_Write(MDR_PORTA, temp);
	delay_ms(1);
	PORT_ResetBits(MDR_PORTB, EN);	//EN reset
}
uint8_t LCD_read_flag_adress() {
	//чтение флага занятости и счетчика адреса
	uint8_t temp;
	PORT_ResetBits(MDR_PORTB, RS);
	PORT_SetBits(MDR_PORTB, RW);
	delay_ms(1);
	PORT_SetBits(MDR_PORTB, EN);		//EN set
	delay_ms(1);
	temp = PORT_ReadInputData(MDR_PORTA) & 0x000f;
	delay_ms(1);
	PORT_ResetBits(MDR_PORTB, EN);	//EN reset
	return temp;
}
//---------------------------------
void LCD_write_data(uint8_t BYTE) {
	//запись данных в активную область
	PORT_SetBits(MDR_PORTB, RS);
	PORT_ResetBits(MDR_PORTB, RW);
	delay_ms(1);
	PORT_SetBits(MDR_PORTB, EN);		//EN set
	delay_ms(1);
	PORT_Write(MDR_PORTA, BYTE);
	delay_ms(1);
	PORT_ResetBits(MDR_PORTB, EN);	//EN reset
}
//---------------------------------
uint8_t LCD_read_data() {
	//чтение данных из активной области
	uint8_t temp;
	PORT_SetBits(MDR_PORTB, RS|RW);
	delay_ms(1);
	PORT_SetBits(MDR_PORTB, EN);		//EN set
	delay_ms(1);
	temp = PORT_ReadInputData(MDR_PORTA) & 0x000f;
	delay_ms(1);
	PORT_ResetBits(MDR_PORTB, EN);	//EN reset
	return temp;
}
//---------------------------------
void LCD_write_string(char *DATA) {
	//запись строки
	while(*DATA) {
		LCD_write_data(*DATA);
		DATA++;
	}
}
//---------------------------------
void LCD_set_line(uint8_t BYTE) {
	//установка строки дисплея
	switch (BYTE) {
		case 1: LCD_set_DDRAM(0x00); break;
		case 2: LCD_set_DDRAM(0x40); break;
		case 3: LCD_set_DDRAM(0x14); break;
		case 4: LCD_set_DDRAM(0x54); break;
		default: break;
	}
}
//---------------------------------
void LCD_set_XY(uint8_t X, uint8_t Y) {
	//произвольная установка курсора
	LCD_set_line(Y);
	while(--X) LCD_cursor(0,1);
}
