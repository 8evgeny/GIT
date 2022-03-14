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
  * TIME. AS A RESULT, MILANDR SHALL NOT BE HELD LIABLE FOR ANY DIRECT, INDIRECT
  * OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2021 Milandr </center></h2>
  ******************************************************************************
  */

/** @addtogroup __MDR32Fx_StdPeriph_Examples MDR32Fx StdPeriph Examples
  * @{
  */

/** @addtogroup __MDR1986VE3_EVAL MDR1986VE3 Evaluation Board
  * @{
  */

/** @addtogroup Ethernet_X2 Ethernet_X2
 *  @{
 */

/* Includes ------------------------------------------------------------------*/
#include "MDR32F9Qx_rst_clk.h"
#include "MDR32F9Qx_port.h"
#include "MDR32F9Qx_eeprom.h"
#include <MDR32F9Qx_bkp.h>
#include "MDR32F9Qx_eth.h"

/* Private macro ------------------------------------------------------------*/
#define ETH_BUFF_LEN    200

/* Private typedef -----------------------------------------------------------*/
typedef struct {
    uint32_t Length;
    uint8_t  Data_Packet[ETH_BUFF_LEN];
    uint32_t Send_Status;               // Reserved for TX status
} ETH_TX_Packet;

/* Private define ------------------------------------------------------------*/
#define Send_ETH1
#define Send_ETH2

#define ETH1_MAC_5				0xC8
#define ETH1_MAC_4				0x60
#define ETH1_MAC_3				0x00
#define ETH1_MAC_2				0x67
#define ETH1_MAC_1				0xA1
#define ETH1_MAC_0				0x10

#define ETH2_MAC_5				0xC8
#define ETH2_MAC_4				0x60
#define ETH2_MAC_3				0x00
#define ETH2_MAC_2				0x67
#define ETH2_MAC_1				0xB1
#define ETH2_MAC_0				0x20

/* Private variables ---------------------------------------------------------*/
PORT_InitTypeDef PORT_InitStructure;
__attribute__ ((aligned (4)))
ETH_TX_Packet ETH1_Send_Packet = {
    .Length     = 114,                                     /* Length = MACs + ETH_TYPE + 100 (PAYLOAD) */
    .Data_Packet = {  ETH2_MAC_5, ETH2_MAC_4, ETH2_MAC_3,  /* DST_MAC_UNI  */
                      ETH2_MAC_2, ETH2_MAC_1, ETH2_MAC_0,  
      
                      ETH1_MAC_5, ETH1_MAC_4, ETH1_MAC_3,  /* SRC_MAC_UNI  */
                      ETH1_MAC_2, ETH1_MAC_1, ETH1_MAC_0,  
    
                      0x00, 100                            /* PAYLOAD SIZE */}
};

__attribute__ ((aligned (4)))
ETH_TX_Packet ETH2_Send_Packet = {
    .Length     = 114,                                     /* Length = MACs + ETH_TYPE + 100 (PAYLOAD) */
    .Data_Packet = {  ETH1_MAC_5, ETH1_MAC_4, ETH1_MAC_3,  /* DST_MAC_UNI  */
                      ETH1_MAC_2, ETH1_MAC_1, ETH1_MAC_0,  

                      ETH2_MAC_5, ETH2_MAC_4, ETH2_MAC_3,  /* SRC_MAC_UNI  */
                      ETH2_MAC_2, ETH2_MAC_1, ETH2_MAC_0,   
    
                      0x00, 100                            /* PAYLOAD SIZE */}
};
uint8_t Receive_Packet[ETH_BUFF_LEN] __attribute__ ((aligned (4)));
ETH_StatusPacketReceptionTypeDef ETH_StatusPacket;
uint32_t i, ETH1_PacketCounterSend, ETH1_PacketCounterRecive, ETH2_PacketCounterSend, ETH2_PacketCounterRecive;


/* Private function prototypes -----------------------------------------------*/
void Delay(__IO uint32_t nCount);
void ClockConfigure(void);

/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */
int main(void)
{
	static ETH_InitTypeDef  ETH_InitStruct;
    
	/* Configure CPU clock */
	ClockConfigure();

	/* Enable peripheral clocks*/
	RST_CLK_PCLKcmd(RST_CLK_PCLK_PORTB, ENABLE);
	/* Reset PORTB settings */
	PORT_DeInit(MDR_PORTB);

	/* LEDs Port Init */
	/* Configure PORTB pins 2,3,4,5,6,7 */
    PORT_StructInit(&PORT_InitStructure);
    
	PORT_InitStructure.PORT_Pin = ( PORT_Pin_0 | PORT_Pin_1 | PORT_Pin_2 | PORT_Pin_3 | PORT_Pin_4 |
								    PORT_Pin_5 | PORT_Pin_6 | PORT_Pin_7 );
	PORT_InitStructure.PORT_MODE 	= PORT_MODE_DIGITAL;
	PORT_InitStructure.PORT_OE  	= PORT_OE_OUT;
	PORT_InitStructure.PORT_SPEED 	= PORT_SPEED_MAXFAST;
	PORT_InitStructure.PORT_FUNC 	= PORT_FUNC_PORT;
	PORT_InitStructure.PORT_PD 		= PORT_PD_DRIVER;
	PORT_Init(MDR_PORTB, &PORT_InitStructure);

	/* Reset ehernet clock settings */
	ETH_ClockDeInit();
    /* Enable the DMA clock for ETH_BUFFER_MODE_FIFO */
    RST_CLK_PCLKcmd(RST_CLK_PCLK_DMA, ENABLE);
    
	/* Enable HSE2 oscillator */
	RST_CLK_HSE2config(RST_CLK_HSE2_ON);
	if(RST_CLK_HSE2status() == ERROR)
		while(1); /* Infinite loop */

	/* Config PHY clock */
	ETH_PHY_ClockConfig(ETH_PHY_CLOCK_SOURCE_HSE2, ETH_PHY_HCLKdiv1);

	/* Init the BRG ETHERNET */
	ETH_BRGInit(ETH_HCLKdiv1);

	/* Enable the ETHERNET1 and 2 clock */
	ETH_ClockCMD(ETH_CLK1, ENABLE);
	ETH_ClockCMD(ETH_CLK2, ENABLE);

	/* Reset to default ethernet settings */
	ETH_DeInit(MDR_ETHERNET1);
	ETH_DeInit(MDR_ETHERNET2);

	/* Init ETH_InitStruct members with its default value */
	ETH_StructInit((ETH_InitTypeDef * ) &ETH_InitStruct);
	
	/* Set the speed of the chennel */
	ETH_InitStruct.ETH_PHY_Mode = ETH_PHY_MODE_AutoNegotiation;
	ETH_InitStruct.ETH_Pause = ENABLE;
	ETH_InitStruct.ETH_Transmitter_RST = SET;
	ETH_InitStruct.ETH_Receiver_RST = SET;

	/* Set the buffer mode */
	ETH_InitStruct.ETH_Buffer_Mode = ETH_BUFFER_MODE_AUTOMATIC_CHANGE_POINTERS;
	ETH_InitStruct.ETH_Source_Addr_HASH_Filter = DISABLE;
	ETH_InitStruct.ETH_Automatic_CRC_Strip = ENABLE;
	ETH_InitStruct.ETH_Automatic_IPG = ENABLE;
	ETH_InitStruct.ETH_Retry_Counter = 0x0F;

	/* Set the buffer size of transmitter and receiver */
	ETH_InitStruct.ETH_Dilimiter = 0x1000;

	/* Set MAC-address for the ETHERNET 1 */
    ETH_InitStruct.ETH_MAC_Address[2] = (ETH1_MAC_0<<8)|ETH1_MAC_1;
	ETH_InitStruct.ETH_MAC_Address[1] = (ETH1_MAC_2<<8)|ETH1_MAC_3;
	ETH_InitStruct.ETH_MAC_Address[0] = (ETH1_MAC_4<<8)|ETH1_MAC_5;
    
    /* Init the ETHERNET 1 */
	ETH_Init(MDR_ETHERNET1, (ETH_InitTypeDef *) &ETH_InitStruct);
	ETH_PHYCmd(MDR_ETHERNET1, ENABLE);
    
    /* Wait until the PHY unit did not come out in the state after a hard reset */
	while(ETH_GetPHYStatus(MDR_ETHERNET1, ETH_PHY_FLAG_READY) != SET){}
    
    /* Set MAC-address for the ETHERNET 2 */
    ETH_InitStruct.ETH_MAC_Address[2] = (ETH2_MAC_0<<8)|ETH2_MAC_1;
	ETH_InitStruct.ETH_MAC_Address[1] = (ETH2_MAC_2<<8)|ETH2_MAC_3;
	ETH_InitStruct.ETH_MAC_Address[0] = (ETH2_MAC_4<<8)|ETH2_MAC_5;

	/* Init the ETHERNET 2 */
	ETH_Init(MDR_ETHERNET2, (ETH_InitTypeDef *) &ETH_InitStruct);
	ETH_PHYCmd(MDR_ETHERNET2, ENABLE);
    
    /* Wait until the PHY unit did not come out in the state after a hard reset */
	while(ETH_GetPHYStatus(MDR_ETHERNET2, ETH_PHY_FLAG_READY) != SET){}

	/* Start ETHERNET 1 and 2 */
	ETH_Start(MDR_ETHERNET1);
	ETH_Start(MDR_ETHERNET2);
    
    /* Wait for autonegotiation to complete */    
    while( !(ETH_ReadPHYRegister(MDR_ETHERNET1, 0x1C, PHY_ECTR) & 1<<12) ){}

    /* Writing Payload to a Package */
    for(i = 14; i < 14+100; i++)  // Payload size = 100, Start Payload = 6(mac_dst) + 6(mac_src) + 2(eth_type) = 14
    {
    	ETH1_Send_Packet.Data_Packet[i] = ETH2_Send_Packet.Data_Packet[i] = i;
    }

	/* Infinite loop */
	while(1)
    {
#if defined (Send_ETH1)
		ETH_SendFrame(MDR_ETHERNET1, (uint32_t *) &ETH1_Send_Packet, ETH1_Send_Packet.Length);
        ETH1_PacketCounterSend++;
#endif
        
#if defined (Send_ETH2)
		ETH_SendFrame(MDR_ETHERNET2, (uint32_t *) &ETH2_Send_Packet, ETH2_Send_Packet.Length);
        ETH2_PacketCounterSend++;
#endif 
		
        while(MDR_ETHERNET2->ETH_R_Head != MDR_ETHERNET2->ETH_R_Tail)
        {
			ETH2_PacketCounterRecive++;
            ETH_StatusPacket.Status = ETH_ReceivedFrame(MDR_ETHERNET2, (uint32_t *) Receive_Packet);
            
            PORT_SetBits(MDR_PORTB, PORT_Pin_1);
			Delay(1000000);
			PORT_ResetBits(MDR_PORTB, PORT_Pin_1);	
			Delay(1000000);
		}
        
        while(MDR_ETHERNET1->ETH_R_Head != MDR_ETHERNET1->ETH_R_Tail)
        {
            ETH1_PacketCounterRecive++;
            ETH_StatusPacket.Status = ETH_ReceivedFrame(MDR_ETHERNET1, (uint32_t *) Receive_Packet);
			
            PORT_SetBits(MDR_PORTB, PORT_Pin_0);
			Delay(1000000);
			PORT_ResetBits(MDR_PORTB, PORT_Pin_0);
			Delay(1000000);
		}
	}
}

/**
  * @brief	Configure CPU clock
  * @param	None
  * @retval None
  */
void ClockConfigure ( void )
{
    /* Reset the clock system settings */
    RST_CLK_DeInit();
    
    /* Enable HSE (High Speed External) clock */
    RST_CLK_HSEconfig(RST_CLK_HSE_ON);
    if(RST_CLK_HSEstatus() != SUCCESS){
        while (1);
    }
    
    /* Configures the CPU_PLL clock source (CPU_CLK = HSE_CLK * 10 = 8 MHz * 10 = 80 MHz) */
    RST_CLK_CPU_PLLconfig(RST_CLK_CPU_PLLsrcHSEdiv1, RST_CLK_CPU_PLLmul10);
    /* Enables the CPU_PLL */
    RST_CLK_CPU_PLLcmd(ENABLE);
    if(RST_CLK_CPU_PLLstatus() == ERROR) {
        while (1);
    }
    
    /* Select the CPU_PLL output as input for CPU_C2_SEL */
    RST_CLK_CPU_PLLuse(ENABLE);
    /* Set CPUClk Prescaler (CPU_C3_SEL = CPU_C2) */
    RST_CLK_CPUclkPrescaler(RST_CLK_CPUclkDIV1);
    
    /* Enables the RST_CLK_PCLK_EEPROM */
    RST_CLK_PCLKcmd(RST_CLK_PCLK_EEPROM, ENABLE);
    /* Sets the code latency value (CPU_CLK up to 100 MHz) */
    EEPROM_SetLatency(EEPROM_Latency_3);
    /* Disables the RST_CLK_PCLK_EEPROM */
    RST_CLK_PCLKcmd(RST_CLK_PCLK_EEPROM, DISABLE);
    
    /* Enables the RST_CLK_PCLK_BKP */
    RST_CLK_PCLKcmd(RST_CLK_PCLK_BKP, ENABLE);
    /* Setting the parameters of the voltage regulator SelectRI and LOW in the BKP controller (CPU_CLK = 80 MHz) */
    BKP_DUccMode(BKP_DUcc_upto_80MHz);

    /* Select the CPU clock source */
    RST_CLK_CPUclkSelection(RST_CLK_CPUclkCPU_C3);	
}

/**
  * @brief  Executes delay loop
  * @param  nCount: specifies the delay
  * @retval None
  */
void Delay(__IO uint32_t nCount)
{
	for (; nCount != 0; nCount--);
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

/** @} */ /* End of group Ethernet_X2 */

/** @} */ /* End of group __MDR1986VE3_EVAL */

/** @} */ /* End of group __MDR32Fx_StdPeriph_Examples */

/******************* (C) COPYRIGHT 2021 Milandr *********/
