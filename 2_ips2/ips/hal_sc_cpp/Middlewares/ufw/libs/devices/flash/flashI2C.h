/*
 * flashI2C.h
 *
 *  Created on: 11 дек. 2020 г.
 *      Author: vader
 */

#ifndef UFW_LIBS_DEVICES_FLASH_FLASHI2C_H_
#define UFW_LIBS_DEVICES_FLASH_FLASHI2C_H_

#include <devices/block_device.h>
#include <system/configs/static_config.h>

namespace ufw
{
  
  class flashI2C : public dev::blockDevice
  {
    public:
      static flashI2C& Instance();
      flashI2C* pointer();

      int blockRead(void *data, address_t address,size_t count) override;
      int blockWrite(void *data, address_t address,size_t count) override;

      void _ioCallback(ufw::ioEvent cb_event,void* parameter);

//      int readSysinfo(staticConfig &data);
//      int writeSysinfo(staticConfig &data);

      int readSystem(void *data,size_t size, address_t address);
      int writeSystem(void *data,size_t size, address_t address);

    private:
      flashI2C ();
      void init();
      int flash_read(void *data, address_t address,uint8_t dev_addr);
      int flash_write(void *data, address_t address,uint8_t dev_addr);



      flashI2C (const flashI2C &other) = delete;
      flashI2C (flashI2C &&other) = delete;
      flashI2C& operator= (const flashI2C &other) = delete;
      flashI2C& operator= (flashI2C &&other) = delete;

      bool rx_busy=false;
      bool tx_busy=false;
      uint8_t m_buffer[2*1024];
  };

} /* namespace ufw */

void flash_check();

#endif /* UFW_LIBS_DEVICES_FLASH_FLASHI2C_H_ */
