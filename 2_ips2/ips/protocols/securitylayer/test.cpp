#include "AES/inc/AES.h"
#include "MD5/inc/MD5.h"
#include "ipscrypt/inc/ipskeygen.h"
#include <iostream>

#define ENC_KEY                                                                \
  {                                                                            \
    0x25, 0x11, 0xa2, 0xff, 0x65, 0x98, 0x36, 0x07, 0x08, 0x09, 0x0a, 0x0b,    \
        0x0c, 0x0d, 0x0e, 0x0f                                                 \
  }
#define KEY_LENGTH 16 * sizeof(unsigned char) * 8
using namespace std;

#define HEX_DATA
#define CRYPT_ECB

int main() {
  unsigned char key[] = ENC_KEY;
  unsigned char data[] = "Hello World!12";
  unsigned char hexdata[16] = {0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
                               0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0};
  unsigned char iv[] = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
                        0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff};
  unsigned char *crypted = NULL;
  unsigned int out_size = 0;
  AES aes(KEY_LENGTH);
  MD5 checksumd5;
  string open;
  cout << sizeof(data) << endl;
  cout << data << endl;
  crypted = aes.EncryptECB(data, sizeof(data), key, out_size);
  cout << crypted << endl;
  cout << out_size << endl;
  for (int i = 1; i < 256; i++) {
#ifndef HEX_DATA
    open = open + to_string(i % 10);
    crypted = aes.EncryptCFB((unsigned char *)open.data(), open.length(), key,
                             iv, out_size);
    cout << open.length() << "   " << out_size
         << " delta=" << out_size - open.length() << " " << open
         << " "; //<<crypted<<endl;
#endif
#ifdef HEX_DATA
    hexdata[0] = i;
#ifdef CRYPT_CBC
    crypted = aes.EncryptCBC(hexdata, 16, key, iv, out_size);
#endif
#ifdef CRYPT_CFB
    crypted = aes.EncryptCFB(hexdata, 16, key, iv, out_size);
#endif
#ifdef CRYPT_ECB
    crypted = aes.EncryptECB(hexdata, 16, key, out_size);
#endif
    cout << "size open=16 "
         << "size crypt=" << out_size << " open hex=";
    printHexArray(hexdata, 16);
    cout << " crypt hex=";
#endif
    printHexArray(crypted, out_size);
    cout << endl;
  }

  /*
   * MD5 algo test
   */

  checksumd5.update((uint8_t)65);
  unsigned char *o_buff;
  unsigned char *o_buff_initialized = new unsigned char[16];
  checksumd5.getMD5sum(o_buff_initialized);
  o_buff = checksumd5.getMD5sum();
  cout << "as copy ";
  printHexArray(o_buff, 16);
  cout << endl;
  cout << "as reference ";
  printHexArray(o_buff_initialized, 16);
  cout << endl;
  cout << "=======================================" << endl;
  string username("Белоконев Кирилл Владимирович");
  cryptkey usr_cert((uint8_t *)username.data());
  ipsKeygen keygen(&usr_cert);
  printHexArray((keygen.streamKey()).data, 16);
  cout << endl;
  printHexArray((keygen.stationKey(1000)).data, 16);
  cout << endl;
  printHexArray((keygen.stationKey(1003)).data, 16);
  cout << endl;
  return 0;
}
