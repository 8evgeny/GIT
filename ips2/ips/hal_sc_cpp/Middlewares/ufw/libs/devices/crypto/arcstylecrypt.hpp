#pragma once

#include <containers/ufw_vector.h>
#include <cstddef>
#include <cinttypes>

namespace ufw
{
  /**
   * @brief The arcCrypt class
   *
   * Simple additive cipher.
   * ~~~~~~~
   * Output[i]=Input[i] ^ cipher[i`%`cipher.size]
   * ~~~~~~~
   */
  class arcCrypt
  {
    public:
      /**
       * @brief arcCrypt Simple constructor from pointer to key
       * @param key pointer
       * @param size size of key(in bytes)
       */
      arcCrypt (void *key, size_t size);
      arcCrypt (const ByteArray &key);
      /**
       * @brief decrypt
       * @param data
       * @param size
       */
      void decrypt (void *data, size_t size);
      /**
       * @brief encrypt
       * @param data
       * @param size
       */
      void encrypt (void *data, size_t size);
      /**
       * @brief decrypt (Single int version)
       * @param data
       * @return
       */
      unsigned int decrypt (unsigned int data);
      /**
       * @brief encrypt (Single int version)
       * @param data
       * @return
       */
      unsigned int encrypt (unsigned int data);
      /**
       * @brief takeKey returns pointer to key
       * @return
       */
      const uint8_t* takeKey ();
      /**
       * @brief setKey Set new key
       * @param key
       * @param size
       */
      void setKey (void *key, size_t size);

    private:
      static void crypt (void *data, size_t size, ByteArray &key);

      ByteArray cipher;

  };

}
