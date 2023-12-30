;
; +-------------------------------------------------------------------------+
; |      This file was generated by The Interactive Disassembler (IDA)      |
; |           Copyright (c) 2022 Hex-Rays, <support@hex-rays.com>           |
; |                      License info: 48-591F-7CB6-D6                      |
; |                       Think-Cell Operations GmbH                        |
; +-------------------------------------------------------------------------+
;
; Input SHA256 : 6DC30967B9F07335ACD19D073F09075CBAF70762D65409B3574E3DA21FA33E74
; Input MD5    : B4A453B89D3FA0D51F2A594DA2CCACE4
; Input CRC32  : 3F767566

; File Name   : Z:\GIT\21_MT9092_to_STM_GITLAB\VX-Mega128\Projects\Simple_for_IDA_PRO\build\test_IDA.hex
; Format      : Intel Hex Object Format

; Processor       : AVR [ATmega128_L]
; Target assembler: AVR Assembler
.equ XL, 26
.equ XH, 27
.equ YL, 28
.equ YH, 29
.equ ZL, 30
.equ ZH, 31

;.MACRO SUBI16 ; Start macro definition
;subi @1,low(@0) ; Subtract low byte
;sbci @2,high(@0) ; Subtract high byte
;.ENDMACRO ; End macro definition
;.CSEG ; Start code segment
;SUBI16 0x1234,r16,r17 ; Sub.0x1234 from r17:r16

; ===========================================================================

; Segment type: Pure code
;.CSEG ; ROM
                .org 0                  ; CODE XREF: SPM_READY↓j

; =============== S U B R O U T I N E =======================================


SPM_READY:
                rjmp    __RESET         ; External Pin, Power-on Reset, Brown-out Reset, Watchdog Reset, and JTAG AVR Reset
; End of function SPM_READY_0

; ---------------------------------------------------------------------------
                nop
                .org 2
                rjmp    SPM_READY       ; Store Program Memory Ready
; ---------------------------------------------------------------------------
                nop
                .org 4
                rjmp    SPM_READY       ; Store Program Memory Ready
; ---------------------------------------------------------------------------
                nop
                .org 6
                rjmp    SPM_READY       ; Store Program Memory Ready
; ---------------------------------------------------------------------------
                nop
                .org 8
                rjmp    SPM_READY       ; Store Program Memory Ready
; ---------------------------------------------------------------------------
                nop
                .org 0xA
                rjmp    SPM_READY       ; Store Program Memory Ready
; ---------------------------------------------------------------------------
                nop
                .org 0xC
                rjmp    SPM_READY       ; Store Program Memory Ready
; ---------------------------------------------------------------------------
                nop
                .org 0xE
                rjmp    SPM_READY       ; Store Program Memory Ready
; ---------------------------------------------------------------------------
                nop
                .org 0x10
                rjmp    SPM_READY       ; Store Program Memory Ready
; ---------------------------------------------------------------------------
                nop
                .org 0x12
                rjmp    SPM_READY       ; Store Program Memory Ready
; ---------------------------------------------------------------------------
                nop
                .org 0x14
                rjmp    SPM_READY       ; Store Program Memory Ready
; ---------------------------------------------------------------------------
                nop
                .org 0x16
                rjmp    SPM_READY       ; Store Program Memory Ready
; ---------------------------------------------------------------------------
                nop
                .org 0x18
                rjmp    SPM_READY       ; Store Program Memory Ready
; ---------------------------------------------------------------------------
                nop
                .org 0x1A
                rjmp    SPM_READY       ; Store Program Memory Ready
; ---------------------------------------------------------------------------
                nop
                .org 0x1C
                rjmp    SPM_READY       ; Store Program Memory Ready
; ---------------------------------------------------------------------------
                nop
                .org 0x1E
                rjmp    SPM_READY       ; Store Program Memory Ready
; ---------------------------------------------------------------------------
                nop
                .org 0x20
                rjmp    SPM_READY       ; Store Program Memory Ready
; ---------------------------------------------------------------------------
                nop
                .org 0x22
                rjmp    SPM_READY       ; Store Program Memory Ready
; ---------------------------------------------------------------------------
                nop
                .org 0x24
                rjmp    SPM_READY       ; Store Program Memory Ready
; ---------------------------------------------------------------------------
                nop
                .org 0x26
                rjmp    SPM_READY       ; Store Program Memory Ready
; ---------------------------------------------------------------------------
                nop
                .org 0x28
                rjmp    SPM_READY       ; Store Program Memory Ready
; ---------------------------------------------------------------------------
                nop
                .org 0x2A
                rjmp    SPM_READY       ; Store Program Memory Ready
; ---------------------------------------------------------------------------
                nop
                .org 0x2C
                rjmp    SPM_READY       ; Store Program Memory Ready
; ---------------------------------------------------------------------------
                nop
                .org 0x2E
                rjmp    SPM_READY       ; Store Program Memory Ready
; ---------------------------------------------------------------------------
                nop
                .org 0x30
                rjmp    SPM_READY       ; Store Program Memory Ready
; ---------------------------------------------------------------------------
                nop
                .org 0x32
                rjmp    SPM_READY       ; Store Program Memory Ready
; ---------------------------------------------------------------------------
                nop
                .org 0x34
                rjmp    SPM_READY       ; Store Program Memory Ready
; ---------------------------------------------------------------------------
                nop
                .org 0x36
                rjmp    SPM_READY       ; Store Program Memory Ready
; ---------------------------------------------------------------------------
                nop
                .org 0x38
                rjmp    SPM_READY       ; Store Program Memory Ready
; ---------------------------------------------------------------------------
                nop
                .org 0x3A
                rjmp    SPM_READY       ; Store Program Memory Ready
; ---------------------------------------------------------------------------
                nop
                .org 0x3C
                rjmp    SPM_READY       ; Store Program Memory Ready
; ---------------------------------------------------------------------------
                nop
                .org 0x3E
                rjmp    SPM_READY       ; Store Program Memory Ready
; ---------------------------------------------------------------------------
                nop
                .org 0x40
                rjmp    SPM_READY       ; Store Program Memory Ready
; ---------------------------------------------------------------------------
                nop
                .org 0x42
                rjmp    SPM_READY       ; Store Program Memory Ready
; ---------------------------------------------------------------------------
                nop
                .org 0x44
                rjmp    SPM_READY       ; Store Program Memory Ready
; ---------------------------------------------------------------------------
                nop

; =============== S U B R O U T I N E =======================================

; External Pin, Power-on Reset, Brown-out Reset, Watchdog Reset, and JTAG AVR Reset

                ; public __RESET
__RESET:                                ; CODE XREF: SPM_READY_0↑j

; FUNCTION CHUNK AT ROM:0063 SIZE 00000002 BYTES

                clr     r1
                out     SREG, r1        ; Status Register
                ser     YL
                ldi     YH, 0x10
                out     SPH, YH         ; Stack Pointer Register High
                out     SPL, YL         ; Stack Pointer Register Low
                rcall   sub_4F
                rjmp    loc_63
; End of function __RESET

; [00000001 BYTES: COLLAPSED FUNCTION SPM_READY. PRESS CTRL-NUMPAD+ TO EXPAND]

; =============== S U B R O U T I N E =======================================


sub_4F:                                 ; CODE XREF: __RESET+6↑p
                ldi     r24, 6
                out     DDRD, r24       ; Port D Data Direction Register

loc_51:                                 ; CODE XREF: sub_4F+13↓j
                out     PORTD, r24      ; Port D Data Register
                ser     ZL
                ldi     ZH, 0x8F

loc_54:                                 ; CODE XREF: sub_4F+6↓j
;                sbiw    Z, 1
;                SUBI16 0x1,ZL,ZH
                brne    loc_54
                rjmp    loc_57
; ---------------------------------------------------------------------------

loc_57:                                 ; CODE XREF: sub_4F+7↑j
                nop
                out     PORTD, r1       ; Port D Data Register
                ser     ZH
                ser     r18
                ldi     r19, 0x59 ; 'Y'

loc_5C:                                 ; CODE XREF: sub_4F+10↓j
                subi    ZH, 1
                sbci    r18, 0
                sbci    r19, 0
                brne    loc_5C
                rjmp    loc_61
; ---------------------------------------------------------------------------

loc_61:                                 ; CODE XREF: sub_4F+11↑j
                nop
                rjmp    loc_51
; End of function sub_4F

; ---------------------------------------------------------------------------
; START OF FUNCTION CHUNK FOR __RESET

loc_63:                                 ; CODE XREF: __RESET+7↑j
                cli

loc_64:                                 ; CODE XREF: __RESET:loc_64↓j
                rjmp    loc_64
; END OF FUNCTION CHUNK FOR __RESET
; ---------------------------------------------------------------------------

;                .exit ;
