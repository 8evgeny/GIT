/*
 * RTC.h
 *
 *  Created on: 26 окт. 2021 г.
 *      Author: User
 */

#ifndef INC_RTC_H_
#define INC_RTC_H_

#include <stdint.h> 
#include <cstddef>
#include <STM32.h>

typedef void (*callback_IRQ)(void);

enum{
  RTC_LSI = 0b10,
  RTC_LSE = 0b01,
  RTC_HSE = 0b11
};

enum Weekday{
  Everyday = 0,
  Mondey,
  Tuesday,
  Wednesday,
  Thursday,
  Friday,
  Saturday,
  Sunday,
  ByDate
};

typedef struct
{
  uint8_t Hour;
  uint8_t Minute;
  uint8_t Sec;
} sRTC_Time;

typedef struct
{
  uint8_t   Date;
  uint8_t   Month;
  uint16_t  Year;
  uint8_t   Week;
} sRTC_Date;

class RTC_main : public PeripheralUnit
{
  public:
    RTC_main(RTC_TypeDef* RTC_Handle);                                                    // Конструктор
    virtual       ~RTC_main();                                                            // Деструктор
    uint8_t       Init(uint8_t RTC_Tact);                                                 // Инициализация RTC
    void          Reset(void);                                                            // Сброс RTC
    void          setDate(uint8_t Date, uint8_t Month, uint16_t Year, uint8_t Week);      // Установка даты
    void          setTime(uint8_t Hour, uint8_t Minute, uint8_t Sec);                     // Установка времени
    uint16_t      getYear(void);                                                          // Читаем год
    uint8_t       getWeek(void);                                                          // День недели
    uint8_t       getMonth(void);                                                         // Месяц
    uint8_t       getDate(void);                                                          // Дату
    uint8_t       getHour(void);                                                          // Час
    uint8_t       getMinute(void);                                                        // Минуты
    uint8_t       getSeconds(void);                                                       // Секунды
    void          setIT_WUCK(callback_IRQ ExternIT, uint16_t Timer);   // Устанавливаем свой обработчик прерывания, с интервалом в секундах
    void          getTime(void);
    void          getDates(void);
    void          setAlarmA(Weekday Day, uint8_t Hour, uint8_t Minute, uint8_t Seconds, uint8_t Date = 0);
    sRTC_Time     Time;
    sRTC_Date     Dates;

  private:
    void          interruptHandler();
    uint32_t      _Alarm_A;
    uint32_t      _Alarm_B;
    uint8_t       _Prediv_A;
    uint16_t      _Prediv_S;
    uint8_t       _RTC_FreqSource = 0;
    IRQn_Type     _Irq;
    bool          _isInitialized = false;
    void          (*_pfUserISR)(void) = NULL;
    RTC_TypeDef*  _RTC_BaseN;
};

#endif /* INC_RTC_H_ */
