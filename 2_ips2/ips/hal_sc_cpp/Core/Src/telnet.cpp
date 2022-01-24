/*
 * telnet.cpp
 *
 *  Created on: 31 авг. 2020 г.
 *      Author: Dmitry Dmitry Donskih
 */

#include "telnet.h"
#include "ips_globals.hpp"
#include "stdarg.h"

#ifdef __cplusplus
extern "C" {
#endif

void printTelnet(const char *fmt,...){
    va_list args;
    va_start(args, fmt);
    ipsSystem::systerm->printfl(fmt,args);
    va_end(args);
}

#ifdef __cplusplus
}
#endif
