#include "tlv320aic3254.h"
#include "cmsis_os.h"
#include "main.h"
#include "../Debug/debug.h"

struct Aic3254Configs {
    uint8_t regOffset;
    uint8_t regVal;
};

//soft reset
static const struct Aic3254Configs SoftReset[] {
    {0x0, 0x0},
    {0x1, 0x1},
};

//config codec clock/pll and dividers
static const struct Aic3254Configs ConfigurePll[] {
    //PLL_clkin = MCLK, codec_clkin = PLL_CLK,
    {0x0, 0x0},
    //Page 0 / Register 4: Clock Setting Register 1, Multiplexers - 0x00 / 0x04 (P0_R4)
    //11: PLL Clock is CODEC_CLKIN
    {0x4, 0x3},
    //001: R = 1
    //001: P = 1
    //1: PLL is powered up
    {0x5, 0x91},
    //J = 8
    {0x6, 0x8},
    //Page 0 / Register 7: Clock Setting Register 4, PLL D Values (MSB) - 0x00 / 0x07 (P0_R7)
    //00 0000 0000 0000: D = 0000
    {0x7, 0x0},
    //Page 0 / Register 8: Clock Setting Register 5, PLL D Values (LSB) - 0x00 / 0x08 (P0_R8)
    //00 0000 0000 0000: D = 0000
    {0x8, 0x0},
    //NDAC = 2, MDAC = 8, dividers powered on
    {0xb, 0x81},
    {0xc, 0x84},
    //DOSR = 128
    {0xd, 0x00},
    {0xe, 0x80},
    //NADC = 2, MADC = 8, dividers powered on
    {0x12, 0x81},
    {0x13, 0x84},
    //AOSR = 128
    //1000 0000: AOSR = 128 (Use with PRB_R1 to PRB_R6, ADC Filter Type A) !!!
    {0x14, 0x80},
};

// Configure Power Supplies
static const struct Aic3254Configs ConfigurePower[] {
    //Configure Power Supplies
    { 0x0, 0x1 },
    //Page 1 / Register 2: LDO Control Register - 0x01 / 0x02 (P1_R2)
    //1: AVDD LDO Powered up
    //1: Analog Blocks Disabled
    //# Power up AVDD LDO
    { 0x2, 0x9 },

    //Page 1 / Register 1: Power Configuration Register - 0x01 / 0x01 (P1_R1)
    //1: Disabled weak connection of AVDD with DVDD
    { 0x1, 0x8 },

    //# Enable Master Analog Power Control
    //# Power up AVDD LDO
    { 0x2, 0x1 },

    //# Set full chip common mode to 0.9V
    //# HP output CM = 1.65V
    //# HP driver supply = LDOin voltage
    //# Line output CM = 1.65V
    //# Line output supply = LDOin voltage
    { 0x0A, 0x3B },
    //#
    //# Select ADC PTM_R4
    { 0x3d, 0x00},
    //w 30 3d 00
    //#
    //# Select DAC PTM_P3/4
    { 0x03, 0x00 },
    { 0x04, 0x00 },


    //# Set the input power-up time to 3.1ms (for ADC)
    { 0x47, 0x32 },
    //# Set the REF charging time to 40ms
    { 0x7b, 0x01 },
};

//IN1_L -> MIC1_IN+
//IN1_R -> MIC1_IN-

//IN2_L -> MIC2_IN+
//IN2_R -> MIC2_IN-

static const struct Aic3254Configs ConfigureADC[] {
    //Select Page 1
    {0x00, 0x01},
    //Route IN2L to LEFT_P with 10K input impedance
    {0x34, 0x10},
    //Route IN2R to LEFT_M with 10K input impedance
    {0x36, 0x10},
    //Route IN1R to RIGHT_P with 10K input impedance
    {0x37, 0x40},
    //Route IN1L to RIGHT_M with 10K input impedance
    {0x39, 0x10},
    //Unmute Left MICPGA, Gain selection of 6dB to
    //make channel gain 0dB, since 20K input
    //impedance is usedsingle ended
    //101 1111: Volume Control = 47.5dB
    //{0x3b, 0x5f},

    //Unmute Right MICPGA, Gain selection of 6dB to
    //make channel gain 0dB, since 20K input
    //impedance is usedsingle ended
    //{0x3c, 0x5f},

    //micbias
    //10: MICBIAS = 2.075V(CM = 0.75V) or MICBIAS = 2.5V(CM = 0.9V)
    //1: MICBIAS powered up
    //1: MICBIAS voltage is generated from LDOIN
    {0x33, 0x58},

    //Select Page 0
    {0x00, 0x00},
    //Power up LADC/RADC
    {0x51, 0xc0},
    //Unmute LADC/RADC
//    {0x52, 0x00},

    //mute LADC/RADC
    {0x52, 0x88},
};

//LOL
//LOR
static const struct Aic3254Configs ConfigureDAC[] {
    {0x00, 0x01},
    //Page 1 / Register 10: Common Mode Control Register - 0x01 / 0x0A (P1_R10)
    //: When Page-1, Reg-10, D1 = 1, then LDOIN input range is 1.8V to 3.6V
    //Output Common Mode for LOL and LOR is 1.65V and output is powered by LDOIN

    {0xa, 0xb},
    // Route LDAC/RDAC to LOL/LOR
    {0x0e, 0x08},
    {0x0f, 0x08},
    // Powerup HPL/HPR and LOL/LOR drivers
    {0x09, 0x0C},
    // Unmute LOL/LOR driver, 0dB Gain
    {0x12, 0x00},//1c
    {0x13, 0x00},
    // Select Page 0
    {0x00, 0x00},
    // DAC => 0dB
    {0x41, 0x10},
    {0x42, 0x10},
    // Powerup LDAC/RDAC
    {0x3f, 0xd4},
    // UnmuteLDAC/RDAC
    {0x40, 0x00},
};

static const struct Aic3254Configs ConfigureFilter[] {
    {0x00, 8},
    {0x00, 0x5},

    {0x00, 44},
    {0x00, 0x5},
};

TLV320AIC3254::TLV320AIC3254()
{
    pI2C = I2C::getInstance();
    pSAI = SAI::getInstance();
}

void TLV320AIC3254::codecInit()
{
    uint32_t i = 0;
    /* Initialize the Control interface of the Audio Codec */

    for (i = 0;
            i < sizeof(SoftReset) / sizeof(struct Aic3254Configs);
            i++) {
        I2C::getInstance()->writeRegister(I2C_ADDRESS, SoftReset[i].regOffset,
                                          SoftReset[i].regVal);
    }

    for (i = 0;
            i < sizeof(ConfigurePll) / sizeof(struct Aic3254Configs);
            i++) {
        I2C::getInstance()->writeRegister(I2C_ADDRESS, ConfigurePll[i].regOffset,
                                          ConfigurePll[i].regVal);
    }

    for (i = 0;
            i < sizeof(ConfigurePower) / sizeof(struct Aic3254Configs);
            i++) {
        I2C::getInstance()->writeRegister(I2C_ADDRESS, ConfigurePower[i].regOffset,
                                          ConfigurePower[i].regVal);
    }

    for (i = 0;
            i < sizeof(ConfigureDAC) / sizeof(struct Aic3254Configs);
            i++) {
        I2C::getInstance()->writeRegister(I2C_ADDRESS, ConfigureDAC[i].regOffset,
                                          ConfigureDAC[i].regVal);
    }

    for (i = 0;
            i < sizeof(ConfigureADC) / sizeof(struct Aic3254Configs);
            i++) {
        I2C::getInstance()->writeRegister(I2C_ADDRESS, ConfigureADC[i].regOffset,
                                          ConfigureADC[i].regVal);
    }

    for (i = 0;
            i < sizeof(ConfigureFilter) / sizeof(struct Aic3254Configs);
            i++) {
        I2C::getInstance()->writeRegister(I2C_ADDRESS, ConfigureADC[i].regOffset,
                                          ConfigureFilter[i].regVal);
    }
}
