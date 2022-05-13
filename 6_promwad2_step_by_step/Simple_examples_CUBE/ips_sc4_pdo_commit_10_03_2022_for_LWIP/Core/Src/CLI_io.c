/**
  ******************************************************************************
  * @file    CLI_io.c
  * @author  dmib@@ETS
  * @brief   This file contains CLI input/output function implementation
  ******************************************************************************
  */
#include "stdio.h"
#include "string.h"
#include "stdarg.h"
#include "system_settings.h"
#include "CLI_CmdProcess.h"
#include "driver_EEPROM.h"
#include "AES_codec.h"
#include "CLI_io.h"

/// enable LWIP support
#define USE_LWIP
/// enable AES encrypting support
#define USE_AES

#ifdef USE_LWIP
#include "lwip.h"
#include "udp_exchange.h"
#endif

/// link to UART7 hardware module control/status structure
extern UART_HandleTypeDef huart7;

#ifdef __GNUC__
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#define GETCHAR_PROTOTYPE int __io_getchar(void)
#else
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#define GETCHAR_PROTOTYPE int fgetc(FILE *f)
#endif /* __GNUC__ */

/// maximum CLI string length
#define CLI_MAX_STR_LENGTH 1400
/// maximum UART buffer length
#define CLI_MAX_UARTBUF_LENGTH 256

/// CLI output string buffer
uint8_t CLI_OutBuffer[CLI_MAX_STR_LENGTH];
/// CLI input string buffer
uint8_t CLI_InBuffer[CLI_MAX_STR_LENGTH];
/// CLI input string buffer byte counter
uint16_t CLI_InBufferCnt;

/// CLI UART RX buffer
uint8_t CLI_RxBuffer[CLI_MAX_STR_LENGTH];
/// CLI UART RX buffer byte counter
uint16_t CLI_RxBufferCnt;

/// CLI UART RX interrupt buffer
uint8_t CLI_UARTRxBuffer[CLI_MAX_UARTBUF_LENGTH];

/// CLI current print level
uint8_t CLI_PrintLevel;

/// CLI UART put char
PUTCHAR_PROTOTYPE
{
  if (HAL_UART_Transmit(&huart7, (uint8_t *)&ch, 1, 0xFFFF)==HAL_OK) return ch;
  else return EOF;
}

/// CLI UART get char
GETCHAR_PROTOTYPE
{
  char ch;
  
  if (HAL_UART_Receive(&huart7, (uint8_t *)&ch, 1, 0xFFFF)==HAL_OK) return ch;
  else return EOF;
}

/// current input UDP socket
void *eth_socket;
/// current CLI answer mode (0 - UART/ 1 - Ethernet)
uint8_t eth_print;

#ifdef USE_LWIP
/// current CLI Ethernet answer IP address
ip_addr_t eth_sender_ip;
#endif

void CLI_init()
{
  eth_socket = NULL;
  eth_print = 0;
  CLI_RxBufferCnt = 0;
  CLI_InBufferCnt = 0;  
  CLI_PrintLevel = CLI_PRINTLEVEL_WORK;
}

void CLI_setPrintLevel(uint8_t lev)
{
   CLI_PrintLevel = lev;
}

void CLI_print_lev(uint8_t lev, char *str, ...){
  uint32_t len;

  if (lev > CLI_PrintLevel) return;

  va_list arg_ptr;
  va_start(arg_ptr, str);
  len = vsnprintf((char *)&CLI_OutBuffer[0], CLI_MAX_STR_LENGTH, str, arg_ptr);
  va_end(arg_ptr);
  HAL_UART_Transmit(&huart7, (uint8_t *)&CLI_OutBuffer[0], len, 0xFFFF);
}

// 012345678901234567890
// CONFIG : XXXXXXXX : <CMD>
#define IP_CLI_HDR_LEN		20
#define IP_CLI_HDR_IPPOS	9

// 012345678901234567
// ACK : XXXXXXXX : <ANS>
#define IP_ACK_HDR_LEN		17
#define IP_ACK_HDR_IPPOS	6
#define IP_ACK_HDR_IPLEN	8

void CLI_print(char *str, ...)
{
  uint32_t len;
  va_list arg_ptr;
  if (!eth_print) {
	  va_start(arg_ptr, str);
	  len = vsnprintf((char *)&CLI_OutBuffer[0], CLI_MAX_STR_LENGTH, str, arg_ptr);
	  va_end(arg_ptr);
	  HAL_UART_Transmit(&huart7, (uint8_t *)&CLI_OutBuffer[0], len, 0xFFFF);
  } else {
#ifdef USE_LWIP
	  sPDOSettings *sset = sysset_get_settings();

	  sprintf((char *)&CLI_OutBuffer[0], "ACK : ");
	  Array2Hex(&sset->network.ip[0], 4, (char *)&CLI_OutBuffer[IP_ACK_HDR_IPPOS]);
	  sprintf((char *)&CLI_OutBuffer[IP_ACK_HDR_IPPOS + IP_ACK_HDR_IPLEN], " : ");

	  va_start(arg_ptr, str);
	  len = vsnprintf((char *)&CLI_OutBuffer[IP_ACK_HDR_LEN], CLI_MAX_STR_LENGTH - IP_ACK_HDR_LEN, str, arg_ptr);
	  va_end(arg_ptr);

#ifdef USE_AES
	  uint8_t rd;

	  rd = len & (AES_BLOCK_SIZE - 1);
	  if (rd) {	// make AES_BLOCK_SIZE align
	    rd = AES_BLOCK_SIZE - rd;
	    memset(&CLI_OutBuffer[IP_ACK_HDR_LEN + len], 0, rd);
	    len = len + rd;
	  }
	  AES_code_data(AES_get_static_state(), &CLI_OutBuffer[IP_ACK_HDR_LEN], &CLI_OutBuffer[IP_ACK_HDR_LEN], len);
#endif

	  udp_ips_send((uint8_t *)&CLI_OutBuffer[0], len + IP_ACK_HDR_LEN, eth_sender_ip, UDP_CTRL_PORT);
#endif
  }
}

void CLI_ethernet_input(uint8_t *data, uint32_t len, void *socket)
{
#ifdef USE_LWIP
	uint8_t _ok;

	if (len < IP_CLI_HDR_LEN) return;
	len -= IP_CLI_HDR_LEN;

	eth_sender_ip.addr = PP_HTONL(Hex2Int((char *)&data[IP_CLI_HDR_IPPOS], 4, &_ok));
	//CLI_print("Sender IP: %.8X\r\n", eth_sender_ip.addr);

#ifdef USE_AES
	if (len & (AES_BLOCK_SIZE - 1)) return; // check AES_BLOCK_SIZE align

	AES_decode_data(AES_get_static_state(), &data[IP_CLI_HDR_LEN], &data[IP_CLI_HDR_LEN], len);
	len = strlen((char *)data) - IP_CLI_HDR_LEN; // get real not AES_BLOCK_SIZE aligned len
#endif

	eth_print = 1;
	CLI_executeCMD((char *)&data[IP_CLI_HDR_LEN], len);
	eth_print = 0;
#endif
}

void CLI_uart_input(uint8_t *data, uint32_t len)
{
  data[len-1] = 0;
  //CLI_print("Executing... %s\r\n",&data[0]);
  CLI_executeCMD((char *)&data[0],len);
}

void CLI_uart_task()
{
   uint16_t i;
   uint8_t data;

   if (CLI_RxBufferCnt) { // something received
	  i = 0;
      while ((CLI_InBufferCnt < CLI_MAX_STR_LENGTH) && (i < CLI_RxBufferCnt)) {
	    data = CLI_RxBuffer[i++];
		switch (data) {
		  case '\r': //break;  // skip
		  case '\n':     // command line received
			 if (CLI_InBufferCnt > 0) {
		      CLI_InBuffer[CLI_InBufferCnt] = data;
			  CLI_uart_input(&CLI_InBuffer[0], CLI_InBufferCnt + 1);
			  CLI_InBufferCnt = 0;
			 };
		     break;
		  default: // simply symbol
		     CLI_InBuffer[CLI_InBufferCnt] = data;
		     CLI_InBufferCnt++;
		}
	  }

	  if (CLI_InBufferCnt >= CLI_MAX_STR_LENGTH) // receive without <\n> overflow
	   CLI_InBufferCnt = 0; // delete line without processing

	  CLI_RxBufferCnt = 0;
   }
   
   HAL_UARTEx_ReceiveToIdle_IT(&huart7, (uint8_t *)&CLI_UARTRxBuffer[0], CLI_MAX_UARTBUF_LENGTH);
}

/// UART RX complete interrupt callback
void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size)
{
	uint8_t idx = 0;

	while ((CLI_RxBufferCnt < CLI_MAX_STR_LENGTH) && (idx < Size)) {
		CLI_RxBuffer[CLI_RxBufferCnt++] = CLI_UARTRxBuffer[idx++];
	}

	HAL_UARTEx_ReceiveToIdle_IT(&huart7, (uint8_t *)&CLI_UARTRxBuffer[0], CLI_MAX_UARTBUF_LENGTH);
}

/// UART error interrupt callback
void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart) {
	HAL_UART_AbortReceive_IT(&huart7);
}

/// table for HEX-string conversion
const char HexTab[]="0123456789ABCDEF";

uint32_t Hex2Int(char* _src, uint8_t _size, uint8_t* _ok)
{
	uint32_t _result = 0;
	int _i, _j;
	char symb;
	if (_size > 4) return 0;
	_size <<= 1;
	for (_i = 0; _i < _size; _i++){
		symb = _src[_i];
		_j = 0;
		while (_j < 16) {
			if (symb == HexTab[_j]) break;
			_j++;
		}
		if (_j >= 16) {
			if (_ok) *_ok = 0;
			return 0;
		}
		_result <<= 4;
		_result |= _j;
	}
	if (_ok) *_ok = 1;
	return _result;
}

void Int2Hex(uint32_t _src, uint8_t _size, char* _dst)
{
  _size = _size << 1;
  _dst += _size - 1;
  while (_size--) {
   *_dst-- = HexTab[_src & 0xF];
   _src >>= 4;
  }
}

uint8_t Hex2Array(char* _src, uint16_t _size, uint8_t* _dst)
{
	uint8_t _ok;

	_size >>= 1;
	while (_size--) {
		*_dst = Hex2Int(_src, 1, &_ok);
		if (!_ok) return 0;
		_dst++;
		_src += 2;
	}
	return 1;
}

void Array2Hex(uint8_t* _src, uint16_t _size, char* _dst)
{
	while (_size--) {
		Int2Hex(*_src, 1, _dst);
		_src++;
		_dst +=2;
	}
}
