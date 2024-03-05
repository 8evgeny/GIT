/*
 * SPI_Soft.h
 *
 *  Created on: 16 июн. 2022 г.
 *      Author: User
 */

#ifndef SPI_SOFT_H_
#define SPI_SOFT_H_

#include "STM32.h"

enum DataSize
{
  Data_8bit,
  Data_16bit
};

enum Mode
{
  Mode_Master,
  Mode_Slave
};

class SPI
{
  public:
    SPI(GPIO_TypeDef* GPIO_MOSI, uint16_t GPIO_PIN_MOSI,
             GPIO_TypeDef* GPIO_MISO, uint16_t GPIO_PIN_MISO,
             GPIO_TypeDef* GPIO_CLK, uint16_t GPIO_PIN_CLK);
    void    Init(uint8_t DataSize, uint8_t Mode);
    void    Transmit(uint16_t Data);
    size_t  Receive(uint8_t* Buff, uint8_t Length);
    bool    getFlagBSY(void){return false;};

  private:
    GPIO_TypeDef*   _GPIO_MOSI;
    uint16_t        _GPIO_PIN_MOSI;
    GPIO_TypeDef*   _GPIO_MISO;
    uint16_t        _GPIO_PIN_MISO;
    GPIO_TypeDef*   _GPIO_CLK;
    uint16_t        _GPIO_PIN_CLK;
    uint8_t         _dataSize;
    bool            _FlagBSY;

    void            write(uint8_t Data);

  protected:

};

#endif /* SPI_SOFT_H_ */
