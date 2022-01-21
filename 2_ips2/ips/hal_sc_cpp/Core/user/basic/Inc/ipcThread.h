/*
 * ipcThread.h
 *
 *  Created on: 24 мая 2020 г.
 *      Author: Donskikh Dmitry
 *      based on <git link>
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
#ifndef IPSLIBS_IPCBASE_INC_IPCTHREAD_H_
#define IPSLIBS_IPCBASE_INC_IPCTHREAD_H_

#include "FreeRTOS.h"
#include "task.h"
#include "cmsis_os2.h"

//#define  xTaskHandle TaskHandle_t

class ipcThread {
private:
    TaskHandle_t taskHandle;
    uint8_t init_method=0;
protected:
    xTaskHandle getTaskHandle();
    //void lazyInit(const char *name, unsigned short stackDepth, char priority);
    void* child=nullptr;

public:
    ipcThread(const char *name, unsigned short stackDepth, char priority, uint8_t lazy=0);
    virtual ~ipcThread();
    void setPriority(char priority);
    virtual void taskRunFcn() =0;

    static void pvTaskCode(void *pvParameters);
};



#endif /* IPSLIBS_IPCBASE_INC_IPCTHREAD_H_ */
