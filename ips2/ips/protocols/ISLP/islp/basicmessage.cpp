#include "basicmessage.hpp"
#include "details.hpp"
#include <debug_macro.h>
#include "nswfl_crc32.hpp"

#include <cstring>

namespace ISLP
  {

  basicMessage::basicMessage () :
	  basicMessage (0)
	  {
    IPS_FUNCTION_DBG
	  }

  basicMessage::basicMessage (size_t size) :
      message(size)
  {
    IPS_FUNCTION_DBG
  }

  basicMessage::basicMessage (uint8_t *data, size_t size) :
    basicMessage (size)
  {
    IPS_FUNCTION_DBG
    std::memcpy (message.data (), data, size * sizeof(uint8_t));
  }

  basicMessage::basicMessage(const basicMessage &other)
  : message(other.message), keyring(other.keyring),
    isvalid(other.isvalid){IPS_FUNCTION_DBG}

  basicMessage::basicMessage(basicMessage && other) noexcept
    : message(std::move(other.message)),
    keyring(other.keyring), isvalid(other.isvalid) {
    IPS_FUNCTION_DBG
  }

  basicMessage::basicMessage (const std::vector<uint8_t> &data) :
        message (data)
  {
    IPS_FUNCTION_DBG
  }

  basicMessage::basicMessage (std::vector<uint8_t> &&data) :
        message (std::move(data))
  {
    IPS_FUNCTION_DBG
  }

  basicMessage::~basicMessage ()
  {
  }

  basicMessage& basicMessage::operator= (const basicMessage &other)
  {
    IPS_FUNCTION_DBG
    if (&other==this)return *this;
    message = other.message;
    keyring = other.keyring;
    isvalid = other.isvalid;
    return *this;
  }

  basicMessage& basicMessage::operator= (basicMessage &&other) noexcept
  {
    IPS_FUNCTION_DBG
    if (&other==this)return *this;
    message = std::move (other.message);
    keyring = other.keyring;
    isvalid = other.isvalid;
    return *this;
  }

  basicMessage& basicMessage::operator= (const std::vector<uint8_t> &other)
  {
    IPS_FUNCTION_DBG
    message = other;
    return *this;
  }

  basicMessage& basicMessage::operator= (std::vector<uint8_t> &&other)
  {
    IPS_FUNCTION_DBG
    message = (std::move (other));
    return *this;
  }

  uint8_t basicMessage::operator[] (size_t index)
  {
    IPS_FUNCTION_DBG
    return message.operator[] (index);
  }

  //uint32_t basicMessage::getUInt(size_t index)
  //{
  //    return *get32ptr(index);
  //}

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
    message.shrink_to_fit ();
  }

  size_t basicMessage::size ()
  {
    IPS_FUNCTION_DBG
    return message.size ();
  }

  uint8_t* basicMessage::data ()
  {
    return message.data ();
  }

  bool basicMessage::checkCRC()
  {
    return checkCRC(*get32ptr(message.size()-sizeof(uint32_t)));
  }

  bool basicMessage::checkCRC(uint32_t crc32)
  {
    return (crc32==getCRC());
  }

  void* basicMessage::getPayloadPtr ()
  {
    return message.data () + ISLP::Offset::Data;
  }

  uint32_t *basicMessage::getChecksumPtr()
  {
    return get32ptr(message.size()-sizeof(uint32_t));
  }

  //uint32_t* basicMessage::getChecksumPtr ()
  //{

  //}

  uint32_t* basicMessage::get32ptr (size_t index)
  {
    return reinterpret_cast<uint32_t*> (message.data () + index);
  }

  uint32_t basicMessage::getCRC()
  {
    NSWFL::Hashing::CRC32 worker;
    worker.Initialize();
    return worker.FullCRC(message.data(),message.size()-sizeof (uint32_t));
  }

  }
