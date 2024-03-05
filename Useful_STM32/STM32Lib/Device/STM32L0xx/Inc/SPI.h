/*
 * UART.h
 *
 *  Created on: 14 апр. 2021 г.
 *      Author: User
 *
 *  Скорее всего придётся делать блокирующие функции без использования фифо
 */

#ifndef SPI_H_
#define SPI_H_

#include "STM32.h"
#include "stdint.h"
#include "stdio.h"

#define SPI_TXBUFF_LENGHT     64
#define SPI_RXBUFF_LENGHT     64

enum SPI_Div
{
  SPI_Div_2   = 0b000,
  SPI_Div_4   = 0b001,
  SPI_Div_8   = 0b010,
  SPI_Div_16  = 0b011,
  SPI_Div_32  = 0b100,
  SPI_Div_64  = 0b101,
  SPI_Div_128 = 0b110,
  SPI_Div_256 = 0b111
};

enum SPI_Mode
{
  Mode_0,
  Mode_1,
  Mode_2,
  Mode_3
};

enum DataSize
{
  SPI_DataSize_8    = 0b0, //8-bit
  SPI_DataSize_16   = 0b1  //16-bit
};

enum MasterSlave
{
  SPI_Slave   = 0,
  SPI_Master  = 1
};

class SPI
{
  public:
    SPI(SPI_TypeDef *Port, bool Alternate = false);                             // Port - порт SPI,
    void init(uint8_t Div, uint8_t DataSize);                                   // Не дописано прерывание

//    size_t Transmit(uint16_t Data);                                             // Не дописано uint8_t
//    size_t Transmit(uint8_t *Buff, uint8_t Length);                             // Не проверено
//    size_t Transmit(uint16_t *Buff, uint8_t Length);                            // Не проверено

//    size_t Receive(void);                                                       // Не дописано uint8, uint16
//    size_t Receive(uint8_t *Buff, uint8_t Length);
//    size_t Receive(uint16_t *Buff, uint8_t Length);

    size_t TransmitReceive(uint16_t Data);                                      // Работает
//    size_t TransmitReceive(uint8_t Data);                                      // Работает
    void TransmitReceive(uint16_t *TxBuff, uint16_t *RxBuff, uint8_t Length);   // Работает
    void TransmitReceive(uint8_t *TxBuff, uint8_t *RxBuff, uint8_t Length);     // Работает

    void   SetMasterSlave(bool Selection);
    void   SetDataSize(uint8_t DataSize);
    void   SetMode(uint8_t Mode);

  private:
    uint32_t      _SpiDiv;
    bool          _Alternate = 0;                                               // Включать альтернативные выводы или нет 0 - нет, 1 - да
    SPI_TypeDef   *_SPI_Port;
    IRQn_Type      _IRQn;
    uint8_t       _DataSize;
    uint8_t       _Mode;

  protected:

};

#endif /* DEVICE_STM32F4XX_INC_UART_H_ */
