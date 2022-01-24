/*
 * ips_complan.h
 *
 *  Created on: Jul 6, 2020
 *      Author: Dmitry Donskih
 */

#ifndef INC_IPS_COMPLAN_H_
#define INC_IPS_COMPLAN_H_

#include "inttypes.h"
#include "stddef.h"

typedef struct
{
    uint32_t CID;                                   // Caller id of station
    uint32_t IPaddr;                 // IP address of station in "internet format" type QHostAddress
} stationIP;

typedef struct
{
    uint32_t size;
    //uint16_t size2;
    stationIP stations[];
} complan;

typedef struct
{
    uint8_t hash[16];
    uint32_t ipaddr;
    uint32_t mask;
    uint32_t gateway;
    char name[64];
} __attribute__((packed)) sysinfo_t;

int complanStationsQuantity (void);
int complanPrintInfo (void);
int systeminfoPrint (void);

#endif /* INC_IPS_COMPLAN_H_ */
