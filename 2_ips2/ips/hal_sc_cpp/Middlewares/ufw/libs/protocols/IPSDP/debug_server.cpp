/*
 * debug_server.cpp - a part of CMppCore -an object engine for Cortex-M
 * microcontrollers.
 *
 *  Created on: 31 янв. 2021 г.
 *      Author: Dmitry Donskih
 *
 * Copyright (c) 2019-2021 by Dmitry Donskih
 * All rights reserved.
 *
 * Attention Software modification allowed only with
 * the permission of the author.
 */
#include <protocols/IPSDP/debug_server.h>
#include <codec.h>


#include <system/System.h>
#include <system/configs/device_config.h>
#include <system/configs/ip_setings.h>
#include <allocation/heap/amazon_heap.h>
static ufw::System &Self = ufw::System::instance ();
static heap::amazonHeap &usr_heap = heap::amazonHeap::instance();
//static ufw::DeviceConfig &Conf = ufw::DeviceConfig::Instance ();



namespace ufw
{
  namespace IPSDP
  {
    enum Offsets
    {
      Header = 0,
      Function = sizeof(uint32_t),
      Parameters = sizeof(uint32_t) * 2,
      OutputString = sizeof(uint32_t) * 3
    };
  }
  
  DebugServer::DebugServer () :
      server (13135, 1, "DebugServer")
  {
  }
  
  DebugServer& DebugServer::Instance ()
  {
    static DebugServer server_inst;
    return server_inst;
  }
  
  DebugServer* DebugServer::pointer ()
  {
    return this;
  }
  
  void DebugServer::printf (const char *fmt, ...)
  {
    uint32_t func=IPSDP::Functions::Output;
    memcpy(&m_buffer[0],&IPSDP::magic,sizeof(uint32_t));
    memcpy(&m_buffer[4],&func,sizeof(uint32_t));
    va_list arp;
    va_start(arp, fmt);
    int len = vsnprintf (&m_buffer[8], 504, fmt, arp);
    va_end(arp);
    server.getClientOutput(0)->write_to((uint8_t*)&m_buffer[0], len);
  }

  DebugServer::~DebugServer ()
  {
  }
  

  /*************************************************
   *
   *************************************************/
  DebugHandler::DebugHandler ()
  {
    m_heartbeat_timer=osTimerNew (&DebugHandler::timeoutFunction, osTimerPeriodic, (void*)this, 0);
    osTimerStart(m_heartbeat_timer, 100);
  }

  DebugHandler::~DebugHandler ()
  {
    osTimerStop(m_heartbeat_timer);
    osTimerDelete(m_heartbeat_timer);
  }
  /**
   *
   * @param msg
   * @return
   */
  int DebugHandler::handleMessage (ByteArray *msg)
  {
    switch (Gettype (msg->data ()))
      {
      case IPSDP::Functions::SetIP:
	SetIP(msg->data());
	break;
      case IPSDP::Functions::SetVol:
	SetVol(msg->data());
	break;
      case IPSDP::Functions::Mute:
	AmpMuteToggle ();
	break;
      case IPSDP::Functions::Reboot:
	Self.reboot ();
	break;
      case IPSDP::Functions::GetInfo:
	GetInfo();
	break;
      case IPSDP::Functions::SetCodec:
	SetCodec(msg->data());
	break;
      case IPSDP::Functions::GetStations:
	break;
      case IPSDP::Functions::GetKeys:
	break;
      case IPSDP::Functions::GetGroups:
	break;
      case IPSDP::Functions::SetConfig:
	break;
      case IPSDP::Functions::EraseEEPROM:
	Self.stddebug()<<"IPSDP::EraseEEPROM\n";
	break;
      default:
	break;
      }
  }
  
  void DebugHandler::setWritebackIF (ufw::ioEndpoint *point)
  {
    m_parent = point;
  }

  uint32_t DebugHandler::Gettype (uint8_t *message)
  {
    if (isValid (message))
      {
	return *(uint32_t*) (message + IPSDP::Offsets::Function);
      }
    return 0;
  }

  void DebugHandler::SetIP (uint8_t *msg)
  {
    ufw::IpSetings temp_settings(*((uint32_t*)(msg + IPSDP::Offsets::Parameters)));
    Self.stddebug()<<"SetUp Ip:"<<*((uint32_t*)(msg + IPSDP::Offsets::Parameters))<<"\n"<<temp_settings.ipaddr[0]<<"."<<temp_settings.ipaddr[1]<<"."<<temp_settings.ipaddr[2]<<"."<<temp_settings.ipaddr[3]<<"\n";
    Self.m_settings->saveIp (temp_settings);
  }

  void DebugHandler::SetVol (uint8_t *msg)
  {
    Self.stddebug ()<<"Volume set:\n";
    codecSetOutVolume (*(msg + IPSDP::Offsets::Parameters));
  }

  void DebugHandler::GetInfo ()
  {
    m_buffer.clear ();
    m_buffer.push_back<uint32_t> (IPSDP::magic);
    m_buffer.push_back<uint32_t> (IPSDP::Functions::GetInfo);
    for (int i = 0; i < Self.m_settings->serial.size (); i++)
      {
	m_buffer.push_back (Self.m_settings->serial[i]);
      }
    for (int i = 0; i < Self.m_settings->mac.size (); i++)
      {
	m_buffer.push_back (Self.m_settings->mac[i]);
      }
    m_parent->write_to(m_buffer.data(), m_buffer.size());

  }



  void DebugHandler::timeoutFunction (void *arg)
  {
    static_cast<DebugHandler*>(arg)->Heartbeat();
  }

  void DebugHandler::Heartbeat ()
  {
    m_hb_buffer.clear ();
    m_hb_buffer.push_back<uint32_t> (IPSDP::magic);
    m_hb_buffer.push_back<uint32_t> (IPSDP::Functions::Heartbeat);
    m_hb_buffer.push_back<uint32_t>(usr_heap.xPortGetFreeHeapSize());
    m_hb_buffer.push_back(xPortGetFreeHeapSize());
    m_parent->write_to(m_hb_buffer.data(), m_hb_buffer.size());

  }

  void DebugHandler::SetCodec (uint8_t *msg)
  {
    Self.stddebug () << "codecRegWriteP(" << *(msg + IPSDP::Offsets::Parameters) << ", "
	<< *(msg + IPSDP::Offsets::Parameters + 1) << ", "
	<< *(msg + IPSDP::Offsets::Parameters + 2) << ")\n";
//    codecRegWriteP(*(msg+IPSDP::Offsets::Parameters), *(msg+IPSDP::Offsets::Parameters+1),  *(msg+IPSDP::Offsets::Parameters+2));
  }

} /* namespace ufw */
