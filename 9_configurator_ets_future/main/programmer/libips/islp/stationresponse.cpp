#include "stationresponse.hpp"
#include "../tools/nswfl_crc32.hpp"
#include "../ips_helpers.hpp"
#include "../debug_macro.h"
#include "details.hpp"
#include <cstring>

namespace ISLP {

stationResponse::stationResponse(uint8_t *data, size_t size) :
      basicMessage (data, size)
{
}

stationResponse::stationResponse(intercomStation &station, arcCrypt *key,bool is_accept) :
    basicMessage()
  {
    message.clear();
    if (key==nullptr)
      {
    return; //TODO: Add exceptioning
      }
    keyring=key;
    serializeStation(station,is_accept);
  }

stationResponse::stationResponse(basicMessage &&other) :
    basicMessage (std::move (other))
{
}

stationResponse::stationResponse(const basicMessage &other) :
basicMessage (other)
{
}

stationResponse& stationResponse::operator=(const basicMessage &other)
{
dynamic_cast<basicMessage&> (*this) = other;
return *this;
}

stationResponse& stationResponse::operator=(basicMessage &&other)
{
dynamic_cast<basicMessage&> (*this) = std::move (other);
return *this;
}

stationResponse& stationResponse::operator<<(basicMessage &&other)
{
dynamic_cast<basicMessage&> (*this) = std::move (other);
return *this;
}

intercomStation* stationResponse::operator ()(arcCrypt *key)
{
if(validate (key))
{
intercomStation *valid_station = new intercomStation (message.data () + Offset::Data);
valid_station->isvalid = isvalid;
std::memcpy (&valid_station->key, key->takeKey (), 4);
return valid_station;
}
return nullptr;
}

bool stationResponse::isAccept()
{
if(Helpers::getU32from<uint8_t> (message.data (), Offset::Function) != ISLP::Functions::Accept)
return false;
return true;
}

bool stationResponse::validate(arcCrypt *key)
{
isvalid = isValid (key);
return isvalid;
}

bool stationResponse::isValid(arcCrypt *key)
{
if(!isvalid)
{
if(!key)
{
return false;
}
if(message.size () != Size::Response)
{
return false;
}
if(Helpers::getU32from<uint8_t> (message.data (), Offset::Magic) != magicnumber_locator)
{
return false;
}
if((Helpers::getU32from<uint8_t> (message.data (), Offset::Function) != ISLP::Functions::StationInfo)
&& (Helpers::getU32from<uint8_t> (message.data (), Offset::Function) != ISLP::Functions::Accept))
{
return false;
}

NSWFL::Hashing::CRC32 worker;
worker.Initialize ();

key->decrypt (message.data () + Offset::Data, Size::Station + Size::CRC);
if(Helpers::getU32from<uint8_t> (message.data (), Offset::CRC)
!= worker.FullCRC (message.data (), Size::Header + Size::Station))
{
key->encrypt (message.data () + Offset::Data, Size::Station + Size::CRC);
return false;
}
}
return true;
}
void stationResponse::serializeStation(intercomStation &station, bool is_accept)
  {
    NSWFL::Hashing::CRC32 worker;
    worker.Initialize();
    Helpers::appendToByteVectorByVal<uint32_t>(&message,magicnumber_locator);
    Helpers::appendToByteVectorByVal<uint32_t>(&message,(is_accept)?(ISLP::Functions::Accept):(ISLP::Functions::StationInfo));
    station.serialize(&message);
    uint32_t crc=worker.FullCRC(data(),size());
    Helpers::appendToByteVectorByVal<uint32_t>(&message,crc);
    keyring->encrypt(data()+Offset::Data,size()-Size::Header);
  }
}
