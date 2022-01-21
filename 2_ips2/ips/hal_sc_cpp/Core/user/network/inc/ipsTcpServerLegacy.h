/*
 * ipsTcpServer.h
 *
 *  Created on: 1 июн. 2020 г.
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


#pragma once
#ifndef USER_NETWORK_INC_IPSTCPSERVERLEGACY_H_
#define USER_NETWORK_INC_IPSTCPSERVERLEGACY_H_

//#include "main.h"
#include "networkers_common.h"

#include "ipcThread.h"
//#include "socket.h"


/*
#define oCONN		void
#define pCONN		void*
*/
class tcpServerLegacy :public ipcThread
{
protected:

	uint16_t 				tcp_port;


	uint8_t					num_clients=0;
	uint8_t					max_clients=255;
	netWorkersControls_t 	control=nwControlRun;
	void*					newConn=NULL;


	virtual void create_connection(int sock,uint32_t addr);
public:
	tcpServerLegacy(const char* name,uint16_t port,uint8_t mclients=255);
	//~tcpServer();
	virtual void notify_dead(void* connection);
	void Pause();
	void Resume();
	void Stop();

	virtual void taskRunFcn() override;
};


class tcpConnection :public ipcThread
{
protected:
	int sock;
	uint32_t addr;
	//uint8_t *io_buffer=NULL;
	uint8_t *tBuf=NULL;
	tcpServerLegacy* owner;

public:
	tcpConnection(tcpServerLegacy* pObj,int active_socket,uint32_t ipDestinationAddr,const char* name);
	virtual ~tcpConnection() override;
	uint32_t put(uint8_t* data,uint16_t size);
	uint32_t getBlocking(uint8_t *buffer, uint16_t maxlen);
	//virtual uint32_t rcvTo(uint8_t *buffer, uint16_t size);
	//void getNonBlockig (void);
	virtual void taskRunFcn();
	virtual void tcpWorker()=0;
};




#endif /* USER_NETWORK_INC_IPSTCPSERVERLEGACY_H_ */
