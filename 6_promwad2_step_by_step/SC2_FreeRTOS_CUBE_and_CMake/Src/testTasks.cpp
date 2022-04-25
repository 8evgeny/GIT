#include "testTasks.h"
#include <stdbool.h>
#include <stdint.h>
#include "cmsis_os.h"
#include "debug.h"
#include "rs232_printf.h"

#ifdef __cplusplus
extern "C" {
#endif

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
void testUART()
{
    osThreadDef(simpletestUART_RTOS, simpletestUART_RTOS, osPriorityNormal, 0, configMINIMAL_STACK_SIZE );
    osThreadCreate(osThread(simpletestUART_RTOS), NULL);
}




void simpleLedTest1_RTOS(void const *argument)
{
    (void)argument;
    bool reset = true;
    uint32_t tickstart = HAL_GetTick();
    uint32_t timeSet = 5;
    uint32_t timeReset = 700;
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
    HAL_Delay(1);
    } //end for(;;)
}
void simpleLedTest2_RTOS(void const *argument)
{
    (void)argument;
    bool reset = true;
    uint32_t tickstart = HAL_GetTick();
    uint32_t timeSet = 5;
    uint32_t timeReset = 500;
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
    HAL_Delay(1);
    } //end for(;;)
}
void simpleLedTest3_RTOS(void const *argument)
{
    (void)argument;
    bool reset = true;
    uint32_t tickstart = HAL_GetTick();
    uint32_t timeSet = 1;
    uint32_t timeReset = 1000;
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
    HAL_Delay(1);
    } //end for(;;)
}
void simpletestUART_RTOS(void const *argument)
{
    (void)argument;
    bool reset = true;
    uint32_t tickstart = HAL_GetTick();
    uint32_t timeSet = 1000;
    uint32_t timeReset = 1000;
    for(;;)
    {

        if(reset)
        {
            if (HAL_GetTick() > tickstart + timeReset)
            {
                  uint8_t buf[21] = {"12345678901234567890"};
                  RS232::getInstance().write(buf,10);
                 reset = false;
                 tickstart = HAL_GetTick();
            }
        }
        if(!reset)
        {
            if (HAL_GetTick() > tickstart + timeSet)
            {
                uint8_t buf[21] = {"asdfghjklqwertyuiopz"};
                RS232::getInstance().write(buf,10);
                 reset = true;
                 tickstart = HAL_GetTick();
            }
        }
    HAL_Delay(1);
    } //end for(;;)
}


#ifdef __cplusplus
}
#endif

//void test_UART()
//{
//    RS232::getInstance().test();
//}
