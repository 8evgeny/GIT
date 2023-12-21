#include "main.h"

int main(void) {
    SetupGPIO();
    while (1) {
        if (!(PINF & 0b10000000)){ //Button 6  загорается  - нулем
            PORTC &= 0b01111111;
        }
        if (PINF | 0b10000000){ //Button 6  гаснет  - единицей
            PORTC |= 0b10000000;
        }
        if (!(PINF & 0b01000000)){ //Button 5
            PORTC &= 0b10111111;
        }
        if (PINF | 0b01000000){
            PORTC |= 0b01000000;
        }
    }
}
