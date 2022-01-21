/*
 * ips_complan.c
 *
 *  Created on: Jul 6, 2020
 *      Author: Dmitry Donskih
 */

#include "ips_complan.h"
#include "telnet.h"

complan *system_complan = NULL;
sysinfo_t system_info;

int complanStationsQuantity (void)
{
  if (!system_complan)
    {
      return -1;
    }
  return system_complan->size;
}
#define IPS_IP_U32_PRINT(data) printTelnet("%d.%d.%d.%d",(data>>24)&0xFF,(data>>16)&0xFF,(data>>8)&0xFF,(data>>0)&0xFF)

int complanPrintInfo (void)
{
  if (!system_complan)
    {
      printTelnet ("ERROR: Commutation plan doesn't exists.\r\n");
      return -1;
    }
  printTelnet ("INFO: Commutation plan consists of %d defined stations: \r\n",
	       complanStationsQuantity ());
  for (int i = 0; i < complanStationsQuantity (); i++)
    {
      printTelnet ("%d:    CID:%d IP: %d.%d.%d.%d \r\n", i, system_complan->stations[i].CID,
		   (((system_complan->stations[i].IPaddr) >> 24) & 0xFF),
		   (((system_complan->stations[i].IPaddr) >> 16) & 0xFF),
		   (((system_complan->stations[i].IPaddr) >> 8) & 0xFF),
		   (((system_complan->stations[i].IPaddr) >> 0) & 0xFF));
    }
  return 0;
}

int systeminfoPrint (void)
{
  printTelnet ("Hash: ");
  for (int i = 0; i < 16; i++)
    {
      printTelnet ("%x ", system_info.hash[i]);
    }
  printTelnet ("\r\nIP     : ");
  IPS_IP_U32_PRINT(system_info.ipaddr);
  printTelnet ("\r\nMASK   : ");
  IPS_IP_U32_PRINT(system_info.mask);
  printTelnet ("\r\nGATEWAY: ");
  IPS_IP_U32_PRINT(system_info.gateway);
  printTelnet ("\r\nName   : %s\r\n", &(system_info.name));
  return 0;
}
