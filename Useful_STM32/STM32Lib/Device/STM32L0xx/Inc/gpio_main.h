/*
 * GPIO.h
 *
 *  Created on: 5 апр. 2021 г.
 *      Author: User
 */

#ifndef GPIO_MAIN_H
#define GPIO_MAIN_H

#include "stdint.h"
#include "STM32.h"

enum TypeOut                  // Тип выхода. OpenDrain/PushPull
{
  Out_Normal,
  Out_Open
};

enum Speed_IO                 // Скорость порта
{
  Speed_Low      = 0b00,
  Speed_Medium   = 0b01,
  Speed_High     = 0b11
};

enum PULL_IO
{
  No_Pull     = 0b00,
  Pull_Up     = 0b01,
  Pull_Down   = 0b10
};

enum IO_Type
{
  Input       = 0b00,
  Output      = 0b01,
  Analog      = 0b11
};

enum IO_Level                 // Просто уровень. 1 или 0
{
  Low   = false,
  High  = true
};

// Функции инициализации пинов
void _SetPin(GPIO_TypeDef *GPIOx_Set, uint16_t GPIO_Pin_Set, uint8_t IO_Set, uint8_t PULL_Set);     // Инициализация одного или нескольких пинов
void _SetPinAlternate(GPIO_TypeDef *GPIOx_Set, uint16_t GPIO_Pin_Set, uint8_t Alternate_Set);       // Инициализация алтернативной функции одного или нескольких пинов

// Функции изменения переменных инициализации
void _SetSpeed(uint8_t Speed_Set);                                                                  // Установка скорости порта
void _SetPull(uint8_t Pull_Set);                                                                    // Установка подтяжки
void _SetTypeOut(uint8_t TypeOutput);                                                               // Тип выхода
void _SetEdge(bool Edge);

// Работа с битами
bool _DigitalReadBit(GPIO_TypeDef *GPIOx_Set, uint16_t GPIO_Pin_Set);                               // Чтение бита
void _DigitalWriteBit(GPIO_TypeDef *GPIOx_Set, uint16_t GPIO_Pin_Set, bool Level);                  // Запись бита

// Работа с данными с разрядностью от 2х до 16ти.
void _DigitalWrite(GPIO_TypeDef *GPIOx_Set, uint16_t Data, uint8_t FirstPin, uint16_t PinMask);     // Запись 16 разрядных данных
uint16_t _DigitalRead(GPIO_TypeDef *GPIOx_Set, uint8_t FirstPin, uint16_t PinMask);                 // Чтение 16 разрядных данных

void _SetDebug_SW(void);                                                                            // Отключение JTAG отладки. Пока не реализована.

#endif /* DEVICE_STM32F4XX_INC_GPIO_H_ */
