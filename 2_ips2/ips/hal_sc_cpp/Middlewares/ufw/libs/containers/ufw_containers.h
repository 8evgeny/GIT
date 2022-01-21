/*
 * ufw_containers.h - a part of ufw++ object envirinment for
 * microcontrollers.
 *
 *   Created on: 20 янв. 2021 г.
 *       Author: Dmitry Donskih
 *
 * Copyright (c) 2019-2020 by Dmitry Donskih
 * All rights reserved.
 * 
 * Attention Software modification allowed only with
 * the permission of the author.
 * 
 */

#pragma once

#define UFW_USING_STD_CONTAINERS

#define UFW_USING_STD_DEQUE
#define UFW_USING_STD_MAP
#define UFW_USING_STD_STACK

#include "base_container.h"
#include "ufw_array.h"
#include "ufw_vector.h"
#include "ufw_string.h"
#include "byte_stream.h"

#ifdef UFW_USING_STD_CONTAINERS

#include <array>
#include <vector>
#include <string>
#include <list>

#ifdef UFW_USING_STD_DEQUE
#include <deque>
#endif
#ifdef UFW_USING_STD_MAP
#include <map>
#endif
#ifdef UFW_USING_STD_STACK
#include <stack>
#endif

#endif



#ifdef UFW_USING_STD_DEQUE
  namespace ufw
  {
  template <typename UT>
  using deque = std::deque<UT>;

  template <typename UT>
  using vector = ufw::ufwVector<UT>;

  }
#endif

