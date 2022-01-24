/*
 * baseapp.cpp
 *
 *  Created on: 5 авг. 2020 г.
 *      Author: Dmitry Dmitry Donskih
 */

#include <baseapp.hpp>
#include "cmsis_os2.h"


extern void pvAppCode(void *pvParameters) {
    (static_cast<abstractApp*>(pvParameters))->startApp();
}


abstractApp::abstractApp(const char *name):app_name{name} {
	prio=osPriorityLow;
}

void abstractApp::setPriority(char priority) {
	prio=priority;
}

void abstractApp::setStackSize(unsigned short stack) {
	s_stack=stack;
}

void abstractApp::Pause() {
	if(state==app_stateRunning){
		vTaskSuspend(handle);
		state=app_statePaused;
	}
}

void abstractApp::Resume() {
	if(state==app_statePaused){
		vTaskResume(handle);
		state=app_stateRunning;
	}
}


void abstractApp::Stop() {
	if(handle!=NULL){
		vTaskDelete(handle);
		state=app_stateStoped;
	}
}

abstractApp::~abstractApp() {
	(sysTaskManagement::getInstance()).eliminate((void*)this,handle);
}


//////////////////////////////////////////////////////////////////////////////
///
//////////////////////////////////////////////////////////////////////////////

baseApp::baseApp(const char *name):abstractApp(name)
{
}


void baseApp::setProgram(AppProgramm_t programm_code,void* arguments) {
	programm=programm_code;
	ctl_block=arguments;
}

int baseApp::Run() {
	if(programm){
		if(state==app_stateStoped){
			xTaskCreate(&pvAppCode, app_name, s_stack, (void*)this, prio, &handle);
			state=app_stateRunning;
		}
	}
	return 0;
}

void baseApp::startApp() {
	programm(ctl_block);
	this->~abstractApp();
}
//////////////////////////////////////////////////////////////////////////////
///
//////////////////////////////////////////////////////////////////////////////


