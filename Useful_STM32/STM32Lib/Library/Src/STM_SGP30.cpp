/*
 * TODO:
 * - Baseline store/restore
 * - proper init
 * - IAQ levels, getIAQLevel()
 */

#include <STM_SGP30.h>

S_SGP30::S_SGP30(){ _I2C_Ptr = NULL; }

int16_t S_SGP30::InitSensors(I2C *I2C_Ptr)
{
  _I2C_Ptr = I2C_Ptr;
  _i2c_address = SGP_I2C_ADDR;

//  SoftReset();

  if(initSGP() != 0)
  {
    setError("Error communicating with SGPC3");
    return -2;
  }
  mInitialized = true;

  uint8_t cmd[CMD_LENGTH] = {0x36, 0x82};

  i2c_write(_i2c_address, cmd, CMD_LENGTH);
  i2c_read(_i2c_address, mDataBuf, 6);
  SerialNumber[0] = ((mDataBuf[0] << 8) | mDataBuf[1]);
  SerialNumber[1] = ((mDataBuf[2] << 8) | mDataBuf[3]);
  SerialNumber[2] = ((mDataBuf[4] << 8) | mDataBuf[5]);

  return 0;
}

/*
 * SGP
 */
int16_t S_SGP30::getProductType() const {return mProductType;}
int16_t S_SGP30::getFeatureSetVersion() const {return mFeatureSetVersion;}

int16_t S_SGP30::measureIAQ()
{
  if(!mInitialized)
  {
    setError("ESS not initialized");
    return -1;
  }

  mSGPMeasurementTimestamp = millis();

  uint8_t cmd[CMD_LENGTH] = { 0x20, 0x08 };

  if(i2c_write(_i2c_address, cmd, CMD_LENGTH))
  {
    setError("error in i2c_write");
    return -1;
  }

  delay(SGP_MEASURE_DELAY);

  int ret = i2c_read(_i2c_address, mDataBuf, SGP_DATA_LENGTH);
  if(ret == -1)
  {
    setError("error in i2c_read");
    return -2;
  }

  if(crc8(mDataBuf, 2) != mDataBuf[2])
  {
    setError("CRC mismatch");
    return -3;
  }

  // SGPC3 only has TVOC; SGP30 sends [eco2, tvoc]
  if(mProductType == PRODUCT_TYPE_SGP30)
  {
    mECO2 = (uint16_t) (mDataBuf[0] << 8) | mDataBuf[1];
    if(crc8(mDataBuf + 3, 2) != mDataBuf[5])
    {
      setError("CRC mismatch");
      return -4;
    }
    mTVOC = (uint16_t) (mDataBuf[3] << 8) | mDataBuf[4];
  }
  else
  {
    mTVOC = (uint16_t) (mDataBuf[0] << 8) | mDataBuf[1];
  }

  return 0;
}

int16_t   S_SGP30::measureIAQraw()
{
  uint8_t cmd[CMD_LENGTH] = { 0x20, 0x50 };

  if(i2c_write(_i2c_address, cmd, CMD_LENGTH))
  {
    setError("error in i2c_write");
    return -1;
  }

  delay(SGP_MEASURE_DELAY);

  int ret = i2c_read(_i2c_address, mDataBuf, SGP_DATA_LENGTH);
  if(ret == -1)
  {
    setError("error in i2c_read");
    return -2;
  }

  if(crc8(mDataBuf, 2) != mDataBuf[2])
  {
    setError("CRC mismatch");
    return -3;
  }
  rawH2 = (mDataBuf[0] << 8) | mDataBuf[1];

  if(crc8(mDataBuf+3, 2) != mDataBuf[5])
  {
    setError("CRC mismatch");
    return -3;
  }
  rawEthanol = (mDataBuf[3] << 8) | mDataBuf[4];

  return 0;
}


int16_t S_SGP30::readFeatureSetInt()
{
  uint8_t cmd[CMD_LENGTH] = { 0x20, 0x2f };

  if(i2c_write(_i2c_address, cmd, CMD_LENGTH))
  {
    setError("error in i2c_write");
    return -1;
  }

  delay(2);

  const uint8_t DATA_LEN = 3;
  uint8_t data[DATA_LEN] = { 0 };
  int ret = i2c_read(_i2c_address, data, DATA_LEN);
  if(ret == -1)
  {
    setError("I2C read error");
    return -3;
  }

  // check CRC
  if(crc8(data, 2) != data[2])
  {
    setError("CRC mismatch");
    return -4;
  }

  // 0 = SGP30, 1 = SGPC3
  mProductType = (data[0] & 0xF0) >> 4;
  mFeatureSetVersion = data[1] & 0xFF;

  return 0;
}

bool S_SGP30::setHumidity(uint32_t absolute_humidity)
{
  if (absolute_humidity > 256000){return false;}

  uint16_t ah_scaled = (uint16_t)(((uint64_t)absolute_humidity * 256 * 16777) >> 24);
  uint8_t command[5];
  command[0] = 0x20;
  command[1] = 0x61;
  command[2] = ah_scaled >> 8;
  command[3] = ah_scaled & 0xFF;
  command[4] = crc8(command+2, 2);

  return i2c_write(_i2c_address, command, 5);
}

int16_t S_SGP30::initSGP()
{
  int ret = readFeatureSetInt();
  // default: SGP30
  SGP_INTERMEASURE_DELAY = SGP30_INTERMEASURE_DELAY;
  SGP_DATA_LENGTH = SGP30_DATA_LENGTH;
  uint8_t cmd[CMD_LENGTH] = { 0x20, 0x03 };

  if(mProductType == PRODUCT_TYPE_SGPC3)
  {
    SGP_INTERMEASURE_DELAY = SGPC3_INTERMEASURE_DELAY;
    cmd[1] = 0xAE;
  }

  // run init air quality
  if(i2c_write(_i2c_address, cmd, CMD_LENGTH))
  {
    setError("error in i2c_write");
    return -1;
  }
  delay(15);

  return ret;
}

//////////////////////////////////////////////////////////////////////////////
// getter for values read earlier
bool S_SGP30::isInitialized(){return mInitialized;}
uint16_t S_SGP30::getTVOC() const{return mTVOC;}
uint16_t S_SGP30::getECO2() const{return mECO2;}
uint16_t S_SGP30::getrawH2() const{return rawH2;}
uint16_t S_SGP30::getrawEth() const{return rawEthanol;}

////////////////////////////////////////////////////////////////////////////
// error handling

inline void S_SGP30::setError(const char *error){strlcpy(mErrorBuf, error, ERROR_BUF_LENGTH);}
const char* S_SGP30::getError() const{return mErrorBuf;}

//////////////////////////////////////////////////////////////////////////////
// helper

int16_t S_SGP30::remainingWaitTimeMS()
{
  uint32_t deltaT = millis() - mSGPMeasurementTimestamp;
  if(deltaT > SGP_INTERMEASURE_DELAY)
  {
    // we're already late, don't wait any longer
    return 0;
  }
  return (SGP_INTERMEASURE_DELAY - deltaT);
}

int8_t S_SGP30::SoftReset(void)
{
  uint8_t cmd[CMD_LENGTH] = {0x00, 0x06};
  int8_t  tmp;
  tmp = i2c_write(_i2c_address, cmd, CMD_LENGTH);
  delay(15);
  return tmp;
}
/*
 * Работаем с I2C
 */
int8_t S_SGP30::i2c_read(uint8_t addr, uint8_t *data, uint16_t count)
{
  if(_I2C_Ptr->MasterRead(addr, data, count, 10) != I2C_OK) return -1;
  return 0;
}

int8_t S_SGP30::i2c_write(uint8_t addr, uint8_t *data, uint16_t count)
{
  if(_I2C_Ptr->MasterWrite(addr, data, count, 10) != I2C_OK) return -1;
  return 0;
}

/*
 * Дополнительные функции
 */
uint8_t S_SGP30::crc8(const uint8_t *data, uint8_t len)
{
  // adapted from SHT21 sample code from http://www.sensirion.com/en/products/humidity-temperature/download-center/

  uint8_t crc = 0xff;
  uint8_t byteCtr;
  for (byteCtr = 0; byteCtr < len; ++byteCtr)
  {
    crc ^= (data[byteCtr]);
    for (uint8_t bit = 8; bit > 0; --bit)
    {
      if(crc & 0x80)
      {
        crc = (crc << 1) ^ 0x31;
      }
      else
      {
        crc = (crc << 1);
      }
    }
  }
  return crc;
}

uint32_t S_SGP30::getAbsoluteHumidity(float temperature, float humidity)
{
  // approximation formula from Sensirion SGP30 Driver Integration chapter 3.15
  const float absoluteHumidity = 216.7f
      * ((humidity / 100.0f) * 6.112f * exp((17.62f * temperature) / (243.12f + temperature)) / (273.15f + temperature)); // [g/m^3]
  const uint32_t absoluteHumidityScaled = static_cast<uint32_t>(1000.0f * absoluteHumidity); // [mg/m^3]
  return absoluteHumidityScaled;
}
