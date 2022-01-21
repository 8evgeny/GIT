/*
 * stream_device_test.cpp
 *
 *  Created on: 3 нояб. 2018 г.
 *      Author: Dmitry Donskih
 *  Copyright © 2018-2020 Dmitry Donskih. All rights reserved.
 */

#pragma once

#include "base_container.h"
#include <type_traits>

#ifndef LIB_VECTOR_RESERVE
#define LIB_VECTOR_RESERVE 32
#endif

namespace ufw {
  template<typename Tp>
    class ufwVector :public baseContainer<Tp>
      {
      public:
	ufwVector() :
	    baseContainer<Tp> ()
	{
	}
	ufwVector(size_t count) :
	    baseContainer<Tp> (count)
	{
	}
	ufwVector(Tp *other, size_t size) :
	    baseContainer<Tp> (other, size)
	{
	}
//        vector(const vector<Tp> &other):baseContainer<Tp>(other){
//            std::cout<<__PRETTY_FUNCTION__<<std::endl;//std::cout<<"vector(const vector<Tp> &other)\n";
//        }
//        vector(vector<Tp> &&other):baseContainer<Tp>(std::move(other)){
//            std::cout<<__PRETTY_FUNCTION__<<std::endl;//std::cout<<"vector(const vector<Tp> &other)\n";
//        }
//        vector(baseContainer<Tp> &&other):baseContainer<Tp>(std::move(other)){
//            std::cout<<__PRETTY_FUNCTION__<<std::endl;//std::cout<<"vector(baseContainer<Tp> &&other)\n";

//        }

//vector(size_t size) :baseContainer<Tp>(size){}

	/*
	 * reallocate vector capacity to "size" elements
	 */
	size_t reserve(size_t size)
	{
	  return this->reallocate(size);
	}
	/*
	 * resize(size) - resize vector to the count of elements <size>
	 * !! In case of CPU cost there's no memory deallocation !! use
	 * vector::fit() procedure
	 */
	size_t resize(size_t size)
	{
	  if(this->allocated < size)
	    {
	      this->reallocate (size);
	    }
	  this->el_count = size;
	  return this->el_count;
	}
	/*
	 * fit() - reallocate vector to equals to number of elements
	 */
	size_t fit()
	{
	  return this->reallocate (this->el_count);
	}
	/*
	 * append(..) - append element &  reserve some memory to reduce CPU usage
	 * for reallocation defined in LIB_VECTOR_RESERVE
	 * (default is 32 elements)
	 */
	virtual ufwVector<Tp>& append(Tp value)
	{
	  make_fit (this->el_count + 1);
	  this->data_ptr[this->el_count] = value;
	  this->el_count++;
	  return *this;
	}

	virtual ufwVector<Tp>& push_back(Tp value)
	{
	  append(value);
	  return *this;
	}
	/*
	 * <count>-is a number of elements, not a byte size
	 */
	virtual ufwVector<Tp>& append(const Tp *d, size_t count)
	{
	  make_fit (this->el_count + count);
	  memcpy (&(this->data_ptr[this->el_count]), d, count * sizeof(Tp)); ///////////////////////////////
	  this->el_count += count;
	  return *this;
	}
	virtual ufwVector<Tp>& append(const ufwVector<Tp> &d)
	{
	  return this->append (d.data (), d.size ()); //((baseContainer<Tp>*)&d)
	}

	ufwVector<Tp> operator+=(const Tp &d)
	{
	  append (d);
	  return *this;
	}

	ufwVector<Tp> operator+=(const ufwVector<Tp> &d)
	{
	  append (d);
	  return *this;
	}

	void push_back (void *data, size_t size)
	{

	  if constexpr (std::is_same<Tp, uint8_t>::value)
	    {
	      append ((Tp*) data, size);

	    } else
	    {
	      size_t count;
	      if (size < sizeof(Tp))
		{
		  count = 1;
		} else
		{
		  count = (size % sizeof(Tp) == 0) ? (size / sizeof(Tp)) : (size / sizeof(Tp) + 1);
		}
	      append ((Tp*) data, count);
	    }

	}
//
	template<typename Ti>
	void push_back(Ti value)
	{
	  push_back(&value,sizeof(Ti));
	}

      protected:

	size_t make_fit(size_t size)
	{
	  if(size > this->allocated) return this->reallocate (size + LIB_VECTOR_RESERVE);
	  return this->allocated;
	}
      };


}

template<typename Tp>
  ufw::baseContainer<Tp> operator+(const Tp &r_op, const ufw::ufwVector<Tp> &l_op)
  {
//    ufwLibrary::vector<Tp> temp(r_op);
//    temp.append(l_op);
    return ufw::ufwVector<Tp> (&r_op, 1).append (l_op);
  }

template<typename Tp>
  ufw::baseContainer<Tp> operator+(const ufw::ufwVector<Tp> &r_op, const Tp &l_op)
  {
    return ufw::ufwVector<Tp> (r_op).append (l_op);
  }

template<typename Tp>
  ufw::ufwVector<Tp> operator+(const ufw::ufwVector<Tp> &r_op, const ufw::ufwVector<Tp> &l_op)
  {
    ufw::ufwVector<Tp> temp (r_op);
    temp.append (l_op);
    return temp;  //ufwLibrary::vector<Tp>(r_op).append(l_op);
  }


  using ByteArray=ufw::ufwVector<uint8_t>;




