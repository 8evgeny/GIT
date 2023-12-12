#include <avr/io.h>
#define F_CPU 14745600UL
#include <util/delay.h>
#include <avr/interrupt.h>

static void SetupTIMER1 (void);

void SetupTIMER1 (void){
   //На частоте тактов 16 МГц прерывание переполнения T/C1
   // произойдет через (счет до 65535):
   // 1 << CS10 4096 mkS (нет прескалера Fclk)
   // 1 << CS11 32.768 mS (Fclk/8)
   // (1 << CS11)|(1 << CS10) 262.144 mS (Fclk/64)
   // 1 << CS12 1048.576 mS (Fclk/256)
   TCCR1B = (1 << CS12);
   TCNT1 = 65536-62439;        //коррекция счетчика, чтобы время было ровно 1 секунда

   /* Разрешение прерывания overflow таймера 1. */
   TIMSK = (1 << TOIE1);
}

ISR (TIMER1_OVF_vect, ISR_BLOCK){

    //теперь прерывание будет происходить через 62439 тиков
    // таймера 1, что на частоте 16 МГц составит 1 сек.
    TCNT1 = 65536-62439;
    //Далее идет код, который будет работать каждую секунду.
    //Желательно, чтобы этот код был короче.
    PORTB = 0b00000011;
    _delay_ms(100);
    PORTB = 0b00000000;
    _delay_ms(100);

}

int main(void)
{

    DDRB |= ( 1 << 0 );     //PB0 PB1  как выходы
    DDRB |= ( 1 << 1 );
    DDRB &= ~( 1 << 2 );    //PB2  как вход
    SetupTIMER1();
    sei();

    while (1)
    {

    }
}
