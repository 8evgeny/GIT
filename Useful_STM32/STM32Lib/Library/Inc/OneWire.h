/*
 * Библиотека 1-Wire
 * Основана на библиотеке от Arduino
 * Автор  : "Paul Stoffregen",
 * E-Mail : "paul@pjrc.com",
 * url    : "http://www.pjrc.com",
 * url    : "https://github.com/PaulStoffregen/OneWire"
 * Версия : "2.3.2"
 *
 * Функции
 * void Init(GPIO_TypeDef *GPIO_Port, uint16_t GPIO_Pin)                        // Инициализация портов и периферии МК. GPIO_Port - порт к которому подключены
 *                                                                              // DS18B20, GPIO_Pin - пин подключения
 * uint8_t reset(void)                                                          // Спрос и определение устройств на линии
 * void write_bit(uint8_t v)                                                    // Запись бита на линию
 * void    select(const uint8_t rom[8]);                                        // Выбор конкретного датчика по адресу
 * void    skip(void);                                                          //
 * void    write_bit(uint8_t v);                                                // Запись бита
 * void    write(uint8_t v, uint8_t power = 0);                                 // Запись байта
 * void    write_bytes(const uint8_t *buf, uint16_t count, bool power = 0);     // Запись буфера
 * uint8_t read_bit(void);                                                      // Чтение бита
 * uint8_t read(void);                                                          // Чтение байта
 * void    read_bytes(uint8_t *buf, uint16_t count);                            // Чтение буфера
 * void    depower(void);
 * uint8_t search(uint8_t *newAddr, bool search_mode = true);                   // Поиск и считывание датчика. Не проверялась на нескольких датчиках одновременно
 * static uint8_t crc8(const uint8_t *addr, uint8_t len);                       // Подсчёт контрольной суммы
 *
 * Не ипользованные функции ввиду работы с одним типом датчика. Другие 1-Wire устройсва не проверялись
 * Если нужно, можно в будущем дописать. DS разных много. Нужно проверить работу на RFID и других DS
 * void    reset_search(void);                                                  // Обнуление переменных используемых в библиотеке джо исходного состояния
 * void    target_search(uint8_t family_code);                                  // Установка переменных в соответствии типа датчика
 *
 * Функции запущены и проверены не все а только те, которые необходимы для поска устройств и замера температуры
 *
 *
 *
 */
#ifndef OneWire_h
#define OneWire_h

#include "STM32.h"
#include <inttypes.h>

// Функции чтения, записи, установки вход/выход - зависимые от типа МК
#define DIRECT_MODE_INPUT   (_GPIO_Port->MODER &= ~(0x11 << (_GPIO_Pin_Pos * 2)))
#define DIRECT_MODE_OUTPUT  {_GPIO_Port->MODER &= ~(0x11 << (_GPIO_Pin_Pos * 2)); _GPIO_Port->MODER |= (0x01 << (_GPIO_Pin_Pos * 2));}
#define DIRECT_READ         ((_GPIO_Port->IDR & _GPIO_Pin) ? 1 : 0)
#define DIRECT_WRITE_LOW    (_GPIO_Port->BSRR |= (_GPIO_Pin << 16))
#define DIRECT_WRITE_HIGH   (_GPIO_Port->BSRR |= _GPIO_Pin)

enum Resolution
{
  Res_9 = 9,
  Res_10 = 10,
  Res_11 = 11,
  Res_12 = 12,
};

class OneWire
{
  public:
    OneWire();
    void    Init(GPIO_TypeDef *GPIO_Port, uint16_t GPIO_Pin);
    uint8_t reset(void);
    void    select(const uint8_t rom[8]);
    void    skip(void);
    void    write_bit(uint8_t v);
    void    write(uint8_t v, uint8_t power = 0);
    void    write_bytes(const uint8_t *buf, uint16_t count, bool power = 0);
    uint8_t read_bit(void);
    uint8_t read(void);
    void    read_bytes(uint8_t *buf, uint16_t count);
    void    depower(void);

    void    reset_search(void);
    void    target_search(uint8_t family_code);
    uint8_t search(uint8_t *newAddr, bool search_mode = true);

    static uint8_t crc8(const uint8_t *addr, uint8_t len);


  private:
    uint8_t       _ROM_NO[8];
    uint8_t       _LastDiscrepancy;
    uint8_t       _LastFamilyDiscrepancy;
    uint8_t       _LastDeviceFlag;
    GPIO_TypeDef  *_GPIO_Port;
    uint16_t      _GPIO_Pin;
    uint8_t       _GPIO_Pin_Pos = 0;
};

#endif
