/*
 * devices.h
 *
 *  Created on: 6 дек. 2020 г.
 *      Author: vader
 */

#pragma once

#ifndef PLATFORM_TEST

#include "main.h"

#else

#endif

namespace ufw
{
  enum ioEvent
  {
    NONE=int(0),
    RxDone=int(1),
    TxDone=int(2),
  };

  enum ioresult
  {
    OK = int(0),
    ERROR = int(-1),
    BUSY = int(-2),
    TIMEOUT = int(-3),
    WRONGCMD = int(-12),
    IncorrectAddress = int(-13),
    UNKNOWN
  };
  inline ioresult Status2UFWresult(HAL_StatusTypeDef status)
  {
    return static_cast<ioresult>(-status);
  }


}


