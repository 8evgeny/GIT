/**
  ******************************************************************************
  * @file    connect_manager.c
  * @author  destr0yer@@ETS
  * @brief   This file contains connect manager implementation
  ******************************************************************************
  */
#include "CLI_io.h"
#include "CLI_CmdTable.h"
#include "CLI_CmdProcess.h"
#include "audio_process.h"
#include "system_settings.h"
#include "udp_exchange.h"
#include "driver_ui.h"
#include "rtp.h"
#include "string.h"
#include "tone_gen_state_processor.h"
#include "SELC_sound_process.h"
#include "cbuffer.h"
#include "connect_manager.h"

/// connect description structures array
cmanager_struct cm_array[CM_ARRAY_COUNT];

/*
IPSPING : <ip> : <code>
IPSPONG : <ip> :  <code>
INVITE : <ip> : <type> : <priority>
ACK : <ip> : <cmdcode> : <anscode>
INVITEACK : <ip>
BYE : <ip>*/

/** @addtogroup CLI_CmdTable
  * @{
  * @defgroup CLI_CmdTable_cmd CM UDP strings
  * @brief CM UDP control strings
  * @{
  */
const char cmdIPSPING[]="IPSPING";
const char cmdIPSPONG[]="IPSPONG";
const char cmdINVITE[]="INVITE";
const char cmdACK[]="ACK";
const char cmdINVITEACK[]="INVITEACK";
const char cmdBYE[]="BYE";
const char cmdCMD[]="CMD";
const char cmdLOG[]="LOG";
/// @}
/// @}

//debug
//extern CBuffer audio_mix_channels[AUDIO_MAX_MIX_CHANNELS];
//extern CBuffer audio_mic_input;
//extern CBuffer audio_mix_output;
//extern uint8_t audio_mix_channels_state[AUDIO_MAX_MIX_CHANNELS];

/// PDO own IP address
ip_addr_t own_ip;

/// PDKV IP address
ip_addr_t server_ip;

/// SPING command time
uint32_t ping_time;
/// SPING command control data
char pinghex[32];

uint32_t cmd_sendpacket_count; // debug
uint32_t cmd_getpacket_count;  // debug

/// invite state time counter
uint8_t inv_count;
/// busy state time counter
uint8_t busy_count;

/// enable sending log flag
uint8_t isSendingLog;

/// buffer for answer command
uint8_t cm_ans_buf[CM_ANS_MAX_LENGTH];

/// table for SPING data generation
const char randomletter[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";

/**
  * @brief send LOG data for abonent
  * @param  [in] abonent_num - abonent index
  */
void fnSendLOG(int abonent_num)
{
	uint16_t str_len;

	ip_addr_t ab_ip;
	IP_ADDR4(&ab_ip, cm_array[abonent_num].ip[0], cm_array[abonent_num].ip[1],
					 cm_array[abonent_num].ip[2], cm_array[abonent_num].ip[3]);

	str_len = snprintf((char *)&cm_ans_buf[0], CM_ANS_MAX_LENGTH, "%s : %s : %.8lX : %.8lX : %s : %.2X",
			      cmdCMD,
				  cmdLOG,
				  SwapUInt32(own_ip.addr),
				  SwapUInt32(ab_ip.addr),
				  cm_array[abonent_num].CID,
				  cm_array[abonent_num].status);
	udp_ips_send_log(&cm_ans_buf[0], str_len);
	switch (cm_array[abonent_num].status) {
			  case ABONENT_SIGA:
					str_len = snprintf((char *)&cm_ans_buf[0], CM_ANS_MAX_LENGTH, "Caller (Duplex):  %s IP:%.8lX is calling\r\n",
								cm_array[abonent_num].CID,
								SwapUInt32(ab_ip.addr));
				  break;
			  case ABONENT_SIGIA:
					str_len = snprintf((char *)&cm_ans_buf[0], CM_ANS_MAX_LENGTH, "Caller (Duplex):  %s IP:%.8lX is on the line\r\n",
								cm_array[abonent_num].CID,
								SwapUInt32(ab_ip.addr));
				  break;
			  case ABONENT_GI:
					str_len = snprintf((char *)&cm_ans_buf[0], CM_ANS_MAX_LENGTH, "The called party (Duplex):  %s IP:%.8lX is called\r\n",
								cm_array[abonent_num].CID,
								SwapUInt32(ab_ip.addr));
				  break;
			  case ABONENT_GISIA:
					str_len = snprintf((char *)&cm_ans_buf[0], CM_ANS_MAX_LENGTH, "The called party (Duplex):  %s IP:%.8lX is on the line\r\n",
								cm_array[abonent_num].CID,
								SwapUInt32(ab_ip.addr));
				  break;
			  case ABONENT_SMPLXI:
					str_len = snprintf((char *)&cm_ans_buf[0], CM_ANS_MAX_LENGTH, "Caller (Simplex):  %s IP:%.8lX is on the line\r\n",
								cm_array[abonent_num].CID,
								SwapUInt32(ab_ip.addr));
				  break;
			  case ABONENT_SMPLXO:
					str_len = snprintf((char *)&cm_ans_buf[0], CM_ANS_MAX_LENGTH, "The called party (Simplex):  %s IP:%.8lX is on the line\r\n",
								cm_array[abonent_num].CID,
								SwapUInt32(ab_ip.addr));
				  break;
			  case ABONENT_DPLXAIO:
					str_len = snprintf((char *)&cm_ans_buf[0], CM_ANS_MAX_LENGTH, "Caller/The called party (Duplex Auto):  %s IP:%.8lX is on the line\r\n",
								cm_array[abonent_num].CID,
								SwapUInt32(ab_ip.addr));
				  break;
			  case ABONENT_BCALL:
					str_len = snprintf((char *)&cm_ans_buf[0], CM_ANS_MAX_LENGTH, "Caller/The called party:  %s IP:%.8lX is busy\r\n",
								cm_array[abonent_num].CID,
								SwapUInt32(ab_ip.addr));
				  break;
			  case ABONENT_MISSING:
					str_len = snprintf((char *)&cm_ans_buf[0], CM_ANS_MAX_LENGTH, "Caller/The called party:  %s IP:%.8lX is not on the line\r\n",
								cm_array[abonent_num].CID,
								SwapUInt32(ab_ip.addr));
				  break;
			  case ABONENT_SB:
					str_len = snprintf((char *)&cm_ans_buf[0], CM_ANS_MAX_LENGTH, "Caller/The called party:  %s IP:%.8lX the call has been cancelled\r\n",
								cm_array[abonent_num].CID,
								SwapUInt32(ab_ip.addr));
				  break;
			  case ABONENT_GB:
					str_len = snprintf((char *)&cm_ans_buf[0], CM_ANS_MAX_LENGTH, "Caller/The called party:  %s IP:%.8lX call rejected\r\n",
								cm_array[abonent_num].CID,
								SwapUInt32(ab_ip.addr));
				  break;
	}
	CLI_print_lev(CLI_PRINTLEVEL_SERVICE, (char*) &cm_ans_buf[0]);
}

/**
  * @brief send LOG data for all active abonents
  */
void fnSendAllLOG()
{
	uint16_t str_len;

	for (int i = 0; i < CM_ARRAY_COUNT; i++)
	{
		if ((cm_array[i].status == ABONENT_IDLE)||(cm_array[i].status == ABONENT_BUSY)) continue;
		//if ((cm_array[i].status != ABONENT_IDLE)||(cm_array[i].status != ABONENT_BUSY))
		{
			ip_addr_t ab_ip;
			IP_ADDR4(&ab_ip, cm_array[i].ip[0], cm_array[i].ip[1], cm_array[i].ip[2], cm_array[i].ip[3]);
			str_len = snprintf((char *)&cm_ans_buf[0], CM_ANS_MAX_LENGTH, "%s : %s : %.8lX : %.8lX : %s : %.2X",
			      cmdCMD,
				  cmdLOG,
				  SwapUInt32(own_ip.addr),
				  SwapUInt32(ab_ip.addr),
				  cm_array[i].CID,
				  cm_array[i].status);
			udp_ips_send_log(&cm_ans_buf[0], str_len);
			//CLI_print("LOG: %s\r\n", (char*) &cm_ans_buf[0]);
			switch (cm_array[i].status) {
			  case ABONENT_SIGA:
					str_len = snprintf((char *)&cm_ans_buf[0], CM_ANS_MAX_LENGTH, "Caller (Duplex):  %s IP:%.8lX is calling\r\n",
								cm_array[i].CID,
								SwapUInt32(ab_ip.addr));
				  break;
			  case ABONENT_SIGIA:
					str_len = snprintf((char *)&cm_ans_buf[0], CM_ANS_MAX_LENGTH, "Caller (Duplex):  %s IP:%.8lX is on the line\r\n",
								cm_array[i].CID,
								SwapUInt32(ab_ip.addr));
				  break;
			  case ABONENT_GI:
					str_len = snprintf((char *)&cm_ans_buf[0], CM_ANS_MAX_LENGTH, "The called party (Duplex):  %s IP:%.8lX is called\r\n",
								cm_array[i].CID,
								SwapUInt32(ab_ip.addr));
				  break;
			  case ABONENT_GISIA:
					str_len = snprintf((char *)&cm_ans_buf[0], CM_ANS_MAX_LENGTH, "The called party (Duplex):  %s IP:%.8lX is on the line\r\n",
								cm_array[i].CID,
								SwapUInt32(ab_ip.addr));
				  break;
			  case ABONENT_SMPLXI:
					str_len = snprintf((char *)&cm_ans_buf[0], CM_ANS_MAX_LENGTH, "Caller (Simplex):  %s IP:%.8lX is on the line\r\n",
								cm_array[i].CID,
								SwapUInt32(ab_ip.addr));
				  break;
			  case ABONENT_SMPLXO:
					str_len = snprintf((char *)&cm_ans_buf[0], CM_ANS_MAX_LENGTH, "The called party (Simplex):  %s IP:%.8lX is on the line\r\n",
								cm_array[i].CID,
								SwapUInt32(ab_ip.addr));
				  break;
			  case ABONENT_DPLXAIO:
					str_len = snprintf((char *)&cm_ans_buf[0], CM_ANS_MAX_LENGTH, "Caller/The called party (Duplex Auto):  %s IP:%.8lX is on the line\r\n",
								cm_array[i].CID,
								SwapUInt32(ab_ip.addr));
				  break;
			  case ABONENT_BCALL:
					str_len = snprintf((char *)&cm_ans_buf[0], CM_ANS_MAX_LENGTH, "Caller/The called party:  %s IP:%.8lX is busy\r\n",
								cm_array[i].CID,
								SwapUInt32(ab_ip.addr));
				  break;
			  case ABONENT_MISSING:
					str_len = snprintf((char *)&cm_ans_buf[0], CM_ANS_MAX_LENGTH, "Caller/The called party:  %s IP:%.8lX is not on the line\r\n",
								cm_array[i].CID,
								SwapUInt32(ab_ip.addr));
				  break;
			  case ABONENT_SB:
					str_len = snprintf((char *)&cm_ans_buf[0], CM_ANS_MAX_LENGTH, "Caller/The called party:  %s IP:%.8lX the call has been cancelled\r\n",
								cm_array[i].CID,
								SwapUInt32(ab_ip.addr));
				  break;
			  case ABONENT_GB:
					str_len = snprintf((char *)&cm_ans_buf[0], CM_ANS_MAX_LENGTH, "Caller/The called party:  %s IP:%.8lX call rejected\r\n",
								cm_array[i].CID,
								SwapUInt32(ab_ip.addr));
				  break;
			}
			CLI_print_lev(CLI_PRINTLEVEL_SERVICE, (char*) &cm_ans_buf[0]);
		}
	}
}

/**
  * @brief send SPING command to abonent
  * @param  [in] abonent - destination abonent IP address
  * @param  [in] abonent_num - destination abonent index
  */
void fnSendIPSPING(ip_addr_t abonent, int abonent_num)
{
	uint16_t j, str_len;

	char *pstr = &cm_array[abonent_num].pinghex[0];
	for (j = 0; j < 32; j++) pstr[j] = randomletter[random () % 36];
	pstr[32] = '\0';

	str_len = snprintf((char *)&cm_ans_buf[0], CM_ANS_MAX_LENGTH, "%s : %s : %.8lX : %s",
			      cmdCMD, cmdIPSPING, SwapUInt32(own_ip.addr), pstr);
	udp_ips_send(&cm_ans_buf[0], str_len, abonent, UDP_CTRL_PORT);
	//CLI_print("Send: %s\r\n", cm_ans_buf);
}

/**
  * @brief send SPONG answer command to abonent
  * @param  [in] abonent - destination abonent IP address
  * @param  [in] code - received code from abonent
  */
void fnSendIPSPONG(ip_addr_t abonent, char* code)
{
	uint16_t str_len;
	str_len = snprintf((char *)&cm_ans_buf[0], CM_ANS_MAX_LENGTH, "%s : %s : %.8lX : %s",
			      cmdCMD, cmdIPSPONG, SwapUInt32(own_ip.addr), code);
	udp_ips_send(&cm_ans_buf[0], str_len, abonent, UDP_CTRL_PORT);
	//CLI_print("Send: %s\r\n", cm_ans_buf);
}

/**
  * @brief send INVITE command to abonent
  * @param  [in] type - type of call
  * @param  [in] priority - priority of call
  * @param  [in] abonent - destination abonent IP address
  * @param  [in] abonent_num - destination abonent index
  */
void fnSendINVITE(int type, int priority, ip_addr_t abonent, int abonent_num)
{
	uint16_t str_len;
	str_len = snprintf((char *)&cm_ans_buf[0], CM_ANS_MAX_LENGTH, "%s : %s : %.8lX : %.2X : %.2X",
			      cmdCMD, cmdINVITE, SwapUInt32(own_ip.addr), type, priority);

	udp_ips_send(&cm_ans_buf[0], str_len, abonent, UDP_CTRL_PORT);

	if (type == INVITETYPEDPLX)
	{
		cm_array[abonent_num].newstatus = ABONENT_SI;
		selc_set_enable(1);
	} else
	if (type == INVITETYPEDPLXA)
	{
		cm_array[abonent_num].newstatus = ABONENT_DPLXA;
		selc_set_enable(1);
	} else
	if (type == INVITETYPESMPLX)
	{
		cm_array[abonent_num].newstatus = ABONENT_SMPLX;
		selc_set_enable(0);
	}
}

/**
  * @brief send ACK answer command to abonent
  * @param  [in] cmdcode - command code to ACK
  * @param  [in] anscode - answer code for ACK
  * @param  [in] abonent - destination abonent IP address
  */
void fnSendACK(int cmdcode, int anscode, ip_addr_t abonent)
{
	uint16_t str_len;
	str_len = snprintf((char *)&cm_ans_buf[0], CM_ANS_MAX_LENGTH, "%s : %s : %.8lX : %.2X : %.2X",
			      cmdCMD, cmdACK, SwapUInt32(own_ip.addr), cmdcode, anscode);
	udp_ips_send(&cm_ans_buf[0], str_len, abonent, UDP_CTRL_PORT);
}

/**
  * @brief send INVITEACK answer command to abonent
  * @param  [in] abonent - destination abonent IP address
  * @param  [in] abonent_num - destination abonent index
  */
void fnSendINVITEACK(ip_addr_t abonent, int abonent_num)
{
	uint16_t str_len;
	str_len = snprintf((char *)&cm_ans_buf[0], CM_ANS_MAX_LENGTH, "%s : %s : %.8lX",
			      cmdCMD, cmdINVITEACK, SwapUInt32(own_ip.addr));
	udp_ips_send(&cm_ans_buf[0], str_len, abonent, UDP_CTRL_PORT);
	if (cm_array[abonent_num].newstatus == ABONENT_DPLXA)
	{
		cm_array[abonent_num].newstatus = ABONENT_DPLXAIO;
		return;
	} else
	if (cm_array[abonent_num].newstatus == ABONENT_GI)
	{
		cm_array[abonent_num].newstatus = ABONENT_GISIA;
	}

}

/**
  * @brief send BYE command to abonent
  * @param  [in] abonent - destination abonent IP address
  * @param  [in] abonent_num - destination abonent index
  */
void fnSendBYE(ip_addr_t abonent, int abonent_num)
{
	uint16_t str_len;
	str_len = snprintf((char *)&cm_ans_buf[0], CM_ANS_MAX_LENGTH, "%s : %s : %.8lX",
			      cmdCMD, cmdBYE, SwapUInt32(own_ip.addr));
	udp_ips_send(&cm_ans_buf[0], str_len, abonent, UDP_CTRL_PORT);
	cm_array[abonent_num].newstatus = ABONENT_SB;
	selc_set_enable(1);
}

void fnReceivedLOG(int numParam, char** params)
{
	ip_addr_t send_ip;
	uint8_t _ok;
	send_ip.addr = SwapUInt32(Hex2Int(params[0], 4, &_ok));
	if (send_ip.addr != server_ip.addr) return;

	isSendingLog = Hex2Int(params[1], 1, &_ok);

	fnSendACK(ACKTYPELOG, isSendingLog, send_ip);

	if (isSendingLog == 3)
		{
			//CLI_print("LOG: %.2X\r\n", isSendingLog);
			fnSendAllLOG();
			isSendingLog = 0;
		}
}

void fnReceivedIPSPING(int numParam, char** params)
{
	ip_addr_t send_ip;
	uint8_t _ok;
	send_ip.addr = SwapUInt32(Hex2Int(params[0], 4, &_ok));
	fnSendIPSPONG(send_ip, params[1]);
}

void fnReceivedIPSPONG(int numParam, char** params)
{
	ip_addr_t send_ip;
	uint8_t abonent_num, _ok;

	send_ip.addr = SwapUInt32(Hex2Int(params[0], 4, &_ok));
	abonent_num = cmanager_get_ip(send_ip);
	if (abonent_num < 0) return;

	if ((memcmp(cm_array[abonent_num].pinghex, &params[1][0], 32)) == 0)
	{
		cm_array[abonent_num].isActive = 1;
	}
	else return;
	for (int i = 0; i < CM_ARRAY_COUNT; i++)
	{
		ip_addr_t ip;
		IP_ADDR4(&ip, cm_array[i].ip[0], cm_array[i].ip[1], cm_array[i].ip[2], cm_array[i].ip[3]);
		if ((ip.addr == send_ip.addr)&&(i != abonent_num))
			cm_array[i].isActive = 1;
	}
}

void fnReceivedINVITE(int numParam, char** params)
{
	ip_addr_t send_ip;
	uint8_t abonent_num, type, priority, _ok;

	(void)priority; // TODO

	send_ip.addr = SwapUInt32(Hex2Int(params[0], 4, &_ok));
	abonent_num = cmanager_get_ip(send_ip);
	if (abonent_num < 0) return;
	//CLI_print("abonent_num: %d\r\n", abonent_num);  // debug
	type = Hex2Int(params[1], 1, &_ok);
	priority = Hex2Int(params[2], 1, &_ok);
	for (int i = 0; i < CM_ARRAY_COUNT; i++)
	{
		ip_addr_t ip;
		IP_ADDR4(&ip, cm_array[i].ip[0], cm_array[i].ip[1], cm_array[i].ip[2], cm_array[i].ip[3]);
		if ((type == INVITETYPEDPLX)&&(cm_array[i].func == FUNC_DUPLEX)&&(ip.addr == send_ip.addr))
		{
			abonent_num = i;
			break;
		}
		else
		if ((type == INVITETYPEDPLXA)&&(cm_array[i].func == FUNC_DUPLEXA)&&(ip.addr == send_ip.addr))
		{
			abonent_num = i;
			break;
		}
		else
		if ((type == INVITETYPESMPLX)&&(cm_array[i].func == FUNC_SIMPLEX)&&(ip.addr == send_ip.addr))
		{
			abonent_num = i;
			break;
		}
	}
	//CLI_print("INVITE: %.2X %.2X\r\n", type, priority);  // debug

	if (cm_array[abonent_num].status == ABONENT_IDLE) //send ACK
	{
		//CLI_print("ACK_IP: %x\r\n", send_ip);
		if (type == INVITETYPEDPLX)
		{
			fnSendACK(ACKTYPEINVITE, ACKOK, send_ip);
			cm_array[abonent_num].newstatus = ABONENT_GI;
			selc_set_enable(1);
			//CLI_print("Abonent %d newstatus: %d\r\n", abonent_num, cm_array[abonent_num].newstatus);
		} else
		if (type == INVITETYPEDPLXA)
		{
			//fnSendACK(1, 1, send_ip);
			cm_array[abonent_num].newstatus = ABONENT_DPLXA;
			fnSendINVITEACK(send_ip, abonent_num);
			selc_set_enable(1);
		} else
		if (type == INVITETYPESMPLX)
		{
			fnSendACK(ACKTYPEINVITE, ACKOK, send_ip);
			cm_array[abonent_num].newstatus = ABONENT_SMPLXI;
			selc_set_enable(0);
		}
		for (int i = 0; i < CM_ARRAY_COUNT; i++)
		{
			ip_addr_t ip;
			IP_ADDR4(&ip, cm_array[i].ip[0], cm_array[i].ip[1], cm_array[i].ip[2], cm_array[i].ip[3]);
			if (ip.addr == send_ip.addr)
				cm_array[i].islostcall = 0;
		}
	}
	else
	//if (cm_array[abonent_num].status == ABONENT_BUSY)
	{
		fnSendACK(ACKTYPEINVITE, ACKBAD, send_ip);
		cm_array[abonent_num].islostcall = 1;
	}
}

void fnReceivedACK(int numParam, char** params)
{
	ip_addr_t send_ip;
	uint8_t abonent_num, cmdcode, anscode, _ok;
	send_ip.addr = SwapUInt32(Hex2Int(params[0], 4, &_ok));
	abonent_num = cmanager_get_ip(send_ip);
	if (abonent_num < 0) return;
	for (int i = 0; i < CM_ARRAY_COUNT; i++)
	{
		ip_addr_t ip;
		IP_ADDR4(&ip, cm_array[i].ip[0], cm_array[i].ip[1], cm_array[i].ip[2], cm_array[i].ip[3]);
		if (cm_array[i].status == ABONENT_BUSY) continue;
		if ((cm_array[i].status != ABONENT_IDLE)&&(ip.addr == send_ip.addr))
		{
			abonent_num = i;
			break;
		}
	}

	cmdcode = Hex2Int(params[1], 1, &_ok);
	anscode = Hex2Int(params[2], 1, &_ok);
	//CLI_print("ACK: %.2X %.2X\r\n", cmdcode, anscode);
	if (anscode)
	{
		if ((cm_array[abonent_num].status == ABONENT_SMPLX)&&((cmdcode == ACKTYPEINVITE)))
		{
			cm_array[abonent_num].newstatus = ABONENT_SMPLXO;
		} else
		if ((cm_array[abonent_num].status == ABONENT_SI)&&((cmdcode == ACKTYPEINVITE)))
		{
			cm_array[abonent_num].newstatus = ABONENT_SIGA;
		} else
		if ((cm_array[abonent_num].status == ABONENT_GISIA)&&((cmdcode == ACKTYPEINVA)))
		{
			cm_array[abonent_num].newstatus = ABONENT_GIGA;
		} else
		if ((cm_array[abonent_num].status == ABONENT_SIGIA)&&((cmdcode == ACKTYPEINVA)))
		{
			cm_array[abonent_num].newstatus = ABONENT_SAGIA;
		} else
		if ((cm_array[abonent_num].status == ABONENT_DPLXA)&&((cmdcode == ACKTYPEINVA)))
		{
			cm_array[abonent_num].newstatus = ABONENT_DPLXAIO;
		} else
		if ((cm_array[abonent_num].status != ABONENT_IDLE)&&((cmdcode == ACKTYPEBYE)))
		{
			cm_array[abonent_num].newstatus = ABONENT_BA;
		}
	} else
	{
		cm_array[abonent_num].newstatus = ABONENT_BCALL;
	}
	//CLI_print("After ACK abonent %d new status: %d\r\n", abonent_num, cm_array[abonent_num].newstatus);
	//CLI_print("After ACK abonent %d old status: %d\r\n", abonent_num, cm_array[abonent_num].status);
}

void fnReceivedINVITEACK(int numParam, char** params)
{
	ip_addr_t send_ip;
	uint8_t abonent_num, _ok;

	send_ip.addr = SwapUInt32(Hex2Int(params[0], 4, &_ok));
	abonent_num = cmanager_get_ip(send_ip);
	if (abonent_num < 0) return;
	for (int i = 0; i < CM_ARRAY_COUNT; i++)
	{
		ip_addr_t ip;
		IP_ADDR4(&ip, cm_array[i].ip[0], cm_array[i].ip[1], cm_array[i].ip[2], cm_array[i].ip[3]);
		if ((cm_array[i].status != ABONENT_IDLE)&&(cm_array[i].status != ABONENT_BUSY)&&(ip.addr == send_ip.addr))
		{
			abonent_num = i;
			break;
		}
	}
	//CLI_print("INVITEACK: %.2X\r\n", abonent_num);  // debug

	if ((cm_array[abonent_num].status == ABONENT_SI)||(cm_array[abonent_num].status == ABONENT_SIGA)) //send ACK
	{
		fnSendACK(ACKTYPEINVA, ACKOK, send_ip);
		cm_array[abonent_num].newstatus = ABONENT_SIGIA;
	}
	else
	if (cm_array[abonent_num].status == ABONENT_DPLXA)
	{
		fnSendACK(ACKTYPEINVA, ACKOK, send_ip);
		cm_array[abonent_num].newstatus = ABONENT_DPLXAIO;
	}
	else
	{
		fnSendACK(ACKTYPEINVA, ACKBAD, send_ip);
	}
}

void fnReceivedBYE(int numParam, char** params)
{
	ip_addr_t send_ip;
	uint8_t abonent_num, _ok;

	send_ip.addr = SwapUInt32(Hex2Int(params[0], 4, &_ok));
	abonent_num = cmanager_get_ip(send_ip);
	if (abonent_num < 0) return;
	fnSendACK(ACKTYPEBYE, 1, send_ip);
	for (int i = 0; i < CM_ARRAY_COUNT; i++)
	{
		if ((cm_array[i].status !=ABONENT_IDLE)&&(cm_array[i].status !=ABONENT_BUSY))
		{
			ip_addr_t ip;
			IP_ADDR4(&ip, cm_array[i].ip[0], cm_array[i].ip[1], cm_array[i].ip[2], cm_array[i].ip[3]);
			if (ip.addr == send_ip.addr)
			{
				cm_array[i].newstatus = ABONENT_BA;
				if (cm_array[i].status ==ABONENT_GI)
				{
					cm_array[i].islostcall = 1;
				}
			}
		}
	}
	if ((cm_array[abonent_num].status !=ABONENT_IDLE)&&(cm_array[abonent_num].status !=ABONENT_BUSY))
	{
		cm_array[abonent_num].newstatus = ABONENT_BA;
		if (cm_array[abonent_num].status ==ABONENT_GI)
		{
			cm_array[abonent_num].islostcall = 1;
		}
	}
}


void cmanager_init(uint16_t abonents_count)
{
	sPDOSettings *sset = sysset_get_settings();
	IP_ADDR4(&own_ip, sset->network.ip[0], sset->network.ip[1], sset->network.ip[2], sset->network.ip[3]);
	IP_ADDR4(&server_ip, sset->network.ipdoc[0], sset->network.ipdoc[1], sset->network.ipdoc[2], sset->network.ipdoc[3]);
	for (int i = 0; i < CM_ARRAY_COUNT; i++)
	{
		cm_array[i].ip = sset->buttons[i].ip;
		cm_array[i].CID = sset->buttons[i].CID;
		cm_array[i].audiobuffer = -1;
		cm_array[i].audiobufsize = AUDIO_IO_BUFFER_SIZE;
		cm_array[i].status = ABONENT_IDLE;
		cm_array[i].newstatus = ABONENT_IDLE;
		cm_array[i].islostcall = 0;
		cm_array[i].isActive = 0;
		cm_array[i].priority = sset->buttons[i].priority;
		cm_array[i].func = sset->buttons[i].func;
		ip_addr_t ab_ip;
		IP_ADDR4(&ab_ip, cm_array[i].ip[0], cm_array[i].ip[1], cm_array[i].ip[2], cm_array[i].ip[3]);
		fnSendIPSPING(ab_ip, i);
	}
	ping_time = HAL_GetTick();
	cmd_sendpacket_count = 0;
	cmd_getpacket_count = 0;
	inv_count = 0;
	busy_count = 0;
	isSendingLog = 0;
}

int8_t cmanager_get_audiobuf(ip_addr_t abonent)
{
	int8_t abonent_num = cmanager_get_ip(abonent);
	if (abonent_num == -1) return -1;
	cm_array[abonent_num].audiobuffer = audio_get_mix_channel();
	audio_reset_mix_channel(cm_array[abonent_num].audiobuffer);
	audio_play_mix_channel(cm_array[abonent_num].audiobuffer);
	return cm_array[abonent_num].audiobuffer;
}

int8_t cmanager_get_bufnum(ip_addr_t abonent)
{
	ip_addr_t ip;
	for (int i = 0; i < CM_ARRAY_COUNT; i++)
	{
		IP_ADDR4(&ip, cm_array[i].ip[0], cm_array[i].ip[1], cm_array[i].ip[2], cm_array[i].ip[3]);
		if (ip.addr == abonent.addr)
		{
			return cm_array[i].audiobuffer;
		}
	}

	return -1;
}

void cmanager_free_audiobuf(uint8_t abonent_num)
{
	if (cm_array[abonent_num].audiobuffer >= 0) audio_free_mix_channel(cm_array[abonent_num].audiobuffer);
}

int8_t cmanager_get_ip(ip_addr_t abonent)
{
	for (int i = 0; i < CM_ARRAY_COUNT; i++)
	{
		ip_addr_t ip;
		IP_ADDR4(&ip, cm_array[i].ip[0], cm_array[i].ip[1], cm_array[i].ip[2], cm_array[i].ip[3]);
		if (ip.addr == abonent.addr)
		{
			return i;
		}
	}
	return -1;
}

void cmanager_process_cmd(uint8_t *cmd, uint16_t cmdlen)
{
	uint8_t parNum, calculate_parNum;
	uint8_t _ok;

	const sCLI_cmd_desc* index_cmd = CLI_searchCMD((char *)cmd);
	if (index_cmd != NULL)
	{
		cmd_getpacket_count++;
		parNum = (*index_cmd).par_num;
		calculate_parNum = CLI_getParNumMsg((char *)cmd);

		if (calculate_parNum < parNum) {
			//CLI_print("Not enough param!\r\n"); // debug
			return;
		}
		CLI_parse_msg((char *)cmd);
		ip_addr_t ab_ip;
		ab_ip.addr = SwapUInt32(Hex2Int(arrPSTR[1], 4, &_ok));
		int8_t abonent_num = cmanager_get_ip(ab_ip);
		if (abonent_num < 0) return;
		if (!cm_array[abonent_num].func) return;
		(*index_cmd).func(calculate_parNum, &arrPSTR[1]);
	}
}

void cmanager_process_button(uint8_t btn_num, uint8_t btn_st)
{
	ip_addr_t ip_btn;
	IP_ADDR4(&ip_btn, cm_array[btn_num].ip[0], cm_array[btn_num].ip[1], cm_array[btn_num].ip[2], cm_array[btn_num].ip[3]);
	if ((ip_btn.addr==0)||(ip_btn.addr == own_ip.addr)) return;
	if (!cm_array[btn_num].func) return;

	if ((btn_st == BTN_VAL_DOWN)&&(cm_array[btn_num].islostcall))
	{
		return;
	}

	if (cm_array[btn_num].islostcall)
	{
		for (int i = 0; i < CM_ARRAY_COUNT; i++)
		{
			if (i == btn_num) continue;
			ip_addr_t ip;
			IP_ADDR4(&ip, cm_array[i].ip[0], cm_array[i].ip[1], cm_array[i].ip[2], cm_array[i].ip[3]);
			if (ip.addr == ip_btn.addr)
			{
				cm_array[i].islostcall = 0;
				ui_setledstate(LED_AB1R + i*2, LED_STATE_OFF);
			}
		}
		cm_array[btn_num].islostcall = 0;
		ui_setledstate(LED_AB1R + btn_num*2, LED_STATE_OFF);
		return;
	}

	if (cm_array[btn_num].status == ABONENT_BUSY) // if another abonent was called
	{
		return;
	}

	if (btn_st == BTN_VAL_UP)
	{
		if ((cm_array[btn_num].func == FUNC_SIMPLEX)&&(cm_array[btn_num].status == ABONENT_SMPLXO))
		{
			fnSendBYE(ip_btn, btn_num);
			return;
		}

		if ((!cm_array[btn_num].isActive)&&(cm_array[btn_num].status == ABONENT_IDLE))
		{
			if (cm_array[btn_num].func == FUNC_DUPLEX)
				cm_array[btn_num].newstatus = ABONENT_MISSING;
			else
			if (cm_array[btn_num].func == FUNC_DUPLEXA)
			{
				cm_array[btn_num].newstatus = ABONENT_MISSING;
				//CLI_print("Abonent N:%d is inactive\r\n", btn_num);
			}
			return;
		}
		if (cm_array[btn_num].status == ABONENT_IDLE) // send INVITE
		{
			for (int i = 0; i < CM_ARRAY_COUNT; i++)
			{
				if (i == btn_num) continue;
				ip_addr_t ip;
				IP_ADDR4(&ip, cm_array[i].ip[0], cm_array[i].ip[1], cm_array[i].ip[2], cm_array[i].ip[3]);
				if (ip.addr == ip_btn.addr)
				{
					cm_array[i].islostcall = 0;
				}
			}
			if (cm_array[btn_num].func == FUNC_DUPLEX)
			{
				fnSendINVITE(INVITETYPEDPLX, cm_array[btn_num].priority, ip_btn, btn_num);
			} else
			if (cm_array[btn_num].func == FUNC_DUPLEXA)
			{
				fnSendINVITE(INVITETYPEDPLXA, cm_array[btn_num].priority, ip_btn, btn_num);
			}
		} else
		if (cm_array[btn_num].status == ABONENT_MISSING)
		{
			//
		} else
		if (cm_array[btn_num].status == ABONENT_GI)
		{
			fnSendINVITEACK(ip_btn, btn_num);
		} else
		if ((cm_array[btn_num].status != ABONENT_IDLE)&&(cm_array[btn_num].status != ABONENT_SMPLXI)) // send BYE
		{
			fnSendBYE(ip_btn, btn_num);
		} else
		if (cm_array[btn_num].status == ABONENT_DPLXA)
		{
			fnSendBYE(ip_btn, btn_num);
		}
	} else
	if (btn_st == BTN_VAL_DOWN)
	{
		if ((!cm_array[btn_num].isActive)&&(cm_array[btn_num].status == ABONENT_IDLE)&&(cm_array[btn_num].func == FUNC_SIMPLEX))
		{
			cm_array[btn_num].newstatus = ABONENT_MISSING;
			return;
		}

		if (cm_array[btn_num].status == ABONENT_IDLE)
			if (cm_array[btn_num].func == FUNC_SIMPLEX)
			{
				for (int i = 0; i < CM_ARRAY_COUNT; i++)
				{
					if (i == btn_num) continue;
					ip_addr_t ip;
					IP_ADDR4(&ip, cm_array[i].ip[0], cm_array[i].ip[1], cm_array[i].ip[2], cm_array[i].ip[3]);
					if (ip.addr == ip_btn.addr)
					{
						cm_array[i].islostcall = 0;
					}
				}
				fnSendINVITE(INVITETYPESMPLX, cm_array[btn_num].priority, ip_btn, btn_num);
			}
	}
}

void cmanager_task()
{
	for (int i = 0; i < CM_ARRAY_COUNT; i++)
	{
		ip_addr_t ab_ip;
		IP_ADDR4(&ab_ip, cm_array[i].ip[0], cm_array[i].ip[1], cm_array[i].ip[2], cm_array[i].ip[3]);

		if (cm_array[i].status != cm_array[i].newstatus)
		{
			//CLI_print("Abonent %d newstatus: %d\r\n", i, cm_array[i].newstatus);
			if (cm_array[i].newstatus == ABONENT_MISSING)
			{
				ui_setledstate(LED_AB1G + i*2, LED_STATE_OFF);
				ui_setledstate(LED_AB1R + i*2, LED_STATE_ON);
				cm_array[i].status = cm_array[i].newstatus;
				if (isSendingLog == 1) fnSendLOG(i);
				tone_genPlayTone(SIG_GEN_UNAVAIL, 1000);	// sound MISSING
			} else
			if (cm_array[i].newstatus == ABONENT_BUSY)
			{
				cm_array[i].status = cm_array[i].newstatus;
			} else
			if (cm_array[i].newstatus == ABONENT_GI)
			{
				ui_setledstate(LED_AB1G + i*2, LED_STATE_OFF);
				ui_setledstate(LED_AB1R + i*2, LED_STATE_FL1);
				cm_array[i].status = cm_array[i].newstatus;
				if (isSendingLog == 1) fnSendLOG(i);
				for (int j = 0; j < CM_ARRAY_COUNT; j++)
					{
						if (i == j) continue;
						cm_array[j].newstatus = ABONENT_BUSY;
					}
				tone_genPlayTone(SIG_GEN_CALL, 0); // sound of calling ON
				cm_array[i].islostcall = 0;
			} else
			if (cm_array[i].newstatus == ABONENT_SIGA)
			{
				ui_setledstate(LED_AB1G + i*2, LED_STATE_OFF);
				ui_setledstate(LED_AB1R + i*2, LED_STATE_ON);
				cm_array[i].status = cm_array[i].newstatus;
				if (isSendingLog == 1) fnSendLOG(i);
				for (int j = 0; j < CM_ARRAY_COUNT; j++)
					{
						if (i == j) continue;
						cm_array[j].newstatus = ABONENT_BUSY;
					}
				tone_genPlayTone(SIG_GEN_SENDCTRL, 0); // sound of dialing ON
				cm_array[i].islostcall = 0;
			} else
			if (cm_array[i].newstatus == ABONENT_IDLE)
			{
				ui_setledstate(LED_AB1G + i*2, LED_STATE_OFF);
				ui_setledstate(LED_AB1R + i*2, LED_STATE_OFF);
				cm_array[i].status = cm_array[i].newstatus;
				if (isSendingLog == 1) fnSendLOG(i);
			} else
			if (cm_array[i].newstatus == ABONENT_SI)
			{
				ui_setledstate(LED_AB1G + i*2, LED_STATE_OFF);
				ui_setledstate(LED_AB1R + i*2, LED_STATE_ON);
				cm_array[i].status = cm_array[i].newstatus;
				if (isSendingLog == 1) fnSendLOG(i);
				//CLI_print("Abonent %d status: %d\r\n", i, cm_array[i].status);
			} else
			if ((cm_array[i].newstatus == ABONENT_SIGIA)||(cm_array[i].newstatus == ABONENT_SAGIA))
			{
				ui_setledstate(LED_AB1G + i*2, LED_STATE_ON);
				ui_setledstate(LED_AB1R + i*2, LED_STATE_OFF);
				ui_setledstate(LED_MKVKL, LED_STATE_ON);
				cmanager_get_audiobuf(ab_ip);
				cm_array[i].status = cm_array[i].newstatus;
				if (isSendingLog == 1) fnSendLOG(i);
				tone_genPlayTone(SIG_GEN_OFF, 0); // sound of dialing OFF
			} else
			if ((cm_array[i].newstatus == ABONENT_GISIA)||(cm_array[i].newstatus == ABONENT_GIGA))
			{
				ui_setledstate(LED_AB1G + i*2, LED_STATE_ON);
				ui_setledstate(LED_AB1R + i*2, LED_STATE_OFF);
				cmanager_get_audiobuf(ab_ip);
				ui_setledstate(LED_MKVKL, LED_STATE_ON);
				cm_array[i].status = cm_array[i].newstatus;
				if (isSendingLog == 1) fnSendLOG(i);
				for (int j = 0; j < CM_ARRAY_COUNT; j++)
					{
						if (i == j) continue;
						cm_array[j].newstatus = ABONENT_BUSY;
					}
				tone_genPlayTone(SIG_GEN_OFF, 0); // sound of calling OFF
			} else
			if (cm_array[i].newstatus == ABONENT_DPLXA)
			{
				ui_setledstate(LED_AB1G + i*2, LED_STATE_ON);
				ui_setledstate(LED_AB1R + i*2, LED_STATE_OFF);
				cm_array[i].status = cm_array[i].newstatus;
				if (isSendingLog == 1) fnSendLOG(i);
				for (int j = 0; j < CM_ARRAY_COUNT; j++)
					{
						if (i == j) continue;
						cm_array[j].newstatus = ABONENT_BUSY;
					}
				cm_array[i].islostcall = 0;
			} else
			if (cm_array[i].newstatus == ABONENT_DPLXAIO)
			{
				ui_setledstate(LED_AB1G + i*2, LED_STATE_ON);
				ui_setledstate(LED_AB1R + i*2, LED_STATE_OFF);
				ui_setledstate(LED_MKVKL, LED_STATE_ON);
				cmanager_get_audiobuf(ab_ip);
				cm_array[i].status = cm_array[i].newstatus;
				if (isSendingLog == 1) fnSendLOG(i);
				for (int j = 0; j < CM_ARRAY_COUNT; j++)
					{
						if (i == j) continue;
						cm_array[j].newstatus = ABONENT_BUSY;
					}
				cm_array[i].islostcall = 0;
			} else
			if ((cm_array[i].newstatus >= ABONENT_SB)&&(cm_array[i].newstatus <= ABONENT_GB))
			{
				ui_setledstate(LED_AB1G + i*2, LED_STATE_OFF);
				ui_setledstate(LED_AB1R + i*2, LED_STATE_OFF);
				ui_setledstate(LED_MKVKL, LED_STATE_OFF);
				cm_array[i].newstatus = ABONENT_IDLE;
				cmanager_free_audiobuf(i);
				cm_array[i].status = cm_array[i].newstatus;
				if (isSendingLog == 1) fnSendLOG(i);
				for (int j = 0; j < CM_ARRAY_COUNT; j++)
					{
						if (i == j) continue;
						cm_array[j].newstatus = ABONENT_IDLE;
					}
				tone_genPlayTone(SIG_GEN_OFF, 0); // all sounds OFF
			} else
			if (cm_array[i].newstatus == ABONENT_BA)
			{
				ui_setledstate(LED_AB1G + i*2, LED_STATE_OFF);
				ui_setledstate(LED_AB1R + i*2, LED_STATE_OFF);
				ui_setledstate(LED_MKVKL, LED_STATE_OFF);
				cm_array[i].newstatus = ABONENT_IDLE;
				cmanager_free_audiobuf(i);
				cm_array[i].status = cm_array[i].newstatus;
				if (isSendingLog == 1) fnSendLOG(i);
				for (int j = 0; j < CM_ARRAY_COUNT; j++)
					{
						if (i == j) continue;
						cm_array[j].newstatus = ABONENT_IDLE;
					}
				tone_genPlayTone(SIG_GEN_OFF, 0); // all sounds OFF
			} else
			if (cm_array[i].newstatus == ABONENT_SMPLX)
			{
				cm_array[i].status = cm_array[i].newstatus;
				if (isSendingLog == 1) fnSendLOG(i);
				cm_array[i].islostcall = 0;
			} else
			if (cm_array[i].newstatus == ABONENT_SMPLXI)
			{
				ui_setledstate(LED_AB1G + i*2, LED_STATE_FL1);
				ui_setledstate(LED_AB1R + i*2, LED_STATE_OFF);
				cmanager_get_audiobuf(ab_ip);
				cm_array[i].status = cm_array[i].newstatus;
				if (isSendingLog == 1) fnSendLOG(i);
				for (int j = 0; j < CM_ARRAY_COUNT; j++)
					{
						if (i == j) continue;
						cm_array[j].newstatus = ABONENT_BUSY;
					}
				cm_array[i].islostcall = 0;
			} else
			if (cm_array[i].newstatus == ABONENT_SMPLXO)
			{
				ui_setledstate(LED_AB1G + i*2, LED_STATE_ON);
				ui_setledstate(LED_AB1R + i*2, LED_STATE_OFF);
				ui_setledstate(LED_MKVKL, LED_STATE_ON);
				cm_array[i].status = cm_array[i].newstatus;
				if (isSendingLog == 1) fnSendLOG(i);
				for (int j = 0; j < CM_ARRAY_COUNT; j++)
					{
						if (i == j) continue;
						cm_array[j].newstatus = ABONENT_BUSY;
					}
				cm_array[i].islostcall = 0;
			} else
			if (cm_array[i].newstatus == ABONENT_BCALL)
			{
				ui_setledstate(LED_AB1G + i*2, LED_STATE_OFF);
				ui_setledstate(LED_AB1R + i*2, LED_STATE_ON);
				cm_array[i].status = cm_array[i].newstatus;
				if (isSendingLog == 1) fnSendLOG(i);
				busy_count++;
				for (int j = 0; j < CM_ARRAY_COUNT; j++)
					{
						if (i == j) continue;
						cm_array[j].newstatus = ABONENT_IDLE;
					}
				tone_genPlayTone(SIG_GEN_BUSY, 1000);	// sound BUSY
			}
		}
		if (cm_array[i].islostcall)
		{
			if (ui_getledstate(LED_AB1R + i*2) != LED_STATE_FL3) {
				ui_setledstate(LED_AB1R + i*2, LED_STATE_FL3);
			}
		}
		else
		{
			if (ui_getledstate(LED_AB1R + i*2) == LED_STATE_FL3) {
				ui_setledstate(LED_AB1R + i*2, LED_STATE_OFF);
			}
		}
	}

	if ((HAL_GetTick() - ping_time) > 1000)
	{

			for (int i = 0; i < CM_ARRAY_COUNT; i++)
			{
				if (cm_array[i].newstatus != ABONENT_IDLE)
				{
					//if (cm_array[i].newstatus != ABONENT_BUSY) CLI_print("Abonent %d newstatus: %d\r\n", i, cm_array[i].newstatus);
				}
				if (((cm_array[i].status == ABONENT_SI)||(cm_array[i].status == ABONENT_DPLXA))&&(inv_count > 4))
				{
					cm_array[i].newstatus = ABONENT_IDLE;
					inv_count = 0;
				} else
				if ((cm_array[i].status == ABONENT_SI)||(cm_array[i].status == ABONENT_DPLXA))
				{
					inv_count++;
				}


				if ((cm_array[i].status == ABONENT_BCALL)&&(busy_count > 1))
				{
					cm_array[i].newstatus = ABONENT_IDLE;
					busy_count = 0;
				} else
				if (cm_array[i].status == ABONENT_BCALL)
				{
					busy_count++;
				}

				if ((cm_array[i].status == ABONENT_MISSING)&&(cm_array[i].newstatus == ABONENT_MISSING))
				{
					ui_setledstate(LED_AB1G + i*2, LED_STATE_OFF);
					ui_setledstate(LED_AB1R + i*2, LED_STATE_OFF);
					cm_array[i].newstatus = ABONENT_IDLE;
				}

				//if(!cm_array[i].isActive)
				{
					//Abonent is inactive
					ip_addr_t ab_ip;
					IP_ADDR4(&ab_ip, cm_array[i].ip[0], cm_array[i].ip[1], cm_array[i].ip[2], cm_array[i].ip[3]);
					if ((ab_ip.addr==0)||(ab_ip.addr == own_ip.addr)) continue;
					fnSendIPSPING(ab_ip, i);
				}
				//else
				if(cm_array[i].isActive)
				{
					//CLI_print("Abonent N:%d is active\r\n", i);
					cm_array[i].isActive++;
					if (cm_array[i].isActive == 3) cm_array[i].isActive = 0;
				}
			}
			ping_time = HAL_GetTick();

	}
}
