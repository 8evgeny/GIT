;Светодиодная мигалка  Studio 7

.org 0x00 ; начало

;00000000 <__vectors>:
vectors:
        rjmp		ctors_end	; 0x8c <__ctors_end>
        nop
        rjmp		bad_interrupt	; 0x9c <__bad_interrupt>
        nop
        rjmp		bad_interrupt	; 0x9c <__bad_interrupt>
        nop
        rjmp		bad_interrupt	; 0x9c <__bad_interrupt>
        nop
        rjmp		bad_interrupt	; 0x9c <__bad_interrupt>
        nop
        rjmp		bad_interrupt	; 0x9c <__bad_interrupt>
        nop
        rjmp		bad_interrupt	; 0x9c <__bad_interrupt>
        nop
        rjmp		bad_interrupt	; 0x9c <__bad_interrupt>
        nop
        rjmp		bad_interrupt	; 0x9c <__bad_interrupt>
        nop
        rjmp		bad_interrupt	; 0x9c <__bad_interrupt>
        nop
        rjmp		bad_interrupt	; 0x9c <__bad_interrupt>
        nop
        rjmp		bad_interrupt	; 0x9c <__bad_interrupt>
        nop
        rjmp		bad_interrupt	; 0x9c <__bad_interrupt>
        nop
        rjmp		bad_interrupt	; 0x9c <__bad_interrupt>
        nop
        rjmp		TIMER1_OVF_vect	; 0xe2 <__vector_14>
        nop
        rjmp		bad_interrupt	; 0x9c <__bad_interrupt>
        nop
        rjmp		bad_interrupt	; 0x9c <__bad_interrupt>
        nop
        rjmp		bad_interrupt	; 0x9c <__bad_interrupt>
        nop
        rjmp		bad_interrupt	; 0x9c <__bad_interrupt>
        nop
        rjmp		bad_interrupt	; 0x9c <__bad_interrupt>
        nop
        rjmp		bad_interrupt	; 0x9c <__bad_interrupt>
        nop
        rjmp		bad_interrupt	; 0x9c <__bad_interrupt>
        nop
        rjmp		bad_interrupt	; 0x9c <__bad_interrupt>
        nop
        rjmp		bad_interrupt	; 0x9c <__bad_interrupt>
        nop
        rjmp		bad_interrupt	; 0x9c <__bad_interrupt>
        nop
        rjmp		bad_interrupt	; 0x9c <__bad_interrupt>
        nop
        rjmp		bad_interrupt	; 0x9c <__bad_interrupt>
        nop
        rjmp		bad_interrupt	; 0x9c <__bad_interrupt>
        nop
        rjmp		bad_interrupt	; 0x9c <__bad_interrupt>
        nop
        rjmp		TIMER3_OVF_vect	; 0x118 <__vector_29>
        nop
        rjmp		bad_interrupt	; 0x9c <__bad_interrupt>
        nop
        rjmp		bad_interrupt	; 0x9c <__bad_interrupt>
        nop
        rjmp		bad_interrupt	; 0x9c <__bad_interrupt>
        nop
        rjmp		bad_interrupt	; 0x9c <__bad_interrupt>
        nop
        rjmp		bad_interrupt	; 0x9c <__bad_interrupt>

;0000008c <__ctors_end>:
ctors_end:
        eor	r1, r1
        out	SREG, r1                ; 63
        ldi	r28, 0xFF               ; 255
        ldi	r29, 0x10               ; 16
        out	SPH, r29                ; 62
        out	SPL, r28                ; 61
        rcall		main            ; 0x136 <main>
        rjmp		exit            ; 0x146 <_exit>

;0000009c <__bad_interrupt>:
bad_interrupt:
        rjmp		vectors         ; 0x0 <__vectors>

;0000009e <SetupTIMER1>:
SetupTIMER1:
        in	r24, TCCR1B             ; 46
        ori	r24, 0x04               ; 4
        out	TCCR1B, r24             ; 46
        in	r24, TCCR1B
        ori	r24, 0x01               ; 1
        out	TCCR1B, r24             ; 46
        ldi	r24, 0xFF               ; 255
        ldi	r25, 0xFF               ; 255
        out	TCNT1H, r25             ; 45
        out	TCNT1L, r24             ; 44
        in	r24, TIMSK              ; 55
        ori	r24, 0x04               ; 4
        out	TIMSK, r24              ; 55
        ret

;000000ba <SetupTIMER3>:
SetupTIMER3:
        ldi	r30, 0x8A               ; 138
        ldi	r31, 0x00               ; 0
        ld	r24, Z
        ori	r24, 0x04               ; 4
        st	Z, r24
        ldi	r30, 0x7D               ; 125
        ldi	r31, 0x00               ; 0
        ld	r24, Z
        ori	r24, 0x04               ; 4
        st	Z, r24
        ret

;000000e2 <__vector_14>:
TIMER1_OVF_vect:
        push	r1
        push	r0
        in	r0, SREG                ; 63
        push	r0
        eor	r1, r1
        push	r24
        push	r25
        ldi	r24, 0xC8               ; 200
        ldi	r25, 0xAF               ; 175
        out	TCNT1H, r25             ; 45
        out	TCNT1L, r24             ; 44
        in	r24, PORTD              ; 18
        ori	r24, 0x06               ; 6
        out	PORTD, r24              ; 18
        ldi	r24, 0xE8               ; 232
        ldi	r25, 0xFD               ; 253  Чем больше число тем раньше срабатывает таймер 3
        sts	0x0089, r25             ; 0x800089 <__TEXT_REGION_LENGTH__+0x7e0089>
        sts	0x0088, r24             ; 0x800088 <__TEXT_REGION_LENGTH__+0x7e0088>
        pop	r25
        pop	r24
        pop	r0
        out	SREG, r0                ; 63
        pop	r0
        pop	r1
        reti

;00000118 <__vector_29>:
TIMER3_OVF_vect:
        push	r1
        push	r0
        in	r0, SREG                ; 63
        push	r0
        eor	r1, r1
        push	r24
        in	r24, PORTD              ; 18
        andi	r24, 0xF9               ; 249
        out	PORTD, r24              ; 18
        pop	r24
        pop	r0
        out	SREG, r0                ; 63
        pop	r0
        pop	r1
        reti

;00000136 <main>:
main:
        sbi	DDRD, 1                 ; 17
        sbi	DDRD, 2                 ; 17
        rcall   SetupTIMER1             ; 0x9e <SetupTIMER1>
        rcall   SetupTIMER3             ; 0xba <SetupTIMER3>
        ldi	r24, 0x06               ; 6
        out	PORTD, r24              ; 18
        sei

main_end:
        rjmp		main_end        ; 0x144 <main+0xe>

;00000146 <_exit>:
exit:
        cli

;00000148 <__stop_program>:
stop_program:
        rjmp		stop_program    ; 0x148 <__stop_program>





