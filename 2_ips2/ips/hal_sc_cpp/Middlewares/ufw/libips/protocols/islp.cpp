/*
 * islp.cpp - a part of ufw++ object envirinment for
 * microcontrollers.
 *
 *   Created on: 21 ноя. 2020 г.
 *       Author: Dmitry Donskih
 *
 * Copyright (c) 2019-2020 by Dmitry Donskih
 * All rights reserved.
 * 
 * Attention Software modification allowed only with
 * the permission of the author.
 * 
 */

#include <protocols/islp.h>
#include <system/System.h>
static ufw::System &Self = ufw::System::instance ();

static const char *s_islp_group = "239.168.10.100";
static const uint16_t s_group_recv_port = 13120;
static const uint16_t s_group_send_port = 13121;
static const uint32_t s_nv_lastyell = 0x50;

namespace ISLP
{
  
  /**
   *
   * @return
   */
  IslpHandler& IslpHandler::Instance ()
  {
    static IslpHandler s_islp;
    return s_islp;
  }
  
  /**
   *
   * @return
   */
  IslpHandler* IslpHandler::pointer ()
  {
    return this;
  }
  
  /**
   *
   */
  IslpHandler::IslpHandler () :
      ufw::rtosThread ("ISLP", 512, osPriorityLow), m_in_sock (s_islp_group, 13120), m_out_sock (
	  s_islp_group, 13121)
  {
    uint32_t temp;
    if (m_out_sock.config () < 0)
      {
	Self.stddebug () << "Outgoing ISLP socket init fail\n";
	LIB_HALERROR_HANDLE;
      }
    if (m_in_sock.config () < 0)
      {
	Self.stddebug () << "Incoming ISLP socket init fail\n";
	LIB_HALERROR_HANDLE;
      }
    Self.stddebug () << "${timer}" << " ISLP subsystem if established\n";
    //Self.ConfigFlash ().byteRead (&temp, s_nv_lastyell, sizeof(uint32_t));
    m_key = new ufw::arcCrypt (&temp, sizeof(uint32_t));
  }

  /**
   * Override to thread that waits untill group yell message comes
   */
  void IslpHandler::threadFunction ()
  {
    ByteArray buffer;
    while(!m_in_sock.isSocketCreated())osDelay(100);
    while(1)
      {
	buffer.resize(512);
	size_t income_size=m_in_sock.mcast_read(buffer.data(), buffer.size());
	if(income_size>0){
	    buffer.resize(income_size);
	    basicMessage incoming(buffer);
	    proceed(incoming);
	}
      }
  }
  
  /**
   *
   * @param message
   * @return
   */
  ISLP::State IslpHandler::operator () (basicMessage &message)
  {
    return proceed (message);
  }
  
  /**
   *
   * @return
   */
  ISLP::State IslpHandler::getState ()
  {
    return m_state;
  }
  
  /**
   *
   * @return
   */
  ufw::arcCrypt* IslpHandler::getKey ()
  {
    return m_key;
  }

  /**
   *
   * @param message
   * @return
   */
  ISLP::State IslpHandler::proceed (basicMessage &message)
  {
    if (getState () == State::Error) return State::Error;
    switch (message.getFunction ())
      {
      case ISLP::Functions::Yell:
	return yellHandler (*reinterpret_cast<yellMsg*> (&message));
      case ISLP::Functions::SetIP:
	return setIpHandler (*reinterpret_cast<setStationMessage*> (&message));
      default:
	return State::None;
      }
  }

  /**
   *
   * @param message
   * @return
   */
  ISLP::State IslpHandler::yellHandler (yellMsg &message)
  {
    State t_state = State::None;
    t_state = checkYellValid (message);

    if (t_state > 0) responce ();

    return m_state;
  }

  /**
   *
   * @param message
   * @return
   */
  ISLP::State IslpHandler::setIpHandler (setStationMessage &message)
  {
    State t_state = State::None;
    intercomStation temp_station;
    if (getStationFromMsg (temp_station, message, m_key) >= 0)
      {
	if (temp_station == m_self)
	  {
	    m_self.copy (temp_station);
	    t_state = State::SelfChanged;
	    if (responce () < 0)
	      {
		Self.stddebug () << "Something goes wrong!\nBut now we must flush and reboot\n";
	      }
	    Self.ConfigFlash().byteWrite((void*)getKey()->takeKey(), s_nv_lastyell, sizeof(uint32_t));
	    ufw::DeviceConfig::Instance().syncSystem(m_self);
	  }
      }
    return m_state = t_state;
  }

  /**
   *
   * @param message
   * @return
   */
  ISLP::State IslpHandler::checkYellValid (yellMsg &message)
  {
    ufw::arcCrypt crypt_worker ((void*) &ISLP::magicnumber_locator, sizeof(ISLP::magicnumber_locator));
    uint32_t message_key = crypt_worker.decrypt (*message.getChecksumPtr ());
    if (message.checkCRC (message_key))
      {
	setKey (message_key);
	return State::KeyChanged;
      }
    return State::None;
  }

  /**
   *
   * @param key
   */
  void IslpHandler::setKey (uint32_t key)
  {
    if (m_key == nullptr)
      {
	m_key = new ufw::arcCrypt ((void*) &key, sizeof(uint32_t));
      } else
      {
	m_key->setKey ((void*) &key, sizeof(uint32_t));
      }
  }

  /**
   *
   * @return
   */
  int IslpHandler::responce ()
  {
    if (!m_key) return -1;
    stationResponse self_response (m_self, m_key, true);
    m_out_sock.mcast_send(self_response.data (),self_response.size ());
    return 0;
  }

  IslpHandler::~IslpHandler ()
  {
    delete m_key;
  }

} /* namespace ips */
