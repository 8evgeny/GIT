.nolist ;исключаем из листинга следующие две строки
.includepath "/usr/share/avra/" ;указали папку с инклудами
.include "2313def.inc" ;присоединили описание проца
.list ;все остальное включаем в листинг

;начало таблицы векторов прерывания

RJMP main ; RESET
RETI ; INT0
RETI ; INT1
RETI ; TIMER1_CAPT
RETI ; TIMER1_COMPA
RETI ; TIMER1_OVF
RJMP tc0i ; TIMER0_OVF
RETI ; USART_RX
RETI ; USART_UDRE
RETI ; USART_TX
RETI ; ANA_COMP
RETI ; PCINT
RETI ; TIMER1_COMPB
RETI ; TIMER0_COMPA
RETI ; TIMER0_COMPB
RETI ; USI_START
RETI ; USI_OVF
RETI ; EE_RDY
RETI ; WDT

;конец таблицы векторов прерывания

tc0i: ;обработка прерывания по переполнению таймера 0

IN   r1,SREG ;сохраняем содержимое флагового регистра
SBI PINB,0
OUT   SREG,r1 ; возвращаем изначальное значение флагового регистра
RETI ; выходим из прерывания

main: ;всякая инициализация
LDI r16,LOW(RAMEND) 
OUT SPL,r16 ; Инициализировали указатель стека
LDI r16,(1<<TOIE0)
OUT TIMSK,r16 ; разрешили прерывание при переполнении таймера 0
LDI r16,(1<<CS02)
OUT TCCR0,r16 ; настроили делитель таймера 0 на 256
LDI r16,1
OUT DDRB,r16 ; настроили порт B
SEI ; разрешили глобальные прерывания

loop: ;начало основного цикла
NOP ;не делаем ничего
RJMP loop ; переходим на начало основного цикла
