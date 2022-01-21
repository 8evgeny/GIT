/*
 *   Copyright (c) 2019-2020 by Dmitry Donskih
 *   All rights reserved.
 *
 *   Attention Software modification allowed only with
 *   the permission of the author.
 *
 */
#pragma once

#include <cstdint>
#include <cstddef>
#include <cstdlib>

namespace ufw
{
  namespace IPSDP
  {
    static const uint32_t magic = 0xDEB01234;
    enum Functions
    {
      Heartbeat = uint32_t (1),
      SetIP = uint32_t (2),
      SetVol = uint32_t (3),
      Mute = uint32_t (4),
      Reboot = uint32_t (5),
      GetInfo = uint32_t (6),
      SetCodec = uint32_t (7),

      GetStations = uint32_t (10),
      GetKeys = uint32_t (11),
      GetGroups = uint32_t (12),
      SetConfig = uint32_t (13),

      EraseEEPROM = uint32_t (20),
      Output = uint32_t (100)
    };
  }
}
