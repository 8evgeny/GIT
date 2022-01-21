/*
 * ipsTcpServer.cpp
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


#include <ipsTcpServerLegacy.h>
#include "debug3.h"
#include "socket.h"
#include "ipssystem.hpp"


extern uint8_t is_lwip_ready;

tcpServerLegacy::tcpServerLegacy(const char *name, uint16_t port, uint8_t mclients)
:ipcThread(name,512,osPriorityAboveNormal3)
{
	tcp_port=port;
	max_clients=mclients;
}

void tcpServerLegacy::taskRunFcn()
{
	int						listener=0, con_socket=0;
	struct sockaddr_in 		server, client;
	struct sockaddr 		*sa, *ca;

	server.sin_addr.s_addr = INADDR_ANY;
	server.sin_family = AF_INET;
	server.sin_port = htons(tcp_port);

	socklen_t dlinna_huya;
	dlinna_huya=sizeof(server);
	sa=(sockaddr*)&server;
	ca=(sockaddr*)&client;

	while (!is_lwip_ready) osDelay(1);

	listener=socket(AF_INET, SOCK_STREAM, 0);
	if(listener < 0){
		Error_Handler();
	}

	int tempsocket=listener;
	if((bind(tempsocket, sa , sizeof(sockaddr_in))) == -1){
		close(listener);
		Error_Handler();
	}
	while(listen(tempsocket, 5)<0)osDelay(1);
	while(control==nwControlRun){

		con_socket = accept(listener, ca, &dlinna_huya);
		if(con_socket<0) {
			close(listener);
			Error_Handler();
		}
		if(num_clients<max_clients){
			create_connection(con_socket,client.sin_addr.s_addr);
			num_clients++;
		}else{
			close(con_socket);
			control=nwControlPause;
		}
		while(control==nwControlPause)
		{
			osDelay(1);
		}
		osDelay(1);
	}

}
/*
tcpServer::~tcpServer() {
	control=nwControlPause;
	if(con_socket>=0){close(con_socket);}
	if(listener>=0){close(listener);}

}
*/
void tcpServerLegacy::notify_dead(void* connection)
{
	num_clients--;
	Resume();
}

/*
void tcpServer::create_connection(int sock, uint32_t addr)
{
	if(num_clients<max_clients){
		newConn = new (tcpConnection(this,sock,addr, "new"));
		num_clients++;
	}else{
		close(sock);
	}
}
*/

void tcpServerLegacy::Pause()
{
	control=nwControlPause;
}

void tcpServerLegacy::Resume()
{
	control=nwControlRun;
}

void tcpServerLegacy::create_connection(int sock, uint32_t addr)
{
	DEBUG_IPS("New Connection")
}

void tcpServerLegacy::Stop()
{
	control=nwControlKill;
}


tcpConnection::tcpConnection(tcpServerLegacy *pObj, int active_socket,
		uint32_t ipDestinationAddr, const char *name)
:ipcThread(name,1024,osPriorityNormal)
{
	sock=active_socket;
	addr=ipDestinationAddr;
	owner=pObj;
	tBuf=(uint8_t*)system_cpp.getBuffer();
}

tcpConnection::~tcpConnection()
{
	close(sock);
	system_cpp.freeBuffer(tBuf);
	child=(void*)this;
}

uint32_t tcpConnection::put(uint8_t *data, uint16_t size)
{
	return write(sock,data,size);
}

uint32_t tcpConnection::getBlocking(uint8_t *buffer, uint16_t maxlen)
{
	return read(sock,buffer,maxlen);
}


void tcpConnection::taskRunFcn()
{
	tcpWorker();
}
