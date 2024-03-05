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

//void Test(void);

//#define INTER_UART4 *(volatile unsigned long *)0x00000110

uart::uart(USART_TypeDef *Port, bool Alternate)
{
  _USART_Port = Port;
  _Alternate  = Alternate;

  // Пока прописаны прерывания для STM32L052
  if(Port == USART1) _IRQn = USART1_IRQn;
  else if(Port == USART2) _IRQn = USART2_IRQn;
  // Далее должны быть прерывания для других МК этой серии
#if defined (false)
  else if(Port == USART3) _IRQn = USART3_IRQn;
  else if(Port == USART6) _IRQn = USART6_IRQn;
#endif
}

// Инициализация
void uart::init(uint32_t BaudRate)
{
  uint32_t FreqUART;
  uint32_t Prescaler;

  // Инициализация портов ввода/вывода и запуск тактирования
  if(_USART_Port == USART1)
  {
    RCC->APB2ENR |= RCC_APB2ENR_USART1EN;
    _SetSpeed(Speed_High);
    _SetPull(Pull_Up);
    if(!_Alternate)
      _SetPinAlternate(GPIOA, GPIO_PIN_0 | GPIO_PIN_1, GPIO_AF0_USART1);
    else
      _SetPinAlternate(GPIOC, GPIO_PIN_10 | GPIO_PIN_11, GPIO_AF0_USART1);
    _SetSpeed(Speed_Low);
  }

  // Рассчитываем делитель для получения скорости передачи
  _BaudRate = BaudRate;
  Prescaler = APBPrescTable[(RCC->CFGR & RCC_CFGR_PPRE1) >> RCC_CFGR_PPRE1_Pos];
  FreqUART = SystemCoreClock >> Prescaler;
  _USART_Port->BRR = UART_BRR_SAMPLING16(FreqUART, _BaudRate);

  // Инициализируем порт
  _USART_Port->CR1 |= (USART_CR1_RE | USART_CR1_TE | USART_CR1_UE);                            // Включаем передатчик и приёмник
  RingBuffInit(&tx_fifo, tx_buff, UART_TXBUFF_LENGHT);
  RingBuffInit(&rx_fifo, rx_buff, UART_RXBUFF_LENGHT);

  RXNEIEnable();

  NVIC_EnableIRQ(_IRQn);
}

uint8_t uart::Transmit(uint8_t *Buff, uint8_t Length)
{
  while(Length)
  {
    ring_put(UART_TXBUFF_LENGHT, &tx_fifo, *Buff);
    Length--;
    Buff++;
  }
  return 0;
}


/*
 * Функции из ардуины
 */

size_t uart::write(uint8_t c)
{
  return ring_put(UART_TXBUFF_LENGHT, &tx_fifo, c);
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

int16_t uart::ring_put(uint16_t BuffLen, RingBuff_t *fifo, uint8_t c)
{
  int16_t ret;

  NVIC_DisableIRQ(_IRQn);
  if((BuffLen - RingBuffNumOfItems(fifo)) > 0)
  {
    RingBuffPut(fifo, c);
    ret = c;
    TXEIEnable(); //запускаем передачу
  }
  else
  {
    ret = -1;
  }
  NVIC_EnableIRQ(_IRQn);

  return ret;
}

int16_t uart::ring_get(RingBuff_t *fifo)
{
  int16_t ret;

  NVIC_DisableIRQ(_IRQn);
  ret = RingBuffGet(fifo);
  NVIC_EnableIRQ(_IRQn);

  return ret;
}


//void Test(void)
//{
//  __ASM("NOP");
//}

/*
 * Прерывания
 */

extern "C"  void USART1_IRQHandler(void)
{
  static uint16_t tmp;

  //Если прерывание приема разрешено и что-то получили
  if((USART1->ISR & USART_ISR_RXNE) && (USART1->CR1 & USART_CR1_RXNEIE))
    {
      tmp = USART1->RDR;
      RingBuffPut(&rx_fifo, tmp);
    }

  if((USART1->ISR & USART_ISR_TXE) && (USART1->CR1 & USART_CR1_TXEIE))              // Если передачтик пуст
  {
    if(RingBuffNumOfItems(&tx_fifo) > 0)                                        // если есть, что передавать, передаём
    {
      tmp = RingBuffGet(&tx_fifo);
      USART1->RDR = (uint8_t)(tmp & 0xFF);
    }
    else                                                                        // если передавать нечего
    {
      USART1->CR1 &= ~USART_CR1_TXEIE;                                           // отключаем прерывание
    }
  }
}

