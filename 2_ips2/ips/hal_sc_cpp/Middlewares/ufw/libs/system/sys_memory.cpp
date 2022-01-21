/*
 * sys_memory.cpp
 *
 *  Created on: 26 нояб. 2020 г.
 *      Author: Dmitry Donskih
 *  Copyright © 2020 Dmitry Donskih. All rights reserved.
 */

#ifndef PLATFORM_TEST

#include "main.h"

#else

#endif

#include <cstdint>
#include <cstddef>
#include <cstdlib>

#include "sys_memory.h"
#include <allocation/heap/amazon_heap.h>
#include <devices/terminal/ITMacrocell.hpp>
#include <types/array_type.h>

#include <cstring>
#include "FreeRTOS.h"

namespace ufw {
  
  namespace dev {

    sysMemory& sysMemory::instance()
    {
      static sysMemory s_instance;
      return s_instance;
    }

    void sysMemory::configureMPU()
    {
      if(is_configured) return;

#ifndef PLATFORM_TEST
      MPU_Region_InitTypeDef MPU_InitStruct;

      /* Disables the MPU */
      HAL_MPU_Disable ();

      MPU_InitStruct.Enable = MPU_REGION_ENABLE;
      MPU_InitStruct.Number = MPU_REGION_NUMBER4;
      MPU_InitStruct.BaseAddress = (uint32_t) &uncacheble;
      MPU_InitStruct.Size = MPU_REGION_SIZE_32KB;
      MPU_InitStruct.SubRegionDisable = 0x0;
      MPU_InitStruct.TypeExtField = MPU_TEX_LEVEL1;
      MPU_InitStruct.AccessPermission = MPU_REGION_FULL_ACCESS;
      MPU_InitStruct.DisableExec = MPU_INSTRUCTION_ACCESS_ENABLE;
      MPU_InitStruct.IsShareable = MPU_ACCESS_NOT_SHAREABLE;
      MPU_InitStruct.IsCacheable = MPU_ACCESS_NOT_CACHEABLE;
      MPU_InitStruct.IsBufferable = MPU_ACCESS_BUFFERABLE;

      HAL_MPU_ConfigRegion (&MPU_InitStruct);

//		MPU_InitStruct.Enable = MPU_REGION_ENABLE;
//		MPU_InitStruct.Number = MPU_REGION_NUMBER3;
//		MPU_InitStruct.BaseAddress = STM32_FLASH_BANK2;
//		MPU_InitStruct.Size = MPU_REGION_SIZE_256KB;
//		MPU_InitStruct.SubRegionDisable = 0x0;
//		MPU_InitStruct.TypeExtField = MPU_TEX_LEVEL1;
//		MPU_InitStruct.AccessPermission = MPU_REGION_FULL_ACCESS;
//		MPU_InitStruct.DisableExec = MPU_INSTRUCTION_ACCESS_ENABLE;
//		MPU_InitStruct.IsShareable = MPU_ACCESS_NOT_SHAREABLE;
//		MPU_InitStruct.IsCacheable = MPU_ACCESS_NOT_CACHEABLE;
//		MPU_InitStruct.IsBufferable = MPU_ACCESS_NOT_BUFFERABLE;
//
//		HAL_MPU_ConfigRegion(&MPU_InitStruct);
      /**Initializes and configures the Region and the memory to be protected for Ethernet descriptors
       */
      MPU_InitStruct.Enable = MPU_REGION_ENABLE;
      MPU_InitStruct.Number = MPU_REGION_NUMBER2;
      MPU_InitStruct.BaseAddress = 0x30000000;
      MPU_InitStruct.Size = MPU_REGION_SIZE_256B;
      MPU_InitStruct.SubRegionDisable = 0x0;
      MPU_InitStruct.TypeExtField = MPU_TEX_LEVEL0;
      MPU_InitStruct.AccessPermission = MPU_REGION_FULL_ACCESS;
      MPU_InitStruct.DisableExec = MPU_INSTRUCTION_ACCESS_ENABLE;
      MPU_InitStruct.IsShareable = MPU_ACCESS_NOT_SHAREABLE;
      MPU_InitStruct.IsCacheable = MPU_ACCESS_NOT_CACHEABLE;
      MPU_InitStruct.IsBufferable = MPU_ACCESS_BUFFERABLE;

      HAL_MPU_ConfigRegion (&MPU_InitStruct);

      /**Initializes and configures the Region and the memory to be protected for LwIP buffers
       */
      MPU_InitStruct.Enable = MPU_REGION_ENABLE;
      MPU_InitStruct.Number = MPU_REGION_NUMBER1;
      MPU_InitStruct.BaseAddress = 0x30008000;
      MPU_InitStruct.Size = MPU_REGION_SIZE_64KB; // initial 16
      MPU_InitStruct.SubRegionDisable = 0x0;
      MPU_InitStruct.TypeExtField = MPU_TEX_LEVEL0;
      MPU_InitStruct.AccessPermission = MPU_REGION_FULL_ACCESS;
      MPU_InitStruct.DisableExec = MPU_INSTRUCTION_ACCESS_ENABLE;
      MPU_InitStruct.IsShareable = MPU_ACCESS_NOT_SHAREABLE;
      MPU_InitStruct.IsCacheable = MPU_ACCESS_CACHEABLE;
      MPU_InitStruct.IsBufferable = MPU_ACCESS_NOT_BUFFERABLE;

      HAL_MPU_ConfigRegion (&MPU_InitStruct);

//      MPU_InitStruct.Enable = MPU_REGION_ENABLE;
//      MPU_InitStruct.Number = MPU_REGION_NUMBER0;
//      MPU_InitStruct.BaseAddress = 0x30000000;
//      MPU_InitStruct.Size = MPU_REGION_SIZE_16KB;
//      MPU_InitStruct.SubRegionDisable = 0x0;
//      MPU_InitStruct.TypeExtField = MPU_TEX_LEVEL1;
//      MPU_InitStruct.AccessPermission = MPU_REGION_FULL_ACCESS;
//      MPU_InitStruct.DisableExec = MPU_INSTRUCTION_ACCESS_ENABLE;
//      MPU_InitStruct.IsShareable = MPU_ACCESS_NOT_SHAREABLE;
//      MPU_InitStruct.IsCacheable = MPU_ACCESS_NOT_CACHEABLE;
//      MPU_InitStruct.IsBufferable = MPU_ACCESS_NOT_BUFFERABLE;
//
//      HAL_MPU_ConfigRegion (&MPU_InitStruct);

      /* Enables the MPU */
      HAL_MPU_Enable (MPU_PRIVILEGED_DEFAULT);
#else
      UFW_DEBUG_OUTPUT << "Static device memory config placeholder\n";
#endif

    }

    void sysMemory::enableCache ()
    {
      #ifndef PLATFORM_TEST
      SCB_EnableICache();
      SCB_EnableDCache();

      #else

      #endif

    }

    void sysMemory::init ()
    {
      if(is_configured)return;
      #ifndef PLATFORM_TEST
      configureMPU();
      enableCache();
      #else

      #endif
      heap::amazonHeap::instance();
      is_configured=true;
      return;
    }

    void* sysMemory::useGPBuffer()
    {
      for(uint8_t i = 0;i < UFW_SYSTEM_GP_BUFFER_COUNT;i++)
	{
	  if(gp_buffer_state[i] != sysMemory::MemLocked)
	    {
	      gp_buffer_state[i] = sysMemory::MemLocked;
	      free_buffers--;
	      return (void*) &gp_buffer_pool[i][0];
	    }
	}
      return nullptr;
    }

    void sysMemory::freeGPBuffer(void *addr)
    {
      for(uint8_t i = 0;i < UFW_SYSTEM_GP_BUFFER_COUNT;i++)
	{
	  if((addr >= &gp_buffer_pool[i][0])
	      && (addr < &gp_buffer_pool[i][UFW_SYSTEM_GP_BUFFER_SIZE]))
	    {
	      gp_buffer_state[i] = sysMemory::MemFree;
	      memset (&gp_buffer_pool[i][0], 0x00, UFW_SYSTEM_GP_BUFFER_SIZE);
	      free_buffers++;
	      return;
	    }
	}
    }

    void* sysMemory::allocateUncacheble(size_t size)
    {
      if(getFreeUncacheble () > size)
	{
	  uint8_t *l_buffer = uncacheble_pos;
	  uncacheble_pos += (size + size % 4);
	  return memset (l_buffer, 0, size);
	}else
	{
	  return nullptr;
	}
    }

    size_t sysMemory::getFreeUncacheble()
    {
      return UFW_SYSTEM_UNCACHEBLE_SIZE - (uncacheble_pos - &uncacheble[0]);
    }

    bool sysMemory::isConfigured()
    {
      return is_configured;
    }

    sysMemory::sysMemory()
    {
      memset (&uncacheble, 0x00, sizeof(uncacheble));
      memset (&gp_buffer_pool, 0x00, sizeof(gp_buffer_pool));
      memset (&gp_buffer_state, sysMemory::MemFree, sizeof(gp_buffer_state));
    }
    
    int sysMemory::FreeUserHeap ()
    {
      return heap::amazonHeap::instance().xPortGetFreeHeapSize();
    }
    
    int sysMemory::FreeSysHeap ()
    {
      return xPortGetFreeHeapSize();
    }
  
  } /* namespace dev */


}

void heap_check (size_t count)
{
  using charptr=uint8_t*;
  ufw::array_type<char*,550> testpool;
  size_t t_element_size=0;
  for(size_t i=0;i<testpool.size();i++)
    {
      testpool[i] = new char[++t_element_size];
      coreOut::Instance()<<"Allocated "<<t_element_size<<" bytes. Free heap:"<<heap::amazonHeap::instance().xPortGetFreeHeapSize()<<"\n";
    }
  for(size_t i=0;i<testpool.size();i++)
      {
        delete testpool[i];
        coreOut::Instance()<<"Another one deleted. Free heap:"<<heap::amazonHeap::instance().xPortGetFreeHeapSize()<<"\n";
      }
}



