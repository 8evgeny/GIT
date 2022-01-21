/*
 * basic_message.cpp - a part of ufw++ object envirinment for
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
#include <protocols/basic_message.h>
#include <devices/crypto/hwCRC.h>
#include <protocols/islp_details.hpp>


#include <utility>


  
  basicMessage::basicMessage ()
  :basicMessage (0)
  {
  }
  
  basicMessage::basicMessage (size_t size)
  :message(size)
  {
  }
  
  basicMessage::basicMessage (uint8_t *data, size_t size)
  :basicMessage (size)
  {
    std::memcpy (message.data (), data, size * sizeof(uint8_t));
  }
  
  basicMessage::basicMessage (const basicMessage &other)
  :message(other.message),
   keyring(other.keyring),
   isvalid(other.isvalid)
  {
  }
  
  basicMessage::basicMessage (basicMessage &&other) noexcept
  :message(std::move(other.message)),
   keyring(other.keyring),
   isvalid(other.isvalid)
  {
  }
  
  basicMessage::basicMessage (const ByteArray &data)
  :message (data)
  {
  }
  
  basicMessage::basicMessage (ByteArray &&data)
  :message (std::move(data))
  {
  }
  
  basicMessage::~basicMessage ()

  {
  }
  
  basicMessage& basicMessage::operator = (const basicMessage &other)
  {
    if (&other==this)return *this;
        message = other.message;
        keyring = other.keyring;
        isvalid = other.isvalid;
        return *this;
  }
  
  basicMessage& basicMessage::operator = (basicMessage &&other) noexcept
  {
    if (&other==this)return *this;
        message = std::move (other.message);
        keyring = other.keyring;
        isvalid = other.isvalid;
        return *this;
  }
  
  basicMessage& basicMessage::operator = (const ByteArray &other)
  {
    message = other;
    return *this;
  }
  
  basicMessage& basicMessage::operator = (ByteArray &&other)
  {
    message = (std::move (other));
    return *this;
  }
  
  uint8_t basicMessage::operator [] (size_t index)
  {
    return message.operator[] (index);
  }
  
  bool basicMessage::isISLP ()
  {
    if (*get32ptr (0) != ISLP::magicnumber_locator) return false;
    return true;
  }
  
  uint32_t basicMessage::getFunction ()
  {
    return *get32ptr (ISLP::Offset::Function);
  }
  
  void basicMessage::setMessage (uint8_t *data, size_t size)
  {
    message.clear ();
    message.resize (size);
    std::memcpy (message.data (), data, size);
    message.fit ();
  }
  
  size_t basicMessage::size ()
  {
    return message.size ();
  }
  
  uint8_t* basicMessage::data ()
  {
    return message.data ();
  }
  
  bool basicMessage::checkCRC ()
  {
    return checkCRC(*get32ptr(message.size()-sizeof(uint32_t)));
  }
  
  bool basicMessage::checkCRC (uint32_t crc32)
  {
    return (crc32==getCRC());
  }
  
  void* basicMessage::getPayloadPtr ()
  {
    return message.data () + ISLP::Offset::Data;
  }
  
  uint32_t* basicMessage::getChecksumPtr ()
  {
    return get32ptr(message.size()-sizeof(uint32_t));
  }
  
  uint32_t* basicMessage::get32ptr (size_t index)
  {
    return reinterpret_cast<uint32_t*> (message.data () + index);
  }
  
  uint32_t basicMessage::getCRC ()
  {
    ufw::hwCRC& worker=ufw::hwCRC::Instance();
    return worker.Calculate(message.data(),message.size()-sizeof (uint32_t));
  }

