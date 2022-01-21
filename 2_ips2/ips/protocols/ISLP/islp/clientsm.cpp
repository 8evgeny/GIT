#include "clientsm.hpp"

#include <QDebug>

namespace ISLP {
  // Класс обработчик входящих сообщений (для станции!!!)
  clientStatemachine::clientStatemachine(intercomStation &self, arcCrypt *key) :
      m_self (&self), m_key (key)
  {
    if(m_self == nullptr)
      {
	m_state = State::Error;
      }
  }
  // Оператор вызова обработчика полученного сообщения
  State clientStatemachine::operator()(basicMessage &message)
  {
    return proceed (message);
  }
  // Устанавливает функцию передачи ответа (в С-стиле). В качетве параметра указатель на функцию.
  void clientStatemachine::setSenderFunction(void(*const callbackFcn)(uint8_t *, size_t))
  {
    senderFcn = callbackFcn;
  }
  // Устанавливает функтор передачи ответа (в С++) стиле. В качестве параметра функтор.
  void clientStatemachine::setSenderFunction(std::function<void(uint8_t *, size_t)> callback)
  {
    m_callback=new std::function(std::move(callback));
  }
  // Возвращает состояние
  State clientStatemachine::getState()
  {
    return m_state;
  }
  // Возвращает ключ
  arcCrypt* clientStatemachine::getKey()
  {
    return m_key;
  }
  // Обработчик сообщений
  State clientStatemachine::proceed(basicMessage &message)
  {
    if(getState () == State::Error) return State::Error;
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
  // Обработчик сообщений Yell
  State clientStatemachine::yellHandler(yellMsg &message)
  {
    State t_state = State::None;
    t_state = checkYellValid (message);

    if(t_state > 0) responce ();

    return m_state;
  }
  // Обработчик сообщений SetIp
  State clientStatemachine::setIpHandler(setStationMessage &message)
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
  // Проверка валидности yell, установка сеансового ключа
  State clientStatemachine::checkYellValid(yellMsg &message)
  {
    arcCrypt crypt_worker ((void*) &ISLP::magicnumber_locator, sizeof(ISLP::magicnumber_locator));
    uint32_t message_key = crypt_worker.decrypt (*message.getChecksumPtr ());
    if(message.checkCRC (message_key))
      {
	setKey (message_key);
	return State::KeyChanged;
      }
    return State::None;
  }
  // Установка текущего ключа
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
  // Ответ
  int clientStatemachine::responce()
  {
    if(!m_key) return -1;
    stationResponse self_response (*m_self, m_key, true);
    if(senderFcn != nullptr)
      {
        qDebug () << "C-function invoking\n";
        senderFcn (self_response.data(), self_response.size());
        qDebug () << "C-function invoked\n";
      }else if(m_callback!= nullptr)
      {
        qDebug () << "Functor invoking\n";
        (*m_callback)(self_response.data(), self_response.size());
        qDebug () << "Functor invoked\n";
      } else
      {
	qDebug () << "No output function!\n Data: " << self_response.data () << "\nSize: "
	    << self_response.size () << "\n";
      }
    return 0;
  }

}
