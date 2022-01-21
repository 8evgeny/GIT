#ifndef BASICMESSAGE_H
#define BASICMESSAGE_H

#include <cstdlib>
#include <cstdint>
#include <vector>

#include "../tools/arcstylecrypt.hpp"

namespace ISLP {

  /**
   * @brief The basicMessage class
   * Parent class to an ISLP messages.
   */

  class basicMessage
    {      
    public:
    inline static unsigned int magicnumber_locator = 0x5e7470cd;

    public:
      basicMessage();
      basicMessage(size_t size);
      basicMessage(uint8_t *data, size_t size);
      basicMessage(const basicMessage &other);
      basicMessage(basicMessage &&other) noexcept;
      basicMessage(const std::vector<uint8_t> &data);
      basicMessage(std::vector<uint8_t> &&data);

      virtual ~basicMessage();

      basicMessage& operator=(const basicMessage &other);
      basicMessage& operator=(basicMessage &&other) noexcept;
      basicMessage& operator=(const std::vector<uint8_t> &other);
      basicMessage& operator=(std::vector<uint8_t> &&other);

      /**
       * @brief operator [] standart "Take by index" operator.
       * @param index
       * @return
       */
      uint8_t operator[](size_t index);


      uint32_t* getChecksumPtr();

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
       * @brief getSessionKey() returns pointer to session key.
       * @return
       */

      arcCrypt* getSessionKey() noexcept;

      bool checkCRC(uint32_t crc32);

    protected:
      void* getPayloadPtr();

      uint32_t* get32ptr(size_t index);

    protected:
      uint32_t getCRC();
      std::vector<uint8_t> *message_ptr = nullptr;
      std::vector<uint8_t> &message;
      arcCrypt *keyring = nullptr;
      bool isvalid = false;
    };
}
#endif // BASICMESSAGE_H
