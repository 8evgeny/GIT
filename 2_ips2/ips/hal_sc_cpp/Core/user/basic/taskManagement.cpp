/*
 * taskManagement.cpp
 *
 *  Created on: 13 авг. 2020 г.
 *      Author: Dmitry Dmitry Donskih
 */

#include "main.h"
#include <taskManagement.hpp>
#include <ips_globals.hpp>
#include "FreeRTOS.h"

sysTaskManagement::sysTaskManagement (uint8_t msgQueueSize = 10)
{
  messageQueue = xQueueCreate(msgQueueSize, sizeof(taskAction_t));
  xTaskCreate (run, "OLD Management", 256, (void*) this, osPriorityNormal, &handle);
}

bool sysTaskManagement::receiveMsg (taskAction_t *ptr)
{
  return xQueueReceive (messageQueue, (void*) ptr, portMAX_DELAY);
}

void sysTaskManagement::messageHandler ()
{
  taskAction_t message_rx;
  while (1)
    {
      receiveMsg (&message_rx);
      switch (message_rx.op)
	{
	case taskManagement::Eliminate:
	  kill_task (message_rx.task);
	  delete_object (message_rx.object);
	  break;
	case taskManagement::Kill:
	  kill_task (message_rx.task);
	  break;
	case taskManagement::Lock:

	  break;
	case taskManagement::Release:

	  break;
	case taskManagement::DuplexCall:
	  break;
	default:
	  break;
	}
    }

}

void sysTaskManagement::kill_task (xTaskHandle task)
{
  vTaskDelete (task);
}

void sysTaskManagement::delete_object (void *obj)
{
  if (obj) vPortFree (obj);
}

void sysTaskManagement::lock_task (xTaskHandle task)
{
}

void sysTaskManagement::unlock_task (xTaskHandle task)
{
}

sysTaskManagement& sysTaskManagement::getInstance ()
{
  static sysTaskManagement instance;
  return instance;
}

bool sysTaskManagement::sendMsg (taskAction_t *ptr)
{
  return xQueueSend(messageQueue, (void* )ptr, 0);
}

void sysTaskManagement::eliminate (void *obj, xTaskHandle task)
{
  taskAction_t eliminator =
    { .op = taskManagement::Eliminate, .object = obj, .task = task };
  sendMsg (&eliminator);
}

void sysTaskManagement::kill (xTaskHandle task)
{
  taskAction_t app_killer =
    { .op = taskManagement::Kill, .object = nullptr, .task = task };
  sendMsg (&app_killer);
}

void sysTaskManagement::run (void *par)
{
  static_cast<sysTaskManagement*> (par)->messageHandler ();
}

sysTaskManagement* sysTaskManagement::pointer ()
{
  return this;
}
