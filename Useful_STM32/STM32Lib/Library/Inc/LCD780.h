/*
 * Библиотека для однострочных, двустрочных, четырёх строчных экранов.
 * Не реализованы функции скроллинга и создания символа
 * Нет руссификации.
 *
 * По мотивам сатьи с сайта http://mypractic.ru/
 * Урок №25
 */

/* Проверка, что библиотека не подключена */
#ifndef _LCD780_H
#define _LCD780_H

// Подключаем для контроллера
#include "STM32.h"

#define DEC 10
#define HEX 16
#define BIN 2

class LCD780
{
  public:
    LCD780(GPIO_TypeDef *GPIOx_RS, uint16_t GPIO_Pin_RS,                        // 4й пин - DI
           GPIO_TypeDef *GPIOx_EN, uint16_t GPIO_Pin_EN,                        // 6й пин - E
           GPIO_TypeDef *GPIOx_RW, uint16_t GPIO_Pin_RW,                        // 5й пин - RW
           GPIO_TypeDef *GPIOx_D4, uint16_t GPIO_Pin_D4,
           GPIO_TypeDef *GPIOx_D5, uint16_t GPIO_Pin_D5,
           GPIO_TypeDef *GPIOx_D6, uint16_t GPIO_Pin_D6,
           GPIO_TypeDef *GPIOx_D7, uint16_t GPIO_Pin_D7,
           uint8_t col, uint8_t row);                           // конструктор

    /*
     *  Описание функций находящихся в библиотеке
     */
    // Комнды
    void init(void);
    void home(void);                                                            // Курсор на начало первой строки
    void setCursor (uint8_t col, uint8_t row);                                  // установка курсора в позицию
    void displayOFF();                                                          // выключение дисплея
    void displayON();                                                           // включение дисплея
    void cursorOFF();                                                           // выключение курсора
    void cursorON();                                                            // включение курсора
    void cursorBlinkOFF();                                                      // выключение мигания курсора
    void cursorBlinkON();                                                       // включение мигания курсора
// Не реализованы
    //void scrollLeft(); // сдвиг влево
    //void scrollRight(); // сдвиг вправо
    //void scrollON(); // автопрокрутка включена
    //void scrollOFF(); // автопрокрутка выключена
    //void leftToRight(); // слева направо
    //void rightToLeft(); // справо налево
    //void createChar(uint8_t location, uint8_t charmap[]); // Создание пользовательского символа

    // Функции вывода
    void writeChar(uint8_t valChar);                                            // Вывод символа на экран
    void writeBuf(uint8_t * buf, uint16_t length);                              // вывод массива символов на экран
    void printUint(uint32_t n, uint8_t base);                                   // вывод на экран uint32_t. BIN, DEC, HEX формат
    void printInt(int32_t n, uint8_t base);                                     // вывод на экран int32_t. тоже что и printUint, со знаком
    void printString(const char str[]);                                         // вывод строки
    void printFloat(float number, uint8_t digits);                              // вывод числа float, digit - кол-во знаков после запятой с округлением


  private:                                                                       // Приватные переменные и функции
    GPIO_TypeDef *_GPIOx_D4, *_GPIOx_D5, *_GPIOx_D6, *_GPIOx_D7, *_GPIOx_EN, *_GPIOx_RS, *_GPIOx_RW, *_GPIOx_Curr;
    uint16_t  _GPIO_Pin_RS, _GPIO_Pin_EN, _GPIO_Pin_RW, _GPIO_Pin_D4, _GPIO_Pin_D5, _GPIO_Pin_D6, _GPIO_Pin_D7, _GPIO_Pin_Curr;
    uint8_t _col;
    uint8_t _row;
    uint8_t _cirill;

    uint8_t _displaycontrol;
    uint8_t _displaymode;

    uint16_t _GPIO_Pin;                                                          // маска вывода
    uint32_t _filterTime;                                                        // время фильтрации
    volatile uint32_t _filterTimeCount;                                          // счетчик времени фильтрации

    void writeByte(uint8_t dt);
    void _SetPin(GPIO_TypeDef *GPIOx_Curr, uint16_t GPIO_Pin_Curr);

    const uint8_t cirillAsciiTable[128] = {
    0xc8, 0xc9, 0xca, 0xcb, 0xcc, 0xcd, 0xce, 0xcf,
    0xd0, 0xd1, 0xd2, 0xd3, 0xd4, 0xd5, 0xd6, 0xd7,
    0xd8, 0xd9, 0xda, 0xdb, 0xdc, 0xdd, 0xde, 0xdf,
    0xe7, 0xe8, 0xe9, 0xea, 0xeb, 0xec, 0xed, 0xee,
    0xef, 0xf0, 0xf1, 0xf2, 0xf3, 0xf4, 0xf5, 0xf6,
    0xa2, 0xf7, 0xf8, 0xf9, 0xfa, 0xfb, 0xfc, 0xfd,
    0xfe, 0xff, 0xe7, 0xe8, 0xe9, 0xea, 0xeb, 0xec,
    0xb5, 0xee, 0xef, 0xf0, 0xf1, 0xf2, 0xfd, 0xff,
    0x41, 0xa0, 0x42, 0xa1, 0xe0, 0x45, 0xa3, 0xa4,
    0xa5, 0xa6, 0x4b, 0xa7, 0x4d, 0x48, 0x4f, 0xa8,
    0x50, 0x43, 0x54, 0xa9, 0xaa, 0x58, 0xe1, 0xab,
    0xac, 0xe2, 0xad, 0xae, 0x62, 0xaf, 0xb0, 0xb1,
    0x61, 0xb2, 0xb3, 0xb4, 0xe3, 0x65, 0xb6, 0xb7,
    0xb8, 0xb9, 0xba, 0xbb, 0xbc, 0xbd, 0x6f, 0xbe,
    0x70, 0x63, 0xbf, 0x79, 0xe4, 0x78, 0xe5, 0xc0,
    0xc1, 0xe6, 0xc2, 0xc3, 0xc4, 0xc5, 0xc6, 0xc7 };

};

#endif /* _H */
