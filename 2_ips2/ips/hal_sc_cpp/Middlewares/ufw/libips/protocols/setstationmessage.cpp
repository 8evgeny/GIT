#include "setstationmessage.hpp"
#include <common/ips_helpers.hpp>
#include <devices/crypto/hwCRC.h>
#include <protocols/islp_details.hpp>

#include <cstring>

namespace ISLP {

  setStationMessage::setStationMessage( intercomStation &station) :
      basicMessage (), m_station (station)
  {
    Helpers::appendToByteVectorByVal<uint32_t> (&(this->message), magicnumber_locator);
    Helpers::appendToByteVectorByVal<uint32_t> (&(this->message), ISLP::Functions::SetIP);
  }

  uint8_t* setStationMessage::operator()()
  {
//    ufw::hwCRC& worker=ufw::hwCRC::Instance();
//    ufw::arcCrypt rc_worker (&(m_station.key), sizeof(m_station.key));
//
//
//    m_station.serialize (&(this->message));
//    Helpers::appendToByteVectorByVal<uint32_t> (&(this->message),
//						worker.Calculate (message.data (), message.size ()));
//    rc_worker.encrypt (message.data () + Offset::Data, message.size () - Size::Header);
//    return message.data ();
    return nullptr;
  }

  setStationMessage::~setStationMessage()
  {

  }

  int getStationFromMsg(intercomStation &station, setStationMessage &t_msg, ufw::arcCrypt *decoder)
  {
    uint8_t *msg_data = t_msg();
    if(t_msg.size () != ISLP::FieldSize::Response)
      {
	return -1;
      }
    if(Helpers::getU32from<uint8_t> (msg_data, Offset::Magic) != magicnumber_locator)
      {
	return -1;
      }
    if(Helpers::getU32from<uint8_t> (msg_data, Offset::Function) != ISLP::Functions::SetIP)
      {
	return -1;
      }

    ufw::hwCRC& worker=ufw::hwCRC::Instance();

    decoder->decrypt (msg_data + ISLP::Offset::Data, ISLP::FieldSize::Station + ISLP::FieldSize::Crc);
    if(Helpers::getU32from<uint8_t> (msg_data, Offset::Crc)
	!= worker.Calculate (msg_data, FieldSize::Header + FieldSize::Station))
      {
	return -1;
      }
    station.setupData (msg_data + Offset::Data);
    return 0;
  }

#ifdef IPS_TESTING
void setStationMessage::modMAC(const IPS::MAC &mac_addr)
{
    IPS::toMAC(&m_station.macaddr[0],mac_addr);
}

void setStationMessage::modSerial(const IPS::SerialNumber &ser_num)
{
    IPS::toSerialNumber(&m_station.serial_number[0],ser_num);
}

void setStationMessage::modType(uint16_t st_type)
{
    m_station.board_type=st_type;
}

uint8_t *setStationMessage::getKey()
{
    return &m_station.key[0];
}

intercomStation setStationMessage::validate(setStationMessage* t_msg)
{
    if(!t_msg)
    {
        IPS_DEBUG_OUTPUT_FCN<<"Empty input\n";
        return intercomStation();
    }
    uint8_t *msg_data=(*t_msg)();
    if(t_msg->size()!=Size::Response)
    {
        IPS_DEBUG_OUTPUT_FCN<<"Wrong size\n";
        return intercomStation();
    }
    if(Helpers::getU32from<uint8_t>(msg_data,Offset::Magic)!=magicnumber_locator){
        IPS_DEBUG_OUTPUT_FCN<<"Wrong magic number\n";
        return intercomStation();
    }
    if(Helpers::getU32from<uint8_t>(msg_data,Offset::Function)!=ISLP::Functions::SetIP)
    {
        IPS_DEBUG_OUTPUT_FCN<<"Wrong function\n";
        return intercomStation();
    }

    NSWFL::Hashing::CRC32 worker;
    worker.Initialize();

    arcCrypt decoder(t_msg->getKey(),4);


    decoder.decrypt(msg_data+Offset::Data,Size::Station+Size::CRC);
    if(Helpers::getU32from<uint8_t>(msg_data,Offset::CRC)!=worker.FullCRC(msg_data,Size::Header+Size::Station)){
        IPS_DEBUG_OUTPUT_FCN<<"Wrong CRC32\n";
        //decoder.encrypt(msg_data+Offset::Data,Size::Station+Size::CRC);
        return intercomStation(msg_data+Offset::Data);
    }
    return intercomStation(msg_data+Offset::Data);

}
#endif

}
