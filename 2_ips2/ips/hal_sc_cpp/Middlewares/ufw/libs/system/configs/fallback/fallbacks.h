/*
 * fallbacks.h - a part of ufw++ object envirinment for
 * microcontrollers.
 *
 *   Created on: 26 янв. 2021 г.
 *       Author: Dmitry Donskih
 *
 * Copyright (c) 2019-2020 by Dmitry Donskih
 * All rights reserved.
 * 
 * Attention Software modification allowed only with
 * the permission of the author.
 * 
 */

#ifndef UFW_LIBS_SYSTEM_CONFIGS_FALLBACK_FALLBACKS_H_
#define UFW_LIBS_SYSTEM_CONFIGS_FALLBACK_FALLBACKS_H_


#include <cstdint>
#include <cstddef>
#include <cstdlib>

#define IPS_DEV_N 9

namespace fbk{

  void FlashFBComplan();
  void FlashFallbackSys();
  void FlashFallBackIP();
}


#endif /* UFW_LIBS_SYSTEM_CONFIGS_FALLBACK_FALLBACKS_H_ */
