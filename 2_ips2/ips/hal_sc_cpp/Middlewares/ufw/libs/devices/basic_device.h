/*
 *  basicDevice.hpp
 *
 *  Created on: 2 мая. 2018 г.
 *      Author: Dmitry Donskih
 *  Copyright © 2018-2020 Dmitry Donskih. All rights reserved.
 */

#ifndef UNTITLED_BASICDEVICE_HPP
#define UNTITLED_BASICDEVICE_HPP

#include <devices/devices.h>

#include <cstdlib>
#include <cstdint>

typedef uint32_t *ctl_reg;
namespace ufw {
  namespace dev {
    enum ioops
      {
      readreg = uint8_t (0x01),
      writereg = uint8_t (0x00)
      };


    /**
     * \class basicDevice
     * \brief - pure virtual class for device driver
     *
     */
    class basicDevice
      {
      public:

	basicDevice();

	virtual int read(void *data, size_t size, uint32_t address) = 0;

	virtual int write(void *data, size_t size, uint32_t address) = 0;

	virtual int ufwioctl(ctl_reg reg, ioops request, uint32_t &value);

      protected:

      private:

      };
  }
}
#endif //UNTITLED_BASICDEVICE_HPP
