/*
    Лёгкая библиотека для работы с BME280 по I2C для Arduino
    Документация: 
    GitHub: https://github.com/GyverLibs/GyverBME280
    
    Egor 'Nich1con' Zaharov for AlexGyver, alex@alexgyver.ru
    https://alexgyver.ru/
    MIT License

    Версии:
    v1.3 - исправлена ошибка при отриц. температуре
    v1.4 - разбил на h и cpp
    v1.5 - добавлена поддержка BMP280

    Переработал для STM32 Анисимов Эдуард
    с согласия автора.
*/

#ifndef STM_BME280_H_
#define STM_BME280_H_

#include <STM32.h>
#include <I2C.h>
// Временно для отладки
//#include <UART.h>
//#include <Dump.h>

//

// Константы для управления
#define NORMAL_MODE	    0x03
#define FORCED_MODE	    0x02

#define STANDBY_500US   0x00
#define STANDBY_10MS    0x06
#define STANDBY_20MS    0x07
#define STANDBY_6250US  0x01
#define STANDBY_125MS   0x02
#define STANDBY_250MS   0x03
#define STANDBY_500MS   0x04
#define STANDBY_1000MS  0x05

#define MODULE_DISABLE	0x00
#define OVERSAMPLING_1  0x01
#define OVERSAMPLING_2  0x02
#define OVERSAMPLING_4  0x03
#define OVERSAMPLING_8  0x04
#define OVERSAMPLING_16 0x05

#define FILTER_DISABLE  0x00
#define FILTER_COEF_2   0x01
#define FILTER_COEF_4   0x02
#define FILTER_COEF_8   0x03
#define FILTER_COEF_16  0x04



class S_BME280
{

  public:
    S_BME280();   								                          // Создание объекта в классе BME280
    uint8_t Init(I2C *I2C_Ptr);							                // Инициализация сенсора с адресом по умолчанию 0x76
    uint8_t Init(I2C *I2C_Ptr, uint8_t address);				    // Инициализация сенсора с нестандартным адресом
    bool    isMeasuring(void);						                  // Возвращает 'true' если производится операция измерения
    float   readTemperature(void);                          // Чтение и вычисление температуры [float , *C]
    float   readPressure(void);					                    // Чтение и вычисление атмосферного давления [float , Pa]
    float   readHumidity(void);					                    // Чтение и вычисление влажности [float , %]
    void    oneMeasurement(void);					                  // Измерение и уход в сон [только  в FORCED_MODE]
    void    setMode(uint8_t mode);
    void    setFilter(uint8_t mode);	                      // Настройка фильтра при различных разрешениях в режимах отличных от стандартного [использовать перед Init()]
    void    setStandbyTime(uint8_t mode);		                // Настройка "сна" перед измерением [только в NORMAL_MODE][использовать перед Init()]
    void    setHumOversampling(uint8_t mode);               // Установить разрешение или отключить модуль измерения влажности [использовать перед Init()]
    void    setTempOversampling(uint8_t mode);	            // Установить разрешение или отключить модуль измерения теипературы [использовать перед Init()]
    void    setPressOversampling(uint8_t mode);             // Установить разрешение или отключить модуль измерения давления [использовать перед Init()]

  private:

    int     _i2c_address      = 0x76;					              // BME280 адрес на шине I2C
    uint8_t _operating_mode   = NORMAL_MODE;		            // Режим сенсора
    uint8_t _standby_time     = STANDBY_250MS;		          // Время перед измерением в NORMAL_MODE
    uint8_t _filter_coef      = FILTER_COEF_16; 		        // Коэффициент фильтра IIR
    uint8_t _temp_oversampl   = OVERSAMPLING_4;	            // Разрешение (точность) измерения температуры |
    uint8_t _hum_oversampl    = OVERSAMPLING_1;	            // Разрешение (точность) измерения влажности   |
    uint8_t _press_oversampl  = OVERSAMPLING_2;	            // Разрешение (точность) измерения давления    |
    I2C*    _I2C_Ptr;

    bool      _reset(void);                                  // BME280 програмный сброс
    int32_t   _readTempInt();                                // Чтение температуры в формате целого
    void      _readCalibrationData(void);                    // Чтение калибрационных ячеек
    uint8_t   _readRegister(uint8_t address);	  	           // Чтение одного 8-bit BME280 регистра
    uint32_t  _readRegister24(uint8_t address);              // Чтение трёх регистров BME280
    bool      _writeRegister(uint8_t address, uint8_t data); // Запись одного 8-bit регистра BME280
    bool      _isMeasuring(void);                            // Проверка состояния
    void      _oneMeasurement(void);                         // Одно измерение в форсированном режиме
    
//    void      _Debug(void); // Функция для отладки

    // Структура хранящая калибрационные данные
    struct
    {
      uint16_t  _T1;
      int16_t   _T2;
      int16_t   _T3;
      uint16_t  _P1;
      int16_t   _P2;
      int16_t   _P3;
      int16_t   _P4;
      int16_t   _P5;
      int16_t   _P6;
      int16_t   _P7;
      int16_t   _P8;
      int16_t   _P9;
      uint8_t   _H1;
      int16_t   _H2;
      uint8_t   _H3;
      int16_t   _H4;
      int16_t   _H5;
      int8_t    _H6;
    } CalibrationData;
};

float _pressureToMmHg(float pressure);		                  // Конвертация [Pa] в [mm Hg]
float _pressureToAltitude(float pressure);	                // Конвертация давления в высоту (Работает криво)

#endif
