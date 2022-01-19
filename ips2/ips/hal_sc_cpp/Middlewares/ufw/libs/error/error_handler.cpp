/*
 * error_handler.cpp
 *
 *  Created on: 26 нояб. 2020 г.
 *      Author: Dmitry Donskih
 *  Copyright © 2020 Dmitry Donskih. All rights reserved.
 */

#ifndef PLATFORM_TEST

#include "main.h"
#include <devices/terminal/ITMacrocell.hpp>

#else

#endif

#include "error_handler.h"
extern __IO uint32_t uwTick;
#ifdef PLATFORM_TEST
#include <iostream>
#endif
namespace ufw {
  namespace errors {

    const char *g_error_type_string[] =
      { "Warning", "Moderate", "Critical","Assert" };
    const char *g_error_place_string[] =
      { "HAL", "IO", "Memory", "CPU", "Logical", "OS","Assertation" };
    /* namespace error */

    void Handle(type type_error, place place_error)
    {
      Handle (type_error, place_error, nullptr);
    }

    void Handle(type type_error, place place_error, const char *message)
    {
#ifdef PLATFORM_TEST
      std::cout << "Error handler exec with reason " << errors::g_error_type_string[type_error]
	  << "::" << errors::g_error_place_string[place_error] << "\n";
      if(message)
	{
	  std::cout << message << "\n";
	}
      exit (-100);
#else
      ufw::ioITM::Instance()<<uwTick<<" Error @"<<errors::g_error_type_string[type_error]
	  <<"::"<<errors::g_error_place_string[place_error] << "\n";
      if(message)
	{
	  ufw::ioITM::Instance()<< message << "\n";
	}
      Error_Handler();

#endif
    }

  }
}

void cErrorHandler (const char *text)
{
  ufw::errors::Handle(ufw::errors::type::Critical, ufw::errors::place::Logical,text);
}
