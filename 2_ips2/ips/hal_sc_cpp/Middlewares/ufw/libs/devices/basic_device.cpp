/*
 *  basicDevice.cpp
 *
 *  Created on: 2 мая. 2018 г.
 *      Author: Dmitry Donskih
 *  Copyright © 2018-2020 Dmitry Donskih. All rights reserved.
 */

#include "basic_device.h"

#include <lib_macros.hpp>
#include <system/System.h>
namespace ufw {
  namespace dev {

    /**
     * \fn  basicDevice()
     * \brief The only constructor for basic device driver
     * "init()" - device
     * \pre
     * \post
     */
    basicDevice::basicDevice()
    {
//	init();
    }

    /**
     * \fn int ioctl(ctl_reg, uint8_t, uint32_t&)
     * \brief Provide access to device registers.
     *
     * \pre
     * \post
     * \param reg - address of register to access: ioops::readreg - to read and ioops::writereg - to write
     * \param request - type of operation
     * \param value
     * \return
     */
    int basicDevice::ufwioctl(ctl_reg reg, ioops request, uint32_t& value)

    {
      if(request == ioops::readreg)
	{
	  value = *reg;
	  return ioresult::OK;
	}else if(request == ioops::writereg)
	{
	  *reg = value;
	  return ioresult::OK;
	}else
	{
	  return ioresult::WRONGCMD;
	}

    }

  }
}
