#ifndef DETAILS_HPP
#define DETAILS_HPP

#include <cstdlib>
#include <cstdint>

//TODO: make header with all ISLP functions
#include "basicmessage.hpp"
#include "yellmsg.hpp"
#include "setstationmessage.hpp"
#include "stationresponse.hpp"
#include "intercomstation.hpp"
#include "arcstylecrypt.hpp"

#define ISLP_YELL_MINPAYLOAD (0x20)
#define ISLP_YELL_MAXPAYLOAD (0xEE)
#define ISLP_YELL_HEADERSIZE (0x11)
#define ISLP_YELL_MAXSIZE    (0xFF)
#define ISLP_YELL_MINSIZE    (ISLP_YELL_MINPAYLOAD+ISLP_YELL_HEADERSIZE)
#define ISLP_YELL_KEYSIZE     4
#define ISLP_MAGIC            0
#define ISLP_FUNCPLACE        4
/*
 *  YELL - an message to station to get its info and set sess
 *  |MAGICNUM|Func ID|Key OFFSET|RC(MAGICNUM)OFFSET|  PAYLOAD  | CRC32 |
 *  0  4b    3  4b   7    4b    11     1byte      12  32-238b  x  4b  x+4
 */

/**
 * @brief ISLP - Announce devices to get ready for configuration, validate IPS devices,
 * and sets up network settings.
 */
namespace ISLP {

  /**
   * @brief 0x5e7470cd Protocol magic number.
   */
  static const unsigned int magicnumber_locator = 0x5e7470cd;

  namespace Size {
    enum
      {
      Magic = 4,
      Function = 4,
      Serial = 12,
      MAC = 6,
      Type = 2,
      IP = 4,
      Name = 12,
      CRC = 4,
      Header = Magic + Function,
      Station = Serial + MAC + Type + (IP * 3) + Name,
      Response = Header + Station + CRC
      };
  }
  namespace Offset {
    enum
      {
      Magic = 0,
      Function = 4,
      Serial = 8,
      MAC = Serial + Size::Serial,
      Type = MAC + Size::MAC,
      IP = Type + Size::Type,
      NetMask = IP + Size::IP,
      Gateway = NetMask + Size::IP,
      Name = Gateway + Size::IP,
      CRC = Name + Size::Name,
      Data = 0 + Size::Header
      };
  }
  enum State
    {
    Error = -1,
    None = 0,
    SelfChanged = 1,
    KeyChanged = 2

    };

  /**
   * @brief Enum describe function codes of protocol.
   */
  enum Functions
    {
    Yell = 0x0101FF01, //!< @ref yellMsg "Yell()" - announce devices to get ready for config, sets up protocol validation key, and request to send their settings.
    SetIP = 0x0101FF02, //!< @ref setStationMessage "SetIP(intercomStation, Key)" - sets new IP/Mask/Gateway to specified station(by Serial number intercomStation::serial_number).
    StationInfo = 0x0F0FFF01, //!< @ref stationResponse "StationInfo(intercomStation)" - station sens their info structure masked with Key sent by Yell message(bytewise XOR operation of payload and crc32 section).
    Accept = 0x0F0F0001 //!< @ref stationResponse "Accept(intercomStation)" - responce by station on SetIP Request. Returns new info structure(struct intercomStation).
    };

}

#endif // DETAILS_HPP
