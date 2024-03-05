// Блокируем повторное включение этого модуля
#ifndef IO_DIGITAL_H
#define IO_DIGITAL_H

#include <stdint.h>
#include <STM32.h>

enum TypeOut                  // Тип выхода. OpenDrain/PushPull
{
  Output            = 0b00,
  Out_OpenDrain     = 0b01,
  Out_Alt_PushPull  = 0b10,
  Out_Alt_OpenDrain = 0b11
};

enum Speed_IO                 // Скорость порта
{
  Speed_Low      = 0b10,
  Speed_Medium   = 0b01,
  Speed_High     = 0b11,
  Speed_WeryHigh = 0b11
};

enum PULL_IO                  // Подтяжка выхода
{
  No_Pull     = 0b00,
  Pull_Up     = 0b01,
  Pull_Down   = 0b10
};

enum IO_Type                  // Тип выхода
{
  In_Analog      = 0b00,
  In_Float_Input = 0b01,
  Input  = 0b10
};

enum IO_Level                 // Просто уровень. 1 или 0
{
  Low   = false,
  High  = true
};

// Функции инициализации пинов
void _SetPin(GPIO_TypeDef *GPIOx_Set, uint16_t GPIO_Pin_Set, uint8_t IO_Set, uint8_t PULL_Set);     // Инициализация одного или нескольких пинов
void _SetPinAlternate(GPIO_TypeDef *GPIOx_Set, uint16_t GPIO_Pin_Set, uint8_t Alternate_Set);       // Инициализация алтернативной функции одного или нескольких пинов

// Функции изменения переменных инициализации. В обязательном порядке вызываются перед функциями
// _SetPin и _SetPinAlternate
void _SetSpeed(uint8_t Speed_Set);                                                                  // Установка скорости порта
void _SetPull(uint8_t Pull_Set);                                                                    // Установка подтяжки
void _SetTypeOut(uint8_t TypeOutput);                                                               // Тип выхода
void _SetEdge(bool Edge);                                                                           // Установка подтяжки

// Работа с битами
bool _DigitalReadBit(GPIO_TypeDef *GPIOx_Set, uint16_t GPIO_Pin_Set);                               // Чтение бита
void _DigitalWriteBit(GPIO_TypeDef *GPIOx_Set, uint16_t GPIO_Pin_Set, bool Level);                  // Запись бита

// Работа с данными с разрядностью от 2х до 16ти.
void _DigitalWrite(GPIO_TypeDef *GPIOx_Set, uint16_t Data, uint8_t FirstPin, uint16_t PinMask);     // Запись 16 разрядных данных
uint16_t _DigitalRead(GPIO_TypeDef *GPIOx_Set, uint8_t FirstPin, uint16_t PinMask);                 // Чтение 16 разрядных данных

void _SetDebug_SW(void);                                                                            // Отключение JTAG отладки. Пока не реализована.


// Закрывающий #endif к блокировке повторного включения
#endif  /* MAIN_H */

//******************************************************************************
//  ENF OF FILE
//****************************************************************************** /*
