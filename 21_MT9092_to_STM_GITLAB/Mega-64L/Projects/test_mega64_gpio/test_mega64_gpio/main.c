/*
 * test_mega64_gpio.c
 *
 * Created: 08.01.2024 11:24:20
 * Author : Professional
 */ 

#include <avr/io.h>
#define F_CPU 1000000UL  //��� ��� �� ����������� ����������
#include <util/delay.h>

int main(void)
{
    DDRE |= ( 1 << 4 );
    DDRE |= ( 1 << 5 );
    DDRE |= ( 1 << 6 );
    DDRE |= ( 1 << 7 );
    //    DDRE = 0b11110000; //����� � ���
    while (1)
    {
	    PORTE = 0b00010000;
	    _delay_ms(500);
	    PORTE = 0b00100000;
	    _delay_ms(500);
	    PORTE = 0b01000000;
	    _delay_ms(500);
	    PORTE = 0b10000000;
	    _delay_ms(500);
    }
}

