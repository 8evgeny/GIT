//****************************************************************************** 
//  Секция include: здесь подключается заголовочный файл к модулю 
//****************************************************************************** 
#include <RS485.h>     // Включаем файл заголовка для нашего модуля
#include  <stdint.h>

//****************************************************************************** 
//  Секция определения переменных, используемых в модуле 
//****************************************************************************** 

//------------------------------------------------------------------------------ 
// Глобальные  
//------------------------------------------------------------------------------ 
//char GlobalVar1; 
//char GlobalVar2; 

//------------------------------------------------------------------------------ 
// Локальные 
//------------------------------------------------------------------------------ 
//static char LocalVar1; 
//static char LocalVar2; 

//****************************************************************************** 
//  Секция прототипов локальных функций 
//****************************************************************************** 
//else Heading.Address = Address;

//void local_func1 (void); 
//void local_func2 (void); 

//****************************************************************************** 
//  Секция описания глобальных функций
//****************************************************************************** 
size_t RS485::Send(uint8_t* Buffer, uint8_t Length, uint8_t TxAddr)
{
  uint8_t SendBuff[32] = {0,};

  _GPIO_Port->BSRR |= _GPIO_Pin;

  /*
   * !!! Заголовок при передаче должен быть переинициализирован.
   * Так как приём осуществляется в него  и будет затёрт новыми данными
   *
   * Инициализируем структуру заголовка и переменные
   */
  Heading.StartByte   = _StartByte;
  Heading.Address     = TxAddr;
  Heading.QuantBytes  = Length;
  Heading.MaskCorrect = 0;
  Heading.Mask        = 0;

  // --------- Инициализация Master/Slave и модификация флагов количества передаваемых байт -------
  if(_Role == RS485_MASTER) Heading.QuantBytes |= 0x80;                         // Если мастер, выставляем первый бит в адресе
  if((TxAddr == _StartByte) || (TxAddr == _StopByte))                           // Если байт адреса стал похож на Старт/Стоп байты,
  {
    Heading.Address = TxAddr ^ 0x01;                                            // чекрыжим его
    Heading.QuantBytes |= 0x40;                                                 // и выставляем нужный флаг
  }

  // ---------- Подготавливаем буфер передачи ---------
  _DataBuff = Buffer;                                                           // Присваиваем нашему указателю адрес буфера
  uint32_t I=0x80000000;                                                        // Первый бит маскировщика в "1"

  for(uint8_t J=0; J<Length; J++ )                                              // И начинаем набивать буфер
  {
    SendBuff[J] = *_DataBuff;                                                   // Присваиваем буферу передачи указатель на передаваемый буфер
    if((*(uint8_t*) _DataBuff == _StartByte) || (*(uint8_t*) _DataBuff == _StopByte)) // Если передаваемый байт похож на на старт/стоп,
    {
      SendBuff[J] ^= 0x01;                                                      // то чекрыжим его
      Heading.Mask |= I;                                                        // и в Маске выставляем нужный бит
    }
    I = I>>1;                                                                   // сдвигаем маскировщик на следующую позицию
    _DataBuff++;                                                                // выбираем следующий байт
  }

  // ---------- Проверяем регистр Маски  и вносим корректировки там, где необходимо -----------
  if(((Heading.Mask & 0xFF000000) == (uint32_t) (_StartByte << 24)) || ((Heading.Mask & 0xFF000000) == (uint32_t)(_StopByte << 24)))
  {
    Heading.MaskCorrect |= MaskCorrect_3;
    Heading.Mask ^= 0x01000000;
  }
  if(((Heading.Mask & 0x00FF0000) == (uint32_t)(_StartByte << 16)) || ((Heading.Mask & 0x00FF0000) == (uint32_t)(_StopByte << 16)))
  {
    Heading.MaskCorrect |= MaskCorrect_2;
    Heading.Mask ^= 0x00010000;
  }
  if(((Heading.Mask & 0x0000FF00) == (uint32_t)(_StartByte << 8)) || ((Heading.Mask & 0x0000FF00) == (uint32_t)(_StopByte << 8)))
  {
    Heading.MaskCorrect |= MaskCorrect_1;
    Heading.Mask ^= 0x00000100;
  }
  if(((Heading.Mask & 0x000000FF) == (uint32_t)_StartByte) || ((Heading.Mask & 0x000000FF) == (uint32_t)_StopByte))
  {
    Heading.MaskCorrect |= MaskCorrect_0;
    Heading.Mask ^= 0x00000001;
  }


  // Рассчитываем CRC !!! нескорректированного !!! буфера передачи
  _CRC_8 = crc8(Buffer, Length);
  if((_CRC_8 == _StartByte) || (_CRC_8 == _StopByte))
  {
    _CRC_8 ^= 0x01;                 // Если CRC совпадает со стартовым или стоповым битом, Крыжим с  "1"
    Heading.MaskCorrect |= MaskCRC;
  }

  // Выплёвываем пакет
  write((uint8_t*) &Heading, sizeof(Heading));
  write(SendBuff, Length);
//  Transmit((uint8_t*) &Heading, sizeof(Heading));
//  Transmit(SendBuff, Length);
  write(_CRC_8);
  write(_StopByte);

  while(!isTxFinish()){;}
//  delay(500);  // !!! Временно
  _GPIO_Port->BSRR |= (_GPIO_Pin << 16);

  return Length;                                                                // Возвращаем количество переданных байт
}

/*
 * Блокирующая функция приёма. Вызывается постоянно в цикле, когда ожидается приём информации
 * Имеет таймаут в 1000 миллисекунд.
 * Если пришли неожидаемые данные, функция не блокирующая
 */
size_t RS485::Receive(uint8_t *Buffer)
{
  int16_t Temp;                                                                 // Переменная для считывания байта
  uint8_t *HeadingPtr = NULL;                                                   // Указатель на заголовок
  HeadingPtr = (uint8_t*) &Heading;
  Heading =
  { 0, };                                                                       // Обнуляем заголовок, что бы не путать переданный с принятым
  uint32_t TimeoutCounter;                                                      // Счётчик для таймаута

  Temp = read();                                                                // Читаем байт
  if((Temp != RS485_Empty) && ((uint8_t) Temp == _StartByte))                   // Если байта нет, вываливемся с ошибкой -1
  {
    // ---- Всё отлично, пытаемся принять заголовок -----
    for (uint8_t J = 0; J < sizeof(Heading); J++)                               // Принимаем заголовок
    {
      if(Temp == _StopByte) return RS485_StructureNoValid;
      HeadingPtr[J] = Temp;                                                     // Принятый байт пишем в структуру
      TimeoutCounter = millis() + 1000;
      do
      {
        if(millis() > TimeoutCounter) return RS485_RxTimeOut;
        Temp = read();
      } while (Temp == RS485_Empty);
    } // Заголовок принят

    // ------ Делаем поправки в заголовке ------
    if(Heading.QuantBytes & 0x40) Heading.Address ^= 0x01;                      // Если требуется, делаем корректировку
    if(Heading.Address != _Address) return RS485_Empty;                         // Если стучатся не к нам, бросаем всё.
    if(!(Heading.QuantBytes & 0x80) ^ _Role) return RS485_Empty;                // Откидываем данные, если они от такого же устройства (Мастер/Slave)
    switch (Heading.MaskCorrect)
    // Корректируем маску
    {
      case MaskCorrect_0:
        Heading.Mask ^= 0x00000001;
        break;
      case MaskCorrect_1:
        Heading.Mask ^= 0x00000100;
        break;
      case MaskCorrect_2:
        Heading.Mask ^= 0x00010000;
        break;
      case MaskCorrect_3:
        Heading.Mask ^= 0x01000000;
        break;
      default:
        break;
    } // Введены поправки в заголовок

    // ---------- Принимаем массив данных ----------
    uint32_t I = Heading.Mask;                                                  // Считываем маску коррекции
    for (uint8_t J = 0; J < (Heading.QuantBytes & 0x3F); J++)
    {
      if((Temp == _StopByte) || (Temp == _StartByte)) return RS485_StructureNoValid;
      if(I & 0x80000000) Temp ^= 0x01;
      I = I << 1;
      Buffer[J] = Temp;
      TimeoutCounter = millis() + 1000;
      do
      {
        if(millis() > TimeoutCounter) return RS485_RxTimeOut;
        Temp = read();
      } while (Temp == RS485_Empty);
    } // Массив принят

    // ------------- Принимаем CRC -------------
    _CRC_8 = crc8(Buffer, (Heading.QuantBytes & 0x3F));

    if(Heading.MaskCorrect & MaskCRC) Temp ^= 0x01;
    if(_CRC_8 != Temp) return RS485_CRCNoValid;
    // Приняли и сравнили CRC

    // -------- Принимаем стоп байт -----------
    TimeoutCounter = millis() + 1000;
    do
    {
      if(millis() > TimeoutCounter) return RS485_RxTimeOut;
      Temp = read();
    } while (Temp == RS485_Empty);

    if(Temp != _StopByte) return RS485_StopNoValid;
    // Пакет принят полностью

    return Heading.QuantBytes & 0x3F;
  }
  return RS485_Empty;
}
//****************************************************************************** 
//  Секция описания локальных функций
//****************************************************************************** 
uint8_t RS485::crc8(const uint8_t *addr, uint8_t len)
{
  uint8_t crc = 0;

  while (len--)
  {
    uint8_t inbyte = *addr++;
    for (uint8_t i = 8; i; i--)
    {
      uint8_t mix = (crc ^ inbyte) & 0x01;
      crc >>= 1;
      if(mix) crc ^= 0x8C;
      inbyte >>= 1;
    }

  }
  return crc;
}

//****************************************************************************** 
//  ENF OF FILE 
//****************************************************************************** 

