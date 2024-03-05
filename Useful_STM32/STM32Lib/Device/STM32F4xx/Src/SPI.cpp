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
  _DataSize     = 0;
  _SpiFaze      = SPI_Faze_0;

  if(Port == SPI1) _IRQn = SPI1_IRQn;
  else if(Port == SPI2) _IRQn = SPI2_IRQn;
  else if(Port == SPI3) _IRQn = SPI3_IRQn;
}

/*
 * Инициализация
 *
 * Следует учесть, что не на всех кристаллах есть возможность передавать от 4х до 16ти бит.
 * На некоторых можно передать только или 8 или 16
 *
 */
void SPI::init(uint8_t Div, bool DataSize, uint8_t TransMode)
{
  _TransMode = TransMode;
  init(Div, DataSize);
}


void SPI::init(uint8_t Div, bool DataSize = SPI_DataSize_8)
{
  _SpiDiv   = Div;
  _DataSize = DataSize;


  _SetSpeed(Speed_WeryHigh);                                                                  // Все пины инициализируем на большую частоту.
  _SetPull(No_Pull);

  _GPIO_Set = 0;
  uint32_t CMD_CR1 = 0;

  // ШАГ 1 и 2 - Инициализируем тактовый генератор и порт ввода вывода SPI


  if(_SPI_Port == SPI1)
  {
    RCC->APB2ENR |= RCC_APB2ENR_SPI1EN;                                                   // Запускаем тактовый генератор для порта SPI1
    if(!_Alternate)
    {
      if((_TransMode == SPI_FullDuplexMaster) || (_TransMode == SPI_FullDuplexSlave)) _GPIO_Set = GPIO_PIN_6 | GPIO_PIN_7;
      else if((_TransMode == SPI_HalfDuplexMaster) || (_TransMode == SPI_ReceiveOnlySlave) || (_TransMode == SPI_TransmitOnlyMaster)) _GPIO_Set = GPIO_PIN_7;
      else if((_TransMode == SPI_HalfDuplexSlave) || (_TransMode == SPI_ReceiveOnlyMaster) | (_TransMode == SPI_TransmitOnlySlave)) _GPIO_Set = GPIO_PIN_6;
      _GPIO_Clk = GPIO_PIN_5;
      _SetPinAlternate(GPIOA, _GPIO_Clk | _GPIO_Set,   GPIO_AF5_SPI1);       // SCK, MISO, MOSI
    }
    else
    {
      if((_TransMode == SPI_FullDuplexMaster) || (_TransMode == SPI_FullDuplexSlave)) _GPIO_Set = GPIO_PIN_4 | GPIO_PIN_5;
      else if((_TransMode == SPI_HalfDuplexMaster) || (_TransMode == SPI_ReceiveOnlySlave) || (_TransMode == SPI_TransmitOnlyMaster)) _GPIO_Set = GPIO_PIN_5;
      else if((_TransMode == SPI_HalfDuplexSlave) || (_TransMode == SPI_ReceiveOnlyMaster) | (_TransMode == SPI_TransmitOnlySlave)) _GPIO_Set = GPIO_PIN_4;
      _GPIO_Clk = GPIO_PIN_3;
      _SetPinAlternate(GPIOB, _GPIO_Clk | _GPIO_Set, GPIO_AF5_SPI1);
    }
  }
  else if(_SPI_Port == SPI2) // --------------------- SPI2 -------------------------------
  {
    RCC->APB1ENR |= RCC_APB1ENR_SPI2EN;                                                   // Запускаем тактовый генератор для порта SPI2
    if(!_Alternate)
    {
      if((_TransMode == SPI_FullDuplexMaster) || (_TransMode == SPI_FullDuplexSlave)) _GPIO_Set = GPIO_PIN_2 | GPIO_PIN_3;
      else if((_TransMode == SPI_HalfDuplexMaster) || (_TransMode == SPI_ReceiveOnlySlave) || (_TransMode == SPI_TransmitOnlyMaster)) _GPIO_Set = GPIO_PIN_3;
      else if((_TransMode == SPI_HalfDuplexSlave) || (_TransMode == SPI_ReceiveOnlyMaster) | (_TransMode == SPI_TransmitOnlySlave)) _GPIO_Set = GPIO_PIN_2;
      _GPIO_Clk = GPIO_PIN_10;
      _SetPinAlternate(GPIOC, _GPIO_Set,   GPIO_AF5_SPI2);       // MISO, MOSI
      _SetPinAlternate(GPIOB, _GPIO_Clk,   GPIO_AF5_SPI2);       // SCK
    }
    else
    {
      if((_TransMode == SPI_FullDuplexMaster) || (_TransMode == SPI_FullDuplexSlave)) _GPIO_Set = GPIO_PIN_14 | GPIO_PIN_15;
      else if((_TransMode == SPI_HalfDuplexMaster) || (_TransMode == SPI_ReceiveOnlySlave) || (_TransMode == SPI_TransmitOnlyMaster)) _GPIO_Set = GPIO_PIN_15;
      else if((_TransMode == SPI_HalfDuplexSlave) || (_TransMode == SPI_ReceiveOnlyMaster) | (_TransMode == SPI_TransmitOnlySlave)) _GPIO_Set = GPIO_PIN_14;
      _GPIO_Clk = GPIO_PIN_13;
      _SetPinAlternate(GPIOB, _GPIO_Clk | _GPIO_Set, GPIO_AF5_SPI2);
    }
  }
  //    if(!_Alternate)
  //      _SetPinAlternate(GPIOC, GPIO_PIN_10 | GPIO_PIN_11 | GPIO_PIN_12, GPIO_AF6_SPI3);    // SCK, MISO, MOSI
  //    else
  //      _SetPinAlternate(GPIOB, GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5, GPIO_AF6_SPI3);       // SCK, MISO, MOSI
  else if(_SPI_Port == SPI3) // --------------------- SPI3 ------------------------------
  {
    RCC->APB1ENR |= RCC_APB1ENR_SPI3EN;                                                   // Запускаем тактовый генератор для порта SPI3
    if(!_Alternate)
    {
      if((_TransMode == SPI_FullDuplexMaster) || (_TransMode == SPI_FullDuplexSlave)) _GPIO_Set = GPIO_PIN_11 | GPIO_PIN_12;
      else if((_TransMode == SPI_HalfDuplexMaster) || (_TransMode == SPI_ReceiveOnlySlave) || (_TransMode == SPI_TransmitOnlyMaster)) _GPIO_Set = GPIO_PIN_12;
      else if((_TransMode == SPI_HalfDuplexSlave) || (_TransMode == SPI_ReceiveOnlyMaster) | (_TransMode == SPI_TransmitOnlySlave)) _GPIO_Set = GPIO_PIN_11;
      _GPIO_Clk = GPIO_PIN_10;
      _SetPinAlternate(GPIOC, _GPIO_Clk | _GPIO_Set,   GPIO_AF6_SPI3);       // SCK, MISO, MOSI
    }
    else
    {
      if((_TransMode == SPI_FullDuplexMaster) || (_TransMode == SPI_FullDuplexSlave)) _GPIO_Set = GPIO_PIN_4 | GPIO_PIN_5;
      else if((_TransMode == SPI_HalfDuplexMaster) || (_TransMode == SPI_ReceiveOnlySlave) || (_TransMode == SPI_TransmitOnlyMaster)) _GPIO_Set = GPIO_PIN_5;
      else if((_TransMode == SPI_HalfDuplexSlave) || (_TransMode == SPI_ReceiveOnlyMaster) | (_TransMode == SPI_TransmitOnlySlave)) _GPIO_Set = GPIO_PIN_4;
      _GPIO_Clk = GPIO_PIN_3;
      _SetPinAlternate(GPIOB, _GPIO_Clk | _GPIO_Set, GPIO_AF6_SPI3);
    }
  }

  // Определяем и программируем режимы SPI
  if((_TransMode == SPI_FullDuplexMaster)   || (_TransMode == SPI_FullDuplexSlave) ||
     (_TransMode == SPI_TransmitOnlyMaster) || (_TransMode == SPI_TransmitOnlySlave))
  {
    if(_TransMode & MASTER_MODE) CMD_CR1 = SPI_CR1_MSTR | SPI_CR1_SSI; else CMD_CR1 = 0;
  }
  else if((_TransMode == SPI_HalfDuplexMaster) || (_TransMode == SPI_HalfDuplexSlave))
  {
    if(_TransMode & MASTER_MODE) CMD_CR1 = SPI_CR1_MSTR | SPI_CR1_SSI | SPI_CR1_BIDIMODE; else CMD_CR1 = SPI_CR1_BIDIMODE;
  }
  else if((_TransMode == SPI_ReceiveOnlyMaster) || (_TransMode == SPI_ReceiveOnlySlave))
  {
    if(_TransMode & MASTER_MODE) CMD_CR1 = SPI_CR1_MSTR | SPI_CR1_SSI | SPI_CR1_RXONLY; else CMD_CR1 = SPI_CR1_RXONLY;
  }

  CMD_CR1 |= SPI_CR1_SSM;

  _SetSpeed(Speed_Low);
  _SetPull(No_Pull);

  // Шаг 3 - Инициализируем порт
  _SPI_Port->CR1  = CMD_CR1;
  _SPI_Port->CR1  |= (_SpiDiv << SPI_CR1_BR_Pos);                                         // Устанавливаем скорость предачи
  _SPI_Port->CR1  |= _SpiFaze;

  if(_MSB)      _SPI_Port->CR1  |= SPI_CR1_LSBFIRST;                                      // Младший бит идёт первым
  if(_DataSize) _SPI_Port->CR1  |= SPI_CR1_DFF;                                           // Устанавливаем количество передаваемых бит = 16
  _SPI_Port->CR1 |= SPI_CR1_BIDIOE;                                                       // Включаем на передачу

  _SPI_Port->CR1  |= SPI_CR1_SPE;                                                         // Включаем SPI
}

/*
 * Функции передачи
 */

// ------- Фунция передачи одного байта --------

size_t SPI::Transmit(uint16_t Data)
{
  if((_TransMode & SPI_HalfDuplexMaster) || (_TransMode & SPI_HalfDuplexSlave))
  {
    _SPI_Port->CR1 &= ~SPI_CR1_SPE;                                                         // Выключаем SPI
    _SPI_Port->CR1 |= SPI_CR1_BIDIOE;
  }

  if(!(_SPI_Port->CR1 & SPI_CR1_SPE)) _SPI_Port->CR1 |= SPI_CR1_SPE;

  while(!(_SPI_Port->SR & SPI_SR_TXE)){};                                                 // Ждем, пока не освободится буфер передатчика
  if(_DataSize)
  {
    _SPI_Port->DR = (uint16_t) Data;                                                      // Заполняем буфер передатчика
  }
  else
  {
    *((__IO uint8_t *)&_SPI_Port->DR) = (uint8_t) Data;
  }
//  while(_SPI_Port->SR & SPI_SR_BSY){};                                             // Ждём, пока не освободится передатчик
  return 0;
}

// ------ Функция передачи 8ми и меньше разрядного буфера -------

size_t SPI::Transmit(uint8_t *Buff, uint16_t Length)
{
  uint16_t _Lenght = Length;
  uint8_t *TxBuffPtr;
  TxBuffPtr = (uint8_t*) Buff;

  if((_TransMode & SPI_HalfDuplexMaster) || (_TransMode & SPI_HalfDuplexSlave))
  {
    _SPI_Port->CR1 &= ~SPI_CR1_SPE;                                                         // Выключаем SPI
    _SPI_Port->CR1 |= SPI_CR1_BIDIOE;
  }

  if(!(_SPI_Port->CR1 & SPI_CR1_SPE)) _SPI_Port->CR1 |= SPI_CR1_SPE;

  while (_Lenght > 0U)
  {
    while (!(_SPI_Port->SR & SPI_SR_TXE)){};                                              // Ждем, пока не освободится буфер передатчика
//    *((__IO uint8_t*) &_SPI_Port->DR) = *((uint8_t*) TxBuffPtr);
    *((__IO uint8_t*) &_SPI_Port->DR) = (*TxBuffPtr);
    TxBuffPtr += sizeof(uint8_t);
    _Lenght--;
  }
//  while(_SPI_Port->SR & SPI_SR_BSY){};                                             // Ждём, пока не освободится передатчик
  return 0;
}

// ------ Функция передачи буфера больше 8 разрядов -----------
size_t SPI::Transmit(uint16_t *Buff, uint16_t Length)
{
  uint16_t _Lenght = Length;
  uint16_t *TxBuffPtr;
  TxBuffPtr = (uint16_t*) Buff;

  if((_TransMode & SPI_HalfDuplexMaster) || (_TransMode & SPI_HalfDuplexSlave))
  {
    _SPI_Port->CR1 &= ~SPI_CR1_SPE;                                                         // Выключаем SPI
    _SPI_Port->CR1 |= SPI_CR1_BIDIOE;
  }

  if(!(_SPI_Port->CR1 & SPI_CR1_SPE)) _SPI_Port->CR1 |= SPI_CR1_SPE;

  while (_Lenght > 0U)
  {
    while (!(_SPI_Port->SR & SPI_SR_TXE)){};                                              // Ждем, пока не освободится буфер передатчика
    _SPI_Port->DR = *((uint16_t*) TxBuffPtr);
    TxBuffPtr += sizeof(uint16_t);
    _Lenght--;
  }
//  while(_SPI_Port->SR & SPI_SR_BSY){};                                                 // Ждём, пока не освободится FIFO передатчика
  return 0;
}

/*
 * Функции приёма
 */

size_t SPI::Receive(void)
{
  while(!(_SPI_Port->SR & SPI_SR_TXE)){};                                                 // Ждем, пока не освободится передатчмк
//  if(_Master) _SPI_Port->DR = 0;                                                                      // Запускаем обмен
  while(!(_SPI_Port->SR & SPI_SR_RXNE)){};                                                // Ждем, пока не появится новое значение в буфере приемника
  return _SPI_Port->DR;                                                                   // Возвращаем значение буфера приемника
}

size_t SPI::Receive(uint16_t *Buff, uint16_t Length)
{
  return 0;
}

size_t SPI::Receive(uint8_t *Buff, uint16_t Length)
{
  uint16_t _Lenght = Length;
  uint8_t *RxBuffPtr;
  RxBuffPtr = (uint8_t*) Buff;

  if(_TransMode & SPI_HalfDuplexMaster)
  {
    _SPI_Port->CR1 &= ~SPI_CR1_SPE;                                                         // Выключаем SPI
    _SPI_Port->CR1 &= ~SPI_CR1_BIDIOE;
  }
  if(!(_SPI_Port->CR1 & SPI_CR1_SPE)) _SPI_Port->CR1 |= SPI_CR1_SPE;

  while (_Lenght > 0U)
  {
    while (!(_SPI_Port->SR & SPI_SR_RXNE)){};                                              //
    (*RxBuffPtr) = *((__IO uint8_t*) &_SPI_Port->DR);
    RxBuffPtr += sizeof(uint8_t);
    _Lenght--;
  }
  if(_TransMode & SPI_HalfDuplexMaster)
  {
    _SPI_Port->CR1 &= ~SPI_CR1_SPE;                                                         // Выключаем SPI
    _SPI_Port->CR1 |= SPI_CR1_BIDIOE;
  }
  return 0;
}



size_t SPI::TransmitReceive(uint16_t Data)
{
  while(!(_SPI_Port->SR & SPI_SR_TXE)){};                                                 // Ждем, пока не освободится передатчмк
//  if(_Master) _SPI_Port->DR = Data;                                                                   // Запускаем обмен
  while(!(_SPI_Port->SR & SPI_SR_RXNE)){};                                                // Ждем, пока не появится новое значение в буфере приемника
  return _SPI_Port->DR;                                                                   // Возвращаем значение буфера приемника
}

size_t SPI::TransmitReceive(uint16_t *TxBuff, uint16_t *RxBuff, uint16_t Length)
{
  return 0;
}

/*
 * Устанавливаем режим Master/Slave
 */

void   SPI::setMasterSlave(bool Selection)
{
  while(_SPI_Port->SR & SPI_SR_BSY){};              // Ждём, пока не освободится FIFO передатчика и приёмника
  if(Selection)
  {
//    _Master = SPI_Master;
    _SPI_Port->CR1 |= SPI_CR1_MSTR;                                                       // Если мастер, переключаем на мастер
  }
  else
  {
//    _Master = SPI_Slave;
    _SPI_Port->CR1 &= ~SPI_CR1_MSTR;                                                      // Иначе - ведомый
  }
}

void SPI::setDataSize(uint8_t DataSize)
{
  _DataSize = DataSize;
}

/*
 * Приём байт с помощью прерывания
 */

size_t  SPI::Receive_IT(void)
{
  return 0;
}

size_t  SPI::Receive_IT(uint8_t *Buff, uint16_t Length)
{
  uint16_t _Lenght = Length;
  uint8_t *RxBuffPtr;
  RxBuffPtr = (uint8_t*) Buff;

  if(_TransMode & SPI_HalfDuplexMaster)
  {
    _SPI_Port->CR1 &= ~SPI_CR1_SPE;                                                         // Выключаем SPI
    _SPI_Port->CR1 &= ~SPI_CR1_BIDIOE;
  }
  if(!(_SPI_Port->CR1 & SPI_CR1_SPE)) _SPI_Port->CR1 |= SPI_CR1_SPE;

//  while (_Lenght > 0U)
//  {
//    while (!(_SPI_Port->SR & SPI_SR_RXNE)){};                                              //
//    (*RxBuffPtr) = *((__IO uint8_t*) &_SPI_Port->DR);
//    RxBuffPtr += sizeof(uint8_t);
//    _Lenght--;
//  }

  if(_TransMode & SPI_HalfDuplexMaster)
  {
    _SPI_Port->CR1 &= ~SPI_CR1_SPE;                                                         // Выключаем SPI
    _SPI_Port->CR1 |= SPI_CR1_BIDIOE;
  }

  _SPI_Port->CR2 |= SPI_CR2_RXNEIE;                                                       // Разрешаем прерывание от приёмника
  NVIC_EnableIRQ(_IRQn);                                                                  // Разрешаем прерывания

  return 0;
}

size_t  SPI::Receive_IT(uint16_t *Buff, uint16_t Length)
{
  return 0;
}

/*
 * Прерывания
 */

extern "C"  void SPI1_IRQHandler(void)
{
//  static uint16_t tmp;

  //Если прерывание приема разрешено и что-то получили
  if((SPI1->SR & SPI_SR_RXNE) && (SPI1->CR2 & SPI_CR2_RXNEIE))
  {

  }

  // Если прерывание передачи разрешено и регист освободился
  if((SPI1->SR & SPI_SR_TXE) && (SPI1->CR2 & SPI_CR2_TXEIE))              // Если передачтик пуст
  {
  }
}



