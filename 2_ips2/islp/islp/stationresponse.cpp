#include "stationresponse.hpp"
#include "nswfl_crc32.hpp"
#include "details.hpp"
#include "ips_helpers.hpp"

#include <debug_macro.h>

#include <cstring>

namespace ISLP {

  stationResponse::stationResponse(uint8_t *data, size_t size) :
	  basicMessage (data, size)
	  {
    IPS_FUNCTION_DBG
	  }

  stationResponse::stationResponse(const intercomStation &station, arcCrypt *key,bool is_accept) :
	basicMessage()
  {
    message.clear();
    if (key==nullptr)
      {
	return; //TODO: Add exceptioning
      }
    keyring=key;
    serializeStation(station,isAccept());

  }

  stationResponse::stationResponse(basicMessage &&other) :
	basicMessage (std::move (other))
  {
    IPS_FUNCTION_DBG
  }

  stationResponse::stationResponse(const basicMessage &other) :
	basicMessage (other)
  {
    IPS_FUNCTION_DBG
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

  intercomStation* stationResponse::operator ()(arcCrypt *key)
  {
    IPS_FUNCTION_DBG
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
    IPS_FUNCTION_DBG
    isvalid = isValid (key);
    return isvalid;
  }

  void stationResponse::serializeStation(const intercomStation &station, bool is_accept)
  {
    NSWFL::Hashing::CRC32 worker;
    worker.Initialize();
    Helpers::appendToByteVectorByVal<uint32_t>(message_ptr,magicnumber_locator);
    Helpers::appendToByteVectorByVal<uint32_t>(message_ptr,(is_accept)?(ISLP::Functions::Accept):(ISLP::Functions::StationInfo));
    station.serialize(message_ptr);
    uint32_t crc=worker.FullCRC(data(),size());
    Helpers::appendToByteVectorByVal<uint32_t>(message_ptr,crc);
    keyring->encrypt(data()+Offset::Data,size()-Size::Header);
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

  bool stationResponse::isValid(arcCrypt *key)
  {
    IPS_FUNCTION_DBG
    if(!isvalid)
      {
	if(!key)
	  {
	    IPS_DEBUG_OUTPUT_FCN << "Thre is no keyring\n";
	    return false;
	  }
	if(message.size () != Size::Response)
	  {
	    IPS_DEBUG_OUTPUT_FCN << "Wrong size Size=" << message.size () << "!=" << Size::Response << "\n";
	    return false;
	  }
	if(Helpers::getU32from<uint8_t> (message.data (), Offset::Magic) != magicnumber_locator)
	  {
	    IPS_DEBUG_OUTPUT_FCN << "Wrong magic number\n";
	    return false;
	  }
	if((Helpers::getU32from<uint8_t> (message.data (), Offset::Function) != ISLP::Functions::StationInfo)
	    && (Helpers::getU32from<uint8_t> (message.data (), Offset::Function) != ISLP::Functions::Accept))
	  {
	    IPS_DEBUG_OUTPUT_FCN << "Wrong function\n";
	    return false;
	  }

	NSWFL::Hashing::CRC32 worker;
	worker.Initialize ();

	key->decrypt (message.data () + Offset::Data, Size::Station + Size::CRC);
	if(Helpers::getU32from<uint8_t> (message.data (), Offset::CRC)
	    != worker.FullCRC (message.data (), Size::Header + Size::Station))
	  {
	    key->encrypt (message.data () + Offset::Data, Size::Station + Size::CRC);
	    IPS_DEBUG_OUTPUT_FCN << "Wrong CRC32\n";
	    return false;
	  }
      }
    return true;
  }

}
