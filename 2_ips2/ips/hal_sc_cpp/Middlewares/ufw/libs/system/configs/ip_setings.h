/*
 * ip_setings.h
 *
 *  Created on: 18 дек. 2020 г.
 *      Author: vader
 */

#pragma once

#include <common/serializer.h>
#include <containers/ufw_vector.h>
#include <identificators/intercomstation.hpp>
#include <sys/_stdint.h>
#include <cstddef>

namespace ufw
{
  
  class IpSetings : public iSerializable
  {
    public:
      IpSetings()=default;
      explicit IpSetings(uint32_t ip);
      IpSetings(const intercomStation& other);
      IpSetings(intercomStation&& other);
      virtual ~IpSetings();
      uint8_t ipaddr[4]{192,168,0,100};
      uint8_t netmask[4]{255,255,255,0};
      uint8_t gateway[4]{192,168,0,1};

      void Serialize (ByteArray &destination)override;
      ByteArray Serialize();

      IpSetings& Deserialize (void *data);
      IpSetings& operator=(const intercomStation& other);
      IpSetings& operator=(intercomStation&& other);

      static constexpr size_t size()
      {
	return sizeof(ipaddr)+sizeof(netmask)+sizeof(gateway);
      }
  };
} /* namespace ufw */

#include <system/System.h>

void _t_test_serialize_crc();

