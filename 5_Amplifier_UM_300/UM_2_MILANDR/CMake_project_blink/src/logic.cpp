#include "MDR32Fx.h"
#include "MDR32F9Qx_rst_clk.h"
#include "MDR32F9Qx_port.h"
#include "main.h"
#include "gpio.h"
#include "usb.h"
#include <string>

void Check() {
	LCD_control(1,0,0);		//включение дисплея и выбор курсора
	LCD_entry_mode(1,0);	//установка направления сдвига
	LCD_clear_display();	//установка курсора в начало
    LCD_function(1,0);	    //страница знакогенератора 0
	LCD_clear_display();
    
//    PORT_SetBits(MDR_PORTB, LED1_ERROR);	//проверка светодиодов
//    PORT_SetBits(MDR_PORTB, LED2_REC);
//    delay_ms(1000);
//    PORT_ResetBits(MDR_PORTB, LED1_ERROR);
//    PORT_ResetBits(MDR_PORTB, LED2_REC);
#if 0
    LCD_set_line(1);
    LCD_write_data(0x41);//А
    LCD_write_data(0xA0);//Б
    LCD_write_data(0x42);//В
    LCD_write_data(0xA1);//Г
    LCD_write_data(0xE0);//Д
    LCD_write_data(0x45);//Е
    LCD_write_data(0xA2);//Ё
    LCD_write_data(0xA3);//Ж
    LCD_write_data(0xA4);//З
    LCD_write_data(0xA5);//И
    LCD_write_data(0xA6);//Й
    LCD_write_data(0x4B);//К
    LCD_write_data(0xA7);//Л
    LCD_write_data(0x4D);//М
    LCD_write_data(0x48);//Н
    LCD_write_data(0x4F);//O
    LCD_write_data(0xA8);//П
    LCD_write_data(0x50);//Р
    LCD_write_data(0x43);//C
    LCD_write_data(0x54);//Т

    LCD_set_line(2);
    LCD_write_data(0xA9);//У
    LCD_write_data(0xAA);//Ф
    LCD_write_data(0x58);//X
    LCD_write_data(0xE1);//Ц
    LCD_write_data(0xAB);//Ч
    LCD_write_data(0xAC);//Ш
    LCD_write_data(0xE2);//Щ
    LCD_write_data(0xAD);//Ъ
    LCD_write_data(0xAE);//Ы
    LCD_write_data(0x08);//Ь
    LCD_write_data(0xAF);//Э
    LCD_write_data(0xB0);//Ю
    LCD_write_data(0xB1);//Я
    LCD_write_data(0x61);//а
    LCD_write_data(0xB2);//б
    LCD_write_data(0xB3);//в
    LCD_write_data(0xB4);//г
    LCD_write_data(0xE3);//д
    LCD_write_data(0x65);//е
    LCD_write_data(0xB5);//ё

    LCD_set_line(3);
    LCD_write_data(0xB6);//ж
    LCD_write_data(0xB7);//з
    LCD_write_data(0xB8);//и
    LCD_write_data(0xB9);//й
    LCD_write_data(0xBA);//к
    LCD_write_data(0xBB);//л
    LCD_write_data(0xBC);//м
    LCD_write_data(0xBD);//н
    LCD_write_data(0x6F);//о
    LCD_write_data(0xBE);//п
    LCD_write_data(0x70);//р
    LCD_write_data(0x63);//с
    LCD_write_data(0xBF);//т
    LCD_write_data(0x79);//у
    LCD_write_data(0xE4);//ф
    LCD_write_data(0x78);//х
    LCD_write_data(0xE5);//ц
    LCD_write_data(0xC0);//ч
    LCD_write_data(0xC1);//ш
    LCD_write_data(0xE6);//щ

    LCD_set_line(4);
    LCD_write_data(0xC2);//ъ
    LCD_write_data(0xC3);//ы
    LCD_write_data(0xC4);//ь
    LCD_write_data(0xC5);//э
    LCD_write_data(0xC6);//ю
    LCD_write_data(0xC7);//я

//    проверка зуммера
    PORT_SetBits(MDR_PORTB, BUZZER);
    delay_ms(200);
    PORT_ResetBits(MDR_PORTB, BUZZER);
#endif

}

void PrintString1(char* str) {
    LCD_set_XY(1, 1);
    std::string ss{str, 20};
    LCD_write_string((char*)ss.c_str());
//    delay_ms(10);
}

void PrintString2(char* str) {
    LCD_set_XY(1, 2);
    std::string ss{str, 20};
    LCD_write_string((char*)ss.c_str());
//    delay_ms(10);
}

void PrintString3(char* str) {
    LCD_set_XY(1, 3);
    std::string ss{str, 20};
    LCD_write_string((char*)ss.c_str());
//    delay_ms(10);
}

void PrintString4(char* str) {
    LCD_set_XY(1, 4);
    std::string ss{str, 20};
    LCD_write_string((char*)ss.c_str());
//    delay_ms(10);
}
