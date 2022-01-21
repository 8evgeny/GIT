/*
 * ufwallocator.cpp
 *
 *  Created on: 3 нояб. 2020 г.
 *      Author: Dmitry Donskih
 *  Copyright © 2020 Dmitry Donskih. All rights reserved.
 */

#ifndef UFWALLOCATOR_H
#define UFWALLOCATOR_H

#include <cstdlib>
#include <cstring>

#include <lib_asserts.h>
#include <lib_macros.hpp>

#ifdef PLATFORM_TEST

#include <iostream>

#define PLATFORM_MEM_VALID  (n<1000000)
#define PLATFORM_MEM_ALLOC(size, type)  std::malloc(size*sizeof(type))
#define PLATFORM_MEM_DEALLOC(size, obj_ptr) std::free(obj_ptr)
#define PLATFORM_MEM_EXCEPTION  throw std::bad_alloc()
#else
#include <allocation/heap/amazon_heap.h>
#include <error/error_handler.h>

#define PLATFORM_MEM_VALID  (heap::amazonHeap::instance().xPortGetFreeHeapSize()>n)
#define PLATFORM_MEM_ALLOC(size, type)  heap::amazonHeap::instance().pvPortMalloc(size*sizeof(type))
#define PLATFORM_MEM_DEALLOC(size, obj_ptr) heap::amazonHeap::instance().vPortFree(obj_ptr)

#endif


template<class T>
  class ufwAllocator
    {
    public:
      typedef T value_type;
      ufwAllocator() = default;

      template<class U>
	constexpr ufwAllocator(const ufwAllocator<U>&) noexcept
	{
	}

      T* allocate(size_t n)
      {
	if(n == 0) return nullptr;
	if(!PLATFORM_MEM_VALID) PLATFORM_MEM_EXCEPTION;
	if(auto p = static_cast<T*> (PLATFORM_MEM_ALLOC(n, T)))
	  {
//	    report (p, n);
	    return p;
	  }
	PLATFORM_MEM_EXCEPTION;
      }

      void deallocate(T *p, size_t n)
      {
//	report (p, n, 0);
	if(p) PLATFORM_MEM_DEALLOC(n, p);
      }
#ifdef PLATFORM_TEST
    private:
      void report(T *p, std::size_t n, bool alloc = true) const
      {
//        std::cout << (alloc ? "Alloc: " : "Dealloc: ") << sizeof(T)*n
//          << " bytes at " << std::hex << std::showbase
//          << reinterpret_cast<void*>(p) << std::dec << '\n';
      }
#else
//    void report = delete;
#endif

    };

template<class T, class U>
  bool operator==(const ufwAllocator<T>&, const ufwAllocator<U>&)
  {
    return true;
  }
template<class T, class U>
  bool operator!=(const ufwAllocator<T>&, const ufwAllocator<U>&)
  {
    return false;
  }

#endif // UFWALLOCATOR_H
