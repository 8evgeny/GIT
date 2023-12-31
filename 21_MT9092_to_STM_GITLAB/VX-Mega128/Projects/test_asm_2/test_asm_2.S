#Светодиодная мигалка  ассемблер  gnu  avr-as
#Сборка через Makefile  https://count-zero.ru/2018/gnu_assembler/

.equ	PD1, 1
.equ	PD2, 2

.org 0x00 ; начало
#include <avr/io.h>
        sbi     _SFR_IO_ADDR(DDRD), PD1;   порт PD1,2 на передачу
        sbi     _SFR_IO_ADDR(DDRD), PD2
        ldi     r25, 0b00000110;
; главный цикл
main:
        in      r24, _SFR_IO_ADDR(PORTD);  r24=PORTD
        eor     r24, r25;    r24 = r24 xor r25
        out     _SFR_IO_ADDR(PORTD), r24;  PORTD=r25
        ldi     r18, 0x3F;   r18=0x3F
        ldi     r19, 0x0D;   r19=0x0D
        ldi     r24, 0x33;   r24=0x03 Тут основная задержка
sleep:
        subi    r18, 0x01;  (r18r19r24)-1  вычитание трехбайтного целого числа
        sbci    r19, 0x00
        sbci    r24, 0x00
        brne    sleep;     если значение в r24 не равно нулю, то переход на начало операции вычитания
        rjmp    main;      возврат на начало главного цикла



