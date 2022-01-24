/**
 ******************************************************************************
 * @file    main.c
 * @author  Milandr Application Team
 * @version V1.2.0
 * @date    24/09/2021
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
#include "MDR32F2C10_port.h"
#include "MDR32F2C10_clk.h"
#include "encoder.h"

/** @addtogroup  __MDR1986VK01_StdPeriph_Examples MDR1986VK01 StdPeriph Examples
 *  @{
 */

/** @addtogroup __MDR1986VK01_EVALBOARD MDR1986VK01 Demo Board
 *  @{
 */

/** @addtogroup encoder encoder
 *  @{
 */

PORT_InitTypeDef encPort;
uint32_t tmp;


void ClockInit()
{
    MDR_CLK->KEY = 0x8555AAA1;
    MDR_CLK->PER0_CLK =  0xFFFFFFFF;
    MDR_CLK->PER1_CLK =  0xFFFFFFFF;

    MDR_BKP->KEY = 0x8555AAA1;

    CLK_XSEConfig( CLK_HSE0, CLK_XSE_ON );
    if( !CLK_XSEWaitReady( CLK_HSE0 ) )
        while(1){}

    CLK_SetSystemClock( MAX_CLK_HSE0 );
}

int main()
{
    ClockInit();
    
    EncInit();
   
    PORT_StructInit( &encPort );
    ENC_PORT->KEY = 0x8555AAA1;

    encPort.PORT_Pin     = ENC_PIN_SW | ENC_PIN_A | ENC_PIN_B;
    encPort.PORT_SOE     = PORT_SOE_IN;
    encPort.PORT_SFUNC   = PORT_SFUNC_15;
    encPort.PORT_SANALOG = PORT_SANALOG_DIGITAL;
    encPort.PORT_SPWR    = PORT_SPWR_10;
    PORT_Init( ENC_PORT, &encPort );

    MDR_QEP1->QEPCTL  = 0x0000F228;

    while(1)
    {
    tmp = getEncCount();
    }
}

/** @} */ /* End of group encoder */

/** @} */ /* End of group __MDR1986VK01_EVALBOARD */

/** @} */ /* End of group __MDR1986VK01_StdPeriph_Examples */

/******************* (C) COPYRIGHT 2021 Milandr *******************************/

/* END OF FILE main.c */
