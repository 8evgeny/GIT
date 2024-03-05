//******************************************************************************
//  Секция include: здесь подключается заголовочный файл к модулю
//******************************************************************************
#include  <stdint.h>
#include "Timers.h"      // Включаем файл заголовка для нашего модуля
#include "STM32.h"

//******************************************************************************
//  Секция определения переменных, используемых в модуле
//******************************************************************************

//------------------------------------------------------------------------------
// Глобальные
//------------------------------------------------------------------------------
extern uint32_t vectorTable_RAM[];
//char GlobalVar1;
//char GlobalVar2;

//------------------------------------------------------------------------------
// Локальные
//------------------------------------------------------------------------------
static volatile uint32_t _SysTickCounter;
static volatile uint32_t _CyclePerMicroseconds;
static volatile uint32_t _usCalibr = 0;
//******************************************************************************
//  Секция прототипов локальных функций
//******************************************************************************
void usCalibr(void);                                                            // Функция калибровки таймера микросекундной задержки

//******************************************************************************
//  Секция описания глобальных функций
//******************************************************************************
  // Инициализируем SysTick
void SysTickConfig(void)
{
//  __disable_irq();
//  vectorTable_RAM[SysTick_Handler] = (uint32_t)SysTickIRQ;       // Подменяем на свой вектор прерывания
//  __enable_irq();

  SystemCoreClockUpdate();
  SysTick_Config(SystemCoreClock/1000);
  NVIC_EnableIRQ(SysTick_IRQn);
  _CyclePerMicroseconds = SystemCoreClock/1000000;
  usCalibr();
}

// Обработка прерывания от SysTick
extern "C" void SysTick_Handler(void)
{
  SysTick->CTRL;                                                                // Сбрасываем флаг прерывания от SysTick
  _SysTickCounter++;                                                            // Инкрементируем переменную счётчик миллисекунд
}

//void SysTickIRQ(void)
//{
//  SysTick->CTRL;                                                                // Сбрасываем флаг прерывания от SysTick
//  _SysTickCounter++;                                                            // Инкрементируем переменную счётчик миллисекунд
//}


//Функция millis()
uint32_t millis(void)
{
  return _SysTickCounter;
}

// Функция micros()
uint32_t micros(void)
{
  return(SysTick->LOAD - SysTick->VAL)/_CyclePerMicroseconds + _SysTickCounter*1000;
}

// Функция delay()
void delay(uint16_t MS)
{
  uint32_t Counter = _SysTickCounter;
  while(_SysTickCounter - Counter < MS){}
}

// Калибровка системы задержки
void usCalibr(void)
{
  uint32_t load = SysTick->LOAD;                                                // Сохраняем значение регистра SysTick->LOAD.
  uint32_t SOME_TICKS = 10; // (uint32_t) 500/_CyclePerMicroseconds;             // 500 тиков МК переводим в микросекунды
  uint32_t n0;
  // Устанавливаем достаточно большой модуль
  // пересчёта для счётчика системного таймера.
  SysTick->LOAD = 0x00FFFFFF;
  // Сбрасываем счётчик системного таймера записью
  // в его регистр любого числа.
  SysTick->VAL = 0;
  bool primask = __get_PRIMASK();                                               // Читаем регистр запрета исключений.
  __disable_irq();                                                              // Запрещаем исключения.
  // Собственно калибровка функции ndelay.
  n0 = SysTick->VAL;
  delayMicroseconds(SOME_TICKS);
  _usCalibr = n0 - SysTick->VAL - SOME_TICKS;
  SysTick->LOAD = load;                                                         // Восстанавливаем конфигурацию системного
  SysTick->VAL = 0;                                                             // таймера и сбрасываем его счётчик.
  // Восстанавливаем предыдущее состояние
  // регистра запрета исключений.
  __set_PRIMASK(primask);
}

// Задержка м микросекундах
void delayMicroseconds(uint16_t US)
{

  uint32_t _CurTick   = SysTick->VAL;                                           // Начальное значение счётчика SysTick.
  uint32_t _PrewTick  = _CurTick;                                               // Предыдущее значение счётчика.
  uint32_t _Ticks     = (uint32_t) US*_CyclePerMicroseconds;                 // Расчёт количества тактов
  uint32_t _LastTick;                                                           // Текущее значение счётчика SysTick.
  // Корректировка.
  if(_Ticks > _usCalibr) _Ticks -= _usCalibr; else return;

  // Выполняем цикл до тех пор, пока не пройдёт
  // заданное количество тактов процессора.
  do
  {
    _LastTick = SysTick->VAL;
    if(_LastTick >= _PrewTick) // Проверка на переполнение, корректировка на модуль пересчёта в случае переполнения.
      _CurTick += SysTick->LOAD + 1;
    _PrewTick = _LastTick;
  } while ((_CurTick - _LastTick) < _Ticks);

}

//******************************************************************************
//  Секция описания локальных функций
//******************************************************************************
//void local_func1 (void)
//{
//    ...;
//}

//******************************************************************************
//  ENF OF FILE
//******************************************************************************

