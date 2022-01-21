/*
 * iSerializable.h
 *
 *  Created on: 11 дек. 2020 г.
 *      Author: vader
 */

#pragma once

#include <type_traits>
#include <containers/ufw_vector.h>


namespace ufw
{
  template<class T>
  using is_serializable = std::is_member_function_pointer<decltype(&T::Serialize)>;

//  class seializedObject
//  {
//  };

  
  class iSerializable
  {
    public:
      virtual void Serialize(ByteArray& destination)=0;
    protected:
      //char* name;
  };

//  template<typedef T>
//

} /* namespace ufw */

template<typename T>
void sSerialize(T* data,size_t count,uint8_t* dest)
{
  T* t_dest=reinterpret_cast<T*>(dest);
  for(size_t i=0;i<count;i++)
    {
      memcpy(t_dest++,data+i,sizeof (T));
    }
}
