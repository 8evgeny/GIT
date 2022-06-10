/**
  ******************************************************************************
  * @file    main.c
  * @author  Milandr Application Team
  * @version V2.0.2
  * @date    21/09/2021
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
#include "MDR32F9Qx_config.h"
#include "MDR32F9Qx_port.h"
#include "MDR32F9Qx_rst_clk.h"
#include "MDR32F9Qx_bkp.h"

/** @addtogroup __MDR32Fx_StdPeriph_Examples MDR32Fx StdPeriph Examples
 * @{
 */

/** @addtogroup __MDR1986VE3_EVAL MDR1986VE3 Evaluation Board
 * @{
 */

/** @addtogroup BKP_STANDBY_3T BKP_STANDBY
 * @{
 */

/* Private variables ---------------------------------------------------------*/
PORT_InitTypeDef PORT_InitStructure;

/* Private functions ---------------------------------------------------------*/
/**
 * @brief  Main program.
 * @param  None
 * @retval None
 */
int main(void)
{
    uint32_t i, j;
    /* Enables the HSI clock on PORTB */
    RST_CLK_PCLKcmd(RST_CLK_PCLK_PORTB, ENABLE);

    PORT_StructInit(&PORT_InitStructure);
    /* Configure PORTB pins 0 for output to switch LEDs on/off */
    PORT_InitStructure.PORT_Pin = PORT_Pin_0;
    PORT_InitStructure.PORT_OE = PORT_OE_OUT;
    PORT_InitStructure.PORT_FUNC = PORT_FUNC_PORT;
    PORT_InitStructure.PORT_MODE = PORT_MODE_DIGITAL;
    PORT_InitStructure.PORT_SPEED = PORT_SPEED_SLOW;

    PORT_Init(MDR_PORTB, &PORT_InitStructure);

    for(j = 0; j < 10; j++)
    {
        if (PORT_ReadInputDataBit(MDR_PORTB, PORT_Pin_0) == 0)
        {
            PORT_SetBits(MDR_PORTB, PORT_Pin_0);
        }
        else
        {
            PORT_ResetBits(MDR_PORTB, PORT_Pin_0);
        }
        for (i = 0; i < 200000; i++);

    }

    PORT_SetBits(MDR_PORTB, PORT_Pin_0);

    /* STANDBY mode */
    BKP_EnterSTANDBYMode();

    while(1);
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

/** @} *//* End of group BKP_STANDBY_3T */

/** @} *//* End of group __MDR1986VE3_EVAL */

/** @} *//* End of group __MDR32Fx_StdPeriph_Examples */

/******************* (C) COPYRIGHT 2021 Milandr *******************************/

/* END OF FILE main.c */

