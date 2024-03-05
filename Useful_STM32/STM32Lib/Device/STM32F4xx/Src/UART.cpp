/*
 * UART.h
 *
 *  Created on: 14 апр. 2021 г.
 *      Author: User
 */

#include "UART.h"

static RingBuff_t tx_fifo;
static uint8_t tx_buff[UART_TXBUFF_LENGHT];

static RingBuff_t rx_fifo;
static uint8_t rx_buff[UART_RXBUFF_LENGHT];

uart::uart(USART_TypeDef *Port, bool Alternate)
{
  _USART_Port = Port;
  _Alternate  = Alternate;

  if(Port == UART4)       _IRQn = UART4_IRQn;
  else if(Port == UART5)  _IRQn = UART5_IRQn;
  else if(Port == USART1) _IRQn = USART1_IRQn;
  else if(Port == USART2) _IRQn = USART2_IRQn;
  else if(Port == USART3) _IRQn = USART3_IRQn;
  else if(Port == USART6) _IRQn = USART6_IRQn;
}

// Инициализация
void uart::init(uint32_t BaudRate)
{
  uint32_t FreqUART;
  uint32_t Prescaler;

// Блок инициализации портов GPIO для работы с UART
// UART4
  if(_USART_Port == UART4)
  {
    RCC->APB1ENR |= RCC_APB1ENR_UART4EN;                                        // Включаем тактирование UART
    _SetSpeed(Speed_WeryHigh);
    _SetPull(Pull_Up);                                                          // При инициализации подтяжка вверх
    if(!_Alternate)                                                             // Если используются не альтенатиные выходы
      _SetPinAlternate(GPIOA, GPIO_PIN_0 | GPIO_PIN_1, GPIO_AF8_UART4);         // инициализируем "родные" выводы для UART4 - A0, A1
    else
      _SetPinAlternate(GPIOC, GPIO_PIN_10 | GPIO_PIN_11, GPIO_AF8_UART4);       // Или альтернативные C10, C11
    _SetSpeed(Speed_Low);
  }

// UART5 - не проверял
  else if(_USART_Port == UART5)
  {
    RCC->APB1ENR |= RCC_APB1ENR_UART5EN;
    _SetSpeed(Speed_WeryHigh);
    _SetPull(Pull_Up);
    // Альтернативы нет
    _SetPinAlternate(GPIOC, GPIO_PIN_12, GPIO_AF8_UART5);
    _SetPinAlternate(GPIOD, GPIO_PIN_2, GPIO_AF8_UART5);
    _SetSpeed(Speed_Low);
  }

// USART1
  else if(_USART_Port == USART1)
  {
    RCC->APB2ENR |= RCC_APB2ENR_USART1EN;
    _SetSpeed(Speed_WeryHigh);
    _SetPull(Pull_Up);
    if(!_Alternate)
      _SetPinAlternate(GPIOA, GPIO_PIN_9 | GPIO_PIN_10, GPIO_AF7_USART1);
    else
      _SetPinAlternate(GPIOB, GPIO_PIN_6 | GPIO_PIN_7, GPIO_AF7_USART1);
    _SetSpeed(Speed_Low);
  }

  // USART2
    else if(_USART_Port == USART2)
    {
      RCC->APB1ENR |= RCC_APB1ENR_USART2EN;
      _SetSpeed(Speed_WeryHigh);
      _SetPull(Pull_Up);
      if(!_Alternate)
        _SetPinAlternate(GPIOA, GPIO_PIN_2 | GPIO_PIN_3, GPIO_AF7_USART2);
      else
        _SetPinAlternate(GPIOD, GPIO_PIN_5 | GPIO_PIN_6, GPIO_AF7_USART2);
      _SetSpeed(Speed_Low);
    }

  // USART3
    else if(_USART_Port == USART1)
    {
      RCC->APB1ENR |= RCC_APB1ENR_USART3EN;
      _SetSpeed(Speed_WeryHigh);
      _SetPull(Pull_Up);
      if(!_Alternate)
        _SetPinAlternate(GPIOB, GPIO_PIN_10 | GPIO_PIN_11, GPIO_AF7_USART3);
      else
        _SetPinAlternate(GPIOD, GPIO_PIN_8 | GPIO_PIN_9, GPIO_AF7_USART3);      // Есть ещё альтернативная пара. Пока не придумал как её подрубать
      _SetSpeed(Speed_Low);
    }

  // USART6
    else if(_USART_Port == USART6)
    {
      RCC->APB2ENR |= RCC_APB2ENR_USART6EN;
      _SetSpeed(Speed_WeryHigh);
      _SetPull(Pull_Up);
      // Альтернативы нет
      _SetPinAlternate(GPIOC, GPIO_PIN_7 | GPIO_PIN_8, GPIO_AF8_USART6);
      _SetSpeed(Speed_Low);
    }

  // Рассчитываем делитель для получения скорости передачи
  _BaudRate = BaudRate;
  // В зависимости от порта выбираем делитель
  if((_USART_Port == USART2) || (_USART_Port == USART3) || (_USART_Port == UART4) || (_USART_Port == UART5))
    Prescaler = APBPrescTable[(RCC->CFGR & RCC_CFGR_PPRE1) >> RCC_CFGR_PPRE1_Pos];
  else
    Prescaler = APBPrescTable[(RCC->CFGR & RCC_CFGR_PPRE2) >> RCC_CFGR_PPRE2_Pos];

  FreqUART = SystemCoreClock >> Prescaler;
  _USART_Port->BRR = UART_BRR_SAMPLING16(FreqUART, _BaudRate);

  // Разрешаем работу UART
  _USART_Port->CR1 |= (USART_CR1_RE | USART_CR1_TE | USART_CR1_UE);                            // Включаем передатчик и приёмник

  TxBuff = tx_buff;                                                                       // Инициализируем указатели на буфер FIFO
  RxBuff = rx_buff;
  RingBuffInit(&tx_fifo, TxBuff, _BuffLength);                                            // Инициализируем переменные управляющие буфером FIFO
  RingBuffInit(&rx_fifo, RxBuff, _BuffLength);

  RXNEIEnable();                                                                          // Разрешаем приём данных

  NVIC_EnableIRQ(_IRQn);                                                                  // Разрешаем прерывание от UART
}

// Написана на всякий пожарный. write заменяет её полностью.
//uint8_t uart::Transmit(uint8_t *Buff, uint8_t Length)
//{
//  while(Length)
//  {
//    ring_put(_BuffLength, &tx_fifo, *Buff);
//    Length--;
//    Buff++;
//  }
//  return 0;
//}

// Выделение памяти
bool uart::SetBuffSize(uint16_t Size)
{
  uint16_t Temp = _BuffLength;
  _BuffLength = Size;

  TxBuff = (uint8_t*) malloc(_BuffLength);        // Выделяем под буфер передачи
  if(TxBuff == NULL)                              // Не получилось. Возвращаемся
  {
    _BuffLength = Temp;
    return false;
  }

  RxBuff = (uint8_t*) malloc(_BuffLength);        // Выделяем под буфер приёма
  if(RxBuff == NULL)                              // Не получилось
  {
    free(TxBuff);                                 // Освобождаем предыдущий буфер.
    _BuffLength = Temp;
    return false;
  }
  RingBuffInit(&tx_fifo, TxBuff, _BuffLength);    // Инициализируем FIFO
  RingBuffInit(&rx_fifo, RxBuff, _BuffLength);
  return true;
}
/*
 * Функции из ардуины
 */

size_t uart::write(uint8_t c)
{
  return ring_put(_BuffLength, &tx_fifo, c);
}


/*
 * Функции от доброго человека
 */

void    uart::ReadBuffClear()   {RingBuffClear(&rx_fifo);}
void    uart::WriteBuffClear()  {RingBuffClear(&tx_fifo);}
int16_t uart::BytesToRead()     {return RingBuffNumOfItems(&rx_fifo);}
int16_t uart::BytesToWrite()    {return RingBuffNumOfItems(&tx_fifo);}
void    uart::TXEIEnable()      {_USART_Port->CR1 |= USART_CR1_TXEIE;}
void    uart::TXEIDisable()     {_USART_Port->CR1 &= ~USART_CR1_TXEIE;}
void    uart::RXNEIEnable()     {_USART_Port->CR1 |= USART_CR1_RXNEIE;}
int16_t uart::read()            {return ring_get(&rx_fifo);}

int16_t uart::ring_put(uint16_t BuffLen, RingBuff_t *fifo, uint8_t c) // Функция для закидывания байта в стек FIFO
{
  int16_t ret;

  while((BuffLen - RingBuffNumOfItems(fifo)) < 5){;}                  // Если буфер близок к переполнению, притормаживаем

  NVIC_DisableIRQ(_IRQn);                                             // Отключаем прерывание, что бы не помешало заполнять буфер
  RingBuffPut(fifo, c);                                               // Закидываем на стек FIFO
  ret = c;                                                            // Зачем? Не помню
  TXEIEnable();                                                       // Запускаем передачу
  NVIC_EnableIRQ(_IRQn);                                              // Разрешаем прерывание

  return ret;
}

int16_t uart::ring_get(RingBuff_t *fifo)                              // Функция забирает байт со стека
{
  int16_t ret;

  NVIC_DisableIRQ(_IRQn);                                             // Запрещаем прерывание, что бы не порушить стек FIFO
  ret = RingBuffGet(fifo);                                            // Забираем байт
  NVIC_EnableIRQ(_IRQn);                                              // Разрешаем прерывание

  return ret;                                                         // Возвращаем байт
}

/*
 * Прерывания
 */
// UART4
extern "C"  void UART4_IRQHandler(void)
{
  static uint16_t tmp;

  // Прерывание от приёмника.
  if((UART4->SR & USART_SR_RXNE) && (UART4->CR1 & USART_CR1_RXNEIE))            // Если прерывание приема разрешено и что-то получили
    {
      tmp = UART4->DR;                                                          // Читаем данные
      RingBuffPut(&rx_fifo, tmp);                                               // и кидаем их в FIFO
    }
  // Прерывание от передатчика.
  if((UART4->SR & USART_SR_TXE) && (UART4->CR1 & USART_CR1_TXEIE))              // Если прерывание разрешено и произошло,
  {
    if(RingBuffNumOfItems(&tx_fifo) > 0)                                        // проверяем есть что передавать?
    {
      tmp = RingBuffGet(&tx_fifo);                                              // Берём из FIFO байт и
      UART4->DR = (uint8_t)(tmp & 0xFF);                                        // передаём его
    }
    else                                                                        // если передавать нечего
    {
      UART4->CR1 &= ~USART_CR1_TXEIE;                                           // отключаем прерывание от передатчика. Позже его включит функция ring_put
    }
  }
}

// UART5
extern "C"  void UART5_IRQHandler(void)
{
  static uint16_t tmp;

  //Если прерывание приема разрешено и что-то получили
  if((UART5->SR & USART_SR_RXNE) && (UART5->CR1 & USART_CR1_RXNEIE))
    {
      tmp = UART5->DR;
      RingBuffPut(&rx_fifo, tmp);
    }

  if((UART5->SR & USART_SR_TXE) && (UART5->CR1 & USART_CR1_TXEIE))              // Если передачтик пуст
  {
    if(RingBuffNumOfItems(&tx_fifo) > 0)                                        // если есть, что передавать, передаём
    {
      tmp = RingBuffGet(&tx_fifo);
      UART4->DR = (uint8_t)(tmp & 0xFF);
    }
    else                                                                        // если передавать нечего
    {
      UART5->CR1 &= ~USART_CR1_TXEIE;                                           // отключаем прерывание
    }
  }
}

// USART1
extern "C"  void USART1_IRQHandler(void)
{
  static uint16_t tmp;

  //Если прерывание приема разрешено и что-то получили
  if((USART1->SR & USART_SR_RXNE) && (USART1->CR1 & USART_CR1_RXNEIE))
    {
      tmp = USART1->DR;
      RingBuffPut(&rx_fifo, tmp);
    }

  if((USART1->SR & USART_SR_TXE) && (USART1->CR1 & USART_CR1_TXEIE))              // Если передачтик пуст
  {
    if(RingBuffNumOfItems(&tx_fifo) > 0)                                        // если есть, что передавать, передаём
    {
      tmp = RingBuffGet(&tx_fifo);
      USART1->DR = (uint8_t)(tmp & 0xFF);
    }
    else                                                                        // если передавать нечего
    {
      USART1->CR1 &= ~USART_CR1_TXEIE;                                           // отключаем прерывание
    }
  }
}

// USART2
extern "C"  void USART2_IRQHandler(void)
{
  static uint16_t tmp;

  //Если прерывание приема разрешено и что-то получили
  if((USART2->SR & USART_SR_RXNE) && (USART2->CR1 & USART_CR1_RXNEIE))
    {
      tmp = USART2->DR;
      RingBuffPut(&rx_fifo, tmp);
    }

  if((USART2->SR & USART_SR_TXE) && (USART2->CR1 & USART_CR1_TXEIE))              // Если передачтик пуст
  {
    if(RingBuffNumOfItems(&tx_fifo) > 0)                                        // если есть, что передавать, передаём
    {
      tmp = RingBuffGet(&tx_fifo);
      USART2->DR = (uint8_t)(tmp & 0xFF);
    }
    else                                                                        // если передавать нечего
    {
      USART2->CR1 &= ~USART_CR1_TXEIE;                                           // отключаем прерывание
    }
  }
}

// USART3
extern "C"  void USART3_IRQHandler(void)
{
  static uint16_t tmp;

  //Если прерывание приема разрешено и что-то получили
  if((USART3->SR & USART_SR_RXNE) && (USART3->CR1 & USART_CR1_RXNEIE))
    {
      tmp = USART3->DR;
      RingBuffPut(&rx_fifo, tmp);
    }

  if((USART3->SR & USART_SR_TXE) && (USART3->CR1 & USART_CR1_TXEIE))              // Если передачтик пуст
  {
    if(RingBuffNumOfItems(&tx_fifo) > 0)                                        // если есть, что передавать, передаём
    {
      tmp = RingBuffGet(&tx_fifo);
      USART3->DR = (uint8_t)(tmp & 0xFF);
    }
    else                                                                        // если передавать нечего
    {
      USART3->CR1 &= ~USART_CR1_TXEIE;                                           // отключаем прерывание
    }
  }
}
