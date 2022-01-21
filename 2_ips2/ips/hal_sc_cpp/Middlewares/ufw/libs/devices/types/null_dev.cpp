/*
 * null_dev.cpp
 *
 *  Created on: 27 нояб. 2020 г.
 *      Author: Dmitry Donskih
 *  Copyright © 2020 Dmitry Donskih. All rights reserved.
 */

#include "null_dev.h"

#ifdef PLATFORM_TEST
#include <iostream>
#else

#endif

static char _placeholder = 0;

namespace ufw {
  namespace dev {

    NullDev::NullDev() :
	streamDevice (&_placeholder, sizeof(_placeholder))
    {
#ifdef PLATFORM_TEST
      std::cout << "dev::Null constructor\n";
#else

#endif
    }

    NullDev::~NullDev()
    {
#ifdef PLATFORM_TEST
      std::cout << "dev::Null destructor\n";
#else

#endif
    }

    void NullDev::resizeBuffer(size_t size)
    {
#ifdef PLATFORM_TEST
      std::cout << "dev::Null resize to " << size << "\n";
#else

#endif
    }

    void NullDev::write_t(const char *buf, size_t size)
    {
#ifdef PLATFORM_TEST
      std::cout << "dev::Null write: " << buf << "\nsize: " << size << "\n";
#else

#endif
    }

    NullDev &NullDev::Instance()
    {
      static NullDev s_instance;
      return s_instance;
    }

    NullDev *NullDev::pointer()
    {
      return this;
    }

    void NullDev::printfl(const char *fmt, ...)
    {
#ifdef PLATFORM_TEST
      std::cout << "dev::Null prinrtfl\n";
#else

#endif
    }
  
  } /* namespace dev */
}
