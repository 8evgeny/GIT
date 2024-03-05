/*
 * SPI.h
 *
 *  Created on: 14 апр. 2021 г.
 *      Author: User
 */

#include "SPI.h"

void Test(void);


SPI::SPI(SPI_TypeDef *Port, bool Alternate)
{
  _SPI_Port     = Port;
  _Alternate    = Alternate;
  _SpiDiv       = SPI_Div_256;
  _DataSize     = SPI_DataSize_8;
  _Mode         = Mode_0;

  if(Port == SPI1) _IRQn = SPI1_IRQn;
  else if(Port == SPI2) _IRQn = SPI2_IRQn;
}

/*
 * Инициализация
 *
 * Следует учесть, что не на всех кристаллах есть возможность передавать от 4х до 16ти бит.
 * На некоторых можно передать только или 8 или 16
 *
 */

void SPI::init(uint8_t Div, uint8_t DataSize = SPI_DataSize_8)
{
  _SpiDiv   = Div;
  _DataSize = DataSize;

  _SetSpeed(Speed_High);                                                                  // Все пины инициализируем на большую частоту.

  // ШАГ 1 и 2 - Инициализируем тактовый генератор и порт ввода вывода SPI
  if(_SPI_Port == SPI1)
  {
    RCC->APB2ENR |= RCC_APB2ENR_SPI1EN;                                                   // Запускаем тактовый генератор для SPI порта
    _SetPull(Pull_Up);
    if(!_Alternate)
      _SetPinAlternate(GPIOA, GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7, GPIO_AF0_SPI1);       // Инициализируем выводы SPI
    else
      _SetPinAlternate(GPIOB, GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5, GPIO_AF0_SPI1);       // Инициализируем альтернативные выводы SPI
  }

  if(_SPI_Port == SPI2)
  {
    RCC->APB2ENR |= RCC_APB1ENR_SPI2EN;
    _SetPull(Pull_Up);
    if(!_Alternate)
      _SetPinAlternate(GPIOB, GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15, GPIO_AF0_SPI2);
    else
      _SetPinAlternate(GPIOB, GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15, GPIO_AF0_SPI2);    // У этого порта альтернативы нет
  }

  _SetSpeed(Speed_Low);

  // Шаг 3 - Инициализируем порт
  _SPI_Port->CR1  &= ~(0b111 << SPI_CR1_BR_Pos);                                          // Сбрасываем регистр скорости предачи
  _SPI_Port->CR1  |= (_SpiDiv << SPI_CR1_BR_Pos);                                         // Устанавливаем скорость предачи
  _SPI_Port->CR1  &= ~SPI_CR1_LSBFIRST;                                                   // Старший бит идёт первым
  _SPI_Port->CR1  |= SPI_CR1_SSM;                                                         // Программное управление SS
  _SPI_Port->CR1  |= SPI_CR1_SSI;                                                         // SS в высоком состоянии
  _SPI_Port->CR1  |= SPI_CR1_MSTR;                                                        // Режим Master (ведущий)
  SetMode(_Mode);                                                                         // Режим работы по умолчанию 0

  _SPI_Port->CR1  |= SPI_CR1_DFF;                                                         //
  _SPI_Port->CR1  &= ~((0b1 ^ _DataSize) << SPI_CR1_DFF_Pos);                             // Устанавливаем количество передаваемых бит. По умолчанию 8

  _SPI_Port->CR1  |= SPI_CR1_SPE;                                                         // Включаем SPI
}

// ----------------- Установка необходимого режима ---------------------------
void SPI::SetMode(uint8_t Mode)
{
  _Mode = Mode;
  _SPI_Port->CR1 &= ~(SPI_CR1_CPOL | SPI_CR1_CPHA);
  switch (_Mode)
  {
    case Mode_0:
      _SPI_Port->CR1 |= (0<<SPI_CR1_CPOL_Pos | 0<<SPI_CR1_CPHA_Pos);
      break;
    case Mode_1:
      _SPI_Port->CR1 |= (0<<SPI_CR1_CPOL_Pos | 1<<SPI_CR1_CPHA_Pos);
      break;
    case Mode_2:
      _SPI_Port->CR1 |= (1<<SPI_CR1_CPOL_Pos | 0<<SPI_CR1_CPHA_Pos);
      break;
    case Mode_3:
      _SPI_Port->CR1 |= (1<<SPI_CR1_CPOL_Pos | 1<<SPI_CR1_CPHA_Pos);
      break;
    default:
      break;
  }
}
/*
 * Функции передачи
 */
/*
// ------- Фунция передачи одного байта --------

size_t SPI::Transmit(uint16_t Data)
{
  while(!(_SPI_Port->SR & SPI_SR_TXE)){};                                                 // Ждем, пока не освободится буфер передатчика
  if(_DataSize > 8)
  {
//    _SPI_Port->CR2 &= ~SPI_CR2_FRXTH;                                                     // Событие RXNE возникает при при передаче 8-16 бит
    _SPI_Port->DR = (uint16_t) Data;                                                      // Заполняем буфер передатчика
  }
  else
  {
//    _SPI_Port->CR2 |= SPI_CR2_FRXTH;                                                      // Событие RXNE возникает при при передаче 4-8 бит
    *((__IO uint8_t *)&_SPI_Port->DR) = (uint8_t) Data;
  }
  while(_SPI_Port->SR & SPI_SR_TXE){};                                                 // Ждём, пока не освободится FIFO передатчика
  return 0;
}

// ------ Функция передачи 8ми и меньше разрядного буфера -------

size_t SPI::Transmit(uint8_t *Buff, uint8_t Length)
{
  uint8_t _Lenght = Length;
  uint8_t *TxBuffPtr;
  TxBuffPtr = (uint8_t*) Buff;

//  _SPI_Port->CR2 |= SPI_CR2_FRXTH;                                                        // Событие RXNE возникает при при передаче 4-8 бит
  while (_Lenght > 0U)
  {
    while (!(_SPI_Port->SR & SPI_SR_TXE)){};                                              // Ждем, пока не освободится буфер передатчика
    *((__IO uint8_t*) &_SPI_Port->DR) = *((uint8_t*) TxBuffPtr);
    TxBuffPtr += sizeof(uint8_t);
    _Lenght--;
  }
  while(_SPI_Port->SR & SPI_SR_TXE){};                                                 // Ждём, пока не освободится FIFO передатчика
  return 0;
}

// ------ Функция передачи буфера больше 8 разрядов -----------
size_t SPI::Transmit(uint16_t *Buff, uint8_t Length)
{
  uint8_t _Lenght = Length;
  uint8_t *TxBuffPtr;
  TxBuffPtr = (uint8_t*) Buff;

  while (_Lenght > 0U)
  {
    while (!(_SPI_Port->SR & SPI_SR_TXE)){};                                              // Ждем, пока не освободится буфер передатчика
    _SPI_Port->DR = *((uint16_t*) TxBuffPtr);
    TxBuffPtr += sizeof(uint16_t);
    _Lenght--;
  }
  while(_SPI_Port->SR & SPI_SR_TXE){};                                                 // Ждём, пока не освободится FIFO передатчика
  return 0;
}
*/
/*
 * Функции приёма
 */
/*
size_t SPI::Receive(void)
{
  while(!(_SPI_Port->SR & SPI_SR_TXE)){};                                                 // Ждем, пока не освободится передатчмк
  _SPI_Port->DR = 0;                                                                      // Запускаем обмен
  while(!(_SPI_Port->SR & SPI_SR_RXNE)){};                                                // Ждем, пока не появится новое значение в буфере приемника
  return _SPI_Port->DR;                                                                   // Возвращаем значение буфера приемника
}

size_t SPI::Receive(uint16_t *Buff, uint8_t Length)
{
  return 0;
}
*/
size_t SPI::TransmitReceive(uint16_t Data)
{
  while(!(_SPI_Port->SR & SPI_SR_TXE)){};                                                 // Ждем, пока не освободится передатчмк
  _SPI_Port->DR = (uint16_t) Data;                                                                   // Запускаем обмен
  while(!(_SPI_Port->SR & SPI_SR_RXNE)){};                                                // Ждем, пока не появится новое значение в буфере приемника
  return (uint16_t) _SPI_Port->DR;                                                                   // Возвращаем значение буфера приемника
}

/*size_t SPI::TransmitReceive(uint8_t Data)
{
  while(!(_SPI_Port->SR & SPI_SR_TXE)){};                                                 // Ждем, пока не освободится передатчмк
  _SPI_Port->DR = (uint8_t) Data;                                                                   // Запускаем обмен
  while(!(_SPI_Port->SR & SPI_SR_RXNE)){};                                                // Ждем, пока не появится новое значение в буфере приемника
  return (uint8_t) _SPI_Port->DR;                                                                   // Возвращаем значение буфера приемника
}*/

void SPI::TransmitReceive(uint16_t *TxBuff, uint16_t *RxBuff, uint8_t Length)
{
  Length = Length / sizeof(TxBuff[0]);                                                    // Общую длину делим на размер элемента
  for(uint8_t I=0; I<Length; I++)
  {
    while(!(_SPI_Port->SR & SPI_SR_TXE)){};                                                 // Ждем, пока не освободится передатчмк
    _SPI_Port->DR = (uint16_t) TxBuff[I];                                                                   // Запускаем обмен
    while(!(_SPI_Port->SR & SPI_SR_RXNE)){};                                                // Ждем, пока не появится новое значение в буфере приемника
    RxBuff[I] = (uint16_t) _SPI_Port->DR;
  }
}

void SPI::TransmitReceive(uint8_t *TxBuff, uint8_t *RxBuff, uint8_t Length)
{
  for(uint8_t I=0; I<Length; I++)
  {
    while(!(_SPI_Port->SR & SPI_SR_TXE)){};                                                 // Ждем, пока не освободится передатчмк
    _SPI_Port->DR = (uint8_t) TxBuff[I];                                                                   // Запускаем обмен
    while(!(_SPI_Port->SR & SPI_SR_RXNE)){};                                                // Ждем, пока не появится новое значение в буфере приемника
    RxBuff[I] = (uint8_t) _SPI_Port->DR;
  }
}

/*
 * Устанавливаем режим Master/Slave
 */

void   SPI::SetMasterSlave(bool Selection)
{
  while((_SPI_Port->SR & SPI_SR_TXE) && (_SPI_Port->SR & SPI_SR_RXNE)){};              // Ждём, пока не освободится FIFO передатчика и приёмника
  if(Selection)
    _SPI_Port->CR1 |= SPI_CR1_MSTR;                                                       // Если мастер, переключаем на мастер
  else
    _SPI_Port->CR1 &= ~SPI_CR1_MSTR;                                                      // Иначе - ведомый
}

void SPI::SetDataSize(uint8_t DataSize)
{
  while((_SPI_Port->SR & SPI_SR_TXE) && (_SPI_Port->SR & SPI_SR_RXNE)){};              // Ждём, пока не освободится FIFO передатчика и приёмника
  _DataSize = DataSize;
  _SPI_Port->CR1  |= SPI_CR1_DFF;                                                         //
  _SPI_Port->CR1  &= ~((0b1 ^ _DataSize) << SPI_CR1_DFF_Pos);                             // Устанавливаем количество передаваемых бит. По умолчанию 8
}

/*
 * Прерывания
 */
/* Пока работаем без них
extern "C"  void SPI1_IRQHandler(void)
{
//  static uint16_t tmp;

  //Если прерывание приема разрешено и что-то получили
//  if((UART4->SR & USART_SR_RXNE) && (UART4->CR1 & USART_CR1_RXNEIE))
//    {
//      tmp = UART4->DR;
//      RingBuffPut(&rx_fifo, tmp);
//    }

//  if((UART4->SR & USART_SR_TXE) && (UART4->CR1 & USART_CR1_TXEIE))              // Если передачтик пуст
//  {
//    if(RingBuffNumOfItems(&tx_fifo) > 0)                                        // если есть, что передавать, передаём
//    {
//      tmp = RingBuffGet(&tx_fifo);
//      UART4->DR = (uint8_t)(tmp & 0xFF);
//    }
//    else                                                                        // если передавать нечего
//    {
//      UART4->CR1 &= ~USART_CR1_TXEIE;                                           // отключаем прерывание
//    }
//  }
}

extern "C"  void SPI2_IRQHandler(void)
{
}
*/

