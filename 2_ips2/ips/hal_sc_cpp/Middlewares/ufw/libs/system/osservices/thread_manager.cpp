/*
 * thread_manager.cpp - a part of ufw++ object envirinment for
 * microcontrollers.
 *
 *   Created on: 22 янв. 2021 г.
 *       Author: Dmitry Donskih
 *
 * Copyright (c) 2019-2020 by Dmitry Donskih
 * All rights reserved.
 * 
 * Attention Software modification allowed only with
 * the permission of the author.
 * 
 */

#include <system/osservices/thread_manager.h>
#include <os/os_thread.h>
#include <lib_asserts.h>
#include <system/System.h>
static ufw::System& Self = ufw::System::instance();

static const size_t queue_depth =10;
//static void runner(void* object)
//{
//  static_cast<ufw::ThreadManager*>(object)->Instance().
//}

namespace ufw
{
  
  ThreadManager& ThreadManager::Instance ()
  {
    static ThreadManager instance_h;
    return instance_h;
  }
  
  ThreadManager* ThreadManager::pointer ()
  {
    return this;
  }
  
  bool ThreadManager::sendMsg (thread_control_t *ptr)
  {
    return xQueueSend(messageQueue, (void* )ptr, 0);
  }
  
  bool ThreadManager::receiveMsg (thread_control_t *ptr)
  {
    return xQueueReceive (messageQueue, (void*) ptr, portMAX_DELAY);
  }
  
  void ThreadManager::messageHandler ()
  {
    Self.stddebug()<<"Thread management runs\n";
    thread_control_t message;
    while (1)
      {
	receiveMsg(&message);
	switch (message.op)
	  {
	  case threadControl::Clean:
	    delete (ufw::rtosThread*)(message.parameter);
	    break;
	  case threadControl::Stop:
	    stop_task (*(xTaskHandle*) message.parameter);
	    break;
	  case threadControl::Pause:
	    pause_task (*(xTaskHandle*) message.parameter);
	    break;
	  case threadControl::Resume:
	    resume_task (*(xTaskHandle*) message.parameter);
	    break;
	  case threadControl::Eliminate:
	    delete message.parameter;
	    break;
	  default:
	    break;
	  }
      }
  }
  
  void ThreadManager::stop_task (xTaskHandle task)
  {
    vTaskDelete (task);
  }
  
  void ThreadManager::delete_object (void *obj)
  {
  }
  
  void ThreadManager::pause_task (xTaskHandle task)
  {
    vTaskSuspend(task);
  }
  
  void ThreadManager::resume_task (xTaskHandle task)
  {
    vTaskResume(task);
  }
  
  ThreadManager::ThreadManager ()
  {
    messageQueue = xQueueCreate(queue_depth, sizeof(thread_control_t));
    xTaskCreate (runner, "Task Management", 256, (void*) this, osPriorityLow, &handle);
  }
  
  void ThreadManager::runner (void *object)
  {
    static_cast<ufw::ThreadManager*> (object)->messageHandler ();
  }

  ThreadManager::~ThreadManager ()
  {
  }

} /* namespace ufw */
