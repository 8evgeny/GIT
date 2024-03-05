/*
 * Библиотека для работы с датчиком DS18B20
 * Разработана на основе библиотеки Гайвера microDS18B20
 * Но библиотека 1-Wire взята от Arduino
 ************************************************************************
 * Light & powerful Dallas 1-Wire DS18B20 thermometer arduino library   *
 * Designed specifically for AlexGyver by Egor 'Nich1con' Zakharov      *
 * V2.0 from 23.02.2020                                                 *
 * V2.1 from 10.04.2020                                                 *
 * V2.2 from 26.04.2020                                                 *
 ************************************************************************
 *
 * Все функции библиотеки сохранены
 * Изменены незначительно работа с СRC и убраны некоторые настройки
 * - Температура всегда считывается во float
 * - CRC считается всегда и по формуле
 */

#ifndef SDALLAS_H_
#define SDALLAS_H_

#include "STM32.h"
#include "stdio.h"
#include "OneWire.h"
#include "functions.h"

/*************************************************** Настройки ***************************************************/
#define DS_TEMP_TYPE float      // float/int - тип данных для рассчета температуры float / int <- (экономит flash, выполняется быстрее)
//#define DS_CHECK_CRC true     // true/false - проверка контрольной суммы принятых данных <- надежнее, но тратит немного больше flash
//#define DS_CRC_USE_TABLE false   // true/false - использовать готовую таблицу контрольной суммы <- значительно быстрее, +256 байт flash

/************************************************* Класс **************************************************/

class SDallas : public OneWire
{
  public:
    SDallas();                                              // Создать обьект термометра без адресации

    void setResolution(uint8_t resolution);                 // Установить разрешение термометра 9-12 бит
    void getAddress(uint8_t *addressArray);                 // Прочитать уникальный адрес термометра в массив
    void setAddress(uint8_t *address);                      // Установить адрес

    void requestTemp(void);                                 // Запросить новое преобразование температуры
    DS_TEMP_TYPE getTemp(void);                             // Прочитать значение температуры
    DS_TEMP_TYPE getTemp(uint8_t *address);                 // Прочитать значение температуры определённого датчика

  private:
    void addressRoutine(void);                              // Процедура адресации / пропуска адресации

    bool          _ds_address_defined = false;              // Флаг - определен ли адрес?
    uint8_t       *_ds_address;                             // Указатель на адрес термометра
    GPIO_TypeDef  *_GPIO_Port;
    uint16_t      _GPIO_Pin;
    bool          _Fantom_Power = true;
};



#endif /* INC_SDALLAS_H_ */
