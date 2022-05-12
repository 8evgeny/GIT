/**
  ******************************************************************************
  * File Name          : ethernetif.c
  * Description        : This file provides code for the configuration
  *                      of the ethernetif.c MiddleWare.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
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
#include "lwip/opt.h"

#include "lwip/timeouts.h"
#include "netif/ethernet.h"
#include "netif/etharp.h"
#include "lwip/ethip6.h"
#include "ethernetif.h"
#include <string.h>
#include "cmsis_os.h"
#include "main.h"
#include "stm32h7xx_hal_conf.h"
#include "rs232_printf.h"
#include "dp83848.h"
#include <stdio.h>

/* Private define ------------------------------------------------------------*/
/* The time to block waiting for input. */
#define TIME_WAITING_FOR_INPUT ( osWaitForever )
/* Stack size of the interface thread */
#define INTERFACE_THREAD_STACK_SIZE ( 350 )
/* Network interface name */
#define IFNAME0 's'
#define IFNAME1 't'
#define ETH_DMA_TRANSMIT_TIMEOUT               ( 20U )

ETH_DMADescTypeDef  DMARxDscrTab[((uint32_t)4U)] __attribute__((section(".RxDecripSection")));/* Ethernet Rx DMA Descriptors */

ETH_DMADescTypeDef  DMATxDscrTab[((uint32_t)4U)] __attribute__((section(".TxDescripSection")));/* Ethernet Tx DMA Descriptors */

uint8_t Rx_Buff[((uint32_t)4U)][((uint32_t)1524U)] __attribute__((section(".RxarraySection"))); /* Ethernet Receive Buffers */

uint8_t Tx_Buff[((uint32_t)4U)][((uint32_t)1524U)] __attribute__((section(".TxarraySection"))); /* Ethernet Transmit Buffers */


/* Semaphore to signal incoming packets */
osSemaphoreId s_xSemaphore = NULL;
/* Global Ethernet handle */
ETH_HandleTypeDef heth;
ETH_TxPacketConfig TxConfig;

/* Memory Pool Declaration */
LWIP_MEMPOOL_DECLARE(RX_POOL, 10, sizeof(struct pbuf_custom), "Zero-copy RX PBUF pool");

int32_t ETH_PHY_IO_Init(void)
{
  /* We assume that MDIO GPIO configuration is already done
     in the ETH_MspInit() else it should be done here
  */

  /* Configure the MDIO Clock */
  HAL_ETH_SetMDIOClockRange(&heth);

  return 0;
}
int32_t ETH_PHY_IO_DeInit (void)
{
  return 0;
}
int32_t ETH_PHY_IO_ReadReg(uint32_t DevAddr, uint32_t RegAddr, uint32_t *pRegVal)
{
  if(HAL_ETH_ReadPHYRegister(&heth, DevAddr, RegAddr, pRegVal) != HAL_OK)
  {
    return -1;
  }

  return 0;
}
int32_t ETH_PHY_IO_WriteReg(uint32_t DevAddr, uint32_t RegAddr, uint32_t RegVal)
{
  if(HAL_ETH_WritePHYRegister(&heth, DevAddr, RegAddr, RegVal) != HAL_OK)
  {
    return -1;
  }

  return 0;
}
int32_t ETH_PHY_IO_GetTick(void)
{
  return HAL_GetTick();
}

DP83848_Object_t DP83848;
DP83848_IOCtx_t  DP83848_IOCtx = {ETH_PHY_IO_Init,
                                  ETH_PHY_IO_DeInit,
                                  ETH_PHY_IO_WriteReg,
                                  ETH_PHY_IO_ReadReg,
                                  ETH_PHY_IO_GetTick};


/* Private functions ---------------------------------------------------------*/

void HAL_ETH_MspInit(ETH_HandleTypeDef *ethHandle)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    if (ethHandle->Instance == ETH) {

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
        PC2     ------> ETH_TXD2
        PC3     ------> ETH_TX_CLK
        PA0/WKUP     ------> ETH_CRS
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
        GPIO_InitStruct.Pin = GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4
                              | GPIO_PIN_5;
        GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
        GPIO_InitStruct.Alternate = GPIO_AF11_ETH;
        HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

        GPIO_InitStruct.Pin = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_7;
        GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
        GPIO_InitStruct.Alternate = GPIO_AF11_ETH;
        HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

        GPIO_InitStruct.Pin = GPIO_PIN_3;
        GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
        GPIO_InitStruct.Alternate = GPIO_AF11_ETH;
        HAL_GPIO_Init(GPIOH, &GPIO_InitStruct);

        GPIO_InitStruct.Pin = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_10 | GPIO_PIN_11
                              | GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_8;
        GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
        GPIO_InitStruct.Alternate = GPIO_AF11_ETH;
        HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

        /* Peripheral interrupt init */
        HAL_NVIC_SetPriority(ETH_IRQn, 1, 0);
        HAL_NVIC_EnableIRQ(ETH_IRQn);
        HAL_NVIC_SetPriority(ETH_WKUP_IRQn, 1, 0);
        HAL_NVIC_EnableIRQ(ETH_WKUP_IRQn);


    }
}

void HAL_ETH_MspDeInit(ETH_HandleTypeDef *ethHandle)
{
    if (ethHandle->Instance == ETH) {
        /* Peripheral clock disable */
        __HAL_RCC_ETH1MAC_CLK_DISABLE();
        __HAL_RCC_ETH1TX_CLK_DISABLE();
        __HAL_RCC_ETH1RX_CLK_DISABLE();

        /**ETH GPIO Configuration
        PC1     ------> ETH_MDC
        PC2     ------> ETH_TXD2
        PC3     ------> ETH_TX_CLK
        PA0/WKUP     ------> ETH_CRS
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
        HAL_GPIO_DeInit(GPIOC, GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4
                        | GPIO_PIN_5);

        HAL_GPIO_DeInit(GPIOA, GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_7);

        HAL_GPIO_DeInit(GPIOH, GPIO_PIN_3);

        HAL_GPIO_DeInit(GPIOB, GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_10 | GPIO_PIN_11
                        | GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_8);

        /* Peripheral interrupt Deinit*/
        HAL_NVIC_DisableIRQ(ETH_IRQn);

        HAL_NVIC_DisableIRQ(ETH_WKUP_IRQn);
    }
}

/**
  * @brief  Ethernet Rx Transfer completed callback
  * @param  heth: ETH handle
  * @retval None
  */
void HAL_ETH_RxCpltCallback(ETH_HandleTypeDef *heth)
{
    osSemaphoreRelease(s_xSemaphore);
}

/*******************************************************************************
                       LL Driver Interface ( LwIP stack --> ETH)
*******************************************************************************/
/**
 * In this function, the hardware should be initialized.
 * Called from ethernetif_init().
 *
 * @param netif the already initialized lwip network interface structure
 *        for this ethernetif
 */
static void low_level_init(struct netif *netif)
{
RS232Puts("low_level_init start\n") ;

    HAL_StatusTypeDef hal_eth_init_status = HAL_OK;
    uint32_t idx = 0;
    /* Start ETH HAL Init */

    uint8_t MACAddr[6] ;
    heth.Instance = ETH;
    MACAddr[0] = 0x40;
    MACAddr[1] = 0x8d;
    MACAddr[2] = 0x5c;
    MACAddr[3] = 0xba;
    MACAddr[4] = 0xf6;
    MACAddr[5] = 0x22;
    heth.Init.MACAddr = &MACAddr[0];
    heth.Init.MediaInterface = HAL_ETH_MII_MODE;
    heth.Init.TxDesc = DMATxDscrTab;
    heth.Init.RxDesc = DMARxDscrTab;
    heth.Init.RxBuffLen = 1524;

    /* USER CODE BEGIN MACADDRESS */
//    sPDOSettings *sset = sysset_get_settings();
//    MACAddr[3] = sset->network.ip[1];
//    MACAddr[4] = sset->network.ip[2];
//    MACAddr[5] = sset->network.ip[3];
    /* USER CODE END MACADDRESS */

    hal_eth_init_status = HAL_ETH_Init(&heth);

    memset(&TxConfig, 0 , sizeof(ETH_TxPacketConfig));
    TxConfig.Attributes = ETH_TX_PACKETS_FEATURES_CSUM | ETH_TX_PACKETS_FEATURES_CRCPAD;
    TxConfig.ChecksumCtrl = ETH_CHECKSUM_IPHDR_PAYLOAD_INSERT_PHDR_CALC;
    TxConfig.CRCPadCtrl = ETH_CRC_PAD_INSERT;

    /* End ETH HAL Init */

    /* Initialize the RX POOL */
    LWIP_MEMPOOL_INIT(RX_POOL);


    if (hal_eth_init_status == HAL_OK) {
        /* Set netif link flag */
        netif->flags |= NETIF_FLAG_LINK_UP;
    }


RS232Puts("$$$$ NEED CHANGE TX RX DESCRIPTORS $$$$\n") ;

//    /* Initialize Tx Descriptors list: Chain Mode */
//    HAL_ETH_DMATxDescListInit(&heth, DMATxDscrTab, &Tx_Buff[0][0], ETH_TXBUFNB);

//    /* Initialize Rx Descriptors list: Chain Mode  */
//    HAL_ETH_DMARxDescListInit(&heth, DMARxDscrTab, &Rx_Buff[0][0], ETH_RXBUFNB);




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
    netif->flags |= NETIF_FLAG_BROADCAST | NETIF_FLAG_ETHARP;
#else
    netif->flags |= NETIF_FLAG_BROADCAST;
#endif /* LWIP_ARP */

    for(idx = 0; idx < ETH_RX_DESC_CNT; idx ++)
    {
      HAL_ETH_DescAssignMemory(&heth, idx, Rx_Buff[idx], NULL);
    }

    /* Set PHY IO functions */
    DP83848_RegisterBusIO(&DP83848, &DP83848_IOCtx);

    /* Initialize the DP83848 ETH PHY */
    DP83848_Init(&DP83848);

    if (hal_eth_init_status == HAL_OK)
    {
    /* Get link state */
    ethernet_link_check_state(netif);
    }
    else
    {
      Error_Handler();
    }

RS232Puts("$$$$ ethernetif.c  353 $$$$\n") ;


    netif->flags |= NETIF_FLAG_IGMP;

    /* create a binary semaphore used for informing ethernetif of frame reception */
    osSemaphoreDef(SEM);
    s_xSemaphore = osSemaphoreCreate(osSemaphore(SEM), 1);

    /* create the task that handles the ETH_MAC */
    osThreadDef(EthIf, ethernetif_input, osPriorityRealtime, 0, INTERFACE_THREAD_STACK_SIZE);
    osThreadCreate(osThread(EthIf), netif);

    /* Enable MAC and DMA transmission and reception */
    HAL_ETH_Start(&heth);

    /**** Configure PHY to generate an interrupt when Eth Link state changes ****/
    /* Read Register Configuration */
//    HAL_ETH_ReadPHYRegister(&heth, PHY_MICR, &regvalue);

//    regvalue |= (PHY_MICR_INT_EN | PHY_MICR_INT_OE);

//    /* Enable Interrupts */
//    HAL_ETH_WritePHYRegister(&heth, PHY_MICR, regvalue);

//    /* Read Register Configuration */
//    HAL_ETH_ReadPHYRegister(&heth, PHY_MISR, &regvalue);

//    regvalue |= PHY_MISR_LINK_INT_EN;

    /* Enable Interrupt on change of link status */
//    HAL_ETH_WritePHYRegister(&heth, PHY_MISR, regvalue);

#endif /* LWIP_ARP || LWIP_ETHERNET */

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
 *       to become availale since the stack doesn't retry to send a packet
 *       dropped because of memory failure (except for the TCP timers).
 */

static err_t low_level_output(struct netif *netif, struct pbuf *p)
{
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
  * @brief  Custom Rx pbuf free callback
  * @param  pbuf: pbuf to be freed
  * @retval None
  */
void pbuf_free_custom(struct pbuf *p)
{
  struct pbuf_custom* custom_pbuf = (struct pbuf_custom*)p;

  LWIP_MEMPOOL_FREE(RX_POOL, custom_pbuf);
}


/**
 * Should allocate a pbuf and transfer the bytes of the incoming
 * packet from the interface into the pbuf.
 *
 * @param netif the lwip network interface structure for this ethernetif
 * @return a pbuf filled with the received packet (including MAC header)
 *         NULL on memory error
   */
static struct pbuf *low_level_input(struct netif *netif)
{
    struct pbuf *p = NULL;
    ETH_BufferTypeDef RxBuff[ETH_RX_DESC_CNT];
    uint32_t framelength = 0, i = 0;
    struct pbuf_custom* custom_pbuf;

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

      custom_pbuf  = (struct pbuf_custom*)LWIP_MEMPOOL_ALLOC(RX_POOL);
      custom_pbuf->custom_free_function = pbuf_free_custom;

      p = pbuf_alloced_custom(PBUF_RAW, framelength, PBUF_REF, custom_pbuf, RxBuff->buffer, framelength);

      return p;
    }
    else
    {
      return NULL;
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
void ethernetif_input(void const *argument)
{
    struct pbuf *p;
    struct netif *netif = (struct netif *) argument;

RS232Puts("****  ethernetif_input  start  ****\n");

    for (;;) {
        if (osSemaphoreWait(s_xSemaphore, TIME_WAITING_FOR_INPUT) == osOK)
        {
            do {
                p = low_level_input(netif);
                if (p != NULL)
                {
RS232Puts("----  ethernetif_input  osSemaphoreWait  ----\n");
                    if (netif->input(p, netif) != ERR_OK) {
                        pbuf_free(p);
                    }
                }
            } while (p != NULL);
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
    /* The user should write ist own code in low_level_output_arp_off function */
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
* @brief  Returns the current time in milliseconds
*         when LWIP_TIMERS == 1 and NO_SYS == 1
* @param  None
* @retval Time
*/
u32_t sys_jiffies(void)
{
    return HAL_GetTick();
}

/**
* @brief  Returns the current time in milliseconds
*         when LWIP_TIMERS == 1 and NO_SYS == 1
* @param  None
* @retval Time
*/
u32_t sys_now(void)
{
    return HAL_GetTick();
}

#if LWIP_NETIF_LINK_CALLBACK
/**
  * @brief  Link callback function, this function is called on change of link status
  *         to update low level driver configuration.
* @param  netif: The network interface
  * @retval None
  */
void ethernetif_update_config(struct netif *netif)
{
    ethernet_link_check_state(netif);

//    __IO uint32_t tickstart = 0;
//    uint32_t regvalue = 0;

//    if (netif_is_link_up(netif)) {
//        /* Restart the auto-negotiation */
//        if (heth.Init.AutoNegotiation != ETH_AUTONEGOTIATION_DISABLE) {
//            /* Enable Auto-Negotiation */
//            HAL_ETH_WritePHYRegister(&heth, PHY_BCR, PHY_AUTONEGOTIATION);

//            /* Get tick */
//            tickstart = HAL_GetTick();

//            /* Wait until the auto-negotiation will be completed */
//            do {
//                HAL_ETH_ReadPHYRegister(&heth, PHY_BSR, &regvalue);

//                /* Check for the Timeout ( 1s ) */
//                if ((HAL_GetTick() - tickstart) > 1000) {
//                    /* In case of timeout */
//                    goto error;
//                }
//            } while (((regvalue & PHY_AUTONEGO_COMPLETE) != PHY_AUTONEGO_COMPLETE));

//            /* Read the result of the auto-negotiation */
//            HAL_ETH_ReadPHYRegister(&heth, PHY_SR, &regvalue);
//            int linkup = !!(regvalue & PHY_LINK_STATUS);

//            /* Configure the MAC with the Duplex Mode fixed by the auto-negotiation process */
//            if ((regvalue & PHY_DUPLEX_STATUS) != (uint32_t)RESET) {
//                /* Set Ethernet duplex mode to Full-duplex following the auto-negotiation */
//                heth.Init.DuplexMode = ETH_MODE_FULLDUPLEX;
//            } else {
//                /* Set Ethernet duplex mode to Half-duplex following the auto-negotiation */
//                heth.Init.DuplexMode = ETH_MODE_HALFDUPLEX;
//            }
//            /* Configure the MAC with the speed fixed by the auto-negotiation process */
//            if (regvalue & PHY_SPEED_STATUS) {
//                /* Set Ethernet speed to 10M following the auto-negotiation */
//                heth.Init.Speed = ETH_SPEED_10M;
//            } else {
//                /* Set Ethernet speed to 100M following the auto-negotiation */
//                heth.Init.Speed = ETH_SPEED_100M;
//            }
//        } else { /* AutoNegotiation Disable */
//error :
//            /* Check parameters */
//            assert_param(IS_ETH_SPEED(heth.Init.Speed));
//            assert_param(IS_ETH_DUPLEX_MODE(heth.Init.DuplexMode));

//            /* Set MAC Speed and Duplex Mode to PHY */
//            HAL_ETH_WritePHYRegister(&heth, PHY_BCR, ((uint16_t)(heth.Init.DuplexMode >> 3) |
//                                     (uint16_t)(heth.Init.Speed >> 1)));
//        }

//        /* ETHERNET MAC Re-Configuration */
//        HAL_ETH_ConfigMAC(&heth, (ETH_MACInitTypeDef *) NULL);

//        /* Restart MAC interface */
//        HAL_ETH_Start(&heth);
//    } else {
//        /* Stop MAC interface */
//        HAL_ETH_Stop(&heth);
//    }

//    ethernetif_notify_conn_changed(netif);
}

/**
  * @brief  This function notify user about link status changement.
  * @param  netif: the network interface
  * @retval None
  */
__weak void ethernetif_notify_conn_changed(struct netif *netif)
{
    /* NOTE : This is function could be implemented in user file
              when the callback is needed,
    */

}
#endif /* LWIP_NETIF_LINK_CALLBACK */

/**
  * @brief  This function handles ethernet DMA interrupt request.
  * @param  None
  * @retval None
  */
void ETH_IRQHandler(void)
{
    HAL_ETH_IRQHandler(&heth);
}


void ETH_WKUP_IRQHandler(void)
{
    HAL_ETH_IRQHandler(&heth);
}
/**
  * @brief  This function sets the netif link status.
  * @param  netif: the network interface
  * @retval None
  */
void ethernetif_set_link(void const *argument)
{
//    uint32_t regvalue = 0;
    struct link_str *link_arg = (struct link_str *)argument;

HAL_Delay(500);
RS232Puts("****  ethernetif_set_link  start  ****\n");

    for (;;)
    {

//        if (osSemaphoreWait(link_arg->semaphore, osWaitForever) == osOK)
//        {


            ethernet_link_check_state(link_arg->netif);



//            /* Read PHY_MISR*/
//            HAL_ETH_ReadPHYRegister(&heth, PHY_MISR, &regvalue);

//            /* Check whether the link interrupt has occurred or not */
//            if ((regvalue & PHY_LINK_INTERRUPT) != (uint16_t)RESET) {
//                /* Read PHY_SR*/
//                HAL_ETH_ReadPHYRegister(&heth, PHY_SR, &regvalue);

//                /* Check whether the link is up or down*/
//                if ((regvalue & PHY_LINK_STATUS) != (uint16_t)RESET) {
//                    netif_set_link_up(link_arg->netif);
//                } else {
//                    netif_set_link_down(link_arg->netif);
//                }
//            }

//        }
    }

}

void ethernet_link_check_state(struct netif *netif)
{
  ETH_MACConfigTypeDef MACConf;
  int32_t PHYLinkState;
  uint32_t linkchanged = 0, speed = 0, duplex =0;

  PHYLinkState = DP83848_GetLinkState(&DP83848);

  if(netif_is_link_up(netif) && (PHYLinkState <= DP83848_STATUS_LINK_DOWN))
  {
      char str[10];
      sprintf(str, "%d", PHYLinkState);
RS232Puts(str);
RS232Puts("****  ethernet_link_check_state  1  ****\n");

    HAL_ETH_Stop(&heth);
    netif_set_down(netif);
    netif_set_link_down(netif);
  }
  else if(!netif_is_link_up(netif) && (PHYLinkState > DP83848_STATUS_LINK_DOWN))
  {
RS232Puts("****  ethernet_link_check_state  2  ****\n");
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
RS232Puts("****  ethernet_link_check_state  3  ****\n");

      /* Get MAC Config MAC */
      HAL_ETH_GetMACConfig(&heth, &MACConf);
      MACConf.DuplexMode = duplex;
      MACConf.Speed = speed;
      HAL_ETH_SetMACConfig(&heth, &MACConf);

      HAL_ETH_Start(&heth);
      netif_set_up(netif);
      netif_set_link_up(netif);
    }
  }

}
