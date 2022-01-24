/*
 * os_mutex.h
 *
 *  Created on: 3 дек. 2020 г.
 *      Author: Dmitry Donskih
 *  Copyright © 2020 Dmitry Donskih. All rights reserved.
 */

#ifndef LIBS_OS_OS_MUTEX_H_
#define LIBS_OS_OS_MUTEX_H_

namespace ufw {
  
  /*
   *
   */
  class osMutex
    {
    public:
      osMutex();
      ~osMutex();
    private:
      osMutex(const osMutex &other) = delete;
      osMutex(osMutex &&other) = delete;
      osMutex& operator=(const osMutex &other) = delete;
      osMutex& operator=(osMutex &&other) = delete;
    };

} /* namespace ufw */

#endif /* LIBS_OS_OS_MUTEX_H_ */
