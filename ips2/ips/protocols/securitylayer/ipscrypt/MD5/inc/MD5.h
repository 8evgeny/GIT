/* MD5

   C++ rewriten version of basic reference inmplementation of MD5
   algorithm by Dmitriy Donskih(skywalker2710@icloud.com)

   original code: md5.c from reference implementation of RFC 1321

   Copyright (C) 1991-2, RSA Data Security, Inc. Created 1991. All
rights reserved.

License to copy and use this software is granted provided that it
is identified as the "RSA Data Security, Inc. MD5 Message-Digest
Algorithm" in all material mentioning or referencing this software
or this function.

License is also granted to make and use derivative works provided
that such works are identified as "derived from the RSA Data
Security, Inc. MD5 Message-Digest Algorithm" in all material
mentioning or referencing the derived work.

RSA Data Security, Inc. makes no representations concerning either
the merchantability of this software or the suitability of this
software for any particular purpose. It is provided "as is"
without express or implied warranty of any kind.

These notices must be retained in any copies of any part of this
documentation and/or software.

*/

#pragma once

#include <cstring>
#include <iostream>

#define MD5_SIZE 16


class MD5 {
public:
  typedef unsigned int size_type;

  MD5();
  MD5(const std::string &text);
  MD5(uint16_t in);
  void update(const unsigned char *buf, size_type length);
  void update(const char *buf, size_type length);
  void update(uint8_t in);
  void update(uint16_t in);
  void update(uint32_t in);
  MD5 &finalize();
  uint8_t *getMD5sum();
  void getMD5sum(uint8_t *out);
  std::string hexdigest() const;

private:
  void init();

  typedef uint8_t uint1;  //  8bit
  typedef uint32_t uint4; // 32bit
  enum { blocksize = 64 };

  void transform(const uint1 block[blocksize]);
  static void decode(uint4 output[], const uint1 input[], size_type len);
  static void encode(uint1 output[], const uint4 input[], size_type len);

  bool finalized;
  uint1 buffer[blocksize]; // last chunk
  uint4 count[2];          // 64bit counter for number of bits (lo, hi)
  uint4 state[4];
  uint1 digest[16];        // result

  // low level logic operations
  static inline uint4 F(uint4 x, uint4 y, uint4 z);
  static inline uint4 G(uint4 x, uint4 y, uint4 z);
  static inline uint4 H(uint4 x, uint4 y, uint4 z);
  static inline uint4 I(uint4 x, uint4 y, uint4 z);
  static inline uint4 rotate_left(uint4 x, int n);
  static inline void FF(uint4 &a, uint4 b, uint4 c, uint4 d, uint4 x, uint4 s,
                        uint4 ac);
  static inline void GG(uint4 &a, uint4 b, uint4 c, uint4 d, uint4 x, uint4 s,
                        uint4 ac);
  static inline void HH(uint4 &a, uint4 b, uint4 c, uint4 d, uint4 x, uint4 s,
                        uint4 ac);
  static inline void II(uint4 &a, uint4 b, uint4 c, uint4 d, uint4 x, uint4 s,
                        uint4 ac);
};

