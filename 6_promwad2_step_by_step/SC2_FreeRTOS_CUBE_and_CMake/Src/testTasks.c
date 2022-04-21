#include "testTasks.h"
#include <stdint.h>
#include <stdbool.h>
#include "cmsis_os.h"

void testLed1()
{
    osThreadDef(simpleLedTest1_RTOS, simpleLedTest1_RTOS, osPriorityHigh, 0, configMINIMAL_STACK_SIZE );
    osThreadCreate(osThread(simpleLedTest1_RTOS), NULL);
}
void testLed2()
{
    osThreadDef(simpleLedTest2_RTOS, simpleLedTest2_RTOS, osPriorityHigh, 0, configMINIMAL_STACK_SIZE );
    osThreadCreate(osThread(simpleLedTest2_RTOS), NULL);
}
void testLed3()
{
    osThreadDef(simpleLedTest3_RTOS, simpleLedTest3_RTOS, osPriorityHigh, 0, configMINIMAL_STACK_SIZE );
    osThreadCreate(osThread(simpleLedTest3_RTOS), NULL);
}
void simpleLedTest1_RTOS()
{

    bool reset = true;
    uint32_t tickstart = HAL_GetTick();
    uint32_t timeSet = 10;
    uint32_t timeReset = 3000;
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
}
void simpleLedTest2_RTOS()
{
    bool reset = true;
    uint32_t tickstart = HAL_GetTick();
    uint32_t timeSet = 10;
    uint32_t timeReset = 2000;
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
}
void simpleLedTest3_RTOS(void const *argument)
{
    (void)argument;
    bool reset = true;
    uint32_t tickstart = HAL_GetTick();
    uint32_t timeSet = 5;
    uint32_t timeReset = 4000;
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
    HAL_Delay(10);
    } //end for(;;)
}
