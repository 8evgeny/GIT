/*
 * locatorUDP.cpp
 *
 *  Created on: 30 сент. 2020 г.
 *      Author: vader
 */

#include <locatorUDP.hpp>
#include <ipssystem.hpp>
#include <ips_globals.hpp>
#include <system_c.h>

//locatorUDP::locatorUDP():basicUDP("0.0.0.0",13121,"UDP Locator",512,osPriorityLow)
//    {
//    }
//
//locatorUDP& locatorUDP::getInstance()
//    {
//    static locatorUDP instance;
//    return instance;
//    }
//
//locatorUDP::~locatorUDP()
//    {
//    }
//
//void locatorUDP::t_runner()
//    {
//    uint8_t *buffer=(uint8_t*)system_cpp.getBuffer();
//    uint32_t transfer_length=0;
//    for(;;){
//	transfer_length=recvfrom(sockt,buffer,1500,NULL,(sockaddr*)&sender,&sender_length);
//	ipsSystem::debug->printfl("Gets %d bytes! Try sennd back\n",transfer_length);
//	sysLedTestToggle();
//	sender.sin_addr.s_addr|=0xFF000000;
//	transfer_length=sendto(sockt,buffer,transfer_length,NULL,(const sockaddr*)&sender,sender_length);
//	ipsSystem::debug->printfl("Send back %d bytes\n",transfer_length);
//    }
//    }
