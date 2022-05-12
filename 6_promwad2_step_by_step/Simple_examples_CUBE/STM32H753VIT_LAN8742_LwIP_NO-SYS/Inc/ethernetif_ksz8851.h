/**
 ******************************************************************************
  * File Name          : ethernetif.h
  * Description        : This file provides initialization code for LWIP
  *                      middleWare.
  ******************************************************************************
  * This notice applies to any and all portions of this file
  * that are not between comment pairs USER CODE BEGIN and
  * USER CODE END. Other portions of this file, whether 
  * inserted by the user or by software development tools
  * are owned by their respective copyright owners.
  *
  * Copyright (c) 2019 STMicroelectronics International N.V. 
  * All rights reserved.
  *
  * Redistribution and use in source and binary forms, with or without 
  * modification, are permitted, provided that the following conditions are met:
  *
  * 1. Redistribution of source code must retain the above copyright notice, 
  *    this list of conditions and the following disclaimer.
  * 2. Redistributions in binary form must reproduce the above copyright notice,
  *    this list of conditions and the following disclaimer in the documentation
  *    and/or other materials provided with the distribution.
  * 3. Neither the name of STMicroelectronics nor the names of other 
  *    contributors to this software may be used to endorse or promote products 
  *    derived from this software without specific written permission.
  * 4. This software, including modifications and/or derivative works of this 
  *    software, must execute solely and exclusively on microcontroller or
  *    microprocessor devices manufactured by or for STMicroelectronics.
  * 5. Redistribution and use of this software other than as permitted under 
  *    this license is void and will automatically terminate your rights under 
  *    this license. 
  *
  * THIS SOFTWARE IS PROVIDED BY STMICROELECTRONICS AND CONTRIBUTORS "AS IS" 
  * AND ANY EXPRESS, IMPLIED OR STATUTORY WARRANTIES, INCLUDING, BUT NOT 
  * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A 
  * PARTICULAR PURPOSE AND NON-INFRINGEMENT OF THIRD PARTY INTELLECTUAL PROPERTY
  * RIGHTS ARE DISCLAIMED TO THE FULLEST EXTENT PERMITTED BY LAW. IN NO EVENT 
  * SHALL STMICROELECTRONICS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
  * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
  * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, 
  * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF 
  * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING 
  * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
  * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */
  

#ifndef __ETHERNETIF_KSZ8851_H__
#define __ETHERNETIF_KSZ8851_H__

#include "lwip/err.h"
#include "lwip/netif.h"

#define KSZ8851_ETHERNET_CONF_ETHADDR0                        0x00
#define KSZ8851_ETHERNET_CONF_ETHADDR1                        0x80
#define KSZ8851_ETHERNET_CONF_ETHADDR2                        0xe1
#define KSZ8851_ETHERNET_CONF_ETHADDR3                        0x00
#define KSZ8851_ETHERNET_CONF_ETHADDR4                        0x00
#define KSZ8851_ETHERNET_CONF_ETHADDR5                        0x00


static void  ethernetif_input_KSZ8851(struct netif *netif);
static void low_level_init_KSZ8851(struct netif *netif);
static err_t low_level_output_KSZ8851(struct netif *netif, struct pbuf *p);
static struct pbuf * low_level_input_KSZ8851(struct netif *netif);
static void ethernetif_input_KSZ8851(struct netif *netif);
err_t ethernetif_init_KSZ8851(struct netif *netif);





/* Within 'USER CODE' section, code will be kept by default at each generation */
/* USER CODE BEGIN 0 */
//void INTN_Handler_KSZ8851();

/* USER CODE END 0 */

/* Exported functions ------------------------------------------------------- */
//err_t ethernetif_init_KSZ8851(struct netif *netif);

//void ethernetif_input_KSZ8851(struct netif *netif);

//u32_t sys_jiffies_KSZ8851(void);
//u32_t sys_now_KSZ8851(void);

/* USER CODE BEGIN 1 */
//void ethernet_link_check_state_KSZ8851(struct netif *netif);

//static void ksz8851snl_rx_populate_queue(struct ksz8851snl_device *p_ksz8851snl_dev);
//void ksz8851snl_rx_init(struct ksz8851snl_device *ps_ksz8851snl_dev);
//void ksz8851snl_tx_init(struct ksz8851snl_device *ps_ksz8851snl_dev);

/* USER CODE END 1 */
#endif

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
