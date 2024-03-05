/*
 * TIM_Base.cpp
 *
 *  Created on: 6 апр. 2022 г.
 *      Author: User
 */

#include <TIM_Base.h>

TIM_Base::TIM_Base(TIM_TypeDef* timerHandle) : _TimBaseN(NULL)
{
  _TimBaseN = timerHandle;
}

TIM_Base::~TIM_Base(){}

uint8_t TIM_Base::InitSec(uint16_t Seconds)
{
  if((Seconds < 1) || (Seconds > 6)) return -1;
  Init(8399, (Seconds * 10000) - 1);
  return 0;
}

uint8_t TIM_Base::InitMS(uint16_t Millis)
{
  if((Millis <1) || (Millis > 65535)) return -1;

  if(Millis <= 65) Init(83, (Millis * 1000) - 1);
  else if(Millis <= 655)    Init(839,  (Millis * 100) - 1);
  else if(Millis <= 6553)   Init(8399, (Millis * 10)  - 1);
  else if(Millis <= 65535)  Init(8399, (Millis * 1)   - 1);
  return 0;
}

uint8_t TIM_Base::InitUS(uint16_t Micros)
{
  if((Micros < 10) || (Micros > 65535)) return -1;
  Init(83, Micros - 1);
  return 0;
}


void TIM_Base::Init(uint16_t Prescaler, uint16_t Counter)
{
  if(!_isInitialized)
  {
    if(_TimBaseN == TIM6)                                   // Для TIM6
    {
      RCC->APB1ENR |= RCC_APB1ENR_TIM6EN;                   // Запускаем тактирование
      _Irq = TIM6_DAC_IRQn;                                 // Запоминаем имя вектора прерывания
    }
    else if(_TimBaseN == TIM7)                              // То же самое для TIM7
    {
      RCC->APB1ENR |= RCC_APB1ENR_TIM7EN;
      _Irq = TIM7_IRQn;
    }

    PeripheralUnit::initIsr(_Irq);                          // Следующие две строки из библиотеки прерываний
    PeripheralUnit::init();                                 //

    NVIC_SetPriority(_Irq, 5);                              // Слегка понижаем приоритет прерывания от таймера
    NVIC_EnableIRQ(_Irq);                                   // Разрешаем прерывание от таймера
  }

  _TimBaseN->PSC = Prescaler;                               // Записываем значение предделителя
  _TimBaseN->ARR = Counter;                                 // В теневой регистр счётчика записываем количество тиков, что нужно посчитать
  _TimBaseN->CR1 |= TIM_CR1_ARPE;                           // Говорим, что нужна предзагрузка из теневого регистра
  _TimBaseN->EGR |= TIM_EGR_UG;                             // Заставляем проинициализировать предделитель и счётчик

  _TimBaseN->SR     = 0;                                    // Сбрасываем флаги
  _TimBaseN->DIER   |= TIM_DIER_UIE;                        // Разрешаем прерывания от переполнения

  _isInitialized = true;                                    //

}

void TIM_Base::SetCallbackIRQ(callback_IRQ ExternIT){ _pfUserISR = ExternIT; }

void TIM_Base::Start(void){ _TimBaseN->CR1 |= TIM_CR1_CEN; }
void TIM_Base::Stop(void){ _TimBaseN->CR1 &= ~TIM_CR1_CEN; }

void TIM_Base::SetIRQ_Priority(uint8_t Priority)
{
  NVIC_DisableIRQ(_Irq);
  NVIC_SetPriority(_Irq, Priority);
  NVIC_EnableIRQ(_Irq);
}

void TIM_Base::SetOPM(bool OPM)
{
  if(OPM) _TimBaseN->CR1 |= TIM_CR1_OPM; else _TimBaseN->CR1 &= ~TIM_CR1_OPM;
}

void TIM_Base::interruptHandler()
{
  if((_TimBaseN->SR & TIM_SR_UIF) && (_TimBaseN->DIER & TIM_DIER_UIE))
  {
    _TimBaseN->SR &= ~TIM_SR_UIF;
    interruptFlag = true;
    if(_pfUserISR != NULL) _pfUserISR();
  }
}
