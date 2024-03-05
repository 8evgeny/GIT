/*
 * ADC.cpp
 *
 *  Created on: 30 сент. 2021 г.
 *      Author: User
 */

//****************************************************************************** 
//  Секция include: здесь подключается заголовочный файл к модулю 
//****************************************************************************** 
#include  "ADC.h"      // Включаем файл заголовка для нашего модуля 

IO_Analog::IO_Analog(ADC_TypeDef *ADCx, GPIO_TypeDef *GPIO)
{
  _ADCx     = ADCx;
  _GPIOx    = GPIO;
}

//IO_Analog::IO_Analog()
//{
//  _ADCx     = ADC1;
//  _GPIOx    = GPIOA;
//}

/*
 *
 */
uint16_t  IO_Analog::AnalogRead(uint16_t Channel)
{
  uint32_t  _GPIO_Pin_Mode;
  uint16_t  _Result;
  uint32_t  Position;                                                            // Счётчик позиции.
  uint32_t  IO_Position = 0x00U;                                                 // Указатель позиции
  uint32_t  IO_Current = 0x00U;                                                  // Текущая позиция

  // Если тактирование порта не включено, включаем его
  if(!(  RCC->IOPENR & RCC_IOPENR_GPIOAEN))   RCC->IOPENR |= RCC_IOPENR_GPIOAEN;

  // Запоминаем настройки порта ввода/вывода
  for (Position = 0U; Position < 16; Position++)                                // Перебираем все возможные позиции
  {
    IO_Position = 0x01U << Position;                                            // Инициализируем указатель на позицию
    IO_Current = (uint32_t) (Channel) & IO_Position;                            // Выделяем текущую позицию из списка переданного в функцию

    if(IO_Current == IO_Position)                                               // Если такая позиция существует. Идём её обрабатывать
    {
      _GPIO_Pin_Mode = (_GPIOx->MODER) & (0b11 << (Position * 2));
      _GPIOx->MODER |= (0b11 << (Position * 2));                                // Включаем нужный порт на ввод GPIO
      _ADCx->CHSELR |= Channel;                                                 // Включаем нужный канал АЦП
      break;                                                                    // Вываливаемся из цикла, больше одного вывода нельзя
                                                                                // инициализировать. Position хранит текущий номер вывода.
    }
  }
  // Включаем внутренний Vrefint
  ADC->CCR |= ADC_CCR_VREFEN;
  // Запускаем преобразование
  _ADCx->CR     |= ADC_CR_ADEN;                                                 // Включаем АЦП
  while(!(_ADCx->ISR & ADC_ISR_ADRDY)){;}                                       // Ждём запуска
  _ADCx->CR     |= ADC_CR_ADSTART;                                              // Запускаем преобразование
  while(!(_ADCx->ISR & ADC_ISR_EOS)){;}                                         // Ждём окончания преобразования
  _Result = _ADCx->DR;                                                          // Так то не нужно, но на всякий пожарный
//  _ADCx->CR     |= ADC_CR_ADDIS;                                                // Выключаем АЦП
  _ADCx->ISR    |= ADC_ISR_EOS;                                                 // Сбрасываем флаг секвенсора
  // Восстанавливаем настройку порта ввода/вывода
  _ADCx->CHSELR &= ~Channel;                                                    // Выключаем использованный канал АЦП
  _GPIOx->MODER &= ~(0b11 << (Position * 2));                                   // Сбрасываем и затем восстанавливаем
  _GPIOx->MODER |= _GPIO_Pin_Mode;                                              // старое значение инициализации порта

  // Замеряем внутренний источник
  _ADCx->CHSELR |= ADC_CHSELR_CHSEL17;                                          // Включаем канал Vrefint
  // Запускаем преобразование
//  _ADCx->CR     |= ADC_CR_ADEN;                                                 // Включаем АЦП
//  while(!(_ADCx->ISR & ADC_ISR_ADRDY)){;}                                       // Ждём запуска
  _ADCx->CR     |= ADC_CR_ADSTART;                                              // Запускаем преобразование
  while(!(_ADCx->ISR & ADC_ISR_EOS)){;}                                         // Ждём окончания преобразования
  _InternalVCC = _ADCx->DR;                                                     // Запоминаем внутренний Vref
  _ADCx->CHSELR &= ~ADC_CHSELR_CHSEL17;                                         // Выключаем использованный канал АЦП
  _ADCx->CR     |= ADC_CR_ADDIS;                                                // Выключаем АЦП
  _ADCx->ISR    |= ADC_ISR_EOS;                                                 // Сбрасываем флаг секвенсора

  // Возвращаем полученное значение
  return _Result;
}

/*
 * Инициализация АЦП
 */
void IO_Analog::Init(void)
{
  RCC->APB2ENR  |= RCC_APB2ENR_ADCEN;                                           // Включаем тактирование АЦП
  _ADCx->CFGR2  &= ~(ADC_CFGR2_CKMODE_0 | ADC_CFGR2_CKMODE_1);                  // Асинхронное преобразование
  ADC->CCR      &= ~(0b0000 << ADC_CCR_PRESC_Pos);                              // Тактовую делим на 2
  ADC->CCR      |= ADC_CCR_PRESC_1;                                             //
  _ADCx->SMPR   |= ADC_SMPR_SMPR_1 | ADC_SMPR_SMPR_2;                           // 79.5 циклов преобразования

  // Калибровка
  if(!(_ADCx->CR & ADC_CR_ADEN) && !(_ADCx->CFGR1 & ADC_CFGR1_DMAEN) && (_CalibrationFactor == 0))
  {
    ADC1->CR      |= ADC_CR_ADCAL;                                              // Калибруем и запоминаем значение
    while((ADC1->CR & ADC_CR_ADCAL) != 0){}
    _CalibrationFactor = _ADCx->CALFACT;
  }

  // Включаем передескритизатор
  _ADCx->CFGR2 |= 0b0100 << ADC_CFGR2_OVSS_Pos;
  _ADCx->CFGR2 |= 0b011 << ADC_CFGR2_OVSR_Pos;
  _ADCx->CFGR2 |= ADC_CFGR2_OVSE;
}
