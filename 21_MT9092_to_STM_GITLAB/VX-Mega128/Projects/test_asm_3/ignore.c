#include <avr/io.h>
#define F_CPU 14745600UL
#include <util/delay.h>
#include <avr/interrupt.h>
#define PIN_INT0 PD0
//Процессор полностью свободен - прерывания от двух таймеров
void SetupTIMER1 (void);
void SetupTIMER3 (void);
void SetupInt0 (void);

void SetupTIMER1 (void){
    TCCR1B |= (1 << CS12);  //Fclk/256
    TCCR1B |= (1 << CS10);  // если добавить то - Fclk/1024
//    TCNT1 = 65536-62439;    //коррекция счетчика, чтобы время было ровно 1 секунда
    TCNT1 = 65535; //Первое срабатывание сразу
    TIMSK |= (1 << TOIE1);   // Разрешение прерывания overflow таймера 1
}

void SetupTIMER3 (void){
    TCCR3B |= (1 << CS32);  //Fclk/256
    ETIMSK |= (1 << TOIE3);  // Разрешение прерывания overflow таймера 3
}

void SetupInt0 (void){
    EIMSK |=  (1<<INT0);    //Разрешаем внешнее прерывание INT0
    EICRA |= (1<<ISC01)|(0<<ISC00);  //настраиваем условие прерывания
}

ISR (TIMER1_OVF_vect){
    TCNT1 = 45000;          //Чем число ближе к 65535  тем быстрее сработает таймер 1 (LED ON)
    PORTD |= 0b00000110;
    TCNT3 = 65000;          //Чем число ближе к 65535  тем быстрее сработает таймер 3 (LED OFF)
}

ISR (TIMER3_OVF_vect) {
        PORTD &= 0b11111001;
    }

int main(void)
{
    DDRD |= ( 1 << 1 );     //PD1 PD2  как выходы
    DDRD |= ( 1 << 2 );

    SetupTIMER1();
    SetupTIMER3();
    PORTD = 0b00000110;
    sei();
    while (1)
    {
    }
}
