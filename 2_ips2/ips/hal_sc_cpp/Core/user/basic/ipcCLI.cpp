
/*
 * ipc_cli.cpp
 *
 *  Created on: 12 авг. 2020 г.
 *      Author: Dmitry Dmitry Donskih
 */

#include <ipcCLI.hpp>
#include "FreeRTOS.h"
#include <cstring>
#include <new>

/*
#define NEW(pointer,type) \
	pointer=(type*)pvPortMalloc(sizeof(type));\
	pointer=(type*)new (pointer)type

#define DELETE(pointer,type) \
	((type*)pointer)->~type();\
	vPortFree((void*)pointer)
*/

ipcCLI::ipcCLI() {
	registred_commands=(cli_app_pair_t*)pvPortMalloc(registred_commands_max*sizeof(cli_app_pair_t));

}

ipcCLI::~ipcCLI() {
	vPortFree((void*)registred_commands);
}

void ipcCLI::registerCommand(const char* cmd,AppProgramm_t function) {
	if ((cmd!=NULL)&&(function!=NULL)){
		if (registred_commands_count>=registred_commands_max){
			registred_commands_max=reallocAppList();
		}
		registred_commands[registred_commands_count].command=cmd;
		registred_commands[registred_commands_count].programm=function;
		registred_commands_count++;
	}
}

void ipcCLI::parseCLI(char* cmdline,uint32_t size,ttY* term) {
	for (int i=0;i<registred_commands_count;i++){
		if(strstr(cmdline,registred_commands[i].command)!=NULL){
			appCLI *cmd_app=(appCLI*)pvPortMalloc(sizeof(appCLI));
			cmd_app=new(cmd_app)appCLI(registred_commands[i].command,registred_commands[i].programm,cmdline,size,term,this);
			cmd_app->Run();
			return;
		}
	}
	term->printfl("Wrong command\n");
}

void ipcCLI::unallocAppResourses(baseApp *application) {
	((appCLI*)application)->~appCLI();
	//vPortFree((void*)application);
}

int ipcCLI::reallocAppList() {
	cli_app_pair_t *temp=(cli_app_pair_t*)pvPortMalloc((registred_commands_max+IPC_CLI_CMDLIST_SIZE)*sizeof(cli_app_pair_t));
	memcpy(temp,registred_commands,registred_commands_max*sizeof(cli_app_pair_t));
	vPortFree((void*)registred_commands);
	registred_commands=temp;
	return registred_commands_max+IPC_CLI_CMDLIST_SIZE;
}




appCLI::appCLI(const char *name, AppProgramm_t function, char *arguments,
		uint32_t argc, ttY *term, ipcCLI *parrent):baseApp(name)
{
	argv=(char*)pvPortMalloc(argc);
	memcpy(argv,arguments,argc);
	programm=function;
	parameters.argv=argv;
	parameters.size=argc;
	parameters.terminal=term;
	parrent_i=parrent;
}
appCLI::~appCLI() {
	vPortFree(argv);
}

void appCLI::startApp()
{
	programm(&parameters);
	parrent_i->unallocAppResourses(this);
	while(1){
		taskYIELD();
	}
}
