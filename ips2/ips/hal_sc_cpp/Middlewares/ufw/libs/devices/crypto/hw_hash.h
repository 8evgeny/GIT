/*
 * hw_hash.h
 *
 *  Created on: 17 дек. 2020 г.
 *      Author: vader
 */

#ifndef UFW_LIBS_DEVICES_CRYPTO_HW_HASH_H_
#define UFW_LIBS_DEVICES_CRYPTO_HW_HASH_H_

#include <cstdint>
#include <cstddef>
#include <cstdlib>

#include <containers/base_container.h>
#include <common/serializer.h>
#include <types/array_type.h>

#define MD5HASH_SIZE		(16)
#define MD5HASH_BYTESIZE	(16)

#ifndef PLATFORM_TEST

#include "main.h"

#else

#endif

namespace ufw
{
using md5 = array_type<uint8_t,16>;//uint8_t[16];

class hwHash
  {
    public:
      static hwHash& Instance();
      hwHash* pointer();

      md5 compute(void *data,size_t size);
      md5 compute(ufw::baseContainer<auto> &data);
      md5 compute(ufw::iSerializable &object);
      void compute(void *data,size_t size,ufw::md5 *result);
      void compute(ufw::baseContainer<auto> &data,ufw::md5 *result);
      void compute(ufw::iSerializable &object,ufw::md5 *result);

      //md5&& compute(void *data,size_t size);


      bool check(void *data,size_t size,ufw::md5& in_hash);

    private:


      hwHash ();
      ~hwHash ();
      hwHash (const hwHash &other) = delete;
      hwHash (hwHash &&other) = delete;
      hwHash& operator= (const hwHash &other) = delete;
      hwHash& operator= (hwHash &&other) = delete;

      md5 m_hash;
      HASH_HandleTypeDef *m_handle;
  };

}

using md5hash_t = ufw::md5;

#endif /* UFW_LIBS_DEVICES_CRYPTO_HW_HASH_H_ */
