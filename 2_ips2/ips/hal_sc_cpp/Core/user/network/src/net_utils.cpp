/*
 * net_utils.cpp
 *
 *  Created on: 2 сент. 2020 г.
 *      Author: Dmitry Donskih
 */
#include "main.h"
#include "cmsis_os2.h"
#include "net_utils.h"

#define IP_READY_TIMEOUT 60000

osMutexId_t	lwipReadyMtx=NULL;

osSemaphoreId_t *lwipReadySem=NULL;

extern uint8_t is_lwip_ready;

void isStackReady(void* arg){
	if(lwipReadyMtx==NULL)
	{
		lwipReadyMtx=osMutexNew(NULL);
		if(osMutexAcquire(lwipReadyMtx, IP_READY_TIMEOUT)<0)
		{
			Error_Handler();
		}
	}
	osMutexRelease(lwipReadyMtx);
	is_lwip_ready=0xFF;

}

