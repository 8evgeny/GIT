/*
 * thread_manager.h - a part of ufw++ object envirinment for
 * microcontrollers.
 *
 *       Author: Dmitry Donskih
 *
 * Copyright (c) 2019-2020 by Dmitry Donskih
 * All rights reserved.
 * 
 * Attention Software modification allowed only with
 * the permission of the author.
 * 
 */

#ifndef UFW_LIBS_SYSTEM_OSSERVICES_THREAD_MANAGER_H_
#define UFW_LIBS_SYSTEM_OSSERVICES_THREAD_MANAGER_H_

#include <cstdint>
#include "FreeRTOS.h"
#include "cmsis_os2.h"
#include "task.h"
#include "queue.h"

namespace ufw
{
  namespace threadControl
  {
    enum
    {
      Clean = (uint8_t) 1,
      Stop = (uint8_t) 2,
      Eliminate = (uint8_t) 3,
      Pause = (uint8_t) 4,
      Resume = (uint8_t) 5
    };
  }

  typedef struct
  {
      uint8_t op;
      void *parameter;
  } thread_control_t;
  
  class ThreadManager
  {
    public:
      static ThreadManager& Instance ();
      ThreadManager* pointer ();

      bool sendMsg (thread_control_t *ptr);

    protected:
      bool receiveMsg (thread_control_t *ptr);
      void messageHandler ();
      void stop_task (xTaskHandle task);
      void delete_object (void *obj);
      void pause_task (xTaskHandle task);
      void resume_task (xTaskHandle task);

    private:
      xQueueHandle messageQueue;
      xTaskHandle handle;

      static void runner(void* object);

      ThreadManager ();
      ~ThreadManager ();
      ThreadManager (const ThreadManager &other) = delete;
      ThreadManager (ThreadManager &&other) = delete;
      ThreadManager& operator= (const ThreadManager &other) = delete;
      ThreadManager& operator= (ThreadManager &&other) = delete;

  };

} /* namespace ufw */

#endif /* UFW_LIBS_SYSTEM_OSSERVICES_THREAD_MANAGER_H_ */
