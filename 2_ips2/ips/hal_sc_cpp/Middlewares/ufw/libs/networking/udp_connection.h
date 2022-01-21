/*
 * udp_connection.h
 *
 *  Created on: 3 дек. 2020 г.
 *      Author: Dmitry Donskih
 *  Copyright © 2020 Dmitry Donskih. All rights reserved.
 */

#ifndef LIBS_NETWORKING_UDP_CONNECTION_H_
#define LIBS_NETWORKING_UDP_CONNECTION_H_

namespace ufw {
  
  /*
   *
   */
  class udpConnection
    {
    public:
      udpConnection();
      virtual ~udpConnection();
      udpConnection(const udpConnection &other) = delete;
      udpConnection(udpConnection &&other) = delete;
      udpConnection& operator=(const udpConnection &other) = delete;
      udpConnection& operator=(udpConnection &&other) = delete;
    };

} /* namespace ufw */

#endif /* LIBS_NETWORKING_UDP_CONNECTION_H_ */
