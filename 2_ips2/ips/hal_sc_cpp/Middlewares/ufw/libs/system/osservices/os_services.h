/*
 * os_services.h - a part of ufw++ object envirinment for
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

#pragma once

#include <taskManagement.hpp>
#include "thread_manager.h"
#include <basicDO.h>
#include <basicDI.hpp>
#include <system/proxy/keyboard_handler.h>

namespace ufw
{
  
  class osServices
    {
      public:
	static osServices& Instance();
	osServices* pointer();
	DOUnit* outputs=nullptr;
	DIUnit* inputs=nullptr;
	KeyboardHandler* keyboard_svc=nullptr;

	sysTaskManagement* TaskManager;

	void AfterIP();

      private:
	osServices ();
	~osServices ();
	osServices (const osServices &other) = delete;
	osServices (osServices &&other) = delete;
	osServices& operator= (const osServices &other) = delete;
	osServices& operator= (osServices &&other) = delete;

    };

} /* namespace ufw */


