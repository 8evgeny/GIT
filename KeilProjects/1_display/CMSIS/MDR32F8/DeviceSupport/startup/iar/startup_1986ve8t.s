;/*****************************************************************************/
;/* startup_1986ve8t.s: Startup file for ARM Cortex-M4F Device Family         */
;/*****************************************************************************/
;/* <<< Use Configuration Wizard in Context Menu >>>                          */
;/*****************************************************************************/
; * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
; * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
; * TIME. AS A RESULT, MILANDR SHALL NOT BE HELD LIABLE FOR ANY DIRECT, INDIRECT
; * OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
; * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
; * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
;/*****************************************************************************/

        MODULE  ?cstartup

        ;; Forward declaration of sections.
        SECTION CSTACK:DATA:NOROOT(3)

        SECTION .intvec:CODE:NOROOT(2)

        EXTERN  __iar_program_start
        EXTERN  SystemInit
        PUBLIC  __vector_table

        DATA
__vector_table
        DCD     sfe(CSTACK)
        DCD     Reset_Handler              ; -15 Reset Handler
        DCD     NMI_Handler                ; -14 NMI Handler
        DCD     HardFault_Handler          ; -13 Hard Fault Handler
        DCD     MemoryMngFault_Handler     ; MemoryMngFault_Handler
        DCD     BusFault_Handler           ; BusFault_Handler
        DCD     UsageFault_Handler         ; UsageFault_Handler
        DCD     0                          ; -9  Reserved
        DCD     0                          ; -8  Reserved
        DCD     0                          ; -7  Reserved
        DCD     0                          ; -6  Reserved
        DCD     SVC_Handler                ; -5  SVCall Handler
        DCD     0                          ; -4  Debug Monitor Handler
        DCD     0                          ; -3  Reserved
        DCD     PendSV_Handler             ; -2  PendSV Handler
        DCD     SysTick_Handler            ; -1  SysTick Handler
		
		
         ; External Interrupts
        DCD     0                          ; 0   Reserved
        DCD     0                          ; 1   Reserved
        DCD     0                          ; 2   Reserved
        DCD     0                          ; 3   Reserved
        DCD     0                          ; 4   Reserved
        DCD     0                          ; 5   Reserved
        DCD     0                          ; 6   Reserved
        DCD     0                          ; 7   Reserved
        DCD     0                          ; 8   Reserved
        DCD     0                          ; 9   Reserved
        DCD     0                          ; 10  Reserved
        DCD     0                          ; 11  Reserved
        DCD     0                          ; 12  Reserved
        DCD     0                          ; 13  Reserved
        DCD     0                          ; 14  Reserved
        DCD     0                          ; 15  Reserved
        DCD     0                          ; 16  Reserved
        DCD     0                          ; 17  Reserved
        DCD     0                          ; 18  Reserved
        DCD     0                          ; 19  Reserved
        DCD     0                          ; 20  Reserved
        DCD     0                          ; 21  Reserved
        DCD     0                          ; 22  Reserved
        DCD     0                          ; 23  Reserved
        DCD     0                          ; 24  Reserved
        DCD     0                          ; 25  Reserved
        DCD     0                          ; 26  Reserved
        DCD     0                          ; 27  Reserved
        DCD     0                          ; 28  Reserved
        DCD     0                          ; 29  Reserved
        DCD     0                          ; 30  Reserved
        DCD     0                          ; 31  Reserved 
        DCD     FT_IF0_Handler             ; 32
        DCD     FT_IF1_Handler             ; 33
        DCD     FT_IF2_Handler             ; 34
        DCD     FT_IF3_Handler             ; 35   
        DCD     CLK_IF_Handler             ; 36 
        DCD     PVD_IF_Handler             ; 37
        DCD     RTC_IF_Handler             ; 38
        DCD     BKP_IF_Handler             ; 39  
        DCD     EXT_INTERROR0_Handler      ; 40  
        DCD     EXT_INTERROR1_Handler      ; 41 
        DCD     EXT_INTERROR2_Handler      ; 42 
        DCD     EXT_INTERROR3_Handler      ; 43 
        DCD     EXT_INTERROR4_Handler      ; 44 
        DCD     EXT_INTERROR5_Handler      ; 45 
        DCD     EXT_INTERROR6_Handler      ; 46 
        DCD     EXT_INTERROR7_Handler      ; 47    
        DCD     0                          ; 48 
        DCD     0                          ; 49         
        DCD     DMA_ERR_Handler            ; 50  
        DCD     DMA_DONE0_Handler          ; 51         
        DCD     DMA_DONE1_Handler          ; 52 
        DCD     DMA_DONE2_Handler          ; 53 
        DCD     DMA_DONE3_Handler          ; 54 
        DCD     DMA_DONE4_Handler          ; 55 
        DCD     DMA_DONE5_Handler          ; 56
        DCD     DMA_DONE6_Handler          ; 57 
        DCD     DMA_DONE7_Handler          ; 58 
        DCD     DMA_DONE8_Handler          ; 59 
        DCD     DMA_DONE9_Handler          ; 60 
        DCD     DMA_DONE10_Handler         ; 61 
        DCD     DMA_DONE11_Handler         ; 62 
        DCD     DMA_DONE12_Handler         ; 63 
        DCD     DMA_DONE13_Handler         ; 64 
        DCD     DMA_DONE14_Handler         ; 65 
        DCD     DMA_DONE15_Handler         ; 66 
        DCD     DMA_DONE16_Handler         ; 67 
        DCD     DMA_DONE17_Handler         ; 68 
        DCD     DMA_DONE18_Handler         ; 69 
        DCD     DMA_DONE19_Handler         ; 70 
        DCD     DMA_DONE20_Handler         ; 71 
        DCD     DMA_DONE21_Handler         ; 72 
        DCD     DMA_DONE22_Handler         ; 73 
        DCD     DMA_DONE23_Handler         ; 74 
        DCD     DMA_DONE24_Handler         ; 75 
        DCD     DMA_DONE25_Handler         ; 76 
        DCD     DMA_DONE26_Handler         ; 77 
        DCD     DMA_DONE27_Handler         ; 78 
        DCD     DMA_DONE28_Handler         ; 79 
        DCD     DMA_DONE29_Handler         ; 80 
        DCD     DMA_DONE30_Handler         ; 81         
        DCD     DMA_DONE31_Handler         ; 82         
        DCD     IRQ_PORTA_Handler          ; 83
        DCD     IRQ_PORTB_Handler          ; 84
        DCD     IRQ_PORTC_Handler          ; 85
        DCD     IRQ_PORTD_Handler          ; 86
        DCD     IRQ_PORTE_Handler          ; 87
        DCD     0                          ; 88  Reserved         
        DCD     INT_ETH0_Handler           ; 89
        DCD     0                          ; 90
        DCD     INT_SPW0_Handler           ; 91
        DCD     0                          ; 92
        DCD     INT_TMR0_Handler           ; 93
        DCD     INT_TMR1_Handler           ; 94
        DCD     INT_TMR2_Handler           ; 95
        DCD     INT_TMR3_Handler           ; 96
        DCD     0        				   ; 97 Reserved	
        DCD     0        				   ; 98 Reserved	
        DCD     INT_CAN0_Handler           ; 99
        DCD     0        				   ; 100 Reserved	
        DCD     0        				   ; 101 Reserved	
        DCD     0        				   ; 102 Reserved	
        DCD     0        				   ; 103 Reserved					
        DCD     INT_SSP0_Handler           ; 104
        DCD     0        				   ; 105 Reserved	
        DCD     0        				   ; 106 Reserved	
        DCD     0        				   ; 107 Reserved	
        DCD     INT_UART0_Handler          ; 108
        DCD     INT_UART1_Handler          ; 109
        DCD     0        				   ; 110 Reserved	
        DCD     0        				   ; 111 Reserved	
        DCD     0        				   ; 112 Reserved	
        DCD     INT_RX_ARC0_Handler        ; 113
        DCD     INT_TX_ARC0_Handler        ; 114
        DCD     0        				   ; 115 Reserved	
        DCD     0        				   ; 116 Reserved	
        DCD     INT_MIL0_Handler           ; 117
        DCD     INT_MIL1_Handler           ; 118
        DCD     INT_ADC0_Handler           ; 119
        DCD     INT_ADC1_Handler           ; 120
        DCD     INT_DAC0_Handler           ; 121
        DCD     INT_DAC1_Handler           ; 122							

		
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;
;; Default interrupt handlers.
;;
        THUMB

        PUBWEAK Reset_Handler
        SECTION .text:CODE:NOROOT(2)
Reset_Handler
        LDR     R0, =SystemInit
        BLX     R0
        
        LDR     R0, =__vector_table
        LDR     R1, =0xE000ED08
        STR     R0, [R1] 
        
        LDR     R0, =__iar_program_start
        BX      R0

        PUBWEAK NMI_Handler
        SECTION .text:CODE:NOROOT(1)
NMI_Handler
        B NMI_Handler

        PUBWEAK HardFault_Handler
        SECTION .text:CODE:NOROOT(1)
HardFault_Handler
        B HardFault_Handler

        PUBWEAK MemoryMngFault_Handler
        SECTION .text:CODE:NOROOT(1)
MemoryMngFault_Handler
        B MemoryMngFault_Handler

        PUBWEAK BusFault_Handler
        SECTION .text:CODE:NOROOT(1)
BusFault_Handler
        B BusFault_Handler

        PUBWEAK UsageFault_Handler
        SECTION .text:CODE:NOROOT(1)
UsageFault_Handler
        B UsageFault_Handler

        PUBWEAK SVC_Handler
        SECTION .text:CODE:NOROOT(1)
SVC_Handler
        B SVC_Handler

        PUBWEAK PendSV_Handler
        SECTION .text:CODE:NOROOT(1)
PendSV_Handler
        B PendSV_Handler

        PUBWEAK SysTick_Handler
        SECTION .text:CODE:NOROOT(1)
SysTick_Handler
        B SysTick_Handler

        PUBWEAK FT_IF0_Handler
        SECTION .text:CODE:NOROOT(1)
FT_IF0_Handler
        B FT_IF0_Handler

        PUBWEAK FT_IF1_Handler    
        SECTION .text:CODE:NOROOT(1)
FT_IF1_Handler    
        B FT_IF1_Handler    

        PUBWEAK FT_IF2_Handler    
        SECTION .text:CODE:NOROOT(1)
FT_IF2_Handler    
        B FT_IF2_Handler    
        
        PUBWEAK FT_IF3_Handler    
        SECTION .text:CODE:NOROOT(1)
FT_IF3_Handler    
        B FT_IF3_Handler    

        PUBWEAK CLK_IF_Handler    
        SECTION .text:CODE:NOROOT(1)
CLK_IF_Handler    
        B CLK_IF_Handler    
        
        PUBWEAK PVD_IF_Handler    
        SECTION .text:CODE:NOROOT(1)
PVD_IF_Handler    
        B PVD_IF_Handler    

        PUBWEAK RTC_IF_Handler    
        SECTION .text:CODE:NOROOT(1)
RTC_IF_Handler    
        B RTC_IF_Handler    
        
        PUBWEAK BKP_IF_Handler    
        SECTION .text:CODE:NOROOT(1)
BKP_IF_Handler    
        B BKP_IF_Handler     
        
        PUBWEAK EXT_INTERROR0_Handler    
        SECTION .text:CODE:NOROOT(1)
EXT_INTERROR0_Handler    
        B EXT_INTERROR0_Handler            

        PUBWEAK EXT_INTERROR1_Handler    
        SECTION .text:CODE:NOROOT(1)
EXT_INTERROR1_Handler    
        B EXT_INTERROR1_Handler    
        
        PUBWEAK EXT_INTERROR2_Handler    
        SECTION .text:CODE:NOROOT(1)
EXT_INTERROR2_Handler    
        B EXT_INTERROR2_Handler    

        PUBWEAK EXT_INTERROR3_Handler    
        SECTION .text:CODE:NOROOT(1)
EXT_INTERROR3_Handler    
        B EXT_INTERROR3_Handler    
        
        PUBWEAK EXT_INTERROR4_Handler    
        SECTION .text:CODE:NOROOT(1)
EXT_INTERROR4_Handler    
        B EXT_INTERROR4_Handler    

        PUBWEAK EXT_INTERROR5_Handler    
        SECTION .text:CODE:NOROOT(1)
EXT_INTERROR5_Handler    
        B EXT_INTERROR5_Handler    
        
        PUBWEAK EXT_INTERROR6_Handler    
        SECTION .text:CODE:NOROOT(1)
EXT_INTERROR6_Handler    
        B EXT_INTERROR6_Handler    

        PUBWEAK EXT_INTERROR7_Handler    
        SECTION .text:CODE:NOROOT(1)
EXT_INTERROR7_Handler    
        B EXT_INTERROR7_Handler       
        
        PUBWEAK DMA_ERR_Handler    
        SECTION .text:CODE:NOROOT(1)
DMA_ERR_Handler    
        B DMA_ERR_Handler         
        
        PUBWEAK DMA_DONE0_Handler    
        SECTION .text:CODE:NOROOT(1)
DMA_DONE0_Handler    
        B DMA_DONE0_Handler           
        
        PUBWEAK DMA_DONE1_Handler    
        SECTION .text:CODE:NOROOT(1)
DMA_DONE1_Handler    
        B DMA_DONE1_Handler    
        
        PUBWEAK DMA_DONE2_Handler    
        SECTION .text:CODE:NOROOT(1)
DMA_DONE2_Handler    
        B DMA_DONE2_Handler    
        
        PUBWEAK DMA_DONE3_Handler    
        SECTION .text:CODE:NOROOT(1)
DMA_DONE3_Handler    
        B DMA_DONE3_Handler    
        
        PUBWEAK DMA_DONE4_Handler    
        SECTION .text:CODE:NOROOT(1)
DMA_DONE4_Handler    
        B DMA_DONE4_Handler    
        
        PUBWEAK DMA_DONE5_Handler    
        SECTION .text:CODE:NOROOT(1)
DMA_DONE5_Handler    
        B DMA_DONE5_Handler    
        
        PUBWEAK DMA_DONE6_Handler    
        SECTION .text:CODE:NOROOT(1)
DMA_DONE6_Handler    
        B DMA_DONE6_Handler    
        
        PUBWEAK DMA_DONE7_Handler    
        SECTION .text:CODE:NOROOT(1)
DMA_DONE7_Handler    
        B DMA_DONE7_Handler    
        
        PUBWEAK DMA_DONE8_Handler    
        SECTION .text:CODE:NOROOT(1)
DMA_DONE8_Handler    
        B DMA_DONE8_Handler    
        
        PUBWEAK DMA_DONE9_Handler    
        SECTION .text:CODE:NOROOT(1)
DMA_DONE9_Handler    
        B DMA_DONE9_Handler    

        PUBWEAK DMA_DONE10_Handler    
        SECTION .text:CODE:NOROOT(1)
DMA_DONE10_Handler    
        B DMA_DONE10_Handler    
        
        PUBWEAK DMA_DONE11_Handler    
        SECTION .text:CODE:NOROOT(1)
DMA_DONE11_Handler    
        B DMA_DONE11_Handler    
        
        PUBWEAK DMA_DONE12_Handler    
        SECTION .text:CODE:NOROOT(1)
DMA_DONE12_Handler    
        B DMA_DONE12_Handler    
        
        PUBWEAK DMA_DONE13_Handler    
        SECTION .text:CODE:NOROOT(1)
DMA_DONE13_Handler    
        B DMA_DONE13_Handler    
        
        PUBWEAK DMA_DONE14_Handler    
        SECTION .text:CODE:NOROOT(1)
DMA_DONE14_Handler    
        B DMA_DONE14_Handler    
        
        PUBWEAK DMA_DONE15_Handler    
        SECTION .text:CODE:NOROOT(1)
DMA_DONE15_Handler    
        B DMA_DONE15_Handler    
        
        PUBWEAK DMA_DONE16_Handler    
        SECTION .text:CODE:NOROOT(1)
DMA_DONE16_Handler    
        B DMA_DONE16_Handler    
        
        PUBWEAK DMA_DONE17_Handler    
        SECTION .text:CODE:NOROOT(1)
DMA_DONE17_Handler    
        B DMA_DONE17_Handler    
        
        PUBWEAK DMA_DONE18_Handler    
        SECTION .text:CODE:NOROOT(1)
DMA_DONE18_Handler    
        B DMA_DONE18_Handler    
        
        PUBWEAK DMA_DONE19_Handler    
        SECTION .text:CODE:NOROOT(1)
DMA_DONE19_Handler    
        B DMA_DONE19_Handler    
        
        PUBWEAK DMA_DONE20_Handler    
        SECTION .text:CODE:NOROOT(1)
DMA_DONE20_Handler    
        B DMA_DONE20_Handler    
        
        PUBWEAK DMA_DONE21_Handler    
        SECTION .text:CODE:NOROOT(1)
DMA_DONE21_Handler    
        B DMA_DONE21_Handler    
        
        PUBWEAK DMA_DONE22_Handler    
        SECTION .text:CODE:NOROOT(1)
DMA_DONE22_Handler    
        B DMA_DONE22_Handler    
        
        PUBWEAK DMA_DONE23_Handler    
        SECTION .text:CODE:NOROOT(1)
DMA_DONE23_Handler    
        B DMA_DONE23_Handler    
        
        PUBWEAK DMA_DONE24_Handler    
        SECTION .text:CODE:NOROOT(1)
DMA_DONE24_Handler    
        B DMA_DONE24_Handler    
        
        PUBWEAK DMA_DONE25_Handler    
        SECTION .text:CODE:NOROOT(1)
DMA_DONE25_Handler    
        B DMA_DONE25_Handler    
        
        PUBWEAK DMA_DONE26_Handler    
        SECTION .text:CODE:NOROOT(1)
DMA_DONE26_Handler    
        B DMA_DONE26_Handler    
        
        PUBWEAK DMA_DONE27_Handler    
        SECTION .text:CODE:NOROOT(1)
DMA_DONE27_Handler    
        B DMA_DONE27_Handler    
        
        PUBWEAK DMA_DONE28_Handler    
        SECTION .text:CODE:NOROOT(1)
DMA_DONE28_Handler    
        B DMA_DONE28_Handler    
        
        PUBWEAK DMA_DONE29_Handler    
        SECTION .text:CODE:NOROOT(1)
DMA_DONE29_Handler    
        B DMA_DONE29_Handler    
        
        PUBWEAK DMA_DONE30_Handler    
        SECTION .text:CODE:NOROOT(1)
DMA_DONE30_Handler    
        B DMA_DONE30_Handler    
        
        PUBWEAK DMA_DONE31_Handler    
        SECTION .text:CODE:NOROOT(1)
DMA_DONE31_Handler    
        B DMA_DONE31_Handler    
        
        PUBWEAK IRQ_PORTA_Handler    
        SECTION .text:CODE:NOROOT(1)
IRQ_PORTA_Handler    
        B IRQ_PORTA_Handler     
        
        PUBWEAK IRQ_PORTB_Handler    
        SECTION .text:CODE:NOROOT(1)
IRQ_PORTB_Handler    
        B IRQ_PORTB_Handler     
        
        PUBWEAK IRQ_PORTC_Handler    
        SECTION .text:CODE:NOROOT(1)
IRQ_PORTC_Handler    
        B IRQ_PORTC_Handler     
        
        PUBWEAK IRQ_PORTD_Handler    
        SECTION .text:CODE:NOROOT(1)
IRQ_PORTD_Handler    
        B IRQ_PORTD_Handler  

        PUBWEAK IRQ_PORTE_Handler    
        SECTION .text:CODE:NOROOT(1)
IRQ_PORTE_Handler    
        B IRQ_PORTE_Handler  		

        PUBWEAK INT_ETH0_Handler    
        SECTION .text:CODE:NOROOT(1)
INT_ETH0_Handler    
        B INT_ETH0_Handler 
		
        PUBWEAK INT_SPW0_Handler    
        SECTION .text:CODE:NOROOT(1)
INT_SPW0_Handler    
        B INT_SPW0_Handler

        PUBWEAK INT_TMR0_Handler    
        SECTION .text:CODE:NOROOT(1)
INT_TMR0_Handler    
        B INT_TMR0_Handler   

        PUBWEAK INT_TMR1_Handler    
        SECTION .text:CODE:NOROOT(1)
INT_TMR1_Handler    
        B INT_TMR1_Handler

        PUBWEAK INT_TMR2_Handler    
        SECTION .text:CODE:NOROOT(1)
INT_TMR2_Handler    
        B INT_TMR2_Handler  

        PUBWEAK INT_TMR3_Handler    
        SECTION .text:CODE:NOROOT(1)
INT_TMR3_Handler    
        B INT_TMR3_Handler

        PUBWEAK INT_CAN0_Handler    
        SECTION .text:CODE:NOROOT(1)
INT_CAN0_Handler    
        B INT_CAN0_Handler

        PUBWEAK INT_SSP0_Handler    
        SECTION .text:CODE:NOROOT(1)
INT_SSP0_Handler    
        B INT_SSP0_Handler  		
        
        PUBWEAK INT_UART0_Handler    
        SECTION .text:CODE:NOROOT(1)
INT_UART0_Handler    
        B INT_UART0_Handler    
        
        PUBWEAK INT_UART1_Handler    
        SECTION .text:CODE:NOROOT(1)
INT_UART1_Handler    
        B INT_UART1_Handler
		
        PUBWEAK INT_RX_ARC0_Handler    
        SECTION .text:CODE:NOROOT(1)
INT_RX_ARC0_Handler    
        B INT_RX_ARC0_Handler 
		
        PUBWEAK INT_TX_ARC0_Handler    
        SECTION .text:CODE:NOROOT(1)
INT_TX_ARC0_Handler    
        B INT_TX_ARC0_Handler 
        
        PUBWEAK INT_MIL0_Handler    
        SECTION .text:CODE:NOROOT(1)
INT_MIL0_Handler    
        B INT_MIL0_Handler

        PUBWEAK INT_MIL1_Handler    
        SECTION .text:CODE:NOROOT(1)
INT_MIL1_Handler    
        B INT_MIL1_Handler  

        PUBWEAK INT_ADC0_Handler    
        SECTION .text:CODE:NOROOT(1)
INT_ADC0_Handler    
        B INT_ADC0_Handler  

        PUBWEAK INT_ADC1_Handler    
        SECTION .text:CODE:NOROOT(1)
INT_ADC1_Handler    
        B INT_ADC1_Handler  		
        
        PUBWEAK INT_DAC0_Handler    
        SECTION .text:CODE:NOROOT(1)
INT_DAC0_Handler    
        B INT_DAC0_Handler    
        
        PUBWEAK INT_DAC1_Handler    
        SECTION .text:CODE:NOROOT(1)
INT_DAC1_Handler    
        B INT_DAC1_Handler    

        END

;* END OF FILE startup_1986ve8t.s */

