#include "main.h"

int main(void) {
    GPIO_Init();
    blink(); //Индикация при включении попеременно моргаем индикаторами




}

static void GPIO_Init() {
    DDRC = 0b00110000; //PC4 PC5 выходы
    DDRB = 0b00110000; //PB4 PB5 выходы
    DDRD = 0b00001100; //PD2 PD3 выходы
}
static void pinON(int num) {
    PORTC |= (1 << num);
}
static void pinOFF(int num) {
    PORTC &= ~(1 << num);
}
static void blink(void){
    pinON(led1);
    _delay_ms(300);
    pinOFF(led1);
    _delay_ms(300);
    pinON(led2);
    _delay_ms(300);
    pinOFF(led2);
    _delay_ms(300);
}
