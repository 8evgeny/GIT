/*
 * fttcpproto.h
 *
 *  Created on: 28 июн. 2020 г.
 *      Author: Dmitry Dmitry Donskih
 */

#ifndef INC_FTTCPPROTO_H_
#define INC_FTTCPPROTO_H_
#ifdef __cplusplus
extern "C"
{
#endif
//#include "system.h"
#include "inttypes.h"
//#include "socket.h"

//typedef unsigned int uint32_t;
//typedef unsigned char uint8_t;

#define FTRANSFER_DEFAULT 2020U
#define FTRANSFER_UNPRIVILGED 2020U
#define FTRANSFER_PORT FTRANSFER_DEFAULT

#define FTRANSFER_CMD_FILESEND		0x01000101	//SendFile(uint32_t file_size,uint8_t drive,uint32_t chunk_size,char name[MAXFILENAME])
#define FTRANSFER_CMD_DATASEND		0x01000102	//File transfer start syncronization command.
#define FTRANSFER_CMD_EOT		0x010001FF	//Wait for Accept(uint32_t size)
#define RESPONSE_ACCEPT			0x0F0F0001	//After that client send data with
#define RESPONSE_ER_FILEXISTS		0xFFFF0101	//DataSend(uint32_t chunk_size,uint8_t chunk[chunk_size])
#define RESPONSE_ER_DRIVEFULL		0xFFFF0102	//with accepting every chunk transfer
#define RESPONSE_ER_WRONGDRIVE		0xFFFF0103	//Connection clothes by server after recieve EOT() command

#define RESPONSE_ER_SIZEMISMATCH	0xFFFF0201	//Mismatch of recieved data type (ex. if server waits for uint8_t[8] and get uint16_t[8] or uint8_t[9])
#define RESPONSE_ERROR			0xFFFFFFFF

#define IPS_BSP_CONF_CMD_SYNC               0x01010200
#define IPS_BSP_CONF_CMD_LOADIPTABLE        0x01010201
#define IPS_BSP_CONF_CMD_LOADSYSINFO        0x01010202
#define IPS_BSP_CONF_CMD_LOADKEYS           0x01010203
#define IPS_BSP_CONF_CMD_LOADCONTROLS       0x01010204
#define IPS_BSP_CONF_CMD_DATASEND           0x01000002
#define IPS_BSP_CONF_CMD_EOT                0x010000FF

#define FSDEVICE_EEPROM				0x01
#define FSDEVICE_INTFLASH			0x02
#define FSDEVICE_SRAM				0x03
#define FSDEVICE_SDMMC				0x04

  void createFTransferServer (void);
  void ftransferServerTask (void *pvParameters);

#ifdef __cplusplus
}
#endif
#endif /* INC_FTTCPPROTO_H_ */
