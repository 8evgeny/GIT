/**
  ******************************************************************************
  * @file    udp_exchange.h
  * @author  destr0yer@@ETS
  * @brief   This file contains UDP exchange functions module header
  ******************************************************************************
  */
#ifndef SRC_UDP_EXCHANGE_H_
#define SRC_UDP_EXCHANGE_H_

#include "lwip.h"
#include "stdint.h"

/** @defgroup udp_exchange UDP exchange functions module
  * @brief UDP exchange functions implementation
  * @{
  */

/// UDP control protocol port
#define UDP_CTRL_PORT 12341
/// UDP audio protocol port
#define UDP_AUDIO_PORT 12344

/// size of audio data in RTP packet
#define SND_BUF_PACKET 160

/// size of UDP packets buffer
#define CMD_BUFFER_SIZE 4000

/**
  * @brief  UDP exchange initialization
  * @param  [in] audio_port - audio UDP port
  * @param  [in] ctrl_port - control UDP port
  * @param  [in] config_port - configurator port
  * @retval none
  */
void udp_ips_init(uint16_t audio_port, uint16_t ctrl_port, uint16_t config_port);

/**
  * @brief  send control data to IP
  * @param  [in] buffer - control data
  * @param  [in] len - size of control data
  * @param  [in] ip - destination IP address
  * @param  [in] port - destination UDP port
  * @retval none
  */
void udp_ips_send(void* buffer, size_t len, ip_addr_t ip, uint16_t port);

/**
  * @brief  send RTP audio data to IP
  * @param  [in] buffer - audio data
  * @param  [in] len - size of audio data
  * @param  [in] ip - destination IP address
  * @retval none
  */
void udp_ips_send_rtp(void* buffer, size_t len, ip_addr_t ip);

/**
  * @brief  send log data to log server
  * @param  [in] buffer - log data
  * @param  [in] len - size of log data
  * @retval none
  */
void udp_ips_send_log(void* buffer, size_t len);

/**
  * @brief  send RTP audio data to log server
  * @param  [in] buffer - audio data
  * @param  [in] len - size of audio data
  * @retval none
  */
void udp_ips_send_rtp_log(void* buffer, size_t len);

/**
  * @brief  get size of received UDP packet
  * @retval result - size of received packet (0 - no packets)
  */
uint16_t CmdCfgBuffer_GetNextPktSize ();

/**
  * @brief  read received UDP packet
  * @param  [inout] data - array for received packet data
  * @param  [in] size - size of data
  * @retval none
  */
void cmdcfg_pckt_read (uint8_t *data, uint32_t size);

/**
  * @}
  */

#endif /* SRC_UDP_EXCHANGE_H_ */
