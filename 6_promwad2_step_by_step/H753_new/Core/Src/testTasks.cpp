#include "testTasks.h"
#include <stdbool.h>
#include <stdint.h>
#include "cmsis_os.h"
#include "../Debug/debug.h"
#include "rs232_printf.h"

#ifdef __cplusplus
extern "C" {
#endif

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




void simpleLedTest1_RTOS(void const *argument)
{
    (void)argument;
    bool reset = true;
    uint32_t tickstart = HAL_GetTick();
    uint32_t timeSet = 10;
    uint32_t timeReset = 3000;

term("startingSimpleLedTest1_RTOS")

    for(;;)
    {
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
    HAL_Delay(10);
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
    HAL_Delay(10);
    } //end for(;;)

    vTaskDelete(nullptr);
}
void simpleLedTest3_RTOS(void const *argument)
{
    (void)argument;
    bool reset = true;
    uint32_t tickstart = HAL_GetTick();
    uint32_t timeSet = 10;
    uint32_t timeReset = 3000;

osDelay(100);
term("startingSimpleLedTest3_RTOS")

    for(;;)
    {
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
                 term("*************  SimpleTestUART_RTOS  *************")
                 reset = false;
                 tickstart = HAL_GetTick();
            }
        }
        if(!reset)
        {
            if (HAL_GetTick() > tickstart + timeSet)
            {
                term("#############  SimpleTestUART_RTOS  #############")
                reset = true;
                tickstart = HAL_GetTick();
            }
        }
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
