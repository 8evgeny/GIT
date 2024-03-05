/*
 * SDallas.cpp
 *
 *  Created on: 13 апр. 2021 г.
 *      Author: User
 */

#include <SDallas.h>
#include "memory.h"


SDallas::SDallas()
{
//  _GPIO_Port  = PORT;
//  _GPIO_Pin   = Pin;
}

void SDallas::getAddress(uint8_t *addressArray)
{
  if(_ds_address_defined || !reset()) return;               // Проверка присутствия
  write(0x33, _Fantom_Power);                                              // Запрос адреса
  uint8_t _calculated_crc = 0;                              // Переменная для CRC8
  uint8_t _temp_address[8];                                 // Временный массив для адреса
  read_bytes(_temp_address, 8);                             // Записать байты во временный массив
  _calculated_crc = crc8(_temp_address, 7);                 // Обновить значение CRC8
  if(_calculated_crc != _temp_address[7]) return;           // Если CRC не сошелся - данные в помойку
  memcpy(addressArray, _temp_address, 8);                   // Если сошелся - переписать массив в основной
}

// Установка адреса
void SDallas::setAddress(uint8_t *address)
{
  _ds_address = address;
  _ds_address_defined = true;
}

// Установка рабочего разрешения
void SDallas::setResolution(uint8_t resolution)
{
  if(!reset()) return;                                      // Сброс и проверка присутствия
  addressRoutine();                                         // Процедура адресации
  write(0x4E, _Fantom_Power);                                              // Запись RAM
  write(0xFF, _Fantom_Power);                                              // Максимум в верхний регистр тревоги
  write(0x00, _Fantom_Power);                                              // Минимум в верхний регистр тревоги
  write(((constrain(resolution, 9, 12) - 9) << 5) | 0x1F, _Fantom_Power);  // Запись конфигурации разрешения
}

// Процедура адресации
void SDallas::addressRoutine(void)
{
  if(_ds_address_defined)                                   // Адрес определен?
  {
    write(0x55, _Fantom_Power);                                            // Говорим термометрам слушать адрес
    {
      write_bytes(_ds_address, 8, _Fantom_Power);                   // Отправляем 8 байт уникального адреса
    }
  }
  else write(0xCC, _Fantom_Power);                                         // Адреса нет - пропускаем адресацию на линии
}

// Запрос температуры
void SDallas::requestTemp(void)
{
  if(!reset()) return;                                      // Проверка присутствия
  skip();
//  addressRoutine();                                         // Процедура адресации
  write(0x44, _Fantom_Power);                                              // Запросить преобразование
}

// Чтение температуры
DS_TEMP_TYPE SDallas::getTemp(void)
{
  uint8_t data[9];                                          // Временный массив для данных
  uint8_t _calculated_crc = 0;                              // Переменная для хранения CRC
  if(!reset()) return 0;                                    // Проверка присутствия
  addressRoutine();                                         // Процедура адресации
  write(0xBE, _Fantom_Power);                               // Запросить температуру
  read_bytes(data, 9);                                      // Записать байты во временный массив
  _ds_address_defined = false;                              // Сбрасываем флаг адреса на случай запроса безадресной температуры
  _calculated_crc = crc8(data, 8);                          // Обновить значение CRC8
  if(_calculated_crc != data[8]) return -155;//0.0;                // Если CRC не сошелся - данные в помойку
  return (DS_TEMP_TYPE) ((data[1] << 8) | data[0]) / 16;    // Рассчитать значение температуры
}

// Чтеие температуры с определённого датчика
DS_TEMP_TYPE SDallas::getTemp(uint8_t *address)
{
  setAddress(address);
  uint8_t data[9];                                          // Временный массив для данных
  uint8_t _calculated_crc = 0;                              // Переменная для хранения CRC
  if(!reset()) return 0;                                    // Проверка присутствия
  addressRoutine();                                         // Процедура адресации
  write(0xBE, _Fantom_Power);                                              // Запросить температуру
  read_bytes(data, 9);                                      // Записать байты во временный массив
  _calculated_crc = crc8(data, 8);                          // Обновить значение CRC8
  if(_calculated_crc != data[8]) return -155;// 0.0;                // Если CRC не сошелся - данные в помойку
  return (DS_TEMP_TYPE) ((data[1] << 8) | data[0]) / 16;    // Рассчитать значение температуры
}
