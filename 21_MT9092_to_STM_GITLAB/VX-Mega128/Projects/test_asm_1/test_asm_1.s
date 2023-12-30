#Светодиодная мигалка  ассемблер  gnu -  avr-as

#.list                          ; включить генерацию листинга

#.org    0x0000                     ; начальное значение для адресации
#.equ    XL, 26
#.equ    XH, 27
#.equ    YL, 28
#.equ    YH, 29
#.equ    ZL, 30
#.equ    ZH, 31
#.equ    RAMEND, 0x10ff
#.equ	PORTD, 0x12
#.equ	PORTD1, 1
#.equ	PORTD2, 2
#.equ	SPH, 0x3e
#.equ	SPL, 0x3d
#.equ	DDRD, 0x11

#; -- инициализация стека --
##LDI R16, Low(RAMEND)  ; младший байт конечного адреса ОЗУ в R16
#LDI R16, 0xFF
#OUT SPL, R16          ; установка младшего байта указателя стека
##LDI R16, High(RAMEND) ; старший байт конечного адреса ОЗУ в R16
#LDI R16, 0x10
#OUT SPH, R16          ; установка старшего байта указателя стека

#.equ Delay, 0x10     ; установка константы времени задержки

#; -- устанавливаем каналы PD0 и PD1 порта PORTD (PD) на вывод --
#LDI R16, 0b00000110   ; поместим в регистр R16 число 6 (0x6)
#OUT DDRD, R16         ; загрузим значение из регистра R16 в порт DDRD

#; -- основной цикл программы --
#Start:
#    SBI PORTD, PORTD1 ; подача на пин с каналом PD1 высокого уровня
#    CBI PORTD, PORTD2 ; подача на пин сканалом PD2 низкого уровня
#    RCALL Wait        ; вызываем подпрограмму задержки по времени
#    SBI PORTD, PORTD2 ; подача на пин с каналом PD1 высокого уровня
#    CBI PORTD, PORTD1
#    RCALL Wait
#    RJMP Start        ; возврат к метке Start, повторяем все в цикле

#; -- подпрограмма задержки по времени --
#Wait:
#    LDI  R17, Delay   ; загрузка константы для задержки в регистр R17
#WLoop0:
#    LDI  R18, 0xFF      ; загружаем число 50 (0x32) в регистр R18
#WLoop1:
#    LDI  R19, 0xFF    ; загружаем число 200 (0xC8, $C8) в регистр R19
#WLoop2:
#    DEC  R19          ; уменьшаем значение в регистре R19 на 1
#    BRNE WLoop2       ; возврат к WLoop2 если значение в R19 не равно 0
#    DEC  R18          ; уменьшаем значение в регистре R18 на 1
#    BRNE WLoop1       ; возврат к WLoop1 если значение в R18 не равно 0
#    DEC  R17          ; уменьшаем значение в регистре R17 на 1
#    BRNE WLoop0       ; возврат к WLoop0 если значение в R17 не равно 0
#RET                   ; возврат из подпрограммы Wait

#00000000 <__vectors>:
vectors:
    rjmp	ctors    	; 0x8c <__ctors_end>
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

#0000008c <__ctors_end>:
ctors:
    eor	r1, r1
    out	0x3f, r1	; 63
    ldi	r28, 0xFF	; 255
    ldi	r29, 0x10	; 16
    out	0x3e, r29	; 62
    out	0x3d, r28	; 61
    rcall	main   	; 0x9e <main>
    rjmp	exit     	; 0xc6 <_exit>

#0000009c <__bad_interrupt>:
interrupt:
    rjmp	vectors    	; 0x0 <__vectors>

#0000009e <main>:
main:
    ldi	r24, 0x06	; 6
    out	0x11, r24	; 17
    out	0x12, r24	; 18
    ldi	r30, 0xFF	; 255
    ldi	r31, 0x8F	; 143
    sbiw	r30, 0x01	; 1
    brne	.-4      	; 0xa8 <main+0xa>
    rjmp	.+0      	; 0xae <main+0x10>
    nop
    out	0x12, r1	; 18
    ldi	r31, 0xFF	; 255
    ldi	r18, 0xFF	; 255
    ldi	r19, 0x59	; 89
    subi	r31, 0x01	; 1
    sbci	r18, 0x00	; 0
    sbci	r19, 0x00	; 0
    brne	.-8      	; 0xb8 <main+0x1a>
    rjmp	.+0      	; 0xc2 <main+0x24>
    nop
    rjmp	.-36     	; 0xa2 <main+0x4>

#000000c6 <_exit>:
exit:
    cli

#000000c8 <__stop_program>:
stop:
    rjmp	stop      	; 0xc8 <__stop_program>
