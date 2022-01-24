
/**
  ******************************************************************************
  * File Name          : Target/lwipopts.h
  * Description        : This file overrides LwIP stack default configuration
  *                      done in opt.h file.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */
 
/* Define to prevent recursive inclusion --------------------------------------*/
#ifndef __LWIPOPTS__H__
#define __LWIPOPTS__H__

#include "main.h"

/*-----------------------------------------------------------------------------*/
/* Current version of LwIP supported by CubeMx: 2.0.3 -*/
/*-----------------------------------------------------------------------------*/

/* Within 'USER CODE' section, code will be kept by default at each generation */
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

#ifdef __cplusplus
 extern "C" {
#endif

/* STM32CubeMX Specific Parameters (not defined in opt.h) ---------------------*/
/* Parameters set in STM32CubeMX LwIP Configuration GUI -*/
/*----- WITH_RTOS enabled (Since FREERTOS is set) -----*/
#define WITH_RTOS 1
/*----- CHECKSUM_BY_HARDWARE enabled -----*/
#define CHECKSUM_BY_HARDWARE 1


/*-----------------------------------------------------------------------------*/

/* LwIP Stack Parameters (modified compared to initialization value in opt.h) -*/
/* Parameters set in STM32CubeMX LwIP Configuration GUI -*/
/*----- Default Value for LWIP_IGMP: 0 ---*/
#define LWIP_IGMP 1
/*----- Default Value for MEMP_NUM_UDP_PCB: 4 ---*/
#define MEMP_NUM_UDP_PCB 16
/*----- Default Value for MEMP_NUM_TCP_PCB: 5 ---*/
#define MEMP_NUM_TCP_PCB 16
/*----- Value in opt.h for MEM_ALIGNMENT: 1 -----*/
#define MEM_ALIGNMENT 4
/*----- Default Value for MEM_SIZE: 1600 ---*/
#define MEM_SIZE (64*1024)
/*----- Default Value for MEMP_OVERFLOW_CHECK: 0 ---*/
#define MEMP_OVERFLOW_CHECK 1
/*----- Default Value for H7 devices: 0x30044000 -----*/
#define LWIP_RAM_HEAP_POINTER 0x30004000
/*----- Default Value for MEMP_NUM_PBUF: 16 ---*/
#define MEMP_NUM_PBUF 64
/*----- Default Value for MEMP_NUM_TCP_PCB_LISTEN: 8 ---*/
#define MEMP_NUM_TCP_PCB_LISTEN 16
/*----- Default Value for MEMP_NUM_NETBUF: 2 ---*/
#define MEMP_NUM_NETBUF 64
/*----- Default Value for MEMP_NUM_NETCONN: 4 ---*/
#define MEMP_NUM_NETCONN 64
/*----- Value supported for H7 devices: 1 -----*/
#define LWIP_SUPPORT_CUSTOM_PBUF 1
/*----- Default Value for ARP_TABLE_SIZE: 10 ---*/
#define ARP_TABLE_SIZE 10
/*----- Default Value for ARP_QUEUEING: 0 ---*/
#define ARP_QUEUEING 1
/*----- Default Value for ETHARP_SUPPORT_VLAN: 0 ---*/
#define ETHARP_SUPPORT_VLAN 1
/*----- Value in opt.h for LWIP_ETHERNET: LWIP_ARP || PPPOE_SUPPORT -*/
#define LWIP_ETHERNET 1
/*----- Default Value for LWIP_BROADCAST_PING: 0 ---*/
#define LWIP_BROADCAST_PING 1
/*----- Default Value for LWIP_MULTICAST_PING: 0 ---*/
#define LWIP_MULTICAST_PING 1
/*----- Value in opt.h for LWIP_DNS_SECURE: (LWIP_DNS_SECURE_RAND_XID | LWIP_DNS_SECURE_NO_MULTIPLE_OUTSTANDING | LWIP_DNS_SECURE_RAND_SRC_PORT) -*/
#define LWIP_DNS_SECURE 7
/*----- Default Value for LWIP_NETBUF_RECVINFO: 0 ---*/
#define LWIP_NETBUF_RECVINFO 1
/*----- Default Value for TCP_MSS: 536 ---*/
#define TCP_MSS 1426
/*----- Value in opt.h for TCP_SND_QUEUELEN: (4*TCP_SND_BUF + (TCP_MSS - 1))/TCP_MSS -----*/
#define TCP_SND_QUEUELEN 9
/*----- Value in opt.h for TCP_SNDQUEUELOWAT: LWIP_MAX(TCP_SND_QUEUELEN)/2, 5) -*/
#define TCP_SNDQUEUELOWAT 5
/*----- Default Value for LWIP_LOOPIF_MULTICAST: 0 ---*/
#define LWIP_LOOPIF_MULTICAST 1
/*----- Value in opt.h for TCPIP_THREAD_STACKSIZE: 0 -----*/
#define TCPIP_THREAD_STACKSIZE 1024*4
/*----- Value in opt.h for TCPIP_THREAD_PRIO: 1 -----*/
#define TCPIP_THREAD_PRIO  3	//24
/*----- Value in opt.h for TCPIP_MBOX_SIZE: 0 -----*/
#define TCPIP_MBOX_SIZE 16
/*----- Value in opt.h for SLIPIF_THREAD_STACKSIZE: 0 -----*/
#define SLIPIF_THREAD_STACKSIZE 1024
/*----- Value in opt.h for SLIPIF_THREAD_PRIO: 1 -----*/
#define SLIPIF_THREAD_PRIO 3
/*----- Value in opt.h for DEFAULT_THREAD_STACKSIZE: 0 -----*/
#define DEFAULT_THREAD_STACKSIZE 1024
/*----- Value in opt.h for DEFAULT_THREAD_PRIO: 1 -----*/
#define DEFAULT_THREAD_PRIO 3
/*----- Value in opt.h for DEFAULT_UDP_RECVMBOX_SIZE: 0 -----*/
#define DEFAULT_UDP_RECVMBOX_SIZE 16
/*----- Value in opt.h for DEFAULT_TCP_RECVMBOX_SIZE: 0 -----*/
#define DEFAULT_TCP_RECVMBOX_SIZE 16
/*----- Value in opt.h for DEFAULT_ACCEPTMBOX_SIZE: 0 -----*/
#define DEFAULT_ACCEPTMBOX_SIZE 16
/*----- Default Value for LWIP_SO_SNDTIMEO: 0 ---*/
#define LWIP_SO_SNDTIMEO 1
/*----- Default Value for LWIP_SO_RCVTIMEO: 0 ---*/
#define LWIP_SO_RCVTIMEO 1
/*----- Value in opt.h for RECV_BUFSIZE_DEFAULT: INT_MAX -----*/
#define RECV_BUFSIZE_DEFAULT 2000000000
/*----- Default Value for SO_REUSE: 0 ---*/
#define SO_REUSE 1
/*----- Default Value for LWIP_SNTP: 0 ---*/
#define LWIP_SNTP 1
/*----- Default Value for LWIP_DISABLE_TCP_SANITY_CHECKS: 0 ---*/
#define LWIP_DISABLE_TCP_SANITY_CHECKS 1
/*----- Default Value for LWIP_DISABLE_MEMP_SANITY_CHECKS: 0 ---*/
#define LWIP_DISABLE_MEMP_SANITY_CHECKS 1
/*----- Default Value for LWIP_PERF: 0 ---*/
#define LWIP_PERF 1
/*----- Default Value for LWIP_STATS: 0 ---*/
#define LWIP_STATS 1
//#define LWIP_STATS_DISPLAY 1

/*----- Value in opt.h for MIB2_STATS: 0 or SNMP_LWIP_MIB2 -----*/
#define MIB2_STATS 0
/*----- Value in opt.h for CHECKSUM_GEN_IP: 1 -----*/
#define CHECKSUM_GEN_IP 0
/*----- Value in opt.h for CHECKSUM_GEN_UDP: 1 -----*/
#define CHECKSUM_GEN_UDP 0
/*----- Value in opt.h for CHECKSUM_GEN_TCP: 1 -----*/
#define CHECKSUM_GEN_TCP 0
/*----- Value in opt.h for CHECKSUM_GEN_ICMP6: 1 -----*/
#define CHECKSUM_GEN_ICMP6 0
/*----- Value in opt.h for CHECKSUM_CHECK_IP: 1 -----*/
#define CHECKSUM_CHECK_IP 0
/*----- Value in opt.h for CHECKSUM_CHECK_UDP: 1 -----*/
#define CHECKSUM_CHECK_UDP 0
/*----- Value in opt.h for CHECKSUM_CHECK_TCP: 1 -----*/
#define CHECKSUM_CHECK_TCP 0
/*----- Value in opt.h for CHECKSUM_CHECK_ICMP: 1 -----*/
#define CHECKSUM_CHECK_ICMP 0
/*----- Value in opt.h for CHECKSUM_CHECK_ICMP6: 1 -----*/
#define CHECKSUM_CHECK_ICMP6 0
/*-----------------------------------------------------------------------------*/
/* USER CODE BEGIN 1 */

#define LWIP_RAM_HEAP_POINTER 0x30004000
#define MEMP_MEM_MALLOC 1
/***
 * TODO: Look
 */
#define LWIP_COMPAT_SOCKETS 2

//#define LWIP_DEBUG 1

//#define TCP_DEBUG                       LWIP_DBG_ON
//#define ETHARP_DEBUG                    LWIP_DBG_ON
//#define PBUF_DEBUG                      LWIP_DBG_ON
//#define SOCKETS_DEBUG                      LWIP_DBG_ON
//#define INET_DEBUG                      LWIP_DBG_ON
//#define MEMP_DEBUG                      LWIP_DBG_ON
//#define MEM_DEBUG                      LWIP_DBG_ON
//#define SYS_DEBUG                      LWIP_DBG_ON
/* USER CODE END 1 */

#ifdef __cplusplus
}
#endif
#endif /*__LWIPOPTS__H__ */

/************************* (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
