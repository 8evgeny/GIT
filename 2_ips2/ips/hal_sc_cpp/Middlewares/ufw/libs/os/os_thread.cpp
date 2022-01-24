/*
 * os_thread.cpp
 *
 *  Created on: 3 дек. 2020 г.
 *      Author: Dmitry Donskih
 *  Copyright © 2020 Dmitry Donskih. All rights reserved.
 */

#include "os_thread.h"
#include <system/System.h>
#include <system/osservices/thread_manager.h>

namespace ufw {
  
  rtosThread::rtosThread()
  :rtosThread("Default",512,osPriorityLow)
  {
  }
  
  rtosThread::rtosThread (const char *name, unsigned short stackDepth, char priority)
  {
    xTaskCreate(pvTaskCode, (const char *) name, stackDepth, (void*) this, priority, &taskHandle);
  }
  
  void rtosThread::setPriority (char priority)
  {
    vTaskPrioritySet(taskHandle, priority);
  }
  
  void rtosThread::SheuldeErase (void *self)
  {
    thread_control_t stop_msg={.op=ufw::threadControl::Clean,.parameter=self};
    ufw::ThreadManager::Instance().sendMsg(&stop_msg);
  }
  
  xTaskHandle rtosThread::getTaskHandle ()
  {
    return taskHandle;
  }

  rtosThread::~rtosThread()
  {
    int some_shit=0;
    some_shit=123;
    ufw::System::instance().stddebug()<<__PRETTY_FUNCTION__<<"\n";
  }
  
  void rtosThread::Stop ()
  {
    vTaskDelete(taskHandle);
  }

  void rtosThread::pvTaskCode (void *pvParameters)
  {
    (static_cast<rtosThread*>(pvParameters))->threadFunction();
  }

} /* namespace ufw */
