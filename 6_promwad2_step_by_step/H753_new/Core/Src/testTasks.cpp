#include "testTasks.h"
#include <stdbool.h>
#include <stdint.h>
#include "cmsis_os.h"
#include "../Debug/debug.h"
#include "rs232_printf.h"

#include "lwip.h"
#include "lwip/init.h"
#include "lwip/netif.h"
#include "ethernetif.h"
#include "SEGGER_RTT.h"
#include <stdio.h>
volatile int _Cnt;
volatile int _Delay;
static char r;
extern uint8_t boardType;
extern unsigned char zvon3_raw[];
extern unsigned int zvon3_raw_len;
extern uint8_t TLC59116F_max_address;
#ifdef __cplusplus
extern "C" {
#endif

extern SAI_HandleTypeDef audioTxSai;
extern osSemaphoreId Netif_LinkSemaphore;

void SeggerTest(void) {

  SEGGER_RTT_ConfigUpBuffer(0, NULL, NULL, 0, SEGGER_RTT_MODE_BLOCK_IF_FIFO_FULL);

  SEGGER_RTT_WriteString(0, "SEGGER Real-Time-Terminal Sample\r\n\r\n");
  SEGGER_RTT_WriteString(0, "###### Testing SEGGER_printf() ######\r\n");

  SEGGER_RTT_printf(0, "printf Test: %%c,         'S' : %c.\r\n", 'S');
  SEGGER_RTT_printf(0, "printf Test: %%5c,        'E' : %5c.\r\n", 'E');
  SEGGER_RTT_printf(0, "printf Test: %%-5c,       'G' : %-5c.\r\n", 'G');
  SEGGER_RTT_printf(0, "printf Test: %%5.3c,      'G' : %-5c.\r\n", 'G');
  SEGGER_RTT_printf(0, "printf Test: %%.3c,       'E' : %-5c.\r\n", 'E');
  SEGGER_RTT_printf(0, "printf Test: %%c,         'R' : %c.\r\n", 'R');

  SEGGER_RTT_printf(0, "printf Test: %%s,      \"RTT\" : %s.\r\n", "RTT");
  SEGGER_RTT_printf(0, "printf Test: %%s, \"RTT\\r\\nRocks.\" : %s.\r\n", "RTT\r\nRocks.");

  SEGGER_RTT_printf(0, "printf Test: %%u,       12345 : %u.\r\n", 12345);
  SEGGER_RTT_printf(0, "printf Test: %%+u,      12345 : %+u.\r\n", 12345);
  SEGGER_RTT_printf(0, "printf Test: %%.3u,     12345 : %.3u.\r\n", 12345);
  SEGGER_RTT_printf(0, "printf Test: %%.6u,     12345 : %.6u.\r\n", 12345);
  SEGGER_RTT_printf(0, "printf Test: %%6.3u,    12345 : %6.3u.\r\n", 12345);
  SEGGER_RTT_printf(0, "printf Test: %%8.6u,    12345 : %8.6u.\r\n", 12345);
  SEGGER_RTT_printf(0, "printf Test: %%08u,     12345 : %08u.\r\n", 12345);
  SEGGER_RTT_printf(0, "printf Test: %%08.6u,   12345 : %08.6u.\r\n", 12345);
  SEGGER_RTT_printf(0, "printf Test: %%0u,      12345 : %0u.\r\n", 12345);
  SEGGER_RTT_printf(0, "printf Test: %%-.6u,    12345 : %-.6u.\r\n", 12345);
  SEGGER_RTT_printf(0, "printf Test: %%-6.3u,   12345 : %-6.3u.\r\n", 12345);
  SEGGER_RTT_printf(0, "printf Test: %%-8.6u,   12345 : %-8.6u.\r\n", 12345);
  SEGGER_RTT_printf(0, "printf Test: %%-08u,    12345 : %-08u.\r\n", 12345);
  SEGGER_RTT_printf(0, "printf Test: %%-08.6u,  12345 : %-08.6u.\r\n", 12345);
  SEGGER_RTT_printf(0, "printf Test: %%-0u,     12345 : %-0u.\r\n", 12345);

  SEGGER_RTT_printf(0, "printf Test: %%u,      -12345 : %u.\r\n", -12345);
  SEGGER_RTT_printf(0, "printf Test: %%+u,     -12345 : %+u.\r\n", -12345);
  SEGGER_RTT_printf(0, "printf Test: %%.3u,    -12345 : %.3u.\r\n", -12345);
  SEGGER_RTT_printf(0, "printf Test: %%.6u,    -12345 : %.6u.\r\n", -12345);
  SEGGER_RTT_printf(0, "printf Test: %%6.3u,   -12345 : %6.3u.\r\n", -12345);
  SEGGER_RTT_printf(0, "printf Test: %%8.6u,   -12345 : %8.6u.\r\n", -12345);
  SEGGER_RTT_printf(0, "printf Test: %%08u,    -12345 : %08u.\r\n", -12345);
  SEGGER_RTT_printf(0, "printf Test: %%08.6u,  -12345 : %08.6u.\r\n", -12345);
  SEGGER_RTT_printf(0, "printf Test: %%0u,     -12345 : %0u.\r\n", -12345);
  SEGGER_RTT_printf(0, "printf Test: %%-.6u,   -12345 : %-.6u.\r\n", -12345);
  SEGGER_RTT_printf(0, "printf Test: %%-6.3u,  -12345 : %-6.3u.\r\n", -12345);
  SEGGER_RTT_printf(0, "printf Test: %%-8.6u,  -12345 : %-8.6u.\r\n", -12345);
  SEGGER_RTT_printf(0, "printf Test: %%-08u,   -12345 : %-08u.\r\n", -12345);
  SEGGER_RTT_printf(0, "printf Test: %%-08.6u, -12345 : %-08.6u.\r\n", -12345);
  SEGGER_RTT_printf(0, "printf Test: %%-0u,    -12345 : %-0u.\r\n", -12345);

  SEGGER_RTT_printf(0, "printf Test: %%d,      -12345 : %d.\r\n", -12345);
  SEGGER_RTT_printf(0, "printf Test: %%+d,     -12345 : %+d.\r\n", -12345);
  SEGGER_RTT_printf(0, "printf Test: %%.3d,    -12345 : %.3d.\r\n", -12345);
  SEGGER_RTT_printf(0, "printf Test: %%.6d,    -12345 : %.6d.\r\n", -12345);
  SEGGER_RTT_printf(0, "printf Test: %%6.3d,   -12345 : %6.3d.\r\n", -12345);
  SEGGER_RTT_printf(0, "printf Test: %%8.6d,   -12345 : %8.6d.\r\n", -12345);
  SEGGER_RTT_printf(0, "printf Test: %%08d,    -12345 : %08d.\r\n", -12345);
  SEGGER_RTT_printf(0, "printf Test: %%08.6d,  -12345 : %08.6d.\r\n", -12345);
  SEGGER_RTT_printf(0, "printf Test: %%0d,     -12345 : %0d.\r\n", -12345);
  SEGGER_RTT_printf(0, "printf Test: %%-.6d,   -12345 : %-.6d.\r\n", -12345);
  SEGGER_RTT_printf(0, "printf Test: %%-6.3d,  -12345 : %-6.3d.\r\n", -12345);
  SEGGER_RTT_printf(0, "printf Test: %%-8.6d,  -12345 : %-8.6d.\r\n", -12345);
  SEGGER_RTT_printf(0, "printf Test: %%-08d,   -12345 : %-08d.\r\n", -12345);
  SEGGER_RTT_printf(0, "printf Test: %%-08.6d, -12345 : %-08.6d.\r\n", -12345);
  SEGGER_RTT_printf(0, "printf Test: %%-0d,    -12345 : %-0d.\r\n", -12345);

  SEGGER_RTT_printf(0, "printf Test: %%x,      0x1234ABC : %x.\r\n", 0x1234ABC);
  SEGGER_RTT_printf(0, "printf Test: %%+x,     0x1234ABC : %+x.\r\n", 0x1234ABC);
  SEGGER_RTT_printf(0, "printf Test: %%.3x,    0x1234ABC : %.3x.\r\n", 0x1234ABC);
  SEGGER_RTT_printf(0, "printf Test: %%.6x,    0x1234ABC : %.6x.\r\n", 0x1234ABC);
  SEGGER_RTT_printf(0, "printf Test: %%6.3x,   0x1234ABC : %6.3x.\r\n", 0x1234ABC);
  SEGGER_RTT_printf(0, "printf Test: %%8.6x,   0x1234ABC : %8.6x.\r\n", 0x1234ABC);
  SEGGER_RTT_printf(0, "printf Test: %%08x,    0x1234ABC : %08x.\r\n", 0x1234ABC);
  SEGGER_RTT_printf(0, "printf Test: %%08.6x,  0x1234ABC : %08.6x.\r\n", 0x1234ABC);
  SEGGER_RTT_printf(0, "printf Test: %%0x,     0x1234ABC : %0x.\r\n", 0x1234ABC);
  SEGGER_RTT_printf(0, "printf Test: %%-.6x,   0x1234ABC : %-.6x.\r\n", 0x1234ABC);
  SEGGER_RTT_printf(0, "printf Test: %%-6.3x,  0x1234ABC : %-6.3x.\r\n", 0x1234ABC);
  SEGGER_RTT_printf(0, "printf Test: %%-8.6x,  0x1234ABC : %-8.6x.\r\n", 0x1234ABC);
  SEGGER_RTT_printf(0, "printf Test: %%-08x,   0x1234ABC : %-08x.\r\n", 0x1234ABC);
  SEGGER_RTT_printf(0, "printf Test: %%-08.6x, 0x1234ABC : %-08.6x.\r\n", 0x1234ABC);
  SEGGER_RTT_printf(0, "printf Test: %%-0x,    0x1234ABC : %-0x.\r\n", 0x1234ABC);

  SEGGER_RTT_printf(0, "printf Test: %%p,      &_Cnt      : %p.\r\n", &_Cnt);

  SEGGER_RTT_WriteString(0, "###### SEGGER_printf() Tests done. ######\r\n");
//  do
//  {
    _Cnt++;
//  }
//  while (1);
}
void SeggerTest2(void)
{
    SEGGER_RTT_WriteString(0, "SEGGER Real-Time-Terminal Sample\r\n");
    SEGGER_RTT_ConfigUpBuffer(0, NULL, NULL, 0, SEGGER_RTT_MODE_NO_BLOCK_SKIP);
    do {
      r = SEGGER_RTT_WaitKey();
      SEGGER_RTT_Write(0, &r, 1);
      r++;
    } while (1);
}


void testLed1()
{
    osThreadDef(simpleLedTest1_RTOS, simpleLedTest1_RTOS, osPriorityNormal, 0, configMINIMAL_STACK_SIZE );
    if ((osThreadCreate(osThread(simpleLedTest1_RTOS), nullptr)) == nullptr)
    {
        term("Failed to create simpleLedTest1_RTOS");
    }
}
void testLed2()
{
    osThreadDef(simpleLedTest2_RTOS, simpleLedTest2_RTOS, osPriorityRealtime, 0, configMINIMAL_STACK_SIZE *5 );
    if ((osThreadCreate(osThread(simpleLedTest2_RTOS), nullptr)) == nullptr)
    {
        term("Failed to create simpleLedTest2_RTOS");
    }
}
void testLed3()
{
//    osDelay (8000);
//    osSemaphoreRelease(Netif_LinkSemaphore);

    osThreadDef(simpleLedTest3_RTOS, simpleLedTest3_RTOS, osPriorityNormal, 0, configMINIMAL_STACK_SIZE );
    if ((osThreadCreate(osThread(simpleLedTest3_RTOS), nullptr)) == nullptr)
    {
        term("Failed to create simpleLedTest3_RTOS");
    }
}
void testUART()
{
    osThreadDef(simpletestUART_RTOS, simpletestUART_RTOS, osPriorityNormal, 0, configMINIMAL_STACK_SIZE );
    if ((osThreadCreate(osThread(simpletestUART_RTOS), nullptr)) == nullptr)
    {
        term("Failed to create simpletestUART_RTOS");
    }
}

//char *logTasks = new char[2048];
static char logTasks[2048];
static char logTasksTime[2048];
void testTasksLog()
{
    osThreadDef(TasksLog, TasksLog, osPriorityNormal, 0, configMINIMAL_STACK_SIZE );
    if ((osThreadCreate(osThread(TasksLog), nullptr)) == nullptr)
    {
        term("Failed to create TasksLog");
    }
}

void simpleLedTest1_RTOS(void const *argument)
{
    osDelay(5000);
    (void)argument;
    bool reset = true;
    uint32_t tickstart = HAL_GetTick();
    uint32_t timeSet = 10;
    uint32_t timeReset = 3000;

term("startingSimpleLedTest1_RTOS")

    for(;;)
    {
    if (boardType == sc2)
    {
        if(reset)
        {
            if (HAL_GetTick() > tickstart + timeReset)
            {
                 HAL_GPIO_WritePin(GPIOG, GPIO_PIN_6|GPIO_PIN_7|GPIO_PIN_8, GPIO_PIN_SET);
                 reset = false;
                 tickstart = HAL_GetTick();
            }
        }

        if(!reset)
        {
            if (HAL_GetTick() > tickstart + timeSet)
            {
                 HAL_GPIO_WritePin(GPIOG, GPIO_PIN_6|GPIO_PIN_7|GPIO_PIN_8, GPIO_PIN_RESET);
                 reset = true;
                 tickstart = HAL_GetTick();
            }
        }
    }
        for (uint8_t i = 0; i < TLC59116F_max_address * 8; ++i)
        {
            GPIO::getInstance()->aLeds[i].ledState = true;
            osDelay(1500);
            GPIO::getInstance()->aLeds[i].ledState = false;
            osDelay(5);
        }

    osDelay(10);
    } //end for(;;)

    vTaskDelete(nullptr);
}
void simpleLedTest2_RTOS(void const *argument)
{
    (void)argument;
    bool reset = true;
    uint32_t tickstart = HAL_GetTick();
    uint32_t timeSet = 10;
    uint32_t timeReset = 2000;

osDelay(50);
term("startingSimpleLedTest2_RTOS")

    for(;;)
    {
    if (boardType == sc2)
    {
            if(reset)
            {
                if (HAL_GetTick() > tickstart + timeReset)
                {
                     HAL_GPIO_WritePin(GPIOC, GPIO_PIN_10|GPIO_PIN_11|GPIO_PIN_12, GPIO_PIN_SET);
                     reset = false;
                     tickstart = HAL_GetTick();
                }
            }
            if(!reset)
            {
                if (HAL_GetTick() > tickstart + timeSet)
                {
                     HAL_GPIO_WritePin(GPIOC, GPIO_PIN_10|GPIO_PIN_11|GPIO_PIN_12, GPIO_PIN_RESET);
                     reset = true;
                     tickstart = HAL_GetTick();
                }
            }
    }

//    SEGGER_RTT_Write(0, "01234567890123456789012345678901234567890123456789012345678901234567890123456789\r\n", 82);
        SEGGER_RTT_WriteString(0, "SEGGER Real-Time-Terminal Sample\r\n");
//        SEGGER_RTT_ConfigUpBuffer(0, NULL, NULL, 0, SEGGER_RTT_MODE_NO_BLOCK_SKIP);
//        do {
//          r = SEGGER_RTT_WaitKey();
//          SEGGER_RTT_Write(0, &r, 1);
//          r++;
//        } while (1);
    osDelay(10);
    } //end for(;;)

    vTaskDelete(nullptr);
}
void simpleLedTest3_RTOS(void const *argument)
{
    (void)argument;
    bool reset = true;
    uint32_t tickstart = HAL_GetTick();
    uint32_t timeSet = 10;
    uint32_t timeReset = 2000;

osDelay(100);
term("--- simpleLedTest3_RTOS ---")

    for(;;)
    {
//        osDelay (8000);
//        osSemaphoreRelease(Netif_LinkSemaphore);
        if(reset)
        {
            if (HAL_GetTick() > tickstart + timeReset)
            {
                 HAL_GPIO_WritePin(TEST_LED_GPIO_Port, TEST_LED_Pin, GPIO_PIN_SET);

                 reset = false;
                 tickstart = HAL_GetTick();
            }
        }
        if(!reset)
        {
            if (HAL_GetTick() > tickstart + timeSet)
            {
                 HAL_GPIO_WritePin(TEST_LED_GPIO_Port, TEST_LED_Pin, GPIO_PIN_RESET);

                 reset = true;
                 tickstart = HAL_GetTick();

//                 HAL_SAI_Transmit_DMA(&audioTxSai, zvon3_raw, zvon3_raw_len/2);
            }
        }
    osDelay(1);
    } //end for(;;)

    vTaskDelete(nullptr);
}

void simpletestUART_RTOS(void const *argument)
{
    (void)argument;
    bool reset = true;
    uint32_t tickstart = HAL_GetTick();
    uint32_t timeSet = 3000;
    uint32_t timeReset = 3000;

osDelay(150);
term("startingSimpleTestUART_RTOS")

    for(;;)
    {

        if(reset)
        {
            if (HAL_GetTick() > tickstart + timeReset)
            {
//                 term("*************  SimpleTestUART_RTOS  *************")
//GPIO::getInstance()->aLeds[3].ledState = 1;
                 reset = false;
                 tickstart = HAL_GetTick();
            }
        }
        if(!reset)
        {
            if (HAL_GetTick() > tickstart + timeSet)
            {
//                term("#############  SimpleTestUART_RTOS  #############")
                reset = true;
                tickstart = HAL_GetTick();
            }
        }
    osDelay(1);
    } //end for(;;)

    vTaskDelete(nullptr);
}

void TasksLog(void const *argument)
{
    (void)argument;

    osDelay(6000);
    term("--- TasksLog ---")

        for(;;)
    {
        vTaskList(logTasks);
//        vTaskGetRunTimeStats(logTasksTime);
        term2(logTasks)
        term1("heap size") term(xPortGetFreeHeapSize())
        osDelay(180000);
    } //end for(;;)

    vTaskDelete(nullptr);
}

#ifdef __cplusplus
}
#endif

//void test_UART()
//{
//    RS232::getInstance().test();
//}
