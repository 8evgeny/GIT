/*
 * ufnew.cpp
 *
 *  Created on: 4 нояб. 2020 г.
 *      Author: Dmitry Donskih
 *  Copyright © 2020 Dmitry Donskih. All rights reserved.
 */

#include <cstring>
#include <stdlib.h>
#include <lib_macros.hpp>

#ifdef PLATFORM_TEST
#include <iostream>
#else
#include <allocation/heap/amazon_heap.h>
#endif


#ifdef UFW_COUNT_ALLOCATIONS
extern unsigned int g_user_alloc_counts=0;
#define UFW_USER_ALLOC_INC	g_user_alloc_counts++;
#else
#define UFW_USER_ALLOC_INC
#endif



void* operator new(std::size_t size)
    {
#ifdef PLATFORM_TEST
//      std::cout << "new allocated" << size << "bytes\n";
      return std::malloc (size);
#else
      UFW_USER_ALLOC_INC
      return heap::amazonHeap::instance().pvPortMalloc(size);
#endif

    }

void operator delete(void *ptr)
    {
#ifdef PLATFORM_TEST
//      std::cout << "deleted " << ptr << "\n";
      std::free (ptr);
#else
      heap::amazonHeap::instance().vPortFree(ptr);
#endif
    }

