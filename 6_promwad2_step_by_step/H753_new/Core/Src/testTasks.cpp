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

extern unsigned char zvon3_raw[];
extern unsigned int zvon3_raw_len;
extern uint8_t TLC59116F_max_address;
#ifdef __cplusplus
extern "C" {
#endif

extern SAI_HandleTypeDef audioTxSai;

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
    osThreadDef(simpleLedTest2_RTOS, simpleLedTest2_RTOS, osPriorityNormal, 0, configMINIMAL_STACK_SIZE );
    if ((osThreadCreate(osThread(simpleLedTest2_RTOS), nullptr)) == nullptr)
    {
        term("Failed to create simpleLedTest2_RTOS");
    }
}
void testLed3()
{
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

char *logTasks = new char[2048];
void testTasksLog()
{
    osThreadDef(TasksLog, TasksLog, osPriorityNormal, 0, configMINIMAL_STACK_SIZE );
    if ((osThreadCreate(osThread(TasksLog), nullptr)) == nullptr)
    {
        term("Failed to create TasksLog");
    }
}
void testSendMcast()
{
    osThreadDef(testSendMcast, SendMcast, osPriorityNormal, 0, configMINIMAL_STACK_SIZE );
    if ((osThreadCreate(osThread(testSendMcast), nullptr)) == nullptr)
    {
        term("Failed to create testSendMcast");
    }
}
void testReceiveMcast()
{
    osThreadDef(testReceiveMcast, ReceiveMcast, osPriorityNormal, 0, configMINIMAL_STACK_SIZE );
    if ((osThreadCreate(osThread(testReceiveMcast), nullptr)) == nullptr)
    {
        term("Failed to create testSendMcast");
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
#ifndef SC4
        if(reset)
        {
            if (HAL_GetTick() > tickstart + timeReset)
            {
                 HAL_GPIO_WritePin(GPIOG, L4_Pin|L5_Pin|L6_Pin, GPIO_PIN_SET);
                 reset = false;
                 tickstart = HAL_GetTick();
            }
        }

        if(!reset)
        {
            if (HAL_GetTick() > tickstart + timeSet)
            {
                 HAL_GPIO_WritePin(GPIOG, L4_Pin|L5_Pin|L6_Pin, GPIO_PIN_RESET);
                 reset = true;
                 tickstart = HAL_GetTick();
            }
        }
#endif
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
#ifndef SC4
        if(reset)
        {
            if (HAL_GetTick() > tickstart + timeReset)
            {
                 HAL_GPIO_WritePin(GPIOC, L1_Pin|L2_Pin|L3_Pin, GPIO_PIN_SET);
                 reset = false;
                 tickstart = HAL_GetTick();
            }
        }
        if(!reset)
        {
            if (HAL_GetTick() > tickstart + timeSet)
            {
                 HAL_GPIO_WritePin(GPIOC, L1_Pin|L2_Pin|L3_Pin, GPIO_PIN_RESET);
                 reset = true;
                 tickstart = HAL_GetTick();
            }
        }
#endif
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
        if(reset)
        {
            if (HAL_GetTick() > tickstart + timeReset)
            {
                 HAL_GPIO_WritePin(TEST_LED_GPIO_Port, TEST_LED_Pin, GPIO_PIN_SET);
//Временный тест SC4
//HAL_GPIO_WritePin(GPIOD, GPIO_PIN_6, GPIO_PIN_SET);

                 reset = false;
                 tickstart = HAL_GetTick();
            }
        }
        if(!reset)
        {
            if (HAL_GetTick() > tickstart + timeSet)
            {
                 HAL_GPIO_WritePin(TEST_LED_GPIO_Port, TEST_LED_Pin, GPIO_PIN_RESET);
//Временный тест SC4
//HAL_GPIO_WritePin(GPIOD, GPIO_PIN_6, GPIO_PIN_RESET);

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
        term(logTasks)
        term1("heap size") term(xPortGetFreeHeapSize())
        osDelay(30000);
    } //end for(;;)

    vTaskDelete(nullptr);
}

void SendMcast(void const *argument)
{
    (void)argument;
    osDelay(11000);
    term("--- SendMcast ---")

        for(;;)
    {



        osDelay(1);
    } //end for(;;)

    vTaskDelete(nullptr);
}

void ReceiveMcast(void const *argument)
{
    (void)argument;
    osDelay(12000);
    term("--- ReceiveMcast ---")

        for(;;)
    {



        osDelay(1);
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
