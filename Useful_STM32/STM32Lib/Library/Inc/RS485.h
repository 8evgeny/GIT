/*
 * File:   main.h
 * Author: User
 *
 * Created on 18 сентября 2019 г., 13:05
 */

// Блокируем повторное включение этого модуля 
#ifndef RS485_H
#define	RS485_H

  //****************************************************************************** 
  //  Секция include: здесь подключаются заголовочные файлы используемых модулей 
  //****************************************************************************** 
#include <STM32.h>
#include <stdint.h> 
#include "UART.h"

  //****************************************************************************** 
  //  Секция определения констант 
  //****************************************************************************** 
  //#define MY_CONST1            1 
  //#define MY_CONST2            2 
  //#define ... 
#define RS485_MASTER  1
#define RS485_SLAVE   0

  //****************************************************************************** 
  //  Секция определения типов 
  //****************************************************************************** 

struct _Heading
{
  uint8_t StartByte = 0x96;
  uint8_t Address;
  uint8_t QuantBytes;
  uint8_t MaskCorrect;
  uint32_t Mask;
} __attribute__ ((__packed__));

  //****************************************************************************** 
  //  Секция определения глобальных переменных 
  //****************************************************************************** 
enum
{
  RS485_Empty             = -1,
  RS485_StartNoValid      = -2,
  RS485_StopNoValid       = -3,
  RS485_CRCNoValid        = -4,
  RS485_RxTimeOut         = -5,
  RS485_AddressNoValid    = -7,
  RS485_StructureNoValid  = -8
};


  //****************************************************************************** 
  //  Секция определения классов
  //****************************************************************************** 

class RS485: public uart
{
  public:
    RS485(USART_TypeDef *Usart, bool Alternate, GPIO_TypeDef *GPIO_Port, uint16_t GPIO_Pin, uint8_t Address) :
        uart::uart(Usart, Alternate)
    {
      _GPIO_Port = GPIO_Port;
      _GPIO_Pin = GPIO_Pin;
      _SetPin(_GPIO_Port, _GPIO_Pin, Output, Pull_Down);
      _GPIO_Port->BSRR |= (_GPIO_Pin << 16);
      _Address = Address;
      if(_Address) _Role = RS485_SLAVE; else _Role = RS485_MASTER;
    }

    size_t Send(uint8_t *Buffer, uint8_t Length, uint8_t TxAddr = 0x00);
    size_t Receive(uint8_t *Buffer);

  protected:

  private:
    static uint8_t crc8(const uint8_t *addr, uint8_t len);

    GPIO_TypeDef  *_GPIO_Port;
    uint16_t      _GPIO_Pin;
    uint8_t       *_DataBuff;                      // Указатель на буфер передачи

    // ----- Переменные для инициализации передатчика ----------
    bool          _Role = RS485_MASTER;                 // Роль назначенная устройству
    const uint8_t _StartByte = 0x96;              // Стартовый байт
    uint8_t       _Address;
    uint8_t       _QuantBytes;                    // Количество передаваемых байт
    uint32_t      _Mask;                          // Маска изменённых байт
    _Heading      Heading;
    uint8_t       _CRC_8;                         // Контрольная сумма
    const uint8_t _StopByte = 0xA9;               // Стоповый байт

    enum
    {
      MaskCRC = 0x10,
      MaskCorrect_0   = 0x01,
      MaskCorrect_1   = 0x02,
      MaskCorrect_2   = 0x04,
      MaskCorrect_3   = 0x08,
    };
};


// Закрывающий #endif к блокировке повторного включения 
#endif	/* MAIN_H */

//****************************************************************************** 
//  ENF OF FILE 
//****************************************************************************** /* 
