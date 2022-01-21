/*
 * fttcpproto.c
 *
 *  Created on: 28 июн. 2020 г.
 *      Author: Dmitry Dmitry Donskih
 */
#include "FreeRTOS.h"
#include "task.h"
#include "cmsis_os2.h"
#include "string.h"
#include "socket.h"
#include "i2cflash.h"
#include "fttcpproto.h"
#include "ips_complan.h"
#include "telnet.h"

#define FTRANSFER_DEFAULT_BUFFER_SIZE	128U

#define FTRANSFER_FILESEND_PAR1			4U
#define FTRANSFER_FILESEND_PAR2			8U
#define FTRANSFER_FILESEND_PAR3			9U
#define FTRANSFER_FILESEND_NAME			13U

#define	__MAX_ERR_FRAMES				16U

#define IPS_BSP_CONF_CMD_DATASEND_HEADER	8U

#define NW_CMD_GETPAR(tfer,type,par)		(*((type*) (tfer + par)))
#define MW_CMD_GETPARPTR(tfer,type,par)		((type*) (tfer + par))

extern complan *system_complan;
extern sysinfo_t system_info;

/*
 * 		Device response constants
 */

static const uint32_t response_accept = RESPONSE_ACCEPT;
static const uint32_t response_error_fileexists = RESPONSE_ER_FILEXISTS;
static const uint32_t response_error_drive_full = RESPONSE_ER_DRIVEFULL;
static const uint32_t response_error_wrongdevice = RESPONSE_ER_WRONGDRIVE;
static const uint32_t response_error = RESPONSE_ERROR;

/*
 * 		Local static functions defenitions
 */
static void createWorker (int sock);
static void ftransferWorkerTask (void *pvParameters);

static uint32_t put (uint8_t *data, uint16_t size, int sock);
static uint32_t getBlocking (uint8_t *buffer, uint16_t maxlen, int sock);
static int EEPROMwrite (int socket, uint8_t *params);
static int complanWrite (int socket, uint8_t *params);
static int sysinfoWrite (int socket, uint8_t *params);
static void ResponseAcceptSize (int socket, uint32_t size);
static void ResponseErrorCommon (int socket);

void createFTransferServer (void)
{
  TaskHandle_t srvHandle = NULL;
  xTaskCreate (ftransferServerTask, "FTransfer Server", 1024, NULL, osPriorityAboveNormal,
	       &srvHandle);

}

void ftransferServerTask (void *pvParameters)
{
  int listener = 0, con_socket = 0;
  struct sockaddr_in server, client;
  struct sockaddr *sa, *ca;

  server.sin_addr.s_addr = INADDR_ANY;
  server.sin_family = AF_INET;
  server.sin_port = htons(FTRANSFER_PORT);

  socklen_t dlinna_huya;
  dlinna_huya = sizeof(server);
  sa = &server;
  ca = &client;

  listener = socket(AF_INET, SOCK_STREAM, 0);
  if (listener < 0)
    {
      Error_Handler ();
    }

  if ((bind(listener, sa, sizeof(struct sockaddr_in))) == -1)
    {
      close(listener);
      Error_Handler ();
    }
  while (listen(listener, 5) < 0)
    osDelay (1);
  while (1)
    {

      con_socket = accept(listener, ca, &dlinna_huya);
      if (con_socket < 0)
	{
	  close(listener);
	  Error_Handler ();
	}
      createWorker (con_socket);
      osDelay (1);
    }

}

/*
 * 		Local static functions
 */

void createWorker (int sock)
{
  TaskHandle_t wrkHandle = NULL;
  xTaskCreate (ftransferWorkerTask, "Transfer N", 1024 * 4, (void*) sock, osPriorityAboveNormal,
	       &wrkHandle);
}

void ftransferWorkerTask (void *pvParameters)
{
  int sock = (int*) pvParameters;
  uint8_t *tBuf = pvPortMalloc (sizeof(uint8_t[FTRANSFER_DEFAULT_BUFFER_SIZE]));
  memset (tBuf, 0, FTRANSFER_DEFAULT_BUFFER_SIZE);
  //int addr=(int)tBuf
  int result = 0;
  while (1)
    {
      int tx_len = getBlocking ((uint8_t*) tBuf, FTRANSFER_DEFAULT_BUFFER_SIZE, sock);
      if (tx_len != 0)
	{
	  if (*((uint32_t*) tBuf) == FTRANSFER_CMD_FILESEND)
	    {
	      switch (*((uint8_t*) (tBuf + FTRANSFER_FILESEND_PAR2)))
		{
		case FSDEVICE_EEPROM:
		  result = EEPROMwrite (sock, tBuf);
		  break;
		default:
		  result = RESPONSE_ER_WRONGDRIVE;
		  put (&result, sizeof(uint32_t), sock);
		  vPortFree (tBuf);
		  close(sock);
		  vTaskDelete ( NULL);
		  break;
		}
	      if ((result != RESPONSE_ERROR) || (result != 0))
		{
		  put (&result, sizeof(uint32_t), sock);
		};
	      close(sock); 	/// !!?? TODO: Make test to close closed socket
	      vPortFree (tBuf);
	      vTaskDelete ( NULL);

	    } else if (*((uint32_t*) tBuf) == IPS_BSP_CONF_CMD_LOADIPTABLE)
	    {
	      result = complanWrite (sock, tBuf);
	      if (result < 0)
		{
		  printTelnet (
		      "FAIL: something goes wrong during routing table transfer ;((( code:%d",
		      result);
		  close(sock);
		  vPortFree (tBuf);
		  vTaskDelete (NULL);
		}
	      //complanPrintInfo();
	    } else if (*((uint32_t*) tBuf) == IPS_BSP_CONF_CMD_LOADSYSINFO)
	    {
	      result = sysinfoWrite (sock, tBuf);
	      if (result < 0)
		{
		  printTelnet (
		      "FAIL: something goes wrong during system info transfer ;((( code:%d",
		      result);
		  close(sock);
		  vPortFree (tBuf);
		  vTaskDelete (NULL);
		}
	      systeminfoPrint ();
	    }
	} else
	{
	  printTelnet ("INFO:Socket %d closed by client\r\n", sock);
	  vPortFree (tBuf);
	  close(sock);
	  vTaskDelete ( NULL);
	}
    }

}
/*
 * 		Network static I/O functions
 */

uint32_t put (uint8_t *data, uint16_t size, int sock)
{
  return write(sock, data, size);
}

uint32_t getBlocking (uint8_t *buffer, uint16_t maxlen, int sock)
{
  return read(sock, buffer, maxlen);
}

/*
 * 		Local device storage R/W functions (static)
 */

int EEPROMwrite (int socket, uint8_t *params)
{
  const uint16_t response_size = sizeof(uint32_t) * 16;
  uint8_t *response = pvPortMalloc (response_size);
  const uint16_t tfer_buffer_size = NW_CMD_GETPAR(params, uint32_t, FTRANSFER_FILESEND_PAR3)
      + sizeof(uint32_t) * 8;
  uint8_t *tfer_buffer = pvPortMalloc (tfer_buffer_size);
  uint32_t file_size = NW_CMD_GETPAR(params, uint32_t, FTRANSFER_FILESEND_PAR1);
  uint16_t error_frames = 0;
  /*
   if (file_size>fsFreeSpace()){return RESPONSE_ER_DRIVEFULL;} //TODO: write getter for free space of filesystem
   */
  char file_name[16];
  memset (&file_name, 0, sizeof(file_name));
  memcpy (&file_name, MW_CMD_GETPARPTR(params, char, FTRANSFER_FILESEND_NAME),
	  sizeof(char) * MAXFILENAME);
  FILE_t new_file;
  fsNewOpen (&file_name, &new_file);
  memcpy (response, &response_accept, sizeof(response_accept));
  memcpy (response + sizeof(response_accept), &file_size, sizeof(file_size));
  put (response, sizeof(response_accept) + sizeof(file_size), socket);
  while (1)
    {
      int tx_len = getBlocking ((uint8_t*) tfer_buffer, tfer_buffer_size, socket);
      if (tx_len <= 0)
	{
	  vPortFree (response);
	  vPortFree (tfer_buffer);
	  fsCleanFileRecord (&new_file); // TODO: write function that cleans record of file fron file system and given handler
	  return RESPONSE_ERROR;
	}
      if (*((uint32_t*) tfer_buffer) == FTRANSFER_CMD_DATASEND)
	{
	  fsWrite (&new_file, *((uint32_t*) (tfer_buffer + sizeof(uint32_t))),
		   tfer_buffer + (sizeof(uint32_t) * 2)); //change to macro
	  memcpy (response, &response_accept, sizeof(response_accept));
	  memcpy (response + sizeof(response_accept),
		  ((uint32_t*) (tfer_buffer + sizeof(uint32_t))), sizeof(uint32_t));
	  put (response, sizeof(response_accept) + sizeof(uint32_t), socket);
	} else if (*((uint32_t*) tfer_buffer) == FTRANSFER_CMD_EOT)
	{
	  fsClose (&new_file);
	  vPortFree (response);
	  vPortFree (tfer_buffer);
	  return 0;
	} else
	{
	  error_frames++;
	  if (error_frames > __MAX_ERR_FRAMES)
	    {
	      vPortFree (response);
	      vPortFree (tfer_buffer);
	      fsCleanFileRecord (&new_file);
	      return RESPONSE_ERROR;
	    }
	}

    }
  return 0;
}

static int complanWrite (int socket, uint8_t *params)
{
  osDelay (100); //TODO: DELETE! Checkpoint
  uint16_t error_frames = 0;
  uint32_t complan_size = NW_CMD_GETPAR(params, uint32_t, FTRANSFER_FILESEND_PAR1);
  uint32_t buffer_size = complan_size + sizeof(uint32_t) * 4;
  if (system_complan != NULL)
    {
      vPortFree (system_complan);
    }
  printTelnet ("INFO:LoadIpTable(%d) command recieved.\r\n", complan_size);
  system_complan = pvPortMalloc (complan_size);
  printTelnet ("INFO:Allocated %d bytes for commplan\r\n", complan_size);
  uint8_t *tfer_buffer = pvPortMalloc (buffer_size);
  ResponseAcceptSize (socket, complan_size);
  while (1)
    {
      int tx_len = getBlocking ((uint8_t*) tfer_buffer, buffer_size, socket);
      printTelnet ("INFO:Recieved %d bytes\r\n", tx_len);
      if (tx_len == 0)
	{
	  printTelnet ("INFO:Socket %d closed by client\r\n", socket);
	  vPortFree (tfer_buffer);
	  return RESPONSE_ERROR;
	}
      if (*((uint32_t*) tfer_buffer) == IPS_BSP_CONF_CMD_DATASEND)
	{
	  printTelnet ("INFO:DataSend() command recieved with %d bytes of data \r\n",
		       tx_len - sizeof(uint32_t) * 2);
	  complan_size = NW_CMD_GETPAR(tfer_buffer, uint32_t, FTRANSFER_FILESEND_PAR1);
	  printTelnet ("INFO:getting %d bytes for commplan\r\n", complan_size);
	  if (complan_size > (tx_len - sizeof(uint32_t) * 2))
	    {
	      int recieved = tx_len - sizeof(uint32_t) * 2;
	      while (recieved < complan_size)
		{
		  int chunk_len = getBlocking (
		      tfer_buffer + recieved + IPS_BSP_CONF_CMD_DATASEND_HEADER,
		      complan_size - recieved, socket); //TODO: Refactor all
		  printTelnet ("INFO:Recieved %d bytes\r\n", chunk_len);
		  recieved += chunk_len;
		}
	    }
	  memcpy (system_complan, tfer_buffer + sizeof(uint32_t) * 2, complan_size);
	  ResponseAcceptSize (socket, complan_size);
	} else if (*((uint32_t*) tfer_buffer) == IPS_BSP_CONF_CMD_EOT)
	{
	  printTelnet ("INFO:EOT() command recieved. Transfer ended.\r\n");
	  vPortFree (tfer_buffer);
	  return 0;
	} else
	{
	  error_frames++;
	  if (error_frames > __MAX_ERR_FRAMES)
	    {
	      printTelnet ("ERROR: Too many errors. Transfer aborting..\r\n");
	      vPortFree (tfer_buffer);
	      ResponseErrorCommon (socket);
	      return RESPONSE_ERROR;
	    }
	}
      osDelay (100); //TODO: DELETE! Checkpoint
    }
}

#define IPS_BSP_SCP_DATASIZE	NW_CMD_GETPAR(params,uint32_t,FTRANSFER_FILESEND_PAR1)
#define IPS_BSP_SCP_BUFFERSIZE	NW_CMD_GETPAR(params,uint32_t,FTRANSFER_FILESEND_PAR1)+sizeof(uint32_t)*4

static int sysinfoWrite (int socket, uint8_t *params)
{
  printTelnet ("INFO:LoadSysInfo(%d) command recieved.\r\n", IPS_BSP_SCP_DATASIZE);
  uint16_t error_frames = 0;
  uint8_t *tfer_buffer = pvPortMalloc (IPS_BSP_SCP_BUFFERSIZE);
  ResponseAcceptSize (socket, IPS_BSP_SCP_DATASIZE);
  while (1)
    {
      int tx_len = getBlocking ((uint8_t*) tfer_buffer, IPS_BSP_SCP_BUFFERSIZE, socket);
      printTelnet ("INFO:Recieved %d bytes\r\n", tx_len);
      if (tx_len == 0)
	{
	  vPortFree (tfer_buffer);
	  return RESPONSE_ERROR;
	}
      if (*((uint32_t*) tfer_buffer) == IPS_BSP_CONF_CMD_DATASEND)
	{
	  printTelnet ("INFO:DataSend() command recieved with %d bytes of data \r\n",
		       tx_len - sizeof(uint32_t) * 2);
	  memcpy (&system_info, tfer_buffer + sizeof(uint32_t) * 2, IPS_BSP_SCP_DATASIZE);
	  ResponseAcceptSize (socket, IPS_BSP_SCP_DATASIZE);
	} else if (*((uint32_t*) tfer_buffer) == IPS_BSP_CONF_CMD_EOT)
	{
	  printTelnet ("INFO:EOT() command recieved. Transfer ended.\r\n");
	  vPortFree (tfer_buffer);
	  return 0;
	} else
	{
	  error_frames++;
	  if (error_frames > __MAX_ERR_FRAMES)
	    {
	      printTelnet ("ERROR: Too many errors. Transfer aborting..\r\n");
	      vPortFree (tfer_buffer);
	      ResponseErrorCommon (socket);
	      return RESPONSE_ERROR;
	    }
	}
      osDelay (100); //TODO: DELETE! Checkpoint
    }
}

#undef IPS_BSP_SCP_DATASIZE
#undef IPS_BSP_SCP_BUFFERSIZE

void ResponseAcceptSize (int socket, uint32_t size)
{
  uint32_t acc_buffer[2];
  acc_buffer[0] = RESPONSE_ACCEPT;
  acc_buffer[1] = size;
  put (&acc_buffer, sizeof(acc_buffer), socket);
}
void ResponseErrorCommon (int socket)
{
  uint32_t error_buffer = RESPONSE_ERROR;
  put (&error_buffer, sizeof(error_buffer), socket);
}

