/**
  ******************************************************************************
  * @file    main.c
  * @author  Milandr Application Team
  * @version V1.0.0
  * @date    02/09/2015
  * @brief   This example shows how to configure PHY and MAC, transmit and receive some low level data
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
#include "MDR1986VE8T.h"
#include "MDR32F8_it.h"
#include "spec.h"

#include "mdr32f8_config.h"
#include "mdr32f8_port.h"
#include "mdr32f8_clkctrl.h"
#include "mdr32f8_eth.h"

/** @addtogroup MDR32F8_StdPeriph_Examples
  * @{
  */

/** @addtogroup MDR32F8_Board_For_MCU
  * @{
  */

/** @addtogroup eth_loopback eth_loopback
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/


#define MAX_ETH_TX_DATA_SIZE 1514 / 4
#define MAX_ETH_RX_DATA_SIZE 1514 / 4

#define FRAME_MAC_SIZE  12
#define FRAME_LEN_SIZE   2
#define FRAME_HEAD_SIZE   (FRAME_MAC_SIZE + FRAME_LEN_SIZE)

#define FRAME_SIZE 64


/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
PORT_InitTypeDef PORT_InitStructure;
ETH_InitTypeDef  ETH_InitStruct;
ETH_StatusPacketReceptionTypeDef StatusRX;

uint32_t tmp;

uint8_t MAC[] = {0xAB,0x78,0x56, 0x34, 0x12, 0x00};

uint8_t FrameTX [MAX_ETH_TX_DATA_SIZE];
uint32_t FrameRX [MAX_ETH_RX_DATA_SIZE];


uint8_t* ptrDataTX;
			
/* Private function prototypes -----------------------------------------------*/
void POR_disable(void);
void KEY_reg_accs(void);
void BlinkLine(uint32_t Pin);
void Ini_BUF(uint32_t addr,uint32_t size,uint32_t delta);
void TestETH(void);
/* Private functions ---------------------------------------------------------*/
uint8_t* Init_FrameTX(uint8_t *destMAC, uint8_t *srcMAC, uint16_t frameLen, uint16_t *payloadLen);
void FillFrameTXData(uint32_t frameIndex, uint32_t frameLength);
/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */
		
int main(void)
{	
	uint32_t ind=0;
    
    /* ONLY REV2 MCU, errata 0015. Disable Power-on-Reset control. Hold the SW4 button down until operation complete */
    //POR_disable();
    
    // Key to access clock control 
	UNLOCK_UNIT (CLK_CNTR_key);
	// Key to access fault control
    UNLOCK_UNIT (FT_CNTR_key); 
	// Key to access BKP control
	UNLOCK_UNIT (BKP_key); 
     
	/* Set CLKCTRL to default */
	CLKCTRL_DeInit();
	/* Enable HSE0 clock */
	CLKCTRL_HSEconfig(CLKCTRL_HSE0_CLK_ON);
    
    BKPCNTR_SRILOWconfig (BKPCNTR_SRILOW_upto_100MHz);
	
	/* Check HSE success, enable PLL0, check PLL0 success, select MAX_CLK src */
	while(CLKCTRL_HSEstatus(CLKCTRL_HSEn_STAT_HSE0_RDY) != SUCCESS){}
		
	/* Select PLL0 clk src, PLL0_N, PLL0_Q to get FINT = FIN*(PLLn_N)/(PLLn_Q+1) */
	CLKCTRL_CPU_PLLconfig(PLL0, CLKCTRL_PLLn_CLK_SELECT_HSE0div1, PLL_DIVQ_Q_1, 8);//PLLn, SRC, Q, N
		while(CLKCTRL_CPU_PLLstatus(0) != SUCCESS){}
			CLKCTRL_MAX_CLKSelection(CLKCTRL_MAX_CLK_PLL0);		

	CLKCTRL_CPUclkPrescaler (CLKCTRL_CPU_CLK_CPUclkDIV2);
		
	CLKCTRL_PER0_CLKcmd(CLKCTRL_PER0_CLK_MDR_PORTC_EN, ENABLE);
            
    UNLOCK_UNIT (PORTC_key);

	ETH_ClockDeInit();
	ETH_PHY_ClockConfig(ETH_PHY_CLOCK_SOURCE_PLL0, ETH_PHY_HCLKdiv1);
	
	/* Init ETH_InitStruct members with its default value */
	ETH_StructInit((ETH_InitTypeDef * ) &ETH_InitStruct);
			
	ETH_InitStruct.ETH_PHY_LB = DISABLE;
	ETH_InitStruct.ETH_PHY_DLB = DISABLE;
	ETH_InitStruct.ETH_PHY_HALFD = DISABLE;
	/*ETH_InitStruct.ETH_PHY_EARLY_DV = 0;*///RESERVED
	ETH_InitStruct.ETH_PHY_DIR = ETH_PHY_CONTROL_DIR_DIRECT;
	ETH_InitStruct.ETH_PHY_BASE_2 = ETH_PHY_CONTROL_BASE_2_TWISTED;
	ETH_InitStruct.ETH_PHY_LINK_PERIOD = 0x2;
	ETH_InitStruct.ETH_PHY_EXT_EN = ETH_PHY_CONTROL_EXT_EN_INTER;
	ETH_InitStruct.ETH_PHY_A_SELR = ETH_PHY_CONTROL_A_SELR_INTER;
	ETH_InitStruct.ETH_PHY_A_TRIM = 0x4;
	ETH_InitStruct.ETH_PHY_A_TRIMR = 0x80;
		
	ETH_InitStruct.ETH_Extension_Mode  = DISABLE;
	ETH_InitStruct.ETH_ColWnd 			= 0x5;
	ETH_InitStruct.ETH_HalfDuplex_Mode = DISABLE;
	ETH_InitStruct.ETH_Buffer_Mode 		 = ETH_BUFFER_MODE_AUTOMATIC_CHANGE_POINTERS;
	ETH_InitStruct.ETH_Register_CLR 	 = DISABLE;
	ETH_InitStruct.ETH_Loopback_Mode   = ENABLE;
	ETH_InitStruct.ETH_DBG_XF          = ENABLE;
	ETH_InitStruct.ETH_DBG_RF          = ENABLE;
    
    ETH_InitStruct.ETH_Dilimiter = 0x1000;
	
	ETH_InitStruct.ETH_Receive_All_Packets = ENABLE;
	ETH_InitStruct.ETH_Error_CRC_Frames_Reception = ENABLE;
	ETH_InitStruct.ETH_Receiver_Event_Mode = ETH_RECEIVER_EVENT_MODE_PACET_RECEIVED;
	ETH_InitStruct.ETH_Receiver_BE = ETH_RECEIVER_BE_LITTLE_ENDIAN;
		
	ETH_InitStruct.ETH_Transmitter_BE = ETH_RECEIVER_BE_LITTLE_ENDIAN;
	ETH_InitStruct.ETH_Transmitter_Event_Mode = ETH_TRANSMITTER_EVENT_MODE_XFIFO_ALMOST_EMPTY;
	ETH_InitStruct.ETH_Automatic_Pad_Strip = ENABLE;
	ETH_InitStruct.ETH_Automatic_Preamble = ENABLE;
	ETH_InitStruct.ETH_Automatic_CRC_Strip = ENABLE;
	ETH_InitStruct.ETH_Automatic_IPG = ENABLE;
	ETH_InitStruct.ETH_Retry_Counter = 0xF;	
		
	ETH_Init(MDR_ETH0, (ETH_InitTypeDef *) &ETH_InitStruct);
	
	ETH_PHY_Reset(MDR_ETH0);
	ETH_PHYCmd(MDR_ETH0, ENABLE);

	ETH_Start(MDR_ETH0);
    
    /*Main cycle*/
    while (1)
    {
    FillFrameTXData (ind, FRAME_SIZE);
    ind++;
    ETH_SendFrame (MDR_ETH0, (uint32_t*) FrameTX, FRAME_SIZE);
    
	while(MDR_ETH0->R_HEAD == MDR_ETH0->R_TAIL){}
        
    StatusRX.Status = ETH_ReceivedFrame (MDR_ETH0, FrameRX);
										
    }
}


void FillFrameTXData(uint32_t frameIndex, uint32_t frameLength)
{
  uint16_t dataCount;
  uint16_t i;
  
  // Запись управляющего слова, МАС и Length фрейма
  ptrDataTX =Init_FrameTX(MAC, MAC, frameLength, &dataCount);
  
  //  Запись индекса фрейма
  ptrDataTX[0] =  frameIndex & 0xFF;
  ptrDataTX[1] = (frameIndex >> 8)  & 0xFF;
  ptrDataTX[2] = (frameIndex >> 16) & 0xFF;
  ptrDataTX[3] = (frameIndex >> 24) & 0xFF;
  
  //  Запись данных для отправки
  for (i = 4; i < dataCount; ++i)
  {
    ptrDataTX[i] = i ;
  }
  // Маркер конца пакета
  ptrDataTX[dataCount - 2] = 0xFF;
  ptrDataTX[dataCount - 1] = frameLength;
}

uint8_t* Init_FrameTX(uint8_t *destMAC, uint8_t *srcMAC, uint16_t frameLen, uint16_t *payloadLen)
{
	uint8_t * ptr_TXFrame = (uint8_t *) &FrameTX[4];
  *payloadLen = frameLen - FRAME_HEAD_SIZE;
  
	//	Count To Send
	*(uint32_t *)&FrameTX[0] = frameLen;
	
	/* Set destanation MAC address */
	ptr_TXFrame[0] 	= destMAC[0];
	ptr_TXFrame[1] 	= destMAC[1];
	ptr_TXFrame[2] 	= destMAC[2];
	ptr_TXFrame[3] 	= destMAC[3];
	ptr_TXFrame[4] 	= destMAC[4];
	ptr_TXFrame[5] 	= destMAC[5];		
	
	/* Set our MAC address */
	ptr_TXFrame[6] 	= srcMAC[0];
	ptr_TXFrame[7] 	= srcMAC[1];
	ptr_TXFrame[8] 	= srcMAC[2];
	ptr_TXFrame[9] 	= srcMAC[3];
	ptr_TXFrame[10] = srcMAC[4];
	ptr_TXFrame[11] = srcMAC[5];	

  // Return dataCount  
	ptr_TXFrame[12] 	= (uint8_t)((*payloadLen) >> 8);
	ptr_TXFrame[13] 	= (uint8_t)((*payloadLen) & 0xFF);	

  // Return DataPtr
  return &ptr_TXFrame[FRAME_HEAD_SIZE];
}

//-----------------------------assert_param--------------------------------
#if (USE_ASSERT_INFO == 1)
void assert_failed(uint32_t file_id, uint32_t line)
{
  /* User can add his own implementation to report the source file ID and line number.
     Ex: printf("Wrong parameters value: file Id %d on line %d\r\n", file_id, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#elif (USE_ASSERT_INFO == 2)
void assert_failed(uint32_t file_id, uint32_t line, const uint8_t* expr)
{
  /* User can add his own implementation to report the source file ID, line number and
     expression text.
     Ex: printf("Wrong parameters value (%s): file Id %d on line %d\r\n", expr, file_id, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif /* USE_ASSERT_INFO */

/**
  * @}
  */ 	

/**
  * @}
  */ 	

/**
  * @}
  */ 	

/******************* (C) COPYRIGHT 2021 Milandr *********/

/* END OF FILE main.c */

