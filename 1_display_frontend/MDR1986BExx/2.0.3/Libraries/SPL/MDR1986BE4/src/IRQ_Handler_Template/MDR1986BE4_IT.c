/**
  ******************************************************************************
  * @file    MDR1986BE4_IT.c
  * @author  Milandr Application Team
  * @version V2.0.0
  * @date    14/05/2021
  * @brief   Main Interrupt Service Routines for MPU MDR1986BE4.
  ******************************************************************************
  * <br><br>
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, MILANDR SHALL NOT BE HELD LIABLE FOR ANY DIRECT, INDIRECT
  * OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2021 Milandr</center></h2>
  */

/* Includes ------------------------------------------------------------------*/
#include "MDR1986BE4_IT.h"

#if defined (_USE_DEBUG_UART_)

    /* Private macro ---------------------------------------------------------*/
    #define DEBUG_PRINTF(...) printf(__VA_ARGS__)

    /* Private function prototypes -------------------------------------------*/
    void Hard_fault_handler_c(unsigned int* hardfault_args);

#endif /* #if defined (_USE_DEBUG_UART_) */

/* Private functions ---------------------------------------------------------*/


/*******************************************************************************
* Function Name  : HardFault_Handler
* Description    : This function handles Hard Fault exception.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void HardFault_Handler(void)
{
#if defined (_USE_DEBUG_UART_)
    unsigned int contr_reg;
    contr_reg = __get_CONTROL();
    if(contr_reg & 2)
    {
#if defined ( __CMCARM__ )
        __ASM MRS R0, PSP;
#else
        __ASM("MRS R0, PSP");
#endif
    }
    else
    {
#if defined ( __CMCARM__ )
        __ASM MRS R0, MSP;
#else
        __ASM("MRS R0, MSP");
#endif
    }
    //top of stack is in R0. It is passed to C-function.
#if defined ( __CMCARM__ )
    __ASM BL (Hard_fault_handler_c);
#else
    __ASM("BL (Hard_fault_handler_c)");
#endif

    /* Go to infinite loop when Hard Fault exception occurs */
    while (1);

#endif /* #if defined (_USE_DEBUG_UART_) */
}

/*******************************************************************************
* Function Name  : SVC_Handler
* Description    : This function handles SVCall exception.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void SVC_Handler(void)
{
}

/*******************************************************************************
* Function Name  : PendSV_Handler
* Description    : This function handles Debug PendSV exception.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void PendSV_Handler(void)
{
}

/*******************************************************************************
* Function Name  : SysTick_Handler
* Description    : This function handles SysTick Handler.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void SysTick_Handler(void)
{
}

/*******************************************************************************
* Function Name  : DMA_IRQHandler
* Description    : This function handles DMA global interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void DMA_IRQHandler(void)
{
}

/*******************************************************************************
* Function Name  : UART1_IRQHandler
* Description    : This function handles UART1 global interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void UART1_IRQHandler(void)
{
}

/*******************************************************************************
* Function Name  : UART2_IRQHandler
* Description    : This function handles UART2 global interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void UART2_IRQHandler(void)
{
}

/*******************************************************************************
* Function Name  : SSP1_IRQHandler
* Description    : This function handles SSP1 global interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void SSP1_IRQHandler(void)
{
}

/*******************************************************************************
* Function Name  : POWER_IRQHandler
* Description    : This function handles POWER global interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void POWER_IRQHandler(void)
{
}

/*******************************************************************************
* Function Name  : WWDG_IRQHandler
* Description    : This function handles WWDG global interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void WWDG_IRQHandler(void)
{
}

/*******************************************************************************
* Function Name  : TIMER1_IRQHandler
* Description    : This function handles TIMER1 global interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void TIMER1_IRQHandler(void)
{
}

/*******************************************************************************
* Function Name  : TIMER2_IRQHandler
* Description    : This function handles TIMER2 global interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void TIMER2_IRQHandler(void)
{
}

/*******************************************************************************
* Function Name  : ADC_IRQHandler
* Description    : This function handles ADC global interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void ADC_IRQHandler(void)
{
}

/*******************************************************************************
* Function Name  : COMPARATOR_IRQHandler
* Description    : This function handles COMPARATOR global interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void COMPARATOR_IRQHandler(void)
{
}

/*******************************************************************************
* Function Name  : BACKUP_IRQHandler
* Description    : This function handles BACKUP global interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void BACKUP_IRQHandler(void)
{
}

/*******************************************************************************
* Function Name  : EXT_INT0_IRQHandler
* Description    : This function handles EXT_INT0 interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void EXT_INT0_IRQHandler(void)
{
}

/*******************************************************************************
* Function Name  : EXT_INT1_IRQHandler
* Description    : This function handles EXT_INT1 interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void EXT_INT1_IRQHandler(void)
{
}

/*******************************************************************************
* Function Name  : EXT_INT2_IRQHandler
* Description    : This function handles EXT_INT2 interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void EXT_INT2_IRQHandler(void)
{
}

/*******************************************************************************
* Function Name  : ADCIU_CH1_IRQHandler
* Description    : This function handles ADCIU_CH1 interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void ADCIU_CH1_IRQHandler(void)
{
}

/*******************************************************************************
* Function Name  : ADCIU_CH2_IRQHandler
* Description    : This function handles ADCIU_CH2 interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void ADCIU_CH2_IRQHandler(void)
{
}

/*******************************************************************************
* Function Name  : ADCIU_CH3_IRQHandler
* Description    : This function handles ADCIU_CH3 interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void ADCIU_CH3_IRQHandler(void)
{
}

/*******************************************************************************
* Function Name  : ADCIU_CH4_IRQHandler
* Description    : This function handles ADCIU_CH4 interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void ADCIU_CH4_IRQHandler(void)
{
}

/*******************************************************************************
* Function Name  : ADCIU_CH5_IRQHandler
* Description    : This function handles ADCIU_CH5 interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void ADCIU_CH5_IRQHandler(void)
{
}

/*******************************************************************************
* Function Name  : ADCIU_CH6_IRQHandler
* Description    : This function handles ADCIU_CH6 interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void ADCIU_CH6_IRQHandler(void)
{
}

/*******************************************************************************
* Function Name  : ADCIU_CH7_IRQHandler
* Description    : This function handles ADCIU_CH7 interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void ADCIU_CH7_IRQHandler(void)
{
}

/*******************************************************************************
* Function Name  : ADCIU_CH8_IRQHandler
* Description    : This function handles ADCIU_CH8 interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void ADCIU_CH8_IRQHandler(void)
{
}

/*******************************************************************************
* Function Name  : ADCIU_IRQHandler
* Description    : This function handles ADCIU interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void ADCIU_IRQHandler(void)
{
}


#if defined (_USE_DEBUG_UART_)
/**
  * @brief  Handles hard fault exception, stores stack and prints stack via UART
  * @param  hardfault_args: MCU stack
  * @retval None
  */
void Hard_fault_handler_c(unsigned int* hardfault_args)
{
    unsigned int stacked_r0;
    unsigned int stacked_r1;
    unsigned int stacked_r2;
    unsigned int stacked_r3;
    unsigned int stacked_r12;
    unsigned int stacked_lr;
    unsigned int stacked_pc;
    unsigned int stacked_psr;

    stacked_r0 = ((unsigned long) hardfault_args[0]);
    stacked_r1 = ((unsigned long) hardfault_args[1]);
    stacked_r2 = ((unsigned long) hardfault_args[2]);
    stacked_r3 = ((unsigned long) hardfault_args[3]);

    stacked_r12 = ((unsigned long) hardfault_args[4]);
    stacked_lr  = ((unsigned long) hardfault_args[5]);
    stacked_pc  = ((unsigned long) hardfault_args[6]);
    stacked_psr = ((unsigned long) hardfault_args[7]);

    DEBUG_PRINTF("[Hard fault handler]\r\n");
    DEBUG_PRINTF("R0 = 0x%x\r\n", stacked_r0);
    DEBUG_PRINTF("R1 = 0x%x\r\n", stacked_r1);
    DEBUG_PRINTF("R2 = 0x%x\r\n", stacked_r2);
    DEBUG_PRINTF("R3 = 0x%x\r\n", stacked_r3);
    DEBUG_PRINTF("R12 = 0x%x\r\n", stacked_r12);
    DEBUG_PRINTF("LR = 0x%x\r\n", stacked_lr);
    DEBUG_PRINTF("PC = 0x%x\r\n", stacked_pc);
    DEBUG_PRINTF("PSR = 0x%x\r\n", stacked_psr);

    /* Go to infinite loop when Hard Fault exception occurs */
    while (1);
}

#endif /* #if defined (_USE_DEBUG_UART_) */


/*********************** (C) COPYRIGHT 2021 Milandr ****************************
*
* END OF FILE MDR1986BE4_IT.c */
