/*
 * os_semaphore.h
 *
 *  Created on: 3 дек. 2020 г.
 *      Author: Dmitry Donskih
 *  Copyright © 2020 Dmitry Donskih. All rights reserved.
 */

#ifndef LIBS_OS_OS_SEMAPHORE_H_
#define LIBS_OS_OS_SEMAPHORE_H_

namespace ufw {
  
  /*
   *
   */
  class osSemaphore
    {
    public:
      osSemaphore();
      ~osSemaphore();

    private:
      osSemaphore(const osSemaphore &other) = delete;
      osSemaphore(osSemaphore &&other) = delete;
      osSemaphore& operator=(const osSemaphore &other) = delete;
      osSemaphore& operator=(osSemaphore &&other) = delete;
    };

} /* namespace ufw */

#endif /* LIBS_OS_OS_SEMAPHORE_H_ */
