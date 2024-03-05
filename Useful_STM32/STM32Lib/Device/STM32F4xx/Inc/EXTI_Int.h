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

typedef struct
{
  union
  {
    uint16_t EXTI_Int;
    struct
    {
      unsigned EXTI_Pin_0  :1;
      unsigned EXTI_Pin_1  :1;
      unsigned EXTI_Pin_2  :1;
      unsigned EXTI_Pin_3  :1;
      unsigned EXTI_Pin_4  :1;
      unsigned EXTI_Pin_5  :1;
      unsigned EXTI_Pin_6  :1;
      unsigned EXTI_Pin_7  :1;
      unsigned EXTI_Pin_8  :1;
      unsigned EXTI_Pin_9  :1;
      unsigned EXTI_Pin_10 :1;
      unsigned EXTI_Pin_11 :1;
      unsigned EXTI_Pin_12 :1;
      unsigned EXTI_Pin_13 :1;
      unsigned EXTI_Pin_14 :1;
      unsigned EXTI_Pin_15 :1;
    };
  };
} sExti_Flag;

enum EXTI_Trigger
{
  EXTI_Trigger_Rising  = 0x01,
  EXTI_Trigger_Falling = 0x02,
  EXTI_Trigger_All     = 0x03
};

typedef void (*callback_IRQ)(void);

class EXTI_Int : public PeripheralUnit
{
  public:
    EXTI_Int(GPIO_TypeDef *GPIO_Port, uint16_t Exti_Pin, uint8_t Exti_Trigger, uint8_t Edge_First = EXTI_Trigger_Rising);
    virtual ~EXTI_Int();
    void    Init(void);
    uint32_t  GetHoldTime(void);
    bool      getFlagHold(void);
    void      clearFlagHold(void);
    void      setTrigger(uint8_t EXTI_Trigger);

  private:
    void          interruptHandler();
    bool          _isInitialized = false;
    bool          _FlagHold = false;
    void          (*_pfUserISR)(void) = NULL;
    GPIO_TypeDef  *_GPIO_Port;
    uint16_t      _Exti_Pin;
    uint8_t       _Exti_Trigger;
    uint8_t       _Exti_Port;
    IRQn_Type     _IRQn;
    uint8_t       _FirstEdge = EXTI_Trigger_Rising;
    uint16_t      _IO_Position  = 0x00U;
    uint32_t      _TimeStart;
    uint32_t      _TimeStop;
};

#endif /* INC_EXTI_INT_H_ */
