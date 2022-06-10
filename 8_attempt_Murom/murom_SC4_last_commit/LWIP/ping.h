#ifndef LWIP_PING_H
#define LWIP_PING_H

#include "lwip/ip_addr.h"

void ping_init(const ip_addr_t* ping_addr);
void ping_free(void);

void ping_send(void);
uint8_t ping_get_result(void);

#endif /* LWIP_PING_H */
