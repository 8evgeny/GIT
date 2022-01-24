/*
 * flashI2C.cpp
 *
 *  Created on: 11 дек. 2020 г.
 *      Author: vader
 */

#include <devices/devices.h>
#include <devices/flash/flashI2C.h>
#include <devices/crypto/hw_hash.h>
#include <containers/ufw_vector.h>
#include <devices/terminal/ITMacrocell.hpp>

#include <system/System.h>

#ifndef PLATFORM_TEST
#include <main.h>
I2C_HandleTypeDef hi2c1;
#else

#endif

#define SYS_IS_EXT_FLASH_COMMON (0xA0)
#define SYS_IS_EXT_FLASH_SYSTEM	(0xA0)	//((0xA0)|(0x10000 >> 15))

#define UFW_PLACEHOLDER_CHECK_HASH(data_ptr,size,hash) (true)

#define HASH_PLACEHOLDER(data,size) (uint32_t(0))

static ufw::flashI2C* s_ptr=nullptr;

namespace ufw
{
  
  flashI2C& flashI2C::Instance ()
  {
    static flashI2C s_instance;
    return s_instance;
  }
  
  /** Reads blocks from device
   */
  int flashI2C::blockRead (void *data, address_t address, size_t count)
  {
    for (int i = 0; i < count; i++)
      {
	if(flash_read((uint8_t*)data+(i*block_size), address+(i*block_size),SYS_IS_EXT_FLASH_COMMON)<0)return ioresult::ERROR;
      }
    return (count*block_size);
  }
  /** Write blocks from device
   */
  int flashI2C::blockWrite (void *data, address_t address, size_t count)
  {
    for (int i = 0; i < count; i++)
      {
	if(flash_write((uint8_t*)data+(i*block_size), address+(i*block_size),SYS_IS_EXT_FLASH_COMMON)<0)return ioresult::ERROR;
      }
    return (count*block_size);
  }

  /** Creates instance of obj, init IO device
   */
  flashI2C::flashI2C()
  :blockDevice(128, 64*1024, false)
  {
    init();
    s_ptr=this;
  }
  
  void flashI2C::init ()
  {
    hi2c1.Instance = I2C1;
    hi2c1.Init.Timing = 0x00602173; //0x00300B29;
    hi2c1.Init.OwnAddress1 = 0;
    hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
    hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
    hi2c1.Init.OwnAddress2 = 0;
    hi2c1.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
    hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
    hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
    if (HAL_I2C_Init (&hi2c1) != HAL_OK)
      {
        Error_Handler ();
      }
    /** Configure Analogue filter
     */
    if (HAL_I2CEx_ConfigAnalogFilter (&hi2c1, I2C_ANALOGFILTER_ENABLE) != HAL_OK)
      {
        Error_Handler ();
      }
    /** Configure Digital filter
     */
    if (HAL_I2CEx_ConfigDigitalFilter (&hi2c1, 0) != HAL_OK)
      {
        Error_Handler ();
      }
    /** I2C Enable Fast Mode Plus
     */
    //HAL_I2CEx_EnableFastModePlus(I2C_FASTMODEPLUS_I2C1);
  }
  
  /** LL IO operations. TODO Rewrite in 1 IO procedure
   */
  int flashI2C::flash_read(void *data, address_t address,uint8_t dev_addr)
  {
    size_t saturated_size =
	((address + block_size) < m_capacity) ? block_size : (m_capacity - address); //TODO: Make an inlinee
    while (HAL_I2C_GetState (&hi2c1) != HAL_I2C_STATE_READY);
    rx_busy = true;
    if (HAL_I2C_Mem_Read (&hi2c1, dev_addr, address, I2C_MEMADD_SIZE_16BIT, (uint8_t*)data, saturated_size,1000)
	!= HAL_ERROR)
      {
	//while (rx_busy);
	return saturated_size;
      } else
      {
	ufw::System::instance ().lastFault.errno = HAL_I2C_GetError (&hi2c1);
	return ioresult::ERROR;
      }
  }
  
  flashI2C* flashI2C::pointer ()
  {
    return this;
  }

  int flashI2C::flash_write(void *data, address_t address, uint8_t dev_addr)
  {
    size_t saturated_size =
	((address + block_size) < m_capacity) ? block_size : (m_capacity - address); //TODO: Make an inlinee
    while (HAL_I2C_GetState (&hi2c1) != HAL_I2C_STATE_READY);
    tx_busy = true;
    if (HAL_I2C_Mem_Write (&hi2c1, dev_addr, address, I2C_MEMADD_SIZE_16BIT, (uint8_t*)data, saturated_size, 1000)
	!= HAL_ERROR)
      {
	//while (rx_busy);
	return saturated_size;
      } else
      {
	ufw::System::instance ().lastFault.errno = HAL_I2C_GetError (&hi2c1);
	return ioresult::ERROR;
      }
  }
  /** Callback from int
   */
  void flashI2C::_ioCallback (ufw::ioEvent cb_event, void *parameter)
  {
    if(cb_event==ufw::ioEvent::RxDone)
      {
	rx_busy=false;
      }
    else if(cb_event==ufw::ioEvent::TxDone)
      {
	tx_busy=false;
      }
  }
  
//  int flashI2C::readSysinfo (staticConfig &data)
//  {
//   if(read(&m_buffer, 0, SYS_IS_EXT_FLASH_SYSTEM)<0)return ioresult::ERROR;
//   if(!UFW_PLACEHOLDER_CHECK_HASH(&m_buffer,data.size,&m_buffer+data.size))return ioresult::ERROR;
//   data.Deserialize(&m_buffer);
//   return ioresult::OK;
//  }
//
//  int flashI2C::writeSysinfo (staticConfig &data)
//  {
//    ByteArray serialized;
//    serialized.reserve(block_size);
//    data.Serialize(serialized);
//    serialized.push_back(HASH_PLACEHOLDER(serialized.data(),serialized.size()));
//    if(write(serialized.data(), 0, SYS_IS_EXT_FLASH_SYSTEM)<0)return ioresult::ERROR;
//    return ioresult::OK;
//  }

  
  int flashI2C::readSystem (void *data, size_t size, address_t address)
  {
    if(flash_read(&m_buffer, address,SYS_IS_EXT_FLASH_SYSTEM)<0)return ufw::ioresult::ERROR;
    memcpy(data,&m_buffer,size);
    return ufw::ioresult::OK;
  }
  
  int flashI2C::writeSystem (void *data, size_t size, address_t address)
  {
    if(flash_read(&m_buffer, address,SYS_IS_EXT_FLASH_SYSTEM)<0)return ufw::ioresult::ERROR;
    memcpy(&m_buffer,data,size);
    if(flash_write(&m_buffer, address,SYS_IS_EXT_FLASH_SYSTEM)<0)return ufw::ioresult::ERROR;
    return ufw::ioresult::OK;
  }



} /* namespace ufw */

/** TODO Refactor to universal handler
 *
 */
void HAL_I2C_MemTxCpltCallback (I2C_HandleTypeDef *hi2c)
{
  if (hi2c==&hi2c1)
    {
      s_ptr->_ioCallback(ufw::ioEvent::TxDone, nullptr);
    }
}

void HAL_I2C_MemRxCpltCallback (I2C_HandleTypeDef *hi2c)
{
  if (hi2c==&hi2c1)
    {
      s_ptr->_ioCallback(ufw::ioEvent::RxDone, nullptr);
    }
}

char test_text[]=
    "\nThis is flash IO operations test message!\n\
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


void flash_check ()
{
  ufw::flashI2C& testfs=ufw::flashI2C::Instance();
  ufw::hwHash& sysmd5=ufw::hwHash::Instance();
  md5hash_t input_hash=sysmd5.compute(&test_text, sizeof(test_text));
  testfs.byteWrite(&test_text, 1024, sizeof(test_text));
  char *test_read = new char[sizeof(test_text)];
  testfs.byteRead(test_read, 1024, sizeof(test_text));
  for(size_t i=0;i<sizeof(test_text);i++)
    {
      if(test_text[i]!=test_read[i])
	{
	  coreOut::Instance()<<"mismath@"<<i<<" text="<<test_text[i]<<" read="<<test_read[i]<<"\n";
	}
    }
  if(sysmd5.check((void*)test_read, sizeof(test_text), input_hash))
    {
      coreOut::Instance()<<"HASH MATCH\n";
    }else
      {
	coreOut::Instance()<<"HASH MISMATCH\n";
      }
  coreOut::Instance()<<(const char*)test_read<<"\n";

}
