/**
  ******************************************************************************
  * @file    connect_manager.h
  * @author  destr0yer@@ETS
  * @brief   This file contains connect manager module header
  ******************************************************************************
  */
#ifndef SRC_CONNECT_MANAGER_H_
#define SRC_CONNECT_MANAGER_H_

#include "lwip.h"

/** @defgroup connect_manager Connect manager module
  * @brief abonent connections manager implementation
  * @{
  */

/// connect description structure type
typedef struct
{
	//ip_addr_t abonent;
	uint8_t *ip;
	uint8_t *CID;
	uint8_t status;
	uint8_t newstatus;
	uint8_t islostcall;
	uint8_t isActive;
	int8_t audiobuffer;
	uint16_t audiobufsize;
	uint8_t priority;
	uint8_t func;
	char pinghex[33];
} cmanager_struct;

/// maximum abonent connections
#define CM_ARRAY_COUNT 48

/// maximum answer data size
#define CM_ANS_MAX_LENGTH		1200

/** @defgroup connect_manager_st abonent connection states
  * @brief abonent connection states codes
  * @{
  */
#define ABONENT_IDLE 	0	// abonent is free
#define ABONENT_SI 		1	// sent INVITE
#define ABONENT_SIGA 	2	// sent INVITE get ACK
#define ABONENT_SIGIA 	3	// sent INVITE get INVITEACK
#define ABONENT_SAGIA 	4	// sent INVITE get INVITEACK sent ACK
#define ABONENT_SB 		6	// sent BYE
#define ABONENT_GB 		7	// get BYE
#define ABONENT_GI 		8	// get INVITE
#define ABONENT_GISIA	9	// get INVITE sent INVITEACK
#define ABONENT_GIGA	10	// get INVITE sent INVITEACK get ACK
#define ABONENT_BA		11	// BYE + ACK
#define ABONENT_SMPLX	12	// SIMPLEX Call
#define ABONENT_SMPLXI	13	// SIMPLEX Input
#define ABONENT_SMPLXO	14	// SIMPLEX Output
#define ABONENT_DPLXA	15	// DUPLEX AUTO
#define ABONENT_DPLXAIO	16	// DUPLEX AUTO
#define ABONENT_BUSY	17	// BUSY
#define ABONENT_BCALL	18	// BUSY Call
#define ABONENT_MISSING	19	// Call Missing abonent
/// @}

/** @defgroup connect_manager_type abonent connection types
  * @brief abonent connection types codes
  * @{
  */
//#define FUNC_SIMPLEX 	3
//#define FUNC_DUPLEX 	4
//#define FUNC_DUPLEXA 	19
#define FUNC_SIMPLEX 	2
#define FUNC_DUPLEX 	3
#define FUNC_DUPLEXA 	4
/// @}

/** @defgroup connect_manager_invtype invite command types
  * @brief invite command types codes
  * @{
  */
#define INVITETYPEDPLX	0
#define INVITETYPEDPLXA 1
#define INVITETYPESMPLX 2
/// @}

/** @defgroup connect_manager_acktype ack answer types
  * @brief ack answer types codes
  * @{
  */
#define ACKTYPEINVITE	1
#define ACKTYPEINVA 	2
#define ACKTYPEBYE	 	3
#define ACKTYPELOG	 	4
/// @}

/** @defgroup connect_manager_ackres ack result
  * @brief ack result codes
  * @{
  */
#define ACKOK		 	1
#define ACKBAD		 	0
/// @}

/**
  * @brief  process SPING command
  * @param  [in] numParam - number of command parameters
  * @param  [in] params - array of pointers to string parameters
  * @retval none
  */
void fnReceivedIPSPING(int numParam, char** params);

/**
  * @brief  process SPONG command
  * @param  [in] numParam - number of command parameters
  * @param  [in] params - array of pointers to string parameters
  * @retval none
  */
void fnReceivedIPSPONG(int numParam, char** params);

/**
  * @brief  process INVITE command
  * @param  [in] numParam - number of command parameters
  * @param  [in] params - array of pointers to string parameters
  * @retval none
  */
void fnReceivedINVITE(int numParam, char** params);

/**
  * @brief  process ACK command
  * @param  [in] numParam - number of command parameters
  * @param  [in] params - array of pointers to string parameters
  * @retval none
  */
void fnReceivedACK(int numParam, char** params);

/**
  * @brief  process INVITEACK command
  * @param  [in] numParam - number of command parameters
  * @param  [in] params - array of pointers to string parameters
  * @retval none
  */
void fnReceivedINVITEACK(int numParam, char** params);

/**
  * @brief  process BYE command
  * @param  [in] numParam - number of command parameters
  * @param  [in] params - array of pointers to string parameters
  * @retval none
  */
void fnReceivedBYE(int numParam, char** params);

/**
  * @brief  process LOG command
  * @param  [in] numParam - number of command parameters
  * @param  [in] params - array of pointers to string parameters
  * @retval none
  */
void fnReceivedLOG(int numParam, char** params);

/**
  * @brief  connect manager initialization
  * @param  [in] abonents_count - actual number of abonents
  * @retval none
  */
void cmanager_init(uint16_t abonents_count);

/**
  * @brief  get current input audio channel for abonent
  * @param  [in] abonent - abonent IP address
  * @retval result - audio mixer channel index (-1 - no channel)
  */
int8_t cmanager_get_bufnum(ip_addr_t abonent);

/**
  * @brief  get new input audio channel for abonent
  * @param  [in] abonent - abonent IP address
  * @retval result - audio mixer channel index (-1 - no free channels)
  */
int8_t cmanager_get_audiobuf(ip_addr_t abonent);

/**
  * @brief  free abonent input audio channel
  * @param  [in] abonent_num - abonent index
  * @retval none
  */
void cmanager_free_audiobuf(uint8_t abonent_num);

/**
  * @brief  search abonent by IP address
  * @param  [in] abonent - abonent IP address
  * @retval result - abonent index (-1 - not found)
  */
int8_t cmanager_get_ip(ip_addr_t abonent);

/**
  * @brief  process received command
  * @param  [in] cmd - command
  * @param  [in] cmdlen - size of cmd
  * @retval none
  */
void cmanager_process_cmd(uint8_t *cmd, uint16_t cmdlen);

/**
  * @brief  process abonent button
  * @param  [in] btn_num - abonent button index
  * @param  [in] btn_st - button change state (up/down)
  * @retval none
  */
void cmanager_process_button(uint8_t btn_num, uint8_t btn_st);

/**
  * @brief  connect manager periodic task
  * @retval none
  */
void cmanager_task();

/**
  * @}
  */

#endif /* SRC_CONNECT_MANAGER_H_ */
