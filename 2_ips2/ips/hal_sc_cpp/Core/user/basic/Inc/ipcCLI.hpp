/*
 * ipcCLI.hpp
 *
 *  Created on: 12 авг. 2020 г.
 *      Author: Dmitry Dmitry Donskih
 */

#ifndef USER_BASIC_INC_IPCCLI_HPP_
#define USER_BASIC_INC_IPCCLI_HPP_

#include <baseapp.hpp>
//#include <ttY.h>
#include <devices/stream_device.h>
#include <cinttypes>

#define IPC_CLI_CMDLIST_SIZE	5

typedef struct{
	ttY* terminal;
	char* argv;
	uint32_t size;
} app_param_t;

typedef void (*CliApp_t)( app_param_t * );

typedef struct{
	const char* command;
	AppProgramm_t programm;
}cli_app_pair_t;



class ipcCLI {
public:
	ipcCLI();
	~ipcCLI();
	void registerCommand(const char* cmd,AppProgramm_t function);
	void parseCLI(char* cmdline,uint32_t size,ttY* term);
	void unallocAppResourses(baseApp *application);
private:

	int reallocAppList();
	int registred_commands_count=0;
	int registred_commands_max=IPC_CLI_CMDLIST_SIZE;
	cli_app_pair_t *registred_commands=nullptr;

};

class appCLI :public baseApp
{
public:
	appCLI(const char* name,AppProgramm_t function,char* arguments,uint32_t argc,ttY *term,ipcCLI* parrent);
	~appCLI();

private:
	void startApp() override;

	ipcCLI* parrent_i=nullptr;
	app_param_t parameters;
	char* argv=nullptr;
};

#endif /* USER_BASIC_INC_IPCCLI_HPP_ */
