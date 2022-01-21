#include "stationresponse.hpp"

#include <common/ips_helpers.hpp>
#include <devices/crypto/hwCRC.h>

#include <cstring>

#include <protocols/islp_details.hpp>

namespace ISLP
{

  stationResponse::stationResponse(uint8_t *data, size_t size) :
	  basicMessage (data, size)
	  {
	  }

  stationResponse::stationResponse(const intercomStation &station, ufw::arcCrypt *key,bool is_accept) :
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

  stationResponse::stationResponse(const ByteArray &other):
    basicMessage(other)
  {
  }

  stationResponse::stationResponse(ByteArray &&other):
    basicMessage(std::move(other))
  {
  }

  stationResponse::stationResponse(basicMessage &&other) :
	basicMessage (std::move (other))
  {

  }

  stationResponse::stationResponse(const basicMessage &other) :
	basicMessage (other)
  {

  }

  stationResponse::~stationResponse()
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

  stationResponse &stationResponse::operator=(const intercomStation &other)
  {
    message.clear();
    if (keyring==nullptr)
      {
	return *this; //TODO: Add exceptioning
      }
    serializeStation(other,false);
    return *this;
  }

  stationResponse& stationResponse::operator<<(basicMessage &&other)
  {
    dynamic_cast<basicMessage&> (*this) = std::move (other);
    return *this;
  }

  intercomStation* stationResponse::operator ()(ufw::arcCrypt *key)
  {
    if(validate (key))
      {
	intercomStation *valid_station = new intercomStation (message.data () + Offset::Data);
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

  bool stationResponse::validate(ufw::arcCrypt *key)
  {

    isvalid = isValid (key);
    return isvalid;
  }

  void stationResponse::serializeStation(const intercomStation &station, bool is_accept)
  {
    ufw::hwCRC& worker=ufw::hwCRC::Instance();
    Helpers::appendToByteVectorByVal<uint32_t>(&message,magicnumber_locator);
    Helpers::appendToByteVectorByVal<uint32_t>(&message,(is_accept)?(ISLP::Functions::Accept):(ISLP::Functions::StationInfo));
    station.serialize(&message);
    uint32_t crc=worker.Calculate(data(),size());
    Helpers::appendToByteVectorByVal<uint32_t>(&message,crc);
    keyring->encrypt(data()+ISLP::Offset::Data,size()-ISLP::FieldSize::Header);
  }
#ifdef IPS_TESTING
  std::vector<uint8_t> stationResponse::serialize(intercomStation station, bool is_valid)
  {
    std::vector<uint8_t> output;
    NSWFL::Hashing::CRC32 worker;
    worker.Initialize();
    Helpers::appendToByteVectorByVal<uint32_t>(&output,magicnumber_locator);
    Helpers::appendToByteVectorByVal<uint32_t>(&output,ISLP::Functions::StationInfo);
    station.serialize(&output);
    uint32_t crc=worker.FullCRC(output.data(),output.size());
    if(!is_valid){
	crc++;
    }
    Helpers::appendToByteVectorByVal<uint32_t>(&output,crc);
    arcCrypt encoder(&station.key,sizeof (station.key));
    encoder.encrypt(output.data()+Offset::Data,output.size()-Size::Header);
    return output;

  }
#endif

  bool stationResponse::isValid(ufw::arcCrypt *key)
  {
    if(!isvalid)
      {
	if(!key)
	  {
	    return false;
	  }
	if(message.size () != ISLP::FieldSize::Response)
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

	ufw::hwCRC& worker=ufw::hwCRC::Instance();

	key->decrypt (message.data () + Offset::Data, FieldSize::Station + FieldSize::Crc);
	if(Helpers::getU32from<uint8_t> (message.data (), Offset::Crc)
	    != worker.Calculate(message.data (), FieldSize::Header + FieldSize::Station))
	  {
	    key->encrypt (message.data () + Offset::Data, FieldSize::Station + FieldSize::Crc);
	    return false;
	  }
      }
    return true;
  }

}
