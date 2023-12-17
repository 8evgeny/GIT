#include <avr/io.h>
#include <avr/iom128.h>
#define F_CPU 8000000UL
#include <util/delay.h>

int main(void)
{
//    DDRB |= ( 1 << 0 );
//    DDRB |= ( 1 << 1 );
    DDRB = 0b00000011;
     while (1)
     {
         PORTB = 0b00000011;
         _delay_ms(100);
         PORTB = 0b00000000;
         _delay_ms(1000);
     }
}
