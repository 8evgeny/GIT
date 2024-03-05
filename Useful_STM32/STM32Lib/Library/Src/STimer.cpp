
#include "STimer.h"

// конструктор
STimer::STimer(timerType type, uint32_t interval)
{
  setInterval(interval);		                                                    // запуск в режиме ИНТЕРВАЛА
  _type = type;				                                                          // установка типа
}

// запуск в режиме интервала
void STimer::setInterval(uint32_t interval)
{
  if(interval != 0)                                                             // защита от ввода 0
  {
    _interval = interval;			                                                  // установка
    _mode = TIMER_INTERVAL;			                                                // режим "интервал"
    start();						                                                        // сброс и запуск
  }
  else stop();						                                                      // остановка, если время == 0
}

// запуск в режиме таймаута
void STimer::setTimeout(uint32_t timeout)
{
  setInterval(timeout);		                                                      // задать интервал и запустить
  _mode = TIMER_TIMEOUT;		// режим "таймаут"
}

// остановить счёт
void STimer::stop()
{
  _state = false;
  _resumeBuffer = ((_type) ? millis() : micros()) - _timer;		                  // запомнили "время остановки"
}

// продолжить счёт
void STimer::resume()
{
  start();
  _timer -= _resumeBuffer;	                                                    // восстановили время остановки
}

// перезапустить счёт
void STimer::start()
{
  _state = true;
  reset();
}

// сброс периода
void STimer::reset()
{
  _timer = (_type) ? millis() : micros();
}

// состояние
bool STimer::isEnabled()
{
  return _state;
}

// проверка таймера v2.0 (соблюдение интервалов, защита от пропуска и переполнения)
bool STimer::isReady()
{
  if(!_state) return false;							                                        // если таймер остановлен
  uint32_t thisTime = (_type) ? millis() : micros();	                          // текущее время
  if(thisTime - _timer >= _interval)                                            // проверка времени
  {
    if(_mode)                                                                   // если режим интервалов
    {
      do
      {
        _timer += _interval;
        if(_timer < _interval) break;			                                      // защита от переполнения uint32_t
      } while (_timer < thisTime - _interval);	                                // защита от пропуска шага
    }
    else                                                                        // если режим таймаута
    {
      _state = false;					                                                  // остановить таймер
    }
    return true;
  }
  else
  {
    return false;
  }
}

// сменить режим вручную
void STimer::setMode(bool mode)
{
  _mode = mode;
}
