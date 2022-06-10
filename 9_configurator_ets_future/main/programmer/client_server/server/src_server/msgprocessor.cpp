#include "msgprocessor.hpp"
#include "include/typedefs.h"
#include "../../../libips/islp/yellmsg.hpp"

#define WAIT_SLEEP_TIME 10

msgProcessor::msgProcessor(QObject *parent) :
  QObject(parent),
  m_reciever(m_group_recv_ip,m_group_recv_port,this),
  m_sender(m_group_send_ip,m_group_send_port,this)
{
  //Создаём приёмник и передатчик, соединяем сигналы
  connect(&m_reciever,SIGNAL(Recieved(std::vector<uint8_t>*)),this,SLOT(recieved(std::vector<uint8_t>*)));
  // Создаем тред жизненного цикла обработчика
  processor=QThread::create(&msgProcessor::process,this);
  processor->setObjectName(IPS_STR("Message processor"));
}

msgProcessor::~msgProcessor()
{
  if(m_key)delete m_key;  // Чистим ключ если необходимо
  processor->requestInterruption(); // Посылаем запрос на прерывание обработчика
  processor->wait(1000); // Ждем завершения (1000мсек)
  delete processor; // Удаляем объект
}

// Запускаем жизненный цикл обработчика.
void msgProcessor::start()
{
  processor->start(); // Сначала процессор
  m_reciever.start(); // Потом приёмник
}

// Сброс ключей, очереди сообщений и списка станций
void msgProcessor::reset()
{
  if(m_key!=nullptr)
    {
      delete m_key;
      m_key=nullptr;
    }
  m_stations.clear();
  m_messages.clear();
}

// Посылаем станциям запрос Yell (На этом этапе генерируется ключ). Станции созданные предыдущим вызовом удаляются(Для предотвращения дублирования).
void msgProcessor::yellStations()
{
  reset(); //Чистим списки станций и очереди сообщений
  ISLP::yellMsg stations_yell(m_key); //Создаем Yell
  m_key=stations_yell.Key(); //Получаем ключ
  m_sender(stations_yell(),stations_yell.size()); // Посылаем сообщение
}

// Доступ к списку станций
std::list<intercomStation> &msgProcessor::stations()
{
  return m_stations;
}

// Слот обработчик принятых сообщений
void msgProcessor::recieved(std::vector<uint8_t> *message)
{
  m_messages.push_back(ISLP::stationResponse(std::move(*message))); // Создаётся объект из вектора сообщения и заталкивается в очередь сообщений
  delete message; // Полученный вектор удаляется
}

// Тред обработчика
void msgProcessor::process()
{
  while(!processor->isInterruptionRequested()) // Бесконечный цикл пока не запрошено прерывание
    {
      while(!m_messages.empty()) // Проверяем не пуст ли список
        {
          messageProcess(m_messages.front()); // Обрабатываем сообщение из начала очереди.
          m_messages.pop_front(); // Удаляем обработанное сообщение
        }
      QThread::msleep(WAIT_SLEEP_TIME); // Когда список пуст засыпаем на WAIT_SLEEP_TIME миллисекунд. чтобы не гонять процессор впустую.
      // Так делать нежелателно. Лучше через примитивы синхронизации, но описывать как работают семафоры, мьютекы итд итп мне некогда.
    }
}

void msgProcessor::messageProcess(ISLP::stationResponse &msg)
{
  if(m_key!=nullptr) { //Если ключ сгенерен
      intercomStation *t_station=msg(m_key); // Получаем указатель на данные станции или nullptr если сообщение невалидно
      if(t_station!=nullptr)
        {
          m_stations.push_back(*t_station); // если сообщение валидно - разыменовываем указатель и заталкиваем (копированием)объект в список
          delete t_station; // чистим память полученного указателя
          emit Refreshed(); // Посылаем сигнал об обновлении списка.
        }
    }
}
