/*
 * string.cpp
 *
 *  Created on: 3 нояб. 2018 г.
 *      Author: Dmitry Donskih
 *  Copyright © 2018-2020 Dmitry Donskih. All rights reserved.
 */

#include "ufw_string.h"

namespace ufw {

  ufwString::ufwString() :
      ufwVector<char> (1)
  {
    el_count = 0;
  }

  ufwString::ufwString(const char *cstr) :
      ufwVector<char> (std::strlen (cstr) + 1)
  {
    el_count -= 1;
    memcpy (this->data_ptr, cstr, el_count * sizeof(char));
  }

  ufwString::ufwString(size_t count) :
  	  ufwVector<char> (count)
        {
        }
  ufwString::ufwString(char *other, size_t size) :
  	  ufwVector<char> (other, size)
        {
        }

  ufwString& ufwString::operator =(const char *cstr)
  {
    memory.deallocate (data_ptr, allocated);
    data_ptr = memory.allocate (std::strlen (cstr) + 1);
    std::memcpy (data_ptr, cstr, std::strlen (cstr));
    allocated = std::strlen (cstr) + 1;
    el_count = std::strlen (cstr);
    return *this;
  }
  //TODO: Do Something with this
  ufwString& ufwString::operator=(baseContainer<char> &&other)
  {
    dynamic_cast<baseContainer<char>&> (*this) = other;
    return *this;
  }
// TODO: Do Something with this too
  ufwString& ufwString::operator=(ufwVector<char> &&other)
  {
    dynamic_cast<ufwVector<char>&> (*this) = other;
    return *this;
  }

//string &string::operator=(const vector<char> &other)
//{
//    *(vector<char>*)this=other;
//    return *this;
//}

//string::operator baseContainer<char>() {
//  return dynamic_cast<baseContainer<char>&>(*this);
//}

  ufwString& ufwString::append(const char *value, size_t count)
  {
    if(count == 0)
      {
	push_back((void*)value, std::strlen (value));
	return *this;   //dynamic_cast<ufwString&> (ufwVector<char>::push_back((void*)value, std::strlen (value)));
      }
    push_back((void*)value, count);
    return *this;
  }

  const char* ufwString::operator()() const
  {
    data_ptr[el_count] = 0x00;
    return data ();
  }

  size_t ufwString::reallocate(size_t size)
  {
    return ufwVector<char>::reallocate (size + 1);
  }

//string::string(const string &other) {
//	// TODO Auto-generated constructor stub

//}

//string::string(string &&other) {
//	// TODO Auto-generated constructor stub

//}

//string& string::operator=(const string &other)
//{
//    *this=other();
//    return *this;
//}

//string& string::operator=(string &&other)
//{

//}

} /* namespace ufwLibrary */
