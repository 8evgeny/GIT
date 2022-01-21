/*
 * subscribers_list.h - a part of ufw++ object envirinment for
 * microcontrollers.
 *
 *   Created on: 20 янв. 2021 г.
 *       Author: Dmitry Donskih
 *
 * Copyright (c) 2019-2020 by Dmitry Donskih
 * All rights reserved.
 * 
 * Attention Software modification allowed only with
 * the permission of the author.
 * 
 */

#pragma once

#include <cstdint>
#include <cstddef>
#include <cstdlib>

#include <common/serializer.h>
#include <containers/ufw_containers.h>
//#define

namespace ufw
{
  /**
   *
   */
  class Subscriber : public iSerializable
  {
    public:
      Subscriber(uint32_t ip,uint32_t cid);

      uint32_t ip();
      uint32_t cid();

      void Serialize(ByteArray& destination) override;


    protected:
      uint32_t m_ip,m_cid;
  };
  
  /**
   *
   */
  class SubscribersList : public iSerializable
  {
    public:
      SubscribersList();
      SubscribersList(size_t size);
      SubscribersList(ufw::vector<Subscriber> &other);

      size_t size();
      void add(Subscriber& station);
      void add(uint32_t ip,uint32_t cid);
      Subscriber& at(size_t index);
      Subscriber& operator[](size_t index);

      void Serialize(ByteArray& destination) override;
      void Deserialize(uint8_t* source);
    protected:
      void add(uint32_t* station);
      ufw::vector<Subscriber> m_subscribers;
  };
} /* namespace ufw */

