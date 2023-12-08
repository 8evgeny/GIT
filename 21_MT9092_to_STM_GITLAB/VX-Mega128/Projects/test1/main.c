#include <avr/io.h>
#include <avr/iom128.h>
#define F_CPU 1000000UL
#include <util/delay.h>
//avrdude -p m128 -c Usbasp -B 4 -P usb -U flash:w:"/home/evg/SOFT/Github/GIT/21_MT9092_to_STM_GITLAB/VX-Mega128/Projects/blink/build/ATmega128_blink.hex":a
#include <avr/interrupt.h>

int main(void)
{
//    DDRB |= ( 1 << 0 );//Эта запись означает, что для нулевого разряда порта B (номер разряда указывает последняя цифра), т. е. для вывода PC0 мы прописали в DDR единицу и тем самым настроили его как выход.
    /*А вот, если бы нам туда понадобилось прописать ноль, т. е. настроить PC0 как вход, то команда для начинающего программиста выглядела бы ещё более странной:
    DDRB &= ~( 1 << 0 );  */
//    DDRC |= ( 1 << 4 );
//    DDRC |= ( 1 << 6 );
//    DDRC |= ( 1 << 5 );
    DDRB = 0b11111111; //Можно и так

     while (1)
     {
         PORTB = 0b01010101;
         _delay_ms(1000);
         PORTB = 0b10101010;
         _delay_ms(1009);
     }

}
