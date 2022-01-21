/*
 * Copyright (c) 2019-2020 by Dmitry Donskih
 * All rights reserved.
 *
 * Attention Software modification allowed only with
 * the permission of the author.
 */

#pragma once

#include "stdint.h"
#include <cstdint>
#include <cstddef>
#include <cstdlib>


#if UINTPTR_MAX == UINT64_MAX || ULONG_MAX == UINT64_MAX
using address_t=uint64_t;
#elif UINTPTR_MAX == UINT32_MAX
using address_t=uint32_t;
#else
#error
#endif

