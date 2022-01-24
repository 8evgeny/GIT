#include "clientsm.hpp"

#include <QDebug>

namespace ISLP {

  clientStatemachine::clientStatemachine(intercomStation *self, arcCrypt *key) :
      m_self (self), m_key (key)
  {
    if(m_self == nullptr)
      {
	m_state = State::Error;
      }
  }

  State clientStatemachine::operator()(basicMessage *message)
  {
    return proceed (message);
  }

  void clientStatemachine::setSenderFunction(void (*callbackFcn)(uint8_t*, size_t))
  {
    senderFcn = callbackFcn;
  }

  State clientStatemachine::getState()
  {
    return m_state;
  }

  arcCrypt* clientStatemachine::getKey()
  {
    return m_key;
  }

  State clientStatemachine::proceed(basicMessage *message)
  {
    if(getState () == State::Error) return State::Error;
    switch (message->getFunction ())
      {
      case ISLP::Functions::Yell:
	return yellHandler (dynamic_cast<yellMsg*> (message));
      case ISLP::Functions::SetIP:
	return setIpHandler (dynamic_cast<setStationMessage*> (message));
      default:
	return State::None;
      }
  }

  State clientStatemachine::yellHandler(yellMsg *message)
  {
    State t_state = State::None;
    t_state = checkYellValid (message);

    if(t_state > 0) responce ();

    return m_state;
  }

  State clientStatemachine::setIpHandler(setStationMessage *message)
  {
    State t_state = State::None;
    intercomStation temp_station;
    if(getStationFromMsg (temp_station, message, m_key) >= 0)
      {
	if(temp_station == (*m_self))
	  {
	    m_self->copy (temp_station);
	    t_state = State::SelfChanged;
	    if(responce () < 0)
	      {
		qDebug () << "Something goes wrong!";
	      }
	  }
      }
    return m_state = t_state;
  }

  State clientStatemachine::checkYellValid(yellMsg *message)
  {
    arcCrypt crypt_worker ((void*) &ISLP::magicnumber_locator, sizeof(ISLP::magicnumber_locator));
    uint32_t message_key = crypt_worker.decrypt (*message->getChecksumPtr ());
    if(message->checkCRC (message_key))
      {
	setKey (message_key);
	return State::KeyChanged;
      }
    return State::None;
  }

  void clientStatemachine::setKey(uint32_t key)
  {
    if(m_key == nullptr)
      {
	m_key = new arcCrypt ((void*) &key, sizeof(uint32_t));
      }else
      {
	m_key->setKey ((void*) &key, sizeof(uint32_t));
      }
  }

  int clientStatemachine::responce()
  {
    if(!m_key) return -1;
    stationResponse self_response (*m_self, m_key, true);
    if(senderFcn == nullptr)
      {
	qDebug () << "No output function!\n Data: " << self_response.data () << "\nSize: "
	    << self_response.size () << "\n";
      }else
      {
	senderFcn (self_response.data (), self_response.size ());
      }
    return 0;
  }

}
