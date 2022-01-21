/*
 * igmp_sender.h
 *
 *  Created on: 3 дек. 2020 г.
 *      Author: Dmitry Donskih
 *  Copyright © 2020 Dmitry Donskih. All rights reserved.
 */

#ifndef LIBS_NETWORKING_IGMP_SENDER_H_
#define LIBS_NETWORKING_IGMP_SENDER_H_

namespace ufw {
  
  /*
   *
   */
  class igmpSender
    {
    public:
      igmpSender();
      virtual ~igmpSender();
      igmpSender(const igmpSender &other) = delete;
      igmpSender(igmpSender &&other) = delete;
      igmpSender& operator=(const igmpSender &other) = delete;
      igmpSender& operator=(igmpSender &&other) = delete;
    };

} /* namespace ufw */

#endif /* LIBS_NETWORKING_IGMP_SENDER_H_ */
