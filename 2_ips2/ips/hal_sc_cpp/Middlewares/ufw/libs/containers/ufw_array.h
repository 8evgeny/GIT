/*
 * stream_device_test.cpp
 *
 *  Created on: 3 нояб. 2018 г.
 *      Author: Dmitry Donskih
 *  Copyright © 2018-2020 Dmitry Donskih. All rights reserved.
 */

#pragma once

#include "base_container.h"

namespace ufw {
  template<typename T>
    class ufwArray :public baseContainer<T>
      {
      public:
	explicit ufwArray(size_t size) :
	    baseContainer<T> (size)
	{
	}

      protected:
//	    virtual size_t reallocate(size_t) = delete;
      };

}
