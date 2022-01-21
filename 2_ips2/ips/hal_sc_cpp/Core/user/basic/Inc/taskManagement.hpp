/*
 * taskManagement.hpp
 *
 *  Created on: 13 авг. 2020 г.
 *      Author: Dmitry Donskih
 */

#ifndef USER_BASIC_INC_TASKMANAGEMENT_HPP_
#define USER_BASIC_INC_TASKMANAGEMENT_HPP_

#include <cstdint>
#include "FreeRTOS.h"
#include "cmsis_os2.h"
#include "task.h"
#include "queue.h"

namespace taskManagement
{
  enum
  {
    Release = (uint8_t) 0,
    Lock = (uint8_t) 1,
    Kill = (uint8_t) 2,
    Eliminate = (uint8_t) 3,
    SimplexCall= (uint8_t) 4,
    DuplexCall=(uint8_t) 5
  };
}

typedef struct
{
    uint8_t op;
    void *object;
    xTaskHandle task;
} taskAction_t;

class sysTaskManagement
{
  private:
    xQueueHandle messageQueue;
    xTaskHandle handle;
  protected:
    sysTaskManagement (uint8_t msgQueueSize);
    sysTaskManagement (const sysTaskManagement&);
    sysTaskManagement& operator= (sysTaskManagement&);
    bool receiveMsg (taskAction_t *ptr);
    void messageHandler ();
    void kill_task (xTaskHandle task);
    void delete_object (void *obj);
    void lock_task (xTaskHandle task);
    void unlock_task (xTaskHandle task);

  public:
    bool sendMsg (taskAction_t *ptr);
    void eliminate (void *obj, xTaskHandle task);
    void kill (xTaskHandle task);
    static void run (void *par);
    static sysTaskManagement& getInstance ();
    sysTaskManagement* pointer();
};

extern void vApplicationStackOverflowHook (xTaskHandle xTask, signed char *pcTaskName);

#endif /* USER_BASIC_INC_TASKMANAGEMENT_HPP_ */
