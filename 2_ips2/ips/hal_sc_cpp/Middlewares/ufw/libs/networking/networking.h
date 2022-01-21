/*
 * networking.h - a part of ufw++ object envirinment for uControllers
 * microcontrollers.
 *
 *  Created on: 10 дек. 2020 г.
 *      Author: Dmitry Donskih
 *
 * Copyright (c) 2019-2020 by Dmitry Donskih
 * All rights reserved.
 *
 * Attention Software modification allowed only with
 * the permission of the author.
 */
#ifndef LIBS_NETWORKING_NETWORKING_H_
#define LIBS_NETWORKING_NETWORKING_H_

#define PLATFORM_TEST //TODO Remove
#ifndef PLATFORM_TEST



#else

#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#endif




#endif /* LIBS_NETWORKING_NETWORKING_H_ */
