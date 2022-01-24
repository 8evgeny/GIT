/*
 * system.c
 *
 *  Created on: Jun 17, 2020
 *      Author: Donskikh Dmitry
 */


#include <system_c.h>
#include "main.h"
#include "FreeRTOS.h"
#include "task.h"
#include "cmsis_os2.h"
#include "stm32h7xx_hal.h"


/*
static volatile uint8_t s_dma_buffers[DMA_BUFFER_SIZE];
static uint8_t *s_dma_buffer_pos=&s_dma_buffers[0];

static uint32_t getFreeDMAspace()
{
	return (DMA_BUFFER_SIZE-(s_dma_buffer_pos-&s_dma_buffers[0]));
}


void* allocateDMAbufferStatic(uint32_t size)
{
	if(getFreeDMAspace()>size){
		uint8_t* l_buffer=s_dma_buffer_pos;
		s_dma_buffer_pos+=(size+size%4);
		return memset(l_buffer,0,size);
	}else{
		return NULL;
	}
}

void* getDMAbuffers(){
	return (void*)s_dma_buffer_pos;
}
*/

typedef struct{
    GPIO_TypeDef* port;
    uint16_t pin;
}gpioPair_t;

///////////////////////////////// Normal operation LED functions
static void blinkerAlarmTask(void* pvParameters);



static TaskHandle_t alarm_task=NULL;

void sysLedNormON(void)
{
	HAL_GPIO_WritePin(LED_NORM_GPIO_Port, LED_NORM_Pin, GPIO_PIN_SET);
}

void sysLedNormOFF(void)
{
	HAL_GPIO_WritePin(LED_NORM_GPIO_Port, LED_NORM_Pin, GPIO_PIN_RESET);
}

void sysLedNormToggle(void)
{
	HAL_GPIO_TogglePin(LED_NORM_GPIO_Port, LED_NORM_Pin);
}

void sysAlarmStart(void)
{
	if(!alarm_task){
		xTaskCreate(blinkerAlarmTask, "Alarm LED!", 128, NULL, osPriorityAboveNormal, &alarm_task);
	}
}

void sysAlarmStop(void) {
	if(alarm_task){
		vTaskDelete(alarm_task);
		alarm_task=NULL;
	}
}

void blinkerAlarmTask(void *pvParameters)
{
	for(;;){
		sysLedNormToggle();
		osDelay(100);
	}

}
/////////////////////////////////////////////////////////////////
/////////////////////////////////////Test LED operation functions


void sysLedTestON(void)
{
    HAL_GPIO_WritePin(TEST_LED_GPIO_Port,TEST_LED_Pin,GPIO_PIN_SET);
}

void sysLedTestOFF(void)
{
	HAL_GPIO_WritePin(TEST_LED_GPIO_Port,TEST_LED_Pin,GPIO_PIN_RESET);
}

void sysLedTestToggle(void)
{
	HAL_GPIO_TogglePin(TEST_LED_GPIO_Port, TEST_LED_Pin);
}

#ifdef SC4BOPLA
void sysLedMicON(void)
    {
    HAL_GPIO_WritePin(LED_MIC_GPIO_Port, LED_MIC_Pin, GPIO_PIN_SET);
    }

void sysLedMicOFF(void)
    {
    HAL_GPIO_WritePin(LED_MIC_GPIO_Port, LED_MIC_Pin, GPIO_PIN_RESET);
    }

void sysLedMicToggle(void)
    {
    HAL_GPIO_TogglePin(LED_MIC_GPIO_Port,LED_MIC_Pin);
    }

void sysAmpPwrOn(void)
    {
    HAL_GPIO_WritePin(AMP_ON_GPIO_Port, AMP_ON_Pin, GPIO_PIN_SET);
    }

void sysAmpPwrOff(void)
    {
    HAL_GPIO_WritePin(AMP_ON_GPIO_Port, AMP_ON_Pin, GPIO_PIN_RESET);
    }
#endif

#ifdef SC2DIOBOARD
static gpioPair_t _getDiscreteOutput(uint16_t output_n);
static gpioPair_t _getDiscreteInput(uint16_t input_n);

static inline gpioPair_t _getDiscreteOutput(uint16_t output_n){
    switch (output_n) {
	case 1: return (gpioPair_t){DO1_GPIO_Port,DO1_Pin};
	case 2: return (gpioPair_t){DO2_GPIO_Port,DO2_Pin};
	case 3: return (gpioPair_t){DO3_GPIO_Port,DO3_Pin};
	case 4: return (gpioPair_t){DO4_GPIO_Port,DO4_Pin};
	case 5: return (gpioPair_t){DO5_GPIO_Port,DO5_Pin};
	case 6: return (gpioPair_t){DO6_GPIO_Port,DO6_Pin};
	case 7: return (gpioPair_t){DO7_GPIO_Port,DO7_Pin};
	case 8: return (gpioPair_t){DO8_GPIO_Port,DO8_Pin};

	default: return (gpioPair_t){NULL,0xFF};
    }
}

static inline gpioPair_t _getDiscreteInput(uint16_t input_n){
    switch (input_n) {
	case 1: return (gpioPair_t){DI1_GPIO_Port,DI1_Pin};
	case 2: return (gpioPair_t){DI2_GPIO_Port,DI2_Pin};
	case 3: return (gpioPair_t){DI3_GPIO_Port,DI3_Pin};
	case 4: return (gpioPair_t){DI4_GPIO_Port,DI4_Pin};
	case 5: return (gpioPair_t){DI5_GPIO_Port,DI5_Pin};
	case 6: return (gpioPair_t){DI6_GPIO_Port,DI6_Pin};
	case 7: return (gpioPair_t){DI7_GPIO_Port,DI7_Pin};
	case 8: return (gpioPair_t){DI8_GPIO_Port,DI8_Pin};

	default: return (gpioPair_t){NULL,0xFF};
    }
}

void sysDiscreteOutputON(uint16_t output){
    gpioPair_t output_pair=_getDiscreteOutput(output);
    if(output_pair.port)HAL_GPIO_WritePin(output_pair.port,output_pair.pin,GPIO_PIN_RESET);

}
void sysDiscreteOutputOFF(uint16_t output){
    gpioPair_t output_pair=_getDiscreteOutput(output);
    if(output_pair.port)HAL_GPIO_WritePin(output_pair.port,output_pair.pin,GPIO_PIN_SET);
}
void sysDiscreteOutputToggle(uint16_t output){
    gpioPair_t output_pair=_getDiscreteOutput(output);
    if(output_pair.port)HAL_GPIO_TogglePin(output_pair.port, output_pair.pin);
}

uint8_t sysDiscreteOutputRead(uint16_t output){
    gpioPair_t output_pair=_getDiscreteOutput(output);
    if(output_pair.port){
	return HAL_GPIO_ReadPin(output_pair.port, output_pair.pin);
    }
    return 0U;
}

void sysDiscreteOutputAllON(void){
    for (uint8_t i=1;i<9;i++){
	sysDiscreteOutputON(i);
        }
}
void sysDiscreteOutputAllOFF(void){
    for (uint8_t i=1;i<9;i++){
	sysDiscreteOutputOFF(i);
        }
}

void sysCheckDiscreteOutputs(void){
    sysDiscreteOutputAllOFF();
    HAL_Delay(50);
    for (uint8_t i=1;i<9;i++){
	sysDiscreteOutputON(i);
	HAL_Delay(100);
    }
    for (uint8_t i=1;i<9;i++){
    	sysDiscreteOutputOFF(i);
    	HAL_Delay(100);
    }
    for (uint8_t i=1;i<9;i++){
        sysDiscreteOutputOFF(i-1);
        sysDiscreteOutputON(i);
        HAL_Delay(100);
    }
    sysDiscreteOutputAllOFF();
    HAL_Delay(50);
}

#endif



