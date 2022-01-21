/*
 * tcp_client_raw.h
 *
 *  Created on: 3 дек. 2020 г.
 *      Author: Dmitry Donskih
 *  Copyright © 2020 Dmitry Donskih. All rights reserved.
 */

#ifndef LIBS_NETWORKING_TCP_CLIENT_H_
#define LIBS_NETWORKING_TCP_CLIENT_H_

namespace ufw {
  
  /*
   *
   */
  class tcpClientRaw
    {
    public:
      tcpClientRaw();
      virtual ~tcpClientRaw();
      tcpClientRaw(const tcpClientRaw &other) = delete;
      tcpClientRaw(tcpClientRaw &&other) = delete;
      tcpClientRaw& operator=(const tcpClientRaw &other) = delete;
      tcpClientRaw& operator=(tcpClientRaw &&other) = delete;
    };

} /* namespace ufw */

#endif /* LIBS_NETWORKING_TCP_CLIENT_H_ */
