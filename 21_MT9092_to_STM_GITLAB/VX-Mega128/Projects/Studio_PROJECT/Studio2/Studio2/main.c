#include "main.h"
typedef struct __file FILE;
typedef long long fpos_t;
// �������� ������� ������ �������
static int uart_putchar(char c, FILE *stream);
// ���������� ���������� ��� ������������ ������
static FILE mystdout = FDEV_SETUP_STREAM(
uart_putchar,     // ������� ������ �������
NULL,           // ������� ����� �������, ��� ������ �� �����
_FDEV_SETUP_WRITE // ����� ������ - ������ �����
);

// ������� ������ �������
static int uart_putchar(char c, FILE *stream)
{
	//    if (c == '\n')
	//        uart_putchar('\r', stream);
	loop_until_bit_is_set(UCSR0A, UDRE0);
	UDR0 = c;
	//    special_output_port = c; //��
	return 0;
}

//// ��������� ������ �� flash
//PROGMEM char str1[] ="Format str from flash\nlong = %15ld\nFlash string = %10S\n";
//// ������ ������ �� flash
//PROGMEM char str2[] = "string form flash";

//__attribute((OS_main))






static int numLedAlive = 5;
static bool led [6];
static bool butON[6];
static bool butOFF[6];
static bool signalPressed[6];

//static int _write(int fd, char *str, int len) {
//    for(int i=0; i<len; i++)
//    {
//        uart_putchar(str[i], stdout);
//    }
//    return len;
//}

//static void Printf(const char* fmt, ...) {
//    char buff[512];
//    va_list args;
//    va_start(args, fmt);
//    vsprintf(buff, fmt, args);
//    USART_sendLine(buff);
//    va_end(args);
//}

//static void Printf(const char* fmt, ...) {
//        char buff[512];
//        va_list args;
//        va_start(args, fmt);
//        sprintf(buff, fmt, args);
//        USART_sendLine(buff);
//        va_end(args);
//}


int main() {
	GPIO_Init();
	USART0_Init();
	stdout = &mystdout;
	//    Printf("Hello, world!\n"); //���������� ���������
	//    printf("Zello, world!\n"); //���������� ���������

	// �������������� ����������� ����������
	stdout = &mystdout;
	// ��������� ������ � RAM � ������� ������ �� RAM
	//    printf("Format str from ram\nlong = %15ld\nRam string = %10s\n", 1234567890l , "string from RAM");
	// ��������� ������ � flash � ������� ������ �� flash
	//    printf_P(str1, 1234567890l , str2);



	//    SetupTIMER1(); �� ��������
	//    SetupTIMER3();
	//    sei();

	while (1) {
		_delay_ms(5);
		checkButtons();

	}//while
}


static void GPIO_Init()
{
	DDRC = 0b11111100;  //6 �����������  PC2 - PC7 �������� (1 - �� �����)
	DDRF = 0b0000000;   //6 �������  PF2 - PF7 (���������������� ����� )
	PORTF = 0b11111100; //�������� � 1
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
	//    TCNT1 = 65536-62439;    //��������� ��������, ����� ����� ���� ����� 1 �������
	TCNT1 = 45535; //������ ������������ �����
	TIMSK |= (1 << TOIE1);   // ���������� ���������� overflow ������� 1
}
static void TIMER3_Init (void){
	TCCR3B &= ~(1 << CS32);  //CSn2
	TCCR3B |= (1 << CS31); //CSn1
	TCCR3B |= (1 << CS30); //CSn0
	ETIMSK |= (1 << TOIE3);  // ���������� ���������� overflow ������� 3
}
static ISR (TIMER1_OVF_vect){
	TCNT1 = 0;          //��� ����� ����� � 65535  ��� ������� ��������� ������ 1 (LED ON)
	setLed(numLedAlive);
	TCNT3 = 0;          //��� ����� ����� � 65535  ��� ������� ��������� ������ 3 (LED OFF)
}
static ISR (TIMER3_OVF_vect) {
	resetLed(numLedAlive);
	++numLedAlive;
	if (numLedAlive == 7)
	numLedAlive = 1;
}
static void USART0_Init() {
	UBRR0H = (unsigned char) (BRC >> 8);  // ���� UART0, �������� = BUAD
	UBRR0L = (unsigned char)  BRC;

	UCSR0B |= (1 << TXEN); //���������� ��������
	UCSR0C |= (1 << UCSZ1) | (1 << UCSZ0); //8 ���
}
static unsigned char USART0_Receive( void ) {
	while ( !(UCSR0A & (1<<RXC0)));         //Wait for data to be received
	return UDR0;                            //Get and return received data from buffer
}
static void USART_sendLine(char *string) {
	while ( *string ) {
		//        uart_putchar(*string, stdout);
		USART_sendChar(*string); // ����������� ���������� ������
		string++;
	}
}
static char USART_receiveChar(void) {
	return ( (UCSR0A >> RXC0) & 1 ) ? UDR0 : 0;  // ���������� �������� ������ �����
}
// �������� ASCII �������
static void USART_sendChar(char character) {
	loop_until_bit_is_set(UCSR0A, UDRE0);
	//    while ( !( UCSR0A & (1<<UDRE0)));   //Wait for empty transmit buffer
	UDR0 = character;                        //Put data into buffer, sends the data
}

static void checkButtons(){
	for (int i = 0; i < 6; ++i) {
		if (Buttons[i]) { //���� ����� ����������
			if (checkButton(i))
			setLed(i);
			else
			resetLed(i);

			if (butON[i] && butOFF[i]) {
				switch (i) {
					case 0: {
						USART_sendLine("Button 1 pressed\r\n");
						//��� ������ ��������� � ������� 1 ������
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
			}//������� � ������� ������

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
			}//������� �� ���������� ������



		}//���� ����� ����������
	}
}


