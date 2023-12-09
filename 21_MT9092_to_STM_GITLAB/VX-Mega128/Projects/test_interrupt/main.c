#include <avr/io.h>
//#include <avr/iom128.h>
#define F_CPU 1000000UL
#include <util/delay.h>
//avrdude -p m128 -c Usbasp -B 4 -P usb -U flash:w:"/home/evg/SOFT/Github/GIT/21_MT9092_to_STM_GITLAB/VX-Mega128/Projects/blink/build/ATmega128_blink.hex":a
#include <avr/interrupt.h>

#define PIN_INT0 PB2
volatile unsigned char flagInt0 = 0;

int main(void)
{
    //настраиваем вывод на вход
    DDRB &= ~(1<<PIN_INT0);
    //включаем подтягивающий резистор
    PORTB |= (1<<PIN_INT0);
    //разрешаем внешнее прерывание на int0
//    GICR |= (1<<INT0);

     while (1)
     {
         if( ~(PINB | ~0x04) )//PB2 нажата   срабатывает с задержкой  пока не кончится цикл _delay_ms(2000) нужно прерывание
         {
             PORTB = 0b00000011;
             _delay_ms(10);
             PORTB = 0b00000000;
             _delay_ms(100);
         }
         else
         {
             PORTB = 0b00000011;
             _delay_ms(10);
             PORTB = 0b00000000;
             _delay_ms(2000);
         }
     }
}
