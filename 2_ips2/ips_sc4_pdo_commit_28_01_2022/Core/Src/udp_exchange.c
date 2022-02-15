/**
  ******************************************************************************
  * @file    udp_exchange.c
  * @author  destr0yer@@ETS
  * @brief   This file contains UDP exchange functions implementation
  ******************************************************************************
  */
#include <string.h>
#include "udp.h"
#include "cli_io.h"
#include "pckt_buffer.h"
#include "rtp.h"
#include "g711.h"
#include "system_settings.h"
#include "audio_process.h"
#include "connect_manager.h"
#include "udp_exchange.h"

/// UDP control protocol LwIP PCB
struct udp_pcb* cfg_udp;
/// UDP audio protocol LwIP PCB
struct udp_pcb* audio_udp;
unsigned int received_pcount; // debug counter

u16_t udp_cfg_port; 	///< UPD configurator port number
u16_t udp_audio_port; 	///< UPD audio port number
u16_t udp_ctrl_port; 	///< UPD PDO exchange port number
u16_t udp_log_port; 	///< UPD command log server port number
u16_t rtp_log_port;		///< UPD audio log server port number
ip_addr_t log_ip;		///< UPD configurator port IP address

/// current RTP header structure
rtp_hdr RTP_header;

/// input packet buffer
uint8_t pckt_cmdcfg_input_buffer[CMD_BUFFER_SIZE];
/// input packet buffer description structure
PktBuffer pckt_cmdcfg_input;

uint16_t CmdCfgBuffer_GetNextPktSize ()
{
	uint16_t pktsize;
	uint8_t get_size = PktBuffer_GetNextPktSize(&pckt_cmdcfg_input, &pktsize);
	if (get_size)
	{
		return pktsize;
	}
	else return 0;
}

void cmdcfg_pckt_read (uint8_t *data, uint32_t size)
{
	PktBuffer_Read(&pckt_cmdcfg_input, (uint8_t *) data);
}

/**
  * @brief audio socket LwIP UDP callback
  */
void udp_rec_callback(void *arg, struct udp_pcb *upcb, struct pbuf *p, const ip_addr_t *addr, u16_t port)
{
	uint8_t *pkt_ptr;

	  received_pcount++;

	  u32_t len = p->len;

	  pkt_ptr = p->payload;

	  ip_addr_t rtp_ip;
	  rtp_ip.addr = rtp_hdr_decode(pkt_ptr);

	  int8_t audiobuf_num = cmanager_get_bufnum(rtp_ip);
	  if (audiobuf_num == -1) return;

	  pkt_ptr = pkt_ptr + RTP_HDR_SIZE;

	  len = len - RTP_HDR_SIZE;

	  int8_t res = audio_write_mix_channel(audiobuf_num, pkt_ptr, len);
	  if (res < 0) CLI_print_lev(CLI_PRINTLEVEL_SVCDEBUG, "Audiobuffer is wrong");

  /* Free the p buffer */
  pbuf_free(p);
}

/**
  * @brief control socket LwIP UDP callback
  */
void udp_cfg_callback(void *arg, struct udp_pcb *upcb, struct pbuf *p, const ip_addr_t *addr, uint16_t port)
{
	uint8_t *pkt_ptr;
	u32_t len;

	len = p->len;
    pkt_ptr = p->payload;

	if ((!memcmp(pkt_ptr, "CMD : ", 6))||(!memcmp(pkt_ptr, "CONFIG : ", 6)))
		PktBuffer_Write(&pckt_cmdcfg_input, pkt_ptr, len);

	/* Free the p buffer */
	pbuf_free(p);
}

void udp_ips_init(uint16_t audio_port, uint16_t ctrl_port, uint16_t config_port)
{
	err_t err;

	udp_cfg_port = config_port;
	udp_audio_port = audio_port;
	udp_ctrl_port = ctrl_port;

	cfg_udp = udp_new();
	audio_udp = udp_new();
	err = udp_bind(cfg_udp, IP_ADDR_ANY, udp_ctrl_port);
	if (err == ERR_OK)
	{
	  /* Set a receive callback for the upcb */
	  udp_recv(cfg_udp, udp_cfg_callback, NULL);
	}
	err = udp_bind(audio_udp, IP_ADDR_ANY, udp_audio_port);
	if (err == ERR_OK)
	{
	  /* Set a receive callback for the upcb */
	  udp_recv(audio_udp, udp_rec_callback, NULL);
	}

	sPDOSettings *sset = sysset_get_settings();
	udp_log_port = sset->network.portlogdoc;
	rtp_log_port = sset->network.portrtpdoc;
	IP_ADDR4(&log_ip, sset->network.ipdoc[0], sset->network.ipdoc[1], sset->network.ipdoc[2], sset->network.ipdoc[3]);
	ip_addr_t addr_int;
	IP_ADDR4(&addr_int, sset->network.ip[0], sset->network.ip[1], sset->network.ip[2], sset->network.ip[3]);
	rtp_hdr_init(&RTP_header, SwapUInt32(addr_int.addr));

	PktBuffer_Init(&pckt_cmdcfg_input, pckt_cmdcfg_input_buffer, CMD_BUFFER_SIZE);
}

void udp_ips_send(void* buffer, size_t len, ip_addr_t ip, uint16_t port)
{
	struct pbuf* udp_buffer = NULL;

	udp_buffer = pbuf_alloc(PBUF_TRANSPORT, len, PBUF_RAM);
	  if (udp_buffer != NULL) {
	    memcpy(udp_buffer->payload, buffer, len);
	    if ((port == udp_audio_port)||(port == rtp_log_port))
	    {
	    	udp_sendto(audio_udp, udp_buffer, &ip, port);
	    } else
	    if (port == udp_ctrl_port)
	    {
	    	udp_sendto(cfg_udp, udp_buffer, &ip, port);
	    }
	    pbuf_free(udp_buffer);
	  }
}

void udp_ips_send_log(void* buffer, size_t len)
{
	struct pbuf* udp_buffer = NULL;

	udp_buffer = pbuf_alloc(PBUF_TRANSPORT, len, PBUF_RAM);
	  if (udp_buffer != NULL) {
	    memcpy(udp_buffer->payload, buffer, len);
	    udp_sendto(cfg_udp, udp_buffer, &log_ip, udp_log_port);
	    pbuf_free(udp_buffer);
	  }
}

void udp_ips_send_rtp(void* buffer, size_t len, ip_addr_t ip)
{
	rtp_hdr_encode(buffer, &RTP_header, len, 0);
	udp_connect(audio_udp, IP_ADDR_ANY, udp_audio_port);
	udp_ips_send(buffer, len, ip, udp_audio_port);
	udp_disconnect(audio_udp);
}

void udp_ips_send_rtp_log(void* buffer, size_t len)
{
	rtp_hdr_encode(buffer, &RTP_header, len, 0);
	udp_connect(audio_udp, IP_ADDR_ANY, rtp_log_port);
	udp_ips_send(buffer, len, log_ip, rtp_log_port);
	udp_disconnect(audio_udp);
}
