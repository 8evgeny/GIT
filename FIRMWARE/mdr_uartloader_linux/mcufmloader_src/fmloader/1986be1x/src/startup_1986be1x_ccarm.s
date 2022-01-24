
Stack_Size      EQU     0x00000080

                AREA    STACK, NOINIT, READWRITE, ALIGN=3
Stack_Mem       SPACE   Stack_Size
__initial_sp

                PRESERVE8
                THUMB

; Vector Table Mapped to Address 0 at Reset

                AREA    RESET, DATA, READONLY 
		
                EXPORT  __Vectors
                EXPORT  __Vectors_End
                EXPORT  __Vectors_Size

__Vectors       DCD     __initial_sp              ; Top of Stack
                DCD     Reset_Handler             ; Reset Handler
__Vectors_End

__Vectors_Size 	EQU 	__Vectors_End - __Vectors

                AREA    |.text|, CODE, READONLY



; Reset Handler
Reset_Handler   PROC
                EXPORT  Reset_Handler             [WEAK]
		IMPORT  main
		LDR	R0, = __Vectors
		LDR	R1, [R0]
		MSR	MSP, R1
		LDR     R0, = main
                BX      R0
                ENDP

                END
