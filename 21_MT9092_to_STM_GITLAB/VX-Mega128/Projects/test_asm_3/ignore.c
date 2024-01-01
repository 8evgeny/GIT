#include <avr/io.h>
#include <avr/iom128.h>
#define F_CPU 14745600UL
#include <util/delay.h>
//#include <avr/interrupt.h>

void sleep(void);
int main(void)
{
//    DDRD |= ( 1 << 1 );
//    DDRD |= ( 1 << 2 );
//    DDRD &= ~( 1 << 0 );  //PD0  как вход

    DDRD = 0b00000110; //Можно и так
     while (1)
     {
//         if( ~(PIND | ~0x01) )//PD0 нажата   срабатывает с задержкой  пока не кончится цикл _delay_ms(2000) нужно прерывание
//         {
//             PORTD = 0b00000110;
//             _delay_ms(100);
//             PORTD = 0b00000000;
//             _delay_ms(100);
//         }
//         else
//         {
             PORTD = 0b00000110;
             sleep();
             PORTD = 0b00000000;
             sleep();;
//         }

     }
//    PORTD = 0b00000100;
}

void sleep(void)
{
    _delay_ms(1000);
}
