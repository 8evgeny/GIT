/*
 * sys_memory.hpp
 *
 *  Created on: 26 нояб. 2020 г.
 *      Author: Dmitry Donskih
 *  Copyright © 2020 Dmitry Donskih. All rights reserved.
 */

#pragma once

#include <cstdlib>
#include <cstdint>
#include <lib_macros.hpp>
#include <allocation/heap/amazon_heap.h>
//#include <system/System.h>

#ifdef PLATFORM_TEST
#endif

#define UFW_SYSTEM_GP_BUFFER_SIZE 	2*1024
#define UFW_SYSTEM_UNCACHEBLE_SIZE 	32*1024
#define UFW_SYSTEM_GP_BUFFER_COUNT	32

namespace ufw {
  namespace dev {

    /*
     *
     */
    class sysMemory
      {
      public:
	/**/
	static sysMemory& instance();

	void init();

	void configureMPU();
	void enableCache();

	void* useGPBuffer();
	void freeGPBuffer(void *addr);
	void* allocateUncacheble(size_t size);
	size_t getFreeUncacheble();

	bool isConfigured();

	int FreeUserHeap();
	int FreeSysHeap();


	int free_buffers = UFW_SYSTEM_GP_BUFFER_COUNT;

	enum
	  {
	  MemLocked,
	  MemFree,
	  };

      private:
	sysMemory();
	uint8_t uncacheble[UFW_SYSTEM_UNCACHEBLE_SIZE];
	uint8_t *uncacheble_pos = &uncacheble[0];
	uint8_t gp_buffer_pool[UFW_SYSTEM_GP_BUFFER_COUNT][UFW_SYSTEM_GP_BUFFER_SIZE];
	char gp_buffer_state[UFW_SYSTEM_GP_BUFFER_COUNT];
	bool is_configured = false;
      };

  } /* namespace dev */
}

void heap_check(size_t count);
