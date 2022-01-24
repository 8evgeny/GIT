/*
 * error_handler.hpp
 *
 *  Created on: 26 нояб. 2020 г.
 *      Author: Dmitry Donskih
 *  Copyright © 2020 Dmitry Donskih. All rights reserved.
 */

#ifndef LIBS_ERROR_ERROR_HANDLER_HPP_
#define LIBS_ERROR_ERROR_HANDLER_HPP_

#include <lib_macros.hpp>

namespace ufw {
  namespace errors {

    enum type
      {
      Warning = 0,
      Moderate = 1,
      Critical = 2,
      Assert = 3
      };
    enum place
      {
      HAL = 0,
      IO = 1,
      Memory = 2,
      CPU = 3,
      Logical = 4,
      OS = 5,
      Assertation = 6
      };

    /*
     *
     */
    void Handle(type type_error, place place_error);
    void Handle(type type_error, place place_error, const char *message);
  
  } /* namespace errors */

}

extern "C" void cErrorHandler(const char *text);

#endif /* LIBS_ERROR_ERROR_HANDLER_HPP_ */
