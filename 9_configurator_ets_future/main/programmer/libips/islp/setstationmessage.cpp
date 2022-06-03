#include "setstationmessage.hpp"
#include "details.hpp"
#include "../ips_helpers.hpp"
#include "../debug_macro.h"
#include "../tools/nswfl_crc32.hpp"

#include <cstring>

namespace ISLP {

  setStationMessage::setStationMessage(const intercomStation &station) :
      basicMessage (), m_station (station)
  {
    Helpers::appendToByteVectorByVal<uint32_t> (&(this->message), magicnumber_locator);
    Helpers::appendToByteVectorByVal<uint32_t> (&(this->message), ISLP::Functions::SetIP);
  }

  uint8_t* setStationMessage::operator()()
  {
    NSWFL::Hashing::CRC32 worker;
    arcCrypt rc_worker (&(m_station.key), sizeof(m_station.key));

    worker.Initialize ();

    m_station.serialize (&(this->message));
    Helpers::appendToByteVectorByVal<uint32_t> (&(this->message),
						worker.FullCRC (message.data (), message.size ()));
    rc_worker.encrypt (message.data () + Offset::Data, message.size () - Size::Header);
    return message.data ();
  }

int getStationFromMsg(intercomStation &station, setStationMessage &t_msg, arcCrypt *decoder)
  {
    uint8_t *msg_data = t_msg();
    if(t_msg.size () != Size::Response)
      {
    //IPS_DEBUG_OUTPUT_FCN << "Wrong size\n";
    return -1;
      }
    if(Helpers::getU32from<uint8_t> (msg_data, Offset::Magic) != setStationMessage::magicnumber_locator)
      {
    //IPS_DEBUG_OUTPUT_FCN << "Wrong magic number\n";
    return -1;
      }
    if(Helpers::getU32from<uint8_t> (msg_data, Offset::Function) != ISLP::Functions::SetIP)
      {
    //IPS_DEBUG_OUTPUT_FCN << "Wrong function\n";
    return -1;
      }

    NSWFL::Hashing::CRC32 worker;
    worker.Initialize ();

    decoder->decrypt (msg_data + Offset::Data, Size::Station + Size::CRC);
    if(Helpers::getU32from<uint8_t> (msg_data, Offset::CRC)
    != worker.FullCRC (msg_data, Size::Header + Size::Station))
      {
    //IPS_DEBUG_OUTPUT_FCN << "Wrong CRC32\n";
    //decoder.encrypt(msg_data+Offset::Data,Size::Station+Size::CRC);
    return -1;
      }
    station.setupData (msg_data + Offset::Data);
    return 0;
  }
}
