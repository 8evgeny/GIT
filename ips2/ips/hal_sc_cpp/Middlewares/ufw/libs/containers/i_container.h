/*
 * i_container.h
 *
 *  Created on: 18 дек. 2020 г.
 *      Author: vader
 */

#ifndef UFW_LIBS_CONTAINERS_I_CONTAINER_H_
#define UFW_LIBS_CONTAINERS_I_CONTAINER_H_

#define  LIB_USE_STDLIB17

#include <cstdint>
#include <cstddef>
#include <cstdlib>
#include <array>

namespace ufw
{
  /**
   * Stdlib container fcns (iterators)
   */
  template<typename T>
  class heap_based_container
  {
      using value_type = T;
      using pointer = T*;
      using reference = T&;
      using const_reference = const T&;
      using const_pointer =const T*;
      using iterator = value_type*;
      using const_iterator = const value_type*;
      using size_type = size_t;
      using difference_type = ptrdiff_t;
#ifdef LIB_USE_STDLIB17
      	using reverse_iterator = std::reverse_iterator<iterator>;
      	using const_reverse_iterator = std::reverse_iterator<const_iterator>;
      #endif

    protected:
      pointer m_data_ptr=nullptr;
      size_type m_length=0;
      size_type m_capacity=0;



    public:
      pointer data ()
      	{
      	  return m_data_ptr;
      	}

      	void* raw ()
      	{
      	  return (void*)m_data_ptr;
      	}

      	/**
      	 * Stdlib access support
      	 */
      	_GLIBCXX17_CONSTEXPR
      	reference
      	operator[](size_type n) noexcept
      	{ return *(&m_data_ptr[n]); }

      	constexpr
      	const_reference
      	operator[](size_type n) const noexcept
      	{ return *(&m_data_ptr[n]); }

      	_GLIBCXX17_CONSTEXPR
      	reference
      	at(size_type n)
      	{
      	  LIB_ASSERT(n<SIZE)
      	  return *(&m_data_ptr[n]);
      	}

      	constexpr
      	const_reference
      	at(size_type n) const
      	{
      	  LIB_ASSERT(n<SIZE)
      	  return *(&m_data_ptr[n]);
      	}
      	/*
      	 * Std lib size operations support
      	 *
      	 */
      	constexpr
      	size_type size () const noexcept
      	{
      	  return m_length;
      	}

      	constexpr
      	size_type max_size () const noexcept
      	{
      	  return m_capacity;
      	}

      	constexpr
      	bool empty () const noexcept
      	{
      	  return size () == 0;
      	}

      	void fill (const value_type &value)
      	{
      #ifdef LIB_USE_STDLIB17
      	  std::fill_n (begin (), size (), value);
      #else
      	  for(auto x:m_data){
      	      x=value;
      	  }
      #endif
      	}


      	/**
      	 * Iterators support include
      	 */
      	_GLIBCXX17_CONSTEXPR
      	iterator begin () noexcept
      	{
      	  return iterator (data ());
      	}

      	_GLIBCXX17_CONSTEXPR
      	const_iterator begin () const noexcept
      	{
      	  return const_iterator (data ());
      	}

      	_GLIBCXX17_CONSTEXPR
      	iterator end () noexcept
      	{
      	  return iterator (data () + m_length);
      	}

      	_GLIBCXX17_CONSTEXPR
      	const_iterator end () const noexcept
      	{
      	  return const_iterator (data () + m_length);
      	}
      #ifdef LIB_USE_STDLIB17
      	_GLIBCXX17_CONSTEXPR
      	reverse_iterator rbegin () noexcept
      	{
      	  return reverse_iterator (end ());
      	}

      	_GLIBCXX17_CONSTEXPR
      	const_reverse_iterator rbegin () const noexcept
      	{
      	  return const_reverse_iterator (end ());
      	}

      	_GLIBCXX17_CONSTEXPR
      	reverse_iterator rend () noexcept
      	{
      	  return reverse_iterator (begin ());
      	}

      	_GLIBCXX17_CONSTEXPR
      	const_reverse_iterator rend () const noexcept
      	{
      	  return const_reverse_iterator (begin ());
      	}
      #endif
      	_GLIBCXX17_CONSTEXPR
      	const_iterator cbegin () const noexcept
      	{
      	  return const_iterator (data ());
      	}

      	_GLIBCXX17_CONSTEXPR
      	const_iterator cend () const noexcept
      	{
      	  return const_iterator (data () + m_length);
      	}
      #ifdef LIB_USE_STDLIB17
      	_GLIBCXX17_CONSTEXPR
      	const_reverse_iterator crbegin () const noexcept
      	{
      	  return const_reverse_iterator (end ());
      	}

      	_GLIBCXX17_CONSTEXPR
      	const_reverse_iterator crend () const noexcept
      	{
      	  return const_reverse_iterator (begin ());
      	}
      #endif
  };

}

#endif /* UFW_LIBS_CONTAINERS_I_CONTAINER_H_ */
