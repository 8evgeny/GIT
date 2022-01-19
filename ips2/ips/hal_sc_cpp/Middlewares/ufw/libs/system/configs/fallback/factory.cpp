/*
 * factory.cpp - a part of ufw++ object envirinment for
 * microcontrollers.
 *
 *   Created on: 26 янв. 2021 г.
 *       Author: Dmitry Donskih
 *
 * Copyright (c) 2019-2020 by Dmitry Donskih
 * All rights reserved.
 * 
 * Attention Software modification allowed only with
 * the permission of the author.
 * 
 */

#include <cstdint>
#include <cstddef>
#include <cstdlib>
#include <types/array_type.h>
#include <devices/flash/flashI2C.h>
#include <devices/crypto/hw_hash.h>
#include <system/configs/config_types.h>
#include <containers/ufw_containers.h>
#include "fallbacks.h"

static ufw::flashI2C &flasher = ufw::flashI2C::Instance ();
static ufw::hwHash &hasher = ufw::hwHash::Instance ();

namespace fbk
{
  using mac_adress = std::array<uint8_t,6>;
  const char *m_serialnumbers[10]
    {
    "002022033001",
    "001012033001",
    "001012033002",
    "001012033003",
    "001012033004",
    "001012033005",
    "001012033006",
    "001012033007",
    "001012033008",
    "001012033009"
    };
  mac_adress m_macaddresses[10]
    {
      { 0x40, 0xD8, 0x55, 0x11, 0x90, 0x2E },
      { 0x40, 0xD8, 0x55, 0x11, 0x90, 0x2F },
      { 0x40, 0xD8, 0x55, 0x11, 0x90, 0x30 },
      { 0x40, 0xD8, 0x55, 0x11, 0x90, 0x31 },
      { 0x40, 0xD8, 0x55, 0x11, 0x90, 0x32 },
      { 0x40, 0xD8, 0x55, 0x11, 0x90, 0x33 },
      { 0x40, 0xD8, 0x55, 0x11, 0x90, 0x34 },
      { 0x40, 0xD8, 0x55, 0x11, 0x90, 0x35 },
      { 0x40, 0xD8, 0x55, 0x11, 0x90, 0x36 },
      { 0x40, 0xD8, 0x55, 0x11, 0x90, 0x37 }
    };
  extern const uint8_t s_uinit_pattern[10] =
    { 108, 1, 75, 81, 33, 120, 74, 45, 85, 56 };
  int fallback_type = 1;
  void FlashFallbackSys ()
  {

    ByteArray fallback ((uint8_t*) m_serialnumbers[IPS_DEV_N], 12);
    fallback.append (m_macaddresses[IPS_DEV_N].data (), sizeof(mac_adress));
    fallback.append ((uint8_t*) &fallback_type, sizeof(int));
    md5hash_t hash = hasher.compute (fallback.data (), fallback.size ());
    flasher.writeSystem (fallback.data (), fallback.size (), ufw::CfgOffset::FactorySettings);
    flasher.writeSystem (hash.data (), hash.size (), ufw::CfgOffset::FactoryHash);

  }
//  void FlashFallbackIPs()
//  {
//    uint32_t ip
//  }

}
