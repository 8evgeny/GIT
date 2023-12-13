#include <avr/io.h>
#define F_CPU 14745600UL
#include <util/delay.h>
#include <avr/interrupt.h>
//Процессор полностью свободен - прерывания от двух таймеров
//Добавляем прерывания от кнопки
static void SetupTIMER1 (void);
static void SetupTIMER3 (void);

void SetupTIMER1 (void){
    TCCR1B |= (1 << CS12);  //Fclk/256
    TCCR1B |= (1 << CS10);  // если добавить то - Fclk/1024
    TCNT1 = 65536-62439;    //коррекция счетчика, чтобы время было ровно 1 секунда
    TIMSK = (1 << TOIE1);   // Разрешение прерывания overflow таймера 1
}

void SetupTIMER3 (void){
    TCCR3B |= (1 << CS32);  //Fclk/256
    ETIMSK = (1 << TOIE3);  // Разрешение прерывания overflow таймера 3
}

ISR (TIMER1_OVF_vect){
    TCNT1 = 20000;          //Чем число ближе к 65535  тем быстрее сработает таймер 1 (LED ON)
    PORTB = 0b00000011;
    TCNT3 = 65000;          //Чем число ближе к 65535  тем быстрее сработает таймер 3 (LED OFF)
}

ISR (TIMER3_OVF_vect) {
    PORTB = 0b00000000;
}

int main(void)
{
    DDRB |= ( 1 << 0 );     //PB0 PB1  как выходы
    DDRB |= ( 1 << 1 );
    DDRB &= ~( 1 << 2 );    //PB2  как вход
    SetupTIMER1();
    SetupTIMER3();
    sei();
    while (1)
    {
    }
}
