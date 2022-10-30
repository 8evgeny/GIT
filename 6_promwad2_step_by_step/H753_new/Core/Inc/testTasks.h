#pragma once
#include "main.h"
#include "gpio.h"

#ifdef __cplusplus
extern "C" {
#endif
//const uint32_t keyXor[8] = {
//    0x10842108, 0xababefef, 0xfe42fe87, 0x23567adc, 0xcefa13f5, 0xabcdef, 0xfefefefe, 0x42435788
//};
const uint8_t key [16]{'1','2','3','4','5','6','7','8','1','2','3','4','5','6','7','8'};

void simpleLedTest1_RTOS(void const *argument);
void simpleLedTest2_RTOS(void const *argument);
void simpleLedTest3_RTOS(void const *argument);
void simpletestUART_RTOS(void const *argument);
void TasksLog(void const *argument);

void testLed1();
void testLed2();
void watchDog();
void testUART();
void testTasksLog();
void testSendMcast();
void testReceiveMcast();
void pinNormaStart();
void pinMkStart();
void writeVolSensToFlashStart();
void testXOR();
void xorEncoding(const char* input, uint32_t inputLength, const char* key, uint8_t keyLength, char* output);

#ifdef __cplusplus
}
#endif

#include "rs232.h"
void test_UART();
