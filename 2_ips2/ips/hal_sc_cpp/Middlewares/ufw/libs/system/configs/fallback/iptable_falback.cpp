/*
 * iptable_falback.cpp - a part of ufw++ object envirinment for
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
 *
Пульт ПДО-16 172.17.10.70 CID1000 (0xac110a46)

Пульт ПДО-32 172.17.10.71 CID1001 (0xac110a47)
Пульт ПДО-16 172.17.10.72 CID1002 (0xac110a48)
Пульт ПДО-16 172.17.10.73 CID1003 (0xac110a49)

Устройство УПВ-2 172.17.10.75 CID1010  (0xac110a4b)
Плата СЛ1   172.17.10.80 CID1020       (0xac110a50)

(0xac110a01) шлюз
(0xffffff00) маска

 *typedef struct {
    uint16_t  number;                               // Key's number
    uint16_t  function;                             // Function's number
    uint16_t  destination;                          // Destination cid
    uint16_t  priority;                             // Priority
    uint16_t  pretone;                              // Pretone length
    uint16_t  pretone_number;                       // Pretone's number in Pretones table
} Key;
 *
 *
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

static ufw::flashI2C& flasher = ufw::flashI2C::Instance();
static ufw::hwHash& hasher = ufw::hwHash::Instance();

namespace fbk{
  uint32_t fallback_ips[] = {
                              1000,0xac110a46,
                              1001,0xac110a47,
                              1002,0xac110a48,
                              1003,0xac110a49,
                              1010,0xac110a4b,
                              1020,0xac110a50
                             };

  uint32_t fallback_groups[]={
      1,3,1010,1002,1003,2,4,1010,1002,1003
  };

//  KeyPad FBSettings[] = {
//      {1,ufw::VoiceControl::GR,1,10,0,0},
//      {2,ufw::VoiceControl::PA,1001,10,0,0},
//      {3,ufw::VoiceControl::PA,1002,10,0,0},
//      {4,ufw::VoiceControl::PA,1003,10,0,0},
//      {5,ufw::VoiceControl::PA,1010,10,0,0},
//      {6,ufw::VoiceControl::DX,1001,10,0,0},
//      {7,ufw::VoiceControl::DX,1002,10,0,0},
//      {9,ufw::VoiceControl::DX,1003,10,0,0},
//      {10,ufw::VoiceControl::DX,1010,10,0,0},
//      {11,ufw::ControlLines::ON,1020,1,0,0},
//      {12,ufw::ControlLines::OFF,1020,1,0,0},
//      {13,ufw::ControlLines::TOG,1020,1,0,0},
//      {14,ufw::Sys::VOLUP,1020,1,0,0},
//      {15,ufw::Sys::VOLDOWN,1020,1,0,0},
//      {16,ufw::Sys::Mute,1020,1,0,0},
//  };




  uint32_t ip_fallback[3]={fallback_ips[IPS_DEV_N*2+1],0xac110a01,0xffffff00};



void FlashFallBackIP()
{
  md5hash_t hash = hasher.compute(&ip_fallback[0], 3*sizeof(uint32_t));
  flasher.writeSystem(&ip_fallback[0], 3*sizeof(uint32_t), ufw::CfgOffset::IpConfig );
  flasher.writeSystem(hash.data(), hash.size(), ufw::CfgOffset::IpConfigHash );
}


}
