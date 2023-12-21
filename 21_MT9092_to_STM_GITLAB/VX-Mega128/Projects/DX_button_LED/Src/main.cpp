#include "main.h"
void SetupGPIO(void);
bool checkLever(int num);


int main() {
    SetupGPIO();

    while (1) {
        if (checkLever(6)){ //Button 6  загорается  - нулем
            PORTC &= 0b01111111;
        }
        else { //Button 6  гаснет  - единицей
            PORTC |= 0b10000000;
        }
        if (checkLever(5)){ //Button 5
            PORTC &= 0b10111111;
        }
        else {
            PORTC |= 0b01000000;
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
        if (!(PINF & 0b00001000)) return true;
        else return false;
    case 4:
        if (!(PINF & 0b0010000)) return true;
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
