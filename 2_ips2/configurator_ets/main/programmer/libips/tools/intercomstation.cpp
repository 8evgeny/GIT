#include "intercomstation.hpp"
#include "../../libips/ips_helpers.hpp"
#include "../../libips/debug_macro.h"
#include "../../libips/names/ipsnames.hpp"

#include <cstring>

intercomStation::intercomStation ()
{
  isvalid = false;
  key[0] = 0;
  key[1] = 0;
  key[2] = 0;
  key[3] = 0;
}

intercomStation::intercomStation (uint8_t *input)
{
  setupData (input);
}

void intercomStation::setupData (uint8_t *input)
{
  uint32_t offset = 0;
  memcpy (&serial_number, input + offset, sizeof(serial_number));
  offset += static_cast<uint32_t>(sizeof(serial_number));
  memcpy (&macaddr, input + offset, sizeof(macaddr));
  offset += static_cast<uint32_t>(sizeof(macaddr));
  memcpy (&board_type, input + offset, sizeof(board_type));
  offset += static_cast<uint32_t>(sizeof(board_type));
  memcpy (&ipaddr, input + offset, sizeof(ipaddr));
  offset += static_cast<uint32_t>(sizeof(ipaddr));
  memcpy (&netmask, input + offset, sizeof(netmask));
  offset += static_cast<uint32_t>(sizeof(netmask));
  memcpy (&gateway, input + offset, sizeof(gateway));
  offset += static_cast<uint32_t>(sizeof(gateway));
  memcpy (&name, input + offset, sizeof(name));
}

void intercomStation::copy(const intercomStation &other)
{
  memcpy (&this->ipaddr, &(other.ipaddr), sizeof(intercomStation::ipaddr));
  memcpy (&this->netmask, &other.netmask, sizeof(intercomStation::netmask));
  memcpy (&this->gateway, &other.gateway, sizeof(intercomStation::gateway));
  memcpy (&this->name, &other.name, sizeof(intercomStation::name));
}

void intercomStation::setName (const char *newname)
{
  if (strlen (newname) >= sizeof(name))
    {
      memcpy (&name, newname, sizeof(name));
    } else
    {
      memcpy (&name, newname, strlen (newname));
    }
}

std::vector<uint8_t> intercomStation::serialize ()
{
  std::vector < uint8_t > out;
  serialize(&out);
  return out;
}

void intercomStation::serialize(std::vector<uint8_t> *output)
{
  for (char x : serial_number)
    {
      output->push_back (static_cast<uint8_t>(x));
    }
  for (uint8_t x : macaddr)
    {
      output->push_back (x);
    }
  Helpers::appendToByteVectorByVal<uint16_t> (output, board_type);
  for (uint8_t x : ipaddr)
    {
      output->push_back (x);
    }
  for (uint8_t x : netmask)
    {
      output->push_back (x);
    }
  for (uint8_t x : gateway)
    {
      output->push_back (x);
    }
  for (char x : name)
    {
      output->push_back (static_cast<uint8_t>(x));
    }
}
bool operator==(const intercomStation &lv, const intercomStation &rv)
{
  for(size_t i = 0;i < sizeof(intercomStation::serial_number);i++)
    {
      if(lv.serial_number[i] != rv.serial_number[i]) return false;
    }
  return true;
}

bool operator!=(const intercomStation &lv, const intercomStation &rv)
{
  for(size_t i = 0;i < sizeof(intercomStation::serial_number);i++)
    {
      if(lv.serial_number[i] != rv.serial_number[i]) return true;
    }
  return false;
}
