/*
 * ufwip.h - a part of ufw++ object envirinment for uControllers
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

#ifndef LIBS_NETWORKING_UFWIP_H_
#define LIBS_NETWORKING_UFWIP_H_

#include <cstdint>
#include <cstddef>

#include <lib_asserts.h>
#include <lib_macros.hpp>

#include <type_traits>

namespace ufw
{
  using ipaddr_t=uint32_t;
  class IP
  {
    public:
      IP()=default;
      IP(const char* cstr);
      IP(void* data);
      IP(uint32_t data);

//      IP& operator=(T data);

//      template<typename T,typename std::enable_if<std::is_pointer<T>>>
//      IP(T data);
//      template<typename T,typename std::enable_if<std::is_pointer<T>>>
//      IP& operator=(T data);

//      bool cstr(const char*)
      uint8_t* operator()();
      int operator int();
      uint8_t operator[](size_t index);
    private:
      ipaddr_t adress={0};


  };
  using macaddr_t=uint8_t[12];
  struct netSettongs
  {
	  IP address;
	  IP mask;
	  IP gateway;
  };

} /* namespace ufw */




#endif /* LIBS_NETWORKING_UFWIP_H_ */
