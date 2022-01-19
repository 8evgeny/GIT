/*
 * null_dev.h
 *
 *  Created on: 27 нояб. 2020 г.
 *      Author: Dmitry Donskih
 *  Copyright © 2020 Dmitry Donskih. All rights reserved.
 */

#pragma once

#include <devices/stream_device.h>

namespace ufw {
  namespace dev {

    /**
     * \class NullDev
     * \brief
     *
     */
    class NullDev :public streamDevice
      {
      public:
        static NullDev& Instance();
        NullDev* pointer();

        void printfl(const char *fmt, ...) final;
        void write_t(const char *buf, size_t size) final;
      protected:
	NullDev();
	~NullDev();
	void resizeBuffer(size_t size) final;


      };
  
  } /* namespace dev */
}
