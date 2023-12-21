#include "main.h"

void SetupGPIO(void)
{
    DDRC = 0b11111100; //6 светодиодов  PC2 - PC7 инверсия (1 - не горит)
    DDRF = 0b0000000; //6 рычагов  PF2 - PF7 (предположительно нулем )
    PORTF = 0b11111100;  //Подтяжка к 1
    PORTC = 0b11111111;
}
