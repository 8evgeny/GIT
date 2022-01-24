#ifndef IPS_HELPERS_HPP
#define IPS_HELPERS_HPP

#include <cstdlib>
#include <cinttypes>

#include <vector>

namespace Helpers {
  template<typename T>
    inline void setU32to(T *dataptr, size_t pos, uint32_t element)
    {
      *(reinterpret_cast<uint32_t*> (dataptr + pos * sizeof(T))) = element;
    }

  template<typename T>
    inline uint32_t getU32from(T *dataptr, size_t pos)
    {
      return *(reinterpret_cast<uint32_t*> (dataptr + pos * sizeof(T)));
    }

  template<typename T>
    inline void appendToByteVectorByVal(std::vector<uint8_t> *reciever, const T &var)
    {
    if(!reciever)return;    
      for(size_t i = 0;i < sizeof(T);i++)
        reciever->push_back(reinterpret_cast<const uint8_t*>(&var)[i]);	
    }
}
#endif // IPS_HELPERS_HPP
