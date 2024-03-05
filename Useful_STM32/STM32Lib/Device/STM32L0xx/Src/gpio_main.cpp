/*
 * GPIO.cpp
 *
 *  Created on: 5 апр. 2021 г.
 *      Author: User
 */

#include "stdint.h"
#include "gpio_main.h"

static uint8_t _GPIO_Speed = Speed_Low;
static uint8_t _GPIO_Pull  = No_Pull;
static uint8_t _TypeOut    = Out_Normal;
static bool    _Edge       = Low;

void _SetDebug_SW(void){} // Пока не реализована за ненадобностью


void _SetSpeed(uint8_t Speed_Set){_GPIO_Speed = Speed_Set;}
void _SetPull(uint8_t Pull_Set){_GPIO_Pull = Pull_Set;}
void _SetTypeOut(uint8_t TypeOutput){ _TypeOut = TypeOutput; }
void _SetEdge(bool Edge){ _Edge = Edge; }

/*
 * Установка функции вывода
 */
void _SetPin(GPIO_TypeDef *GPIOx_Set, uint16_t GPIO_Pin_Set, uint8_t IO_Set, uint8_t PULL_Set)
{
  GPIO_TypeDef *_GPIOx_Set = GPIOx_Set;
  uint16_t _GPIO_Pin_Set = GPIO_Pin_Set;

  if(_GPIOx_Set == GPIOA) RCC->IOPENR |= RCC_IOPENR_GPIOAEN;
  if(_GPIOx_Set == GPIOB) RCC->IOPENR |= RCC_IOPENR_GPIOBEN;
  if(_GPIOx_Set == GPIOC) RCC->IOPENR |= RCC_IOPENR_GPIOCEN;
  if(_GPIOx_Set == GPIOD) RCC->IOPENR |= RCC_IOPENR_GPIODEN;
//  if(_GPIOx_Set == GPIOF) RCC->AHBENR |= RCC_AHBENR_GPIOFEN;

// Переменные используемые в функции
  uint32_t Position;                                                            // Счётчик позиции.
  uint32_t IO_Position = 0x00U;                                                 // Указатель позиции
  uint32_t IO_Current = 0x00U;                                                  // Текущая позиция

  // Инициализируем вывод
  for (Position = 0U; Position < 16; Position++)                                // Перебираем все возможные позиции
  {
    IO_Position = 0x01U << Position;                                            // Инициализируем указатель на позицию
    IO_Current = (uint32_t) (_GPIO_Pin_Set) & IO_Position;                      // Выделяем текущую позицию из списка переданного в функцию

    if(IO_Current == IO_Position)                                               // Если такая позиция существует. Идём её обрабатывать
    {
      if(_Edge)
        _GPIOx_Set->BSRR = Position;                                            // Устанавливаем 1 на выходе
      else
        _GPIOx_Set->BSRR = Position << 16;                                      // Сбрасываем в 0

      if(IO_Set)                                                                // INPUT = 0, OUTPUT = 1
      {
        _GPIOx_Set->OSPEEDR &= ~(0b11 << (Position * 2));                       // Устанавливаем низкую скорость порта
        _GPIOx_Set->OSPEEDR |= _GPIO_Speed << (Position * 2);                   // И затем необходимую нам. ШАГ 4
        _GPIOx_Set->OTYPER &= ~(1U << Position);                                // Устанавливаем тип вывода push-pull. ШАГ 3
        _GPIOx_Set->OTYPER |= (_TypeOut << Position);
      }

      switch (PULL_Set)
      // Определяем тип подтяжки и программируем нужный. ШАГ 5
      {
        case No_Pull:
          _GPIOx_Set->PUPDR &= ~(0b11 << (Position * 2));                       // Сбрасываем регистр подтяжки
          break;
        case Pull_Up:                                                           // Подтягиваем к верху
          _GPIOx_Set->PUPDR &= ~(0b11 << (Position * 2));
          _GPIOx_Set->PUPDR |= (Pull_Up << (Position * 2));
          break;
        case Pull_Down:                                                         // Подтягиваем книзу
          _GPIOx_Set->PUPDR &= ~(0b11 << (Position * 2));
          _GPIOx_Set->PUPDR |= (Pull_Down << (Position * 2));
          break;
        default:
          break;
      }

      _GPIOx_Set->MODER &= ~(0b11 << (Position * 2));                           // Сбрасываем порт на вход
      _GPIOx_Set->MODER |= (IO_Set << (Position * 2));                          // Если ф-я выхода, то выход. ШАГ 2

      _GPIOx_Set->ODR &= ~(1 << Position);                                      // Принудительно переводим пин в "0"
    }
  }
}

/*
 * Установка альтернативной фунции вывода
 */

void _SetPinAlternate(GPIO_TypeDef *GPIOx_Set, uint16_t GPIO_Pin_Set, uint8_t Alternate_Set)
{
  // !!! У Cortex-M4 AFIOEN в RCC нет. Для других контроллеров быть внимательнее
  GPIO_TypeDef *_GPIOx_Set = GPIOx_Set;
  uint16_t _GPIO_Pin_Set = GPIO_Pin_Set;

  if(_GPIOx_Set == GPIOA) RCC->IOPENR |= RCC_IOPENR_GPIOAEN;
  if(_GPIOx_Set == GPIOB) RCC->IOPENR |= RCC_IOPENR_GPIOBEN;
  if(_GPIOx_Set == GPIOC) RCC->IOPENR |= RCC_IOPENR_GPIOCEN;
  if(_GPIOx_Set == GPIOD) RCC->IOPENR |= RCC_IOPENR_GPIODEN;
//  if(_GPIOx_Set == GPIOF) RCC->AHBENR |= RCC_AHBENR_GPIOFEN;

  uint32_t Position;
  uint32_t IO_Position = 0x00U;
  uint32_t IO_Current = 0x00U;

  // Инициализируем вывод
  for (Position = 0U; Position < 16; Position++)
  {
    IO_Position = 0x01U << Position;
    IO_Current = (uint32_t) (_GPIO_Pin_Set) & IO_Position;

    if(IO_Current == IO_Position)
    {
      _GPIOx_Set->OSPEEDR &= ~(0b11 << (Position * 2));                         // Сброс скорости портов
      _GPIOx_Set->OSPEEDR |= (_GPIO_Speed << (Position * 2));                   // Скорость портов
      _GPIOx_Set->PUPDR &= ~(0b11 << (Position * 2));                           // Сброс подтяжек нет
      _GPIOx_Set->PUPDR |= (_GPIO_Pull << (Position * 2));                      // Выставляем необходимые подтяжки
      _GPIOx_Set->OTYPER &= ~(0b1 << Position);                                 // Выход обычный
      _GPIOx_Set->OTYPER |= (_TypeOut << Position);

      if(Position < 8)
      {
        _GPIOx_Set->AFR[0] &= ~(0b1111 << (Position * 4));                      // ARFH стираем только те биты, что будем программировать
        _GPIOx_Set->AFR[0] |= (Alternate_Set << (Position * 4));                // ARFH = Альтернативная функция
      }
      else
      {
        _GPIOx_Set->AFR[1] &= ~(0b1111 << ((Position - 8) * 4));                // ARFL стираем только те биты, что будем программировать
        _GPIOx_Set->AFR[1] |= (Alternate_Set << ((Position - 8) * 4));          // ARFL = Альтернативная функция
      }
      _GPIOx_Set->MODER &= ~(0b11 << (Position * 2));                           // Сброс портов, если там что то есть
      _GPIOx_Set->MODER |= (0b10 << (Position * 2));                            // Альтернативная функция
    }
  }
}

bool _DigitalReadBit(GPIO_TypeDef *GPIOx_Set, uint16_t GPIO_Pin_Set)
{
  if(GPIOx_Set->IDR & GPIO_Pin_Set)
    return 1;
  else
    return 0;
}

void _DigitalWriteBit(GPIO_TypeDef *GPIOx_Set, uint16_t GPIO_Pin_Set, bool Level)
{
  if(Level)
    GPIOx_Set->BSRR = GPIO_Pin_Set;                                                   // Устанавливаем 1 на выходе
  else
    GPIOx_Set->BSRR = GPIO_Pin_Set << 16;                                             // Сбрасываем в 0
}

void _DigitalWrite(GPIO_TypeDef *GPIOx_Set, uint16_t Data, uint8_t FirstPin, uint16_t PinMask)
{
  GPIOx_Set->ODR &= ~PinMask;                                                   // Сбрасываем выводы соответствующие пуллу наших выводов
  GPIOx_Set->ODR |= ((Data << FirstPin) & PinMask);                             // Выводим
}

uint16_t _DigitalRead(GPIO_TypeDef *GPIOx_Set, uint8_t FirstPin, uint16_t PinMask)
{
  return ((GPIOx_Set->IDR & PinMask) >> FirstPin);
}
