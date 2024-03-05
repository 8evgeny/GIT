/*
 * TIM_Base.cpp
 *
 *  Created on: 6 апр. 2022 г.
 *      Author: User
 */

#include <TIM_Base.h>

extern uint32_t vectorTable_RAM[];
void (*pfUserISR)(void) = NULL;

TIM_Base::TIM_Base(uint8_t Tim_Base)
{
  switch (Tim_Base)
  {
    case Tim_Base_6:
      _TimBaseN = TIM6;
      _TimIrq = TIM6_IRQn;
      break;
    case Tim_Base_7:
      _TimBaseN = TIM7;
      _TimIrq = TIM7_IRQn;
      break;
    default:
      break;
  }
}

void TIM_Base::Init(uint16_t Prescaler, uint16_t Counter, callback_IRQ ExternIT)
{
  if(_TimBaseN == TIM6) RCC->APB1ENR |= RCC_APB1ENR_TIM6EN;
  else if (_TimBaseN == TIM7) RCC->APB1ENR |= RCC_APB1ENR_TIM7EN;

  _TimBaseN->PSC = Prescaler;
  _TimBaseN->ARR = Counter;

  if(ExternIT != NULL)
  {
    __disable_irq();
    vectorTable_RAM[_TimIrq + 16] = (uint32_t)myISR_TIM;       // Подменяем на свой вектор прерывания
    __enable_irq();
    pfUserISR = ExternIT;                                             // Ссылка на наш, внешний обработчик прерывания
    _TimBaseN->DIER |= TIM_DIER_UIE;                                  // Разрешаем прерывания от переполнения
    NVIC_EnableIRQ(_TimIrq);
  }

  _TimBaseN->SR   = 0;                                                // Сбрасываем флаги
  _TimBaseN->CR1  |= TIM_CR1_CEN;
}

void TIM_Base::myISR_TIM(void)
{
  TIM6->SR &= ~TIM_SR_UIF;
//  if(_TimBaseN->SR & TIM_SR_UIF)
  {
//    _TimBaseN->SR &= ~TIM_SR_UIF;
    pfUserISR();
  }
}
