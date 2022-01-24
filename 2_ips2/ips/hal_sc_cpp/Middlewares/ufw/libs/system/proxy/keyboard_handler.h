/*
 * keyboard_handler.h - a part of ufw++ object envirinment for
 * microcontrollers.
 *
 *   Created on: 24 дек. 2020 г.
 *       Author: Dmitry Donskih
 *
 * Copyright (c) 2019-2020 by Dmitry Donskih
 * All rights reserved.
 * 
 * Attention Software modification allowed only with
 * the permission of the author.
 * 
 */

#ifndef UFW_LIBS_SYSTEM_PROXY_KEYBOARD_HANDLER_H_
#define UFW_LIBS_SYSTEM_PROXY_KEYBOARD_HANDLER_H_

#include "FreeRTOS.h"
#include "cmsis_os2.h"
#include "task.h"
#include "queue.h"

namespace ufw
{
  struct Event
  {
      uint32_t op;
      uint16_t owner;
      uint16_t callee;
  };
  
  class KeyboardHandler
  {
    public:
      bool sendMsg (Event *ptr);
      static void run (void *par);
      static KeyboardHandler& Instance ();
      KeyboardHandler* pointer();
    private:
      xQueueHandle messageQueue;
      xTaskHandle handle;
    protected:
      KeyboardHandler (uint8_t msgQueueSize);
      bool receiveMsg (Event *ptr);
      void messageHandler ();
      void MicOff();

  };

} /* namespace ufw */

#endif /* UFW_LIBS_SYSTEM_PROXY_KEYBOARD_HANDLER_H_ */
