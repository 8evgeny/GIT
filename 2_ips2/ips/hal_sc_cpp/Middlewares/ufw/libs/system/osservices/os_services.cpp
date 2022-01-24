/*
 * os_services.cpp - a part of ufw++ object envirinment for
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

#include <system/osservices/os_services.h>
#include <lib_asserts.h>
#include <system/System.h>
static ufw::System& Self = ufw::System::instance();
extern __IO uint32_t uwTick;

namespace ufw
{
  
  osServices& osServices::Instance ()
  {
    static osServices s_services_host;
    return s_services_host;
  }
  
  osServices* osServices::pointer ()
  {
    return this;
  }
  
  osServices::osServices ()
  {
    if(!Self.isOSloaded())LIB_ERROR_HANDL;
    Self.stddebug()<<"Thread management starts!\n";
    ThreadManager::Instance();
    TaskManager=sysTaskManagement::getInstance().pointer();
    Self.stddebug()<<uwTick<<" Task management loaded\n";
    osDelay(10);
    outputs=DOUnit::getInstance().pointer();
    Self.stddebug()<<uwTick<<" Digital outputs created "<<outputs->getCount()<<" units\n";
    osDelay(10);
    //while(!Self.isIPready())osDelay(5);

  }

  osServices::~osServices ()
  {
  }

  void osServices::AfterIP ()
  {
    osDelay(10);
    inputs=DIUnit::GetInstance().pointer();
    Self.stddebug()<<uwTick<<" Digital outputs created.\n";
    keyboard_svc=KeyboardHandler::Instance().pointer();
    Self.stddebug()<<uwTick<<" Keyboard are handled.\n";
  }

} /* namespace ufw */
