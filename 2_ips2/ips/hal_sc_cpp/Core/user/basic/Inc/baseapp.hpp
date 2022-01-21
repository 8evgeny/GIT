/*
 * baseapp.hpp
 *
 *  Created on: 5 авг. 2020 г.
 *      Author: Dmitry Dmitry Donskih
 */

#ifndef USER_BASIC_INC_BASEAPP_HPP_
#define USER_BASIC_INC_BASEAPP_HPP_

#include "FreeRTOS.h"
#include "task.h"
#include <taskManagement.hpp>

typedef void (*AppProgramm_t)( void * );
typedef enum{
	app_stateStoped		=0,
	app_stateRunning	=1,
	app_statePaused		=2
} app_state_t;

class abstractApp {
public:
	abstractApp(const char* name);
	virtual ~abstractApp();
	void setPriority(char priority);
	void setStackSize(unsigned short stack);
	virtual int Run()=0;
	virtual void startApp()=0;
	void Pause();
	void Resume();
	void Stop();
protected:
	xTaskHandle handle=nullptr;
	//
	unsigned short s_stack=512;
	char prio=0;
	const char *app_name;
	app_state_t state=app_stateStoped;
	//
};


class baseApp :public abstractApp{

public:
	baseApp(const char* name);
	virtual void setProgram(AppProgramm_t programm_code,void* arguments);
	virtual int Run() override;
protected:
	virtual void startApp();
	AppProgramm_t programm=nullptr;
	void* ctl_block=nullptr;
};




#endif /* USER_BASIC_INC_BASEAPP_HPP_ */
