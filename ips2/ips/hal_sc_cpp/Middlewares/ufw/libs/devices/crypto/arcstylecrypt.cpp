#include "arcstylecrypt.hpp"
#include <cstring>

namespace ufw
{

  arcCrypt::arcCrypt (void *key, size_t size)
  {
    setKey (key, size);
  }

  arcCrypt::arcCrypt (const ByteArray &key) :
      cipher (key)
  {
  }

  void arcCrypt::decrypt (void *data, size_t size)
  {
    crypt (data, size, cipher);
  }

  void arcCrypt::encrypt (void *data, size_t size)
  {
    crypt (data, size, cipher);
  }

  unsigned int arcCrypt::decrypt (unsigned int data)
  {
    crypt (&data, sizeof(data), cipher);
    return data;
  }

  unsigned int arcCrypt::encrypt (unsigned int data)
  {
    crypt (&data, sizeof(data), cipher);
    return data;
  }

  const uint8_t* arcCrypt::takeKey ()
  {
    return cipher.data ();
  }

  void arcCrypt::setKey (void *key, size_t size)
  {
    cipher.clear ();
    cipher.resize (size);
    memcpy (cipher.data (), key, size);
    //cipher.shrink_to_fit ();
  }

  void arcCrypt::crypt (void *data, size_t size, ByteArray &key)
  {
    for (uint32_t i = 0; i < size; i++)
      {
	(reinterpret_cast<uint8_t*> (data))[i] ^= key[i % key.size ()];
      }
  }

}
