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
         PORTC = 0b01010100;
         _delay_ms(200);
         PORTC = 0b10101000;
         _delay_ms(200);
     }
}
