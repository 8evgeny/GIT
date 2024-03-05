/*
 * IO_Digital.cpp
 *
 *  Created on: 10 февр. 2022 г.
 *      Author: User
 */

#include <IO_Digital.h>

// Конструктор для обычной инициализации выводов
IO_Digital::IO_Digital(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin, bool IO, uint8_t PULL)
{
  _GPIOx = GPIOx;                                                     //  Запоминаем: Номер порта
  _GPIO_Pin = GPIO_Pin;                                               // Номер вывода
  _IO = IO;                                                           // Тип вывода Вход/выход и т.д
  _PULL = PULL;                                                       // Подтяжку
  _SetPin(_GPIOx, _GPIO_Pin, _IO, _PULL);                             // Вызываем инициализацию
}

//  Конструктор для инициализации выводов с предварительной установкой выходного уровня
IO_Digital::IO_Digital(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin, bool Edge)
{
  _SetEdge(Edge);                                                     // Говорим, какой уровень нам нужен после инициализации
  _GPIOx = GPIOx;                                                     //  Запоминаем: Номер порта
  _GPIO_Pin = GPIO_Pin;                                               // Номер вывода
  _IO = Output;                                                       // Тип вывода Вход/выход и т.д
  _PULL = No_Pull;                                                    // Подтяжку
  _SetPin(_GPIOx, _GPIO_Pin, _IO, _PULL);                             // Вызываем инициализацию
  _SetEdge(Low);                                                      // Сбрасываем переменную к значению по умолчанию
}

// Конструктор для параллельного интерфейса
IO_Digital::IO_Digital(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin, uint8_t Size, bool IO, uint8_t PULL)
{
  uint16_t Pins = 0;
  uint16_t PinShift = GPIO_Pin;
  _GPIOx = GPIOx;
  _FirstPin = 0;

  while(PinShift)                                                     // Подсчитываем номер пина с которого начинается вывод
  {
    _FirstPin++;
    PinShift = PinShift >> 1;
  }
  _FirstPin--;

  while(Size--)                                                       // Собираем список инициализируемых пинов
  {
    Pins = Pins << 1;
    Pins |= GPIO_Pin;
  }
  _PinMask = Pins;
  IO_Digital(GPIOx, Pins, IO, PULL);                                  // Инициализируем их
}

void IO_Digital::digitalWrite(bool DWrite)
{
  _DigitalWriteBit(_GPIOx, _GPIO_Pin, DWrite);                        // Вызываем нашу старую знакомую функцию записи бита
}

bool IO_Digital::digitalRead()
{
  return _DigitalReadBit(_GPIOx, _GPIO_Pin);                          // Вызываем функцию чтения бита
}

void IO_Digital::digitalTogglePin(void)
{
  if(_DigitalReadBit(_GPIOx, _GPIO_Pin))                              // Проверяем в каком состоянии вывод на данный момент
    _DigitalWriteBit(_GPIOx, _GPIO_Pin, Low);                         // Если был 1, сбрасываем в 0
  else
    _DigitalWriteBit(_GPIOx, _GPIO_Pin, High);                        // Если был 0, устанавливаем в 1
}

// Пишется выбранное количество бит в соответствующий пулл выводов
void IO_Digital::digitalWriteByte(uint16_t Data)
{
  _DigitalWrite(_GPIOx, Data, _FirstPin, _PinMask);
}

uint16_t IO_Digital::digitalReadByte(void)
{
  return _DigitalRead(_GPIOx, _FirstPin, _PinMask);
}

void IO_Digital::setInput(void)
{
  _SetPin(_GPIOx, _PinMask, Input, _PULL);                           // Инициализируем порты на вход
}

void IO_Digital::setOutput(void)
{
  _SetPin(_GPIOx, _PinMask, Output, _PULL);                          // Инициализируем порты на выход
}
