/**
  ******************************************************************************
  * @file    main.c
  * @author  Milandr Application Team
  * @version V2.0.2
  * @date    23/09/2021
  * @brief   Main program body.
  ******************************************************************************
  * <br><br>
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, MILANDR SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2021 Milandr</center></h2>
  */

/* Includes ------------------------------------------------------------------*/
#include "MDR32F9Qx_port.h"
#include "MDR32F9Qx_rst_clk.h"
#include "MDR32F9Qx_adciu.h"

/** @addtogroup __MDR1986BE4_StdPeriph_Examples MDR1986BE4 StdPeriph Examples
  * @{
  */

/** @addtogroup __MDR1986BE4_EVAL MDR1986BE4 Evaluation Board
  * @{
  */

/** @addtogroup ADCIU_Interrupt ADCIU_Interrupt
  * @{
  */

/* Private variables ---------------------------------------------------------*/
PORT_InitTypeDef PORT_InitStructure;
ADCIU_InitTypeDef ADCIU_InitStruct;
ADCIU_ChannelInitTypeDef ADCIU_ChannelInitStruct;

int32_t ADCIU_data[512];
uint32_t count = 0;

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */
int main(void)
{
    /* Reset clock setting */
    RST_CLK_DeInit();

    /* Enable HSE */
    RST_CLK_HSEconfig(RST_CLK_HSE_ON);

    /* Wait untile HSE not ready */
    if(RST_CLK_HSEstatus() == ERROR)
        while(1);

    /* Config the PLL */
    RST_CLK_CPU_PLLconfig(RST_CLK_CPU_PLLsrcHSEdiv1, RST_CLK_CPU_PLLmul4);

    /* Enable PLL */
    RST_CLK_CPU_PLLcmd(ENABLE);

    /* Wait until PLL not ready */
    if(RST_CLK_CPU_PLLstatus() == ERROR)
        while(1);

    /* Use PLL */
    RST_CLK_CPU_PLLuse(ENABLE);
    /* Set clock prescaller */
    RST_CLK_CPUclkPrescaler(RST_CLK_CPUclkDIV1);
    /* Set CPU_C3 clock source */
    RST_CLK_CPUclkSelection(RST_CLK_CPUclkCPU_C3);

    SystemCoreClockUpdate();

    /* Enable the ADCIU clock and port a clock */
    RST_CLK_PCLKcmd((RST_CLK_PCLK_ADCUI | RST_CLK_PCLK_PORTA), ENABLE);

    /* Init the PORTA */
    PORT_InitStructure.PORT_Pin = PORT_Pin_11;
    PORT_InitStructure.PORT_FUNC = PORT_FUNC_PORT;
    PORT_InitStructure.PORT_MODE = PORT_MODE_DIGITAL;
    PORT_InitStructure.PORT_OE = PORT_OE_OUT;
    PORT_InitStructure.PORT_SPEED = PORT_SPEED_MAXFAST;

    PORT_Init(MDR_PORTA, &PORT_InitStructure);

    /* ADCIU clock deinit */
    RST_CLK_ADCIUclkDeInit();

    /* Select the ADCIU clock source */
    RST_CLK_ADCIUclkSelection(RST_CLK_ADCIUclkPLLCPU);

    /* Set the ADCIU clock diveder */
    RST_CLK_ADCIUclkPrescaler(RST_CLK_ADCIUclkDIV8);

    /* Enable ADCIU clock */
    RST_CLK_ADCIUclkCMD(ENABLE);

    /* Reset the ADCIU */
    ADCIU_DeInit();

    /* ADCIU init struct */
    ADCIU_StructInit(&ADCIU_InitStruct);

    /* ADCIU init */
    ADCIU_Init(&ADCIU_InitStruct);

    /* ADCIU init channel struct */
    ADCIU_ChannelStructInit(&ADCIU_ChannelInitStruct);

    ADCIU_ChannelInitStruct.ADCIU_ChopperState = ENABLE;
    ADCIU_ChannelInitStruct.ADCIU_IT_FIFO_NotEmptyState = ENABLE;
    ADCIU_ChannelInitStruct.ADCIU_IT_FIFO_OverflowState = ENABLE;

    /* ADCIU init he first channel */
    ADCIU_InitChannel(ADCIU_CHANNEL_NUMBER1, &ADCIU_ChannelInitStruct);

    /* Enable ADCIU IRQ */
    NVIC_EnableIRQ(ADCIU_CH1_IRQn);

    /* Enalbe ADCIU */
    ADCIU_ChannelCmd(ADCIU_CHANNEL_NUMBER1, ENABLE);

    while(1);
}

/**
  * @brief ADCIU channel 1 IQR handler.
  * @param  None
  * @retval None
  */
void ADCIU_CH1_IRQHandler()
{
    if(ADCIU_GetStatusFlag(ADCIU_CHANNEL_NUMBER1, ADCIU_FLAG_FITO_NOT_EMPTY))
    {
        ADCIU_data[count] = ADCIU_GetResult(ADCIU_CHANNEL_NUMBER1);
        if(ADCIU_data[count]& (1 << 21))
        {
            ADCIU_data[count] |=  (0xFFUL << 24);
        }
        count++;
    }
    if(ADCIU_GetStatusFlag(ADCIU_CHANNEL_NUMBER1, ADCIU_FLAG_FIFO_OVERFLOW))
    {
        ADCIU_ClearIT_OVERFLOW(ADCIU_CHANNEL_NUMBER1);
        PORT_SetBits(MDR_PORTA, PORT_Pin_11);
    }
    if(count == 512)
    {
        ADCIU_ChannelCmd(ADCIU_CHANNEL_NUMBER1, DISABLE);
    }
}

/**
  * @brief  Reports the source file name, the source line number
  *         and expression text (if USE_ASSERT_INFO == 2) where
  *         the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @param  expr:
  * @retval None
  */
#if (USE_ASSERT_INFO == 1)
void assert_failed(uint8_t* file, uint32_t line)
{
    /* User can add his own implementation to report the source file name and line number.
       Ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

    /* Infinite loop */
    while (1)
    {
    }
}
#elif (USE_ASSERT_INFO == 2)
void assert_failed(uint8_t* file, uint32_t line, const uint8_t* expr)
{
    /* User can add his own implementation to report the source file name, line number and
       expression text.
       Ex: printf("Wrong parameters value (%s): file %s on line %d\r\n", expr, file, line) */

    /* Infinite loop */
    while (1)
    {
    }
}
#endif /* USE_ASSERT_INFO */

/** @} */ /* End of group ADCIU */

/** @} */ /* End of group __MDR1986BE4_EVAL */

/** @} */ /* End of group __MDR1986BE4_StdPeriph_Examples */

/******************* (C) COPYRIGHT 2021 Milandr *******************************/

/* END OF FILE main.c */

