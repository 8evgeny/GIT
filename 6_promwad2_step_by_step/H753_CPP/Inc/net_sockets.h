#ifndef NET_SOCKETS_H
#define NET_SOCKETS_H

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

int32_t netInit(const char *ipSet, const char *netmaskSet, const char *gatwaySet);

#ifdef __cplusplus
}
#endif


#endif /* net_sockets.h */
