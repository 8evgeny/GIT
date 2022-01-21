/*
 * system.hpp
 *
 *  Created on: 26 нояб. 2020 г.
 *      Author: Dmitry Donskih
 *  Copyright © 2020 Dmitry Donskih. All rights reserved.
 */

#pragma once


#include <ufw_allocation.h>
#include <devices/terminal/ITMacrocell.hpp>
#include <devices/crypto/hwCRC.h>
#include <devices/crypto/hw_hash.h>
#include <devices/flash/flashI2C.h>
#include <system/osservices/os_services.h>
#include <system/netservices/net_services.h>
#include <system/configs/device_config.h>
#include "sys_memory.h"
/**
 * An palceholder Macro;
 */
#define UFWINS	ufw::System::instance()

struct sysFaultCode{
    uint32_t dev_id=0;
    uint32_t errno=0;
};

namespace ufw {

  class System final
    {
    public:
      static System& instance();
      System *pointer();

      int initHW();

      void* getBuffer();
      void freeBuffer(void *buffer);
      int getFreeBuffers();
      void* allocateDmaBuffer(size_t size);

      void osStartedCallback();
      bool isOSloaded();
      void ipStackReadyCallback();
      bool isIPready();

      hwHash& Hash();
      hwCRC& CRC32();
      ttY& stddebug();
      flashI2C& ConfigFlash();
      osServices& servicesOS();
      netServices& servicesNet();

      DeviceConfig* m_settings=nullptr;

      ufw::dev::sysMemory& MemoryManager();


      ufw_DeviceAllocator<uint8_t>& deviceMemory();
      ufw_StaticAllocator<uint8_t>& staticMemory();

      const size_t buffer_size = UFW_SYSTEM_GP_BUFFER_SIZE;

      void reboot();

      sysFaultCode lastFault;

    private:
      bool flag_os_running=false;
      bool flag_ip_up=false;

      flashI2C* m_config_flash=nullptr;

      osServices* m_osServices=nullptr;
      netServices* m_net_services=nullptr;
      ttY& system_debug=ufw::ioITM::Instance();
      dev::sysMemory &m_memory=dev::sysMemory::instance();
      ufw_DeviceAllocator<uint8_t> m_devicemem_manager;
      ufw_StaticAllocator<uint8_t> m_staticmem_manager;


      System();
      ~System();
      System(const System &other) = delete;
      System(System &&other) = delete;
      System& operator=(const System &other) = delete;
      System& operator=(System &&other) = delete;
    };

}


