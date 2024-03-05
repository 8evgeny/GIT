/*
 * TIM_Base.h
 *
 *  Created on: 6 апр. 2022 г.
 *      Author: User
 */

#ifndef INC_TIM_BASE_H_
#define INC_TIM_BASE_H_

#include "STM32.h"
#include "stdint.h"
#include "cstddef"

enum
{
  Tim_Base_6,
  Tim_Base_7
};

typedef void (*callback_IRQ)(void);

class TIM_Base
{
  public:
    TIM_Base(uint8_t Tim_Base);
    void Init(uint16_t Prescaler, uint16_t Counter, callback_IRQ ExternIT);
    callback_IRQ  callback_TIM_Base = NULL;

  private:
    TIM_TypeDef*    _TimBaseN;
    uint16_t        _Prescaler;
    uint16_t        _Counter;
    static void myISR_TIM(void);
//    void myISR_TIM(void);
    IRQn_Type      _TimIrq;

};

#endif /* INC_TIM_BASE_H_ */
