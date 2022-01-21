/*
 * staticConfig.cpp
 *
 *  Created on: 14 дек. 2020 г.
 *      Author: vader
 */

#include <system/configs/static_config.h>
#include <cstring>

namespace ufw
{
  
  void staticConfig::Serialize (ByteArray &destination)
  {
    destination.push_back(&serial,sizeof(serial));
    destination.push_back(&mac, sizeof(mac));

  }
  
  staticConfig& staticConfig::Deserialize (void *data)
  {
    size_t offset=0;
    memcpy(&serial[0],(uint8_t*)data,sizeof(serial));
    offset+=sizeof(serial);
    memcpy(&mac[0],(uint8_t*)data+offset,sizeof(mac));
    offset+=sizeof(mac);
    memcpy(&type_id,(uint8_t*)data+offset,sizeof(type_id));
    return *this;
  }
  

} /* namespace ufw */
