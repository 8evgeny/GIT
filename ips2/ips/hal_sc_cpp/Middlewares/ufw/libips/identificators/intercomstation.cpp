#include "intercomstation.hpp"
#include <common/ips_helpers.hpp>
//#include "debug_macro.h"
//#include <names/ipsnames.hpp>
#include <system/configs/ip_setings.h>

#include <cstring>

intercomStation::intercomStation()
:serial(&serial_number[0]),
 mac(&macaddr[0])
{
}

intercomStation::intercomStation(uint8_t *input)
:intercomStation()
{
  setupData (input);
}

void intercomStation::setupData(uint8_t *input)
{
  uint32_t offset = 0;
  memcpy (&serial_number, input + offset, sizeof(serial_number));
  offset += sizeof(serial_number);
  memcpy (&macaddr, input + offset, sizeof(macaddr));
  offset += sizeof(macaddr);
  memcpy (&board_type, input + offset, sizeof(board_type));
  offset += sizeof(board_type);
  memcpy (&ipaddr, input + offset, sizeof(ipaddr));
  offset += sizeof(ipaddr);
  memcpy (&netmask, input + offset, sizeof(netmask));
  offset += sizeof(netmask);
  memcpy (&gateway, input + offset, sizeof(gateway));
  offset += sizeof(gateway);
  memcpy (&name, input + offset, sizeof(name));
}

void intercomStation::setName(const char *newname)
{
  memset (&name, 0, sizeof(name));
  if(strlen (newname) >= sizeof(name))
    {
      memcpy (&name, newname, sizeof(name));
    }else
    {
      memcpy (&name, newname, strlen (newname));
    }

}

void intercomStation::copy(const intercomStation &other)
{
  memcpy (&this->ipaddr, &(other.ipaddr), sizeof(intercomStation::ipaddr));
  memcpy (&this->netmask, &other.netmask, sizeof(intercomStation::netmask));
  memcpy (&this->gateway, &other.gateway, sizeof(intercomStation::gateway));
  memcpy (&this->name, &other.name, sizeof(intercomStation::name));
}

void intercomStation::getSerial(char *data) const
{
  if(data) memcpy (data, &serial_number, sizeof(serial_number));
}

void intercomStation::getMAC(uint8_t *data) const
{
  if(data) memcpy (data, &macaddr, sizeof(macaddr));
}

uint32_t intercomStation::getIPaddr(uint8_t *data) const
{
  if(data) memcpy (data, &ipaddr, sizeof(ipaddr));
  return *(reinterpret_cast<uint32_t*> (&data));
}
//
//QString intercomStation::getSerialStr() const
//{
//  QString output;
//  for(char x : serial_number)
//    {
//      output = output + x;
//    }
//  return output;
//}
//
//QString intercomStation::getMACStr() const
//{
//  QString output;
//  for(uint8_t x : macaddr)
//    {
//      output = output + (QString::number (x, 16) + ":");
//    }
//  output.remove (output.length () - 1, 1);
//  return output;
//}
//
//QString intercomStation::getTypeStr() const
//{
//  return IPS::Stations::QTypeName (board_type);
//}
//
//QString intercomStation::getIPaddr() const
//{
//  QString output;
//  for(uint8_t x : ipaddr)
//    {
//      output = output + (QString::number (x) + ".");
//    }
//  output.remove (output.length () - 1, 1);
//  return output;
//}
//
//QString intercomStation::getNetmask() const
//{
//  QString output;
//  for(uint8_t x : netmask)
//    {
//      output = output + (QString::number (x) + ".");
//    }
//  output.remove (output.length () - 1, 1);
//  return output;
//}
//
//QString intercomStation::getGateway() const
//{
//  QString output;
//  for(uint8_t x : gateway)
//    {
//      output = output + (QString::number (x) + ".");
//    }
//  output.remove (output.length () - 1, 1);
//  return output;
//}
//
//QString intercomStation::getName() const
//{
//  QString output;
//  for(char x : name)
//    {
//      output = output + x;
//    }
//  return output;
//}
#ifdef IPS_USING_QT
void intercomStation::print()
{
    IPS_DEBUG_OUTPUT_FCN<<"Station:";
    IPS_DEBUG_OUTPUT_FCN<<getName()<<" ("<<getTypeStr()<<")";
    IPS_DEBUG_OUTPUT_FCN<<"S/N    :"<<getSerialStr();
    IPS_DEBUG_OUTPUT_FCN<<"MAC    :"<<getMACStr();
    IPS_DEBUG_OUTPUT_FCN<<"IP     :"<<getIPaddr();
    IPS_DEBUG_OUTPUT_FCN<<"Mask   :"<<getNetmask();
    IPS_DEBUG_OUTPUT_FCN<<"Gateway:"<<getGateway();
    if (isvalid)
    {
        IPS_DEBUG_OUTPUT_FCN<<"Station has valid sign!";
    }else
    {
        IPS_DEBUG_OUTPUT_FCN<<"Warning station is marked as invalid!";
    }

}
#endif

void intercomStation::setIpSettings (ufw::IpSetings &other)
{
  memcpy(&ipaddr,&other.ipaddr,sizeof(ipaddr));
  memcpy(&netmask,&other.netmask,sizeof(netmask));
  memcpy(&gateway,&other.gateway,sizeof(gateway));
}

ByteArray intercomStation::serialize()
{
  ByteArray out;
  serialize (&out);
  return out;
}

uint16_t intercomStation::type () const
{
  return board_type;
}

void intercomStation::serialize(ByteArray *output) const
{
  for(char x : serial_number)
    {
      output->push_back ((uint8_t) x);
    }
  for(uint8_t x : macaddr)
    {
      output->push_back (x);
    }
  Helpers::appendToByteVectorByVal<uint16_t> (output, board_type);
  for(uint8_t x : ipaddr)
    {
      output->push_back (x);
    }
  for(uint8_t x : netmask)
    {
      output->push_back (x);
    }
  for(uint8_t x : gateway)
    {
      output->push_back (x);
    }
  for(char x : name)
    {
      output->push_back ((uint8_t) x);
    }
}

//bool operator==(const IPS::SerialNumber& lv,const IPS::SerialNumber& rv)
//{
//    for(size_t i=0;i<sizeof (lv.c);i++)
//    {
//        if(lv.c[i]!=rv.c[i])return false;
//    }
//    return true;
//}
//bool operator!=(const IPS::SerialNumber& lv,const IPS::SerialNumber& rv)
//{
//    for(size_t i=0;i<sizeof (lv.c);i++)
//    {
//        if(lv.c[i]==rv.c[i])return false;
//    }
//    return true;
//}

//bool operator==(const IPS::MAC& lv,const IPS::MAC& rv)
//{
//    if(lv.i==rv.i){return true;}else{return false;}
//}

//bool operator!=(const IPS::MAC& lv,const IPS::MAC& rv)
//{
//    if(lv.i!=rv.i){return true;}else{return false;}
//}

bool operator==(const intercomStation &lv, const intercomStation &rv)
{
  return (lv.serial==rv.serial);
}

bool operator!=(const intercomStation &lv, const intercomStation &rv)
{
  return (lv.serial!=rv.serial);
}

void intercomStation::setUnique (ufw::staticConfig& config)
{
  memcpy(&serial_number[0],&(config.serial),sizeof(serial_number));
  memcpy(&macaddr[0],&(config.mac),sizeof(macaddr));
  memcpy(&board_type,&(config.type_id),sizeof(board_type));
}
//namespace IPS {

//SerialNumber toSerialNumber(const char *sn)
//{
//    SerialNumber serial;
//    memcpy(&serial,sn,sizeof(serial.c));
//    return serial;
//}

//void toSerialNumber(char *dest, const SerialNumber &src)
//{
//    std::memcpy(dest,&src.c,sizeof (src.c));
//}

//MAC toMAC(const uint8_t *src)
//{
//    MAC mac;
//    memset(&mac,0,sizeof(mac));
//    memcpy(&mac,src,sizeof(uint8_t[6]));
//    return mac;
//}

//void toMAC(uint8_t *dest, const MAC &src)
//{
//    std::memcpy(dest,&src.b,sizeof (src.b));
//}

//IP toIPaddr(uint8_t a, uint8_t b, uint8_t c, uint8_t d)
//{
//    IP ret_ip={{a,b,c,d}};
//    return ret_ip;
//}

//void toIPaddr(uint8_t *dest, uint8_t a, uint8_t b, uint8_t c, uint8_t d)
//{
//    dest[0]=a;
//    dest[1]=b;
//    dest[2]=c;
//    dest[3]=d;
//}

//void toIPaddr(uint8_t *dest, IP src)
//{
//    std::memcpy(dest,&src.b,sizeof (src.b));
//}

//}
