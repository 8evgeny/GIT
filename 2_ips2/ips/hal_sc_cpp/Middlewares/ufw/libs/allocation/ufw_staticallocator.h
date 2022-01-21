/*
 * ufw_staticallocator.h
 *
 *  Created on: 26 нояб. 2020 г.
 *      Author: Dmitry Donskih
 *  Copyright © 2020 Dmitry Donskih. All rights reserved.
 */

#ifndef LIBS_ALLOCATION_UFW_STATICALLOCATOR_H_
#define LIBS_ALLOCATION_UFW_STATICALLOCATOR_H_

#include <cstdlib>
#include <cstring>

#include <lib_asserts.h>
#include <lib_macros.hpp>

#include <system/sys_memory.h>


template<class T>
  class ufw_StaticAllocator
    {
    protected:
      ufw::dev::sysMemory &m_memory = ufw::dev::sysMemory::instance ();
    public:
      typedef T value_type;
      ufw_StaticAllocator() = default;

      template<class U>
	constexpr ufw_StaticAllocator(const ufw_StaticAllocator<U>&) noexcept
	{
	}

      T* allocate(size_t n)
      {
	if(n == 0) return nullptr;
	if((m_memory.free_buffers == 0) || (n > UFW_SYSTEM_GP_BUFFER_SIZE)) PLATFORM_MEM_EXCEPTION;
	if(auto p = static_cast<T*> (m_memory.useGPBuffer ()))
	  {
	    report (p, n);
	    return p;
	  }
	PLATFORM_MEM_EXCEPTION;
      }

      void deallocate(T *p, size_t n)
      {
	if(p) m_memory.freeGPBuffer (p);
	report (p, n, 0);
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
  bool operator==(const ufw_StaticAllocator<T>&, const ufw_StaticAllocator<U>&)
  {
    return true;
  }
template<class T, class U>
  bool operator!=(const ufw_StaticAllocator<T>&, const ufw_StaticAllocator<U>&)
  {
    return false;
  }

#endif /* LIBS_ALLOCATION_UFW_STATICALLOCATOR_H_ */
