/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : ethernetif.c
  * Description        : This file provides code for the configuration
  *                      of the ethernetif.c MiddleWare.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
//#include "rs232.h"

/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "lwip/opt.h"
#include "lwip/timeouts.h"
#include "netif/ethernet.h"
#include "netif/etharp.h"
#include "lwip/ethip6.h"
#include "ethernetif.h"
//#include "lan8742.h"
#include <string.h>
#include "cmsis_os.h"
#include "lwip/tcpip.h"
#include "rs232_printf.h"

/* Within 'USER CODE' section, code will be kept by default at each generation */
/* USER CODE BEGIN 0 */
extern uint8_t macAdr5;
#include "dp83848.h"

/* USER CODE END 0 */

/* Private define ------------------------------------------------------------*/
/* The time to block waiting for input. */
#define TIME_WAITING_FOR_INPUT ( portMAX_DELAY )
/* USER CODE BEGIN OS_THREAD_STACK_SIZE_WITH_RTOS */
/* Stack size of the interface thread */
#define INTERFACE_THREAD_STACK_SIZE ( 350 )
/* USER CODE END OS_THREAD_STACK_SIZE_WITH_RTOS */
/* Network interface name */
#define IFNAME0 's'
#define IFNAME1 't'

/* ETH Setting  */
#define ETH_DMA_TRANSMIT_TIMEOUT               ( 20U )
/* ETH_RX_BUFFER_SIZE parameter is defined in lwipopts.h */

/* USER CODE BEGIN 1 */

/* USER CODE END 1 */

/* Private variables ---------------------------------------------------------*/
/*
@Note: This interface is implemented to operate in zero-copy mode only:
        - Rx buffers are allocated statically and passed directly to the LwIP stack
          they will return back to ETH DMA after been processed by the stack.
        - Tx Buffers will be allocated from LwIP stack memory heap,
          then passed to ETH HAL driver.

@Notes:
  1.a. ETH DMA Rx descriptors must be contiguous, the default count is 4,
       to customize it please redefine ETH_RX_DESC_CNT in ETH GUI (Rx Descriptor Length)
       so that updated value will be generated in stm32xxxx_hal_conf.h
  1.b. ETH DMA Tx descriptors must be contiguous, the default count is 4,
       to customize it please redefine ETH_TX_DESC_CNT in ETH GUI (Tx Descriptor Length)
       so that updated value will be generated in stm32xxxx_hal_conf.h

  2.a. Rx Buffers number must be between ETH_RX_DESC_CNT and 2*ETH_RX_DESC_CNT
  2.b. Rx Buffers must have the same size: ETH_RX_BUFFER_SIZE, this value must
       passed to ETH DMA in the init field (heth.Init.RxBuffLen)
  2.c  The RX Ruffers addresses and sizes must be properly defined to be aligned
       to L1-CACHE line size (32 bytes).
*/

#if defined ( __ICCARM__ ) /*!< IAR Compiler */

#pragma location=0x30000000
ETH_DMADescTypeDef  DMARxDscrTab[ETH_RX_DESC_CNT]; /* Ethernet Rx DMA Descriptors */
#pragma location=0x30000060
ETH_DMADescTypeDef  DMATxDscrTab[ETH_TX_DESC_CNT]; /* Ethernet Tx DMA Descriptors */
#pragma location=0x30000200
uint8_t Rx_Buff[ETH_RX_DESC_CNT][ETH_RX_BUFFER_SIZE]; /* Ethernet Receive Buffers */

#elif defined ( __CC_ARM )  /* MDK ARM Compiler */

__attribute__((at(0x30000000))) ETH_DMADescTypeDef  DMARxDscrTab[ETH_RX_DESC_CNT]; /* Ethernet Rx DMA Descriptors */
__attribute__((at(0x30000060))) ETH_DMADescTypeDef  DMATxDscrTab[ETH_TX_DESC_CNT]; /* Ethernet Tx DMA Descriptors */
__attribute__((at(0x30000200))) uint8_t Rx_Buff[ETH_RX_DESC_CNT][ETH_RX_BUFFER_SIZE]; /* Ethernet Receive Buffer */

#elif defined ( __GNUC__ ) /* GNU Compiler */

ETH_DMADescTypeDef DMARxDscrTab[ETH_RX_DESC_CNT] __attribute__((section(".RxDecripSection"))); /* Ethernet Rx DMA Descriptors */
ETH_DMADescTypeDef DMATxDscrTab[ETH_TX_DESC_CNT] __attribute__((section(".TxDecripSection")));   /* Ethernet Tx DMA Descriptors */
uint8_t Rx_Buff[ETH_RX_DESC_CNT][ETH_RX_BUFFER_SIZE] __attribute__((section(".RxArraySection"))); /* Ethernet Receive Buffers */

#endif

/* USER CODE BEGIN 2 */
//uint8_t Tx_Buff[ETH_TX_DESC_CNT][ETH_RX_BUFFER_SIZE] __attribute__((section(".TxArraySection"))); /* 4 Tx buffers of size ETH_TX_BUF_SIZE  */
/* USER CODE END 2 */

osSemaphoreId RxPktSemaphore = NULL; /* Semaphore to signal incoming packets */

/* Global Ethernet handle */
ETH_HandleTypeDef heth;
ETH_TxPacketConfig TxConfig;

/* Memory Pool Declaration */
LWIP_MEMPOOL_DECLARE(RX_POOL, 10, sizeof(struct pbuf_custom), "Zero-copy RX PBUF pool");

/* Private function prototypes -----------------------------------------------*/
int32_t ETH_PHY_IO_Init(void);
int32_t ETH_PHY_IO_DeInit (void);
int32_t ETH_PHY_IO_ReadReg(uint32_t DevAddr, uint32_t RegAddr, uint32_t *pRegVal);
int32_t ETH_PHY_IO_WriteReg(uint32_t DevAddr, uint32_t RegAddr, uint32_t RegVal);
int32_t ETH_PHY_IO_GetTick(void);

//lan8742_Object_t LAN8742;
//lan8742_IOCtx_t  LAN8742_IOCtx = {ETH_PHY_IO_Init,
//                                  ETH_PHY_IO_DeInit,
//                                  ETH_PHY_IO_WriteReg,
//                                  ETH_PHY_IO_ReadReg,
//                                  ETH_PHY_IO_GetTick};

/* USER CODE BEGIN 3 */
DP83848_Object_t DP83848;
DP83848_IOCtx_t  DP83848_IOCtx = {ETH_PHY_IO_Init,
                                  ETH_PHY_IO_DeInit,
                                  ETH_PHY_IO_WriteReg,
                                  ETH_PHY_IO_ReadReg,
                                  ETH_PHY_IO_GetTick};

void printReg_DP83848(uint16_t reg, const char* name)
{
    uint32_t regvalue;
    DP83848.IO.ReadReg(DP83848.DevAddr, reg, &regvalue);
}

void printAll_Regs_DP83848(){
    printReg_DP83848(DP83848_BMCR, "DP83848_BMCR");
    printReg_DP83848(DP83848_BMSR, "DP83848_BMSR");
    printReg_DP83848(DP83848_PHYIDR1, "DP83848_PHYIDR1");
    printReg_DP83848(DP83848_PHYIDR2, "DP83848_PHYIDR2");
    printReg_DP83848(DP83848_ANAR, "DP83848_ANAR");
    printReg_DP83848(DP83848_ANLPAR, "DP83848_ANLPAR");
    printReg_DP83848(DP83848_ANER, "DP83848_ANER");
    printReg_DP83848(DP83848_ANNPTR, "DP83848_ANNPTR");
    printReg_DP83848(DP83848_PHYSTS, "DP83848_PHYSTS");
    printReg_DP83848(DP83848_MICR, "DP83848_MICR");
    printReg_DP83848(DP83848_MISR, "DP83848_MISR");
    printReg_DP83848(DP83848_FCSCR, "DP83848_FCSCR");
    printReg_DP83848(DP83848_RECR, "DP83848_RECR");
    printReg_DP83848(DP83848_PCSR, "DP83848_PCSR");
    printReg_DP83848(DP83848_RBR, "DP83848_RBR");
    printReg_DP83848(DP83848_LEDCR, "DP83848_LEDCR");
    printReg_DP83848(DP83848_PHYCR, "DP83848_PHYCR");
    printReg_DP83848(DP83848_10BTSCR, "DP83848_10BTSCR");
    printReg_DP83848(DP83848_CDCTRL1, "DP83848_CDCTRL1");
    printReg_DP83848(DP83848_EDCR, "DP83848_EDCR");
}

/* USER CODE END 3 */

/* Private functions ---------------------------------------------------------*/
void pbuf_free_custom(struct pbuf *p);
void Error_Handler(void);

void HAL_ETH_MspInit(ETH_HandleTypeDef* ethHandle)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(ethHandle->Instance==ETH)
  {
  /* USER CODE BEGIN ETH_MspInit 0 */

  /* USER CODE END ETH_MspInit 0 */
    /* Enable Peripheral clock */
    __HAL_RCC_ETH1MAC_CLK_ENABLE();
    __HAL_RCC_ETH1TX_CLK_ENABLE();
    __HAL_RCC_ETH1RX_CLK_ENABLE();

    __HAL_RCC_GPIOC_CLK_ENABLE();
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOH_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
    /**ETH GPIO Configuration
    PC1     ------> ETH_MDC
    PC2_C     ------> ETH_TXD2
    PC3_C     ------> ETH_TX_CLK
    PA0     ------> ETH_CRS
    PA1     ------> ETH_RX_CLK
    PA2     ------> ETH_MDIO
    PH3     ------> ETH_COL
    PA7     ------> ETH_RX_DV
    PC4     ------> ETH_RXD0
    PC5     ------> ETH_RXD1
    PB0     ------> ETH_RXD2
    PB1     ------> ETH_RXD3
    PB10     ------> ETH_RX_ER
    PB11     ------> ETH_TX_EN
    PB12     ------> ETH_TXD0
    PB13     ------> ETH_TXD1
    PB8     ------> ETH_TXD3
    */
    GPIO_InitStruct.Pin = MII_MOC_Pin|MII_TXD2_Pin|MII_TX_CLK_Pin|MII_RXD0_Pin
                          |MII_RXD1_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF11_ETH;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = MII_CRS_Pin|MII_RX_CLK_Pin|MII_MDIO_Pin|MII_RX_DV_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF11_ETH;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = MII_COL_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF11_ETH;
    HAL_GPIO_Init(MII_COL_GPIO_Port, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = MII_RXD2_Pin|MII_RXD3_Pin|MII_RX_ER_Pin|MII_TX_EN_Pin
                          |MII_TXD0_Pin|MII_TXD1_Pin|MII_TXD3_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF11_ETH;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    /* Peripheral interrupt init */
    HAL_NVIC_SetPriority(ETH_IRQn, 5, 0);
    HAL_NVIC_EnableIRQ(ETH_IRQn);
//    HAL_NVIC_SetPriority(ETH_WKUP_IRQn, 1, 0);
//    HAL_NVIC_EnableIRQ(ETH_WKUP_IRQn);
  /* USER CODE BEGIN ETH_MspInit 1 */

  /* USER CODE END ETH_MspInit 1 */
  }
}

void HAL_ETH_MspDeInit(ETH_HandleTypeDef* ethHandle)
{
  if(ethHandle->Instance==ETH)
  {
  /* USER CODE BEGIN ETH_MspDeInit 0 */

  /* USER CODE END ETH_MspDeInit 0 */
    /* Disable Peripheral clock */
    __HAL_RCC_ETH1MAC_CLK_DISABLE();
    __HAL_RCC_ETH1TX_CLK_DISABLE();
    __HAL_RCC_ETH1RX_CLK_DISABLE();

    /**ETH GPIO Configuration
    PC1     ------> ETH_MDC
    PC2_C     ------> ETH_TXD2
    PC3_C     ------> ETH_TX_CLK
    PA0     ------> ETH_CRS
    PA1     ------> ETH_RX_CLK
    PA2     ------> ETH_MDIO
    PH3     ------> ETH_COL
    PA7     ------> ETH_RX_DV
    PC4     ------> ETH_RXD0
    PC5     ------> ETH_RXD1
    PB0     ------> ETH_RXD2
    PB1     ------> ETH_RXD3
    PB10     ------> ETH_RX_ER
    PB11     ------> ETH_TX_EN
    PB12     ------> ETH_TXD0
    PB13     ------> ETH_TXD1
    PB8     ------> ETH_TXD3
    */
    HAL_GPIO_DeInit(GPIOC, MII_MOC_Pin|MII_TXD2_Pin|MII_TX_CLK_Pin|MII_RXD0_Pin
                          |MII_RXD1_Pin);

    HAL_GPIO_DeInit(GPIOA, MII_CRS_Pin|MII_RX_CLK_Pin|MII_MDIO_Pin|MII_RX_DV_Pin);

    HAL_GPIO_DeInit(MII_COL_GPIO_Port, MII_COL_Pin);

    HAL_GPIO_DeInit(GPIOB, MII_RXD2_Pin|MII_RXD3_Pin|MII_RX_ER_Pin|MII_TX_EN_Pin
                          |MII_TXD0_Pin|MII_TXD1_Pin|MII_TXD3_Pin);

    /* Peripheral interrupt Deinit*/
    HAL_NVIC_DisableIRQ(ETH_IRQn);

  /* USER CODE BEGIN ETH_MspDeInit 1 */

  /* USER CODE END ETH_MspDeInit 1 */
  }
}

/**
  * @brief  Ethernet Rx Transfer completed callback
  * @param  heth: ETH handle
  * @retval None
  */
void HAL_ETH_RxCpltCallback(ETH_HandleTypeDef *heth)
{

//RS232Puts("!!!! HAL_ETH_RxCpltCallback !!!!\r\n");

  osSemaphoreRelease(RxPktSemaphore);
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

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
static void low_level_init(struct netif *netif)
{

//RS232Puts("--low_level_init--\r\n");

  HAL_StatusTypeDef hal_eth_init_status = HAL_OK;
  uint32_t idx = 0;
  ETH_MACConfigTypeDef MACConf;
  int32_t PHYLinkState;
  uint32_t duplex, speed = 0;
  /* Start ETH HAL Init */

   uint8_t MACAddr[6] ;
  heth.Instance = ETH;
  MACAddr[0] = 0x00;
  MACAddr[1] = 0x80;
  MACAddr[2] = 0xE1;
  MACAddr[3] = 0x00;
  MACAddr[4] = 0x00;
  MACAddr[5] = macAdr5;
  heth.Init.MACAddr = &MACAddr[0];
  heth.Init.MediaInterface = HAL_ETH_MII_MODE;
  heth.Init.TxDesc = DMATxDscrTab;
  heth.Init.RxDesc = DMARxDscrTab;
  heth.Init.RxBuffLen = 1524;

  hal_eth_init_status = HAL_ETH_Init(&heth);

  memset(&TxConfig, 0 , sizeof(ETH_TxPacketConfig));
  TxConfig.Attributes = ETH_TX_PACKETS_FEATURES_CSUM | ETH_TX_PACKETS_FEATURES_CRCPAD;
  TxConfig.ChecksumCtrl = ETH_CHECKSUM_IPHDR_PAYLOAD_INSERT_PHDR_CALC;
  TxConfig.CRCPadCtrl = ETH_CRC_PAD_INSERT;

  /* End ETH HAL Init */

  /* Initialize the RX POOL */
  LWIP_MEMPOOL_INIT(RX_POOL);

#if LWIP_ARP || LWIP_ETHERNET

  /* set MAC hardware address length */
  netif->hwaddr_len = ETH_HWADDR_LEN;

  /* set MAC hardware address */
  netif->hwaddr[0] =  heth.Init.MACAddr[0];
  netif->hwaddr[1] =  heth.Init.MACAddr[1];
  netif->hwaddr[2] =  heth.Init.MACAddr[2];
  netif->hwaddr[3] =  heth.Init.MACAddr[3];
  netif->hwaddr[4] =  heth.Init.MACAddr[4];
  netif->hwaddr[5] =  heth.Init.MACAddr[5];

  /* maximum transfer unit */
  netif->mtu = ETH_MAX_PAYLOAD;

  /* Accept broadcast address and ARP traffic */
  /* don't set NETIF_FLAG_ETHARP if this device is not an ethernet one */
  #if LWIP_ARP
    netif->flags |= NETIF_FLAG_BROADCAST | NETIF_FLAG_ETHARP
//            | NETIF_FLAG_LINK_UP
            | NETIF_FLAG_IGMP  //?????? ?????????? ?????????? ???? ???????????????????????????? ?? ???????????????????? ????????????
            ;
  #else
    netif->flags |= NETIF_FLAG_BROADCAST;
  #endif /* LWIP_ARP */

  for(idx = 0; idx < ETH_RX_DESC_CNT; idx ++)
  {
    HAL_ETH_DescAssignMemory(&heth, idx, Rx_Buff[idx], NULL);
//    HAL_ETH_DescAssignMemory(&heth, idx, Rx_Buff[idx], Tx_Buff[idx]);
  }

  /* create a binary semaphore used for informing ethernetif of frame reception */
  osSemaphoreDef(SEM);
  RxPktSemaphore = osSemaphoreCreate(osSemaphore(SEM) , 1 );

  /* create the task that handles the ETH_MAC */
/* USER CODE BEGIN OS_THREAD_DEF_CREATE_CMSIS_RTOS_V1 */
  osThreadDef(EthIf_____, ethernetif_input, osPriorityRealtime, 0, INTERFACE_THREAD_STACK_SIZE );
  osThreadCreate (osThread(EthIf_____), netif);
/* USER CODE END OS_THREAD_DEF_CREATE_CMSIS_RTOS_V1 */
/* USER CODE BEGIN PHY_PRE_CONFIG */

  ETH_MACFilterConfigTypeDef FilterConfig;

  FilterConfig.PromiscuousMode = 1;
  FilterConfig.PassAllMulticast = 1;

  HAL_ETH_SetMACFilterConfig(&heth, &FilterConfig);

/* USER CODE END PHY_PRE_CONFIG */

#endif /* LWIP_ARP || LWIP_ETHERNET */

/* USER CODE BEGIN LOW_LEVEL_INIT */

  DP83848_RegisterBusIO(&DP83848, &DP83848_IOCtx);
  DP83848_Init(&DP83848);

  if (hal_eth_init_status == HAL_OK)
  {
    PHYLinkState = DP83848_GetLinkState(&DP83848);

    /* Get link state */
    if(PHYLinkState <= DP83848_STATUS_LINK_DOWN)
    {
      netif_set_link_down(netif);
      netif_set_down(netif);
    }
    else
    {
      switch (PHYLinkState)
      {
      case DP83848_STATUS_100MBITS_FULLDUPLEX:
//RS232_write_c("\n\r--DP83848_STATUS_100MBITS_FULLDUPLEX--\n\r", sizeof ("\n\r--DP83848_STATUS_100MBITS_FULLDUPLEX--\n\r"));
        duplex = ETH_FULLDUPLEX_MODE;
        speed = ETH_SPEED_100M;
        break;
      case DP83848_STATUS_100MBITS_HALFDUPLEX:
//RS232_write_c("\n\r--DP83848_STATUS_100MBITS_HALFDUPLEX--\n\r", sizeof ("\n\r--DP83848_STATUS_100MBITS_HALFDUPLEX--\n\r"));
        duplex = ETH_HALFDUPLEX_MODE;
        speed = ETH_SPEED_100M;
        break;
      case DP83848_STATUS_10MBITS_FULLDUPLEX:
//RS232_write_c("\n\r--DP83848_STATUS_10MBITS_FULLDUPLEX--\n\r", sizeof ("\n\r--DP83848_STATUS_10MBITS_FULLDUPLEX--\n\r"));
        duplex = ETH_FULLDUPLEX_MODE;
        speed = ETH_SPEED_10M;
        break;
      case DP83848_STATUS_10MBITS_HALFDUPLEX:
//RS232_write_c("\n\r--DP83848_STATUS_10MBITS_HALFDUPLEX--\n\r", sizeof ("\n\r--DP83848_STATUS_10MBITS_HALFDUPLEX--\n\r"));
        duplex = ETH_HALFDUPLEX_MODE;
        speed = ETH_SPEED_10M;
        break;
      default:
//RS232_write_c("\n\r--ETH_FULLDUPLEX_MODE--\n\r", sizeof ("\n\r--ETH_FULLDUPLEX_MODE--\n\r"));
        duplex = ETH_FULLDUPLEX_MODE;
        speed = ETH_SPEED_100M;
        break;
      }

    /* Get MAC Config MAC */
    HAL_ETH_GetMACConfig(&heth, &MACConf);
    MACConf.DuplexMode = duplex;
    MACConf.Speed = speed;
    HAL_ETH_SetMACConfig(&heth, &MACConf);

    HAL_StatusTypeDef stat = HAL_ETH_Start_IT(&heth);
//    HAL_StatusTypeDef stat = HAL_ETH_Start(&heth);

    char msgUart7[50];
    memset(msgUart7,' ',50);
    sprintf(msgUart7,"%X%s", (uint8_t)stat,"\r\n");
//    RS232_write_c("\rHAL_ETH_Start_IT_status = ", sizeof ("\rHAL_ETH_Start_IT_status = "));
//    RS232_write_c(msgUart7, sizeof (msgUart7));

    netif_set_up(netif);
    netif_set_link_up(netif);

    }

  }
  else
  {
      RS232Puts("Error_Handler");
    Error_Handler();
  }
/* USER CODE END LOW_LEVEL_INIT */
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
//ETH_BufferTypeDef Txbuffer[ETH_TX_DESC_CNT]__attribute__((section(".TxArraySection")));
static err_t low_level_output(struct netif *netif, struct pbuf *p)
{
//RS232Puts("low_level_output\r\n");
  uint32_t i=0;
  struct pbuf *q;
  err_t errval = ERR_OK;
  ETH_BufferTypeDef Txbuffer[ETH_TX_DESC_CNT];

  memset(Txbuffer, 0 , ETH_TX_DESC_CNT*sizeof(ETH_BufferTypeDef));

  for(q = p; q != NULL; q = q->next)
  {
    if(i >= ETH_TX_DESC_CNT)
      return ERR_IF;

    Txbuffer[i].buffer = q->payload;
    Txbuffer[i].len = q->len;

    if(i>0)
    {
      Txbuffer[i-1].next = &Txbuffer[i];
    }

    if(q->next == NULL)
    {
      Txbuffer[i].next = NULL;
    }

    i++;
  }

  TxConfig.Length =  p->tot_len;
  TxConfig.TxBuffer = Txbuffer;

  HAL_ETH_Transmit(&heth, &TxConfig, ETH_DMA_TRANSMIT_TIMEOUT);
  return errval;
}

/**
 * Should allocate a pbuf and transfer the bytes of the incoming
 * packet from the interface into the pbuf.
 *
 * @param netif the lwip network interface structure for this ethernetif
 * @return a pbuf filled with the received packet (including MAC header)
 *         NULL on memory error
   */
static struct pbuf * low_level_input(struct netif *netif)
{
//RS232Puts ("** low_level_input **\r\n");
  struct pbuf *p = NULL;
  ETH_BufferTypeDef RxBuff[ETH_RX_DESC_CNT];
  uint32_t framelength = 0, i = 0;
  struct pbuf_custom* custom_pbuf;

  memset(RxBuff, 0 , ETH_RX_DESC_CNT*sizeof(ETH_BufferTypeDef));

  for(i = 0; i < ETH_RX_DESC_CNT -1; i++)
  {
    RxBuff[i].next=&RxBuff[i+1];
  }

  if (HAL_ETH_GetRxDataBuffer(&heth, RxBuff) == HAL_OK)
  {
    HAL_ETH_GetRxDataLength(&heth, &framelength);

    /* Build Rx descriptor to be ready for next data reception */
    HAL_ETH_BuildRxDescriptors(&heth);

#if !defined(DUAL_CORE) || defined(CORE_CM7)
    /* Invalidate data cache for ETH Rx Buffers */
    SCB_InvalidateDCache_by_Addr((uint32_t *)RxBuff->buffer, framelength);
#endif

    custom_pbuf  = (struct pbuf_custom*)LWIP_MEMPOOL_ALLOC(RX_POOL);
    if(custom_pbuf != NULL)
    {
      custom_pbuf->custom_free_function = pbuf_free_custom;
      p = pbuf_alloced_custom(PBUF_RAW, framelength, PBUF_REF, custom_pbuf, RxBuff->buffer, framelength);
    }
  }

  return p;
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
void ethernetif_input(void const * argument)
{
    RS232Puts("--- ethernetif_input ---\r\n");
  struct pbuf *p;
  struct netif *netif = (struct netif *) argument;

  for( ;; )
  {
//    if (HAL_ETH_IsRxDataAvailable(&heth)) osSemaphoreRelease(RxPktSemaphore);
    if (osSemaphoreWait(RxPktSemaphore, TIME_WAITING_FOR_INPUT) == osOK)
    {
      do
      {
        p = low_level_input( netif );
        if (p != NULL)
        {
//            RS232Puts("--packet--\r\n");
          if (netif->input( p, netif) != ERR_OK )
          {
            pbuf_free(p);
          }
        }
      } while(p!=NULL);
    }
  }

}

#if !LWIP_ARP
/**
 * This function has to be completed by user in case of ARP OFF.
 *
 * @param netif the lwip network interface structure for this ethernetif
 * @return ERR_OK if ...
 */
static err_t low_level_output_arp_off(struct netif *netif, struct pbuf *q, const ip4_addr_t *ipaddr)
{
  err_t errval;
  errval = ERR_OK;

/* USER CODE BEGIN 5 */

/* USER CODE END 5 */

  return errval;

}
#endif /* LWIP_ARP */

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
err_t ethernetif_init(struct netif *netif)
{
  LWIP_ASSERT("netif != NULL", (netif != NULL));

#if LWIP_NETIF_HOSTNAME
  /* Initialize interface hostname */
  netif->hostname = "lwip";
#endif /* LWIP_NETIF_HOSTNAME */

  netif->name[0] = IFNAME0;
  netif->name[1] = IFNAME1;
  /* We directly use etharp_output() here to save a function call.
   * You can instead declare your own function an call etharp_output()
   * from it if you have to do some checks before sending (e.g. if link
   * is available...) */

#if LWIP_IPV4
#if LWIP_ARP || LWIP_ETHERNET
#if LWIP_ARP
  netif->output = etharp_output;
#else
  /* The user should write its own code in low_level_output_arp_off function */
  netif->output = low_level_output_arp_off;
#endif /* LWIP_ARP */
#endif /* LWIP_ARP || LWIP_ETHERNET */
#endif /* LWIP_IPV4 */

#if LWIP_IPV6
  netif->output_ip6 = ethip6_output;
#endif /* LWIP_IPV6 */

  netif->linkoutput = low_level_output;

  /* initialize the hardware */
  low_level_init(netif);

  return ERR_OK;
}

/**
  * @brief  Custom Rx pbuf free callback
  * @param  pbuf: pbuf to be freed
  * @retval None
  */
void pbuf_free_custom(struct pbuf *p)
{
  struct pbuf_custom* custom_pbuf = (struct pbuf_custom*)p;

  LWIP_MEMPOOL_FREE(RX_POOL, custom_pbuf);
}

/* USER CODE BEGIN 6 */

/**
* @brief  Returns the current time in milliseconds
*         when LWIP_TIMERS == 1 and NO_SYS == 1
* @param  None
* @retval Current Time value
*/
u32_t sys_jiffies(void)
{
  return HAL_GetTick();
}

/**
* @brief  Returns the current time in milliseconds
*         when LWIP_TIMERS == 1 and NO_SYS == 1
* @param  None
* @retval Current Time value
*/
u32_t sys_now(void)
{
  return HAL_GetTick();
}

/* USER CODE END 6 */

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

void ethernet_link_thread(void const * argument)
{
  ETH_MACConfigTypeDef MACConf;
  int32_t PHYLinkState;
  uint32_t linkchanged = 0, speed = 0, duplex =0;

  struct netif *netif = (struct netif *) argument;
/* USER CODE BEGIN ETH link init */
osDelay(500);
  RS232Puts("--- ethernet_link_thread ---\r\n");

/* USER CODE END ETH link init */

  for(;;)
  {

/* USER CODE BEGIN ETH link Thread core code for User BSP */

      PHYLinkState = DP83848_GetLinkState(&DP83848);

//      char msgUart7[50];
//      memset(msgUart7,' ',50);
//      sprintf(msgUart7,"%s %d %s", "\rPHYLinkState = ",PHYLinkState,"\r\n");
//      RS232Puts(msgUart7);

      if(netif_is_link_up(netif) && (PHYLinkState <= DP83848_STATUS_LINK_DOWN))
      {
RS232Puts("--DP83848_STATUS_LINK_DOWN--\n\r");
        HAL_ETH_Stop_IT(&heth);
        netif_set_down(netif);
        netif_set_link_down(netif);
      }
      else if(!netif_is_link_up(netif) && (PHYLinkState > DP83848_STATUS_LINK_DOWN))
      {
        switch (PHYLinkState)
        {
        case DP83848_STATUS_100MBITS_FULLDUPLEX:
RS232Puts("DP83848_STATUS_100MBITS_FULLDUPLEX\n\r");
          duplex = ETH_FULLDUPLEX_MODE;
          speed = ETH_SPEED_100M;
          linkchanged = 1;
          break;
        case DP83848_STATUS_100MBITS_HALFDUPLEX:
RS232Puts("--DP83848_STATUS_100MBITS_HALFDUPLEX--\n\r");
          duplex = ETH_HALFDUPLEX_MODE;
          speed = ETH_SPEED_100M;
          linkchanged = 1;
          break;
        case DP83848_STATUS_10MBITS_FULLDUPLEX:
RS232Puts("--DP83848_STATUS_10MBITS_FULLDUPLEX--\n\r");
          duplex = ETH_FULLDUPLEX_MODE;
          speed = ETH_SPEED_10M;
          linkchanged = 1;
          break;
        case DP83848_STATUS_10MBITS_HALFDUPLEX:
RS232Puts("--DP83848_STATUS_10MBITS_HALFDUPLEX--\n\r");
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

          HAL_ETH_Start_IT(&heth);

          netif_set_up(netif);
          netif_set_link_up(netif);

//          printAll_Regs_DP83848();

        }

      }

/* USER CODE END ETH link Thread core code for User BSP */

    osDelay(1000);
  }
}
/* USER CODE BEGIN 8 */

/* USER CODE END 8 */

