/*
 * hwCRC.cpp
 *
 *  Created on: 6 дек. 2020 г.
 *      Author: vader
 */

#include <devices/crypto/hwCRC.h>
#include <error/error_handler.h>

CRC_HandleTypeDef hcrc;

namespace ufw
{
  
  hwCRC::hwCRC()
  :m_handle(&hcrc)
  {
    Init();
  }
  
  hwCRC& hwCRC::Instance ()
  {
    static hwCRC m_instance;
    return m_instance;
  }
  
  hwCRC* hwCRC::Pointer ()
  {
    return this;
  }

  hwCRC::~hwCRC ()
  {
  }
  
  ioresult hwCRC::setPolynomial (crcType type, uint32_t poly)
  {
    return Status2UFWresult(HAL_CRCEx_Polynomial_Set(m_handle, poly, type));
  }
  
  uint32_t hwCRC::Calculate (void *data_ptr, size_t size)
  {
    if(HAL_CRC_GetState(m_handle)==HAL_CRC_STATE_ERROR)return ioresult::ERROR;
    while(HAL_CRC_GetState(m_handle)!=HAL_CRC_STATE_READY);
    return 0xFFFFFFFF^HAL_CRC_Calculate(m_handle,reinterpret_cast<uint32_t*>(data_ptr), size);
  }
  
  uint32_t hwCRC::Calculate (ufw::baseContainer<auto>& data)
  {
    return Calculate(reinterpret_cast<void*>(data.data()), data.size());
  }
  
  uint32_t hwCRC::Calculate (ufw::iSerializable &object)
  {
    ByteArray temp;
    object.Serialize(temp);
    return Calculate(temp);		//(temp.data(), temp.size()));
  }

  void hwCRC::Init ()
  {
    using namespace ufw;

    m_handle->Instance = CRC;
    m_handle->Init.DefaultPolynomialUse = DEFAULT_POLYNOMIAL_ENABLE;  		//this settings
    m_handle->Init.DefaultInitValueUse = DEFAULT_INIT_VALUE_ENABLE;		//+result XOR FFFFFFFF
    m_handle->Init.InputDataInversionMode = CRC_INPUTDATA_INVERSION_BYTE;	//gives us IEEE 802.3
    m_handle->Init.OutputDataInversionMode = CRC_OUTPUTDATA_INVERSION_ENABLE;	//std CRC32
    m_handle->InputDataFormat = CRC_INPUTDATA_FORMAT_BYTES;
    if (HAL_CRC_Init (m_handle) != HAL_OK)
      {
	errors::Handle(errors::type::Critical, errors::place::HAL,__PRETTY_FUNCTION__);
      }
//    HAL_CRCEx_Polynomial_Set(m_handle, 0x04C11DB7, CRC_POLYLENGTH_32B);
  }

} /* namespace ufw */


#ifdef UFW_CRC32_TEST

static const char* test_hi = "Hello world";

int CRC32Test ()
{
  int test_crc_result=0;
  test_crc_result=ufw::hwCRC::Instance().Calculate((void*)test_hi, strlen(test_hi));
  return test_crc_result;
}

#endif

