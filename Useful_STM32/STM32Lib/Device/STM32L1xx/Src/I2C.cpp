/*
 * I2C.cpp
 *
 *  Created on: 2 февр. 2022 г.
 *      Author: User
 */

#include "I2C.h"

I2C::I2C(I2C_TypeDef *I2C_Port, bool Alternate)
{
  _I2C_Port       = I2C_Port;
  _Alternate  = Alternate;

  if(_I2C_Port == I2C1) {_IRQn_ER = I2C1_ER_IRQn; _IRQn_EV = I2C1_EV_IRQn;}
  else if (_I2C_Port == I2C2) {_IRQn_ER = I2C2_ER_IRQn; _IRQn_EV = I2C2_EV_IRQn;}
//  else if (_I2C_Port == I2C3) {_IRQn_ER = I2C3_ER_IRQn; _IRQn_EV = I2C3_EV_IRQn;}
}

void I2C::Init(uint8_t Mode)
{
  uint32_t freqrange;
  uint32_t pclk1;

  _Mode = Mode;

  _SetSpeed(Speed_WeryHigh);
  _SetPull(No_Pull);
  _SetTypeOut(Out_Open);

  if(_I2C_Port == I2C1)
  {
    RCC->APB1ENR |= RCC_APB1ENR_I2C1EN;
    if(!_Alternate)
      _SetPinAlternate(GPIOB, GPIO_PIN_6 | GPIO_PIN_7, GPIO_AF4_I2C1);
    else
      _SetPinAlternate(GPIOB, GPIO_PIN_8 | GPIO_PIN_9, GPIO_AF4_I2C1);
  }

  else if(_I2C_Port == I2C2)
  {
    RCC->APB1ENR |= RCC_APB1ENR_I2C2EN;
    _SetPinAlternate(GPIOB, GPIO_PIN_10 | GPIO_PIN_11, GPIO_AF4_I2C2);
  }

//  else if(_I2C_Port == I2C3)
//  {
//    RCC->APB1ENR |= RCC_APB1ENR_I2C3EN;
//    _SetPinAlternate(GPIOA, GPIO_PIN_8, GPIO_AF4_I2C3);
//    _SetPinAlternate(GPIOC, GPIO_PIN_9, GPIO_AF4_I2C3);
//  }

  _SetSpeed(Speed_Low);
  _SetTypeOut(Out_Normal);

  _I2C_Port->CR1 |= I2C_CR1_SWRST;                                                                  // Сбрасываем I2C
  _I2C_Port->CR1 &= ~I2C_CR1_SWRST;

  pclk1 = SystemCoreClock >> APBPrescTable[(RCC->CFGR & RCC_CFGR_PPRE1)>> RCC_CFGR_PPRE1_Pos];      // Частота шины APB1
  freqrange = pclk1/1000000;                                                                        // Необходимая частота тактирования I2C

  _I2C_Port->CR2 |= (freqrange << I2C_CR2_FREQ_Pos);                                                // Устанавливаем частоту тактирования
  _I2C_Port->TRISE |= ((freqrange +1) << I2C_TRISE_TRISE_Pos);                                      // Масимальное время фронта

  switch (_Mode)
  {
    case Mode_Slow:
      _I2C_Port->CCR  |= ((((pclk1-1)/(100000*2))+1) & I2C_CCR_CCR);                                // Вычисляем время для 100 000Гц передачи, в будущем можно циферки подставлять
      _I2C_Port->CCR  &= ~(I2C_CCR_DUTY | I2C_CCR_FS);                                              // Выставляем низкую скорость передачи
      break;
    case Mode_Fast:
      _I2C_Port->CCR  |= ((((pclk1-1)/(400000*25))+1) & I2C_CCR_CCR);                               // Вычисляем время для 400 000Гц передачи, в будущем можно циферки подставлять
      _I2C_Port->CCR  |= (I2C_CCR_DUTY | I2C_CCR_FS);                                               // Выставляем высокую скорость передачи
      break;
    default:
      break;
  }
  _I2C_Port->OAR1 = 0x4000;
  _I2C_Port->CR1  |= I2C_CR1_PE;                                                                    // Разрешаем работу порта
}

int16_t I2C::isReady(uint16_t DevAddress, uint8_t Repeat, uint16_t TimeOut)
{
  uint8_t   I2C_Repeat = 0;                                                                         // Подсчёт количества повторений
  uint32_t  I2C_Time;                                                                               // Переменная для TimeOut
  bool      Tmp1, Tmp2;

  // Ждём сброса флага BUSY
  I2C_Time = millis();
  if(_isBusyFlag(SET, TimeOut) != I2C_OK) return I2C_BUSY;
  if ((_I2C_Port->CR1 & I2C_CR1_PE) != I2C_CR1_PE) _I2C_Port->CR1 |= I2C_CR1_PE;                    // Включаем передатчик
  _I2C_Port->CR1 &= ~I2C_CR1_POS;                                                                   // Отключаем POS

  do                                                                                                // Долбимся заданное количество повторений
  {
    _I2C_Port->CR1 |= I2C_CR1_START;                                                                // Стартуем

    I2C_Time = millis();
    if(_isSB_Flag(RESET, TimeOut) != I2C_OK) return I2C_SB_ERROR;

    _I2C_Port->DR = (uint8_t) (DevAddress << 1) & ~1U;                                              // Выплёвываем адрес со сброшенным битом RD

    I2C_Time = millis();
    Tmp1 = _I2C_Port->SR1 & I2C_SR1_ADDR;
    Tmp2 = _I2C_Port->SR1 & I2C_SR1_AF;
    while((Tmp1 == 0) && (Tmp2 == 0))           // Пока нет флагов, ждём
    {
      if(millis() > (I2C_Time + TimeOut)) break;                                                    // Надоело, выходим
      Tmp1 = _I2C_Port->SR1 & I2C_SR1_ADDR;
      Tmp2 = _I2C_Port->SR1 & I2C_SR1_AF;
    }

    if(_I2C_Port->SR1 & I2C_SR1_ADDR)                                                               // Если флаг ADDR всё таки был,
    {
      _I2C_Port->CR1  |= I2C_CR1_STOP;                                                              // Посылаем STOP
      (void) _I2C_Port->SR1;                                                                        // Чистим флаги
      (void) _I2C_Port->SR2;

      I2C_Time = millis();
      while(_I2C_Port->SR2 & I2C_SR2_BUSY)
      {
        if(millis() > (I2C_Time + TimeOut)) return I2C_BUSY;                                       // Если флаг BUSY установлен, а время вышло, это ошибка
      }
      return I2C_OK;
    }
    else
    {
      _I2C_Port->CR1 |= I2C_CR1_STOP;                                                               // Флага ADDR не было, посылаем STOP
      _I2C_Port->SR1 &= ~I2C_SR1_AF;                                                                // Сбрасываем флаг AF

      I2C_Time = millis();
      while(_I2C_Port->SR2 & I2C_SR2_BUSY)
      {
        if(millis() > (I2C_Time + TimeOut)) return I2C_BUSY;                                       // Если флаг BUSY установлен, а время вышло, это ошибка
      }
    }

    I2C_Repeat++;
  } while (I2C_Repeat < Repeat);
  return I2C_ERROR;
}

int16_t I2C::ScanDevice(uint8_t *BuffDevice)
{
  uint8_t Count = 0;

  for(uint8_t I=0; I<127; I++)
  {
    if(!(isReady(I, 1, 10)))
    {
      *BuffDevice = I;
      BuffDevice++;
      Count++;
    }
  }
  return Count;
}



int16_t I2C::MasterRead(uint8_t DevAddress, uint8_t* DataBuff, uint16_t Size, uint16_t TimeOut)
{
  int16_t   Status = 0;
  uint16_t  Count = Size;

  // Ждём сброса флага BUSY
  if(_isBusyFlag(SET, TimeOut) != I2C_OK) return I2C_BUSY;

  if ((_I2C_Port->CR1 & I2C_CR1_PE) != I2C_CR1_PE) _I2C_Port->CR1 |= I2C_CR1_PE;                    // Включаем передатчик если не включен
  _I2C_Port->CR1 &= ~I2C_CR1_POS;                                                                   // Отключаем POS

  // Посылаем запрос на чтение памяти
  Status = _RequestDataRead(DevAddress, TimeOut);
  if(Status != I2C_OK) return Status;

  // Проверяем количество передаваемых байт на 0, 1, 2 и более
  if(Size == 0)                                                                                     // Если жопа, то СТОП
  {
    (void) (_I2C_Port->SR1);                                                                        // Сбрасываем все флаги
    (void) (_I2C_Port->SR2);
    _I2C_Port->CR1 |= I2C_CR1_STOP;                                                                 // Генерим STOP
  }
  else if(Size == 1)                                                                                // Если 1
  {
    _I2C_Port->CR1 &= ~I2C_CR1_ACK;                                                                 // Сбрасываем ACK
    (void) (_I2C_Port->SR1);                                                                        // Сбрасываем все флаги
    (void) (_I2C_Port->SR2);
    _I2C_Port->CR1 |= I2C_CR1_STOP;                                                                 // Генерим STOP
  }
  else if(Size == 2)                                                                                // Если 2
  {
    _I2C_Port->CR1 &= ~I2C_CR1_ACK;                                                                 // Сбрасываем ACK
    _I2C_Port->CR1 |= I2C_CR1_POS;                                                                  // Устанавливаем POS
    (void) (_I2C_Port->SR1);                                                                        // Сбрасываем все флаги
    (void) (_I2C_Port->SR2);
  }
  else                                                                                              // Если больше 2х
  {
    _I2C_Port->CR1 |= I2C_CR1_ACK;                                                                  // Устанавливаем АСК
    (void) (_I2C_Port->SR1);                                                                        // Сбрасываем все флаги
    (void) (_I2C_Port->SR2);
  }

  // Далее цикл чтения
  while(Count > 0)
  {
    if(Count <= 3)                                                                                  // Если считываемых байт осталось 3 и менее
    {
      // Один байт
      if(Count == 1)
      {
        if(_isRXNE_Flag(TimeOut) != I2C_OK) return I2C_RXNE_ERROR;
        *DataBuff = (uint8_t) _I2C_Port->DR;
        DataBuff++;
        Size--;
        Count--;
      }
      // Два байта
      else if(Count == 2)
      {
        if(_isBTF_Flag(RESET, TimeOut) != I2C_OK) return I2C_BTF_ERROR;
        _I2C_Port->CR1 |= I2C_CR1_STOP;
        *DataBuff = (uint8_t) _I2C_Port->DR;
        DataBuff++;
        Size--;
        Count--;
        *DataBuff = (uint8_t) _I2C_Port->DR;
        DataBuff++;
        Size--;
        Count--;
      }
      // Три байта
      else
      {
        if(_isBTF_Flag(RESET, TimeOut) != I2C_OK) return I2C_BTF_ERROR;
        _I2C_Port->CR1 &= ~I2C_CR1_ACK;
        *DataBuff = (uint8_t) _I2C_Port->DR;
        DataBuff++;
        Size--;
        Count--;
        if(_isBTF_Flag(RESET, TimeOut) != I2C_OK) return I2C_BTF_ERROR;
        _I2C_Port->CR1 |= I2C_CR1_STOP;
        *DataBuff = (uint8_t) _I2C_Port->DR;
        DataBuff++;
        Size--;
        Count--;
        *DataBuff = (uint8_t) _I2C_Port->DR;
        DataBuff++;
        Size--;
        Count--;
      }
    }
    else                                                                                            // Если считываемых байт ещё больше 3х
    {
      if(_isRXNE_Flag(TimeOut) != I2C_OK) return I2C_RXNE_ERROR;
      *DataBuff = (uint8_t) _I2C_Port->DR;
      DataBuff++;
      Size--;
      Count--;
      if(((_I2C_Port->SR1 & I2C_SR1_BTF) >> I2C_SR1_BTF_Pos) == SET)
      {
        *DataBuff = (uint8_t) _I2C_Port->DR;
        DataBuff++;
        Size--;
        Count--;
      }
    }
  }
  return I2C_OK;
}

int16_t I2C::MasterWrite(uint8_t DevAddress, uint8_t* DataBuff, uint16_t Size, uint16_t TimeOut)
{
  uint16_t Status;

  // Проверяем занятость устройства
  if(_isBusyFlag(SET, TimeOut) != I2C_OK) return I2C_BUSY;
  // Проверяем, включено ли устройство
  if ((_I2C_Port->CR1 & I2C_CR1_PE) != I2C_CR1_PE) _I2C_Port->CR1 |= I2C_CR1_PE;                    // Включаем передатчик если не включен
  // Очищаем бит POS
  _I2C_Port->CR1 &= ~I2C_CR1_POS;                                                                   // Отключаем POS
  // Обращаемся к функции RequestMemoryWrite
  Status = _RequestDataWrite(DevAddress, TimeOut);
  // Очищаем флаги
  (void) (_I2C_Port->SR1);                                                                        // Сбрасываем все флаги
  (void) (_I2C_Port->SR2);


  while(Size > 0)
  {
    if(_isTXE_Flag(TimeOut) == I2C_AF_ERROR)
    {
      _I2C_Port->CR1 |= I2C_CR1_STOP;
      return I2C_AF_ERROR;
    }
    _I2C_Port->DR = (uint8_t) *DataBuff;
    DataBuff++;
    Size--;

    if((_isBTF_Flag(SET, TimeOut) != I2C_OK) && (Size != 0))
    {
      _I2C_Port->DR = (uint8_t) *DataBuff;
      DataBuff++;
      Size--;
    }
  }

  Status = _isBTF_Flag_TimeOut(TimeOut);
  if(Status != I2C_OK)
  {
    if(Status == I2C_AF_ERROR) _I2C_Port->CR1 |= I2C_CR1_STOP;
    return I2C_AF_ERROR;
  }
  _I2C_Port->CR1 |= I2C_CR1_STOP;
  return I2C_OK;
}

/*
 * Чтение памяти EEPROM и железа с подобной организацией
 */
int16_t I2C::MasterMemRead(uint8_t DevAddress, uint16_t MemAddress, uint8_t MemAddSise, uint8_t* DataBuff, uint16_t Size, uint16_t TimeOut)
{
  int16_t   Status = 0;
  uint16_t  Count = Size;

  // Ждём сброса флага BUSY
  if(_isBusyFlag(SET, TimeOut) != I2C_OK) return I2C_BUSY;

  if ((_I2C_Port->CR1 & I2C_CR1_PE) != I2C_CR1_PE) _I2C_Port->CR1 |= I2C_CR1_PE;                    // Включаем передатчик если не включен
  _I2C_Port->CR1 &= ~I2C_CR1_POS;                                                                   // Отключаем POS

  // Посылаем запрос на чтение памяти
  Status = _RequestMemoryRead(DevAddress, MemAddress, MemAddSise, TimeOut);
  if(Status != I2C_OK) return Status;

  // Проверяем количество передаваемых байт на 0, 1, 2 и более
  if(Size == 0)                                                                                     // Если жопа, то СТОП
  {
    (void) (_I2C_Port->SR1);                                                                        // Сбрасываем все флаги
    (void) (_I2C_Port->SR2);
    _I2C_Port->CR1 |= I2C_CR1_STOP;                                                                 // Генерим STOP
  }
  else if(Size == 1)                                                                                // Если 1
  {
    _I2C_Port->CR1 &= ~I2C_CR1_ACK;                                                                 // Сбрасываем ACK
    (void) (_I2C_Port->SR1);                                                                        // Сбрасываем все флаги
    (void) (_I2C_Port->SR2);
    _I2C_Port->CR1 |= I2C_CR1_STOP;                                                                 // Генерим STOP
  }
  else if(Size == 2)                                                                                // Если 2
  {
    _I2C_Port->CR1 &= ~I2C_CR1_ACK;                                                                 // Сбрасываем ACK
    _I2C_Port->CR1 |= I2C_CR1_POS;                                                                  // Устанавливаем POS
    (void) (_I2C_Port->SR1);                                                                        // Сбрасываем все флаги
    (void) (_I2C_Port->SR2);
  }
  else                                                                                              // Если больше 2х
  {
    (void) (_I2C_Port->SR1);                                                                        // Сбрасываем все флаги
    (void) (_I2C_Port->SR2);
  }

  // Далее цикл чтения
  while(Count > 0)
  {
    if(Count <= 3)                                                                                  // Если считываемых байт осталось 3 и менее
    {
      // Один байт
      if(Count == 1)
      {
        if(_isRXNE_Flag(TimeOut) != I2C_OK) return I2C_RXNE_ERROR;
        *DataBuff = (uint8_t) _I2C_Port->DR;
        DataBuff++;
        Size--;
        Count--;
      }
      // Два байта
      else if(Count == 2)
      {
        if(_isBTF_Flag(RESET, TimeOut) != I2C_OK) return I2C_BTF_ERROR;
        _I2C_Port->CR1 |= I2C_CR1_STOP;
        *DataBuff = (uint8_t) _I2C_Port->DR;
        DataBuff++;
        Size--;
        Count--;
        *DataBuff = (uint8_t) _I2C_Port->DR;
        DataBuff++;
        Size--;
        Count--;
      }
      // Три байта
      else
      {
        if(_isBTF_Flag(RESET, TimeOut) != I2C_OK) return I2C_BTF_ERROR;
        _I2C_Port->CR1 &= ~I2C_CR1_ACK;
        *DataBuff = (uint8_t) _I2C_Port->DR;
        DataBuff++;
        Size--;
        Count--;
        if(_isBTF_Flag(RESET, TimeOut) != I2C_OK) return I2C_BTF_ERROR;
        _I2C_Port->CR1 |= I2C_CR1_STOP;
        *DataBuff = (uint8_t) _I2C_Port->DR;
        DataBuff++;
        Size--;
        Count--;
        *DataBuff = (uint8_t) _I2C_Port->DR;
        DataBuff++;
        Size--;
        Count--;
      }
    }
    else                                                                                            // Если считываемых байт ещё больше 3х
    {
      if(_isRXNE_Flag(TimeOut) != I2C_OK) return I2C_RXNE_ERROR;
      *DataBuff = (uint8_t) _I2C_Port->DR;
      DataBuff++;
      Size--;
      Count--;
      if(((_I2C_Port->SR1 & I2C_SR1_BTF) >> I2C_SR1_BTF_Pos) == SET)
      {
        *DataBuff = (uint8_t) _I2C_Port->DR;
        DataBuff++;
        Size--;
        Count--;
      }
    }
  }
  return I2C_OK;
}

/*
 * Запись EEPROM и железа с подобной организацией
 */
int16_t I2C::MasterMemWrite(uint8_t DevAddress, uint16_t MemAddress, uint8_t MemAddSise, uint8_t* DataBuff, uint16_t Size, uint16_t TimeOut)
{
  uint16_t Status;

  // Проверяем занятость устройства
  if(_isBusyFlag(SET, TimeOut) != I2C_OK) return I2C_BUSY;
  // Проверяем, включено ли устройство
  if ((_I2C_Port->CR1 & I2C_CR1_PE) != I2C_CR1_PE) _I2C_Port->CR1 |= I2C_CR1_PE;                    // Включаем передатчик если не включен
  // Очищаем бит POS
  _I2C_Port->CR1 &= ~I2C_CR1_POS;                                                                   // Отключаем POS
  // Обращаемся к функции RequestMemoryWrite
  Status = _RequestMemoryWrite(DevAddress, MemAddress, MemAddSise, TimeOut);

  while(Size > 0)
  {
    if(_isTXE_Flag(TimeOut) == I2C_AF_ERROR)
    {
      _I2C_Port->CR1 |= I2C_CR1_STOP;
      return I2C_AF_ERROR;
    }
    _I2C_Port->DR = (uint8_t) *DataBuff;
    DataBuff++;
    Size--;

    if((_isBTF_Flag(SET, TimeOut) != I2C_OK) && (Size != 0))
    {
      _I2C_Port->DR = (uint8_t) *DataBuff;
      DataBuff++;
      Size--;
    }
  }

  Status = _isBTF_Flag_TimeOut(TimeOut);
  if(Status != I2C_OK)
  {
    if(Status == I2C_AF_ERROR) _I2C_Port->CR1 |= I2C_CR1_STOP;
    return I2C_AF_ERROR;
  }
  _I2C_Port->CR1 |= I2C_CR1_STOP;
  return I2C_OK;
}

/*
 * Системные функции находящиеся в private
 */
int16_t I2C::_SendSlaveAddress(uint8_t DevAddress, uint16_t TimeOut)
{
  uint32_t I2C_Time;

  // Устанавливаем бит ACK
  _I2C_Port->CR1 |= I2C_CR1_ACK;
  // Устанавливаем бит START
  _I2C_Port->CR1 |= I2C_CR1_START;
  // Ждём сброса SB
  I2C_Time = millis();
  while (!(_I2C_Port->SR1 & I2C_SR1_SB))
  {
    if(millis() > (I2C_Time + TimeOut))
    {
      if (_I2C_Port->CR1 & I2C_CR1_START) return I2C_WRONG_START;                                 // Похоже старт затянулся
    }
  }
// Если адрес 7 бит пуляем его
  _I2C_Port->DR = (uint8_t) (DevAddress << 1) & ~1U;                                              // Выплёвываем адрес со сброшенным битом RD
  // Если 10бит оставляем место для пуляния
  // Ждём флага ADDR
  I2C_Time = millis();
  while(((_I2C_Port->SR1 & I2C_SR1_ADDR) == 0) && ((_I2C_Port->SR1 & I2C_SR1_AF) == 0))           // Пока нет флагов, ждём
  {
    if(millis() > (I2C_Time + TimeOut)) break;                                                    // Надоело, выходим
  }

  if(_I2C_Port->SR1 & I2C_SR1_ADDR)                                                               // Если флаг ADDR всё таки был,
  {
    _I2C_Port->CR1  |= I2C_CR1_STOP;                                                              // Посылаем STOP
    (void) _I2C_Port->SR1;                                                                        // Чистим флаги
    (void) _I2C_Port->SR2;

    I2C_Time = millis();
    while(_I2C_Port->SR2 & I2C_SR2_BUSY)
    {
      if(millis() > (I2C_Time + TimeOut)) return I2C_BUSY;                                       // Если флаг BUSY установлен, а время вышло, это ошибка
    }
    return I2C_OK;
  }
  else
  {
    _I2C_Port->CR1 |= I2C_CR1_STOP;                                                               // Флага ADDR не было, посылаем STOP
    (void) _I2C_Port->SR1;                                                                        // Чистим флаги
    (void) _I2C_Port->SR2;

    I2C_Time = millis();
    while(_I2C_Port->SR2 & I2C_SR2_BUSY)
    {
      if(millis() > (I2C_Time + TimeOut)) return I2C_BUSY;                                       // Если флаг BUSY установлен, а время вышло, это ошибка
    }
    return I2C_NOT_ADDRESS;
  }
  return I2C_OK;
}

int16_t   I2C::_RequestDataRead(uint8_t DevAddress, uint16_t TimeOut)
{
  //  uint32_t  I2C_Time;
    int32_t   Status;

    _I2C_Port->CR1 |= I2C_CR1_ACK;                                                                    // Включаем ACK
    _I2C_Port->CR1 |= I2C_CR1_START;                                                                  // Выдаём старт

    // Проверяем SB и START
    if(_isSB_Flag(RESET, TimeOut))
    {
      if((_I2C_Port->CR1 & I2C_CR1_START) == I2C_CR1_START) return I2C_WRONG_START;
    }
    // Загоняем адрес на линию в режиме записи
    _I2C_Port->DR = (uint8_t) (DevAddress << 1) & ~1U;                                              // Выплёвываем адрес со сброшенным битом RD

    // Проверяем флаг ADDR и AF
    Status = _isAddressAF_Flag(TimeOut);
    if(Status != I2C_OK) return Status;

    // Очищаем флаги
    (void) _I2C_Port->SR1;
    (void) _I2C_Port->SR2;

      _I2C_Port->CR1 |= I2C_CR1_START;                                                                  // Выдаём старт
      // Проверяем SB и START
      if(_isSB_Flag(RESET, TimeOut))
      {
        if((_I2C_Port->CR1 & I2C_CR1_START) == I2C_CR1_START) return I2C_WRONG_START;
      }

      _I2C_Port->DR = (uint8_t) (DevAddress << 1) | 1U;                                              // Выплёвываем адрес с установленным битом RD
      // Проверяем флаг ADDR и AF
      Status = _isAddressAF_Flag(TimeOut);
      if(Status != I2C_OK) return Status;
//    }
    return I2C_OK;
}

int16_t   I2C::_RequestDataWrite(uint8_t DevAddress, uint16_t TimeOut)
{
  int32_t Status;
  _I2C_Port->CR1 |= I2C_CR1_START;                                              // Запускаем START
  if(_isSB_Flag(RESET, TimeOut) != I2C_OK)
  {
    if((_I2C_Port->CR1 & I2C_CR1_START) == I2C_CR1_START)
    {
      return I2C_WRONG_START;
    }
    return I2C_TIME_OUT;
  }

  _I2C_Port->DR = (DevAddress << 1) & ~1U;                                      // Выдаём на шину адрес устройства
  // Проверяем флаг ADDR и AF
  Status = _isAddressAF_Flag(TimeOut);
  if(Status != I2C_OK) return Status;

//  // Очищаем флаги
//  (void) _I2C_Port->SR1;
//  (void) _I2C_Port->SR2;
//
//  // Ждём когда поднимется TXE
//  Status = _isTXE_Flag(TimeOut);
//  if(Status != I2C_OK)
//  {
//    if(Status == I2C_AF_ERROR) _I2C_Port->CR1 |= I2C_CR1_STOP;
//    return Status;
//  }
//
//  // Закидываем 8 или 16 бит адрес
//  if(MemAddSise == Size_Add_8bit)
//  {
//    _I2C_Port->DR = MemAddress & 0xFF;                                                              // Пихаем 8бит адрес
//  }
//  else
//  {
//    _I2C_Port->DR = (MemAddress >> 8) & 0xFF;                                                       // Пихаем старшие 8 бит адрес
//    // Ждём когда поднимется TXE
//    Status = _isTXE_Flag(TimeOut);
//    if(Status != I2C_OK)
//    {
//      if(Status == I2C_AF_ERROR) _I2C_Port->CR1 |= I2C_CR1_STOP;
//      return Status;
//    }
//    _I2C_Port->DR = MemAddress & 0xFF;                                                              // Пихаем младшие 8 бит
//    // Ждём когда поднимется TXE
//    Status = _isTXE_Flag(TimeOut);
//    if(Status != I2C_OK)
//    {
//      if(Status == I2C_AF_ERROR) _I2C_Port->CR1 |= I2C_CR1_STOP;
//      return Status;
//    }
//  }
  return I2C_OK;
}


int16_t I2C::_RequestMemoryRead(uint8_t DevAddress, uint16_t MemAddress, uint8_t MemAddSise, uint16_t TimeOut)
{
//  uint32_t  I2C_Time;
  int32_t   Status;

  _I2C_Port->CR1 |= I2C_CR1_ACK;                                                                    // Включаем ACK
  _I2C_Port->CR1 |= I2C_CR1_START;                                                                  // Выдаём старт

  // Проверяем SB и START
  if(_isSB_Flag(RESET, TimeOut))
  {
    if((_I2C_Port->CR1 & I2C_CR1_START) == I2C_CR1_START) return I2C_WRONG_START;
  }
  // Загоняем адрес на линию в режиме записи
  _I2C_Port->DR = (uint8_t) (DevAddress << 1) & ~1U;                                              // Выплёвываем адрес со сброшенным битом RD

  // Проверяем флаг ADDR и AF
  Status = _isAddressAF_Flag(TimeOut);
  if(Status != I2C_OK) return Status;

  // Очищаем флаги
  (void) _I2C_Port->SR1;
  (void) _I2C_Port->SR2;

  // Ждём когда поднимется TXE
  Status = _isTXE_Flag(TimeOut);
  if(Status != I2C_OK)
  {
    if(Status == I2C_AF_ERROR) _I2C_Port->CR1 |= I2C_CR1_STOP;
    return Status;
  }

  // Закидываем 8 или 16 бит адрес
  if(MemAddSise == Size_Add_8bit)
  {
    _I2C_Port->DR = MemAddress & 0xFF;                                                              // Пихаем 8бит адрес
  }
  else
  {
    _I2C_Port->DR = (MemAddress >> 8) & 0xFF;                                                       // Пихаем старшие 8 бит адрес
    // Ждём когда поднимется TXE
    Status = _isTXE_Flag(TimeOut);
    if(Status != I2C_OK)
    {
      if(Status == I2C_AF_ERROR) _I2C_Port->CR1 |= I2C_CR1_STOP;
      return Status;
    }
    _I2C_Port->DR = MemAddress & 0xFF;                                                              // Пихаем младшие 8 бит
    // Ждём когда поднимется TXE
    Status = _isTXE_Flag(TimeOut);
    if(Status != I2C_OK)
    {
      if(Status == I2C_AF_ERROR) _I2C_Port->CR1 |= I2C_CR1_STOP;
      return Status;
    }

    _I2C_Port->CR1 |= I2C_CR1_START;                                                                  // Выдаём старт
    // Проверяем SB и START
    if(_isSB_Flag(RESET, TimeOut))
    {
      if((_I2C_Port->CR1 & I2C_CR1_START) == I2C_CR1_START) return I2C_WRONG_START;
    }

    _I2C_Port->DR = (uint8_t) (DevAddress << 1) | 1U;                                              // Выплёвываем адрес с установленным битом RD
    // Проверяем флаг ADDR и AF
    Status = _isAddressAF_Flag(TimeOut);
    if(Status != I2C_OK) return Status;
  }
  return I2C_OK;
}

int16_t I2C::_RequestMemoryWrite(uint8_t DevAddress, uint16_t MemAddress, uint8_t MemAddSise, uint16_t TimeOut)
{
  int32_t Status;
  _I2C_Port->CR1 |= I2C_CR1_START;                                              // Запускаем START
  if(_isSB_Flag(RESET, TimeOut) != I2C_OK)
  {
    if((_I2C_Port->CR1 & I2C_CR1_START) == I2C_CR1_START)
    {
      return I2C_WRONG_START;
    }
    return I2C_TIME_OUT;
  }

  _I2C_Port->DR = (DevAddress << 1) & ~1U;                                      // Выдаём на шину адрес устройства
  // Проверяем флаг ADDR и AF
  Status = _isAddressAF_Flag(TimeOut);
  if(Status != I2C_OK) return Status;

  // Очищаем флаги
  (void) _I2C_Port->SR1;
  (void) _I2C_Port->SR2;

  // Ждём когда поднимется TXE
  Status = _isTXE_Flag(TimeOut);
  if(Status != I2C_OK)
  {
    if(Status == I2C_AF_ERROR) _I2C_Port->CR1 |= I2C_CR1_STOP;
    return Status;
  }

  // Закидываем 8 или 16 бит адрес
  if(MemAddSise == Size_Add_8bit)
  {
    _I2C_Port->DR = MemAddress & 0xFF;                                                              // Пихаем 8бит адрес
  }
  else
  {
    _I2C_Port->DR = (MemAddress >> 8) & 0xFF;                                                       // Пихаем старшие 8 бит адрес
    // Ждём когда поднимется TXE
    Status = _isTXE_Flag(TimeOut);
    if(Status != I2C_OK)
    {
      if(Status == I2C_AF_ERROR) _I2C_Port->CR1 |= I2C_CR1_STOP;
      return Status;
    }
    _I2C_Port->DR = MemAddress & 0xFF;                                                              // Пихаем младшие 8 бит
    // Ждём когда поднимется TXE
    Status = _isTXE_Flag(TimeOut);
    if(Status != I2C_OK)
    {
      if(Status == I2C_AF_ERROR) _I2C_Port->CR1 |= I2C_CR1_STOP;
      return Status;
    }
  }
  return I2C_OK;
}

/*
 * Функции проверки флагов
 */
// Проверка флага Busy
int16_t I2C::_isBusyFlag(bool FlagSet, uint16_t TimeOut)
{
  uint32_t TickStart = millis();
  while(((_I2C_Port->SR2 & I2C_SR2_BUSY) >> I2C_SR2_BUSY_Pos) == FlagSet)
  {
    if((millis() - TickStart) > TimeOut)
    {
      return I2C_ERROR;
    }
  }
  return I2C_OK;
}

// Проверяем SB флаг
int16_t I2C::_isSB_Flag(bool FlagSet, uint16_t TimeOut)
{
  uint32_t TickStart = millis();
  while(((_I2C_Port->SR1 & I2C_SR1_SB) >> I2C_SR1_SB_Pos) == FlagSet)
  {
    if((millis() - TickStart) > TimeOut)
    {
      return I2C_ERROR;
    }
  }
  return I2C_OK;
}

// Проверяем флаг ADDR  и AF
int16_t I2C::_isAddressAF_Flag(uint16_t TimeOut)
{
  uint32_t TickStart = millis();
  while(((_I2C_Port->SR1 & I2C_SR1_ADDR) >> I2C_SR1_ADDR_Pos) == RESET)
  {
    if(((_I2C_Port->SR1 & I2C_SR1_AF) >> I2C_SR1_AF_Pos) == SET)
    {
      _I2C_Port->CR1 |= I2C_CR1_STOP;
      _I2C_Port->SR1 &= ~I2C_SR1_AF;
      return I2C_AF_ERROR;
    }

    if((millis() - TickStart) > TimeOut)
    {
      return I2C_TIME_OUT;
    }
  }
  return I2C_OK;
}

// Проверка флага TXE
int16_t I2C::_isTXE_Flag(uint16_t TimeOut)
{
  uint32_t TickStart = millis();
  while(((_I2C_Port->SR1 & I2C_SR1_TXE) >> I2C_SR1_TXE_Pos) == RESET)
  {
    if(((_I2C_Port->SR1 & I2C_SR1_AF) >> I2C_SR1_AF_Pos) == SET)
    {
      _I2C_Port->SR1 &= ~I2C_SR1_AF;
      return I2C_AF_ERROR;
    }

    if((millis() - TickStart) > TimeOut)
    {
      return I2C_TIME_OUT;
    }
  }
  return I2C_OK;
}

// Проверка флага RXNE
int16_t I2C::_isRXNE_Flag(uint16_t TimeOut)
{
  uint32_t TickStart = millis();
  while(((_I2C_Port->SR1 & I2C_SR1_RXNE) >> I2C_SR1_RXNE_Pos) == RESET)
  {
    if(((_I2C_Port->SR1 & I2C_SR1_STOPF) >> I2C_SR1_STOPF_Pos) == SET)
    {
      _I2C_Port->SR1 &= ~I2C_SR1_STOPF;
      return I2C_STOPF_ERROR;
    }

    if((millis() - TickStart) > TimeOut)
    {
      return I2C_TIME_OUT;
    }
  }
  return I2C_OK;
}

// Проверка флага BTF
int16_t I2C::_isBTF_Flag(bool FlagSet, uint16_t TimeOut)
{
  uint32_t TickStart = millis();
  while(((_I2C_Port->SR1 & I2C_SR1_BTF) >> I2C_SR1_BTF_Pos) == FlagSet)
  {
    if((millis() - TickStart) > TimeOut)
    {
      return I2C_BTF_ERROR;
    }
  }
  return I2C_OK;
}

int16_t I2C::_isBTF_Flag_TimeOut(uint16_t TimeOut)
{
  uint32_t TickStart = millis();
  while(((_I2C_Port->SR1 & I2C_SR1_BTF) >> I2C_SR1_BTF_Pos) == RESET)
  {
    if(((_I2C_Port->SR1 & I2C_SR1_AF) >> I2C_SR1_AF_Pos) != I2C_OK) return I2C_AF_ERROR;

    if((millis() - TickStart) > TimeOut)
    {
      return I2C_BTF_ERROR;
    }
  }
  return I2C_OK;
}







