#include "clientsm.hpp"
#include "setstationmessage.hpp"
#include "stationresponse.hpp"
#include "../ips_helpers.hpp"
#include "details.hpp"
#include "../debug_macro.h"
#include "../tools/nswfl_crc32.hpp"
#include <functional>

namespace ISLP {

  clientStatemachine::clientStatemachine(intercomStation *self, arcCrypt *key) :
      m_self (self), m_key (key)
  {
    if(m_self == nullptr)
      {
	m_state = State::Error;
      }
  }

// Устанавливает функцию передачи ответа (в С-стиле). В качетве параметра указатель на функцию.
void clientStatemachine::setSenderFunction(void(*const callbackFcn)(uint8_t*, size_t))
{
      senderFcn = callbackFcn;
}

// Устанавливает функтор передачи ответа (в С++) стиле. В качестве параметра функтор.
void clientStatemachine::setSenderFunction(std::function<void(uint8_t *, size_t)> callback)
{
    m_callback=new std::function(std::move(callback));
}

// Оператор вызова обработчика полученного сообщения
State clientStatemachine::operator()(basicMessage *message)
{
   return proceed (message);
}

int clientStatemachine::getStationFromMsg(intercomStation &station, setStationMessage &t_msg, arcCrypt *decoder)
    {
      uint8_t *msg_data = t_msg();
      if(t_msg.size () != Size::Response)
        {      
      return -1;
        }
      if(Helpers::getU32from<uint8_t> (msg_data, Offset::Magic) != magicnumber_locator)
        {      
      return -1;
        }
      if(Helpers::getU32from<uint8_t> (msg_data, Offset::Function) != ISLP::Functions::SetIP)
        {     
      return -1;
        }

      NSWFL::Hashing::CRC32 worker;
      worker.Initialize ();

      decoder->decrypt (msg_data + Offset::Data, Size::Station + Size::CRC);
      if(Helpers::getU32from<uint8_t> (msg_data, Offset::CRC)
      != worker.FullCRC (msg_data, Size::Header + Size::Station))
        {      
      //decoder.encrypt(msg_data+Offset::Data,Size::Station+Size::CRC);
      return -1;
        }
      station.setupData (msg_data + Offset::Data);
      return 0;
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
    if(getStationFromMsg(temp_station, *message, m_key) >= 0)
      {
    if(temp_station == (*m_self))
      {
        m_self->copy (temp_station);
        t_state = State::SelfChanged;
        if(responce () < 0)
          {
//		qDebug () << "Something goes wrong!";
          }
      }
      }
    return m_state = t_state;
  }

  State clientStatemachine::checkYellValid(yellMsg *message)
  {
    arcCrypt crypt_worker (static_cast<void*>(&magicnumber_locator), sizeof(magicnumber_locator));
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
    m_key = new arcCrypt (static_cast<void*>(&key), sizeof(uint32_t));
      }else
      {
    m_key->setKey(static_cast<void*>(&key), sizeof(uint32_t));
      }
  }
  int clientStatemachine::responce()
  {
    if(!m_key) return -1;
    stationResponse self_response(*m_self, m_key, true);
    if(senderFcn == nullptr)
      {
    //qDebug () << "No output function!\n Data: " << self_response.data () << "\nSize: "
    //    << self_response.size () << "\n";
      }else
      {
    senderFcn (self_response.data (), self_response.size ());
      }
    return 0;
  }
}
