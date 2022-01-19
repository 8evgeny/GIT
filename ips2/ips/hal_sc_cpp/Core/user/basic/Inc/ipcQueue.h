/* OS Queue wrapper for RTOS
 *
 *
 *
 *
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


#pragma once




#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"


class ipcQueue {
private:
    xQueueHandle messageQueue;
public:
    ipcQueue(uint8_t messageQueueSize, size_t itemSize) {
        messageQueue = xQueueCreate(messageQueueSize, itemSize);
    }
    ipcQueue() {
        messageQueue = 0;
    }
    ~ipcQueue() {
    	vQueueDelete(messageQueue);
    }
    inline bool send(void *ptr){
        return xQueueSend(messageQueue, ptr, 0);
    }
    inline bool sendFromIsr(void *ptr){
        //isHigherProirityTaskWoken is ignored in this method
        return xQueueSendFromISR(messageQueue, ptr, 0);
    }
    inline bool receive(void *ptr){
        return xQueueReceive(messageQueue, ptr, portMAX_DELAY);
    }
    inline bool receiveFromISR(void *ptr){
        //isHigherProirityTaskWoken is ignored in this method
        return xQueueReceiveFromISR(messageQueue, ptr, 0);
    }
};
