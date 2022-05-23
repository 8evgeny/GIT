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
#include "MDR32F9Qx_ssp.h"
#include "MDR32F9Qx_rst_clk.h"
#include "MDR32F9Qx_port.h"

/** @addtogroup __MDR1986BE4_StdPeriph_Examples MDR1986BE4 StdPeriph Examples
  * @{
  */

/** @addtogroup __MDR1986BE4_EVAL MDR1986BE4 Evaluation Board
  * @{
  */

/** @addtogroup SPI_FullDuplex SPI_FullDuplex
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
typedef enum { FAILED = 0, PASSED = !FAILED} TestStatus;

/* Private define ------------------------------------------------------------*/
#define BufferSize         32

/* Private variables ---------------------------------------------------------*/
SSP_InitTypeDef sSSP;
PORT_InitTypeDef PORT_InitStructure;

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void)
{
    RST_CLK_DeInit();
    RST_CLK_CPU_PLLconfig(RST_CLK_CPU_PLLsrcHSIdiv2,0);
    /* Enable peripheral clocks ----------------------------------------------*/
    RST_CLK_PCLKcmd((RST_CLK_PCLK_RST_CLK | RST_CLK_PCLK_SSP1),ENABLE);
    RST_CLK_PCLKcmd((RST_CLK_PCLK_PORTA ), ENABLE);

    /* Init NVIC */
    SCB->AIRCR = 0x05FA0000 | ((uint32_t)0x500);
    /* Disable all interrupt */
    NVIC->ICPR[0] = 0xFFFFFFFF;
    NVIC->ICER[0] = 0xFFFFFFFF;

    /* Disable all DMA request */
    MDR_DMA->CHNL_REQ_MASK_CLR = 0xFFFFFFFF;
    MDR_DMA->CHNL_USEBURST_CLR = 0xFFFFFFFF;

    /* Reset PORTA settings */
    PORT_DeInit(MDR_PORTA);

    /*Init structure*/
    PORT_StructInit (&PORT_InitStructure);

    /* Configure SSP1 pins: FSS, CLK, RXD, TXD */
    /* Configure PORTA pins 12, 13, 15 */
    PORT_InitStructure.PORT_FUNC  = PORT_FUNC_MAIN;
    PORT_InitStructure.PORT_Pin   = (PORT_Pin_12 | PORT_Pin_13 | PORT_Pin_15);
    PORT_InitStructure.PORT_MODE 	= PORT_MODE_DIGITAL;
    PORT_InitStructure.PORT_OE    = PORT_OE_OUT;
    PORT_Init(MDR_PORTA, &PORT_InitStructure);

    PORT_InitStructure.PORT_Pin   = (PORT_Pin_14);
    PORT_InitStructure.PORT_OE    = PORT_OE_IN;
    PORT_Init(MDR_PORTA, &PORT_InitStructure);

    /* Reset all SSP settings */
#if defined (MDR_LEGACY_SUPPORT) && (MDR_LEGACY_SUPPORT) == 153
    SSP_DeInit(MDR_SSP1);
#else
    SSP_DeInit();
#endif

#if defined (MDR_LEGACY_SUPPORT) && (MDR_LEGACY_SUPPORT) == 153
    SSP_BRGInit(MDR_SSP1,SSP_HCLKdiv16);
#else
    SSP_BRGInit(SSP_HCLKdiv16);
#endif

    /* SSP1 MASTER configuration ---------------------------------------------*/
    SSP_StructInit (&sSSP);

    sSSP.SSP_SCR  = 0x10;
    sSSP.SSP_CPSDVSR = 2;
    sSSP.SSP_Mode = SSP_ModeMaster;
    sSSP.SSP_WordLength = SSP_WordLength16b;
    sSSP.SSP_SPH = SSP_SPH_1Edge;
    sSSP.SSP_SPO = SSP_SPO_Low;
    sSSP.SSP_FRF = SSP_FRF_SPI_Motorola;
    sSSP.SSP_HardwareFlowControl = SSP_HardwareFlowControl_SSE;
#if defined (MDR_LEGACY_SUPPORT) && (MDR_LEGACY_SUPPORT) == 153
    SSP_Init(MDR_SSP1,&sSSP);
#else
    SSP_Init(&sSSP);
#endif

    /* Enable SSP1 */
#if defined (MDR_LEGACY_SUPPORT) && (MDR_LEGACY_SUPPORT) == 153
    SSP_Cmd(MDR_SSP1, ENABLE);
#else
    SSP_Cmd(ENABLE);
#endif

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

/** @} */ /* End of group SPI_FullDuplex */

/** @} */ /* End of group __MDR1986BE4_EVAL */

/** @} */ /* End of group __MDR1986BE4_StdPeriph_Examples */

/******************* (C) COPYRIGHT 2021 Milandr *******************************/

/* END OF FILE main.c */


