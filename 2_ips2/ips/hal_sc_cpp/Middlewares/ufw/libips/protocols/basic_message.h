/*
 * basic_message.h - a part of ufw++ object envirinment for
 * microcontrollers.
 *
 *   Created on: 21 дек. 2020 г. 2020 г.
 *       Author: Dmitry Donskih
 *
 * Copyright (c) 2019-2020 by Dmitry Donskih
 * All rights reserved.
 * 
 * Attention Software modification allowed only with
 * the permission of the author.
 * 
 */

#ifndef UFW_LIBIPS_PROTOCOLS_BASIC_MESSAGE_H_
#define UFW_LIBIPS_PROTOCOLS_BASIC_MESSAGE_H_

#include <cstdint>
#include <cstddef>
#include <cstdlib>

#include <containers/ufw_vector.h>
#include <devices/crypto/arcstylecrypt.hpp>


  class basicMessage
    {
    public:
      basicMessage();
      basicMessage(size_t size);
      basicMessage(uint8_t *data, size_t size);
      basicMessage(const basicMessage &other);
      basicMessage(basicMessage &&other) noexcept;
      basicMessage(const ByteArray &data);
      basicMessage(ByteArray &&data);

      virtual ~basicMessage();

      basicMessage& operator=(const basicMessage &other);
      basicMessage& operator=(basicMessage &&other) noexcept;
      basicMessage& operator=(const ByteArray &other);
      basicMessage& operator=(ByteArray &&other);

      /**
       * @brief operator [] standart "Take by index" operator.
       * @param index
       * @return
       */
      uint8_t operator[](size_t index);

      /**
       * @brief isISLP Checks is given data ISLP message
       * @return
       * @note doesn't check CRC, function ID and othe stuff. Use message specific functions.
       */
      bool isISLP();
      /**
       * @brief getFunction Returns @ref ISLP::Function "function ID".
       * @return
       */
      uint32_t getFunction();
      /**
       * @brief setMessage sets a new message in.
       * @param data
       * @param size
       */
      void setMessage(uint8_t *data, size_t size);

      /**
       * @brief size Returns size of data in.
       * @return
       */
      size_t size();

      /**
       * @brief data returns pointer to data.
       * @return
       */
      uint8_t* data();

      /**
       * @brief checkCRC
       * @return
       */
      bool checkCRC();

      /**
       * @brief checkCRC
       * @param crc32
       * @return
       */
      bool checkCRC(uint32_t crc32);

      void* getPayloadPtr();

      uint32_t* getChecksumPtr();

      uint32_t* get32ptr(size_t index);

    protected:
      uint32_t getCRC();
      //std::vector<uint8_t> *message_ptr = nullptr;
      ByteArray message;
      ufw::arcCrypt *keyring = nullptr;
      bool isvalid = false;
    };


#endif /* UFW_LIBIPS_PROTOCOLS_BASIC_MESSAGE_H_ */
