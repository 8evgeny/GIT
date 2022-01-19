/*
 * monitor.cpp
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


#ifndef USER_UTILS_MONITOR_CPP_
#define USER_UTILS_MONITOR_CPP_

#include "monitor.h"
#include "cmsis_os2.h"
//#include "ipsUartDev.h"
#include "string.h"
#include <ips_globals.hpp>
#include "ipssystem.hpp"


#define MON_WIN_SIZE 512

extern ipsUART uart3;

monitorTask::monitorTask(ttY* term,uint16_t refresh_time)
:ipcThread("System Monitor",1024,osPriorityBelowNormal4){
	r_time=refresh_time;
	stout=term;
	//ips_mon_buffer=(char*)pvPortMalloc(sizeof(char[MON_WIN_SIZE]));
};
void monitorTask::changeRefreshTime(uint16_t refresh_time_ms){
	r_time=refresh_time_ms;
};

void monitorTask::setTerm() {
	stout=ipsSystem::monterm;
}

void monitorTask::taskRunFcn()
{
	osDelay(1);
	static char ips_mon_buffer[80*24];
	size_t pos=0;
	memset(ips_mon_buffer,0,MON_WIN_SIZE);


	///char *buffer=(char*)pvPortMalloc(sizeof(char[128]));
	for (;;){
		if(stout){

			vTaskList(ips_mon_buffer);
			pos=strlen(ips_mon_buffer);
			*(ips_mon_buffer+pos)='\n';
			vTaskGetRunTimeStats((ips_mon_buffer+pos+1));

			stout->cls();
			pos=strlen(ips_mon_buffer);
			*(ips_mon_buffer+pos)='\n';
			stout->write_t(ips_mon_buffer, pos+1);
			int free_heap=xPortGetFreeHeapSize();
			stout->printfl("Heap avaible:	%i \r\n",free_heap);
			stout->printfl("Free system buffers: %i \r\n",system_cpp.getFreeBuffers());


		}
		osDelay(r_time);
		pos=0;
		memset(ips_mon_buffer,0,MON_WIN_SIZE);

	}
}
;

void monitorTask::setTerm(ttY *term)
{
	stout=term;
}

#endif /* USER_UTILS_MONITOR_CPP_ */
