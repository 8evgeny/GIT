/*
 * ips_call_control.h - a part of CMppCore -an object engine for Cortex-M
 * microcontrollers.
 *
 *  Created on: 27 янв. 2021 г.
 *      Author: Dmitry Donskih
 *
 * Copyright (c) 2019-2021 by Dmitry Donskih
 * All rights reserved.
 *
 * Attention Software modification allowed only with
 * the permission of the author.
 */
#ifndef UFW_LIBS_MANAGER_IPS_CALL_CONTROL_H_
#define UFW_LIBS_MANAGER_IPS_CALL_CONTROL_H_

namespace ufw
{
  
  class IPSCallControl
    {
      public:
	static IPSCallControl& Instance();

      private:
	IPSCallControl ();
	~IPSCallControl ();
	IPSCallControl (const IPSCallControl &other) = delete;
	IPSCallControl (IPSCallControl &&other) = delete;
	IPSCallControl& operator= (const IPSCallControl &other) = delete;
	IPSCallControl& operator= (IPSCallControl &&other) = delete;
    };

} /* namespace ufw */

#endif /* UFW_LIBS_MANAGER_IPS_CALL_CONTROL_H_ */
