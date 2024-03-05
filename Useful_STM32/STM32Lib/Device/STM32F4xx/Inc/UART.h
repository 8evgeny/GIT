/*
 * UART.h
 *
 *  Created on: 14 апр. 2021 г.
 *      Author: User
 */

#ifndef UART_H_
#define UART_H_

#include "STM32.h"
#include "stdint.h"
#include "stdlib.h"
#include "RingFIFO.h"
#include "Print.h"

#define UART_TXBUFF_LENGHT     64
#define UART_RXBUFF_LENGHT     64

#define UART_DIV_SAMPLING16(_PCLK_, _BAUD_)            ((uint32_t)((((uint64_t)(_PCLK_))*25U)/(4U*((uint64_t)(_BAUD_)))))

#define UART_DIVMANT_SAMPLING16(_PCLK_, _BAUD_)        (UART_DIV_SAMPLING16((_PCLK_), (_BAUD_))/100U)
#define UART_DIVFRAQ_SAMPLING16(_PCLK_, _BAUD_)        ((((UART_DIV_SAMPLING16((_PCLK_), (_BAUD_)) - (UART_DIVMANT_SAMPLING16((_PCLK_), (_BAUD_)) * 100U)) * 16U) + 50U) / 100U)
/* UART BRR = mantissa + overflow + fraction
            = (UART DIVMANT << 4) + (UART DIVFRAQ & 0xF0) + (UART DIVFRAQ & 0x0FU) */
#define UART_BRR_SAMPLING16(_PCLK_, _BAUD_)            ((UART_DIVMANT_SAMPLING16((_PCLK_), (_BAUD_)) << 4U) + \
                                                        (UART_DIVFRAQ_SAMPLING16((_PCLK_), (_BAUD_)) & 0xF0U) + \
                                                        (UART_DIVFRAQ_SAMPLING16((_PCLK_), (_BAUD_)) & 0x0FU))

class uart : public Print
{
  public:
    uart(USART_TypeDef *Port, bool Alternate = false);

    void init(uint32_t BaudRate);

//    uint8_t Transmit(uint8_t *Buff, uint8_t Length);        // Не нужна. write заменяет полностью

    // --- Функции от доброго человека ---
    int16_t read();                                         // Прочитать символ из буфера приемника. Return: -1 - нет данных для чтения, 0..255 - прочитанный символ
    int16_t BytesToRead();                                  // Получить количество непрочитанных байт в буфере приемника
    int16_t BytesToWrite();                                 // Получить количество еще не отправленных байт в буфере передатчика
    void ReadBuffClear();                                   // Очистить буфер приемника
    void WriteBuffClear();                                  // Очистить буфер передатчика
    // -----------------------------------

    bool SetBuffSize(uint16_t Size);                        // Установить новый размер буфера

    // Функции возвращающие ошибки
    // Функции проверки окончания приёма/передачи устройством USART
    bool    isTxFinish(void)               { if((_USART_Port->SR & USART_SR_TXE) && (_USART_Port->SR & USART_SR_TC)) return true; else return false;}
    // ToDo доделать функцию isRxFinish
//    bool    isRxFinish(void)               { if((_USART_Port->SR & USART_SR_RXNE) && (_USART_Port->SR & USART_SR_)) return true; else return false;}
    // Функции из ардуины
    virtual size_t write(uint8_t);
    inline  size_t write(unsigned long n) { return write((uint8_t)n); }
    inline  size_t write(long n)          { return write((uint8_t)n); }
    inline  size_t write(unsigned int n)  { return write((uint8_t)n); }
    inline  size_t write(int n)           { return write((uint8_t)n); }

    using   Print::write; // используется для write(str) и write(buf, size) как Print

  private:
    uint32_t      _BaudRate;
    bool          _Alternate = 0;                                     // Включать альтернативные выводы или нет 0 - нет, 1 - да
    USART_TypeDef *_USART_Port;
    IRQn_Type      _IRQn;

    // Задел на будущее, если это вообще кому нибудь понадобится
    bool          _WordLength = 0;
    bool          _Parity = 0;
    bool          _StopBits = 0;

    uint16_t      _BuffLength = 64;                                  // Длина буфера по умолчанию
    uint8_t       *TxBuff;                                            // Указатель на буфер приёма
    uint8_t       *RxBuff;                                            // Указатель на буфер передачи

  protected:
    void TXEIEnable();
    void TXEIDisable();
    void RXNEIEnable();
    int16_t ring_get(RingBuff_t *fifo);
    int16_t ring_put(uint16_t BuffLen, RingBuff_t *fifo, uint8_t c);

};

#endif /* DEVICE_STM32F4XX_INC_UART_H_ */
