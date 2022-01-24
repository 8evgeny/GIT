/*
 *   Copyright (c) 2019-2020 by Dmitry Donskih
 *   All rights reserved.
 *
 *   Attention Software modification allowed only with
 *   the permission of the author.
 *
 */
#include "ctrlmessage.h"
#include <cstring>

#define UFW_CC_VOICECON_MASK 0x0000F000
#define UFW_CC_VOICECONP2P_MASK 0x00001000

#define UFW_CC_LINECTL_MASK 0x00000100
#define UFW_CC_SYSCTL_MASK 0x00010000
#define UFW_CC_RESPONCE_MASK 0x00010000

namespace ufw{

void CtlMessage::Serialize(uint8_t *message)
{
  int *imsg=reinterpret_cast<int*>(message);
  memcpy(imsg++,&id,sizeof(int));
  memcpy(imsg++,&function,sizeof(int));
  memcpy(imsg++,&destination,sizeof(int));
  memcpy(imsg++,&prio,sizeof(int));
  memcpy(imsg++,&parameters,sizeof(int));
  memcpy(imsg++,&source,sizeof(int));
}

void CtlMessage::Serialize(ByteArray &message)
{
  message.clear();
  message.resize(cm_size());
  Serialize(message.data());
}

CtlMessage& CtlMessage::Deserialize(uint8_t *message)
{
  int *imsg=reinterpret_cast<int*>(message);
  memcpy(&id,imsg++,sizeof (int));
  memcpy(&function,imsg++,sizeof (int));
  memcpy(&destination,imsg++,sizeof (int));
  memcpy(&prio,imsg++,sizeof (int));
  memcpy(&parameters,imsg++,sizeof (int));
  memcpy(&source,imsg++,sizeof (int));
  return *this;
}

bool CtlMessage::isP2PVoice()
{
  return (function&UFW_CC_VOICECONP2P_MASK)>0;
}

bool CtlMessage::isVoice()
{
  return (function&UFW_CC_VOICECON_MASK)>0;
}

bool CtlMessage::isLineControl()
{
  return (function&UFW_CC_LINECTL_MASK)>0;
}

bool CtlMessage::isResponse()
{
  return (function&UFW_CC_RESPONCE_MASK)>0;
}

bool CtlMessage::isSysControl()
{
  return (function&UFW_CC_SYSCTL_MASK)>0;
}

int CtlMessage::Deserialize(ByteArray &message)
{
  if(message.size()<cm_size())return -1;
  Deserialize(message.data());
  return cm_size();
}

}

uint8_t getAnswerKey(ufw::CtlMessage &msg, Keymap &keys)
{
  uint8_t fallback=0xFF;
  for(auto [x,y]: keys)
    {
      if(y.isP2PVoice())
        {
          if(y.destination==msg.source)
            {
              if(y.function==msg.function)return x; // if function and dest matches return item
              fallback=(fallback==0xFF)?x:fallback; // get first item matched by Caller-Callee pair as fallback
            }
        }
    }
  return fallback; //return first match by CID or 0xFF
}

void SerializeKeymap(Keymap &keys, uint8_t *dest)
{
  uint32_t sz=keys.size();
  memcpy(dest,&sz,sizeof (uint32_t));
  dest+=sizeof (uint32_t);
  for(auto [x,y]:keys)
    {
      *dest=x;
      y.Serialize(++dest);
      dest+=ufw::CtlMessage::cm_size();
    }

}

void DeserializeKeymap(uint8_t *src, Keymap &keys)
{
  keys.clear();
  uint32_t size_map=*(uint32_t*)src;
  src+=sizeof (uint32_t);
  uint8_t key;
  ufw::CtlMessage value;
  for(uint32_t i=0;i<size_map;i++)
    {
      key=*src++;
      value.Deserialize(src);
      src+=ufw::CtlMessage::cm_size();
      keys.insert({key,value});
    }

}

bool operator == (const ufw::CtlMessage &lv, const ufw::CtlMessage &rv)
{
  if((lv.id==rv.id)&&(lv.function==rv.function)&&(lv.destination==rv.destination))
    {
    return true;
    }
  return false;
}

bool operator != (const ufw::CtlMessage &lv, const ufw::CtlMessage &rv)
{
  return !(lv==rv);
}
