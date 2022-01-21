/*
 * interfaces_net.h - a part of CMppCore -an object engine for Cortex-M
 * microcontrollers.
 *
 *  Created on: 22 дек. 2020 г.
 *      Author: Dmitry Donskih
 *
 * Copyright (c) 2019-2020 by Dmitry Donskih
 * All rights reserved.
 *
 * Attention Software modification allowed only with
 * the permission of the author.
 */
#ifndef UFW_LIBS_COMMON_INTERFACES_NET_H_
#define UFW_LIBS_COMMON_INTERFACES_NET_H_

namespace ufw
{
  class iConnection
  {
      virtual void declareDisconnect()=0;
  };
  
  class iNetconOwner
  {
    public:
      virtual void closeConnection()=0;
  };

} /* namespace ufw */

#endif /* UFW_LIBS_COMMON_INTERFACES_NET_H_ */
