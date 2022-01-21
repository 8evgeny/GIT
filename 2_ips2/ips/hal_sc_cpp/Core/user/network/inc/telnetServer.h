/*
 * telnetServer.h
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


#ifndef USER_NETWORK_INC_TELNETSERVER_H_
#define USER_NETWORK_INC_TELNETSERVER_H_

#include "main.h"
//#include "ttY.h"
#include "monitor.h"
#include <ipcCLI.hpp>
#include <devices/stream_device.h>
#include <ipsTcpServerLegacy.h>



class ipsTelnet :public tcpServerLegacy
{
protected:
	tcpConnection* obMem=NULL;
	ipcCLI *processor=NULL;
	void create_connection(int sock,uint32_t addr) override;

public:
	virtual void notify_dead(void* connection) override;
	ipsTelnet(const char* name,ipcCLI *cli_processor,uint16_t port=23,uint8_t mclients=8);


};

class telnetConnection :public tcpConnection, ttY
{
private:
	//monitorTask to_telnet_tty;
	ipsTelnet *owner_tn;
	uint32_t string_offset=0;
	ipcCLI *processor=NULL;

	int parseTelnetOpts(uint16_t size);
	inline void cleanBuffer();
public:
	telnetConnection(ipsTelnet* pObj,ipcCLI *cli_processor,int active_socket,uint32_t ipDestinationAddr,const char* name);
	virtual void tcpWorker()override;
	void write_t (const char* buf,size_t size);


};



#endif /* USER_NETWORK_INC_TELNETSERVER_H_ */
