/*
 * array_type.h
 *
 *  Created on: 21 дек. 2020 г.
 *      Author: vader
 */

#ifndef UFW_LIBS_TYPES_ARRAY_TYPE_H_
#define UFW_LIBS_TYPES_ARRAY_TYPE_H_

#define LIB_USE_STDLIB17

#include <cstdint>
#include <cstddef>
#include <cstdlib>
#include <cstring>

#ifdef LIB_USE_STDLIB17
#include <array>

#endif

#include <lib_asserts.h>

namespace ufw
{
  template<typename T, size_t SIZE>
    class array_type
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
      public:

	array_type () = default;
	array_type (const array_type &other) = default;
	array_type (array_type &&other) = default;
	array_type (std::initializer_list<T>);
//	{
//	  std::copy
//	}
	array_type& operator= (const array_type &other) = default;
	array_type& operator= (array_type &&other) = default;


	array_type (T *data)
	{
	  memcpy (&m_data,data,size());
	}

	pointer data ()
	{
	  return &m_data[0];
	}

	void* raw ()
	{
	  return (void*)&m_data;
	}

	void clear()
	{
	  memset(&m_data,0,sizeof(m_data));
	}

	/**
	 * Stdlib access support
	 */
	_GLIBCXX17_CONSTEXPR
	reference
	operator[](size_type n) noexcept
	{ return *(&m_data[n]); }

	constexpr
	const_reference
	operator[](size_type n) const noexcept
	{ return *(&m_data[n]); }

	_GLIBCXX17_CONSTEXPR
	reference
	at(size_type n)
	{
	  LIB_ASSERT(n<SIZE)
	  return *(&m_data[n]);
	}

	constexpr
	const_reference
	at(size_type n) const
	{
	  LIB_ASSERT(n<SIZE)
	  return *(&m_data[n]);
	}
	/*
	 * Std lib size operations support
	 *
	 */
	constexpr
	size_type size () const noexcept
	{
	  return SIZE;
	}

	constexpr
	size_type max_size () const noexcept
	{
	  return SIZE;
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
	  return iterator (data () + SIZE);
	}

	_GLIBCXX17_CONSTEXPR
	const_iterator end () const noexcept
	{
	  return const_iterator (data () + SIZE);
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
	  return const_iterator (data () + SIZE);
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


      private:
	T m_data[SIZE];
    };

} /* namespace ufw */

template<typename T, size_t SIZE>
  _GLIBCXX17_INLINE
  bool
  operator==(const ufw::array_type<T, SIZE>& lv, const ufw::array_type<T, SIZE>& rv)
  { return std::equal(lv.begin(), lv.end(), rv.begin()); }

template<typename T, size_t SIZE>
  _GLIBCXX17_INLINE
  bool
  operator!=(const ufw::array_type<T, SIZE>& lv, const ufw::array_type<T, SIZE>& rv)
  { return !(lv==rv); }



#endif /* UFW_LIBS_TYPES_ARRAY_TYPE_H_ */
