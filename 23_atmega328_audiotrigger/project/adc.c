#include "adc.h"
//----------------------------------------
void ADC_Init(void) {
	ADCSRA |= (1<<ADEN) // Разрешение использования АЦП
	|(1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0);//Делитель 128 = 64 кГц
	ADMUX |= (1<<REFS1)|(1<<REFS0); //Внутренний Источник ОН 2,56в, вход ADC0
}

unsigned int ADC_convert (int num) {
    switch (num) {
        case ADC0: {
        ADMUX &= 0b11110000;
            break;
        }
        case ADC1: {
            ADMUX &= 0b11110000;
            ADMUX |= (1 << 0);
            break;
        }
        case ADC2: {
            ADMUX &= 0b11110000;
            ADMUX |= (1 << 1);
            break;
        }
        case ADC3: {
            ADMUX &= 0b11110000;
            ADMUX |= (1 << 0);
            ADMUX |= (1 << 1);
            break;
        }
    }
	ADCSRA |= (1<<ADSC); //Начинаем преобразование
	while((ADCSRA & (1<<ADSC))); //проверим закончилось ли аналого-цифровое преобразование
	return (unsigned int) ADC;
}
