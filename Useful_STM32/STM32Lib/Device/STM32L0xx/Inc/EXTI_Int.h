/*
 * EXTI_Int.h
 *
 *  Created on: 27 янв. 2022 г.
 *      Author: User
 */

#ifndef INC_EXTI_INT_H_
#define INC_EXTI_INT_H_

#include "STM32.h"

enum EXTI_Port
{
  EXTI_Port_A = 0b0000,
  EXTI_Port_B = 0b0001,
  EXTI_Port_C = 0b0010,
  EXTI_Port_D = 0b0011,
  EXTI_Port_E = 0b0100,
  EXTI_Port_F = 0b0101,
  EXTI_Port_G = 0b0110,
  EXTI_Port_H = 0b0111,
  EXTI_Port_I = 0b1000
};

enum EXTI_Trigger
{
  EXTI_Trigger_Rising  = 0x01,
  EXTI_Trigger_Falling = 0x02,
  EXTI_Trigger_All     = 0x03
};

//typedef void (*callback_IRQ)(void);

class EXTI_Int //: public PeripheralUnit
{
  public:
    EXTI_Int(GPIO_TypeDef *GPIO_Port, uint16_t Exti_Pin, uint8_t Exti_Trigger, uint8_t Edge_First = EXTI_Trigger_Rising);
    virtual   ~EXTI_Int();
    void      Init(void);
    uint32_t  GetHoldTime(void);
    bool      getFlagHold(void);
    void      clearFlagHold(void);
    void      setTrigger(uint8_t EXTI_Trigger);
    bool      isClick(void);
    bool      isPressed(void);
    void      activateInt(void);
    void      setTimeStart(void);

  private:
    bool          _isInitialized = false;
    void          (*_pfUserISR)(void) = NULL;
    GPIO_TypeDef  *_GPIO_Port;
    uint16_t      _Exti_Pin;
    uint8_t       _Exti_Port;
    IRQn_Type     _IRQn;
    uint16_t      _IO_Position  = 0x00U;
};

#endif /* INC_EXTI_INT_H_ */
