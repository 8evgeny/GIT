#include "main.h"
void SetupGPIO(void);
bool checkLever(int num);
void setLed(int num);
void resetLed(int num);

int main() {
    SetupGPIO();

    while (1) {
        for (int i = 1; i <= 6; ++i) {
            if (checkLever(i)) setLed(i);
            else resetLed(i);
        }
    }
}


void SetupGPIO()
{
    DDRC = 0b11111100;  //6 светодиодов  PC2 - PC7 инверсия (1 - не горит)
    DDRF = 0b0000000;   //6 рычагов  PF2 - PF7 (предположительно нулем )
    PORTF = 0b11111100; //Подтяжка к 1
    PORTC = 0b11111111;
}

bool checkLever(int num){ //1 - 6
    switch(num) {
    case 1:
        if (!(PINF & 0b00000100)) return true;
        else return false;
    case 2:
        if (!(PINF & 0b00001000)) return true;
        else return false;
    case 3:
        if (!(PINF & 0b00010000)) return true;
        else return false;
    case 4:
        if (!(PINF & 0b00100000)) return true;
        else return false;
    case 5:
        if (!(PINF & 0b01000000)) return true;
        else return false;
    case 6:
        if (!(PINF & 0b10000000)) return true;
        else return false;
    default :
        break;
    }
    return false;
}

void setLed(int num)
{
    switch(num) {
        case 1:
            PORTC &= 0b11111011;
            break;
        case 2:
            PORTC &= 0b11110111;
            break;
        case 3:
            PORTC &= 0b11101111;
            break;
        case 4:
            PORTC &= 0b11011111;
            break;
        case 5:
            PORTC &= 0b10111111;
            break;
        case 6:
            PORTC &= 0b01111111;
            break;
        default :
            break;
    }
}

void resetLed(int num)
{
    switch(num) {
        case 1:
            PORTC |= 0b00000100;
            break;
        case 2:
            PORTC |= 0b00001000;
            break;
        case 3:
            PORTC |= 0b00010000;
            break;
        case 4:
            PORTC |= 0b00100000;
            break;
        case 5:
            PORTC |= 0b01000000;
            break;
        case 6:
            PORTC |= 0b10000000;
            break;
        default :
            break;
    }
}
