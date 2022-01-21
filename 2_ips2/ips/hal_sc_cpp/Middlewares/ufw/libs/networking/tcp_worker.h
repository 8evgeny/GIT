/*
 * tcp_worker.h
 *
 *  Created on: 3 дек. 2020 г.
 *      Author: Dmitry Donskih
 *  Copyright © 2020 Dmitry Donskih. All rights reserved.
 */

#ifndef LIBS_NETWORKING_TCP_WORKER_H_
#define LIBS_NETWORKING_TCP_WORKER_H_

namespace ufw {
  
  /*
   *
   */
  class tcpWorker :public osThread
    {
    public:
      tcpWorker();
      virtual ~tcpWorker();
      tcpWorker(const tcpWorker &other) = delete;
      tcpWorker(tcpWorker &&other) = delete;
      tcpWorker& operator=(const tcpWorker &other) = delete;
      tcpWorker& operator=(tcpWorker &&other) = delete;
    };

} /* namespace ufw */

#endif /* LIBS_NETWORKING_TCP_WORKER_H_ */
