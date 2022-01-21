/*
 * ufwip.cpp - a part of ufw++ object envirinment for uControllers
 * microcontrollers.
 *
 *  Created on: 9 дек. 2020 г.
 *      Author: Dmitry Donskih
 *
 * Copyright (c) 2019-2020 by Dmitry Donskih
 * All rights reserved.
 *
 * Attention Software modification allowed only with
 * the permission of the author.
 */
#include "ufwip.h"
#include "networking.h"

namespace ufw
{
  
  uint8_t* IP::operator()()
  {
	return reinterpret_cast<uint8_t*>(&adress);
  }
  
  
  int IP::operator int ()
  {
	return adress;
  }
  
  IP::IP (const char *cstr)
  {
	adress= inet_addr(cstr);
  }
  
  IP::IP (void *data)
  {
  }
  
  IP::IP (uint32_t data)
  {
	adress=htonl(data);
  }
  
  
//  bool IP::cstr ()
//  {
//	return false;
//  }

  uint8_t IP::operator [] (size_t index)
  {
	if(index>3)
	  {
		return 0; ///make an error
	  }
	return reinterpret_cast<uint8_t*>(&adress)[index];
  }

} /* namespace ufw */
