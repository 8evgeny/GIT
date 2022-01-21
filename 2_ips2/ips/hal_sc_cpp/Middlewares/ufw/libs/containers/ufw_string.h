/*
 * string.hpp
 *
 *  Created on: 3 нояб. 2018 г.
 *      Author: Dmitry Donskih
 *  Copyright © 2018-2020 Dmitry Donskih. All rights reserved.
 */

#ifndef REPO_LIBS_CONTAINERS_STRING_HPP_
#define REPO_LIBS_CONTAINERS_STRING_HPP_

#include "ufw_vector.h"

namespace ufw {

  class ufwString :public ufwVector<char>
    {
    public:
      ufwString();
      ufwString(const char *cstr); //make string object from C-style string
      ufwString(size_t count);
      ufwString(char *other, size_t size);
      ufwString& operator=(const char *cstr);
      ufwString& operator=(baseContainer<char> &&other);
      ufwString& operator=(ufwVector<char> &&other);

//    operator baseContainer<char>();

      virtual ufwString& append(const char *value, size_t count = 0) override;

      const char* operator()() const;
    private:
      size_t reallocate(size_t size) override;
    };

} /* namespace ufwLibrary */

#endif /* REPO_LIBS_CONTAINERS_STRING_HPP_ */
