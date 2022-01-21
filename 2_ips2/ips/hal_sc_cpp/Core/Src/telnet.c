/*
 * telnet.c
 *
 *  Created on: Jun 15, 2020
 *      Author: Donskikh Dmitry
 */

#include "FreeRTOS.h"
#include "task.h"
#include "cmsis_os2.h"
#include "socket.h"
#include "telnet.h"
#include "string.h"
#include "printf.h"
#include "i2cflash.h"

#define BSP_EEPROM_TESING_SIZE 5*1024

//#define BSP_EEPROM_CHECK_1
//#define BSP_EEPROM_CHECK_2
#define BSP_EEPROM_CHECK_3
#define BSP_EEPROM_CHECK_RO

static const char msg_wrong[]="Wrong command\n";

static const char msg_freemem[]="freemem cmd placeholder\n";
static const char msg_ps[]="ps -stack cmd placeholder\n";
static const char msg_ps_cpu[]="ps -cpu cmd placeholder\n";

extern void configureTimerForRunTimeStats(void);

static void createWorker(int sock);
static void telnetWorkerTask(void *pvParameters);
static uint32_t put(uint8_t *data, uint16_t size, int sock);
static uint32_t getBlocking(uint8_t *buffer, uint16_t maxlen, int sock);
static int parseTelnetOpts(uint8_t *data, uint16_t size, int sock);
static void paseCLI(uint8_t *line,int sock);

static void list(int sock);
static void nodeList(int sock);
static void eepromTesting(int sock);
static void eepromErasing(uint64_t size);
static void monitorTask(int sock);
static void eepromFill();

static int g_s_socket=-1;
static uint8_t telnet_print_buffer[2*1024];


void internal_printf(int sock,uint8_t *buffer,const char *fmt,...);

void createTelnetServer (void)
{
	TaskHandle_t srvHandle=NULL;
	xTaskCreate(telnetServerTask, "Telnet Server", 1024, NULL, osPriorityAboveNormal, &srvHandle);
}

void telnetServerTask (void *pvParameters)
{
	int						listener=0, con_socket=0;
		struct sockaddr_in 		server, client;
		struct sockaddr 		*sa, *ca;

		server.sin_addr.s_addr = INADDR_ANY;
		server.sin_family = AF_INET;
		server.sin_port = htons(TELNET_PORT);

		socklen_t dlinna_huya;
		dlinna_huya=sizeof(server);
		sa=&server;
		ca=&client;

		listener=socket(AF_INET, SOCK_STREAM, 0);
		if(listener < 0){
			Error_Handler();
		}

		if((bind(listener, sa , sizeof(struct sockaddr_in))) == -1){
			close(listener);
			Error_Handler();
		}
		while(listen(listener, 5)<0)osDelay(1);
		while(1){

			con_socket = accept(listener, ca, &dlinna_huya);
			if(con_socket<0) {
				close(listener);
				Error_Handler();
			}
			createWorker(con_socket);
			osDelay(1);
		}
}

static void createWorker(int sock)
{
	TaskHandle_t wrkHandle=NULL;
	xTaskCreate(telnetWorkerTask, "Telnet worker N", 1024*4, (void*)sock, osPriorityAboveNormal, &wrkHandle);
}

static void telnetWorkerTask(void *pvParameters)
{
	int sock=(int*)pvParameters;
	uint8_t *tBuf=pvPortMalloc(sizeof(uint8_t[TERM_RES_VT100]));
	memset(tBuf,0,TERM_RES_VT100);
	uint32_t string_len=0;
	uint32_t string_offset=0;
	g_s_socket=sock;
	printTelnet("Telnet CLI module for ST Platform Framework(c) Donskikh Dmitriy 2019-2020\r\n\r\n");
	printTelnet("Socket used: %d\r\n\r\n",sock);
	printTelnet("System commands:\r\n\ reboot - soft restart MCU;\r\n ps --cpu - show current running processes with CPU time usage;\r\n ps --stack - stack usage;\r\n\ freemem - show free heap memory\r\n");
	printTelnet("CTRL+C to disconnect.\r\n\r\n");
	for(;;){
			int tx_len=getBlocking((uint8_t*)tBuf+string_offset, TERM_RES_VT100,sock);
			if(tx_len>0){
				/*
				if (strchr((uint8_t*)tBuf,EOF)!=NULL){
					vPortFree(tBuf);
					close(sock);
					vTaskDelete( NULL );
				}
				*/
				if (strchr((uint8_t*)tBuf,0xFF)!=NULL)
				{
					if (parseTelnetOpts(tBuf,tx_len+string_offset,sock)<0)
					{
						vPortFree(tBuf);
						if(sock==g_s_socket){g_s_socket=-1;}
						close(sock);
						vTaskDelete( NULL );
					}
					memset(tBuf,0,TERM_RES_VT100);
					string_offset=0;
				}else
				{
					if (strchr((uint8_t*)tBuf,'\n')!=NULL)
					{
						//put((uint8_t*)tBuf,string_offset+tx_len,sock);
						paseCLI(tBuf, sock);
						memset(tBuf,0,TERM_RES_VT100);
						string_offset=0;
					}else
					{
						string_offset+=tx_len;
					}
				}
			}else{
				vPortFree(tBuf);
				if(sock==g_s_socket){g_s_socket=-1;}
				close(sock);
				vTaskDelete( NULL );
			}
		}

}

static uint32_t put(uint8_t *data, uint16_t size, int sock)
{
	return write(sock,data,size);
}

static uint32_t getBlocking(uint8_t *buffer, uint16_t maxlen, int sock)
{
	return read(sock,buffer,maxlen);
}

int parseTelnetOpts(uint8_t *data, uint16_t size, int sock)
{
	char* cmdpos=(char*)data;
	while (cmdpos<data+size){
		cmdpos=strchr(cmdpos,0xFF);
		if (cmdpos==NULL){return 0;}
		switch (*(++cmdpos)) {
			case 0xF4:
				return -1;
				break;
			default:
				break;
		}
	}
	return 0;
}

void paseCLI(uint8_t *line,int sock)
{
	uint8_t *cli_buffer=pvPortMalloc(TERM_RES_VT100);
	if (strstr(line,"freemem")!=NULL){
		internal_printf(sock,cli_buffer,"Free heap is: %d bytes\n",xPortGetFreeHeapSize());
	}else if (strstr(line,"ps --stack")!=NULL){
		vTaskList(cli_buffer);
		put(cli_buffer,strlen(cli_buffer),sock);
	}else if (strstr(line,"ps --cpu")!=NULL){
		vTaskGetRunTimeStats(cli_buffer);
		put(cli_buffer,strlen(cli_buffer),sock);
	}else if (strstr(line,"monitor")!=NULL){
		vPortFree(cli_buffer);
		monitorTask(sock);
	}else if (strstr(line,"reboot")!=NULL){
		NVIC_SystemReset();
	}else if (strstr(line,"list")!=NULL){
		if (strstr(line,"nodes")!=NULL){
		    nodeList(sock);
		}else{
		    list(sock);
		}
	}else if (strstr(line,"format eeprom")!=NULL){
		fsFormat();
		internal_printf(sock, cli_buffer,"EEPROM Filesystem formated\n");
	}else if (strstr(line,"check")!=NULL){
		if (strstr(line,"eeprom")!=NULL){
		    eepromTesting(sock);
		}else{
		}
	}else if (strstr(line,"erase")!=NULL){
		if (strstr(line,"eeprom")!=NULL){
		    eepromErasing(1024*10);
		}else{
		}
	}else if (strstr(line,"fill")!=NULL){
		if (strstr(line,"eeprom")!=NULL){
		    eepromFill();
		}else{
		}
	}
	//put(&msg_wrong,sizeof(msg_wrong),sock);
	vPortFree(cli_buffer);
	return;
}

void internal_printf(int sock,uint8_t *buffer,const char *fmt,...)
{
	va_list arp;
	va_start(arp, fmt);
	int len=vsnprintf(buffer,TERM_RES_VT100,fmt,arp);
	va_end(arp);
	put (buffer,len,sock);
}

void list(int sock)
{
    uint8_t *buf=pvPortMalloc(TERM_RES_VT100);
    fstable table;
    mkFStable(&table);
    for(int i=0;i<IPS_BSP_MAXFILES;i++){
	if(table.fsnodes[i].fbegin!=0){
	    internal_printf(sock, buf, "%d : %s %d bytes.\n",i,table.fsnodes[i].name,table.fsnodes[i].fsize);
	}
    }
    vPortFree(buf);
}

void nodeList(int sock)
{
    uint8_t *buf=pvPortMalloc(TERM_RES_VT100);
    fstable table;
    mkFStable(&table);
    for(int i=0;i<IPS_BSP_MAXFILES;i++){
	internal_printf(sock, buf, "%d: start:%d size:%d bytes name: %s \n",i,table.fsnodes[i].fbegin,table.fsnodes[i].fsize,table.fsnodes[i].name);
    }
    vPortFree(buf);
}

void eepromTesting(int sock){
    uint8_t *buf=pvPortMalloc(TERM_RES_VT100);
    uint8_t *buf_out=pvPortMalloc(BSP_EEPROM_TESING_SIZE);

#ifndef BSP_EEPROM_CHECK_RO
    uint8_t *buf_in=pvPortMalloc(BSP_EEPROM_TESING_SIZE); //2KB
    for (int i=0;i<BSP_EEPROM_TESING_SIZE;i++){
	if(i%26==25){
	    *(buf_in+i)='\n';
	}else{
	    *(buf_in+i)=i%26+65;
	}
    }
#endif
    int offset=0;
#ifdef BSP_EEPROM_CHECK_1
    internal_printf(sock, buf, "1. Bytewise writing - Bytewise reading(2048bytes)\n");
    for (int i=0;i<BSP_EEPROM_TESING_SIZE;i++){
	extFlashWrite(offset+i, buf_in+i, 1);
    }
    for (int i=0;i<BSP_EEPROM_TESING_SIZE;i++){
    	extFlashRead(buf_out+i, offset+i, 1);
    }
    for (int i=0;i<BSP_EEPROM_TESING_SIZE;i++){
    	internal_printf(sock, buf, "%c",*(buf_out+i));
    	if(i%26==25){
    	    internal_printf(sock, buf, "\n");
    	}
    }
#endif
#ifdef BSP_EEPROM_CHECK_2
    internal_printf(sock, buf, "\n2. Bytewise writing - Block reading(2048bytes)\n");
    memset(buf_out,0x0,BSP_EEPROM_TESING_SIZE);
    for (int i=0;i<BSP_EEPROM_TESING_SIZE;i++){
	extFlashWrite(offset+i, buf_in+i, 1);
    }
    extFlashRead(buf_out, offset, BSP_EEPROM_TESING_SIZE);
    for (int i=0;i<BSP_EEPROM_TESING_SIZE;i++){
        internal_printf(sock, buf, "%c",*(buf_out+i));
        if(i%26==25){
            internal_printf(sock, buf, "\n");
        }
    }
#endif
#ifdef BSP_EEPROM_CHECK_3
#ifndef BSP_EEPROM_CHECK_RO
    internal_printf(sock, buf, "\n3. Block writing - Block ant bytewise reading(BSP_EEPROM_TESING_SIZE bytes)\n");
    memset(buf_out,0x0,BSP_EEPROM_TESING_SIZE);
    extFlashWrite(offset, buf_in, BSP_EEPROM_TESING_SIZE);
    osDelay(100);
    extFlashRead(buf_out, offset, BSP_EEPROM_TESING_SIZE);
    for (int i=0;i<BSP_EEPROM_TESING_SIZE;i++){
        internal_printf(sock, buf, "%c",*(buf_out+i));
        if(i%26==25){
            internal_printf(sock, buf, "\n");
        }
    }
#endif
    internal_printf(sock, buf, "\n3.2 One more with bytewise...\n");
    memset(buf_out,0x0,BSP_EEPROM_TESING_SIZE);
    for (int i=0;i<BSP_EEPROM_TESING_SIZE;i++){
	extFlashRead(buf_out+i, offset+i, 1);
	//osDelay(1);
    }
    for (int i=0;i<BSP_EEPROM_TESING_SIZE;i++){
        internal_printf(sock, buf, "%c",*(buf_out+i));
        //osDelay(1);
    }
#endif

    vPortFree(buf);
#ifndef BSP_EEPROM_CHECK_RO
    vPortFree(buf_in);
#endif
    vPortFree(buf_out);
}

void eepromErasing(uint64_t size)
{
    uint8_t zero=0;
    uint8_t offset=0;

    for (uint64_t i=0;i<size;i++){
    	extFlashWrite(i, &zero, 1);
    }

}

void monitorTask(int sock){
    uint8_t *buff=pvPortMalloc(TERM_RES_VT100);
    while(1){
	put("\033[2J",8,sock);
	vTaskList(buff);
	put(buff,strlen(buff),sock);
	vTaskGetRunTimeStats(buff);
	put(buff,strlen(buff),sock);
	internal_printf(sock,buff,"Free heap is: %d bytes\n",xPortGetFreeHeapSize());
	osDelay(400);
    }
}

void eepromFill(){
    int offset=0;
    uint8_t *buf_in=pvPortMalloc(BSP_EEPROM_TESING_SIZE); //2KB
    //uint8_t *buf_out=pvPortMalloc(BSP_EEPROM_TESING_SIZE);
    for (int i=0;i<BSP_EEPROM_TESING_SIZE;i++){
	if(i%26==25){
    	    *(buf_in+i)='\n';
    	}else{
    	    *(buf_in+i)=i%26+65;
    	}
    }
    extFlashWrite(offset, buf_in, BSP_EEPROM_TESING_SIZE);
    vPortFree(buf_in);

}

void printTelnet(const char *fmt,...){
    if (g_s_socket<0){
	return;
    }
    va_list arp;
    va_start(arp, fmt);
    int len=vsnprintf(&telnet_print_buffer,2*1024,fmt,arp);
    va_end(arp);
    put (&telnet_print_buffer,len,g_s_socket);
}


