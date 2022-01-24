
/*
 * cli_apps.cpp
 *
 *  Created on: 13 авг. 2020 г.
 *      Author: Dmitry Donskih
 */
#include <cli_apps.hpp>
#include <cstring>
#include "FreeRTOS.h"
#include "ipssystem.hpp"
#include <ips_globals.hpp>

#include <new>

#include <testi2c.h>
#include <dev_tests.hpp>

#include <basicDI.hpp>
#include <system_c.h>

#define TESTS

#define CLI_PARAM	reinterpret_cast<app_param_t*>(args)

void cliApplications::monitor(void *args) {
	char *cli_buffer=(char*)system_cpp.getBuffer();
	while(1){
		CLI_PARAM->terminal->printfl("\033[2J");
		vTaskList(cli_buffer);
		CLI_PARAM->terminal->write_t(cli_buffer, strlen(cli_buffer));
		memset(cli_buffer,0x00,UFW_SYSTEM_GP_BUFFER_SIZE);
		vTaskGetRunTimeStats(cli_buffer);
		CLI_PARAM->terminal->write_t(cli_buffer, strlen(cli_buffer));
		memset(cli_buffer,0x00,UFW_SYSTEM_GP_BUFFER_SIZE);
		cliApplications::freemem(args);
		osDelay(400);
	}
	system_cpp.freeBuffer(cli_buffer);

}

void cliApplications::freemem(void *args) {
	reinterpret_cast<app_param_t*>(args)->terminal->printfl("Free heap is: %d bytes\n",xPortGetFreeHeapSize());
	reinterpret_cast<app_param_t*>(args)->terminal->printfl("Free system buffers: %i \r\n",system_cpp.getFreeBuffers());

}

void cliApplications::ps(void *args) {
	char *cli_buffer=(char*)pvPortMalloc(sizeof(char[80*24]));
	if(strstr(CLI_PARAM->argv,"--cpu"))
	{
		vTaskGetRunTimeStats(cli_buffer);//vTaskGetRunTimeStats(&cli_buffer[0]);
		CLI_PARAM->terminal->write_t(cli_buffer, strlen(cli_buffer));
	} else
	{
		vTaskList(cli_buffer);
		CLI_PARAM->terminal->write_t(cli_buffer, strlen(cli_buffer));
	}
	vPortFree((void*)cli_buffer);
}

void cliApplications::setdebug(void *args) {
	ipsSystem::debug=CLI_PARAM->terminal;
	CLI_PARAM->terminal->printfl("Now this terminal is used as debug output!\r\n");
}

void cliApplications::setsysterm(void *args) {
	ipsSystem::systerm=CLI_PARAM->terminal;
	CLI_PARAM->terminal->printfl("Now this terminal is used as system output!\r\n");
}

void cliApplications::stats(void *args) {
}

void cliApplications::reboot(void *args) {
	CLI_PARAM->terminal->printfl("System restarts now!\r\n");
	system_cpp.reboot();
}

void cliApplications::kbtest(void *args){
    CLI_PARAM->terminal->printfl("I2C3 Keyboard testing software!\r\n");
    cliApplications::setdebug(args);
#ifdef SC4BOPLA
    sysKeyboardDiag();
#else
    CLI_PARAM->terminal->printfl("These aren't the boards you're looking for!\r\n");
    CLI_PARAM->terminal->printfl("It can do about his business.!\r\nThis is SC2 board it has no keyboard I/O Check begins\r\n");
    sysCheckDiscreteOutputs();
#endif
}

void cliApplications::autest(void *args){
    test_sine_audio();
}

void cliApplications::udp_player(void *args){
    test_udp_player();
}

void cliApplications::scankb(void *args)
    {
    cliApplications::setdebug(args);
#ifdef SC4BOPLA
    DIUnit::GetInstance().test();
#endif
    }

////////////////////////////////////////////////////////////////////
///
////////////////////////////////////////////////////////////////////
#ifdef TESTS
//#include <sysflash.hpp>
#include <sysRNG.hpp>
#endif

void cliApplications::makeAppList(ipcCLI *cli_processor)
{
	cli_processor->registerCommand("monitor",&cliApplications::monitor);
	cli_processor->registerCommand("freemem",&cliApplications::freemem);
	cli_processor->registerCommand("monitor",&cliApplications::monitor);
	cli_processor->registerCommand("setdebug",&cliApplications::setdebug);
	cli_processor->registerCommand("setsysterm",&cliApplications::setsysterm);
	cli_processor->registerCommand("stats",&cliApplications::stats);
	cli_processor->registerCommand("reboot",&cliApplications::reboot);
	cli_processor->registerCommand("ps",&cliApplications::ps);
	cli_processor->registerCommand("kbtest",&cliApplications::kbtest);
	cli_processor->registerCommand("autest", &cliApplications::autest);
	cli_processor->registerCommand("player", &cliApplications::udp_player);
	cli_processor->registerCommand("scankb", &cliApplications::scankb);
#ifdef TESTS
//	cli_processor->registerCommand("testflash", &testFlashFs);
//	cli_processor->registerCommand("eraseflash", &formatIntFlash);
//	cli_processor->registerCommand("testrng", &test_rng);

#endif
    }


