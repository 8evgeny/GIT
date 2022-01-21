/*
 * udp.h
 *
 *  Created on: May 30, 2020
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


#ifndef USER_INC_IPSUDP_H_
#define USER_INC_IPSUDP_H_
//#include "cmsis_os.h"

#include <cmsis_os2.h>
#include <ipc_pipe.hpp>
#include <ipcThread.h>
#include <os/os_thread.h>
#include <sys/_stdint.h>
#include "socket.h"

class basicPipe;
struct sockaddr;
struct sockaddr_in;

//#define UDP_MAX_RCV 1500
//#define IP_READY_TIMEOUT 60000

#define MCASTSOC_MTU (1536)
#define MCAST_CLOSED (-1000)


class basicUDP :public ufw::rtosThread
{
public:
    basicUDP(uint32_t ip,uint16_t port,const char* name="UDP",uint16_t stack_size=128,char priority=osPriorityLow);
	basicUDP(const char* ip,uint16_t port,const char* name="UDP",uint16_t stack_size=128,char priority=osPriorityLow);
	virtual ~basicUDP();
	void threadFunction()override;
	void Remove();
protected:
	uint32_t read_d(uint8_t *data,uint32_t max_size);
	void write_d(uint8_t *data,uint32_t size);
	virtual void setOptions();
	virtual void runner()=0;


	struct sockaddr_in self, client;

	struct sockaddr *self_p, *client_p;
	int sockt;

};

class testUDPAudio :public basicUDP
{
public:
    testUDPAudio(uint32_t ip,uint16_t port,uint8_t* in_buffer);
    testUDPAudio(const char* ip,uint16_t port,uint8_t* in_buffer);
    void setPipe(basicPipe *audio_pipe);
    //void taskRunFcn() override;
protected:
    void runner() override;

    basicPipe* o_pipe=nullptr;
    uint8_t* buffer=nullptr;
    uint32_t size=0;

};

class UDPAudioIn :public basicUDP
{
public:
    UDPAudioIn(uint32_t ip,uint16_t port,uint8_t* in_buffer);
    UDPAudioIn(const char* ip,uint16_t port,uint8_t* in_buffer);
    void setPipe(basicPipe *audio_pipe);
    virtual ~UDPAudioIn();
    //void taskRunFcn() override;
protected:
    void runner() override;

    basicPipe* o_pipe=nullptr;
    uint8_t* buffer=nullptr;
    uint32_t size=0;

};

class UDPAudioOut :public basicUDP
{
public:
    UDPAudioOut(uint32_t ip,uint16_t port,uint8_t* out_buffer);
    UDPAudioOut(const char* ip,uint16_t port,uint8_t* out_buffer);
    void setPipe(basicPipe *audio_pipe);
    virtual ~UDPAudioOut();
    //void taskRunFcn() override;
protected:
    void runner() override;

    basicPipe* i_pipe=nullptr;
    uint8_t* buffer=nullptr;
    uint32_t size=0;

};

class UDPout
{
  public:
    UDPout(uint32_t ip,uint16_t port);
    void write_to(uint32_t ip,void *data,uint32_t size);

    void* writeBuffer();
  private:
    struct sockaddr_in client;

    struct sockaddr *pc;
    uint8_t* buffer=nullptr;
    int sockt=-1000;

};





extern "C" void isStackReady(void* arg);

#endif /* USER_INC_IPSUDP_H_ */
