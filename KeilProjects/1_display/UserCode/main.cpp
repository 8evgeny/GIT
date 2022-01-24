#include "gpio.h"
#include "main.h"
#include "usb.h"
#include <string>
#include <vector>
#include <string.h>
#include <chrono>


int main (int argc, char** argv) {

	__disable_irq();
	RCC_init();
	GPIO_init();
	__enable_irq();

    TIM1_init();
//    IWDT_init();

    VCom_Configuration();
    USB_CDC_Init(Buffer, 165, SET);
    Setup_CPU_Clock();
    Setup_USB();

    LCD_init();
    Check();

    while (1)
    {
        while(Buffer[0] == 0x00)
        {
            firstScreen();
            delay_ms(1000);
            LCD_set_line(4); LCD_write_string((char*)"                    ");
            delay_ms(1000);
//            IWDG_ReloadCounter();	//сбрасываем IWDT
        }

            parsingBuffer();

            if(BufferLCD[0] == 49) //если 1 то горит первый светодиод
            {
                PORT_SetBits(MDR_PORTB, LED2_REC);
            }
            else
            {
                PORT_ResetBits(MDR_PORTB, LED2_REC);
            }
            if(BufferLCD[1] == 49) //если 1 то горит второй светодиод
            {
                PORT_SetBits(MDR_PORTB, LED1_ERROR);
            }
            else
            {
                PORT_ResetBits(MDR_PORTB, LED1_ERROR);
            }
            if (BufferLCD[2] == 49) //Постоянный сигнал
            {
                PORT_SetBits(MDR_PORTB, BUZZER);
            }
            else //Выключен
            {
                PORT_ResetBits(MDR_PORTB, BUZZER);
            }

    //        checkLCD1();
    //        checkLCD2();
    //        checkBUZZER();

            char  str1[21]; //4 строки на экране
            char  str2[21];
            char  str3[21];
            char  str4[21];

            memcpy (str1, BufferLCD + 3, 20);
            memcpy (str2, BufferLCD + 23 , 20);
            memcpy (str3, BufferLCD + 43 , 20);
            memcpy (str4, BufferLCD + 63 , 20);

            PrintString1(str1);
            PrintString2(str2);
            PrintString3(str3);
            PrintString4(str4);

//            IWDG_ReloadCounter();	//сбрасываем IWDT
//        }
	}	
}

void parsingBuffer()
{
    uint8_t* pBuff = Buffer;
    uint8_t* pBuffLCD = BufferLCD;

    for (int numSymbols = 0; numSymbols < 83; ++numSymbols)
    {
        uint8_t temp = *pBuff;
        if ((temp & 0xD0) != 0xD0)
        {
            *pBuffLCD = *pBuff;
            ++pBuff;
            ++pBuffLCD;
        }
        else //unicode
        {
            if (*pBuff == 0xD0)
            {
                ++pBuff;
                if (*pBuff == 0x90) *pBuffLCD = 0x41;     //А
                else if (*pBuff == 0x81) *pBuffLCD = 0xA2;//Ё
                else if (*pBuff == 0x91) *pBuffLCD = 0xA0;//Б
                else if (*pBuff == 0x92) *pBuffLCD = 0x42;//В
                else if (*pBuff == 0x93) *pBuffLCD = 0xA1;//Г
                else if (*pBuff == 0x94) *pBuffLCD = 0xE0;//Д
                else if (*pBuff == 0x95) *pBuffLCD = 0x45;//Е
                else if (*pBuff == 0x96) *pBuffLCD = 0xA3;//Ж
                else if (*pBuff == 0x97) *pBuffLCD = 0xA4;//З
                else if (*pBuff == 0x98) *pBuffLCD = 0xA5;//И
                else if (*pBuff == 0x99) *pBuffLCD = 0xA6;//Й
                else if (*pBuff == 0x9A) *pBuffLCD = 0x4B;//К
                else if (*pBuff == 0x9B) *pBuffLCD = 0xA7;//Л
                else if (*pBuff == 0x9C) *pBuffLCD = 0x4D;//М
                else if (*pBuff == 0x9D) *pBuffLCD = 0x48;//Н
                else if (*pBuff == 0x9E) *pBuffLCD = 0x4F;//O
                else if (*pBuff == 0x9F) *pBuffLCD = 0xA8;//П
                else if (*pBuff == 0xA0) *pBuffLCD = 0x50;//Р
                else if (*pBuff == 0xA1) *pBuffLCD = 0x43;//C
                else if (*pBuff == 0xA2) *pBuffLCD = 0x54;//Т
                else if (*pBuff == 0xA3) *pBuffLCD = 0xA9;//У
                else if (*pBuff == 0xA4) *pBuffLCD = 0xAA;//Ф
                else if (*pBuff == 0xA5) *pBuffLCD = 0x58;//X
                else if (*pBuff == 0xA6) *pBuffLCD = 0xE1;//Ц
                else if (*pBuff == 0xA7) *pBuffLCD = 0xAB;//Ч
                else if (*pBuff == 0xA8) *pBuffLCD = 0xAC;//Ш
                else if (*pBuff == 0xA9) *pBuffLCD = 0xE2;//Щ
                else if (*pBuff == 0xAA) *pBuffLCD = 0xAD;//Ъ
                else if (*pBuff == 0xAB) *pBuffLCD = 0xAE;//Ы
                else if (*pBuff == 0xAC) *pBuffLCD = 0x08;//Ь
                else if (*pBuff == 0xAD) *pBuffLCD = 0xAF;//Э
                else if (*pBuff == 0xAE) *pBuffLCD = 0xB0;//Ю
                else if (*pBuff == 0xAF) *pBuffLCD = 0xB1;//Я
                else if (*pBuff == 0xB0) *pBuffLCD = 0x61;//а
                else if (*pBuff == 0xB1) *pBuffLCD = 0xB2;//б
                else if (*pBuff == 0xB2) *pBuffLCD = 0xB3;//в
                else if (*pBuff == 0xB3) *pBuffLCD = 0xB4;//г
                else if (*pBuff == 0xB4) *pBuffLCD = 0xE3;//д
                else if (*pBuff == 0xB5) *pBuffLCD = 0x65;//е
                else if (*pBuff == 0xB6) *pBuffLCD = 0xB6;//ж
                else if (*pBuff == 0xB7) *pBuffLCD = 0xB7;//з
                else if (*pBuff == 0xB8) *pBuffLCD = 0xB8;//и
                else if (*pBuff == 0xB9) *pBuffLCD = 0xB9;//й
                else if (*pBuff == 0xBA) *pBuffLCD = 0xBA;//к
                else if (*pBuff == 0xBB) *pBuffLCD = 0xBB;//л
                else if (*pBuff == 0xBC) *pBuffLCD = 0xBC;//м
                else if (*pBuff == 0xBD) *pBuffLCD = 0xBD;//н
                else if (*pBuff == 0xBE) *pBuffLCD = 0x6F;//о
                else if (*pBuff == 0xBF) *pBuffLCD = 0xBE;//п
            }
            else if (*pBuff == 0xD1)
            {
                ++pBuff;
                if (*pBuff == 0x80) *pBuffLCD = 0x70;     //р
                else if (*pBuff == 0x91) *pBuffLCD = 0xB5;//ё
                else if (*pBuff == 0x81) *pBuffLCD = 0x63;//с
                else if (*pBuff == 0x82) *pBuffLCD = 0xBF;//т
                else if (*pBuff == 0x83) *pBuffLCD = 0x79;//у
                else if (*pBuff == 0x84) *pBuffLCD = 0xE4;//ф
                else if (*pBuff == 0x85) *pBuffLCD = 0x78;//х
                else if (*pBuff == 0x86) *pBuffLCD = 0xE5;//ц
                else if (*pBuff == 0x87) *pBuffLCD = 0xC0;//ч
                else if (*pBuff == 0x88) *pBuffLCD = 0xC1;//ш
                else if (*pBuff == 0x89) *pBuffLCD = 0xE6;//щ
                else if (*pBuff == 0x8A) *pBuffLCD = 0xC2;//ъ
                else if (*pBuff == 0x8B) *pBuffLCD = 0xC3;//ы
                else if (*pBuff == 0x8C) *pBuffLCD = 0xC4;//ь
                else if (*pBuff == 0x8D) *pBuffLCD = 0xC5;//э
                else if (*pBuff == 0x8E) *pBuffLCD = 0xC6;//ю
                else if (*pBuff == 0x8F) *pBuffLCD = 0xC7;//я
            }
            else //другой символ
            {
                ++pBuff;
                *pBuffLCD = 0x20;
            }

            ++pBuff;
            ++pBuffLCD;
        }
    }

    USB_CDC_Reset(); //Экран заполнен
}

void firstScreen()
{
    LCD_set_line(1);
    LCD_write_string((char*)"    GIT-COMM IPS    ");
    LCD_set_XY(9, 2);
    LCD_write_data(0xA8);//П
    LCD_write_data(0xE0);//Д
    LCD_write_data(0x4B);//К
    LCD_write_data(0x42);//В
    LCD_set_XY(6, 4);
    LCD_write_data(0xA4);//З
    LCD_write_data(0x61);//а
    LCD_write_data(0xB4);//г
    LCD_write_data(0x70);//р
    LCD_write_data(0x79);//у
    LCD_write_data(0xB7);//з
    LCD_write_data(0xBA);//к
    LCD_write_data(0x61);//а
    LCD_write_string((char*)"...");
}
