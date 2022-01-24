/*
 * keyboard_handler.cpp - a part of ufw++ object envirinment for
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

#include <system/proxy/keyboard_handler.h>
#include <system/proxy/call_controller.h>
#include <basicDI.hpp>
#include <codec.h>



ufw::KeyboardHandler* input_handle_pointer=nullptr;

namespace ufw
{
  
  bool KeyboardHandler::sendMsg (Event *ptr)
  {
    return xQueueSend(messageQueue, (void* )ptr, 0);
  }
  
  void KeyboardHandler::run (void *par)
  {
    static_cast<KeyboardHandler*> (par)->messageHandler ();
  }
  
  KeyboardHandler& KeyboardHandler::Instance ()
  {
    static KeyboardHandler s_kb_handler(20);
    input_handle_pointer=s_kb_handler.pointer();
    return s_kb_handler;
  }
  
  KeyboardHandler* KeyboardHandler::pointer ()
  {
    return this;
  }
  
  KeyboardHandler::KeyboardHandler (uint8_t msgQueueSize)
  {
    messageQueue = xQueueCreate(msgQueueSize, sizeof(Event));
    ufw::CallController::Instance();
    xTaskCreate (run, "KeyboardHandler", (4*1024), (void*) this, osPriorityNormal, &handle);
  }
  
  bool KeyboardHandler::receiveMsg (Event *ptr)
  {
    return xQueueReceive (messageQueue, (void*) ptr, portMAX_DELAY);
  }
  
  void KeyboardHandler::messageHandler ()
  {
    Event rx;
    while (1)
      {
	receiveMsg(&rx);
	switch (rx.op)
		{
		case KeyboardRise:
		  ufw::CallController::Instance().KbdRaise(rx.owner);
		  break;
		case KeyboardFall:
		  ufw::CallController::Instance().KbdFall(rx.owner);
		  break;

		default:
		  break;
		}
      }
  }
  
  void KeyboardHandler::MicOff ()
  {
  }

} /* namespace ufw */
