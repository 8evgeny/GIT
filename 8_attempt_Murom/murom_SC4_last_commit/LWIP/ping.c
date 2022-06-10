/**
 * @file
 * Ping sender module
 *
 */
#include "lwip/opt.h"

#if LWIP_RAW /* don't build if not configured for use in lwipopts.h */

#include "lwip/mem.h"
#include "lwip/raw.h"
#include "lwip/icmp.h"
#include "lwip/netif.h"
#include "lwip/sys.h"
#include "lwip/timeouts.h"
#include "lwip/inet_chksum.h"
#include "lwip/prot/ip4.h"

#include "ping.h"

/** ping identifier - must fit on a u16_t */
#define PING_ID        0xAADB

/** ping additional data size to include in the packet */
#define PING_DATA_SIZE 32

/** ping result action - no default action */
uint32_t PING_RESULT;

/* ping variables */
static const ip_addr_t* ping_target;
static u16_t ping_seq_num;
static struct raw_pcb *ping_pcb;

/** Prepare a echo ICMP request */
static void
ping_prepare_echo( struct icmp_echo_hdr *iecho, u16_t len)
{
  size_t i;
  size_t data_len = len - sizeof(struct icmp_echo_hdr);

  ICMPH_TYPE_SET(iecho, ICMP_ECHO);
  ICMPH_CODE_SET(iecho, 0);
  iecho->chksum = 0;
  iecho->id     = PING_ID;
  iecho->seqno  = lwip_htons(++ping_seq_num);

  /* fill the additional data buffer with some data */
  for(i = 0; i < data_len; i++) {
    ((char*)iecho)[sizeof(struct icmp_echo_hdr) + i] = (char)i;
  }

  iecho->chksum = inet_chksum(iecho, len);
}

/* Ping using the raw ip */
static u8_t
ping_recv(void *arg, struct raw_pcb *pcb, struct pbuf *p, const ip_addr_t *addr)
{
  struct icmp_echo_hdr *iecho;
  LWIP_UNUSED_ARG(arg);
  LWIP_UNUSED_ARG(pcb);
  LWIP_UNUSED_ARG(addr);
  LWIP_ASSERT("p != NULL", p != NULL);

  if (p->tot_len >= (PBUF_IP_HLEN + sizeof(struct icmp_echo_hdr))) {
    iecho = (struct icmp_echo_hdr *)((uint8_t *)p->payload + PBUF_IP_HLEN);

    if ((iecho->id == PING_ID) && (iecho->seqno == lwip_htons(ping_seq_num))) {
      /* do some ping result processing */
      PING_RESULT = 1;
      pbuf_free(p);
      return 1; /* eat the packet */
     }
  }

  return 0; /* don't eat the packet */
}

void ping_send(void)
{
	struct pbuf *p;
	struct icmp_echo_hdr *iecho;

	size_t ping_size = sizeof(struct icmp_echo_hdr) + PING_DATA_SIZE;

	PING_RESULT = 0;

	p = pbuf_alloc(PBUF_IP, (u16_t)ping_size, PBUF_RAM);
	if (!p) return;

	if ((p->len == p->tot_len) && (p->next == NULL)) {
	    iecho = (struct icmp_echo_hdr *)p->payload;

	    ping_prepare_echo(iecho, (u16_t)ping_size);

	    raw_sendto(ping_pcb, p, ping_target);
	}
	pbuf_free(p);
}

void ping_init(const ip_addr_t* ping_addr)
{
  ping_target = ping_addr;
  ping_pcb = raw_new(IP_PROTO_ICMP);

  raw_recv(ping_pcb, ping_recv, NULL);
  raw_bind(ping_pcb, IP_ADDR_ANY);
}

void ping_free(void)
{
  raw_remove(ping_pcb);
}

uint8_t ping_get_result(void)
{
  return PING_RESULT;
}

#endif /* LWIP_RAW */
