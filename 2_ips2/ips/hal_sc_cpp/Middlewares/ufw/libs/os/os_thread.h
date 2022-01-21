/*
 * os_thread.h
 *
 *  Created on: 3 дек. 2020 г.
 *      Author: Dmitry Donskih
 *  Copyright © 2020 Dmitry Donskih. All rights reserved.
 */

#ifndef LIBS_OS_OS_THREAD_H_
#define LIBS_OS_OS_THREAD_H_

#include "FreeRTOS.h"
#include "task.h"
#include "cmsis_os2.h"

namespace ufw {
  
  /*
   *
   */
  class rtosThread
    {
    public:
      rtosThread();
      rtosThread(const char *name, unsigned short stackDepth, char priority);
      void setPriority(char priority);
      void SheuldeErase(void* self);
      void Stop();

      xTaskHandle getTaskHandle();

      virtual ~rtosThread();

    protected:
      TaskHandle_t taskHandle;

      virtual void threadFunction() =0;
      static void pvTaskCode(void *pvParameters);

    private:
      rtosThread(const rtosThread &other) = delete;
      rtosThread(rtosThread &&other) = delete;
      rtosThread& operator=(const rtosThread &other) = delete;
      rtosThread& operator=(rtosThread &&other) = delete;
    };

} /* namespace ufw */

#endif /* LIBS_OS_OS_THREAD_H_ */
