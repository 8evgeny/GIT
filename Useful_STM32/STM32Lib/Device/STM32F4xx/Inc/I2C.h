/*
 * I2C.h
 *
 *  Created on: 2 февр. 2022 г.
 *      Author: User
 *      Использует I2C единолично. Может имеет смысл написать так, что бы отдавать ссылку на проинициализированный
 *      интерфейс, как в GFX_Button. Иначе он будет пользоваться им единолично.
 */

#ifndef INC_I2C_H_
#define INC_I2C_H_

#include "STM32.h"

#define I2C_OK              0
#define I2C_TIME_OUT        -1
#define I2C_BUSY            -2
#define I2C_WRONG_START     -3
#define I2C_ERROR           -4
#define I2C_NOT_ADDRESS     -5
#define I2C_RXNE_ERROR      -6
#define I2C_AF_ERROR        -7
#define I2C_STOPF_ERROR     -8
#define I2C_BTF_ERROR       -9
#define I2C_TXNE_ERROR      -10
#define I2C_SB_ERROR        -11
#define I2C_END             -20


enum Mode_Speed
{
  Mode_Slow = 0,
  Mode_Fast = 1               // Этот режим ещё не дописан.
};

enum Mem_AddrSize
{
  Size_Add_16bit,
  Size_Add_8bit
};


class I2C
{
  public:
    I2C(I2C_TypeDef *I2C_Port, bool Alternate = false);                         // Конструктор I2C
    void      Init(uint8_t Mode);                                               // Инициализация I2C Slow/Fast
    int16_t   isReady(uint16_t DevAddress, uint8_t Repeat, uint16_t TimeOut);   // Проверка готовности к обмену устройства с известным адресом
    int16_t   ScanDevice(uint8_t *BuffDevice);                                  // Сканирование устройств на шине
    int16_t   MasterRead(uint8_t DevAddress, uint8_t* DataBuff, uint16_t Size, uint16_t TimeOut);                 // Чтение с устройства
    int16_t   MasterWrite(uint8_t DevAddress, uint8_t* DataBuff, uint16_t Size, uint16_t TimeOut);                 // Чтение с устройства
    int16_t   MasterMemRead(uint8_t DevAddress, uint16_t MemAddress, uint8_t MemAddSise, uint8_t* DataBuff, uint16_t Size, uint16_t TimeOut);
    int16_t   MasterMemWrite(uint8_t DevAddress, uint16_t MemAddress, uint8_t MemAddSise, uint8_t* DataBuff, uint16_t Size, uint16_t TimeOut);

  private:
    int16_t   _SendSlaveAddress(uint8_t DevAddress, uint16_t TimeOut);          // Далее служебные функции
    int16_t   _RequestMemoryRead(uint8_t DevAddress, uint16_t MemAddress, uint8_t MemAddSise, uint16_t TimeOut);
    int16_t   _RequestMemoryWrite(uint8_t DevAddress, uint16_t MemAddress, uint8_t MemAddSise, uint16_t TimeOut);
    int16_t   _RequestDataRead(uint8_t DevAddress, uint16_t TimeOut);
    int16_t   _RequestDataWrite(uint8_t DevAddress, uint16_t TimeOut);
    int16_t   _isBusyFlag(bool FlagSet, uint16_t TimeOut);                      // Проверка занятости устройства
    int16_t   _isSB_Flag(bool FlagSet, uint16_t TimeOut);                       // Проверка флага SB
    int16_t   _isAddressAF_Flag(uint16_t TimeOut);                              // Проверка подтверждения адреса
    int16_t   _isTXE_Flag(uint16_t TimeOut);                                    // Проверка конца передачи
    int16_t   _isRXNE_Flag(uint16_t TimeOut);                                   // Проверка конца приёма
    int16_t   _isBTF_Flag(bool FlagSet, uint16_t TimeOut);                      // Проверка флага BTF
    int16_t   _isBTF_Flag_TimeOut(uint16_t TimeOut);                            // Ещё одна проверка BTF с таймаутом

    I2C_TypeDef *_I2C_Port;                                                     // Адрес порта I2C
    bool        _Alternate;                                                     // Использование альтернативных выводов
    IRQn_Type   _IRQn_ER, _IRQn_EV;                                             // Вектора прерываний (пока не используется)
    uint8_t     _Mode;                                                          // Хранит режим I2C

};

#endif /* INC_I2C_H_ */
