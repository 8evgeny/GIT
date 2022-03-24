#ifndef FREERTOS_CONFIG_H
#define FREERTOS_CONFIG_H

#include <stdint.h>
#include "MDR32Fx.h"

extern uint32_t SystemCoreClock;

#define configCPU_CLOCK_HZ                    (SystemCoreClock)
#define configTICK_RATE_HZ                    ((TickType_t)1000)
#define configTOTAL_HEAP_SIZE                 ((size_t)(10 * 1024))
#define configMINIMAL_STACK_SIZE              ((unsigned short)130)
#define configCHECK_FOR_STACK_OVERFLOW        0
#define configMAX_PRIORITIES                  (5)
#define configUSE_PREEMPTION                  1
#define configIDLE_SHOULD_YIELD               1
#define configMAX_TASK_NAME_LEN               (10)

#define configUSE_TIMERS                      0
#define configTIMER_TASK_PRIORITY             (2)
#define configTIMER_QUEUE_LENGTH              5
#define configTIMER_TASK_STACK_DEPTH          (configMINIMAL_STACK_SIZE * 2)

#define configUSE_MUTEXES                     1
#define configUSE_RECURSIVE_MUTEXES           1
#define configUSE_COUNTING_SEMAPHORES         1
#define configUSE_QUEUE_SETS                  1
#define configUSE_IDLE_HOOK                   0
#define configUSE_TICK_HOOK                   0
#define configUSE_MALLOC_FAILED_HOOK          0
#define configUSE_16_BIT_TICKS                0

#define INCLUDE_vTaskPrioritySet              1
#define INCLUDE_uxTaskPriorityGet             1
#define INCLUDE_vTaskDelete                   1
#define INCLUDE_vTaskSuspend                  1
#define INCLUDE_vTaskDelayUntil               1
#define INCLUDE_vTaskDelay                    1
#define INCLUDE_eTaskGetState                 1

#ifdef __NVIC_PRIO_BITS
  #define configPRIO_BITS                     __NVIC_PRIO_BITS
#else
  #define configPRIO_BITS                     4
#endif

#define configLIBRARY_LOWEST_INTERRUPT_PRIORITY   15
#define configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY   5
#define configKERNEL_INTERRUPT_PRIORITY   ( configLIBRARY_LOWEST_INTERRUPT_PRIORITY << (8 - configPRIO_BITS) )
#define configMAX_SYSCALL_INTERRUPT_PRIORITY   (configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY << (8 - configPRIO_BITS))

#define configASSERT( x ) if( ( x ) == 0 ) { taskDISABLE_INTERRUPTS(); for( ;; ); } 

#define xPortPendSVHandler                    PendSV_Handler
#define vPortSVCHandler                       SVC_Handler
#define xPortSysTickHandler                   SysTick_Handler

#include "freertos_evr.h"

#endif /* FREERTOS_CONFIG_H */
