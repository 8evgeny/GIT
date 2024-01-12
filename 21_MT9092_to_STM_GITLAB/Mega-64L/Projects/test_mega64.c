#include <avr/io.h>
#define F_CPU 1000000UL  //так как от внутреннего ренератора
#include <util/delay.h>
#include <avr/interrupt.h>

int main(void)
{
    DDRE |= ( 1 << 4 );
    DDRE |= ( 1 << 5 );
    DDRE |= ( 1 << 6 );
    DDRE |= ( 1 << 7 );
//    DDRE = 0b11110000; //ћожно и так
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
