/*
 * announcer_test.h - a part of ufw++ object envirinment for
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

#pragma once

//
#include <io/net/multicast.h>
#include <devices/stream_device.h>
//#include <system/System.h>

namespace ufw
{
  
  class AnnouncerTest :public ttY
  {
    public:
      AnnouncerTest ();
      ~AnnouncerTest ();
      void announce(const char* str);
      void announce();
      virtual void write_t(const char *buf, size_t size) override ;
    protected:

//
//      System& m_sys = ufw::System::instance(); // System::instance();
      multicastSocketOut m_output;
  };

} /* namespace ufw */


