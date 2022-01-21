/*
 * islp_details.h - a part of ufw++ object envirinment for
 * microcontrollers.
 *
 *   Created on: 21 дек. 2020 г. 2020 г.
 *       Author: Dmitry Donskih
 *
 * Copyright (c) 2019-2020 by Dmitry Donskih
 * All rights reserved.
 * 
 * Attention Software modification allowed only with
 * the permission of the author.
 * 
 */
#pragma once

//#include <cstdlib>
#include <cstdint>

//TODO: make header with all ISLP functions
#include "basic_message.h"
#include "yellmsg.hpp"
#include "setstationmessage.hpp"
#include "stationresponse.hpp"

//#include "intercomstation.hpp"
#include <system/configs/device_config.h>
//#include "arcstylecrypt.hpp"
#include <devices/crypto/arcstylecrypt.hpp>

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
namespace ISLP
{

  /**
   * @brief 0x5e7470cd Protocol magic number.
   */
  static const unsigned int magicnumber_locator = 0x5e7470cd;

  namespace FieldSize
  {
    enum
    {
      Magic = (4),
      Function = (4),
      Serial = (12),
      MAC = (6),
      Type = (2),
      IP = (4),
      Name = (12),
      Crc = (4),
      Header = ((int) Magic + (int) Function),
      Station = ((int) Serial + (int) MAC + (int) Type + ((int) IP * 3) + (int) Name),
      Response = ((int) Header + (int) Station + (int) Crc)
    };
  }
  namespace Offset
  {
    enum
    {
      Magic = 0,
      Function = 4,
      Serial = 8,
      MAC = ((int) Serial + (int) ISLP::FieldSize::Serial),
      Type = ((int) Offset::MAC + (int) ISLP::FieldSize::MAC),
      IP = ((int) Offset::Type + (int) ISLP::FieldSize::Type),
      NetMask = ((int) Offset::IP + (int) ISLP::FieldSize::IP),
      Gateway = ((int) Offset::NetMask + (int) ISLP::FieldSize::IP),
      Name = ((int) Offset::Gateway + (int) ISLP::FieldSize::IP),
      Crc = ((int) Name + (int) ISLP::FieldSize::Name),
      Data = ((int) FieldSize::Header),
    };
  }
  enum State : int
  {
    Error = -1,
    None = 0,
    SelfChanged = 1,
    KeyChanged = 2
  };

  /**
   * @brief Enum describe function codes of protocol.
   */
  enum Functions : uint32_t
  {
    Yell = 0x0101FF01, //!< @ref yellMsg "Yell()" - announce devices to get ready for config, sets up protocol validation key, and request to send their settings.
    SetIP = 0x0101FF02, //!< @ref setStationMessage "SetIP(intercomStation, Key)" - sets new IP/Mask/Gateway to specified station(by Serial number intercomStation::serial_number).
    StationInfo = 0x0F0FFF01, //!< @ref stationResponse "StationInfo(intercomStation)" - station sens their info structure masked with Key sent by Yell message(bytewise XOR operation of payload and crc32 section).
    Accept = 0x0F0F0001 //!< @ref stationResponse "Accept(intercomStation)" - responce by station on SetIP Request. Returns new info structure(struct intercomStation).
  };

}

