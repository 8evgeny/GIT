#include <avr/io.h>
#include <avr/iom128.h>
#define F_CPU 14745600UL
#include <util/delay.h>
//avrdude -p m128 -c Usbasp -B 4 -P usb -U flash:w:"/home/evg/SOFT/Github/GIT/21_MT9092_to_STM_GITLAB/VX-Mega128/Projects/blink/build/ATmega128_blink.hex":a
#include <avr/interrupt.h>

int main(void)
{
    DDRB |= ( 1 << 0 );//Эта запись означает, что для нулевого разряда порта B (номер разряда указывает последняя цифра), т. е. для вывода PB0 мы прописали в DDR единицу и тем самым настроили его как выход.
    /*А вот, если бы нам туда понадобилось прописать ноль, т. е. настроить PB0 как вход, то команда для начинающего программиста выглядела бы ещё более странной:
    DDRB &= ~( 1 << 0 );  */
    DDRB |= ( 1 << 1 );
    DDRB &= ~( 1 << 2 );  //PB2  как вход

//    DDRB = 0b00000011; //Можно и так
     while (1)
     {
         if( ~(PINB | ~0x04) )//PB2 нажата   срабатывает с задержкой  пока не кончится цикл _delay_ms(2000) нужно прерывание
         {
             PORTB = 0b00000011;
             _delay_ms(100);
             PORTB = 0b00000000;
             _delay_ms(100);
         }
         else
         {
             PORTB = 0b00000011;
             _delay_ms(100);
             PORTB = 0b00000000;
             _delay_ms(3000);
         }
     }
}
