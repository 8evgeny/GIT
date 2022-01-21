#include "device_tests.h"
#include <devices/types/mem_block.h>
#include <devices/types/mem_device.h>
#include <devices/types/print_stream.h>
#include <system/system.h>

#ifdef PLATFORM_TEST
#include <cstdlib>
#include <algorithm>
#include <thread>
#include <chrono>
#include <iostream>
#endif

extern ufw::system* g_system;

static const char test_msg[]="\nThis is flash IO operations test message!\n\
    Text placeholder will write to internal flash memory to FS sectors.\n\n\
    abcdefghijklmnopqrstuvwxyz ABCDEFGHIJKLMNOPQRSTUVWXYZ 0123456789\n\
    Lorem ipsum dolor sit amet, consectetur adipiscing elit,\n\
    sed do eiusmod tempor incididunt ut labore et dolore magna aliqua.\n\
    Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris\n\
    nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor in\n\
    reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla\n\
    pariatur. Excepteur sint occaecat cupidatat non proident, sunt in\n\
    culpa qui officia deserunt mollit anim id est laborum.\n\
    That's all folks ;-)\n";

bool test_cycle(ufw::dev::blockDevice& test_device,size_t size)
{
  if(size>test_device.capacity()) return false;
  address_t address=static_cast<uint8_t>(std::rand());
  uint8_t *temp_write=new uint8_t[size];
  for(size_t i=0;i<size;i++)
    {
      temp_write[i]=static_cast<uint8_t>(std::rand());
    }
  if((test_device.byteWrite(temp_write,address,size))<0)
    {
      delete [] temp_write;
      return false;
    }
  uint8_t *temp_read=new uint8_t[size];
  if((test_device.byteRead(temp_read,address,size))<0)
    {
      delete [] temp_write;
      delete [] temp_read;
      return false;
    }
  bool result=std::lexicographical_compare(temp_write,temp_write+size,temp_read,temp_read+size,std::equal_to<uint8_t>());
  delete [] temp_write;
  delete [] temp_read;
  return result;
}

bool device_tests()
{
  using namespace std::chrono_literals;
  ufw::dev::printStream dev_print;
  ttY &dev_tty=dev_print;
  ufw::dev::memDevice &dev_basic_test=ufw::dev::memDevice::Instance();
  ufw::dev::memDevice *dev_basic_ptr=dev_basic_test.pointer();

  ufw::dev::memBlock &dev_block_test=ufw::dev::memBlock::Instance();
  ufw::dev::memBlock *dev_block_ptr=dev_block_test.pointer();

  dev_tty<<dev_basic_test<<dev_block_test;

  dev_basic_ptr->write((void*)&test_msg[0], strlen(test_msg),10);
  dev_block_ptr->byteWrite((void*)&test_msg[0],10, strlen(test_msg));

  dev_tty<<dev_basic_test<<dev_block_test;
  ufw::system* l_system=g_system;
  if(l_system!=nullptr)dev_tty<<"system started\n";
  for(size_t i=0;i<(dev_block_test.capacity()-0x100);i++)
    {
      std::cout<<"Test R/W with "<<i<<" bytes\n";
      if(!test_cycle(dev_block_test,i))
        {
          std::cout<<"Failed!\n";
        }else
        {
          std::cout<<"Succeed!\n";
        }
      dev_block_test.clear();
      std::cout.flush();
      //std::this_thread::sleep_for(100ms);
    }
  std::cout<<"This test must fall\n";
  if(test_cycle(dev_block_test,dev_block_test.capacity()+1))
    {
      std::cout<<"Succeed! But it's wrong\n";
      std::this_thread::sleep_for(500ms);
      return false;
    }
  std::cout<<"OK it's fail\n";
  std::cout.flush();
  std::this_thread::sleep_for(500ms);
  return true;
}
