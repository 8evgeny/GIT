/*
 * block_device.hpp
 *
 *  Created on: 2 мая. 2018 г.
 *      Author: Dmitry Donskih
 *  Copyright © 2018-2020 Dmitry Donskih. All rights reserved.
 */

#ifndef DEVICES_BLOCK_DEVICE_HPP_
#define DEVICES_BLOCK_DEVICE_HPP_

#include "basic_device.h"
#include <common/blocks.h>

namespace ufw {
  namespace dev {

    /**
     * \class blockDevice - Block I/O device interface
     * \brief
     *
     */
    class blockDevice
      {
      public:
        blockDevice(uint16_t bs,size_t dev_capacity, bool is_aligned = false);

        /**
         * \fn int blockRead(void*, uint32_t)
         * \brief Reads block of data (address is automatic aligned if "allign_required" is set)
         *
         * \pre
         * \post
         * \param data
         * \param address
         * \return
         */
        virtual int blockRead(void *data, address_t address,size_t count)=0;

	/**
	 * \fn int blockWrite(void*, uint32_t)
	 * \brief Write block of data (address is automatic aligned if "allign_required" is set)
	 *
	 * \pre
	 * \post
	 * \param data
	 * \param address
	 * \return
	 */
	virtual int blockWrite(void *data, address_t address,size_t count)=0;

	int byteRead(void *data, address_t address, size_t size);
	int byteWrite(void *data, address_t address, size_t size);

	uint16_t blockSize();
	size_t capacity();

	blockDevice& interface();

      protected:
        const uint16_t block_size;
        const size_t m_capacity;
	bool align_required;

      private:
        blockDevice(const blockDevice &other) = delete;
        blockDevice(blockDevice &&other) = delete;
        blockDevice& operator=(const blockDevice &other) = delete;
        blockDevice& operator=(blockDevice &&other) = delete;
      };

  } /* namespace dev */
}
#endif /* DEVICES_BLOCK_DEVICE_HPP_ */
