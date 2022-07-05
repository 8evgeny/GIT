#pragma once
#include "main.h"
#include "gpio_sc2_board.h"

#ifdef __cplusplus
extern "C" {
#endif

void simpleLedTest1_RTOS(void const *argument);
void simpleLedTest2_RTOS(void const *argument);
void simpleLedTest3_RTOS(void const *argument);
void simpletestUART_RTOS(void const *argument);
void TasksLog(void const *argument);
void SendMcast(void const *argument);
void ReceiveMcast(void const *argument);

void testLed1();
void testLed2();
void testLed3();
void testUART();
void testTasksLog();
void testSendMcast();
void testReceiveMcast();

#ifdef __cplusplus
}
#endif

#include "rs232.h"
void test_UART();
