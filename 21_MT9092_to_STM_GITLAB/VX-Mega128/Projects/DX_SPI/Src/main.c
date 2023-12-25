#include "main.h"

static int numLedAlive = 5;
static bool led [6];
static bool butON[6];
static bool butOFF[6];
static bool signalPressed[6];

int main() {
    GPIO_Init();
    USART0_Init();
    SPI_Init();
    stdout = &mystdout;

//    SetupTIMER1(); не работает
//    SetupTIMER3();
//    sei();

    while (1) {
        _delay_ms(10);
        checkButtons();
        SPI_WriteString("55");
    }//while
}

static void GPIO_Init()
{
    DDRC = 0b11111100;  //6 светодиодов  PC2 - PC7 инверсия (1 - не горит)
    DDRF = 0b0000000;   //6 рычагов  PF2 - PF7 (предположительно нулем )
    PORTF = 0b11111100; //Подтяжка к 1
    PORTC = 0b11111111;
}
static bool checkButton(int num){ //0 - 5
    switch(num) {
    case 0:
        if (!(PINF & 0b00000100)) {
            butON[num] = true;
            return true;
        }
        else {
            butOFF[num] = true; butON[num] = false;
            return false;
        }
        break;
    case 1:
        if (!(PINF & 0b00001000)) {
            butON[num] = true;
            return true;
        }
        else {
            butOFF[num] = true; butON[num] = false;
            return false;
        }
        break;
    case 2:
        if (!(PINF & 0b00010000)) {
            butON[num] = true;
            return true;
        }
        else {
            butOFF[num] = true; butON[num] = false;
            return false;
        }
        break;
    case 3:
        if (!(PINF & 0b00100000)) {
            butON[num] = true;
            return true;
        }
        else {
            butOFF[num] = true; butON[num] = false;
            return false;
        }
        break;
    case 4:
        if (!(PINF & 0b01000000)) {
            butON[num] = true;
            return true;
        }
        else {
            butOFF[num] = true; butON[num] = false;
            return false;
        }
        break;
    case 5:
        if (!(PINF & 0b10000000)) {
            butON[num] = true;
            return true;
        }
        else {
            butOFF[num] = true; butON[num] = false;
            return false;
        }
        break;
    default :
        break;
    }
    return false;
}
static void setLed(int num)
{
    switch(num) {
        case 0:
            PORTC &= 0b11111011;
            led[num] = true;
            break;
        case 1:
            PORTC &= 0b11110111;
            led[num] = true;
            break;
        case 2:
            PORTC &= 0b11101111;
            led[num] = true;
            break;
        case 3:
            PORTC &= 0b11011111;
            led[num] = true;
            break;
        case 4:
            PORTC &= 0b10111111;
            led[num] = true;
            break;
        case 5:
            PORTC &= 0b01111111;
            led[num] = true;
            break;
        default :
            break;
    }
}
static void resetLed(int num)
{
    switch(num) {
        case 0:
            PORTC |= 0b00000100;
            led[num] = false;
            break;
        case 1:
            PORTC |= 0b00001000;
            led[num] = false;
            break;
        case 2:
            PORTC |= 0b00010000;
            led[num] = false;
            break;
        case 3:
            PORTC |= 0b00100000;
            led[num] = false;
            break;
        case 4:
            PORTC |= 0b01000000;
            led[num] = false;
            break;
        case 5:
            PORTC |= 0b10000000;
            led[num] = false;
            break;
        default :
            break;
    }
}
static void TIMER1_Init (void){
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
static void TIMER3_Init (void){
    TCCR3B &= ~(1 << CS32);  //CSn2
    TCCR3B |= (1 << CS31); //CSn1
    TCCR3B |= (1 << CS30); //CSn0
    ETIMSK |= (1 << TOIE3);  // Разрешение прерывания overflow таймера 3
}
static ISR (TIMER1_OVF_vect){
    TCNT1 = 0;          //Чем число ближе к 65535  тем быстрее сработает таймер 1 (LED ON)
    setLed(numLedAlive);
    TCNT3 = 0;          //Чем число ближе к 65535  тем быстрее сработает таймер 3 (LED OFF)
}
static ISR (TIMER3_OVF_vect) {
    resetLed(numLedAlive);
    ++numLedAlive;
    if (numLedAlive == 7)
        numLedAlive = 1;
}
static void USART0_Init() {
    UBRR0H = (unsigned char) (BRC >> 8);  // порт UART0, скорость = BUAD
    UBRR0L = (unsigned char)  BRC;

    UCSR0B |= (1 << TXEN); //разрешение передачи
    UCSR0C |= (1 << UCSZ1) | (1 << UCSZ0); //8 бит
}
static unsigned char USART0_Receive( void ) {
    while ( !(UCSR0A & (1<<RXC0)));         //Wait for data to be received
    return UDR0;                            //Get and return received data from buffer
}
static void USART_sendLine(char *string) {
    while ( *string ) {
//        uart_putchar(*string, stdout);
        USART_sendChar(*string); // посимвольно отправляем строку
        string++;
    }
}
static char USART_receiveChar(void) {
    return ( (UCSR0A >> RXC0) & 1 ) ? UDR0 : 0;  // возвращаем значение буфера приёма
}
static void USART_sendChar(char character) {
    loop_until_bit_is_set(UCSR0A, UDRE0);
//    while ( !( UCSR0A & (1<<UDRE0)));   //Wait for empty transmit buffer
    UDR0 = character;                        //Put data into buffer, sends the data
}
static void checkButtons(){
    for (int i = 0; i < 6; ++i) {
        if (Buttons[i]) { //Если рычаг существует
            if (checkButton(i))
                setLed(i);
            else
                resetLed(i);

            if (butON[i] && butOFF[i]) {
                switch (i) {
                case 0: {
                    USART_sendLine("Button 1 pressed\r\n");
                    //Тут сигнал одиночный о нажатии 1 рычага
                    signalPressed[i] = 1;
                }
                break;
                case 1: {
                    USART_sendLine("Button 2 pressed\r\n");
                    signalPressed[i] = 1;
                }
                break;
                case 2: {
                    USART_sendLine("Button 3 pressed\r\n");
                    signalPressed[i] = 1;
                }
                break;
                case 3: {
                    USART_sendLine("Button 4 pressed\r\n");
                    signalPressed[i] = 1;
                }
                break;
                case 4: {
                    USART_sendLine("Button 5 pressed\r\n");
                    signalPressed[i] = 1;
                }
                break;
                case 5: {
                    USART_sendLine("Button 6 pressed\r\n");
                    signalPressed[i] = 1;
                }
                break;
                }
                butOFF[i] = 0;
            }//Сигналы о нажатии рычага

            if (signalPressed[i] && !butON[i]) {
                switch (i) {
                case 0: {
                    USART_sendLine("Button 1 relesed\r\n");
                    signalPressed[i] = 0;
                }
                break;
                case 1: {
                    USART_sendLine("Button 2 relesed\r\n");
                    signalPressed[i] = 0;
                }
                break;
                case 2: {
                    USART_sendLine("Button 3 relesed\r\n");
                    signalPressed[i] = 0;
                }
                break;
                case 3: {
                    USART_sendLine("Button 4 relesed\r\n");
                    signalPressed[i] = 0;
                }
                break;
                case 4: {
                    USART_sendLine("Button 5 relesed\r\n");
                    signalPressed[i] = 0;
                }
                break;
                case 5: {
                    USART_sendLine("Button 6 relesed\r\n");
                    signalPressed[i] = 0;
                }
                break;
                }
            }//Сигналы об отпускании рычага



        }//Если рычаг существует
    }
}
static int uart_putchar(char c, FILE *stream)
{
    //    if (c == '\n')
    //        uart_putchar('\r', stream);
    loop_until_bit_is_set(UCSR0A, UDRE0);
    UDR0 = c;
    return 0;
}

void SPI_Init(void) {/*инициализация SPI модуля в режиме master*/
    /*настройка портов ввода-вывода
   все выводы, кроме MISO выходы*/
    DDRB |= (1<<SPI_MOSI)|(1<<SPI_SCK)|(1<<SPI_SS)|(0<<SPI_MISO);
    //   PORTB |= (1<<SPI_MOSI)|(1<<SPI_SCK)|(1<<SPI_SS)|(1<<SPI_MISO);
    /*разрешение прерываний и spi,старший бит вперед, мастер, режим 0*/
    SPCR = /*(1<<SPIE)|*/(1<<SPE)|(0<<DORD)|(1<<MSTR)|(1<<CPOL)|(0<<CPHA)|(0<<SPR1)|(0<<SPR0); //Fclk/4
    SPSR &= ~(1<<SPI2X);
}
void SPI_WriteByte(uint8_t data) {//Передача одного байта данных по SPI
    PORTB &= ~(1<<SPI_SS);
    _delay_us(2);
    SPDR = data;
    while(!(SPSR & (1<<SPIF)));
    _delay_us(2);
    PORTB |= (1<<SPI_SS);
    _delay_us(5);
}
void SPI_WriteString(char *string) {
    while ( *string ) {
        SPI_WriteByte(*string); // посимвольно отправляем строку
        string++;
    }
}

