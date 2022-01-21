
/*
 * telnetServer.cpp
 *
 *  Created on: 2 июн. 2020 г.
 *      Author: Donskikh Dmitry
 */
/*
MIT License

Copyright (c) 2020 Dmitry Donskikh

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/



#include "telnetServer.h"
#include "FreeRTOS.h"
#include "debug3.h"
#include <new>
#include <ips_globals.hpp>
#include "ipssystem.hpp"
#include <cstring>



#define NEW(pointer,type) \
	pointer=(type*)pvPortMalloc(sizeof(type));\
	pointer=(type*)new (pointer)type

#define DELETE(pointer,type) \
	((type*)pointer)->~type();\
	vPortFree((void*)pointer)


ipsTelnet::ipsTelnet(const char *name,ipcCLI *cli_processor ,uint16_t port, uint8_t mclients)
:tcpServerLegacy(name,port,mclients)
{
	processor=cli_processor;
	DEBUG_IPS("Telnet server started");
}


void ipsTelnet::create_connection(int sock, uint32_t addr)
{
	NEW(obMem,telnetConnection)(this,processor,sock,addr,"telnet1");
	DEBUG_IPS("Telnet client connected");

}

void ipsTelnet::notify_dead(void *connection) {

	//DELETE(connection,telnetConnection);
	DEBUG_IPS("Telnet client disconnected");
	num_clients--;
	Resume();
	((telnetConnection*)connection)->~telnetConnection();

}


telnetConnection::telnetConnection(ipsTelnet *pObj,ipcCLI *cli_processor, int active_socket, uint32_t ipDestinationAddr, const char *name)
	:tcpConnection(pObj,active_socket,ipDestinationAddr,name),ttY()//,to_telnet_tty(NULL,500)
{
	owner_tn=pObj;//to_telnet_tty.setTerm(this);
	processor=cli_processor;
}

void telnetConnection::tcpWorker()
{
	ipsSystem::monterm=this;
	ipsSystem::sysmonitor->setTerm();
	for(;;){

		int tx_len=getBlocking((uint8_t*)tBuf, UFW_SYSTEM_GP_BUFFER_SIZE);
		if(tx_len>0){
			//put((uint8_t*)tBuf,tx_len);
		    if (strchr((const char*)tBuf,0xFF)!=NULL)
		    	{
		    	if (parseTelnetOpts(tx_len+string_offset)<0)
		    	    {
		    		memset(tBuf,0,UFW_SYSTEM_GP_BUFFER_SIZE); //TODO: add as private class member
		    		//if(sock==g_s_socket){g_s_socket=-1;}
		    		owner_tn->notify_dead(this);
		    		while(true){osDelay(100);};
		    	    }
		    	    cleanBuffer();
		    	    string_offset=0;
		    	}else
		    	{
		    	    if (strchr((const char*)tBuf,'\n')!=NULL)
		    		{
		    	    	*(strchr((char*)tBuf,'\n'))=0x00;
		    	    	*(strchr((char*)tBuf,'\r'))=0x00;
		    	    	processor->parseCLI((char*)tBuf,string_offset+tx_len, this);
		    		    cleanBuffer();
		    		    string_offset=0;
		    		}else
		    		{
		    		    string_offset+=tx_len;
		    		}
		    	}

		}else{
			owner_tn->notify_dead(this);
			while(true){osDelay(100);};
		}
	}
}

void telnetConnection::write_t(const char *buf, size_t size)
{
	put((uint8_t*)buf,size);
}

int telnetConnection::parseTelnetOpts(uint16_t size)
{
	uint32_t cmdpos=0;
	while (cmdpos<size){

		if (strchr((const char*)tBuf,0xFF)==NULL){return 0;}
		switch (*(tBuf+(++cmdpos))) {
			case 0xF4:
				return -1;
				break;
			default:
				break;
		}
	}
	return 0;
}

inline void telnetConnection::cleanBuffer()
    {
	memset(tBuf,0,TERM_RES_VT100);
    }
