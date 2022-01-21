/*
C++ key generator for GITComm IPS stations
Used as part of GIT-Comm IPS project.

Copyright (c) 2020 Donskih Dmitriy

*/

#pragma once

#ifndef IPS_MBED
#include <cstdlib>
#include <ctime>
#include <fstream>

using namespace std;
#endif

#include "AES/inc/AES.h"
#include "MD5/inc/MD5.h"
#include "cryptkey.h"
#include "ips_crypt_defines.h"

class ipsKeygen {
public:
  ipsKeygen(cryptkey *system_certificate = NULL);
  cryptkey stationKey(uint16_t CID);
  cryptkey streamKey();

private:
  void initRng();
  void rngGenNewKey();
  MD5 md5worker;
  cryptkey certificate;
  uint8_t rng_buffer[KEY_SIZE];
  int rngtype = 0;
};
