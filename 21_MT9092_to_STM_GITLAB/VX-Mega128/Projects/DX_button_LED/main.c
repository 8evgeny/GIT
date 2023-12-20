#include <avr/io.h>
#include <avr/iom128.h>
#define F_CPU 4000000UL
#include <util/delay.h>
#include <avr/interrupt.h>

int main(void)
{
    DDRC = 0b11111100; //6 светодиодов  PC2 - PC7 инверсия (1 - не горит)
    DDRF = 0b0000000; //6 рычагов  PF2 - PF7 (предположительно нулем )
    PORTF = 0b11111100;  //Подтяжка к 1
    PORTC = 0b11111111;
     while (1)
     {
         if (!(PINF & 0b10000000)) //Button 6
         {
             PORTC = 0b01111111;
         }
         if (!(PINF & 0b01000000)) //Button 5
         {
             PORTC = 0b10111111;
         }
     }
}
