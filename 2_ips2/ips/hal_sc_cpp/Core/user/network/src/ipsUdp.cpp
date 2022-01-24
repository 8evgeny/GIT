/*
 * udp.cpp
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


//#include "ipsUartDev.h"
#include <ipsUdp.h>
//#include "main.h"
#include <ips_globals.hpp>



#define PORT_TEST 1234

//osMutexId_t	lwipReadyMtx=NULL;
//
//osSemaphoreId_t *lwipReadySem=NULL;

extern uint8_t is_lwip_ready;

//
//uint8_t udp_rcv_buffer[UDP_MAX_RCV];
//
//extern UART_HandleTypeDef huart3;
//extern ipsUART uart3;

//udptestTask::udptestTask()
//:ipcThread("UDP TEST",1024,osPriorityNormal)
//{
//};
//
//void udptestTask::taskRunFcn()
//{
//	while(!lwipReadyMtx){osThreadYield();}
//
//	if(osMutexAcquire(lwipReadyMtx, IP_READY_TIMEOUT)<0){
//		Error_Handler();
//	}
//	osMutexRelease(lwipReadyMtx);
//
//	int sock_t;
//	unsigned short port;
//	struct sockaddr_in server, client;
//	char udp_send_b[512];
//	struct sockaddr *pc, *ps;
//	uint32_t l_recieved=0;
//
//	pc= (sockaddr*)&client;
//	ps= (sockaddr*)&server;
//
//	port = htons(PORT_TEST);
//struct sockaddr_in
//	client.sin_family		=AF_INET;
//	client.sin_addr.s_addr	=IPADDR_ANY;
//	client.sin_port			=htons(0x04d2);
//
//	server.sin_family      	= AF_INET;            /* Internet Domain    */
//	server.sin_port        	= port;               /* Server Port        */
//	server.sin_addr.s_addr 	= inet_addr("192.168.0.172"); /* Server's Address   */
//
//
//	if ((sock_t = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
//	   {
//	       Error_Handler();
//	   }
//
//
//
//	for(;;)if ((sock_t = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
//	   {
//	       Error_Handler();
//	   }
//	{
//
//		//sendto(sock_t, udp_send_b, i+1, 0,ps, sizeof(server))
//	for (int i=10;i<510;i++)
//		  {
//				for (int j=0;j<i;j++)
//				{
//					udp_send_b[j]=(j%94)+32;
//				}
//				udp_send_b[i-1]='\r';
//				udp_send_b[i]='\n';
//
//				if (sendto(sock_t, udp_send_b, i+1, 0,ps, sizeof(server)) < 0)
//					   {
//						osDelay(1000);
//						sendto(sock_t, udp_send_b, i+1, 0,ps, sizeof(server));
//					   }
//
//				osDelay(100);
//		  	};
//	}
//
//}



basicUDP::basicUDP(uint32_t ip, uint16_t port,const char *name,uint16_t stack_size,char priority):ufw::rtosThread(name,stack_size,priority)
{
	self.sin_family			=AF_INET;
	self.sin_addr.s_addr	=INADDR_ANY;//IP_ADDR_ANY;
	self.sin_port			=htons(port);

	client.sin_family		=AF_INET;
	client.sin_addr.s_addr	=htonl(ip);
	client.sin_port			=htons(port);

	self_p				=(sockaddr*)&self;
	client_p			=(sockaddr*)&client;


}




basicUDP::basicUDP(const char *ip, uint16_t port,const char *name,uint16_t stack_size,char priority):ufw::rtosThread(name,stack_size,priority)
{
	self.sin_family			=AF_INET;
	self.sin_addr.s_addr	=INADDR_ANY;//IP_ADDR_ANY;
	self.sin_port			=htons(port);

	client.sin_family		=AF_INET;
	client.sin_addr.s_addr	=inet_addr(ip);
	client.sin_port			=htons(port);

	self_p				=(sockaddr*)&self;
	client_p			=(sockaddr*)&client;


}

basicUDP::~basicUDP()
{
  shutdown(sockt, SHUT_RDWR);
  close(sockt);
}


void basicUDP::threadFunction(){
    while (!is_lwip_ready) osDelay(1);

    if ((sockt = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
    	{
    	    Error_Handler();
    	}


    runner();

}

uint32_t basicUDP::read_d(uint8_t *data, uint32_t max_size)
{
	return recv(sockt,data,max_size,0);
}

void basicUDP::write_d(uint8_t *data, uint32_t size)
{
  sendto(sockt, data, size, 0, client_p, sizeof(client));
}

testUDPAudio::testUDPAudio(uint32_t ip, uint16_t port, uint8_t *in_buffer):basicUDP(ip,port,"UDPAtest",512,osPriorityLow)
{
    buffer=in_buffer;
}

testUDPAudio::testUDPAudio(const char *ip, uint16_t port, uint8_t *in_buffer):basicUDP(inet_addr(ip),port,"UDPAtest",512,osPriorityLow)
{
    buffer=in_buffer;
}
void testUDPAudio::setPipe(basicPipe *audio_pipe){
    o_pipe=audio_pipe;
    size=o_pipe->getSize();
}


void testUDPAudio::runner()
{
    while(!o_pipe)osDelay(1);
    while(1){
    	uint32_t recieved_bytes=read_d(buffer, size);
    	while(recieved_bytes<size){
    	    recieved_bytes+=read_d(buffer+recieved_bytes, size-recieved_bytes);
    	}
    	o_pipe->push(buffer);
    }
}

void basicUDP::Remove ()
{
  SheuldeErase(this);
}

void basicUDP::setOptions ()
{
}

UDPAudioIn::UDPAudioIn(uint32_t ip, uint16_t port, uint8_t *in_buffer):basicUDP(ip,port,"UDPAin",512,osPriorityLow)
{
    buffer=in_buffer;
}

UDPAudioIn::UDPAudioIn(const char *ip, uint16_t port, uint8_t *in_buffer):basicUDP(inet_addr(ip),port,"UDPAin",512,osPriorityLow)
{
    buffer=in_buffer;
}
void UDPAudioIn::setPipe(basicPipe *audio_pipe){
    o_pipe=audio_pipe;
    size=o_pipe->getSize();
}


void UDPAudioIn::runner()
{


  if (bind(sockt,self_p,sizeof(self)) < 0)
    {
      Error_Handler();
    }


  while(!o_pipe)osDelay(1);
  while(1){
      uint32_t recieved_bytes=read_d(buffer, size);
      while(recieved_bytes<size){
	  recieved_bytes+=read_d(buffer+recieved_bytes, size-recieved_bytes);
      }
      o_pipe->push(buffer);
  }
}
UDPAudioIn::~UDPAudioIn ()
{

}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
///
///
/// //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
UDPAudioOut::UDPAudioOut(uint32_t ip, uint16_t port, uint8_t *out_buffer):basicUDP(ip,port,"UDPAout",512,osPriorityLow)
{
    buffer=out_buffer;
}


UDPAudioOut::UDPAudioOut(const char *ip, uint16_t port, uint8_t *out_buffer):basicUDP(inet_addr(ip),port,"UDPAout",512,osPriorityLow)
{
    buffer=out_buffer;
}
void UDPAudioOut::setPipe(basicPipe *audio_pipe){
    i_pipe=audio_pipe;
    size=i_pipe->getSize();

}

void UDPAudioOut::runner()
{
    while(!i_pipe)osDelay(1);
    while(1){
	i_pipe->poplk(buffer);

	write_d(buffer, size);

    }
}

UDPAudioOut::~UDPAudioOut ()
{
}

UDPout::UDPout (uint32_t ip, uint16_t port)
{
  client.sin_family		=AF_INET;
  client.sin_addr.s_addr	=htonl(ip);
  client.sin_port		=htons(port);

  buffer= new uint8_t[1550];
  pc=(sockaddr*)&client;
  if ((sockt = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
      	{
      	    Error_Handler();
      	}

}

void UDPout::write_to (uint32_t ip, void *data, uint32_t size)
{
  client.sin_addr.s_addr=htonl(ip);
  int f_mem_tem=xPortGetFreeHeapSize();
  sendto(sockt, data, size, 0, pc, sizeof(client));
}

void* UDPout::writeBuffer ()
{
  return buffer;
}
