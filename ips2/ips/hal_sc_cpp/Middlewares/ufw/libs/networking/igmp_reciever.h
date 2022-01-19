/*
 * igmp_reciever.h
 *
 *  Created on: 3 дек. 2020 г.
 *      Author: Dmitry Donskih
 *  Copyright © 2020 Dmitry Donskih. All rights reserved.
 */

#ifndef LIBS_NETWORKING_IGMP_RECIEVER_H_
#define LIBS_NETWORKING_IGMP_RECIEVER_H_

namespace ufw {
  
  /*
   *
   */
  class igmpReciever :public osThread
    {
    public:
      igmpReciever();
      virtual ~igmpReciever();
      igmpReciever(const igmpReciever &other) = delete;
      igmpReciever(igmpReciever &&other) = delete;
      igmpReciever& operator=(const igmpReciever &other) = delete;
      igmpReciever& operator=(igmpReciever &&other) = delete;
    };

} /* namespace ufw */

#endif /* LIBS_NETWORKING_IGMP_RECIEVER_H_ */
