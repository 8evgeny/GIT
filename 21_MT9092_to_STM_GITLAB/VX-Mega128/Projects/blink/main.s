	.file	"main.c"
__SP_H__ = 0x3e
__SP_L__ = 0x3d
__SREG__ = 0x3f
__tmp_reg__ = 0
__zero_reg__ = 1
.global	__mulsf3
.global	__ltsf2
.global	__gtsf2
.global	__fixunssfsi
	.text
.global	main
	.type	main, @function
main:
	push r28
	push r29
	in r28,__SP_L__
	in r29,__SP_H__
	sbiw r28,28
	in __tmp_reg__,__SREG__
	cli
	out __SP_H__,r29
	out __SREG__,__tmp_reg__
	out __SP_L__,r28
/* prologue: function */
/* frame size = 28 */
/* stack size = 30 */
.L__stack_usage = 30
	ldi r24,lo8(55)
	ldi r25,0
	ldi r18,lo8(55)
	ldi r19,0
	mov r30,r18
	mov r31,r19
	ld r18,Z
	ori r18,lo8(1)
	mov r30,r24
	mov r31,r25
	st Z,r18
	ldi r24,lo8(55)
	ldi r25,0
	ldi r18,lo8(55)
	ldi r19,0
	mov r30,r18
	mov r31,r19
	ld r18,Z
	ori r18,lo8(2)
	mov r30,r24
	mov r31,r25
	st Z,r18
.L18:
	ldi r24,lo8(56)
	ldi r25,0
	ldi r18,lo8(3)
	mov r30,r24
	mov r31,r25
	st Z,r18
	ldi r24,0
	ldi r25,0
	ldi r26,lo8(72)
	ldi r27,lo8(67)
	std Y+1,r24
	std Y+2,r25
	std Y+3,r26
	std Y+4,r27
	ldi r18,0
	ldi r19,0
	ldi r20,lo8(122)
	ldi r21,lo8(67)
	ldd r22,Y+1
	ldd r23,Y+2
	ldd r24,Y+3
	ldd r25,Y+4
	rcall __mulsf3
	mov r27,r25
	mov r26,r24
	mov r25,r23
	mov r24,r22
	std Y+19,r24
	std Y+20,r25
	std Y+21,r26
	std Y+22,r27
	ldi r18,0
	ldi r19,0
	ldi r20,lo8(-128)
	ldi r21,lo8(63)
	ldd r22,Y+19
	ldd r23,Y+20
	ldd r24,Y+21
	ldd r25,Y+22
	rcall __ltsf2
	tst r24
	brge .L23
	ldi r24,lo8(1)
	ldi r25,0
	std Y+24,r25
	std Y+23,r24
	rjmp .L4
.L23:
	ldi r18,0
	ldi r19,lo8(-1)
	ldi r20,lo8(127)
	ldi r21,lo8(71)
	ldd r22,Y+19
	ldd r23,Y+20
	ldd r24,Y+21
	ldd r25,Y+22
	rcall __gtsf2
	cp __zero_reg__,r24
	brge .L24
	ldi r18,0
	ldi r19,0
	ldi r20,lo8(32)
	ldi r21,lo8(65)
	ldd r22,Y+1
	ldd r23,Y+2
	ldd r24,Y+3
	ldd r25,Y+4
	rcall __mulsf3
	mov r27,r25
	mov r26,r24
	mov r25,r23
	mov r24,r22
	mov r22,r24
	mov r23,r25
	mov r24,r26
	mov r25,r27
	rcall __fixunssfsi
	mov r27,r25
	mov r26,r24
	mov r25,r23
	mov r24,r22
	std Y+24,r25
	std Y+23,r24
	rjmp .L7
.L8:
	ldi r24,lo8(25)
	ldi r25,0
	std Y+26,r25
	std Y+25,r24
	ldd r24,Y+25
	ldd r25,Y+26
/* #APP */
 ;  105 "/usr/lib/avr/include/util/delay_basic.h" 1
	1: sbiw r24,1
	brne 1b
 ;  0 "" 2
/* #NOAPP */
	std Y+26,r25
	std Y+25,r24
	ldd r24,Y+23
	ldd r25,Y+24
	sbiw r24,1
	std Y+24,r25
	std Y+23,r24
.L7:
	ldd r24,Y+23
	ldd r25,Y+24
	or r24,r25
	brne .L8
	rjmp .L9
.L24:
	ldd r22,Y+19
	ldd r23,Y+20
	ldd r24,Y+21
	ldd r25,Y+22
	rcall __fixunssfsi
	mov r27,r25
	mov r26,r24
	mov r25,r23
	mov r24,r22
	std Y+24,r25
	std Y+23,r24
.L4:
	ldd r24,Y+23
	ldd r25,Y+24
	std Y+28,r25
	std Y+27,r24
	ldd r24,Y+27
	ldd r25,Y+28
/* #APP */
 ;  105 "/usr/lib/avr/include/util/delay_basic.h" 1
	1: sbiw r24,1
	brne 1b
 ;  0 "" 2
/* #NOAPP */
	std Y+28,r25
	std Y+27,r24
.L9:
	ldi r24,lo8(56)
	ldi r25,0
	mov r30,r24
	mov r31,r25
	st Z,__zero_reg__
	ldi r24,0
	ldi r25,0
	ldi r26,lo8(122)
	ldi r27,lo8(68)
	std Y+5,r24
	std Y+6,r25
	std Y+7,r26
	std Y+8,r27
	ldi r18,0
	ldi r19,0
	ldi r20,lo8(122)
	ldi r21,lo8(67)
	ldd r22,Y+5
	ldd r23,Y+6
	ldd r24,Y+7
	ldd r25,Y+8
	rcall __mulsf3
	mov r27,r25
	mov r26,r24
	mov r25,r23
	mov r24,r22
	std Y+9,r24
	std Y+10,r25
	std Y+11,r26
	std Y+12,r27
	ldi r18,0
	ldi r19,0
	ldi r20,lo8(-128)
	ldi r21,lo8(63)
	ldd r22,Y+9
	ldd r23,Y+10
	ldd r24,Y+11
	ldd r25,Y+12
	rcall __ltsf2
	tst r24
	brge .L25
	ldi r24,lo8(1)
	ldi r25,0
	std Y+14,r25
	std Y+13,r24
	rjmp .L12
.L25:
	ldi r18,0
	ldi r19,lo8(-1)
	ldi r20,lo8(127)
	ldi r21,lo8(71)
	ldd r22,Y+9
	ldd r23,Y+10
	ldd r24,Y+11
	ldd r25,Y+12
	rcall __gtsf2
	cp __zero_reg__,r24
	brge .L26
	ldi r18,0
	ldi r19,0
	ldi r20,lo8(32)
	ldi r21,lo8(65)
	ldd r22,Y+5
	ldd r23,Y+6
	ldd r24,Y+7
	ldd r25,Y+8
	rcall __mulsf3
	mov r27,r25
	mov r26,r24
	mov r25,r23
	mov r24,r22
	mov r22,r24
	mov r23,r25
	mov r24,r26
	mov r25,r27
	rcall __fixunssfsi
	mov r27,r25
	mov r26,r24
	mov r25,r23
	mov r24,r22
	std Y+14,r25
	std Y+13,r24
	rjmp .L15
.L16:
	ldi r24,lo8(25)
	ldi r25,0
	std Y+16,r25
	std Y+15,r24
	ldd r24,Y+15
	ldd r25,Y+16
/* #APP */
 ;  105 "/usr/lib/avr/include/util/delay_basic.h" 1
	1: sbiw r24,1
	brne 1b
 ;  0 "" 2
/* #NOAPP */
	std Y+16,r25
	std Y+15,r24
	ldd r24,Y+13
	ldd r25,Y+14
	sbiw r24,1
	std Y+14,r25
	std Y+13,r24
.L15:
	ldd r24,Y+13
	ldd r25,Y+14
	or r24,r25
	brne .L16
	rjmp .L17
.L26:
	ldd r22,Y+9
	ldd r23,Y+10
	ldd r24,Y+11
	ldd r25,Y+12
	rcall __fixunssfsi
	mov r27,r25
	mov r26,r24
	mov r25,r23
	mov r24,r22
	std Y+14,r25
	std Y+13,r24
.L12:
	ldd r24,Y+13
	ldd r25,Y+14
	std Y+18,r25
	std Y+17,r24
	ldd r24,Y+17
	ldd r25,Y+18
/* #APP */
 ;  105 "/usr/lib/avr/include/util/delay_basic.h" 1
	1: sbiw r24,1
	brne 1b
 ;  0 "" 2
/* #NOAPP */
	std Y+18,r25
	std Y+17,r24
.L17:
	rjmp .L18
	.size	main, .-main
	.ident	"GCC: (GNU) 5.4.0"
