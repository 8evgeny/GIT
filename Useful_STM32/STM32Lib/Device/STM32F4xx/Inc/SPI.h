/*
 * UART.h
 *
 *  Created on: 14 апр. 2021 г.
 *      Author: User
 *
 *
 */

#ifndef SPI_H_
#define SPI_H_

#include "STM32.h"

#define MASTER_MODE  0x08

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

enum DataSize
{
  SPI_DataSize_8    = 0, //8-bit
  SPI_DataSize_16   = 1  //16-bit
};

enum MasterSlave
{
  SPI_Slave   = 0,
  SPI_Master  = 1
};

/*
 * Режимы SPI
 */
enum ModeTransmit
{
  SPI_FullDuplexMaster    = 0b1100,
  SPI_FullDuplexSlave     = 0b0100,
  SPI_HalfDuplexMaster    = 0b1000,
  SPI_HalfDuplexSlave     = 0b0000,
  SPI_ReceiveOnlyMaster   = 0b1010,
  SPI_ReceiveOnlySlave    = 0b0010,
  SPI_TransmitOnlyMaster  = 0b1011,
  SPI_TransmitOnlySlave   = 0b0011
};                          //|||+-- Передача-1 (Режим только передачи/приёма)  _TransmitOnly
                            //||+--- Только передача/приём-1                    _TransRecOnly
                            //|+---- Дуплекс-1, полудуплекс-0                   _FullDuplex
                            //+----- Ведущий-1, ведомый-0                       _MasterSlave
/*
 * Фаза синхронизации
 */
enum Faze
{
  SPI_Faze_0 = 0x00,
  SPI_Faze_1 = 0x01,
  SPI_Faze_2 = 0x02,
  SPI_Faze_3 = 0x03
};

class SPI
{
  public:
    SPI(SPI_TypeDef *Port, bool Alternate = false); // Port - порт SPI,
    void    init(uint8_t Div, bool DataSize, uint8_t TransMode);
    void    init(uint8_t Div, bool DataSize);

    size_t  Transmit(uint16_t Data);                                                       // Работает на всех режимах
    size_t  Transmit(uint8_t *Buff, uint16_t Length);                                      // Работает на всех режимах
    size_t  Transmit(uint16_t *Buff, uint16_t Length);                                     // Работает на всех режимах

    size_t  Receive(void);
    size_t  Receive(uint8_t *Buff, uint16_t Length);                                        // Проверено в ведущем полудуплексе
    size_t  Receive(uint16_t *Buff, uint16_t Length);

    size_t  TransmitReceive(uint16_t Data);
    size_t  TransmitReceive(uint16_t *TxBuff, uint16_t *RxBuff, uint16_t Length);

    // Функции с использованием прерывания
    size_t  Receive_IT(void);
    size_t  Receive_IT(uint8_t *Buff, uint16_t Length);
    size_t  Receive_IT(uint16_t *Buff, uint16_t Length);

    // Служебные функции
    void    setMasterSlave(bool Selection);
    void    setDataSize(uint8_t DataSize);
    void    setFazeMode(uint8_t Faze);

    bool    getFlagBSY(void) {if(_SPI_Port->SR & SPI_SR_BSY)  return true; else return false;}
    bool    getFlagTXE(void) {if(_SPI_Port->SR & SPI_SR_TXE)  return true; else return false;}
    bool    getFlagRXNE(void){if(_SPI_Port->SR & SPI_SR_RXNE) return true; else return false;}

  private:
    uint8_t       _SpiDiv;
    uint8_t       _SpiFaze;
    bool          _Alternate = 0;                                     // Включать альтернативные выводы или нет 0 - нет, 1 - да
    uint8_t       _TransMode = SPI_FullDuplexMaster;
    bool          _MSB = 0;                                           // Старший бит вперёд, по умолчанию
    bool          _DataSize;
    SPI_TypeDef   *_SPI_Port;
    IRQn_Type      _IRQn;
    uint16_t      _GPIO_Set;                                          // Пины MOSI, MISO которые будут инициализироваться
    uint16_t      _GPIO_Clk;                                          // Пины CLK


  protected:

};

#endif /* DEVICE_STM32F4XX_INC_UART_H_ */
