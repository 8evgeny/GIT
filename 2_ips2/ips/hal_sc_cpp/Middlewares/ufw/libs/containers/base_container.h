/*
 * stream_device_test.cpp
 *
 *  Created on: 20 сент. 2018 г.
 *      Author: Dmitry Donskih
 *  Copyright © 2018-2020 Dmitry Donskih. All rights reserved.
 */

#pragma once

//#include "../lib_asserts.h" //<lib_asserts.h>
#include <lib_asserts.h>
#include <cstdlib>

#include <allocation/ufw_allocator.h>
#include <lib_macros.hpp>

//CONTAINER_DBGMSG - Platform specified debug output

namespace ufw {

  template<typename T>
    class baseContainer
      {
      protected:
	size_t el_count = 0;
	size_t allocated = 0;
	T *data_ptr = nullptr;
	ufwAllocator<T> memory;

	virtual T& takeByIndex(uint32_t n)
	{
	  return *(data_ptr+n);
	}

	virtual size_t reallocate(size_t size)
	{
	  if(size >= el_count)
	    {
	      T *temp = memory.allocate (size);
	      std::memcpy (temp, data_ptr, el_count * sizeof(T));
	      memory.deallocate (data_ptr, allocated);
	      data_ptr = temp;
	      allocated = size;
	    }
	  return allocated;
	}
      public:

	baseContainer() :
	    baseContainer (0)
	{
	}
	explicit baseContainer(size_t count) :
	    el_count (count), allocated (count)
	{
	  if(el_count)
	    {
	      data_ptr = memory.allocate (el_count);
	    }
	}

	baseContainer(T *other, size_t size)
	{
	  data_ptr = memory.allocate (size);
	  allocated = size;
	  std::memcpy (data_ptr, other, size);
	  el_count = size;
	}

	baseContainer(const baseContainer<T> &other)
	{
	  data_ptr = memory.allocate (other.allocated);
	  allocated = other.allocated;
	  std::memcpy (data_ptr, other.data_ptr, other.el_count * sizeof(T));
	  el_count = other.el_count;
	}

	baseContainer(baseContainer<T> &&other) noexcept :
	    data_ptr (other.data_ptr), el_count (other.el_count), allocated (other.allocated)
	{
	  other.data_ptr = nullptr;
	}

	baseContainer<T>& operator=(const baseContainer<T> &other)
	{
	  if(&other == this) return *this;
	  reallocate (other.allocated);
	  allocated = other.allocated;
	  std::memcpy (data_ptr, other.data_ptr, other.el_count);
	  el_count = other.el_count;
	  return *this;
	}
	baseContainer<T>& operator=(baseContainer<T> &&other) noexcept
	{
	  if(&other == this) return *this;
	  data_ptr = other.data_ptr;
	  el_count = other.el_count;
	  allocated = other.allocated;
	  other.data_ptr = nullptr;
	  return *this;

	}

	virtual ~baseContainer()
	{
	  memory.deallocate (data_ptr, allocated);
	}

	size_t length() const
	{
	  return el_count;
	}

	size_t size() const
	{
	  return (length()*sizeof(T));
	}

	T* data() const
	{
	  return data_ptr;
	}
	T& operator[](uint32_t n)
	{
//	  LIB_ASSERT(n<el_count)
	  return takeByIndex (n);
	}

	void clear(){
	  memory.deallocate(data_ptr, allocated);
	  el_count=0;
	  allocated=0;
	}

      };

}
