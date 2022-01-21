/*
C++ key generator for GITComm IPS stations
Used as part of GIT-Comm IPS project.

Copyright (c) 2020 Donskih Dmitriy

*/

#include "ipskeygen.h"

ipsKeygen::ipsKeygen(cryptkey *system_certificate) {
  if (!system_certificate) {
    IPS_ERR_HANDLER;
  }
  certificate = system_certificate;
  memset(rng_buffer, 0, KEY_SIZE);
  initRng();
}

cryptkey ipsKeygen::stationKey(uint16_t CID) {
  md5worker.update(CID);
  cout << endl << "Cid MD5=";
  printHexArray(md5worker.getMD5sum(), 16);
  cout << endl;
  return certificate ^ (md5worker.getMD5sum());
}

cryptkey ipsKeygen::streamKey() {
  rngGenNewKey();
  return cryptkey(rng_buffer);
}

void ipsKeygen::initRng() {
  rngtype = IPS_RNG_STD;
#ifdef IPS_LINUX
  ifstream rngfile;
  rngfile.open("/dev/hwrng", ios_base::binary);
  rngfile.get((char *)(&rng_buffer), KEY_SIZE);
  if (rngfile.good()) {
    rngfile.close();
    rngtype = IPS_RNG_LINUX_HW;
    cout << "Hardware RNG" << endl;
    return;
  }
  rngfile.close();

  rngfile.open("/dev/random", ios_base::binary);
  rngfile.get((char *)(&rng_buffer), KEY_SIZE);
  if (rngfile.good()) {
    rngfile.close();
    rngtype = IPS_RNG_LINUX_SW;
    cout << "Software RNG" << endl;
    return;
  }
  rngfile.close();
#endif
}

void ipsKeygen::rngGenNewKey() {
  ifstream rngfile;
  switch (rngtype) {
  case IPS_RNG_LINUX_HW:
    rngfile.open("/dev/hwrng", ios_base::binary);
    rngfile.get((char *)(&rng_buffer), KEY_SIZE);
    if (!rngfile.good()) {
      IPS_ERR_HANDLER;
    }
    rngfile.close();
    break;
  case IPS_RNG_LINUX_SW:
    rngfile.open("/dev/random", ios_base::binary);
    rngfile.get((char *)(&rng_buffer), KEY_SIZE);
    if (!rngfile.good()) {
      IPS_ERR_HANDLER;
    }
    rngfile.close();
    break;
  case IPS_RNG_STD:
    std::srand(std::time(nullptr));
    int t_rng_buff[KEY_SIZE / 4];
    for (int i = 0; i < (KEY_SIZE / 4); i++) {
      t_rng_buff[i] = std::rand();
    }
    memmove(t_rng_buff, rng_buffer, KEY_SIZE);
    break;
  default:
    IPS_ERR_HANDLER;
    break;
  }
}
