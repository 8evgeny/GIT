#ifndef INC_STM_SGP30_H_OLD_
#define INC_STM_SGP30_H_OLD_

#include <math.h>
#include "STM32.h"
#include <stdint.h>
#include <string.h>
#include "I2C.h"

class S_SGP30
{
  public:
    S_SGP30();

    int16_t InitSensors(I2C *I2C_Ptr);

    int16_t   measureIAQ();
    int16_t   measureIAQraw();
    bool      isInitialized();

    int16_t   getProductType() const;
    int16_t   getFeatureSetVersion() const;
    uint32_t  getAbsoluteHumidity(float temperature, float humidity);
    bool      setHumidity(uint32_t absolute_humidity);

    uint16_t  getTVOC() const;
    uint16_t  getECO2() const; // SGP30 only
    uint16_t  getrawH2() const;
    uint16_t  getrawEth() const;
    uint16_t  SerialNumber[3];

    const char* getError() const;

    int16_t   remainingWaitTimeMS();

    const static int PRODUCT_TYPE_SGP30 = 0;
    const static int PRODUCT_TYPE_SGPC3 = 1;

  private:
    int       _i2c_address = 0x58;                            // SGP адрес на шине I2C
    I2C*      _I2C_Ptr;                                       // Указатель на I2C

    int8_t    i2c_write(uint8_t addr, uint8_t *data, uint16_t count);
    int8_t    i2c_read(uint8_t addr, uint8_t *data, uint16_t count);
    static    uint8_t crc8(const uint8_t *data, uint8_t len);

    inline void setError(const char *error);
    int16_t   readFeatureSetInt();
    int16_t   initSGP();
    int8_t    SoftReset(void);

    const static int ERROR_BUF_LENGTH = 255;
    const static int CMD_LENGTH = 2;

    const static int SGP30_DATA_LENGTH = 6;
    const static int SGPC3_DATA_LENGTH = 3;
    const static int SGP_I2C_ADDR = 0x58;
    const static int SGP_MEASURE_DELAY = 50;

    const static int SGPC3_INTERMEASURE_DELAY = 2000;
    const static int SGP30_INTERMEASURE_DELAY = 1000;

    uint32_t SGP_INTERMEASURE_DELAY = SGP30_INTERMEASURE_DELAY;
    int SGP_DATA_LENGTH = SGP30_DATA_LENGTH;

    uint16_t mTVOC = 0;
    uint16_t mECO2 = 0;
    uint16_t rawEthanol = 0;
    uint16_t rawH2 = 0;

    int mProductType = 0;
    int mFeatureSetVersion = 0;

    unsigned long mSGPMeasurementTimestamp = 0;

    bool      mInitialized = false;
    char      mErrorBuf[ERROR_BUF_LENGTH];
    uint8_t   mDataBuf[SGP30_DATA_LENGTH];
};

#endif /* SENSIRION_ESS */
