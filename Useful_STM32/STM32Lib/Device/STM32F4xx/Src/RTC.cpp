/*
 * RTC.cpp
 *
 *  Created on: 26 окт. 2021 г.
 *      Author: User
 */

//****************************************************************************** 
//  Секция include: здесь подключается заголовочный файл к модулю 
//****************************************************************************** 
#include  "RTC.h"      // Включаем файл заголовка для нашего модуля 


RTC_main::RTC_main(RTC_TypeDef* RTC_Handle) : _RTC_BaseN(NULL)
{
  _RTC_BaseN  = RTC_Handle;                                           // Не знаю зачем (RTC то один, других не бывает), но пусть будет
  _Alarm_A    = 0;                                                    // Будильник А
  _Alarm_B    = 0;                                                    // Будильник Б
}

RTC_main::~RTC_main(){};                                              // Всех убью, один останусь

// ---------------------- Сброс RTC ----------------------------------//
void RTC_main::Reset(void)
{
  RCC->APB1ENR |= RCC_APB1ENR_PWREN;                                  // Включить тактирование PWR и Backup
  RCC->AHB1ENR |= RCC_AHB1ENR_BKPSRAMEN;
  PWR->CR |= PWR_CR_DBP;                                              // Разрешить доступ к Backup области
  RCC->BDCR |= RCC_BDCR_BDRST;                                        // Сбросить Backup область
  RCC->BDCR &= ~RCC_BDCR_BDRST;
  PWR->CR &= ~PWR_CR_DBP;                                             // запретить доступ к Backup области
  RCC->APB1ENR &= ~RCC_APB1ENR_PWREN;                                 // Выключить тактирование PWR и Backup
  RCC->AHB1ENR &= ~RCC_AHB1ENR_BKPSRAMEN;
}

// --------------------- Инициализация RTC ---------------------------//
uint8_t RTC_main::Init(uint8_t RTC_Tact)
{
  _Irq = RTC_WKUP_IRQn;                                               // Это прерывание будем устанавливать

  PeripheralUnit::initIsr(_Irq);                                      // Следующие две строки из библиотеки прерываний
  PeripheralUnit::init();                                             //

  uint32_t StartUpCounter;                                            // Счётчик Таймаута

  /*
   * При тактировании от LSI и HSE часы не идут, даже при подключенной батарейке,
   * но сами часы проинициализированы. Необходимо только запустить тактирование
   * и часы пойдут, но время в регистрах будет то же, что и момент отключения питания.
   */
  if((RTC_Tact == RTC_LSI) && (RCC->BDCR & RCC_BDCR_RTCEN))
    RCC->CSR  |= RCC_CSR_LSION;                                       // Запуск при тактировании от LSI

  if((RTC_Tact == RTC_HSE) && (RCC->BDCR & RCC_BDCR_RTCEN))
    RCC->CFGR |= RCC_CFGR_RTCPRE_3;                                   // Установка делителя. Так как при отключении питания он сбрасывается

  if(!(RCC->BDCR & RCC_BDCR_RTCEN))                                   // Проверка RTC, если не включены, то инициализировать
  {
    RCC->APB1ENR  |= RCC_APB1ENR_PWREN;                               // Включить тактирование PWR и Backup
    RCC->AHB1ENR  |= RCC_AHB1ENR_BKPSRAMEN;
    PWR->CR       |= PWR_CR_DBP;                                      // Разрешить доступ к Backup области
    RCC->BDCR     |= RCC_BDCR_BDRST;                                  // Сбросить Backup область
    RCC->BDCR     &= ~RCC_BDCR_BDRST;
//
    switch (RTC_Tact)
    {
      // ---------- LSI ------------ //
      case RTC_LSI:                                                   // Внутренний генератор 32 000 Гц
        _Prediv_A = 0x7F;                                             // Предделитель RTC для LSI
        _Prediv_S = 0xF9;
        RCC->BDCR |= RCC_BDCR_RTCSEL_1;                               // Выбрать источник LSI
        RCC->BDCR |= RCC_BDCR_RTCEN;                                  // Подать тактирование
        RCC->CSR  |= RCC_CSR_LSION;                                   // Включаем LSI
        for (StartUpCounter = 0;; StartUpCounter++)                   // В цикле ждём определённое время ожидая запуска
        {
          if(RCC->CSR & RCC_CSR_LSIRDY) break;                        // Запуск есть, выходим из цикла
          if(StartUpCounter > 0x500000)                               // Время истекло,
          {
            RCC->CR &= ~RCC_CSR_LSION;                                // Отключаем LSI
            RCC->BDCR &= 0x00 << RCC_BDCR_RTCSEL_Pos;                 // Сросить выбор тактирования
            RCC->BDCR &= ~RCC_BDCR_RTCEN;                             // Отключить тактирование
            return false;                                             // Запуска нет, выходим из функции
          }
        }
        break;
      // ----------- LSE ----------- //
      case RTC_LSE:                                                   // Кварцевый генератор 32 768 Гц
        _Prediv_A = 0x7F;                                             // Делитель для кварца
        _Prediv_S = 0xFF;
        RCC->BDCR |= RCC_BDCR_RTCSEL_0;                               // Выбрать LSE источник (кварц 32768)
        RCC->BDCR |= RCC_BDCR_RTCEN;                                  // Подать тактирование
        RCC->BDCR |= RCC_BDCR_LSEON;                                  // Включить LSE
        for (StartUpCounter = 0;; StartUpCounter++)                   // В цикле ждём определённое время ожидая запуска
        {
          if(RCC->BDCR & RCC_BDCR_LSERDY) break;                      // Запуск есть, выходим из цикла
          if(StartUpCounter > 0x500000)                               // Время истекло,
          {
            RCC->CR &= ~RCC_BDCR_LSEON;                               // Отключаем LSE
            RCC->BDCR &= 0x00 << RCC_BDCR_RTCSEL_Pos;                 // Сбросить выбор тактирования
            RCC->BDCR &= ~RCC_BDCR_RTCEN;                             // Отключить тактирование
            return false;                                             // Запуска нет, выходим из функции
          }
        }
        break;
      // ----------- HSE ----------- //
      case RTC_HSE:                                                   // Основной кварцевый резонатор
        _Prediv_A = 0x7C;                                             // Предделитель RTC
        _Prediv_S = 0x1F3F;
        RCC->CFGR |= RCC_CFGR_RTCPRE_3;                               // Делим на 8, получаем тактирование 1МГц
        RCC->BDCR |= RCC_BDCR_RTCSEL_1 | RCC_BDCR_RTCSEL_0;           // Выбрать источник
        RCC->BDCR |= RCC_BDCR_RTCEN;                                  // Подать тактирование
        break;
      default:
        break;
    }
    RTC->WPR = 0xCA;                                                  // Выключаем защиту от записи
    RTC->WPR = 0x53;
    RTC->ISR |= RTC_ISR_INIT;                                         // Входим в режим редактирования
    for (StartUpCounter = 0;; StartUpCounter++)                       // В цикле ждём определённое время ожидая запуска
    {
      if(RTC->ISR & RTC_ISR_INITF) break;                             // Инициализация разрешена, выходим из цикла
      if(StartUpCounter > 0x500000)                                   // Время истекло,
      {
        RCC->CR &= ~RCC_CSR_LSION;                                    // Отключаем LSI
        RCC->BDCR &= ~RCC_BDCR_LSEON;                                 // Отключаем LSE
        RCC->BDCR &= 0x00 << RCC_BDCR_RTCSEL_Pos;                     // Отключаем выбор источника
        PWR->CR &= ~PWR_CR_DBP;                                       // Запретить доступ к Backup области
        RTC->WPR = 0xFF;                                              // Включаем защиту от записи
        RCC->BDCR &= ~RCC_BDCR_RTCEN;                                 // Отключить тактирование
        return false;                                                 // Запуска нет, выходим из функции
      }
    }
    RTC->PRER &= ~(0x7F << RTC_PRER_PREDIV_A_Pos);
    RTC->PRER |= _Prediv_A << RTC_PRER_PREDIV_A_Pos;                  // Настроить делител
    RTC->PRER &= ~(0x7FFF << RTC_PRER_PREDIV_S_Pos);
    RTC->PRER |= _Prediv_S << RTC_PRER_PREDIV_S_Pos;                  //
    RTC->ISR &= ~(RTC_ISR_INIT);                                      // Выходим из режима редактирования
    RTC->WPR = 0xFF;                                                  // Включаем защиту от записи
    PWR->CR &= ~PWR_CR_DBP;                                           // запретить доступ к Backup области
    _RTC_FreqSource = RTC_Tact;
  }
  return _RTC_FreqSource;
}

/*
 * Инициализация прерывания периодического счётчика
 * Счётчик считает секундные импульсы.
 * Timer - Количество секунд
 */
void RTC_main::setIT_WUCK(callback_IRQ ExternIT, uint16_t Timer)
{
  // Устанавливаем прерывание
  EXTI->IMR |= EXTI_IMR_MR22;                                         // Разрешаем прерывание от RTC
  EXTI->RTSR |= EXTI_RTSR_TR22;                                       // Прерывание по фронту

  PWR->CR |= PWR_CR_DBP;                                              // Разрешить доступ к Backup области
  RTC->WPR = 0xCA;                                                    // Выключаем защиту от записи
  RTC->WPR = 0x53;

  // Выключаем таймер
  RTC->CR &= ~RTC_CR_WUTE;                                            // Включаем таймер
  while(!(RTC->ISR & RTC_ISR_WUTWF)) {;}                              // Ждём, когда включится
  RTC->ISR &= ~RTC_ISR_WUTF;                                          // Сбрасываем флаг
  RTC->CR |= RTC_CR_WUTIE;                                            // Разрешаем прерывание от WUTE
  // Ставим делитель на нужный режим
  RTC->WUTR = Timer - 1; //2048; //4096;                              // Кидаем значение в счётчик
  RTC->CR &= ~RTC_CR_WUCKSEL;                                         // Сбрасываем биты, получаем деление на 16
  RTC->CR |= RTC_CR_WUCKSEL_2;                                        // Устанавливаем 3й бит, получаем секундные импульсы на входе счётчика

  RTC->CR |= RTC_CR_WUTE;                                             // Запускаем счётчик
  while(RTC->ISR & RTC_ISR_WUTWF) {;}                                 // Ждём запуска

  RTC->WPR = 0xFF;                                                    // Включаем защиту от записи
  PWR->CR &= ~PWR_CR_DBP;                                             // запретить доступ к Backup области

  _pfUserISR = ExternIT;                                              // Назначаем внешнее прерывание
  NVIC_EnableIRQ(RTC_WKUP_IRQn);                                      // Разрешаем прерывание от WKUP
}

/*
 * Установка даты.
 * Формат: setDate( ДД, ММ, ГГГГ, Н );
 *
 */
void RTC_main::setDate(uint8_t Date, uint8_t Month, uint16_t Year, uint8_t Week )
{
  uint16_t tempYear = Year%100;

  PWR->CR |= PWR_CR_DBP;                                              // Разрешить доступ к Backup области
  RTC->WPR = 0xCA;                                                    // Выключаем защиту от записи
  RTC->WPR = 0x53;
  RTC->ISR |= RTC_ISR_INIT;                                           // Входим в режим редактирования календаря
  while (!(RTC->ISR & RTC_ISR_INITF)){};                              // Ждем подтверждения входа в режим редактирования
  RTC->DR = ((tempYear/10) << RTC_DR_YT_Pos)  |
            ((tempYear%10) << RTC_DR_YU_Pos)  |
            (Week << RTC_DR_WDU_Pos)      |
            ((Month/10) << RTC_DR_MT_Pos) |
            ((Month%10) << RTC_DR_MU_Pos) |
            ((Date/10) << RTC_DR_DT_Pos)  |
            ((Date%10) << RTC_DR_DU_Pos) ;
  RTC->ISR &= ~(RTC_ISR_INIT);                                        // Выходим из режима редактирования
  RTC->WPR = 0xFF;                                                    // Включаем защиту от записи
  PWR->CR &= ~PWR_CR_DBP;                                             // запретить доступ к Backup области
}

/*
 * Установка времени
 * Формат: setTime( ЧЧ, ММ, СС );
 */
void RTC_main::setTime(uint8_t Hour, uint8_t Minute, uint8_t Sec)
{
  PWR->CR |= PWR_CR_DBP;                                              // Разрешить доступ к Backup области
  RTC->WPR = 0xCA;                                                    // Выключаем защиту от записи
  RTC->WPR = 0x53;
  RTC->ISR |= RTC_ISR_INIT;                                           // Входим в режим редактирования календаря
  while (!(RTC->ISR & RTC_ISR_INITF)){};                              // Ждем подтверждения входа в режим редактирования
  RTC->TR = ((Hour/10) << RTC_TR_HT_Pos)    |
            ((Hour%10) << RTC_TR_HU_Pos)    |
            ((Minute/10) << RTC_TR_MNT_Pos) |
            ((Minute%10) << RTC_TR_MNU_Pos) |
            ((Sec/10) << RTC_TR_ST_Pos)     |
            ((Sec%10) << RTC_TR_SU_Pos);
  RTC->ISR &= ~(RTC_ISR_INIT);                                        // Выходим из режима редактирования
  RTC->WPR = 0xFF;                                                    // Включаем защиту от записи
  PWR->CR &= ~PWR_CR_DBP;                                             // запретить доступ к Backup области
}

// Вернуть год
uint16_t  RTC_main::getYear(void)
{
  return ((RTC->DR & RTC_DR_YT) >> RTC_DR_YT_Pos) *10 + ((RTC->DR & RTC_DR_YU) >> RTC_DR_YU_Pos) + 2000;
}

// Вернуть день недели
uint8_t   RTC_main::getWeek(void)
{
  return ((RTC->DR & RTC_DR_WDU) >> RTC_DR_WDU_Pos);
}

// Вернуть месяц
uint8_t   RTC_main::getMonth(void)
{
  return ((RTC->DR & RTC_DR_MT) >> RTC_DR_MT_Pos) *10 + ((RTC->DR & RTC_DR_MU) >> RTC_DR_MU_Pos);
}

// Вернуть дату
uint8_t   RTC_main::getDate(void)
{
  return ((RTC->DR & RTC_DR_DT) >> RTC_DR_DT_Pos) *10 + ((RTC->DR & RTC_DR_DU) >> RTC_DR_DU_Pos);
}

// Вернуть час
uint8_t   RTC_main::getHour(void)
{
  return ((RTC->TR & RTC_TR_HT) >> RTC_TR_HT_Pos) *10 + ((RTC->TR & RTC_TR_HU) >> RTC_TR_HU_Pos);
}

// Вернуть минуты
uint8_t   RTC_main::getMinute(void)
{
  return ((RTC->TR & RTC_TR_MNT) >> RTC_TR_MNT_Pos) *10 + ((RTC->TR & RTC_TR_MNU) >> RTC_TR_MNU_Pos);
}

// Вернуть секунды
uint8_t   RTC_main::getSeconds(void)
{
  return ((RTC->TR & RTC_TR_ST) >> RTC_TR_ST_Pos) *10 + ((RTC->TR & RTC_TR_SU) >> RTC_TR_SU_Pos);
}

void RTC_main::getTime(void)
{
  uint32_t Temp = RTC->TR;
  Time.Hour    = ((Temp & RTC_TR_HT) >> RTC_TR_HT_Pos) *10 + ((Temp & RTC_TR_HU) >> RTC_TR_HU_Pos);
  Time.Minute  = ((Temp & RTC_TR_MNT) >> RTC_TR_MNT_Pos) *10 + ((Temp & RTC_TR_MNU) >> RTC_TR_MNU_Pos);
  Time.Sec     = ((Temp & RTC_TR_ST) >> RTC_TR_ST_Pos) *10 + ((Temp & RTC_TR_SU) >> RTC_TR_SU_Pos);
}

void RTC_main::getDates(void)
{
  uint32_t  Temp = RTC->DR;
  Dates.Date  = ((Temp & RTC_DR_DT) >> RTC_DR_DT_Pos) *10 + ((Temp & RTC_DR_DU) >> RTC_DR_DU_Pos);
  Dates.Month = ((Temp & RTC_DR_MT) >> RTC_DR_MT_Pos) *10 + ((Temp & RTC_DR_MU) >> RTC_DR_MU_Pos);
  Dates.Year  = ((Temp & RTC_DR_YT) >> RTC_DR_YT_Pos) *10 + ((Temp & RTC_DR_YU) >> RTC_DR_YU_Pos) + 2000;
  Dates.Week  = ((Temp & RTC_DR_WDU) >> RTC_DR_WDU_Pos);
}

/*
 * Установка будильника А
 */
void RTC_main::setAlarmA(Weekday Day, uint8_t Hour, uint8_t Minute, uint8_t Seconds, uint8_t Date)
{
  uint32_t Temp = 0;
  PWR->CR |= PWR_CR_DBP;                                              // Разрешить доступ к Backup области
  RTC->WPR = 0xCA;                                                    // Выключаем защиту от записи
  RTC->WPR = 0x53;
  RTC->CR   &= ~RTC_CR_ALRAE;
  while (!(RTC->ISR & RTC_ISR_ALRAWF)){};                              // Ждем подтверждения входа в режим редактирования

  Temp = ((Hour/10) << RTC_ALRMAR_HT_Pos)    |
         ((Hour%10) << RTC_ALRMAR_HU_Pos)    |
         ((Minute/10) << RTC_ALRMAR_MNT_Pos) |
         ((Minute%10) << RTC_ALRMAR_MNU_Pos) |
         ((Seconds/10) << RTC_ALRMAR_ST_Pos) |
         ((Seconds%10) << RTC_ALRMAR_SU_Pos);

  if(Day == Everyday)
  {
    Temp |= RTC_ALRMAR_MSK4;
  }
  else if(Day == ByDate)
  {
    Temp |= (((Date/10) << RTC_ALRMAR_DT_Pos) | ((Date%10) << RTC_ALRMAR_DU_Pos));
  }
  else
  {
    Temp |= ((1U << RTC_ALRMAR_WDSEL_Pos) | (Day << RTC_ALRMAR_DU_Pos));
  }

  RTC->ALRMAR = Temp;
  RTC->CR   |= RTC_CR_ALRAE;
  RTC->WPR = 0xFF;                                                    // Включаем защиту от записи
  PWR->CR &= ~PWR_CR_DBP;                                             // запретить доступ к Backup области
}

// Прерывание
void RTC_main::interruptHandler()
{
  if((_RTC_BaseN->ISR & RTC_ISR_WUTF) && (_RTC_BaseN->CR & RTC_CR_WUTIE))
  {
    PWR->CR |= PWR_CR_DBP;                                              // Разрешить доступ к Backup области
    RTC->ISR &= ~RTC_ISR_WUTF;
    EXTI->PR |= EXTI_PR_PR22;
    PWR->CR &= ~PWR_CR_DBP;                                             // запретить доступ к Backup области
    interruptFlag = true;
    if(_pfUserISR != NULL) _pfUserISR();
  }
}

//#endif
//****************************************************************************** 
//  ENF OF FILE 
//****************************************************************************** 

