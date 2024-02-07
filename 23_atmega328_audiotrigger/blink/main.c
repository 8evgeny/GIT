#include <avr/io.h>
//#include <avr/iom128.h>
#define F_CPU 16000000UL
#include <util/delay.h>
#include <avr/interrupt.h>

int main(void)
{
    DDRC = 0b00110000;
     while (1)
     {
         PORTC = 0b00110000;
         _delay_ms(100);
         PORTC = 0b00000000;
         _delay_ms(3000);
     }
}
