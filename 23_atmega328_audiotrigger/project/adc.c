#include "adc.h"

void ADC_Init(void) {
	ADCSRA |= (1<<ADEN) // Разрешение использования АЦП
    |(1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0);//Делитель 128 = 64 кГц
//    ADMUX |= (1<<REFS1)|(1<<REFS0); //Внутренний Источник ОН 2,56в
//    ADMUX |= (0<<REFS1)|(0<<REFS0); //Внутреннее опорное напряжение, равное напряжению питания
    ADMUX |= (0<<REFS1)|(1<<REFS0); //Опорное напряжение подаётся на вход AREF извне
}
unsigned int ADC_convert (int numADCin) {
    switch (numADCin) {
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
