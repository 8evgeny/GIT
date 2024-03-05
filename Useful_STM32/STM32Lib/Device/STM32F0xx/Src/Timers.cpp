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
  while(_SysTickCounter - Counter < MS)
    {
    }
}

// Калибровка системы задержки
void usCalibr(void)
{
  uint32_t load;
  uint32_t SOME_TICKS = _CyclePerMicroseconds;

  bool primask = __get_PRIMASK();
  __disable_irq();

  load = SysTick->LOAD;

  SysTick->LOAD = 0x00FFFFFF;
  SysTick->VAL = 0;

  uint32_t n0 = SysTick->VAL;

  delayMicroseconds(SOME_TICKS);

  _usCalibr = n0 - SysTick->VAL - (SOME_TICKS * _CyclePerMicroseconds);

  SysTick->LOAD = load;
  SysTick->VAL = 0;

  __enable_irq();
  __set_PRIMASK(primask);
}

// Задержка м микросекундах
void delayMicroseconds(uint16_t US)
{
  uint32_t Cycles = US * _CyclePerMicroseconds;

  uint32_t n0 = SysTick->VAL;
  uint32_t np = n0;
  uint32_t nc;

  if(Cycles > _usCalibr) Cycles -= _usCalibr; else return;

  do
  {
    nc = SysTick->VAL;
    if(nc >= np)
      n0 += SysTick->LOAD + 1;
    np = nc;
  } while ((n0 - nc) < Cycles);
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

