/*
 * config_types.h - a part of ufw++ object envirinment for
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

#ifndef UFW_LIBS_SYSTEM_CONFIGS_CONFIGPLAN_CONFIG_TYPES_H_
#define UFW_LIBS_SYSTEM_CONFIGS_CONFIGPLAN_CONFIG_TYPES_H_

#include <cstdint>
#include <cstddef>
#include <cstdlib>

#define IPS_DEVICE_SERIAL_SIZE 12

//namespace ufw{
//enum VoiceControl{
//  PA=uint32_t(1),
//  DX=uint32_t(2),
//  GR=uint32_t(3)
//};
//
//enum ControlLines{
//  ON=uint32_t(11),
//  OFF=uint32_t(12),
//  TOG=uint32_t(13)
//};
//enum Sys{
//  VOLUP=uint32_t(21),
//  VOLDOWN=uint32_t(22),
//  Mute=uint32_t(23)
//};
//enum Responce{
//  ACK=uint32_t(256),
//  Cancel=uint32_t(257),
//  Busy=uint32_t(258)
//};
//
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
//}

namespace ufw{
  enum CfgOffset
  {
    FactorySettings=uint32_t(0x00),
    FactoryHash=uint32_t(0x20),
    IpConfig=uint32_t(0x40),
    IpConfigHash=uint32_t(0x60),
    Name=uint32_t(0x80),
    NameHash=uint32_t(0xA0),
    YellKey=uint32_t(0xC0),
    ConfigAdresses=uint32_t(0x100)
  };
}

typedef struct {
    uint32_t CID;                                   // Caller id of station
    uint32_t IPaddr;                                // IP address of station in "internet format" type QHostAddress
}stationIP;

typedef struct {
    uint16_t  number;                               // Key's number
    uint16_t  function;                             // Function's number
    uint16_t  destination;                          // Destination cid
    uint16_t  priority;                             // Priority
    uint16_t  pretone;                              // Pretone length
    uint16_t  pretone_number;                       // Pretone's number in Pretones table
} KeyPad;

//typedef struct {
//    uint16_t    size_iptable;                       // number of records
//    stationIP   iptable[size];                      // array of structures stationIP
//    uint16_t    size_keyboard;                      // number of keys
//    Key         keyboard[size];                     // array of structures Key
//}routingTable;

typedef struct {
    uint16_t key;                                   // Key number
    uint16_t count_lines;                           // Count of activate Contro lLines
    uint32_t CID;                                   // Caller id of station
    uint16_t destination;                           // Destination number
}activateControlLine;

//typedef struct {
//    uint16_t             size_lines;                // count of key with lines
//    activateControlLine  lines[size];               // array of activateControlLine
//} activateControlLinesTable;


#endif /* UFW_LIBS_SYSTEM_CONFIGS_CONFIGPLAN_CONFIG_TYPES_H_ */
