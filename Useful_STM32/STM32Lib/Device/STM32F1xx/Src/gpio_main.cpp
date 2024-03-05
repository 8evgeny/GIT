//******************************************************************************
//  Секция include: здесь подключается заголовочный файл к модулю
// Работа с портами ввода/вывода
// Функции:
//    - IO_Digital() - настраивает пин порта на ввод вывод
//******************************************************************************
#include  <stdint.h>
#include "gpio_main.h"      // Включаем файл заголовка для нашего модуля

// Инициализация переменных стандартными значениями
static uint8_t _GPIO_Speed = Speed_Low;
static uint8_t _GPIO_Pull  = No_Pull;
static uint8_t _TypeOut    = Output;
static bool    _Edge       = Low;

void _SetDebug_SW(void){} // Пока не реализована за ненадобностью

// Функции переинициализации системных переменных
void _SetSpeed(uint8_t Speed_Set){ _GPIO_Speed = Speed_Set; }
void _SetPull(uint8_t Pull_Set){ _GPIO_Pull = Pull_Set; }
void _SetTypeOut(uint8_t TypeOutput){ _TypeOut = TypeOutput; }
void _SetEdge(bool Edge){ _Edge = Edge; }

/*
 * Установка простой функции вывода
 */
void _SetPin(GPIO_TypeDef *GPIOx_Set, uint16_t GPIO_Pin_Set, uint8_t IO_Set, uint8_t PULL_Set)
{
  // Инициализируем внутренние переменные используемые функциями
  GPIO_TypeDef *_GPIOx_Set = GPIOx_Set;                               // Порт
  uint16_t _GPIO_Pin_Set = GPIO_Pin_Set;                              // Используемые выводы

  // Включение тактирования необходимого порта
  if(_GPIOx_Set == GPIOA) RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;
  else if(_GPIOx_Set == GPIOB) RCC->APB2ENR |= RCC_APB2ENR_IOPBEN;
  else if(_GPIOx_Set == GPIOC) RCC->APB2ENR |= RCC_APB2ENR_IOPCEN;
  else if(_GPIOx_Set == GPIOD) RCC->APB2ENR |= RCC_APB2ENR_IOPDEN;
  else if(_GPIOx_Set == GPIOE) RCC->APB2ENR |= RCC_APB2ENR_IOPEEN;
//  else if(_GPIOx_Set == GPIOF) RCC->APB2ENR |= RCC_APB2ENR_IOPFEN;
//  else if(_GPIOx_Set == GPIOG) RCC->APB2ENR |= RCC_APB2ENR_IOPGEN;
//  else if(_GPIOx_Set == GPIOH) RCC->APB2ENR |= RCC_APB2ENR_IOPHEN;

  uint32_t Position;
  uint32_t IO_Position = 0x00U;
  uint32_t IO_Current = 0x00U;

  // Инициализируем вывод в цикле, так как _GPIO_Pin_Set может содержать описание нескольких выводов
  for (Position = 0U; Position < 16; Position++)
  {
    IO_Position = 0x01U << Position;
    IO_Current = (uint32_t) (_GPIO_Pin_Set) & IO_Position;

    if(IO_Current == IO_Position)
    {
      if(_Edge)
        _GPIOx_Set->BSRR = (1U << Position);                                            // Устанавливаем 1 на выходе
      else
        _GPIOx_Set->BSRR = ((1U << Position) << 16);                                      // Сбрасываем в 0

      if(Position < 8)
      {
        _GPIOx_Set->CRL &= ~(0b0100<<(Position*4));                                            // Выставляем порт в исходное состояние как при сбросе
        if(IO_Set)
        {
          _GPIOx_Set->CRL |= (0b0011<<(Position*4));                                           // Настраиваем на выход с максимальной скоростью в режиме PUSH/PULL
        }
        else
        {
          switch (PULL_Set)
          {
            case No_Pull:
              _GPIOx_Set->CRL |= (0b0100<<(Position*4));
              break;
            case Pull_Up:
              _GPIOx_Set->CRL |= (0b1000<<(Position*4));
              _GPIOx_Set->ODR |= (1<<Position);
              break;
            case Pull_Down:
              _GPIOx_Set->CRL |= (0b1000<<(Position*4));
              _GPIOx_Set->ODR &= ~(1<<Position);
              break;
            default:
              break;
          }
        }
      }
      else
      {
        _GPIOx_Set->CRH &= ~(0b0100<<(Position*4));                                            // Выставляем порт в исходное состояние как при сбросе
        if(IO_Set)
        {
          _GPIOx_Set->CRH |= (0b0011<<(Position*4));                                           // Настраиваем на выход с максимальной скоростью в режиме PUSH/PULL
        }
        else
        {
          switch (PULL_Set)
          {
            case No_Pull:
              _GPIOx_Set->CRH |= (0b0100<<(Position*4));
              break;
            case Pull_Up:
              _GPIOx_Set->CRH |= (0b1000<<(Position*4));
              _GPIOx_Set->ODR |= (1<<Position);
              break;
            case Pull_Down:
              _GPIOx_Set->CRH |= (0b1000<<(Position*4));
              _GPIOx_Set->ODR &= ~(1<<Position);
              break;
            default:
              break;
          }
        }
      }
//      if(IO_Set)                                                                // INPUT = 0, OUTPUT = 1
//      {
//        _GPIOx_Set->OSPEEDR &= ~(0b11 << (Position * 2));                       // Устанавливаем низкую скорость порта
//        _GPIOx_Set->OSPEEDR |=  _GPIO_Speed << (Position * 2);
//        _GPIOx_Set->OTYPER  &= ~(1U << Position);                               // Устанавливаем тип вывода push-pull
//        _GPIOx_Set->OTYPER |= (_TypeOut << Position);
//      }
//
//      switch (PULL_Set)
//      {
//        case No_Pull:
//          _GPIOx_Set->PUPDR &= ~(0b11 << (Position * 2));                       // Сбрасываем регистр подтяжки
//          break;
//        case Pull_Up:                                                           // Подтягиваем к верху
//          _GPIOx_Set->PUPDR &= ~(0b11 << (Position * 2));
//          _GPIOx_Set->PUPDR |=  (Pull_Up << (Position * 2));
//          break;
//        case Pull_Down:                                                         // Подтягиваем к низу
//          _GPIOx_Set->PUPDR &= ~(0b11 << (Position * 2));
//          _GPIOx_Set->PUPDR |=  (Pull_Down << (Position * 2));
//          break;
//        default:
//          break;
//      }
//
//      _GPIOx_Set->MODER &= ~(0b11 << (Position * 2));                           // Сбрасываем порт на вход
//      _GPIOx_Set->MODER |=  (IO_Set << (Position * 2));                         // Если ф-я выхода, то выход
    }
  }
}

//  if(I<8)
//  {
//    _GPIOx->CRL &= ~(0b0100<<(I*4));                                            // Выставляем порт в исходное состояние как при сбросе
//    if(_IO)
//    {
//      _GPIOx->CRL |= (0b0011<<(I*4));                                           // Настраиваем на выход с максимальной скоростью в режиме PUSH/PULL
//    }
//    else
//    {
//      switch (PULL)
//      {
//        case NO_PULL:
//          _GPIOx->CRL |= (0b0100<<(I*4));
//          break;
//        case PULL_UP:
//          _GPIOx->CRL |= (0b1000<<(I*4));
//          _GPIOx->ODR |= (1<<I);
//          break;
//        case PULL_DOWN:
//          _GPIOx->CRL |= (0b1000<<(I*4));
//          _GPIOx->ODR &= ~(1<<I);
//          break;
//        default:
//          break;
//      }
//    }
//  }
//  else
//  {
//    _GPIOx->CRH &= ~(0b0100<<((I-8)*4));                                            // Выставляем порт в исходное состояние как при сбросе
//    if(_IO)
//    {
//      _GPIOx->CRH |= (0b0011<<((I-8)*4));                                           // Настраиваем на выход с максимальной скоростью в режиме PUSH/PULL
//    }
//    else
//    {
//      switch (PULL)
//      {
//        case NO_PULL:
//          _GPIOx->CRH |= (0b0100<<((I-8)*4));
//          break;
//        case PULL_UP:
//          _GPIOx->CRH |= (0b1000<<((I-8)*4));
//          _GPIOx->ODR |= (1<<I);
//          break;
//        case PULL_DOWN:
//          _GPIOx->CRH |= (0b1000<<((I-8)*4));
//          _GPIOx->ODR &= ~(1<<I);
//          break;
//        default:
//          break;
//      }
//    }
//  }


//******************************************************************************
//  ENF OF FILE
//******************************************************************************

