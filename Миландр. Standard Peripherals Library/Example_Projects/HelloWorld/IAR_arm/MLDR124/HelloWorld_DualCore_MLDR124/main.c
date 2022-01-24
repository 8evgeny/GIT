#include <mldr124.h>
#include "mldr124_port.h"
#include <stdio.h>

volatile uint32_t hardFaultNum;

#define locked   1
#define unlocked 0

#define BRD_V3

#ifdef BRD_V3
    #define LedGreen PORT_Pin_17
    #define LedBlue  PORT_Pin_19
    #define LedRed   PORT_Pin_21

#elif defined (BRD_V2)
    #define LedGreen PORT_Pin_16
    #define LedBlue  PORT_Pin_18
    #define LedRed   PORT_Pin_20
#endif

#define LED_PORT PORTC
#define LED_DELAY_G        800000
#define LED_DELAY_B       1000000

void Delay(uint32_t waitTicks)
{
    volatile uint32_t i;
    for (i = 0; i < waitTicks; i++);
}

void LED_Show(uint32_t ledSel, uint32_t delay)
{
    PORT_SetBits(LED_PORT, ledSel);
    Delay(delay);
    PORT_ResetBits(LED_PORT, ledSel);
    Delay(delay);
}


extern void lock_mutex(void * mutex);
extern void unlock_mutex(void * mutex);

unsigned int output_mutex = unlocked;

void vErrorHandler( void );

//  ОПИСАНИЕ:
//    Два ядра независимо исполняют два бесконечных цикла, в каждом цикле мигает свой светодиод.
//
//    Распараллеливание ядер происходит в Reset_Handler - startup_mldr124_DC.s
//    После Reset ядра остаются разделенными и вместе начинают исполнять начальный загрузчик.
//    Из-за этого ядра конфликтуют и поэтому по Reset проект не перезапускается.
//    Мигание двумя светодиодами наблюдается только при включении питания!

int main()
{
    volatile uint32_t cnt;
    PORT_InitTypeDef Port;

    unlock_mutex(&output_mutex);
    
    if (SMPID == 0)
    {
        CLK_CNTR->KEY = 0x8555AAA1;
        CLK_CNTR->PER0_CLK =  0xFFFFFFFF;
        CLK_CNTR->PER1_CLK =  0xFFFFFFFF; 

        LED_PORT->KEY = 0x8555AAA1;
        PORT_StructInit( &Port );

        Port.PORT_Pin     = LedGreen | LedBlue;
        Port.PORT_SOE     = PORT_SOE_OUT;
        Port.PORT_SFUNC   = PORT_SFUNC_PORT;
        Port.PORT_SANALOG = PORT_SANALOG_DIGITAL;
        Port.PORT_SPWR    = PORT_SPWR_10;
        PORT_Init( LED_PORT, &Port );
    }

    if (SMPID)
    {
        while (1)
        {
            lock_mutex(&output_mutex); // Wait until the mutex is acquired 
            // call  nonreenterable
            unlock_mutex(&output_mutex); // Leave critical section - release mutex 

            LED_Show(LedBlue, LED_DELAY_B);
        }
    }
    else
    {
        while (1)
        {
            lock_mutex(&output_mutex); // Wait until the mutex is acquired 
            // call  nonreenterable
            unlock_mutex(&output_mutex); // Leave critical section - release mutex 

            LED_Show(LedGreen, LED_DELAY_G);
        }
    }
}

void vErrorHandler( void )
{
    while( 1 )
    {
    }
}

void Hard_fault_handler_c(unsigned int* hardfault_args)
{
    volatile unsigned int stacked_r0;
    volatile unsigned int stacked_r1;
    volatile unsigned int stacked_r2;
    volatile unsigned int stacked_r3;
    volatile unsigned int stacked_r12;
    volatile unsigned int stacked_lr;
    volatile unsigned int stacked_pc;
    volatile unsigned int stacked_psr;

    FT_CNTR->KEY = 0x8555AAA1;

    if( !( FT_CNTR->EVENT[1] &0x00000099 ) )
    {
        stacked_r0 = ((unsigned long) hardfault_args[0]);
        stacked_r1 = ((unsigned long) hardfault_args[1]);
        stacked_r2 = ((unsigned long) hardfault_args[2]);
        stacked_r3 = ((unsigned long) hardfault_args[3]);

        stacked_r12 = ((unsigned long) hardfault_args[4]);
        stacked_lr  = ((unsigned long) hardfault_args[5]);
        stacked_pc  = ((unsigned long) hardfault_args[6]);
        stacked_psr = ((unsigned long) hardfault_args[7]);

//        printf ("[Hard fault handler]\r\n");
//        printf ("R0 = 0x%08x\r\n", stacked_r0);
//        printf ("R1 = 0x%08x\r\n", stacked_r1);
//        printf ("R2 = 0x%08x\r\n", stacked_r2);
//        printf ("R3 = 0x%08x\r\n", stacked_r3);
//        printf ("R12 = 0x%08x\r\n", stacked_r12);
//        printf ("LR = 0x%08x\r\n", stacked_lr);
//        printf ("PC = 0x%08x\r\n", stacked_pc);
//        printf ("PSR = 0x%08x\r\n", stacked_psr);

        while(1);
    }

    FT_CNTR->KEY = 0x00000000;
    hardFaultNum++;
}

#if defined (__ICCARM__)
void HardFault_Handler( void )
{
    unsigned int contr_reg;

    contr_reg = __get_CONTROL();
    if(contr_reg&2)
    {
        asm("MRS R0, PSP");
    }
    else
    {
        asm("MRS R0, MSP");
    }
    asm("B    (Hard_fault_handler_c)"); // Top of stack is in R0. It is passed to C-function.
}
#endif
