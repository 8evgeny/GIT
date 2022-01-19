/*
 * ipcThread.cpp
 *
 *  Created on: 24 мая 2020 г.
 *      Author: Donskikh Dmitry
 */
/*
MIT License

Copyright (c) 2020 Dmitry Donskikh

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/



#include "ipcThread.h"
#include "cmsis_os2.h"
#include <taskManagement.hpp>


//extern void pvTaskCode(void *pvParameters) {
//    (static_cast<ipcThread*>(pvParameters))->taskRunFcn();
//}

ipcThread::ipcThread(const char *name, unsigned short stackDepth=128, char priority=osPriorityNormal, uint8_t lazy) {
    if(!lazy)xTaskCreate(pvTaskCode, (const char *) name, stackDepth, (void*) this, priority, &taskHandle);
//    init_method=lazy;
}

ipcThread::~ipcThread() {
	(sysTaskManagement::getInstance()).eliminate(child,taskHandle);
}

void ipcThread::setPriority(char priority) {
	vTaskPrioritySet(taskHandle, priority);
    }

TaskHandle_t ipcThread::getTaskHandle()
    {
    return taskHandle;
    }

//void ipcThread::lazyInit(const char *name, unsigned short stackDepth=128, char priority=osPriorityNormal)
//    {
//	if(init_method)xTaskCreate(pvTaskCode, (const char *) name, stackDepth, (void*) this, priority, &taskHandle);
//	init_method=0;
//    }

void ipcThread::pvTaskCode(void *pvParameters){
    (static_cast<ipcThread*>(pvParameters))->taskRunFcn();
}
