/*
 * TIM_Base.h
 *
 *  Created on: 6 апр. 2022 г.
 *      Author: User
 */

#ifndef INC_TIM_BASE_H_
#define INC_TIM_BASE_H_

#include "STM32.h"

typedef void (*callback_IRQ)(void);

class TIM_Base : public PeripheralUnit
{
  public:
    TIM_Base(TIM_TypeDef* timerHandle);                     // Конструктор
    virtual ~TIM_Base();
    void    Init(uint16_t Prescaler, uint16_t Counter);     // Инициализация прямой задачей предделителя и счётчика
    uint8_t InitSec(uint16_t Seconds);                      // Задаём в секундах от 1 до 6
    uint8_t InitMS(uint16_t Millis);                        // Задаём в миллисекундах от 1 до 65535
    uint8_t InitUS(uint16_t Micros);                        // Задаём в микросекундах от 10 до 65535
    void    Start(void);                                    // Запускаем таймер
    void    Stop(void);                                     // Останавливаем таймер
    void    SetOPM(bool OPM);                               // Включаем режим Одного импульса
    void    SetIRQ_Priority(uint8_t Priority);              // Устанавливаем приоритет прерывания
    void    SetCallbackIRQ(callback_IRQ ExternIT = NULL);   // Устанавливаем свой обработчик прерывания

  private:
    void            interruptHandler();
    TIM_TypeDef*    _TimBaseN;
    IRQn_Type       _Irq;
    bool            _isInitialized = false;
    void            (*_pfUserISR)(void) = NULL;

};

#endif /* INC_TIM_BASE_H_ */
