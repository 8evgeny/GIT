/*
 * staticConfig.h
 *
 *  Created on: 14 дек. 2020 г.
 *      Author: vader
 */

#ifndef UFW_LIBS_SYSTEM_STATICCONFIG_H_
#define UFW_LIBS_SYSTEM_STATICCONFIG_H_

#include <cstdint>
#include <cstddef>
#include <cstdlib>

#include <common/serializer.h>

namespace ufw
{
  
  class staticConfig : public iSerializable
  {
    public:
      char serial[12]={};
      uint8_t mac[6]={};
      uint32_t type_id=0;

//    static const size_t size=sizeof(serial)+sizeof(mac)+sizeof(type_id);
      void Serialize(ByteArray& destination) override;
      staticConfig& Deserialize(void* data);
      static constexpr size_t size()
      {
	return sizeof(serial)+sizeof(mac)+sizeof(type_id);
      }
  };

} /* namespace ufw */

#endif /* UFW_LIBS_SYSTEM_STATICCONFIG_H_ */
