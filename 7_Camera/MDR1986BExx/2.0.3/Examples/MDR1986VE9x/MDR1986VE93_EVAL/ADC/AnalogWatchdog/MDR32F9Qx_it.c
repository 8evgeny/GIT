/**
  ******************************************************************************
  * @file    MDR32F9Qx_it.c
  * @author  Milandr Application Team
  * @version V2.0.0
  * @date    28/07/2021
  * @brief   Main Interrupt Service Routines.
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
#include "MDR32F9Qx_it.h"
#include "MDR32F9Qx_port.h"

/* Private variables ---------------------------------------------------------*/
int tmp ;
extern __IO uint32_t H_Level;

/* Private functions ---------------------------------------------------------*/


/*******************************************************************************
* Function Name  : ADC_IRQHandler
* Description    : This function handles ADC global interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void ADC_IRQHandler(void)
{
    if(ADC1_GetFlagStatus(ADCx_FLAG_OUT_OF_RANGE) == SET)
    {
        /* Turns LED1 On */
        PORT_SetBits(MDR_PORTF, PORT_Pin_0);
    }
    else
    {
        /* Turns LED1 Off */
        PORT_ResetBits(MDR_PORTF, PORT_Pin_0);
    }
    tmp = MDR_ADC->ADC1_RESULT & 0x0FFF;
    if(tmp > H_Level)
    {
        /* Turns LED2 On */
        PORT_SetBits(MDR_PORTF, PORT_Pin_1);
    }
    else
    {
        /* Turns LED2 Off */
        PORT_ResetBits(MDR_PORTF, PORT_Pin_1);
    }
    /* Clear ADC1 OUT_OF_RANGE interrupt bit */
    MDR_ADC->ADC1_STATUS = (ADCx_IT_END_OF_CONVERSION | ADCx_IT_OUT_OF_RANGE)<<2;
}


/******************* (C) COPYRIGHT 2021 Milandr *******************************/

/* END OF FILE MDR32F9Qx_it.c */
