/*
 * network_helpers.h
 *
 *  Created on: 19 дек. 2020 г.
 *      Author: vader
 */

#ifndef UFW_LIBS_TYPES_NETWORK_HELPERS_H_
#define UFW_LIBS_TYPES_NETWORK_HELPERS_H_

#include <cstdint>
#include <cstddef>
#include <cstdlib>

#include <lib_asserts.h>

#define UFW_MAKEU32(a,b,c,d) (((u32_t)((a) & 0xff) << 24) | \
                               ((u32_t)((b) & 0xff) << 16) | \
                               ((u32_t)((c) & 0xff) << 8)  | \
                                (u32_t)((d) & 0xff))

namespace ufw
{
  namespace types
  {
    /*
     * wrapper to arrays mades it readonly
     */
    template<typename T,size_t SIZE>
    class array_view
    {
	using value_type = T;
	using pointer = T*;
	using reference = T&;
	using const_ref = const T&;

      public:
	array_view(pointer to_array)
	  :array_to_view(to_array)
	{
	}
	value_type operator[](size_t index) const
	{
	  if (index>=SIZE)
	    {
	      return array_to_view[(SIZE-1)];
	    }
	  return array_to_view[index];

	}
	size_t size()
	{
	  return SIZE;
	}
      private:
	pointer array_to_view;

	array_view()=delete;
	array_view (const array_view &other) = delete;
	array_view (array_view &&other) = delete;
	array_view& operator= (const array_view &other) = delete;
	array_view& operator= (array_view &&other) = delete;

    };
  }

  using ip_t_view=ufw::types::array_view<uint8_t,4>;
  using mac_t_view=ufw::types::array_view<uint8_t,6>;
  using setial_t_view=ufw::types::array_view<char,12>;

}

template<typename T,size_t SIZE>
bool operator==(const ufw::types::array_view<T,SIZE> &lv, const ufw::types::array_view<T,SIZE> &rv)
{
  for (size_t i=0;i<SIZE;i++){
      if (rv[i]!=lv[i]) return false;
  }
  return true;
}

template<typename T,size_t SIZE>
bool operator!=(const ufw::types::array_view<T,SIZE> &lv, const ufw::types::array_view<T,SIZE> &rv)
{
  for (size_t i=0;i<SIZE;i++){
      if (rv[i]==lv[i])return false;
  }
  return true;
}


#endif /* UFW_LIBS_TYPES_NETWORK_HELPERS_H_ */
