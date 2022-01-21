/*
 * system.cpp
 *
 *  Created on: 26 нояб. 2020 г.
 *      Author: Dmitry Donskih
 *  Copyright © 2020 Dmitry Donskih. All rights reserved.
 */

#ifndef PLATFORM_TEST

#include "main.h"
#include "hal_init.h"

#else

#endif

#include <system/System.h>
#include <error/error_handler.h>
#include <devices/crypto/hwCRC.h>
#include <devices/crypto/hw_hash.h>
#include <devices/flash/flashI2C.h>
#include <system/core/clock_system.h>
#include <system/configs/ip_setings.h>



extern __IO uint32_t uwTick;

namespace ufw
{
  
  System::System ()
  {
    if (initHW () < 0) errors::Handle (errors::Critical, errors::HAL, __PRETTY_FUNCTION__);
  }

  System& System::instance ()
  {
    static System s_instance;
    return s_instance;
  }

  System* System::pointer ()
  {
    return this;
  }

  int System::initHW ()
  {
#ifndef PLATFORM_TEST
//    coreOut::Instance()<<"Ok start. Init Core Clock..";
    ClockSystem::Instance();
    coreOut::Instance()<<".Ok\nInit internal memory..";
    dev::sysMemory::instance().init();
    coreOut::Instance()<<"Memory init sucsessful.\nInit HAL..";
    if(HAL_Init()!=HAL_OK)return -1;
    HAL_BSP_Init ();
    hwCRC::Instance();
    hwHash::Instance();
    m_config_flash=flashI2C::Instance().pointer();
    return 0;

#else

	    return 0;
#endif


  }

  System::~System ()
  {
//    errors::Handle(errors::Critical, errors::Logical,__PRETTY_FUNCTION__);
  }
  
  void* System::getBuffer ()
  {
    return m_memory.useGPBuffer ();
  }
  
  void System::freeBuffer (void *buffer)
  {
    m_memory.freeGPBuffer (buffer);
  }
  
  int System::getFreeBuffers ()
  {
    return m_memory.free_buffers;
  }
  
  void* System::allocateDmaBuffer (size_t size)
  {
    return m_memory.allocateUncacheble (size);
  }
  
  ufw_DeviceAllocator<uint8_t>& System::deviceMemory ()
  {
    return m_devicemem_manager;
  }
  
  ufw_StaticAllocator<uint8_t>& System::staticMemory ()
  {
    return m_staticmem_manager;
  }
  
  void System::reboot ()
  {
    NVIC_SystemReset ();
  }
  
  hwHash& System::Hash ()
  {
    return hwHash::Instance();
  }
  
  hwCRC& System::CRC32 ()
  {
    return hwCRC::Instance();
  }
  

  void System::osStartedCallback ()
  {
    flag_os_running=true;
    m_settings=ufw::DeviceConfig::Instance().pointer();

    stddebug()<<uwTick<<" Amazon FreeRTOS Kernell running\n";
    m_osServices=ufw::osServices::Instance().pointer();
  }
  
  ttY& System::stddebug ()
  {
    return system_debug;
  }
  
  bool System::isOSloaded ()
  {
    return flag_os_running;
  }
  
  void System::ipStackReadyCallback ()
  {
    flag_ip_up=true;
    stddebug()<<uwTick<<"LwIP running\n";
    m_net_services=ufw::netServices::Instance().pointer();
  }
  
  bool System::isIPready ()
  {
    return flag_ip_up;
  }
  
  osServices& System::servicesOS ()
  {
    if(!m_osServices)errors::Handle(errors::Assert, errors::Assertation, "You try get OS before OS starts\n");
    return *m_osServices;
  }

  netServices& System::servicesNet ()
  {
    if(!m_net_services)errors::Handle(errors::Assert, errors::Assertation, "Net services not ready yet\n");
    return *m_net_services;
  }
  
  flashI2C& System::ConfigFlash ()
  {
    return *m_config_flash;
  }
  
  ufw::dev::sysMemory& System::MemoryManager()
  {
    return m_memory;
  }

}
