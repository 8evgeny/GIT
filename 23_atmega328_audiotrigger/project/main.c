#include "main.h"
#include <stdio.h>
    uint8_t del_1;
    uint8_t del_2;
    uint8_t hold_1;
    uint8_t hold_2;
    char tmp[100];
    uint8_t tmp2 = 0;
    unsigned int adc_value;
int main(void) {

    GPIO_Init();
    USART0_Init();
    blink();
    ADC_Init(); //Инициализируем АЦП

    _delay_ms(1000);
    readEncoders();

    while (1) {
//        USART_sendLine("Test UART\r\n");
        _delay_ms(1000);
        adc_value = ADC_convert(); //Вызовем преобразование
        printADC();
        readEncoders();
        printEncoders();
        blink();
    }
}

static void GPIO_Init() {
    DDRB &= ~(1 << PORTB0); //PB0 PB1 PB2 PB3  входы
    DDRB &= ~(1 << PORTB1);
    DDRB &= ~(1 << PORTB2);
    DDRB &= ~(1 << PORTB3);
    PORTB |= (1 << PORTB0); //Подтяжка входов к 1
    PORTB |= (1 << PORTB1);
    PORTB |= (1 << PORTB2);
    PORTB |= (1 << PORTB3);
    DDRB |= (1 << PORTB4); //PB4 PB5 выходы
    DDRB |= (1 << PORTB5);

    DDRC |= (1 << PORTC4); //PC4 PC5 выходы
    DDRC |= (1 << PORTC5);

    DDRD |= (1 << PORTD2); //PD2 PD3 выходы
    DDRD |= (1 << PORTD3);
    DDRD &= ~(1 << PORTD4); //PD4 PD5 PD6 PD7  входы
    DDRD &= ~(1 << PORTD5);
    DDRD &= ~(1 << PORTD6);
    DDRD &= ~(1 << PORTD7);
    PORTD |= (1 << PORTD4); //Подтяжка входов к 1
    PORTD |= (1 << PORTD5);
    PORTD |= (1 << PORTD6);
    PORTD |= (1 << PORTD7);
}
static void pinON(int num) {
    PORTC |= (1 << num);
}
static void pinOFF(int num) {
    PORTC &= ~(1 << num);
}
static void blink(void){
    pinON(led1);
    _delay_ms(50);
    pinOFF(led1);
    _delay_ms(50);
    pinON(led2);
    _delay_ms(50);
    pinOFF(led2);
    _delay_ms(50);
}
static void readEncoders(void){

    readEncoderHold1();
    readEncoderHold2();
    readEncoderDel1();
    readEncoderDel2();
}
static void printEncoders(void){

    sprintf(tmp, "1 channel: delay %d\thold %d\r\n2 channel: delay %d\thold %d\r\n", del_1, hold_1, del_2, hold_2);
    USART_sendLine(tmp);
    USART_sendLine("\r\n");
}
static void readEncoderDel1(){
    PORTB &= ~(1 << PORTB4);
    _delay_ms(10);
    del_1 = (~PINB) & 0b00001111;
    _delay_ms(10);
    PORTB |= (1 << PORTB4);
}
static void readEncoderHold1(){
    PORTB &= ~(1 << PORTB5);
    _delay_ms(10);
    hold_1 = (~PINB) & 0b00001111;
    _delay_ms(10);
    PORTB |= (1 << PORTB5);
}
static void readEncoderDel2(){
    PORTD &= ~(1 << PORTD3);
    _delay_ms(10);
    del_2 = ((~PIND) & 0b11110000) >> 4 ;
    _delay_ms(10);
    PORTD |= (1 << PORTD3);
}
static void readEncoderHold2(){
    PORTD &= ~(1 << PORTD2);
    _delay_ms(10);
    hold_2 = ((~PIND) & 0b11110000) >> 4 ;
    _delay_ms(10);
    PORTD |= (1 << PORTD2);
}
static void USART0_Init() {
    uint16_t ubrr;
    ubrr = F_CPU/9600/16-1;           // скорость обмена 115200 бит/с
    UBRR0H = (unsigned char)(ubrr>>8);
    UBRR0L = (unsigned char)ubrr;
    UCSR0B = (1<<RXEN0)|(1<<TXEN0);     // включаем приёмник и передатчик
    UCSR0C = (1<<USBS0)|(3<<UCSZ00);    // асинхронный режим, размер посылки 8 бит, проверка чётности отключена, 1 стоп-бит
}
static void USART_sendChar(char character){
    while ( !( UCSR0A & (1<<UDRE0)));   //Wait for empty transmit buffer
    UDR0 = character;                        //Put data into buffer, sends the data
}
static void USART_sendLine(char *string) {
    while ( *string ) {
        USART_sendChar(*string); // посимвольно отправляем строку
        string++;
    }
}
static char USART_receiveChar(void) {
    return ( (UCSR0A >> RXC0) & 1 ) ? UDR0 : 0;  // возвращаем значение буфера приёма
}
static void USART0_Transmit( unsigned char data ) {
    while ( !( UCSR0A & (1<<UDRE0)));   //Wait for empty transmit buffer
    UDR0 = data;                        //Put data into buffer, sends the data
}
static unsigned char USART0_Receive( void ) {
    while ( !(UCSR0A & (1<<RXC0)));         //Wait for data to be received
    return UDR0;                            //Get and return received data from buffer
}
static void printADC(void){
    sprintf(tmp, "ADC: %d\r\n", adc_value);
    USART_sendLine(tmp);
}
