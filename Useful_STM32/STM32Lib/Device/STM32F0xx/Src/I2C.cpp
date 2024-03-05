/*
 * I2C.cpp
 *
 *  Created on: 12 нояб. 2021 г.
 *      Author: User
 */

#include  "I2C.h"      // Включаем файл заголовка для нашего модуля 

I2C::I2C(I2C_TypeDef *I2C_Port, uint8_t Speed = I2C_Standart_Mode, uint8_t Mode = I2C_Master, uint8_t Alternate = true)
{
  uint8_t _PRESC, _SCLL, _SCLH, _SDADEL, _SCLDEL;

  _I2C_Port     = I2C_Port;
  _SpeedMode    = Speed;
  _TransferMode = Mode;
  _Alternate    = Alternate;

  // Частота тактирования модуля
//  RCC->CFGR3 |= RCC_CFGR3_I2C1SW;                           // Переключаем тактирование I2C на тактирование от 48МГц

  // Инициализация портов и тактирования I2C
  if(_I2C_Port == I2C1)
  {
    RCC->APB1ENR |= RCC_APB1ENR_I2C1EN;                                         // Включаем тактирование модуля
    _SetSpeed(Speed_High);                                                      // Скорость GPIO самая высокая
//    _SetPull(Pull_Up);                                                          // Подтяжка портов к питанию
    _SetTypeOut(Out_Open);

    if(_Alternate)                                                              // Назначаем выводы GPIO
      _SetPinAlternate(GPIOA, GPIO_PIN_9 | GPIO_PIN_10, GPIO_AF4_I2C1);         // Порт A - альтернативный, на случай если F занят кварцевым резонатором
    else
      _SetPinAlternate(GPIOF, GPIO_PIN_0 | GPIO_PIN_1, GPIO_AF4_I2C1);          // Порт F - основной
    _IRQn = I2C1_IRQn;
  }
#if defined (STM32F070xB) || (STM32F030x8) || (STM32F030xC)
  else
  {
    RCC->APB1ENR |= RCC_APB1ENR_I2C2EN;                                         // Здесь тактирование для второго порта
    _SetSpeed(Speed_High);                                                      // Скорость GPIO самая высокая
    _SetPull(Pull_Up);                                                          // Подтяжка портов к питанию
    if(_Alternate)                                                              // Назначаем выводы GPIO
      _SetPinAlternate(GPIOB, GPIO_PIN_13 | GPIO_PIN_14, GPIO_AF1_I2C2);        // Порт B - альтернативный, на случай если F занят кварцевым резонатором
    else
      _SetPinAlternate(GPIOB, GPIO_PIN_10 | GPIO_PIN_11, GPIO_AF1_I2C2);        // Порт B - основной
    _IRQn = I2C2_IRQn;
  }
#endif

  //AU_RST_HIGH;
  // Инициализация модуля в режиме I2C


  // Режим стандарт, быстый или супер быстрый
  // Частота SCL
  switch (_SpeedMode)
  {
    case I2C_Standart_Mode:
      _SCLL   = 0x5D;//0x13;
      _SCLH   = 0x3E;//0x0F;
      _SDADEL = 0x00;//0x02;
      _SCLDEL = 0x03;//0x04;
      _PRESC  = 0x02;//0x0B;
      break;
    case I2C_Fast_Mode:
      _SCLL   = 0x09;
      _SCLH   = 0x03;
      _SDADEL = 0x03;
      _SCLDEL = 0x03;
      _PRESC  = 0x05;
      break;
    case I2C_Fast_ModePlus:
      _SCLL   = 0x03;
      _SCLH   = 0x01;
      _SDADEL = 0x00;
      _SCLDEL = 0x01;
      _PRESC  = 0x05;
      break;
    default:
      break;
  }
  _I2C_Port->TIMINGR |= _PRESC  << I2C_TIMINGR_PRESC_Pos;   // Фаршируем регистр таймингов. Константы взяты из Reference Manual
  _I2C_Port->TIMINGR |= _SCLL   << I2C_TIMINGR_SCLL_Pos;
  _I2C_Port->TIMINGR |= _SCLH   << I2C_TIMINGR_SCLH_Pos;
  _I2C_Port->TIMINGR |= _SDADEL << I2C_TIMINGR_SDADEL_Pos;
  _I2C_Port->TIMINGR |= _SCLDEL << I2C_TIMINGR_SCLDEL_Pos;

  // включение модуля
  _I2C_Port->OAR1 &= ~I2C_OAR1_OA1EN;
  _I2C_Port->OAR1 |= I2C_OAR1_OA1EN;
  _I2C_Port->CR2  |= I2C_CR2_AUTOEND;
  _I2C_Port->CR2  |= I2C_CR2_NACK;
  _I2C_Port->CR1  |= I2C_CR1_PE;
}

/*
 * Функции приёма/передачи
 *
 */
void I2C::send(uint8_t Address, uint8_t Data)
{
//  _I2C_Port->CR2 = I2C_CR2_AUTOEND | (1<<16) | (Address << 1) | I2C_WRITE;
  if ((_I2C_Port->ISR & I2C_ISR_TXE) == I2C_ISR_TXE)
  {
//    _I2C_Port->TXDR = Data; /* Byte to send */
//    _I2C_Port->CR2 |= I2C_CR2_START; /* Go */
    MODIFY_REG(_I2C_Port->CR2,
                ((I2C_CR2_SADD | I2C_CR2_NBYTES | I2C_CR2_RELOAD | I2C_CR2_AUTOEND | \
                  (I2C_CR2_RD_WRN & (uint32_t)(I2C_CR2_START >> (31U - I2C_CR2_RD_WRN_Pos))) | I2C_CR2_START | I2C_CR2_STOP)), \
                (uint32_t)(((uint32_t)Address & I2C_CR2_SADD) |
                           (((uint32_t) 1 << I2C_CR2_NBYTES_Pos) & I2C_CR2_NBYTES) | I2C_CR2_AUTOEND | I2C_CR2_START));
    _I2C_Port->ISR &= ~I2C_ISR_STOPF;
    _I2C_Port->CR2 &= (uint32_t)~((uint32_t)(I2C_CR2_SADD | I2C_CR2_HEAD10R | I2C_CR2_NBYTES | I2C_CR2_RELOAD | I2C_CR2_RD_WRN));
  }
}
