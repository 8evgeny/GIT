/* Includes ------------------------------------------------------------------*/
#include "MDR32Fx.h"
#include "MDR32F9Qx_config.h"
#include "MDR32F9Qx_can.h"
#include "MDR32F9Qx_rst_clk.h"
#include "MDR32F9Qx_port.h"
#include "MDR32F9Qx_it.h"
#include "init.h" /**<  */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"

/* Private functions ---------------------------------------------------------*/
void vLed1(void *argument)
{
    while(1)
    {
        PORT_SetBits(MDR_PORTB, PORT_Pin_0);
        vTaskDelay(5000);
        PORT_ResetBits(MDR_PORTB, PORT_Pin_0);
		vTaskDelay(5000);
    }
}


void vLed2(void *argument)
{
    while(1)
    {
        PORT_SetBits(MDR_PORTB, PORT_Pin_1);
        vTaskDelay(500);
        PORT_ResetBits(MDR_PORTB, PORT_Pin_1);
		vTaskDelay(500);
    }
}


void vLed3(void *argument)
{
    while(1)
    {
        PORT_SetBits(MDR_PORTB, PORT_Pin_2);
        vTaskDelay(1000);
        PORT_ResetBits(MDR_PORTB, PORT_Pin_2);
		vTaskDelay(1000);
    }
}


void vLed4(void *argument)
{
    while(1)
    {
        PORT_SetBits(MDR_PORTB, PORT_Pin_3);
        vTaskDelay(2000);
        PORT_ResetBits(MDR_PORTB, PORT_Pin_3);
		vTaskDelay(2000);
    }
}


void vApplicationIdleHook (void)
{
}


int main(void)
{
	init ();
    xTaskCreate(vLed1, "1", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 1, NULL);
    xTaskCreate(vLed2, "2", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 1, NULL);
    xTaskCreate(vLed3, "3", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 1, NULL);
    xTaskCreate(vLed4, "4", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 1, NULL);
    
    // Запускаем диспетчер и понеслась. 	
    vTaskStartScheduler();
}
























#if (USE_ASSERT_INFO == 1)
void assert_failed(uint32_t file_id, uint32_t line)
{
  while (1) {}
}
#elif (USE_ASSERT_INFO == 2)
void assert_failed(uint32_t file_id, uint32_t line, const uint8_t* expr);
{
  while (1) {}
}
#endif

/* END OF FILE main.c */
