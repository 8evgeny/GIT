/*
 * tcp_server.h
 *
 *  Created on: 3 дек. 2020 г.
 *      Author: Dmitry Donskih
 *  Copyright © 2020 Dmitry Donskih. All rights reserved.
 */

#ifndef LIBS_NETWORKING_TCP_SERVER_H_
#define LIBS_NETWORKING_TCP_SERVER_H_

namespace ufw {
  
  /*
   *
   */
  class tcpServer
    {
    public:
      tcpServer();
      virtual ~tcpServer();

    protected:
      virtual void create_connection(int sock,uint32_t addr);



    private:
      tcpServer(const tcpServer &other) = delete;
      tcpServer(tcpServer &&other) = delete;
      tcpServer& operator=(const tcpServer &other) = delete;
      tcpServer& operator=(tcpServer &&other) = delete;
    };

} /* namespace ufw */

#endif /* LIBS_NETWORKING_TCP_SERVER_H_ */
