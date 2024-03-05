/*
 * IO_Digital.h
 *
 *  Created on: 10 февр. 2022 г.
 *      Author: User
 */

#ifndef IO_DIGITAL_H_
#define IO_DIGITAL_H_

#include "STM32.h"

class IO_Digital
{
  public:
    IO_Digital(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin, bool IO, uint8_t PULL);              // Инициализация порта
    IO_Digital(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin, bool Edge);                          // Инициализация порта на выход с предустановкой его в одно из состояний
    IO_Digital(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin, uint8_t Size, bool IO, uint8_t PULL);// Инициализаровать последовательность выводов порта GPIOx, начиная с GPIO_Pin, количество Size
    void      digitalWrite(bool DWrite);                                                    // Вывод на пин 0 или 1
    bool      digitalRead(void);                                                            // Чтение состояния пина
    void      digitalTogglePin(void);                                                       // Переключение пина в противоположное состояние
    void      digitalWriteByte(uint16_t Data);
    uint16_t  digitalReadByte(void);
    // Дополнительные функции используемые в двунаправленных интерфейсах
    void      setInput(void);                                                               // Переводит ранее пронициализированные выводы на вход
    void      setOutput(void);                                                              // Переводит ранее проинициализированные выводы на выход

  private:
    GPIO_TypeDef  *_GPIOx;                                                                  // Переменная содержащая Имя порта
    uint16_t      _GPIO_Pin;                                                                // Переменная содержащая позицию вывода
    bool          _IO;                                                                      // Переменная хранит тип - вход/выход.
    uint8_t       _PULL;                                                                    // Переменная хранит тип подтяжки VCC/GND/NONE
    uint8_t       _FirstPin = 0;                                                            // Переменная хранит номер первого пина при групповой инициализации
    uint16_t      _PinMask = 0;                                                             // Переменная хранит список инициализируемых выводов
};


#endif /* IO_DIGITAL_H_ */
