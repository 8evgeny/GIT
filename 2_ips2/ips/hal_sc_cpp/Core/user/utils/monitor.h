/*
 * monitor.h
 *
 *  Created on: May 28, 2020
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


#ifndef USER_UTILS_MONITOR_H_
#define USER_UTILS_MONITOR_H_

#include "FreeRTOS.h"
#include "ipcThread.h"
//#include "ttY.h"
#include <devices/stream_device.h>

class monitorTask :public ipcThread
{
private:
	uint16_t r_time;
	ttY *stout=NULL;
	//static char ips_mon_buffer[512];
public:
	monitorTask(ttY* term,uint16_t refresh_time=100);
	void setTerm(ttY* term);
	void setTerm();
	void changeRefreshTime(uint16_t refresh_time_ms);
	void taskRunFcn();
};




#endif /* USER_UTILS_MONITOR_H_ */
