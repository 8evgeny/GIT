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

#include <map>

using StationIP=std::map<uint32_t,uint32_t>;

/**
 * @brief cid2peer - gives station IP by it's CID from CID list
 * @param cid
 * @param ip_list
 * @return
 */
inline uint32_t cid2peer (uint32_t cid,StationIP& ip_list)
{
  return ip_list.at(cid);
}

/**
 * @brief peer2cid - gives station CID by it's IP from CID list
 * @param peer
 * @param ip_list
 * @return
 */
inline uint32_t peer2cid (uint32_t peer,StationIP& ip_list)
{
  for(auto [x,y]: ip_list)
        {
          if(y==peer)
            {
              return x;
            }
        }
  return -1;
}



namespace ufw{
  /**
 * @brief The VoiceControl enum - IP voice communications commands
 */
enum VoiceControl{
  PA=uint32_t(0x00001001),
  DX=uint32_t(0x00001002),
  GR=uint32_t(0x00002003)
};

/**
 * @brief The ControlLines enum - Control lines control
 */
enum ControlLines{
  ON=uint32_t(0x00000101),
  OFF=uint32_t(0x00000102),
  TOG=uint32_t(0x00000103)
};

/**
 * @brief The Sys enum
 */
enum Sys{
  VOLUP=uint32_t(0x00010001),
  VOLDOWN=uint32_t(0x00010002),
  Mute=uint32_t(0x00010003),
  REBOOT=uint32_t(0x0001000F)
};
enum Responce{
  ACK=uint32_t(0x01000001),
  Cancel=uint32_t(0x01000002),
  Busy=uint32_t(0x01000003)
};

//enum CfgOffset
//{
//  FactorySettings=uint32_t(0x00),
//  FactoryHash=uint32_t(0x20),
//  IpConfig=uint32_t(0x40),
//  IpConfigHash=uint32_t(0x60),
//  Name=uint32_t(0x80),
//  NameHash=uint32_t(0xA0),
//  YellKey=uint32_t(0xC0),
//  ConfigAdresses=uint32_t(0x100)
//
//};

};


