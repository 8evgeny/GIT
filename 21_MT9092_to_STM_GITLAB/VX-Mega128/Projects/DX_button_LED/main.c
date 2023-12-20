#include <avr/io.h>
#include <avr/iom128.h>
#define F_CPU 4000000UL
#include <util/delay.h>
#include <avr/interrupt.h>

int main(void)
{
    DDRC = 0b11111100;
     while (1)
     {
         PORTC = 0b01111111;
         _delay_ms(10);
         PORTC = 0b11111111;
         _delay_ms(1000);
     }
}
