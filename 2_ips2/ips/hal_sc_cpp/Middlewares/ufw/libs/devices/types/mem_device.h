/*
 * mem_device.h
 *
 *  Created on: 3 дек. 2020 г.
 *      Author: Dmitry Donskih
 *  Copyright © 2020 Dmitry Donskih. All rights reserved.
 */

#ifndef LIBS_DEVICES_TYPES_MEM_DEVICE_H_
#define LIBS_DEVICES_TYPES_MEM_DEVICE_H_

#ifdef PLATFORM_TEST

#include <devices/basic_device.h>
#include <devices/stream_device.h>


namespace ufw {
  
  namespace dev {

    /*
     *
     */
    class memDevice :public ufw::dev::basicDevice
      {
      public:
        static memDevice& Instance();
        memDevice* pointer();

	int read(void *data, size_t size, uint32_t address) override;

	int write(void *data, size_t size, uint32_t address) override;

	int ioctl(ctl_reg reg, ioops request, uint32_t &value) override;

	void clear();

      private:
        memDevice();

	memDevice(const memDevice &other) = delete;
	memDevice(memDevice &&other) = delete;
	memDevice& operator=(const memDevice &other) = delete;
	memDevice& operator=(memDevice &&other) = delete;

	uint8_t block[2*1024];

        friend ttY& operator<<(ttY& terminal,ufw::dev::memDevice& data);
      };

  } /* namespace dev */

}

#endif
#endif /* LIBS_DEVICES_TYPES_MEM_DEVICE_H_ */
