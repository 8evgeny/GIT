/*
 * EXTI_Int.cpp
 *
 *  Created on: 27 янв. 2022 г.
 *      Author: User
 */

#include <EXTI_Int.h>

//sExti_Flag Exti_Flag = {0};
static  uint32_t  _TimeStart;
static  uint32_t  _TimeStop;
static  uint8_t   _Exti_Trigger;
static  bool      _FlagHold = false;
static  uint8_t   _FirstEdge = EXTI_Trigger_Rising;
static  uint16_t  _IO_Pin;

EXTI_Int::EXTI_Int(GPIO_TypeDef *GPIO_Port, uint16_t Exti_Pin, uint8_t Exti_Trigger, uint8_t Edge_First)
{
  _GPIO_Port    = GPIO_Port;
  _Exti_Pin     = Exti_Pin;
  _Exti_Trigger = Exti_Trigger;

  RCC->APB2ENR  |= RCC_APB2ENR_SYSCFGEN;                    // Включаем тактирование системного регистра

  _SetPin(_GPIO_Port, _Exti_Pin, Input, Pull_Down);           // Переключаем пин на вход с подтяжкой вниз

  if(_GPIO_Port == GPIOA)      _Exti_Port = EXTI_Port_A;    // Определяемся какой порт будем инициализировать
  else if(_GPIO_Port == GPIOB) _Exti_Port = EXTI_Port_B;
  else if(_GPIO_Port == GPIOC) _Exti_Port = EXTI_Port_C;
  else if(_GPIO_Port == GPIOD) _Exti_Port = EXTI_Port_D;
  else if(_GPIO_Port == GPIOH) _Exti_Port = EXTI_Port_H;

  uint8_t  Position;
  uint16_t IO_Current   = 0x00U;

  // Узнаём номер позиции вывода
  for (Position = 0U; Position < 16; Position++)
  {
    _IO_Position = 0x01U << Position;
    IO_Current  = _Exti_Pin & _IO_Position;

    if(IO_Current == _IO_Position) break;                    // После выхода в переменной Position номер позиции вывода
  }
  _IO_Pin = _IO_Position;

  switch (Position)                                         // В зависимости от того, на какой вывод назначено прерывание,
  {                                                         // выбираем соответствующий обработчик.
    case 0:
    case 1:
      _IRQn = EXTI0_1_IRQn;
      break;
    case 2:
    case 3:
      _IRQn = EXTI2_3_IRQn;
      break;
    case 4:
    case 5: case 6: case 7: case 8: case 9:
    case 10: case 11: case 12: case 13: case 14: case 15:
      _IRQn = EXTI4_15_IRQn;
      break;
    default:
      break;
  }

  SYSCFG->EXTICR[Position/4] |= (_Exti_Port << ((Position & 3) * 4));       // В нужном нам регистре говорим с какого порта, какой пин будет работать

  // Программируем фронт на который у нас будет первая реакция
  if(((Exti_Trigger & EXTI_Trigger_Rising) == EXTI_Trigger_Rising) ||
      ((Exti_Trigger == EXTI_Trigger_All) && (Edge_First == EXTI_Trigger_Rising)))
    EXTI->RTSR |= (1U << Position);
  else
    EXTI->FTSR |= (1U << Position);

  EXTI->PR  |= (1U << Position);                                      // Сбрасываем на всякий пожарный флаг прерывания
  EXTI->IMR |= (1U << Position);                                      // Разрешаем прерывание от вывода
}

void EXTI_Int::Init(void)
{
//  PeripheralUnit::initIsr(_IRQn);                          // Следующие две строки из библиотеки прерываний
//  PeripheralUnit::init();                                 //

  NVIC_EnableIRQ(_IRQn);                                              // Разрешаем прерывание от внешних выводов
}

EXTI_Int::~EXTI_Int(){}

bool EXTI_Int::isClick(void)
{
  return 0;
}

bool EXTI_Int::isPressed(void)
{
  return _FlagHold;
}


uint32_t  EXTI_Int::GetHoldTime(void)
{
  _FlagHold = false;
  return _TimeStop;
}

bool EXTI_Int::getFlagHold(void)
{
  return _FlagHold;
}

void EXTI_Int::clearFlagHold(void)
{
  _FlagHold = false;
}

void EXTI_Int::setTrigger(uint8_t EXTI_Trigger)
{
  _Exti_Trigger = EXTI_Trigger;
}

void EXTI_Int::activateInt(void)
{
  EXTI->SWIER |= (1U << _IO_Position);
}

void EXTI_Int::setTimeStart(void)
{
  _TimeStart = millis();
}
//void EXTI_Int::setFirstEdge(uint8_t Edge)
//{
//  if(Edge != EXTI_Trigger_All) _FirstEdge = Edge;
//}

//void EXTI_Int::interruptHandler()
extern "C" void EXTI0_1_IRQHandler(void)
{
  if((EXTI->PR & EXTI_PR_PR0) && (EXTI->IMR & EXTI_IMR_IM0))
  {
    EXTI->PR |= EXTI_PR_PR0;
    _TimeStop = 0;

    if(_Exti_Trigger == EXTI_Trigger_All)
    {
      if(EXTI->RTSR & _IO_Pin)                                    // Преравание от переднего фронта разпешено?
      {
        EXTI->RTSR &= ~_IO_Pin;                                  // Запрещаем прерывание от переднего фронта
        EXTI->FTSR |= _IO_Pin;                                   // Разрешаем прерывание от заднего фронта
        if(_FirstEdge == EXTI_Trigger_Rising) _TimeStart = millis();
        else
        {
          _TimeStop = millis() - _TimeStart;
          _FlagHold = true;
        }
      }
      else if(EXTI->FTSR & _IO_Pin)                               // Если прерывание произошло по заднему фронту
      {
        EXTI->RTSR |= _IO_Pin;                                   // Разрешаем прерывание по переднему фронту
        EXTI->FTSR &= ~_IO_Pin;                                  // Запрещаем прерывание по заднему фронту
        if(_FirstEdge == EXTI_Trigger_Falling) _TimeStart = millis();
        else
        {
          _TimeStop = millis() - _TimeStart;
          _FlagHold = true;
        }
      }
    }
  }
}



