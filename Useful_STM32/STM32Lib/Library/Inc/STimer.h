#ifndef _STIMER_H
#define _STIMER_H

/*
 STimer - библиотека для многофункциональной отработки нажатия кнопки.
 За основу взята библиотека Гайвера с сайта: https://alexgyver.ru/gyverbutton/

 Документацию на оригинальную библиотеку читай здесь: https://alexgyver.ru/gyverbutton/


 STimer - полноценный таймер на базе системных millis() / micros()
 - Миллисекундный и микросекундный таймер
 - Два режима работы:
 - Режим интервала: таймер "срабатывает" каждый заданный интервал времени
 - Режим таймаута: таймер "срабатывает" один раз по истечении времени (до следующего перезапуска)
 - Служебные функции:
 - Старт
 - Стоп
 - Сброс
 - Продолжить

 Базовая версия послужившая образцом:
 - 3.2
 - Добавлен isEnabled
 - Возможность не запускать таймер при создании
 */
#include "STM32.h"

#define MANUAL 0
#define AUTO 1
#define TIMER_TIMEOUT 0
#define TIMER_INTERVAL 1

enum timerType
{
  US, MS,
};

// ============== STimer (микросекундный и миллисекундный таймер) ================
class STimer
{
  public:
    STimer(timerType type = MS, uint32_t interval = 0);	// объявление таймера с указанием типа и интервала (таймер не запущен, если не указывать)
    void setInterval(uint32_t interval);// установка интервала работы таймера (также запустит и сбросит таймер) - режим интервала
    void setTimeout(uint32_t timeout);	// установка таймаута работы таймера (также запустит и сбросит таймер) - режим таймаута
    bool isReady();	    					                                      // возвращает true, когда пришло время
    bool isEnabled();		    			                                      // вернуть состояние таймера (остановлен/запущен)
    void reset();							                                          // сброс таймера на установленный период работы
    void start();							                                          // запустить/перезапустить (со сбросом счёта)
    void stop();							                                          // остановить таймер (без сброса счёта)
    void resume();							                                        // продолжить (без сброса счёта)

    // служебное
    void setMode(bool mode);  				      // установка режима работы вручную: AUTO или MANUAL (TIMER_INTERVAL / TIMER_TIMEOUT)

  private:
    uint32_t _timer = 0;
    uint32_t _interval = 0;
    uint32_t _resumeBuffer = 0;
    bool _mode = true;
    bool _state = false;
    bool _type = true;
};

#endif
