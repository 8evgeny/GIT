/*
 * subscribers_list.cpp - a part of ufw++ object envirinment for
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

#include <system/configs/configplan/subscribers_list.h>

namespace ufw
{
  
  Subscriber::Subscriber (uint32_t ip, uint32_t cid)
  :m_ip(ip),
   m_cid(cid)
  {
  }
  
  uint32_t Subscriber::ip ()
  {
    return m_ip;
  }
  
  uint32_t Subscriber::cid ()
  {
    return m_cid;
  }
  
  void Subscriber::Serialize (ByteArray &destination)
  {
    destination.append((uint8_t*)&m_ip, sizeof(m_ip));
    destination.append((uint8_t*)&m_cid, sizeof(m_cid));
  }

  SubscribersList::SubscribersList ()
  :m_subscribers()
  {
  }

  SubscribersList::SubscribersList (size_t size)
  :m_subscribers(size)
  {
  }

  SubscribersList::SubscribersList (ufw::vector<Subscriber> &other)
  :m_subscribers(other)
  {
  }

  size_t SubscribersList::size ()
  {
    return m_subscribers.length();
  }

  void SubscribersList::add (Subscriber &station)
  {
    m_subscribers.append(station);
  }

  void SubscribersList::add (uint32_t ip, uint32_t cid)
  {
    Subscriber temp(ip,cid);
    add(temp);
  }

  void SubscribersList::Serialize (ByteArray &destination)
  {
    destination.push_back(size());
    for(size_t i=0;i<size();i++)
      {
	destination.push_back(m_subscribers[i].ip());
	destination.push_back(m_subscribers[i].cid());
      }
  }

  void SubscribersList::Deserialize (uint8_t* source)
  {
    uint8_t* data=source+sizeof(size_t);
    for(size_t i=0;i<*(size_t*)source;i++)
      {
	add(((uint32_t*)data)+i);
      }
  }
  
  Subscriber& SubscribersList::at (size_t index)
  {
    return m_subscribers[index];
  }

  Subscriber& SubscribersList::operator [] (size_t index)
  {
    return m_subscribers[index];
  }

  void SubscribersList::add (uint32_t *station)
  {
    add(*station,*(station+1));
  }

} /* namespace ufw */
