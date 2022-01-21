/**
  ******************************************************************************
  * File Name          : ethernetif.c
  * Description        : This file provides code for the configuration
  *                      of the ethernetif.c MiddleWare.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include <string.h>
#include "main.h"
#include "dp83848.h"
#include "CLI_io.h"
#include "driver_ui.h"
#include "ethernetif_test.h"

/* Private define ------------------------------------------------------------*/

/* ETH Setting  */
#define ETH_DMA_TRANSMIT_TIMEOUT               ( 20U )
/* ETH_RX_BUFFER_SIZE parameter is defined in lwipopts.h */

/* Private variables ---------------------------------------------------------*/

extern ETH_DMADescTypeDef DMARxDscrTab[ETH_RX_DESC_CNT]; /* Ethernet Rx DMA Descriptors */
extern ETH_DMADescTypeDef DMATxDscrTab[ETH_TX_DESC_CNT];   /* Ethernet Tx DMA Descriptors */
extern uint8_t Rx_Buff[ETH_RX_DESC_CNT][ETH_MAX_PACKET_SIZE]; /* Ethernet Receive Buffers */

/* Global Ethernet handle */
extern ETH_HandleTypeDef heth;
extern ETH_TxPacketConfig TxConfig;

/* Private function prototypes -----------------------------------------------*/
int32_t ETH_PHY_IO_Init(void);
int32_t ETH_PHY_IO_DeInit (void);
int32_t ETH_PHY_IO_ReadReg(uint32_t DevAddr, uint32_t RegAddr, uint32_t *pRegVal);
int32_t ETH_PHY_IO_WriteReg(uint32_t DevAddr, uint32_t RegAddr, uint32_t RegVal);
int32_t ETH_PHY_IO_GetTick(void);

DP83848_Object_t DP83848;
const DP83848_IOCtx_t  DP83848_IOCtx = {ETH_PHY_IO_Init,
                                  ETH_PHY_IO_DeInit,
                                  ETH_PHY_IO_WriteReg,
                                  ETH_PHY_IO_ReadReg,
                                  ETH_PHY_IO_GetTick};

uint32_t Cur_PHYLinkState;

/* Private functions ---------------------------------------------------------*/

/*******************************************************************************
                       LL Driver Interface ( LwIP stack --> ETH)
*******************************************************************************/
/**
 * @brief In this function, the hardware should be initialized.
 * Called from ethernetif_init().
 *
 * @param netif the already initialized lwip network interface structure
 *        for this ethernetif
 */
void low_level_init()
{
  uint32_t idx = 0;
  /* Start ETH HAL Init */

  for(idx = 0; idx < ETH_RX_DESC_CNT; idx ++)
  {
    HAL_ETH_DescAssignMemory(&heth, idx, Rx_Buff[idx], NULL);
  }

  /* Set PHY IO functions */
  DP83848_RegisterBusIO(&DP83848, (DP83848_IOCtx_t *)&DP83848_IOCtx);

  /* Initialize the DP83848 ETH PHY */
  DP83848_Init(&DP83848);

  Cur_PHYLinkState = DP83848_STATUS_LINK_DOWN;
  /* Get link state */
  ethernet_link_check_state();
}

/**
 * This function should do the actual transmission of the packet. The packet is
 * contained in the pbuf that is passed to the function. This pbuf
 * might be chained.
 *
 * @param netif the lwip network interface structure for this ethernetif
 * @param p the MAC packet to send (e.g. IP packet including MAC addresses and type)
 * @return ERR_OK if the packet could be sent
 *         an err_t value if the packet couldn't be sent
 *
 * @note Returning ERR_MEM here if a DMA queue of your MAC is full can lead to
 *       strange results. You might consider waiting for space in the DMA queue
 *       to become available since the stack doesn't retry to send a packet
 *       dropped because of memory failure (except for the TCP timers).
 */

void low_level_output(uint8_t *p, uint16_t len)
{
  ETH_BufferTypeDef Txbuffer[ETH_TX_DESC_CNT];

  memset(Txbuffer, 0 , ETH_TX_DESC_CNT*sizeof(ETH_BufferTypeDef));

  Txbuffer[0].buffer = p;
  Txbuffer[0].len = len;
  Txbuffer[0].next = NULL;

  TxConfig.Length =  len;
  TxConfig.TxBuffer = Txbuffer;

  if (HAL_ETH_Transmit(&heth, &TxConfig, ETH_DMA_TRANSMIT_TIMEOUT)!=HAL_OK)
	 CLI_print("ETH TX ERR") ;
}

/**
 * Should allocate a pbuf and transfer the bytes of the incoming
 * packet from the interface into the pbuf.
 *
 * @param netif the lwip network interface structure for this ethernetif
 * @return a pbuf filled with the received packet (including MAC header)
 *         NULL on memory error
   */
uint16_t low_level_input(ETH_BufferTypeDef *RxBuff)
{
  uint32_t framelength = 0, i = 0;

  memset(RxBuff, 0 , ETH_RX_DESC_CNT*sizeof(ETH_BufferTypeDef));

  for(i = 0; i < ETH_RX_DESC_CNT -1; i++)
  {
    RxBuff[i].next=&RxBuff[i+1];
  }

  if (HAL_ETH_IsRxDataAvailable(&heth))
  {
    HAL_ETH_GetRxDataBuffer(&heth, RxBuff);
    HAL_ETH_GetRxDataLength(&heth, &framelength);

    /* Build Rx descriptor to be ready for next data reception */
    HAL_ETH_BuildRxDescriptors(&heth);

#if !defined(DUAL_CORE) || defined(CORE_CM7)
    /* Invalidate data cache for ETH Rx Buffers */
    SCB_InvalidateDCache_by_Addr((uint32_t *)RxBuff->buffer, framelength);
#endif

    return framelength;
  }
  else
  {
    return 0;
  }
}

/**
 * This function should be called when a packet is ready to be read
 * from the interface. It uses the function low_level_input() that
 * should handle the actual reception of bytes from the network
 * interface. Then the type of the received packet is determined and
 * the appropriate input function is called.
 *
 * @param netif the lwip network interface structure for this ethernetif
 */
uint16_t ethernetif_input(ETH_BufferTypeDef *RxBuff)
{
  /* move received packet into a new pbuf */
  return low_level_input(RxBuff);
}

/**
 * Should be called at the beginning of the program to set up the
 * network interface. It calls the function low_level_init() to do the
 * actual setup of the hardware.
 *
 * This function should be passed as a parameter to netif_add().
 *
 * @param netif the lwip network interface structure for this ethernetif
 * @return ERR_OK if the loopif is initialized
 *         ERR_MEM if private data couldn't be allocated
 *         any other err_t on error
 */
void ethernetif_init()
{
  /* initialize the hardware */
  low_level_init();
}

/*******************************************************************************
                       PHI IO Functions
*******************************************************************************/
/**
  * @brief  Initializes the MDIO interface GPIO and clocks.
  * @param  None
  * @retval 0 if OK, -1 if ERROR
  */
int32_t ETH_PHY_IO_Init(void)
{
  /* We assume that MDIO GPIO configuration is already done
     in the ETH_MspInit() else it should be done here
  */

  /* Configure the MDIO Clock */
  HAL_ETH_SetMDIOClockRange(&heth);

  return 0;
}

/**
  * @brief  De-Initializes the MDIO interface .
  * @param  None
  * @retval 0 if OK, -1 if ERROR
  */
int32_t ETH_PHY_IO_DeInit (void)
{
  return 0;
}

/**
  * @brief  Read a PHY register through the MDIO interface.
  * @param  DevAddr: PHY port address
  * @param  RegAddr: PHY register address
  * @param  pRegVal: pointer to hold the register value
  * @retval 0 if OK -1 if Error
  */
int32_t ETH_PHY_IO_ReadReg(uint32_t DevAddr, uint32_t RegAddr, uint32_t *pRegVal)
{
  if(HAL_ETH_ReadPHYRegister(&heth, DevAddr, RegAddr, pRegVal) != HAL_OK)
  {
    return -1;
  }

  return 0;
}

/**
  * @brief  Write a value to a PHY register through the MDIO interface.
  * @param  DevAddr: PHY port address
  * @param  RegAddr: PHY register address
  * @param  RegVal: Value to be written
  * @retval 0 if OK -1 if Error
  */
int32_t ETH_PHY_IO_WriteReg(uint32_t DevAddr, uint32_t RegAddr, uint32_t RegVal)
{
  if(HAL_ETH_WritePHYRegister(&heth, DevAddr, RegAddr, RegVal) != HAL_OK)
  {
    return -1;
  }

  return 0;
}

/**
  * @brief  Get the time in millisecons used for internal PHY driver process.
  * @retval Time value
  */
int32_t ETH_PHY_IO_GetTick(void)
{
  return HAL_GetTick();
}

/**
  * @brief  Check the ETH link state then update ETH driver and netif link accordingly.
  * @param  argument: netif
  * @retval None
  */
void ethernet_link_check_state()
{
  ETH_MACConfigTypeDef MACConf;
  int32_t PHYLinkState;
  uint32_t linkchanged = 0, speed = 0, duplex =0;

  PHYLinkState = DP83848_GetLinkState(&DP83848);

  if (Cur_PHYLinkState==PHYLinkState) return;
  Cur_PHYLinkState=PHYLinkState;

  if((PHYLinkState <= DP83848_STATUS_LINK_DOWN))
  {
    HAL_ETH_Stop(&heth);
    ui_setledstate(LED_NORMA, LED_STATE_FL3);
  }
  else if((PHYLinkState > DP83848_STATUS_LINK_DOWN))
  {
    switch (PHYLinkState)
    {
    case DP83848_STATUS_100MBITS_FULLDUPLEX:
      duplex = ETH_FULLDUPLEX_MODE;
      speed = ETH_SPEED_100M;
      linkchanged = 1;
      break;
    case DP83848_STATUS_100MBITS_HALFDUPLEX:
      duplex = ETH_HALFDUPLEX_MODE;
      speed = ETH_SPEED_100M;
      linkchanged = 1;
      break;
    case DP83848_STATUS_10MBITS_FULLDUPLEX:
      duplex = ETH_FULLDUPLEX_MODE;
      speed = ETH_SPEED_10M;
      linkchanged = 1;
      break;
    case DP83848_STATUS_10MBITS_HALFDUPLEX:
      duplex = ETH_HALFDUPLEX_MODE;
      speed = ETH_SPEED_10M;
      linkchanged = 1;
      break;
    default:
      break;
    }

    if(linkchanged)
    {
      /* Get MAC Config MAC */
      HAL_ETH_GetMACConfig(&heth, &MACConf);
      MACConf.DuplexMode = duplex;
      MACConf.Speed = speed;
      HAL_ETH_SetMACConfig(&heth, &MACConf);

      HAL_ETH_Start(&heth);
      if (get_pdo_work_mode()==WORK_MODE_OK)
        ui_setledstate(LED_NORMA, LED_STATE_ON);
      else
        ui_setledstate(LED_NORMA, LED_STATE_FL1);
    }
  }

}
/* USER CODE BEGIN 8 */

/* USER CODE END 8 */
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

