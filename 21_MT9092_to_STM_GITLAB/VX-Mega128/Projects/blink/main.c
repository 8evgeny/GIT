#include <avr/io.h>
#include <avr/iom128.h>
#define F_CPU 8000000UL
#include <util/delay.h>

#include <avr/interrupt.h>

volatile unsigned char z1;

ISR(TIMER0_OVF_vect)
{
/*  TCNT0=0xB2;
    z1++;
*/
}


int main(void)
{
//    DDRB |= ( 1 << 0 );//Эта запись означает, что для нулевого разряда порта B (номер разряда указывает последняя цифра), т. е. для вывода PC0 мы прописали в DDR единицу и тем самым настроили его как выход.
    /*А вот, если бы нам туда понадобилось прописать ноль, т. е. настроить PC0 как вход, то команда для начинающего программиста выглядела бы ещё более странной:
    DDRB &= ~( 1 << 0 );  */
//    DDRC |= ( 1 << 4 );
//    DDRC |= ( 1 << 6 );
//    DDRC |= ( 1 << 5 );
//    DDRC = 0b11111111; //Можно и так

//     while (1)
//     {
//         PORTC = 0b11101111;
//         _delay_ms(200);
//         for(int i=0;i<1000000;i++){}
//         PORTC = 0b00010000;
//         _delay_ms(200);
//         for(int i=0;i<1000000;i++){}
//     }
//     PORTB = 0b0000001;

    //TIMER0 -------------------------------------------------
    // Timer/Counter 0 initialization
    // Clock source: System Clock
    // Clock value: 7,813 kHz
    // Mode: Normal top=0xFF
    // OC0 output: Disconnected
    // Timer Period: 9,984 ms
    ASSR=0<<AS0;
    TCCR0=(0<<WGM00) | (0<<COM01) | (0<<COM00) | (0<<WGM01) | (1<<CS02) | (1<<CS01) | (1<<CS00);
    TCNT0=0xB2;
    OCR0=0x00;


    // Timer(s)/Counter(s) Interrupt(s) initialization
        TIMSK=(0<<OCIE2) | (0<<TOIE2) | (0<<TICIE1) | (0<<OCIE1A) | (0<<OCIE1B) | (0<<TOIE1) | (0<<OCIE0) | (1<<TOIE0);
        ETIMSK=(0<<TICIE3) | (0<<OCIE3A) | (0<<OCIE3B) | (0<<TOIE3) | (0<<OCIE3C) | (0<<OCIE1C);


        DDRA = 0xFF;
        DDRC = 0xFF;
        PORTA = 0;
        PORTC = 0;
        z1=0;

        sei();

        //PORTA |= (1<<PA4); _delay_ms(1000);
        PORTC |= (1<<PD5); _delay_ms(1000);

        PORTA |= (1<<PA3); _delay_ms(1000);
        PORTA |= (1<<PA5); _delay_ms(1000);
        PORTA |= (1<<PA6); _delay_ms(1000);

        PORTA = 0;
        PORTC = 0;

        while (1)
        {
            PORTC ^= (1<<PC5); _delay_ms(500);
        }



}
