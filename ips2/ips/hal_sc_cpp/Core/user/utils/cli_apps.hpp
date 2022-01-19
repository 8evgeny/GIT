/*
 * cli_apps.hpp
 *
 *  Created on: 13 авг. 2020 г.
 *      Author: Dmitry Donskih
 */

#ifndef USER_UTILS_CLI_APPS_HPP_
#define USER_UTILS_CLI_APPS_HPP_

#include <ipcCLI.hpp>


namespace cliApplications{
void monitor(void* args);
void freemem(void* args);
void ps(void* args);
void setdebug(void *args);
void setsysterm(void *args);
void stats(void *args);
void reboot(void *args);

void kbtest(void *args);

void autest(void *args);

void udp_player(void *args);

void scankb(void* args);

void makeAppList(ipcCLI *cli_processor);
}
#endif /* USER_UTILS_CLI_APPS_HPP_ */
