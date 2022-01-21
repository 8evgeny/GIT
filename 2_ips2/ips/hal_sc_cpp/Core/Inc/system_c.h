/*
 * system.h
 *
 *  Created on: Jun 17, 2020
 *      Author: Donskikh Dmitry
 */

#ifndef INC_SYSTEM_C_H_
#define INC_SYSTEM_C_H_

#include "stdint.h"

#ifdef __cplusplus
extern "C"
{
#endif

#define	DMA_BUFFER_SIZE	32*1024

  void sysLedNormON (void);
  void sysLedNormOFF (void);
  void sysLedNormToggle (void);
  void sysAlarmStart (void);
  void sysAlarmStop (void);

  void sysLedTestON (void);
  void sysLedTestOFF (void);
  void sysLedTestToggle (void);

  void sysLedMicON (void);
  void sysLedMicOFF (void);
  void sysLedMicToggle (void);

  void sysAmpPwrOn (void);
  void sysAmpPwrOff (void);

  void sysDiscreteOutputON (uint16_t output);
  void sysDiscreteOutputOFF (uint16_t output);
  void sysDiscreteOutputToggle (uint16_t output);
  uint8_t sysDiscreteOutputRead (uint16_t output);

  void sysDiscreteOutputAllON (void);
  void sysDiscreteOutputAllOFF (void);

  void sysCheckDiscreteOutputs (void);
//void* allocateDMAbufferStatic(uint32_t size);
//void* getDMAbuffers();

/*
 typedef struct {
 uint32_t CID;									//Caller id of station
 uint32_t IPaddr;								//IP address of station in "internet format"
 }stationIP;

 typedef struct {
 uint16_t 	size;								//number of records
 stationIP	station[size]; 						//array of structures CID,IP (32bit,32bit)
 }routingTable;

 #define CMD_CONF_LOADIPTABLE	0x0101000101 		//control protocol format:
 #define CMD_RESPONSE_ACCEPT		0x0000000001		// Configurator -> CMD_CONF_LOADIPTABLE Size
 // CMD_RESPONSE_ACCEPT <- Device
 // Configurator -> CMD_CONF_LOADIPTABLE routingTable
 // CMD_RESPONSE_ACCEPT <- Device
 */
#ifdef __cplusplus
}
#endif
#endif /* INC_SYSTEM_C_H_ */
