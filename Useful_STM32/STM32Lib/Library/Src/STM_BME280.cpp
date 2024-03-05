#include <STM_BME280.h>
#include <math.h>

/*
 * Утилиты
 */

float _pressureToAltitude(float pressure)
{
  if(!pressure) return 0;									                                                // Если модуль измерения давления отключен, возвращается '0'
  pressure /= 100.0F;											                                                // Конвертация [Pa] в [hPa]
  return 44330.0 * (1.0 - pow(pressure / 1013.25, 0.1903));	                              // Вычисление высоты
}

float _pressureToMmHg(float pressure){ return (float) (pressure * 0.00750061683f); }			// Конвертация [Pa] в [mm Hg]

S_BME280::S_BME280()
{
  _I2C_Ptr = NULL;
}

/*
 * Настройки
 */
/* ============ Setup & begin ============ */

uint8_t S_BME280::Init(I2C* I2C_Ptr){ return Init(I2C_Ptr, 0x76); }

uint8_t S_BME280::Init(I2C* I2C_Ptr, uint8_t address)
{
  _i2c_address  = address;
  _I2C_Ptr      = I2C_Ptr;
  /* === Проверяем BME280 это или что то другое === */
  if(!_reset()) return false;                                                             // Програмный сброс и проверка ответа
  uint8_t ID = _readRegister(0xD0);                                                       // Читаем регистр ID
  if(ID != 0x60 && ID != 0x58) return false;	                                            // Если нужный ID (bme/bmp280), то всё в порядке
  _readCalibrationData();       					                                                // Читаем калибрационные данные

  /* === Загружаем настройки в BME280 === */
  _writeRegister(0xF2, _hum_oversampl);                                              			          //
  _writeRegister(0xF2, _readRegister(0xF2));                             			                      //
  _writeRegister(0xF4, ((_temp_oversampl << 5) | (_press_oversampl << 2) | _operating_mode));       //
  _writeRegister(0xF5, ((_standby_time << 5) | (_filter_coef << 2)));                			          //
  return ID;
}

void S_BME280::setMode(uint8_t mode){ _operating_mode = mode; }
void S_BME280::setFilter(uint8_t mode){ _filter_coef = mode; }
void S_BME280::setStandbyTime(uint8_t mode){ _standby_time = mode; }
void S_BME280::setHumOversampling(uint8_t mode){ _hum_oversampl = mode; }
void S_BME280::setTempOversampling(uint8_t mode){ _temp_oversampl = mode; }
void S_BME280::setPressOversampling(uint8_t mode){ _press_oversampl = mode; }

/* ============ Чиение ============ */

int32_t S_BME280::_readTempInt(void)
{
  int32_t temp_raw = _readRegister24(0xFA);			                                          // Читаем три регистра подряд
  if(temp_raw == 0x800000) return 0;								                                      // Если в регистре код как после сброса, возвращаем 0

  temp_raw >>= 4;													                                                // Рассчитываем температуру и применяем поправки
  int32_t value_1 = ((((temp_raw >> 3) - ((int32_t) CalibrationData._T1 << 1))) * ((int32_t) CalibrationData._T2)) >> 11;
  int32_t value_2 =
      (((((temp_raw >> 4) - ((int32_t) CalibrationData._T1)) * ((temp_raw >> 4) - ((int32_t) CalibrationData._T1))) >> 12)
          * ((int32_t) CalibrationData._T3)) >> 14;

  return ((int32_t) value_1 + value_2);							                                      // Возвращаем температуру как целое
}

float S_BME280::readTemperature(void)
{
  int32_t temp_raw = _readTempInt();
  float T = (temp_raw * 5 + 128) >> 8;
  return T / 100.0;												                                                // Возвращаем температуру после пересчёта
}

float S_BME280::readPressure(void)
{
  uint32_t press_raw;

  press_raw = _readRegister24(0xF7);			                                                // Читаем регистры хранящие давление
  if(press_raw == 0x800000) return 0;							                                        // Если значение в регистрах по умолчанию, возвращаем 0

  press_raw >>= 4;												                                                // Рассчитываем давление и применяем поправки
  int64_t value_1 = ((int64_t) _readTempInt()) - 128000;
  int64_t value_2 = value_1 * value_1 * (int64_t) CalibrationData._P6;
  value_2 = value_2 + ((value_1 * (int64_t) CalibrationData._P5) << 17);
  value_2 = value_2 + (((int64_t) CalibrationData._P4) << 35);
  value_1 = ((value_1 * value_1 * (int64_t) CalibrationData._P3) >> 8) + ((value_1 * (int64_t) CalibrationData._P2) << 12);
  value_1 = (((((int64_t) 1) << 47) + value_1)) * ((int64_t) CalibrationData._P1) >> 33;

  if(!value_1) return 0; 										                                              // На что только не пойдёшь, что бы на 0 не поделить

  int64_t p = 1048576 - press_raw;
  p = (((p << 31) - value_2) * 3125) / value_1;
  value_1 = (((int64_t) CalibrationData._P9) * (p >> 13) * (p >> 13)) >> 25;
  value_2 = (((int64_t) CalibrationData._P8) * p) >> 19;
  p = ((p + value_1 + value_2) >> 8) + (((int64_t) CalibrationData._P7) << 4);

  return (float) (p / 256);											                                          // Возвращаем давление в Паскалях
}

float S_BME280::readHumidity(void)
{
  int32_t hum_raw;
  int32_t value;
  uint8_t Buff[5] = {0,};
  uint8_t Temp;

  Buff[0] = 0xFD;
  Temp = _I2C_Ptr->MasterWrite(_i2c_address, Buff, 1, 12);
  if(Temp != I2C_OK) return 0;
  Temp = _I2C_Ptr->MasterRead(_i2c_address, Buff, 2, 12);

  hum_raw = ((uint16_t) Buff[0] << 8) | (uint16_t) Buff[1];
  if(hum_raw == 0x8000) return 0;

  value = (_readTempInt() - ((int32_t) 76800));
  value = (((((hum_raw << 14) - (((int32_t) CalibrationData._H4) << 20) - (((int32_t) CalibrationData._H5) * value))
      + ((int32_t) 16384)) >> 15)
      * (((((((value * ((int32_t) CalibrationData._H6)) >> 10)
          * (((value * ((int32_t) CalibrationData._H3)) >> 11) + ((int32_t) 32768))) >> 10) + ((int32_t) 2097152))
          * ((int32_t) CalibrationData._H2) + 8192) >> 14));
  value = (value - (((((value >> 15) * (value >> 15)) >> 7) * ((int32_t) CalibrationData._H1)) >> 4));
  value = (value < 0) ? 0 : value;
  value = (value > 419430400) ? 419430400 : value;
  float h = (value >> 12);

  return h / 1024.0;
}

/* ============ Мелочь всякая ============ */

bool S_BME280::_isMeasuring(void)
{
  return (bool) ((_readRegister(0xF3) & 0x08) >> 3);
}

void S_BME280::_oneMeasurement(void)
{
  _writeRegister(0xF4, ((_readRegister(0xF4) & 0xFC) | 0x02));
}


/* ============ Внутренние функции ============ */

/* = BME280 программный сброс = */
bool S_BME280::_reset(void)
{
  if(!_writeRegister(0x0E, 0xB6)) return false;
  delay(10);
  return true;
}


/* = Чтение трёх BME280 регистров = */
uint32_t S_BME280::_readRegister24(uint8_t address)
{
  uint8_t Buff[5];
  uint8_t Temp;
  Buff[0] = address;
  Temp = _I2C_Ptr->MasterWrite(_i2c_address, Buff, 1, 12);
  if(Temp != I2C_OK) return 0x800000;
  Temp = _I2C_Ptr->MasterRead(_i2c_address, Buff, 3, 12);
  return (((uint32_t) Buff[0] << 16) | ((uint32_t) Buff[1] << 8) | (uint32_t) Buff[2]);
}


/* = Записать байт в регистр BME280 = */
bool S_BME280::_writeRegister(uint8_t address, uint8_t data)
{
  uint8_t Buff[2];
  Buff[0] = data;
  _I2C_Ptr->MasterMemWrite(_i2c_address, address, Size_Add_8bit, Buff, 1, 12);
  return true;
}


/* = Читать байт из регистра BME280 = */
uint8_t S_BME280::_readRegister(uint8_t address)
{
  uint8_t Temp;
  uint8_t Buff[2];
  Temp = _I2C_Ptr->MasterWrite(_i2c_address, &address, 1, 12);
  if(Temp != I2C_OK) return Temp;
  Temp = _I2C_Ptr->MasterRead(_i2c_address, Buff, 1, 12);
  if(Temp != I2C_OK) return Temp;
  return Buff[0];
}

/* = Чтение калибрационных регистров и сохранение в структуре = */
void S_BME280::_readCalibrationData(void)
{
  uint8_t Buff[30] = {0,};
  Buff[0] = 0x88;

  _I2C_Ptr->MasterWrite(_i2c_address, Buff, 1, 12);
  _I2C_Ptr->MasterRead(_i2c_address, Buff, 25, 12);

  CalibrationData._T1 = (Buff[0] | (Buff[1] << 8));
  CalibrationData._T2 = (Buff[2] | (Buff[3] << 8));
  CalibrationData._T3 = (Buff[4] | (Buff[5] << 8));
  CalibrationData._P1 = (Buff[6] | (Buff[7] << 8));
  CalibrationData._P2 = (Buff[8] | (Buff[9] << 8));
  CalibrationData._P3 = (Buff[10] | (Buff[11] << 8));
  CalibrationData._P4 = (Buff[12] | (Buff[13] << 8));
  CalibrationData._P5 = (Buff[14] | (Buff[15] << 8));
  CalibrationData._P6 = (Buff[16] | (Buff[17] << 8));
  CalibrationData._P7 = (Buff[18] | (Buff[19] << 8));
  CalibrationData._P8 = (Buff[20] | (Buff[21] << 8));
  CalibrationData._P9 = (Buff[22] | (Buff[23] << 8));
  CalibrationData._H1 = Buff[24];

  Buff[0] = 0xE1;
  _I2C_Ptr->MasterWrite(_i2c_address, Buff, 1, 12);
  _I2C_Ptr->MasterRead(_i2c_address, Buff, 8, 12);

  CalibrationData._H2 = (Buff[0] | (Buff[1] << 8));
  CalibrationData._H3 = Buff[2];
  CalibrationData._H4 = (Buff[3] << 4);
  uint8_t interVal = Buff[4];
  CalibrationData._H4 |= (interVal & 0xF);
  CalibrationData._H5 = (((interVal & 0xF0) >> 4) | (Buff[5] << 4));
  CalibrationData._H6 = Buff[6];
}

/*
 * Функция использовалась для отладки
 */
//void S_BME280::_Debug(void)
//{
//  uint8_t Buff[32] = {0,};
//  Buff[0] = 0xF2;
//
//  _I2C_Ptr->MasterWrite(_i2c_address, Buff, 1, 12);
//  _I2C_Ptr->MasterRead(_i2c_address, Buff, 13, 12);
//
//  myUART.println("\r\n0xF2");
//  myUART.print("osrs_h : "); myUART.println(Buff[0], HEX);
//  while(myUART.BytesToWrite()){;}
//
//  myUART.println("\n\r0xF3");
//  myUART.print("im_update : "); myUART.print(Buff[1]&0x01, HEX); myUART.print(", measuring : "); myUART.println(Buff[1]>>3, HEX);
//  while(myUART.BytesToWrite()){;}
//
//  myUART.println("\n\r0xF4");
//  myUART.print("osrs_t : "); myUART.println(Buff[2]>>5, HEX);
//  myUART.print("osrs_p : "); myUART.println((Buff[2]>>2)&0x07, HEX);
//  delay(1);
//  myUART.print("mode   : "); myUART.println(Buff[2]&3, HEX);
//  while(myUART.BytesToWrite()){;}
//
//  myUART.println("\n\r0xF5");
//  myUART.print("t_sb   : "); myUART.println(Buff[3]>>5, HEX);
//  myUART.print("filter : "); myUART.println((Buff[3]>>2)&0x07, HEX);
//  while(myUART.BytesToWrite()){;}
//  myUART.print("spi3ew : "); myUART.println(Buff[3]&0x01, HEX);
//  while(myUART.BytesToWrite()){;}
//
//  myUART.println("\n\r0xF6");
//  myUART.print("NA     : "); myUART.println(Buff[4], HEX);
//  while(myUART.BytesToWrite()){;}
//
//  myUART.println("\n\r0xF7");
//  myUART.print("press  : "); myUART.println((Buff[5]<<16 | Buff[6]<<8 | Buff[7]), HEX);
//  while(myUART.BytesToWrite()){;}
//
//  myUART.println("\n\r0xFA");
//  myUART.print("temp   : "); myUART.println((Buff[8]<<16 | Buff[9]<<8 | Buff[10]), HEX);
//  while(myUART.BytesToWrite()){;}
//
//  myUART.println("\n\r0xFD");
//  myUART.print("hum    : "); myUART.println((Buff[11]<<8 | Buff[12]), HEX);
//  delay(1);
//}
//
