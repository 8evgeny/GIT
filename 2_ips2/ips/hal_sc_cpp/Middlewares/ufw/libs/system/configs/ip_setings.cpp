/*
 * ip_setings.cpp
 *
 *  Created on: 18 дек. 2020 г.
 *      Author: vader
 */

#include <system/configs/ip_setings.h>
#include <cstring>
#include <utility>

namespace ufw
{
  
  void IpSetings::Serialize (ByteArray &destination)
  {
    destination.push_back(&ipaddr,sizeof(ipaddr));
    destination.push_back(&netmask,sizeof(netmask));
    destination.push_back(&gateway,sizeof(gateway));
  }
  
  IpSetings& IpSetings::Deserialize (void *data)
  {
    size_t offset=0;
    memcpy(&ipaddr[0],data,sizeof(ipaddr));
    offset+=sizeof(ipaddr);
    memcpy(&netmask[0],data,sizeof(netmask));
    offset+=sizeof(netmask);
    memcpy(&gateway[0],data,sizeof(gateway));
    offset+=sizeof(gateway);
    return *this;
  }
  
  IpSetings& IpSetings::operator = (const intercomStation &other)
  {
    memcpy(&ipaddr,&(other.ipaddr),sizeof(ipaddr));
    memcpy(&netmask,&other.netmask,sizeof(netmask));
    memcpy(&gateway,&other.gateway,sizeof(gateway));
    return *this;
  }
  
  ByteArray IpSetings::Serialize ()
  {
    ByteArray temp;
    Serialize(temp);
    return temp;
  }

  IpSetings& IpSetings::operator = (intercomStation &&other)
  {
    std::swap(ipaddr, other.ipaddr);
    std::swap(netmask, other.netmask);
    std::swap(gateway, other.gateway);
    return *this;
  }
  
  IpSetings::IpSetings (const intercomStation &other)
  {
    memcpy(&ipaddr,&other.ipaddr,sizeof(ipaddr));
    memcpy(&netmask,&other.netmask,sizeof(netmask));
    memcpy(&gateway,&other.gateway,sizeof(gateway));
  }
  
  IpSetings::IpSetings (intercomStation &&other)
  {
    std::swap(ipaddr, other.ipaddr);
    std::swap(netmask, other.netmask);
    std::swap(gateway, other.gateway);
  }
  
  IpSetings::IpSetings (uint32_t ip)
  :netmask{255,255,255,0}
  {
    memcpy(&ipaddr,&ip,sizeof(uint32_t));
    memcpy(&gateway,&ip,sizeof(uint32_t));
    gateway[3]=1;
  }
  
  IpSetings::~IpSetings ()
  {
  }

} /* namespace ufw */



void _t_test_serialize_crc ()
{
  ufw::IpSetings test_ip;
  uint32_t test1=ufw::System::instance().CRC32().Calculate(test_ip);
  ufw::System::instance().stddebug()<<"Checksum Serial="<<test1<<"\n";
  uint32_t test2=ufw::System::instance().CRC32().Calculate(&test_ip,ufw::IpSetings::size());
  ufw::System::instance().stddebug()<<"Checksum object="<<test2<<"\n";
}
