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

#include "ctl_types.h"

#ifdef UFW_USE_VECTOR
#include <vector>

using ByteArray=std::vector<uint8_t>;
#else
#include <containers/ufw_containers.h>
#endif

#include <map>

/**
 *
 */
namespace ufw
{
  /**
   *
   */
  struct CtlMessage
  {
      uint32_t id;
      uint32_t function;
      uint32_t destination;
      uint32_t prio;
      uint32_t parameters;
      uint32_t source;

      void Serialize (uint8_t *message);
      void Serialize (ByteArray &message);
      int Deserialize (ByteArray &message);
      CtlMessage& Deserialize (uint8_t *message);

      bool isP2PVoice ();
      bool isVoice ();
      bool isLineControl ();
      bool isResponse ();
      bool isSysControl ();
      inline bool isOurMessage(uint32_t self_cid)
      {
	return (destination==self_cid);
      }
      inline bool isICaller(uint32_t self_cid)
      {
	return (destination!=self_cid);
      }
      inline bool isCallee(uint32_t self_cid)
      {
	return (destination==self_cid);
      }
      constexpr static inline size_t cm_size ()
      {
	return sizeof(id) + sizeof(function) + sizeof(destination) + sizeof(prio)
	    + sizeof(parameters) + sizeof(source);
      }
  };
}
/**
 *
 */
using Keymap = std::map<uint8_t, ufw::CtlMessage>;

uint8_t getAnswerKey (ufw::CtlMessage &msg, Keymap &keys);
void SerializeKeymap (Keymap &keys, uint8_t *dest);
void DeserializeKeymap (uint8_t *src, Keymap &keys);

bool operator== (const ufw::CtlMessage &lv, const ufw::CtlMessage &rv);
bool operator!= (const ufw::CtlMessage &lv, const ufw::CtlMessage &rv);
