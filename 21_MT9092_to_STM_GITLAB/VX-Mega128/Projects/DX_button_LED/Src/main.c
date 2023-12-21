#include "main.h"

int numLedAlive = 5;
bool led1;
bool led2;
bool led3;
bool led4;
bool led5;
bool led6;

int main() {
    GPIO_Init();
    USART0_Init();

//    SetupTIMER1(); не работает
//    SetupTIMER3();
//    sei();

    while (1) {
        for (int i = 1; i <= 6; ++i) {
//            setLed(i);
//            _delay_ms(5);
//            resetLed(i);

            if (Levers[i-1]) {
                if (checkLever(i)) setLed(i);
                else resetLed(i);
            }

            _delay_ms(5);
//            USART_sendLine("Test USART0\r\n");
        }
    }//while
}


void GPIO_Init()
{
    DDRC = 0b11111100;  //6 светодиодов  PC2 - PC7 инверсия (1 - не горит)
    DDRF = 0b0000000;   //6 рычагов  PF2 - PF7 (предположительно нулем )
    PORTF = 0b11111100; //Подтяжка к 1
    PORTC = 0b11111111;
}
bool checkLever(int num){ //1 - 6
    switch(num) {
    case 1:
        if (!(PINF & 0b00000100)) {
            USART_sendLine("lever 1 pressed\r\n");
            return true;
        }
       else return false;
        break;
    case 2:
        if (!(PINF & 0b00001000)) {
            USART_sendLine("lever 2 pressed\r\n");
            return true;
        }
        else return false;
        break;
    case 3:
        if (!(PINF & 0b00010000)) return true;
        else return false;
        break;
    case 4:
        if (!(PINF & 0b00100000)) return true;
        else return false;
        break;
    case 5:
        if (!(PINF & 0b01000000)) return true;
        else return false;
        break;
    case 6:
        if (!(PINF & 0b10000000)) return true;
        else return false;
        break;
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
            led1 = true;
            break;
        case 2:
            PORTC &= 0b11110111;
            led2 = true;
            break;
        case 3:
            PORTC &= 0b11101111;
            led3 = true;
            break;
        case 4:
            PORTC &= 0b11011111;
            led4 = true;
            break;
        case 5:
            PORTC &= 0b10111111;
            led5 = true;
            break;
        case 6:
            PORTC &= 0b01111111;
            led6 = true;
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
            led1 = false;
            break;
        case 2:
            PORTC |= 0b00001000;
            led2 = false;
            break;
        case 3:
            PORTC |= 0b00010000;
            led3 = false;
            break;
        case 4:
            PORTC |= 0b00100000;
            led4 = false;
            break;
        case 5:
            PORTC |= 0b01000000;
            led5 = false;
            break;
        case 6:
            PORTC |= 0b10000000;
            led6 = false;
            break;
        default :
            break;
    }
}
void TIMER1_Init (void){
#if 0
CSn2 CSn1 CSn0 Description
0 0 0 No clock source. (Timer/Counter stopped)
0 0 1 clkI/O /1 (No prescaling
0 1 0 clkI/O /8 (From prescaler)
0 1 1 clkI/O /64 (From prescaler)
1 0 0 clkI/O /256 (From prescaler)
1 0 1 clkI/O /1024 (From prescaler)
1 1 0 External clock source on Tn pin. Clock on falling edge
1 1 1 External clock source on Tn pin. Clock on rising edge
#endif
    TCCR1B |= (1 << CS12);  //CSn2
    TCCR1B &= ~(1 << CS11); //CSn1
    TCCR1B |= (1 << CS10); //CSn0
    //    TCNT1 = 65536-62439;    //коррекция счетчика, чтобы время было ровно 1 секунда
    TCNT1 = 45535; //Первое срабатывание сразу
    TIMSK |= (1 << TOIE1);   // Разрешение прерывания overflow таймера 1
}
void TIMER3_Init (void){
    TCCR3B &= ~(1 << CS32);  //CSn2
    TCCR3B |= (1 << CS31); //CSn1
    TCCR3B |= (1 << CS30); //CSn0
    ETIMSK |= (1 << TOIE3);  // Разрешение прерывания overflow таймера 3
}
ISR (TIMER1_OVF_vect){
    TCNT1 = 0;          //Чем число ближе к 65535  тем быстрее сработает таймер 1 (LED ON)
    setLed(numLedAlive);
    TCNT3 = 0;          //Чем число ближе к 65535  тем быстрее сработает таймер 3 (LED OFF)
}
ISR (TIMER3_OVF_vect) {
    resetLed(numLedAlive);
    ++numLedAlive;
    if (numLedAlive == 7)
        numLedAlive = 1;
}
void USART0_Init() {
    UBRR0H = (unsigned char) (BRC >> 8);  // порт UART0, скорость = BUAD
    UBRR0L = (unsigned char)  BRC;

    UCSR0B |= (1 << TXEN); //разрешение передачи
    UCSR0C |= (1 << UCSZ1) | (1 << UCSZ0); //8 бит
}
unsigned char USART0_Receive( void ) {
    while ( !(UCSR0A & (1<<RXC0)));         //Wait for data to be received
    return UDR0;                            //Get and return received data from buffer
}
void USART_sendLine(char *string) {
    while ( *string ) {
        USART_sendChar(*string); // посимвольно отправляем строку
        string++;
    }
}
char USART_receiveChar(void) {
    return ( (UCSR0A >> RXC0) & 1 ) ? UDR0 : 0;  // возвращаем значение буфера приёма
}
// Отправка ASCII символа
void USART_sendChar(char character) {
    while ( !( UCSR0A & (1<<UDRE0)));   //Wait for empty transmit buffer
    UDR0 = character;                        //Put data into buffer, sends the data
}
