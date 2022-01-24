;/*****************************************************************************/
;/* 1986VE8T.s: Startup file for ARM Cortex-M4F Device Family                   */
;/*****************************************************************************/
;/* <<< Use Configuration Wizard in Context Menu >>>                          */
;/*****************************************************************************/
;/* This file is part of the uVision/ARM development tools.                   */
;/* Copyright (c) 2005-2008 Keil Software. All rights reserved.               */
;/* This software may only be used under the terms of a valid, current,       */
;/* end user licence from KEIL for a compatible version of KEIL software      */
;/* development tools. Nothing else gives you the right to use this software. */
;/*****************************************************************************/


;// <h> Stack Configuration
;//   <o> Stack Size (in Bytes) <0x0-0xFFFFFFFF:8>
;// </h>

Stack_Size      EQU     0x00001000

                AREA    STACK, NOINIT, READWRITE, ALIGN=3
Stack_Mem       SPACE   Stack_Size
__initial_sp


;// <h> Heap Configuration
;//   <o>  Heap Size (in Bytes) <0x0-0xFFFFFFFF:8>
;// </h>

Heap_Size       EQU     0x00001000
BKP_KEY			EQU		0x8555aaa1
BKP_BASE_KEY	EQU		0x400010F0	
BKP_RG60_0		EQU		0x40001100	
BKP_RG60_1		EQU		0x40001110	
BKP_RG60_2		EQU		0x40001120	
	

                AREA    HEAP, NOINIT, READWRITE, ALIGN=3
__heap_base
Heap_Mem        SPACE   Heap_Size
__heap_limit

                PRESERVE8
                THUMB

; Vector Table Mapped to Address 0 at Reset

                AREA    RESET, DATA, READONLY
	            EXPORT  __Vectors
                EXPORT  __Vectors_End
                EXPORT  __Vectors_Size

__Vectors       DCD     __initial_sp              ; Top of Stack
                DCD     Reset_Handler             ; Reset Handler
                DCD     NMI_Handler               ; NMI Handler
                DCD     HardFault_Handler         ; Hard Fault Handler
                DCD     MemoryMngFault_Handler    ; MemoryMngFault_Handler
                DCD     BusFault_Handler          ; BusFault_Handler
                DCD     UsageFault_Handler        ; UsageFault_Handler
                DCD     0                         ; Reserved
                DCD     0                         ; Reserved
                DCD     0                         ; Reserved
                DCD     0                         ; Reserved
				DCD     SVC_Handler				  ; SVCall Handler
                DCD     0                         ; Reserved
                DCD     0                         ; Reserved
                DCD     PendSV_Handler            ; PendSV Handler
                DCD     SysTick_Handler           ; SysTick Handler
                DCD     0           			  ; Reserved16
                DCD     0          				  ; Reserved17
                DCD     0           			  ; Reserved18
                DCD     0           			  ; Reserved19
                DCD     0           			  ; Reserved20
                DCD     0           			  ; Reserved21
                DCD     0           			  ; Reserved22
                DCD     0           			  ; Reserved23
                DCD     0           			  ; Reserved24
                DCD     0           			  ; Reserved25
                DCD     0           			  ; Reserved26
                DCD     0           			  ; Reserved27
                DCD     0           			  ; Reserved28
                DCD     0           			  ; Reserved29
                DCD     0           			  ; Reserved30
                DCD     0          				  ; Reserved31
                DCD     0           			  ; Reserved32
                DCD     0           			  ; Reserved33
                DCD     0           			  ; Reserved34
                DCD     0           			  ; Reserved35
                DCD     0           			  ; Reserved36
                DCD     0           			  ; Reserved37
				DCD 	0	
				DCD 	0	
				DCD 	0	
				DCD 	0	
				DCD 	0	
				DCD 	0	
				DCD 	0	
				DCD 	0	
				DCD 	0	
				DCD 	0	
                DCD     FT_IF0_Handler            ; FT_IF0_Handler
                DCD     FT_IF1_Handler            ; FT_IF1_Handler
                DCD     FT_IF2_Handler            ; FT_IF2_Handler
                DCD     FT_IF3_Handler            ; FT_IF3_Handler
                DCD     CLK_IF_Handler            ; CLK_IF_Handler
                DCD     PVD_IF_Handler            ; PVD_IF_Handler
                DCD     RTC_IF_Handler            ; RTC_IF_Handler
                DCD     BKP_IF_Handler            ; BKP_IF_Handler
                DCD     EXT_INTERROR0_Handler     ; EXT_INTERROR0_Handler
                DCD     EXT_INTERROR1_Handler     ; EXT_INTERROR1_Handler
                DCD     EXT_INTERROR2_Handler     ; EXT_INTERROR2_Handler
                DCD     EXT_INTERROR3_Handler     ; EXT_INTERROR3_Handler
                DCD     EXT_INTERROR4_Handler     ; EXT_INTERROR4_Handler
                DCD     EXT_INTERROR5_Handler     ; EXT_INTERROR5_Handler
                DCD     EXT_INTERROR6_Handler     ; EXT_INTERROR6_Handler
                DCD     EXT_INTERROR7_Handler     ; EXT_INTERROR7_Handler
                DCD     0				          ; Reserved					
                DCD     0        				  ; Reserved				
                DCD     DMA_ERR_Handler           ; DMA_ERR_Handler	
                DCD     DMA_DONE0_Handler         ; DMA_DONE0_Handler	
                DCD     DMA_DONE1_Handler         ; DMA_DONE1_Handler
                DCD     DMA_DONE2_Handler         ; DMA_DONE2_Handler	
                DCD     DMA_DONE3_Handler         ; DMA_DONE3_Handler	
                DCD     DMA_DONE4_Handler     	  ; DMA_DONE4_Handler	
                DCD     DMA_DONE5_Handler      	  ; DMA_DONE5_Handler	
                DCD     DMA_DONE6_Handler      	  ; DMA_DONE6_Handler	
                DCD     DMA_DONE7_Handler         ; DMA_DONE7_Handler	
                DCD     DMA_DONE8_Handler         ; DMA_DONE8_Handler	
                DCD     DMA_DONE9_Handler         ; DMA_DONE9_Handler	
                DCD     DMA_DONE10_Handler        ; DMA_DONE10_Handler	
                DCD     DMA_DONE11_Handler     	  ; DMA_DONE11_Handler
                DCD     DMA_DONE12_Handler	 	  ; DMA_DONE12_Handler	
                DCD     DMA_DONE13_Handler		  ; DMA_DONE13_Handler
                DCD     DMA_DONE14_Handler		  ; DMA_DONE14_Handler	
                DCD     DMA_DONE15_Handler		  ; DMA_DONE15_Handler	
                DCD     DMA_DONE16_Handler		  ; DMA_DONE16_Handler	
                DCD     DMA_DONE17_Handler		  ; DMA_DONE17_Handler	
                DCD     DMA_DONE18_Handler		  ; DMA_DONE18_Handler	
                DCD     DMA_DONE19_Handler		  ; DMA_DONE19_Handler	
                DCD     DMA_DONE20_Handler		  ; DMA_DONE20_Handler	
                DCD     DMA_DONE21_Handler		  ; DMA_DONE21_Handler	
                DCD     DMA_DONE22_Handler		  ; DMA_DONE22_Handler	
                DCD     DMA_DONE23_Handler		  ; DMA_DONE23_Handler
                DCD     DMA_DONE24_Handler		  ; DMA_DONE24_Handler	
                DCD     DMA_DONE25_Handler		  ; DMA_DONE25_Handler
                DCD     DMA_DONE26_Handler		  ; DMA_DONE26_Handler	
                DCD     DMA_DONE27_Handler		  ; DMA_DONE27_Handler	
                DCD     DMA_DONE28_Handler		  ; DMA_DONE28_Handler	
                DCD     DMA_DONE29_Handler		  ; DMA_DONE29_Handler	
                DCD     DMA_DONE30_Handler		  ; DMA_DONE30_Handler	
                DCD     DMA_DONE31_Handler		  ; DMA_DONE31_Handler				
                DCD     IRQ_PORTA_Handler         ; IRQ_PORTA_Handler					
                DCD     IRQ_PORTB_Handler         ; IRQ_PORTB_Handler
                DCD     IRQ_PORTC_Handler         ; IRQ_PORTC_Handler
                DCD     IRQ_PORTD_Handler         ; IRQ_PORTD_Handler
                DCD     IRQ_PORTE_Handler         ; IRQ_PORTE_Handler
                DCD     0					      ; Reserved
                DCD     INT_ETH0_Handler          ; INT_ETH0_Handler
                DCD     0        				  ; Reserved	
				DCD     INT_SPW0_Handler          ; INT_SPW0_Handler
                DCD     0        				  ; Reserved	
                DCD     INT_TMR0_Handler          ; INT_TMR0_Handler
                DCD     INT_TMR1_Handler          ; INT_TMR1_Handler
                DCD     INT_TMR2_Handler          ; INT_TMR2_Handler
                DCD     INT_TMR3_Handler          ; INT_TMR3_Handler
                DCD     0        				  ; Reserved	
                DCD     0        				  ; Reserved	
                DCD     INT_CAN0_Handler          ; INT_CAN0_Handler
                DCD     0        				  ; Reserved	
                DCD     0        				  ; Reserved	
                DCD     0        				  ; Reserved	
                DCD     0        				  ; Reserved					
                DCD     INT_SSP0_Handler          ; INT_SSP0_Handler	
                DCD     0        				  ; Reserved	
                DCD     0        				  ; Reserved	
                DCD     0        				  ; Reserved	
                DCD     INT_UART0_Handler         ; INT_UART0_Handler
                DCD     INT_UART1_Handler         ; INT_UART1_Handler
                DCD     0        				  ; Reserved	
                DCD     0        				  ; Reserved	
                DCD     0        				  ; Reserved	
                DCD     INT_RX_ARC0_Handler       ; INT_RX_ARC0_Handler
                DCD     INT_TX_ARC0_Handler       ; INT_TX_ARC0_Handler
                DCD     0        				  ; Reserved	
                DCD     0        				  ; Reserved	
                DCD     INT_MIL0_Handler          ; INT_MIL0_Handler
                DCD     INT_MIL1_Handler          ; INT_MIL1_Handler
                DCD     INT_ADC0_Handler          ; INT_ADC0_Handler
                DCD     INT_ADC1_Handler          ; INT_ADC1_Handler
                DCD     INT_DAC0_Handler          ; INT_DAC0_Handler
                DCD     INT_DAC1_Handler          ; INT_DAC1_Handler											
__Vectors_End

__Vectors_Size  EQU  __Vectors_End - __Vectors
	
                AREA    |.text|, CODE, READONLY


; Reset Handler

Reset_Handler   PROC
                EXPORT  Reset_Handler			[WEAK]
                IMPORT  __main

;				LDR		R0, =BKP_BASE_KEY
;				LDR		R1, =BKP_KEY
;				STR		R1, [R0]	
;				
;				LDR		R0, =BKP_RG60_0
;				LDR		R1,[R0]
;				ORR		R1,R1,#0x10000000
;				STR		R1,[R0]
;				LDR		R0, =BKP_RG60_1
;				STR		R1,[R0]
;				LDR		R0, =BKP_RG60_2
;				STR		R1,[R0]
;				B		.
				
				LDR     R0,=__main
				BX      R0
                ENDP


; Dummy Exception Handlers (infinite loops which can be modified)                

NMI_Handler     PROC
                EXPORT  NMI_Handler						[WEAK]
                B       .
                ENDP

HardFault_Handler     PROC
                EXPORT  HardFault_Handler				[WEAK]
                B       .
                ENDP

MemoryMngFault_Handler     PROC
                EXPORT  MemoryMngFault_Handler			[WEAK]
                B       .
                ENDP

BusFault_Handler     PROC
                EXPORT  BusFault_Handler				[WEAK]
                B       .
                ENDP

UsageFault_Handler     PROC
                EXPORT  UsageFault_Handler				[WEAK]
                B       .
                ENDP

SVC_Handler     PROC
                EXPORT  SVC_Handler						[WEAK]
                B       .
                ENDP

PendSV_Handler     PROC
                EXPORT  PendSV_Handler					[WEAK]
                B       .
                ENDP

SysTick_Handler     PROC
                EXPORT  SysTick_Handler					[WEAK]
                B       .
                ENDP
                
FT_IF0_Handler     PROC
                EXPORT  FT_IF0_Handler					[WEAK]
                B       .
                ENDP

FT_IF1_Handler     PROC
                EXPORT  FT_IF1_Handler					[WEAK]
                B       .
                ENDP

FT_IF2_Handler     PROC
                EXPORT  FT_IF2_Handler					[WEAK]
                B       .
                ENDP

FT_IF3_Handler     PROC
                EXPORT  FT_IF3_Handler					[WEAK]
                B       .
                ENDP

CLK_IF_Handler     PROC
                EXPORT  CLK_IF_Handler					[WEAK]
                B       .
                ENDP

PVD_IF_Handler     PROC
                EXPORT  PVD_IF_Handler					[WEAK]
                B       .
                ENDP

RTC_IF_Handler     PROC
                EXPORT  RTC_IF_Handler					[WEAK]
                B       .
                ENDP

BKP_IF_Handler     PROC
                EXPORT  BKP_IF_Handler					[WEAK]
                B       .
                ENDP

EXT_INTERROR0_Handler     PROC
                EXPORT  EXT_INTERROR0_Handler			[WEAK]
                B       .
                ENDP

EXT_INTERROR1_Handler     PROC
                EXPORT  EXT_INTERROR1_Handler			[WEAK]
                B       .
                ENDP

EXT_INTERROR2_Handler     PROC
                EXPORT  EXT_INTERROR2_Handler			[WEAK]
                B       .
                ENDP

EXT_INTERROR3_Handler     PROC
                EXPORT  EXT_INTERROR3_Handler			[WEAK]
                B       .
                ENDP

EXT_INTERROR4_Handler     PROC
                EXPORT  EXT_INTERROR4_Handler			[WEAK]
                B       .
                ENDP

EXT_INTERROR5_Handler     PROC
                EXPORT  EXT_INTERROR5_Handler			[WEAK]
                B       .
                ENDP

EXT_INTERROR6_Handler     PROC
                EXPORT  EXT_INTERROR6_Handler			[WEAK]
                B       .
                ENDP

EXT_INTERROR7_Handler     PROC
                EXPORT  EXT_INTERROR7_Handler			[WEAK]
                B       .
                ENDP

IF_ERR_SCR_Handler     PROC
                EXPORT  IF_ERR_SCR_Handler				[WEAK]
                B       .
                ENDP

IF_FIN_SCR_Handler     PROC
                EXPORT  IF_FIN_SCR_Handler				[WEAK]
                B       .
                ENDP

DMA_ERR_Handler     PROC
                EXPORT  DMA_ERR_Handler					[WEAK]
                B       .
                ENDP
                
DMA_DONE0_Handler     PROC
                EXPORT  DMA_DONE0_Handler				[WEAK]
                B       .
                ENDP
					
DMA_DONE1_Handler     PROC
                EXPORT  DMA_DONE1_Handler				[WEAK]
                B       .
                ENDP					

DMA_DONE2_Handler     PROC
                EXPORT  DMA_DONE2_Handler				[WEAK]
                B       .
                ENDP
					
DMA_DONE3_Handler     PROC
                EXPORT  DMA_DONE3_Handler				[WEAK]
                B       .
                ENDP
					
DMA_DONE4_Handler     PROC
                EXPORT  DMA_DONE4_Handler				[WEAK]
                B       .
                ENDP
					
DMA_DONE5_Handler     PROC
                EXPORT  DMA_DONE5_Handler				[WEAK]
                B       .
                ENDP
					
DMA_DONE6_Handler     PROC
                EXPORT  DMA_DONE6_Handler				[WEAK]
                B       .
                ENDP
					
DMA_DONE7_Handler     PROC
                EXPORT  DMA_DONE7_Handler				[WEAK]
                B       .
                ENDP
					
DMA_DONE8_Handler     PROC
                EXPORT  DMA_DONE8_Handler				[WEAK]
                B       .
                ENDP
					
DMA_DONE9_Handler     PROC
                EXPORT  DMA_DONE9_Handler				[WEAK]
                B       .
                ENDP
					
DMA_DONE10_Handler     PROC
                EXPORT  DMA_DONE10_Handler				[WEAK]
                B       .
                ENDP
					
DMA_DONE11_Handler     PROC
                EXPORT  DMA_DONE11_Handler				[WEAK]
                B       .
                ENDP					

DMA_DONE12_Handler     PROC
                EXPORT  DMA_DONE12_Handler				[WEAK]
                B       .
                ENDP					

DMA_DONE13_Handler     PROC
                EXPORT  DMA_DONE13_Handler				[WEAK]
                B       .
                ENDP
					
DMA_DONE14_Handler     PROC
                EXPORT  DMA_DONE14_Handler				[WEAK]
                B       .
                ENDP
					
DMA_DONE15_Handler     PROC
                EXPORT  DMA_DONE15_Handler				[WEAK]
                B       .
                ENDP
					
DMA_DONE16_Handler     PROC
                EXPORT  DMA_DONE16_Handler				[WEAK]
                B       .
                ENDP
					
DMA_DONE17_Handler     PROC
                EXPORT  DMA_DONE17_Handler				[WEAK]
                B       .
                ENDP
					
DMA_DONE18_Handler     PROC
                EXPORT  DMA_DONE18_Handler				[WEAK]
                B       .
                ENDP
					
DMA_DONE19_Handler     PROC
                EXPORT  DMA_DONE19_Handler				[WEAK]
                B       .
                ENDP
					
DMA_DONE20_Handler     PROC
                EXPORT  DMA_DONE20_Handler				[WEAK]
                B       .
                ENDP
					
DMA_DONE21_Handler     PROC
                EXPORT  DMA_DONE21_Handler				[WEAK]
                B       .
                ENDP
					
DMA_DONE22_Handler     PROC
                EXPORT  DMA_DONE22_Handler				[WEAK]
                B       .
                ENDP

DMA_DONE23_Handler     PROC
                EXPORT  DMA_DONE23_Handler				[WEAK]
                B       .
                ENDP
					
DMA_DONE24_Handler     PROC
                EXPORT  DMA_DONE24_Handler				[WEAK]
                B       .
                ENDP
					
DMA_DONE25_Handler     PROC
                EXPORT  DMA_DONE25_Handler				[WEAK]
                B       .
                ENDP
					
DMA_DONE26_Handler     PROC
                EXPORT  DMA_DONE26_Handler				[WEAK]
                B       .
                ENDP
					
DMA_DONE27_Handler     PROC
                EXPORT  DMA_DONE27_Handler				[WEAK]
                B       .
                ENDP
					
DMA_DONE28_Handler     PROC
                EXPORT  DMA_DONE28_Handler				[WEAK]
                B       .
                ENDP
					
DMA_DONE29_Handler     PROC
                EXPORT  DMA_DONE29_Handler				[WEAK]
                B       .
                ENDP
					
DMA_DONE30_Handler     PROC
                EXPORT  DMA_DONE30_Handler				[WEAK]
                B       .
                ENDP
					
DMA_DONE31_Handler     PROC
                EXPORT  DMA_DONE31_Handler				[WEAK]
                B       .
                ENDP
					
IRQ_PORTA_Handler     PROC
                EXPORT  IRQ_PORTA_Handler				[WEAK]
                B       .
                ENDP

IRQ_PORTB_Handler     PROC
                EXPORT  IRQ_PORTB_Handler				[WEAK]
                B       .
                ENDP

IRQ_PORTC_Handler     PROC
                EXPORT  IRQ_PORTC_Handler				[WEAK]
                B       .
                ENDP

IRQ_PORTD_Handler     PROC
                EXPORT  IRQ_PORTD_Handler				[WEAK]
                B       .
                ENDP

IRQ_PORTE_Handler     PROC
                EXPORT  IRQ_PORTE_Handler				[WEAK]
                B       .
                ENDP

INT_ETH0_Handler     PROC
                EXPORT  INT_ETH0_Handler				[WEAK]
                B       .
                ENDP

INT_SPW0_Handler     PROC
                EXPORT  INT_SPW0_Handler				[WEAK]
                B       .
                ENDP

INT_TMR0_Handler     PROC
                EXPORT  INT_TMR0_Handler				[WEAK]
                B       .
                ENDP

INT_TMR1_Handler     PROC
                EXPORT  INT_TMR1_Handler				[WEAK]
                B       .
                ENDP

INT_TMR2_Handler     PROC
                EXPORT  INT_TMR2_Handler				[WEAK]
                B       .
                ENDP

INT_TMR3_Handler     PROC
                EXPORT  INT_TMR3_Handler				[WEAK]
                B       .
                ENDP

INT_CAN0_Handler     PROC
                EXPORT  INT_CAN0_Handler				[WEAK]
                B       .
                ENDP

INT_SSP0_Handler     PROC
                EXPORT  INT_SSP0_Handler				[WEAK]
                B       .
                ENDP

INT_UART0_Handler     PROC
                EXPORT  INT_UART0_Handler				[WEAK]
                B       .
                ENDP

INT_UART1_Handler     PROC
                EXPORT  INT_UART1_Handler				[WEAK]
                B       .
                ENDP
					
INT_RX_ARC0_Handler     PROC
                EXPORT  INT_RX_ARC0_Handler				[WEAK]
                B       .
                ENDP

INT_TX_ARC0_Handler     PROC
                EXPORT  INT_TX_ARC0_Handler				[WEAK]
                B       .
                ENDP

INT_MIL0_Handler     PROC
                EXPORT  INT_MIL0_Handler				[WEAK]
                B       .
                ENDP
					
INT_MIL1_Handler     PROC
                EXPORT  INT_MIL1_Handler				[WEAK]
                B       .
                ENDP

INT_ADC0_Handler     PROC
                EXPORT  INT_ADC0_Handler				[WEAK]
                B       .
                ENDP
					
INT_ADC1_Handler     PROC
                EXPORT  INT_ADC1_Handler				[WEAK]
                B       .
                ENDP

INT_DAC0_Handler     PROC
                EXPORT  INT_DAC0_Handler				[WEAK]
                B       .
                ENDP
					
INT_DAC1_Handler     PROC
                EXPORT  INT_DAC1_Handler				[WEAK]
                B       .
                ENDP
					
                ALIGN



; User Initial Stack & Heap

                IF      :DEF:__MICROLIB
                
                EXPORT  __initial_sp
                EXPORT  __heap_base
                EXPORT  __heap_limit
                
                ELSE
                
                IMPORT  __use_two_region_memory
               EXPORT  __user_initial_stackheap
__user_initial_stackheap

                LDR     R0, =  Heap_Mem
                LDR     R1, =(Stack_Mem + Stack_Size)
                LDR     R2, = (Heap_Mem +  Heap_Size)
                LDR     R3, = Stack_Mem
                BX      LR

                ALIGN

                ENDIF


              END
