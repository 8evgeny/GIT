/*
 * mem_block.h
 *
 *  Created on: 3 дек. 2020 г.
 *      Author: Dmitry Donskih
 *  Copyright © 2020 Dmitry Donskih. All rights reserved.
 */

#ifndef LIBS_DEVICES_TYPES_MEM_BLOCK_H_
#define LIBS_DEVICES_TYPES_MEM_BLOCK_H_

#ifdef PLATFORM_TEST

#include <devices/block_device.h>
#include <devices/stream_device.h>

namespace ufw {
  namespace dev {

    /*
     *
     */
    class memBlock :public ufw::dev::blockDevice
      {
      public:
        static memBlock& Instance();
        memBlock* pointer();

	int blockRead(void *data, address_t address,size_t count)override;
	int blockWrite(void *data, address_t address,size_t count)override;
	void clear();

      private:
        memBlock();


        uint8_t block[2*1024];


	memBlock(const memBlock &other) = delete;
	memBlock(memBlock &&other) = delete;
	memBlock& operator=(const memBlock &other) = delete;
	memBlock& operator=(memBlock &&other) = delete;

	friend ttY& operator<<(ttY& terminal,ufw::dev::memBlock& data);

        int read(void *data, size_t size, address_t address);
        int write(void *data, size_t size, address_t address);
    };

  } /* namespace dev */
}
#endif
#endif /* LIBS_DEVICES_TYPES_MEM_BLOCK_H_ */
