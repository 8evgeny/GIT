#include "basicmessage.hpp"
#include "details.hpp"
#include "../../libips/debug_macro.h"
#include "../../libips/tools/nswfl_crc32.hpp"

#include <cstring>

namespace ISLP
{

  basicMessage::basicMessage () :
      basicMessage (0)
  {  
}

basicMessage::basicMessage (size_t size) :
  message_ptr (new std::vector<uint8_t> (size)), message (*message_ptr)
{
}

basicMessage::basicMessage (uint8_t *data, size_t size) :
basicMessage (size)
{
std::memcpy (message.data (), data, size * sizeof(uint8_t));
}

basicMessage::basicMessage(const basicMessage &other)
    : message(other.message), keyring(other.keyring),
      isvalid(other.isvalid){}

basicMessage::basicMessage(basicMessage && other) noexcept
    : message_ptr(other.message_ptr), message(*message_ptr),
      keyring(other.keyring), isvalid(other.isvalid) {
  other.message_ptr = nullptr;  
}

basicMessage::basicMessage (const std::vector<uint8_t> &data) :
basicMessage (0)
{
*message_ptr = data;
}

basicMessage::basicMessage (std::vector<uint8_t> &&data) :
basicMessage (0)
{
*message_ptr = std::move (data);
}

basicMessage::~basicMessage ()
{
if (message_ptr) delete message_ptr;
}

basicMessage& basicMessage::operator= (const basicMessage &other)
{
message = other.message;
keyring = other.keyring;
isvalid = other.isvalid;
return *this;
}

basicMessage& basicMessage::operator= (basicMessage &&other) noexcept
{
message = std::move (other.message);
keyring = other.keyring;
isvalid = other.isvalid;
return *this;
}

basicMessage& basicMessage::operator= (const std::vector<uint8_t> &other)
{
message = other;
return *this;
}

basicMessage& basicMessage::operator= (std::vector<uint8_t> &&other)
{
message = (std::move (other));
return *this;
}

uint8_t basicMessage::operator[] (size_t index)
{
return message.operator[] (index);
}

bool basicMessage::isISLP ()
{
if (*get32ptr (0) != magicnumber_locator) return false;
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

uint32_t *basicMessage::getChecksumPtr()
{
    return get32ptr(message.size()-sizeof(uint32_t));
}

size_t basicMessage::size ()
{
return message.size ();
}

uint8_t* basicMessage::data ()
{
return message.data ();
}
arcCrypt* basicMessage::getSessionKey() noexcept
{
    return keyring;
}

bool basicMessage::checkCRC(uint32_t crc32)
{
    return (crc32==getCRC());
}
uint32_t basicMessage::getCRC()
{
    NSWFL::Hashing::CRC32 worker;
    worker.Initialize();
    return worker.FullCRC(message.data(),message.size()-sizeof (uint32_t));
}
void* basicMessage::getPayloadPtr ()
{
return message.data () + ISLP::Offset::Data;
}

uint32_t* basicMessage::get32ptr (size_t index)
{
return reinterpret_cast<uint32_t*> (message.data () + index);
}
}
