/*
 * ips_keyboard.h - a part of CMppCore -an object engine for Cortex-M
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
#ifndef UFW_LIBS_MANAGER_IPS_KEYBOARD_H_
#define UFW_LIBS_MANAGER_IPS_KEYBOARD_H_

#include <system/configs/config_types.h>
#include <functional>

namespace ufw
{
  struct Key{
      uint32_t function;
      uint32_t destination;
      uint32_t parameters;
      uint32_t prio;
  };
  
  class IPSKeyboard
    {
      public:
	IPSKeyboard ();
	Key& operator[](int index);


      private:
	Key Keypad[32]{};
	int LoadKeys();
	~IPSKeyboard ();
	IPSKeyboard (const IPSKeyboard &other) = delete;
	IPSKeyboard (IPSKeyboard &&other) = delete;
	IPSKeyboard& operator= (const IPSKeyboard &other) = delete;
	IPSKeyboard& operator= (IPSKeyboard &&other) = delete;
    };
} /* namespace ufw */

#endif /* UFW_LIBS_MANAGER_IPS_KEYBOARD_H_ */
