#pragma once
#include <cstring>
#include <cstdlib>
/*
 * This is an OpenSSL-compatible implementation of the RSA Data Security,
 * Inc. MD5 Message-Digest Algorithm (RFC 1321).
 *
 * Written by Solar Designer <solar at openwall.com> in 2001, and placed
 * in the public domain.  There's absolutely no warranty.
 *
 * This differs from Colin Plumb's older public domain implementation in
 * that no 32-bit integer data type is required, there's no compile-time
 * endianness configuration, and the function prototypes match OpenSSL's.
 * The primary goals are portability and ease of use.
 *
 * This implementation is meant to be fast, but not as fast as possible.
 * Some known optimizations are not included to reduce source code size
 * and avoid compile-time configuration.
 */

/*
 * Updated by Scott MacVicar for arduino
 * <scott@macvicar.net>
 */




namespace MD5 {

typedef unsigned long u32plus;

typedef struct {
	u32plus lo, hi;
	u32plus a, b, c, d;
	unsigned char buffer[64];
	u32plus block[16];
} CTX;

unsigned char *make_hash1(char *arg);
unsigned char *make_hash2(const void *data, size_t size);
char *make_digest(const unsigned char *digest, int len);
const void *body(void *ctxBuf, const void *data, size_t size);
void Init(void *ctxBuf);
void Final(unsigned char *result, void *ctxBuf);
void Update(void *ctxBuf, const void *data, size_t size);

}

