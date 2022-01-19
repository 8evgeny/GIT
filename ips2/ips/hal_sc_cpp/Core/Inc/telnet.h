/*
 * telnet.h
 *
 *  Created on: Jun 15, 2020
 *      Author: Donskikh Dmitry
 */

#ifndef INC_TELNET_H_
#define INC_TELNET_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "socket.h"

#define TELNET_DEFAULT 23
#define TELNET_UNPRIVILGED 2323
#define TELNET_PORT TELNET_DEFAULT
#define TERM_RES_VT100 80*24

//void createTelnetServer (void);
//void telnetServerTask (void *pvParameters);

void printTelnet(const char *fmt,...);

#ifdef __cplusplus
}
#endif
#endif /* INC_TELNET_H_ */
