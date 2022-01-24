/*
 * hw_hash.cpp
 *
 *  Created on: 17 дек. 2020 г.
 *      Author: vader
 */

#include <cstring>

#include <devices/crypto/hw_hash.h>
#include <containers/ufw_vector.h>
#include <lib_asserts.h>

#ifndef PLATFORM_TEST

HASH_HandleTypeDef hhash;

#else

#endif

#define LIB_HASH_ERROR  result->clear(); // TODO: PLACEHOLDER Write real error handle code

#ifndef UFW_HASH_TIMEOUT
#define UFW_HASH_TIMEOUT 100
#endif

namespace ufw
{
//  md5::md5 (uint32_t *data)
//  :md5()
//  {
//    memcpy(&m_md5data,data,MD5HASH_BYTESIZE);
//  }
//
//  md5::md5 ()
//  {
//  }
//
//  uint32_t* md5::data ()
//  {
//    return &m_md5data[0];
//  }
//
//  void* md5::address ()
//    {
//      return &m_md5data;
//    }
//
//
//  size_t md5::size ()
//  {
//    return MD5HASH_BYTESIZE;
//  }
//
//  size_t md5::length ()
//  {
//    return MD5HASH_SIZE;
//  }
//
//  uint32_t& md5::operator [] (size_t index)
//  {
//    LIB_ASSERT((index<MD5HASH_SIZE))
//    return *(&m_md5data[0]+index);
//  }
//
//  void md5::clear ()
//  {
//    memset(&m_md5data,0x00,MD5HASH_BYTESIZE);
//  }

  
  hwHash& hwHash::Instance ()
  {
    static hwHash s_hasher;
    return s_hasher;
  }
  
  hwHash* hwHash::pointer ()
  {
    return this;
  }

  hwHash::hwHash ()
  :m_handle(&hhash)
  {
    m_handle->Init.DataType = HASH_DATATYPE_8B;
    if (HAL_HASH_Init (m_handle) != HAL_OK)
      {
	LIB_HALERROR_HANDLE;
      }
  }
  
  md5 hwHash::compute(void *data, size_t size)
  {
    m_hash.clear();
    compute(data,size,&m_hash);
    return m_hash;

  }

  void hwHash::compute (void *data, size_t size, ufw::md5 *result)
  {
    if(!result)return;
    while(m_handle->State==HAL_HASH_STATE_BUSY);

    if(HAL_HASH_GetStatus(m_handle)!=HAL_OK)
      {
	LIB_HASH_ERROR
	return;
      }
    if(HAL_HASH_MD5_Start(m_handle,(uint8_t*)data, size, result->data(), UFW_HASH_TIMEOUT)!=HAL_OK)
      {
	LIB_HASH_ERROR
	return;
      }
    return;
  }
  
  void hwHash::compute (ufw::baseContainer<auto> &data, ufw::md5 *result)
  {
    compute(data.data(),data.size(),result);
  }

  void hwHash::compute (ufw::iSerializable &object, ufw::md5 *result)
  {
    ByteArray temp;
    object.Serialize(temp);
    compute(temp.data(),temp.size(),result);
  }

  bool hwHash::check (void *data, size_t size, ufw::md5 &in_hash)
  {
    m_hash.clear();
    compute(data,size,&m_hash);
    for(uint8_t i=0;i<MD5HASH_SIZE;i++)
      {
	if(m_hash[i]!=in_hash[i])return false;
      }
    return true;
  }

//  md5&& hwHash::compute(void *data,size_t size)
//  {
//    md5 temp;
//    compute(data, size, &temp);
//    return std::move(temp);
//  }

  hwHash::~hwHash ()
  {
  }
  


} /* namespace ufw */
