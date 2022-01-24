/*
 * net_services.h - a part of ufw++ object envirinment for
 * microcontrollers.
 *
 *   Created on: 22 дек. 2020 г.
 *       Author: Dmitry Donskih
 *
 * Copyright (c) 2019-2020 by Dmitry Donskih
 * All rights reserved.
 * 
 * Attention Software modification allowed only with
 * the permission of the author.
 * 
 */

#ifndef UFW_LIBS_SYSTEM_NETSERVICES_NET_SERVICES_H_
#define UFW_LIBS_SYSTEM_NETSERVICES_NET_SERVICES_H_

#include "announcer_test.h"

namespace ufw
{
  
  class netServices
    {
      public:
	static netServices& Instance();
	netServices* pointer();

	ufw::AnnouncerTest* announce_test;

      private:
	netServices ();
	~netServices ();
	netServices (const netServices &other) = delete;
	netServices (netServices &&other) = delete;
	netServices& operator= (const netServices &other) = delete;
	netServices& operator= (netServices &&other) = delete;

    };

} /* namespace ufw */

#endif /* UFW_LIBS_SYSTEM_NETSERVICES_NET_SERVICES_H_ */
