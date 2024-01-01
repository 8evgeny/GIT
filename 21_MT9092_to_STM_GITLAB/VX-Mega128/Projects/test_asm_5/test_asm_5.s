;Светодиодная мигалка  ассемблер  gnu  avr-as  https://count-zero.ru/2014/blink_asm/
;Сборка CMAKE

.equ	PD1, 1
.equ	PD2, 2

.equ XL, 26
.equ XH, 27
.equ YL, 28
.equ YH, 29
.equ ZL, 30
.equ ZH, 31
.equ SPL, 0x3D
.equ SPH, 0x3E
.equ SREG, 0x3F
.equ    DDRD, 0x11
.equ    PORTD, 0x12

.org 0x00 ; начало
#include <avr/io.h>
vectors:
        rjmp	reset   	; 0x8c <__ctors_end>
        nop
        rjmp	interrupt    	; 0x9c <__bad_interrupt>
        nop
        rjmp	interrupt    	; 0x9c <__bad_interrupt>
        nop
        rjmp	interrupt    	; 0x9c <__bad_interrupt>
        nop
        rjmp	interrupt    	; 0x9c <__bad_interrupt>
        nop
        rjmp	interrupt   	; 0x9c <__bad_interrupt>
        nop
        rjmp	interrupt    	; 0x9c <__bad_interrupt>
        nop
        rjmp	interrupt    	; 0x9c <__bad_interrupt>
        nop
        rjmp	interrupt    	; 0x9c <__bad_interrupt>
        nop
        rjmp	interrupt    	; 0x9c <__bad_interrupt>
        nop
        rjmp	interrupt    	; 0x9c <__bad_interrupt>
        nop
        rjmp	interrupt    	; 0x9c <__bad_interrupt>
        nop
        rjmp	interrupt    	; 0x9c <__bad_interrupt>
        nop
        rjmp	interrupt    	; 0x9c <__bad_interrupt>
        nop
        rjmp	interrupt     	; 0x9c <__bad_interrupt>
        nop
        rjmp	interrupt     	; 0x9c <__bad_interrupt>
        nop
        rjmp	interrupt     	; 0x9c <__bad_interrupt>
        nop
        rjmp	interrupt     	; 0x9c <__bad_interrupt>
        nop
        rjmp	interrupt     	; 0x9c <__bad_interrupt>
        nop
        rjmp	interrupt     	; 0x9c <__bad_interrupt>
        nop
        rjmp	interrupt     	; 0x9c <__bad_interrupt>
        nop
        rjmp	interrupt     	; 0x9c <__bad_interrupt>
        nop
        rjmp	interrupt     	; 0x9c <__bad_interrupt>
        nop
        rjmp	interrupt     	; 0x9c <__bad_interrupt>
        nop
        rjmp	interrupt     	; 0x9c <__bad_interrupt>
        nop
        rjmp	interrupt     	; 0x9c <__bad_interrupt>
        nop
        rjmp	interrupt     	; 0x9c <__bad_interrupt>
        nop
        rjmp	interrupt     	; 0x9c <__bad_interrupt>
        nop
        rjmp	interrupt    	; 0x9c <__bad_interrupt>
        nop
        rjmp	interrupt     	; 0x9c <__bad_interrupt>
        nop
        rjmp	interrupt     	; 0x9c <__bad_interrupt>
        nop
        rjmp	interrupt     	; 0x9c <__bad_interrupt>
        nop
        rjmp	interrupt     	; 0x9c <__bad_interrupt>
        nop
        rjmp	interrupt     	; 0x9c <__bad_interrupt>
        nop
        rjmp	interrupt     	; 0x9c <__bad_interrupt>

interrupt:
        rjmp	vectors       	; 0x0 <__vectors>

reset:
        eor	r1, r1
        out	_SFR_IO_ADDR(SREG), r1	; 63 /* 0x3F SREG */
        ldi	r28, 0xFF	; 255
        ldi	r29, 0x10	; 16
        out	_SFR_IO_ADDR(SPH), r29	; 62 /* 0x3D..0x3E SP */
        out	_SFR_IO_ADDR(SPL), r28	; 61
        rcall	main     	; 0x9e <main>
        rjmp	exit     	; 0xe0 <_exit>

main:
        sbi     0x11, PD1;   порт PD1,2 на передачу
        sbi     0x11, PD2
        ldi     r25, 0b00000110;
        in      r24, 0x12;  r24=PORTD
        eor     r24, r25;    r24 = r24 xor r25
        out     0x12, r24;  PORTD=r25
        ldi     r18, 0x3F;   r18=0x3F
        ldi     r19, 0x0D;   r19=0x0D
        ldi     r24, 0x13;   Тут основная задержка

sleep: ;Код IDA  такой-же
        subi    r18, 0x01;  (r18r19r24)-1  вычитание трехбайтного целого числа
        sbci    r19, 0x00
        sbci    r24, 0x00
        brne    sleep;     если значение в r24 не равно нулю, то переход на начало операции вычитания
        rjmp    main;      возврат на начало главного цикла

exit:
        cli



