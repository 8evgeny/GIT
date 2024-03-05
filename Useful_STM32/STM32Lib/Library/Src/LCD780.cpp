#include "LCD780.h"

//----------------- методы класса

// конструктор
LCD780::LCD780(GPIO_TypeDef *GPIOx_RS, uint16_t GPIO_Pin_RS,
               GPIO_TypeDef *GPIOx_EN, uint16_t GPIO_Pin_EN,
               GPIO_TypeDef *GPIOx_RW, uint16_t GPIO_Pin_RW,
               GPIO_TypeDef *GPIOx_D4, uint16_t GPIO_Pin_D4,
               GPIO_TypeDef *GPIOx_D5, uint16_t GPIO_Pin_D5,
               GPIO_TypeDef *GPIOx_D6, uint16_t GPIO_Pin_D6,
               GPIO_TypeDef *GPIOx_D7, uint16_t GPIO_Pin_D7,
               uint8_t col, uint8_t row)

{

#if defined(STM32F030x6)
  _GPIOx_RS    = GPIOx_RS;
  _GPIOx_EN    = GPIOx_EN;
  _GPIOx_RW    = GPIOx_RW;
  _GPIOx_D4    = GPIOx_D4;
  _GPIOx_D5    = GPIOx_D5;
  _GPIOx_D6    = GPIOx_D6;
  _GPIOx_D7    = GPIOx_D7;
  _GPIO_Pin_RS = GPIO_Pin_RS;
  _GPIO_Pin_EN = GPIO_Pin_EN;
  _GPIO_Pin_RW = GPIO_Pin_RW;
  _GPIO_Pin_D4 = GPIO_Pin_D4;
  _GPIO_Pin_D5 = GPIO_Pin_D5;
  _GPIO_Pin_D6 = GPIO_Pin_D6;
  _GPIO_Pin_D7 = GPIO_Pin_D7;
  _col         = col;
  _row         = row;
//  _cirill      = cirill;

  if((_GPIOx_D4 == GPIOA) || (_GPIOx_D5 == GPIOA) || (_GPIOx_D6 == GPIOA) || (_GPIOx_D7 == GPIOA) || (_GPIOx_EN == GPIOA) || (_GPIOx_RS == GPIOA) || (_GPIOx_RW == GPIOA)) RCC->AHBENR |= RCC_AHBENR_GPIOAEN;
  if((_GPIOx_D4 == GPIOB) || (_GPIOx_D5 == GPIOB) || (_GPIOx_D6 == GPIOB) || (_GPIOx_D7 == GPIOB) || (_GPIOx_EN == GPIOB) || (_GPIOx_RS == GPIOB) || (_GPIOx_RW == GPIOA)) RCC->AHBENR |= RCC_AHBENR_GPIOBEN;
  if((_GPIOx_D4 == GPIOC) || (_GPIOx_D5 == GPIOC) || (_GPIOx_D6 == GPIOC) || (_GPIOx_D7 == GPIOC) || (_GPIOx_EN == GPIOC) || (_GPIOx_RS == GPIOC) || (_GPIOx_RW == GPIOA)) RCC->AHBENR |= RCC_AHBENR_GPIOCEN;
  if((_GPIOx_D4 == GPIOD) || (_GPIOx_D5 == GPIOD) || (_GPIOx_D6 == GPIOD) || (_GPIOx_D7 == GPIOD) || (_GPIOx_EN == GPIOD) || (_GPIOx_RS == GPIOD) || (_GPIOx_RW == GPIOA)) RCC->AHBENR |= RCC_AHBENR_GPIODEN;
  if((_GPIOx_D4 == GPIOF) || (_GPIOx_D5 == GPIOF) || (_GPIOx_D6 == GPIOF) || (_GPIOx_D7 == GPIOF) || (_GPIOx_EN == GPIOF) || (_GPIOx_RS == GPIOF) || (_GPIOx_RW == GPIOA)) RCC->AHBENR |= RCC_AHBENR_GPIOFEN;


  _SetPin(GPIOx_EN, GPIO_Pin_EN);  // Инициализируем вывод EN
  _SetPin(GPIOx_RS, GPIO_Pin_RS);  // Инициализируем вывод RS
  _SetPin(GPIOx_RW, GPIO_Pin_RW);  // Инициализируем вывод RW
  _SetPin(GPIOx_D4, GPIO_Pin_D4);  // Инициализируем вывод D4
  _SetPin(GPIOx_D5, GPIO_Pin_D5);  // Инициализируем вывод D5
  _SetPin(GPIOx_D6, GPIO_Pin_D6);  // Инициализируем вывод D6
  _SetPin(GPIOx_D7, GPIO_Pin_D7);  // Инициализируем вывод D7
#endif // defined(STM32F030x6)
}

void LCD780::init(void)
{
  // инициализация дисплея
  delay(15);                                                                    // задержка 15 мс

  _displaycontrol= 0x0c;
  _displaymode= 6;

  _GPIOx_RS -> BSRR = _GPIO_Pin_RS << 16;                                       // сброс бита RS

  // установка 3 на шину данных
  _GPIOx_D7 -> BSRR = _GPIO_Pin_D7 << 16;
  _GPIOx_D6 -> BSRR = _GPIO_Pin_D6 << 16;
  _GPIOx_D5 -> BSRR = _GPIO_Pin_D5;
  _GPIOx_D4 -> BSRR = _GPIO_Pin_D4;

  delayMicroseconds(1);                                                         // задержка

  // строб
  _GPIOx_EN -> BSRR = _GPIO_Pin_EN;                                             // установка бита EN
  delayMicroseconds(1);                                                         // задержка
  _GPIOx_EN -> BSRR = _GPIO_Pin_EN << 16;                                       // сброс бита EN
  delayMicroseconds(4500);                                                      // задержка

  // строб
  _GPIOx_EN -> BSRR = _GPIO_Pin_EN;                                             // установка бита EN
  delayMicroseconds(1);                                                         // задержка
  _GPIOx_EN -> BSRR = _GPIO_Pin_EN << 16;                                       // сброс бита EN
  delayMicroseconds(200);                                                       // задержка

  // строб
  _GPIOx_EN -> BSRR = _GPIO_Pin_EN;                                             // установка бита EN
  delayMicroseconds(1);                                                         // задержка
  _GPIOx_EN -> BSRR = _GPIO_Pin_EN << 16;                                       // сброс бита EN
  delayMicroseconds(200);                                                       // задержка

  _GPIOx_D4 -> BSRR = _GPIO_Pin_D4 << 16;
  delayMicroseconds(1);                                                         // задержка

  // строб
  _GPIOx_EN -> BSRR = _GPIO_Pin_EN;                                             // установка бита EN
  delayMicroseconds(1);                                                         // задержка
  _GPIOx_EN -> BSRR = _GPIO_Pin_EN << 16;                                       // сброс бита EN
  delayMicroseconds(200);                                                       // задержка

  // режим дисплея
  if( _row < 2 ) writeByte(0x20);                                                // одна строка
  else writeByte(0x28);                                                         // две строки
  delayMicroseconds(50);                                                        // задержка

  // дисплей включен, курсор отключен
  writeByte(_displaycontrol);
  delayMicroseconds(50);                                                        // задержка

  // очистка дисплея
  writeByte(0x01);
  delayMicroseconds(50);                                                        // задержка

  // печать слева направо
  writeByte(0x06);
  delayMicroseconds(50);                                                        // задержка

}

/*******************************************************************************
 *  Далее описание всех функций
 ******************************************************************************/

/*
 * Команды
 */

// выключение курсора
void LCD780::cursorOFF()
{
  _GPIOx_RS->BSRR = _GPIO_Pin_RS << 16;    // сброс бита RS
  _displaycontrol &= 0b11111101;
  writeByte(_displaycontrol);
  delayMicroseconds(50);                                                                   // задержка
}

// включение курсора
void LCD780::cursorON()
{
  _GPIOx_RS->BSRR = _GPIO_Pin_RS << 16;    // сброс бита RS
  _displaycontrol |= 0b00001010;
  writeByte(_displaycontrol);
  delayMicroseconds(50);                                                                   // задержка
}

// выключение мигания курсора
void LCD780::cursorBlinkOFF()
{
  _GPIOx_RS->BSRR = _GPIO_Pin_RS << 16;    // сброс бита RS
  _displaycontrol &= 0b11111110;
  writeByte(_displaycontrol);
  delayMicroseconds(50);                                                                   // задержка
}

// включение мигания курсора
void LCD780::cursorBlinkON()
{
  _GPIOx_RS->BSRR = _GPIO_Pin_RS << 16;    // сброс бита RS
  _displaycontrol |= 0b00001001;
  writeByte(_displaycontrol);
  delayMicroseconds(50);                                                                   // задержка
}

// выключение дисплея
void LCD780::displayOFF()
{
  _GPIOx_RS->BSRR = _GPIO_Pin_RS << 16;    // сброс бита RS
  _displaycontrol &= 0b11111011;
  writeByte (_displaycontrol);
  delayMicroseconds(50);                                                                   // задержка
}

// включение дисплея
void LCD780::displayON()
{
  _GPIOx_RS->BSRR = _GPIO_Pin_RS << 16;    // сброс бита RS
  _displaycontrol |= 0b00001100;
  writeByte (_displaycontrol);
  delayMicroseconds(50);                                                                   // задержка
}

// установка курсора в позицию
void LCD780::setCursor(uint8_t col, uint8_t row)
{
  _GPIOx_RS->BSRR = _GPIO_Pin_RS << 16;    // сброс бита RS

  if(_row == 1)
  {
    // одна строка
    if(col <= 8) writeByte(0x80 | (col - 1));
    else writeByte(0x80 | (col - 1 + 0x40));
  }
  else if(_row == 2)
  {
    // две строки
    if(row == 1) writeByte(0x80 | (col - 1));
    else writeByte(0x80 | (col - 1 + 0x40));
  }
  if(_row == 4)
  {
    if(row == 1) writeByte(0x80 | (col - 1));
    else if(row == 2) writeByte(0x80 | (col - 1 + 0x40));
    else if(row == 3) writeByte(0x80 | (col - 1 + _col));
    else if(row == 4) writeByte(0x80 | (col - 1 + 0x40 + _col));
  }
  delayMicroseconds(50);                                                                   // задержка
}

/*
 * Функции вывода
 */

// вывод символа на экран
void LCD780::writeChar(uint8_t valChar)
{
  _GPIOx_RS->BSRR = _GPIO_Pin_RS;                                               // установка бита RS
//  if((valChar >= 0x80) && (_cirill != 0)) writeByte(cirillAsciiTable[valChar - 0x80]);
//  else
  writeByte(valChar);
  delayMicroseconds(50);                                                                   // задержка
}

// вывод на экран uint32_t
void LCD780::printUint(uint32_t n, uint8_t base)
{

  uint8_t buf[33] =
  { '\0' };
  uint8_t *str = buf + 31;
  uint8_t c;
  do
  {
    c = n % base;
    n /= base;
    *--str = c < 10 ? c + '0' : c + 'A' - 10;
  } while (n);

  for (uint8_t i = 0; i < 32; i++)
  {
    if(str[i] == 0) break;
    writeChar(str[i]);
  }
}

// вывод на экран int32_t
void LCD780::printInt(int32_t n, uint8_t base)
{

  if((base == 10) && (n < 0))
  {
    writeChar('-');
    n = -n;
  }
  printUint(n, base);
}

// вывод числа float
void LCD780::printFloat(float number, uint8_t digits)
{

  // отрицательные числа
  if(number < 0.0)
  {
    writeChar('-');
    number = -number;
  }

  // округление
  float rounding = 0.5;
  for (uint8_t i = 0; i < digits; ++i)
    rounding /= 10.0;
  number += rounding;

  // разделение на целую и дробную части
  uint32_t int_part = (uint32_t) number;
  float remainder = number - (float) int_part;
  printUint(int_part, 10);

  // вывод дробной части
  if(digits > 0) writeChar('.');

  while (digits-- > 0)
  {
    remainder *= 10.0;
    uint16_t toPrint = (uint16_t) (remainder);
    printUint(toPrint, 10);
    remainder -= toPrint;
  }
}

// вывод массива символов на экран
void LCD780::writeBuf(uint8_t *buf, uint16_t length)
{
  _GPIOx_RS->BSRR = _GPIO_Pin_RS;    // установка бита RS
  for (uint16_t i = 0; i < length; i++)
  {
    if(((*(buf + i)) >= 0x80) && (_cirill != 0)) writeByte(cirillAsciiTable[(*(buf + i)) - 0x80]);
    else writeByte((*(buf + i)));
    delayMicroseconds(50);                                                                   // задержка
  }
}

// курсор в начало
void LCD780::home(void)
{
  _GPIOx_RS->BSRR = _GPIO_Pin_RS << 16;    // сброс бита RS
  writeByte(0x01);
  delayMicroseconds(50);                                                                   // задержка
  writeByte(0x2);
  delay(2);
}

// вывод строки
void LCD780::printString(const char str[])
{

  for (uint8_t i = 0; i < 200; i++)
  {
    if(str[i] == 0) break;
    writeChar(str[i]);
  }
}

// запись байта в LCD
void LCD780::writeByte(uint8_t dt)
{

  // вывод старшей тетрады данных
  if( (dt & 0x80) == 0 ) _GPIOx_D7 -> BSRR = _GPIO_Pin_D7 << 16;
  else _GPIOx_D7 -> BSRR = _GPIO_Pin_D7;
  if( (dt & 0x40) == 0 ) _GPIOx_D6 -> BSRR = _GPIO_Pin_D6 << 16;
  else _GPIOx_D6 -> BSRR = _GPIO_Pin_D6;
  if( (dt & 0x20) == 0 ) _GPIOx_D5 -> BSRR = _GPIO_Pin_D5 << 16;
  else _GPIOx_D5 -> BSRR = _GPIO_Pin_D5;
  if( (dt & 0x10) == 0 ) _GPIOx_D4 -> BSRR = _GPIO_Pin_D4 << 16;
  else _GPIOx_D4 -> BSRR = _GPIO_Pin_D4;

  delayMicroseconds(1);                                                                   // задержка

  _GPIOx_EN -> BSRR = _GPIO_Pin_EN; // установка бита EN

  delayMicroseconds(1);                                                                   // задержка

  _GPIOx_EN -> BSRR = _GPIO_Pin_EN << 16; // сброс бита EN

  delayMicroseconds(1);                                                                   // задержка

  // вывод младшей тетрады данных
  if( (dt & 0x08) == 0 ) _GPIOx_D7 -> BSRR = _GPIO_Pin_D7 << 16;
  else _GPIOx_D7 -> BSRR = _GPIO_Pin_D7;
  if( (dt & 0x04) == 0 ) _GPIOx_D6 -> BSRR = _GPIO_Pin_D6 << 16;
  else _GPIOx_D6 -> BSRR = _GPIO_Pin_D6;
  if( (dt & 0x02) == 0 ) _GPIOx_D5 -> BSRR = _GPIO_Pin_D5 << 16;
  else _GPIOx_D5 -> BSRR = _GPIO_Pin_D5;
  if( (dt & 0x01) == 0 ) _GPIOx_D4 -> BSRR = _GPIO_Pin_D4 << 16;
  else _GPIOx_D4 -> BSRR = _GPIO_Pin_D4;

  delayMicroseconds(1);                                                                   // задержка

  _GPIOx_EN -> BSRR = _GPIO_Pin_EN; // установка бита EN

  delayMicroseconds(1);                                                                   // задержка

  _GPIOx_EN -> BSRR = _GPIO_Pin_EN << 16; // сброс бита EN
}

void LCD780::_SetPin(GPIO_TypeDef *GPIOx_Curr, uint16_t GPIO_Pin_Curr)
{
  _GPIOx_Curr     = GPIOx_Curr;
  _GPIO_Pin_Curr  = GPIO_Pin_Curr;

  uint32_t I;
  uint16_t D;

  // Инициализируем вывод EN
  I = 0;
  D = _GPIO_Pin_Curr;
  while(I < 16) { D = D >> 1; if(D == 0) break; I++; }                          // Подсчитываем номер бита который соответствует настраиваемому выводу
  if(I < 16)
  {
    _GPIOx_Curr->MODER    &= ~(0b11 << (I * 2));                                    // Затираем биты порта
    _GPIOx_Curr->MODER    |=  0b01 << (I * 2);                                    // Переводим пин на выход
    _GPIOx_Curr->OSPEEDR  |=  0b11 << (I * 2);                                    // Врубаем максимальную скорость порта
  }
  else
  {
    _GPIOx_Curr->MODER    &= ~(0b11 << ((I - 16) * 2));
    _GPIOx_Curr->MODER    |=  0b01 << ((I - 16) * 2);
    _GPIOx_Curr->OSPEEDR  |=  0b11 << ((I - 16) * 2);
  }
  _GPIOx_Curr->ODR &= ~ (1 << I);                                                 // Принудительно переводим пин в "0"

}
