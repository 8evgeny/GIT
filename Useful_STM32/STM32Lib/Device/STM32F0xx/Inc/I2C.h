/*
 * I2C.h
 *
 *  Created on: 12 нояб. 2021 г.
 *      Author: User
 */

#ifndef INC_I2C_H_
#define INC_I2C_H_

//****************************************************************************** 
//  Секция include: здесь подключаются заголовочные файлы используемых модулей 
//****************************************************************************** 
#include <stdint.h> 
#include <STM32.h>
#include "gpio_main.h"

#define I2C_ADDRESS 0xA0
#define I2C_READ    0x00
#define I2C_WRITE   0x01

enum
{
  I2C_Standart_Mode,
  I2C_Fast_Mode,
  I2C_Fast_ModePlus
};

enum
{
  I2C_Master,
  I2C_Slave
};

class I2C
{
  public:
    I2C(I2C_TypeDef *I2C_Port, uint8_t Speed, uint8_t Mode, uint8_t Alternate);
    void send(uint8_t Address, uint8_t Data);

  private:
    uint8_t       _Alternate;
    uint8_t       _SpeedMode;
    uint8_t       _TransferMode;
    I2C_TypeDef   *_I2C_Port;
    IRQn_Type     _IRQn;
};

#endif /* INC_I2C_H_ */
