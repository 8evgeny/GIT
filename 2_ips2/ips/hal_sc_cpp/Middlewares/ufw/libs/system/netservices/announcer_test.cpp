/*
 * announcer_test.cpp - a part of ufw++ object envirinment for
 * microcontrollers.
 *
 *   Created on: 22 дек. 2020 г.
 *       Author: Dmitry Donskih
 *
 * Copyright (c) 2019-2020 by Dmitry Donskih
 * All rights reserved.
 * 
 * Attention Software modification allowed only with
 * the permission of the author.
 * 
 */

#include <system/netservices/announcer_test.h>
#include <cstring>
#include <system/System.h>
#include <system/configs/device_config.h>
//static ufw::DeviceConfig &Conf = ufw::DeviceConfig::Instance();
static ufw::System& Self = ufw::System::instance();
extern __IO uint32_t uwTick;

namespace ufw
{
  /**
   *
   */
  AnnouncerTest::AnnouncerTest ()
  : ufw::dev::streamDevice(1024),
   m_output("239.168.10.111",13131)
  {
    m_output.config();
  }
  /**
   *
   * @param str
   */
  void AnnouncerTest::announce (const char *str)
  {
    m_output.mcast_send((uint8_t*)str, strlen(str));
  }
  /**
   *
   */
  void AnnouncerTest::announce ()
  {
//    (*(ttY*)this)<<"ufwlib4 v0.41 Multicast module announce\n"<<"User RAM1: " << Self.MemoryManager().FreeUserHeap() << "\nSystem RAM: "<< Self.MemoryManager().FreeSysHeap()<<"\n";
    this->printfl("ufwlib4 v0.41 Device announce!\nBSP: Git-Comm IPS board SC!\n IP:%d.%d.%d.%d \n",Self.m_settings->ipaddr[0],Self.m_settings->ipaddr[1],Self.m_settings->ipaddr[2],Self.m_settings->ipaddr[3]);
  }
  /**
   *
   */
  AnnouncerTest::~AnnouncerTest ()
  {
    delete [] linebuffer;
  }
  /**
   *
   * @param buf
   * @param size
   */
  void AnnouncerTest::write_t (const char *buf, size_t size)
  {
    m_output.mcast_send(reinterpret_cast<uint8_t*>(linebuffer), size);
  }

} /* namespace ufw */
