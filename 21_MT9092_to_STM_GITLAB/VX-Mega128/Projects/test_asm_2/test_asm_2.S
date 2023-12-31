#Светодиодная мигалка  ассемблер  gnu  avr-as
#Сборка через Makefile  https://count-zero.ru/2018/gnu_assembler/

.equ	PD1, 1
.equ	PD2, 2

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

reset:
        eor	r1, r1
        out	0x3f, r1	; 63
        ldi	r28, 0xFF	; 255
        ldi	r29, 0x10	; 16
        out	0x3e, r29	; 62
        out	0x3d, r28	; 61
        rcall	main     	; 0x9e <main>
        rjmp	exit     	; 0xe0 <_exit>

interrupt:
        rjmp	vectors    	; 0x0 <__vectors>

main:
        sbi     _SFR_IO_ADDR(DDRD), PD1;   порт PD1,2 на передачу
        sbi     _SFR_IO_ADDR(DDRD), PD2
        ldi     r25, 0b00000110;
        in      r24, _SFR_IO_ADDR(PORTD);  r24=PORTD
        eor     r24, r25;    r24 = r24 xor r25
        out     _SFR_IO_ADDR(PORTD), r24;  PORTD=r25
        ldi     r18, 0x3F;   r18=0x3F
        ldi     r19, 0x0D;   r19=0x0D
        ldi     r24, 0x13;   r24=0x03 Тут основная задержка
sleep:
        subi    r18, 0x01;  (r18r19r24)-1  вычитание трехбайтного целого числа
        sbci    r19, 0x00
        sbci    r24, 0x00
        brne    sleep;     если значение в r24 не равно нулю, то переход на начало операции вычитания
        rjmp    main;      возврат на начало главного цикла

exit:
        cli

