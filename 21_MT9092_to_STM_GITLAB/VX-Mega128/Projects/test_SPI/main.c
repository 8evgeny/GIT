#include <avr/io.h>
#define F_CPU 14745600UL
#include <util/delay.h>
#include <avr/interrupt.h>
#define PIN_INT0 PD0

#define SPI_MISO 3
#define SPI_MOSI 2
#define SPI_SCK 1
#define SPI_SS 0

uint8_t flagButton;
//Процессор полностью свободен - прерывания от двух таймеров
//Добавляем прерывания от кнопки
//Добавляем 4 кнопки - чтобы выбрать кнопки нужно PC2 - PC7 в 1  PG0 - 1  PG1  в 0
//Добавляю UART
//Добавляю SPI
void SetupTIMER1 (void);
void SetupTIMER3 (void);
void SetupInt0 (void);
void SetupGPIO (void);
void USART0_Init(void);
void USART0_Transmit( unsigned char data );
unsigned char USART0_Receive(void);
void USART_sendChar(char character);
void USART_sendLine(char *string);
char USART_receiveChar(void);
void SPI_Init(void);
void SPI_WriteByte(uint8_t data);
uint8_t SPI_WriteReadByte(uint8_t data);
void SPI_WriteArray(uint8_t num, uint8_t *data);
void SPI_ReadArray(uint8_t num, uint8_t *data);

int main(void) {
    SetupGPIO();
    SetupTIMER1();
    SetupTIMER3();
    SetupInt0();
    sei();
    USART0_Init();
    SPI_Init();

    uint8_t bufSPI[10] = {11, 12, 13, 14, 15, 16, 17, 18, 19, 0x00};
    while (1) {
        if (!(PINA & 0b00010000)) { //Button 1
            PORTD = 0b00000110;
        }
        if (!(PINA & 0b00100000)) { //Button 2
            PORTD = 0b00000110;
        }
        if (!(PINA & 0b01000000)) { //Button 3
            PORTD = 0b00000110;
        }
        if (!(PINA & 0b10000000)) { //Button 4
            PORTD = 0b00000110;
        }
        USART_sendLine("Test USART0\r\n");
//        SPI_WriteArray(10, bufSPI);
//        SPI_WriteByte(0x11);
    }
}

void SetupGPIO (void) {
    DDRD |= ( 1 << 1 );     //PD1 PD2  как выходы
    DDRD |= ( 1 << 2 );
    DDRD &= ~(1<<PIN_INT0);    //PD0  как вход
    DDRA &= ~(1 << 4);    //как входы
    DDRA &= ~(1 << 5);
    DDRA &= ~(1 << 6);
    DDRA &= ~(1 << 7);
    PORTA = 0b11110000;  //Подтяжка к 1
    PORTD = 0b00000110;
    DDRC = 0b11111111;     //PC0 - PC7  как выходы
    PORTC = 0b11111111;    // в 1
//PG0 - 1  PG1  в 0
    DDRG = 0b11111111;
    PORTG = 0b11111101;
}

void SetupTIMER1 (void) {
    TCCR1B |= (1 << CS12);  //Fclk/256
    TCCR1B |= (1 << CS10);  // если добавить то - Fclk/1024
//    TCNT1 = 65536-62439;    //коррекция счетчика, чтобы время было ровно 1 секунда
    TCNT1 = 65535; //Первое срабатывание сразу
    TIMSK |= (1 << TOIE1);   // Разрешение прерывания overflow таймера 1
}

void SetupTIMER3 (void) {
    TCCR3B |= (1 << CS32);  //Fclk/256
    ETIMSK |= (1 << TOIE3);  // Разрешение прерывания overflow таймера 3
}

void SetupInt0 (void) {
    EIMSK |=  (1<<INT0);    //Разрешаем внешнее прерывание INT0
    EICRA |= (1<<ISC01)|(0<<ISC00);  //настраиваем условие прерывания
}

ISR (TIMER1_OVF_vect) {
    TCNT1 = 25000;          //Чем число ближе к 65535  тем быстрее сработает таймер 1 (LED ON)
    PORTD |= 0b00000010;
    PORTD &= ~0b00000100;
    TCNT3 = 65000;          //Чем число ближе к 65535  тем быстрее сработает таймер 3 (LED OFF)
}

ISR (TIMER3_OVF_vect) {
        PORTD &= 0b11111101;
    }

ISR (INT0_vect) {
    switch (flagButton) {
    case 0: flagButton = 1; PORTD |= 0b00000100;
        break;
    case 1: flagButton = 0; PORTD &= 0b11111011;
        break;
    default:
        break;
    }
}

// Отправка ASCII символа
void USART_sendChar(char character)
{
    while ( !( UCSR0A & (1<<UDRE0)));   //Wait for empty transmit buffer
    UDR0 = character;                        //Put data into buffer, sends the data
}
#if 0
// Отправка единичного HEX символа
void USART_sendHex(uint8_t number)
{
    if ( number < 10 )
        USART_sendChar(48 + (number & 15)); // отправка [0-9]
    else if	( number < 16 )
        USART_sendChar(55 + (number & 15)); // отправка [A-F]
}
// Отправка байта в HEX формате
void USART_sendByteHex(uint8_t number)
{
    USART_sendHex(number >> 4); // старшие 4 бита
    USART_sendHex(number & 15); // младшие 4 бита
}
#endif

void USART_sendLine(char *string) {
    while ( *string ) {
        USART_sendChar(*string); // посимвольно отправляем строку
        string++;
    }
}

char USART_receiveChar(void) {
    return ( (UCSR0A >> RXC0) & 1 ) ? UDR0 : 0;  // возвращаем значение буфера приёма
}

void USART0_Init() {
    uint16_t ubrr;
    ubrr = F_CPU/115200/16-1;           // скорость обмена 115200 бит/с
    UBRR0H = (unsigned char)(ubrr>>8);
    UBRR0L = (unsigned char)ubrr;
    UCSR0B = (1<<RXEN0)|(1<<TXEN0);     // включаем приёмник и передатчик
    UCSR0C = (1<<USBS0)|(3<<UCSZ00);    // асинхронный режим, размер посылки 8 бит, проверка чётности отключена, 1 стоп-бит
}

void USART0_Transmit( unsigned char data ) {
    while ( !( UCSR0A & (1<<UDRE0)));   //Wait for empty transmit buffer
    UDR0 = data;                        //Put data into buffer, sends the data
}

unsigned char USART0_Receive( void ) {
    while ( !(UCSR0A & (1<<RXC0)));         //Wait for data to be received
    return UDR0;                            //Get and return received data from buffer
}

/*инициализация SPI модуля в режиме master*/
void SPI_Init(void) {
   /*настройка портов ввода-вывода
   все выводы, кроме MISO выходы*/
   DDRB |= (1<<SPI_MOSI)|(1<<SPI_SCK)|(1<<SPI_SS)|(0<<SPI_MISO);
   PORTB |= (1<<SPI_MOSI)|(1<<SPI_SCK)|(1<<SPI_SS)|(1<<SPI_MISO);

   /*разрешение spi,старший бит вперед,мастер, режим 0*/
   SPCR = (1<<SPE)|(0<<DORD)|(1<<MSTR)|(0<<CPOL)|(0<<CPHA)|(1<<SPR1)|(0<<SPR0);
   SPSR = (0<<SPI2X);
}

//Передача одного байта данных по SPI
void SPI_WriteByte(uint8_t data) {
   PORTB &= ~(1<<SPI_SS);
   SPDR = data;
   while(!(SPSR & (1<<SPIF)));
   PORTB |= (1<<SPI_SS);
}

//Передача и прием одного байта данных по SPI
uint8_t SPI_WriteReadByte(uint8_t data) {
   uint8_t report;
   PORTB &= ~(1<<SPI_SS);
   SPDR = data;
   while(!(SPSR & (1<<SPIF)));
   report = SPDR;
   PORTB |= (1<<SPI_SS);
   return report;
}

//Передача нескольких байтов данных по SPI *data – указатель на массив передаваемых данных, а num – размерность массива
void SPI_WriteArray(uint8_t num, uint8_t *data) {
   PORTB &= ~(1<<SPI_SS);
   while(num--){
      SPDR = *data++;
      while(!(SPSR & (1<<SPIF)));
   }
   PORTB |= (1<<SPI_SS);
}

//Передачи и прием нескольких байтов данных по SPI *data – указатель на массив передаваемых данных, а num – размерность массива.
//Принятые данные будут сохраняться в том же массиве.
void SPI_ReadArray(uint8_t num, uint8_t *data) {
   PORTB &= ~(1<<SPI_SS);
   while(num--){
      SPDR = *data;
      while(!(SPSR & (1<<SPIF)));
      *data++ = SPDR;
   }
   PORTB |= (1<<SPI_SS);
}
