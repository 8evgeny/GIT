#include "tlv320aic3254.h"
#include "cmsis_os.h"
#include "main.h"
#include "../Debug/debug.h"
#include "rs232.h"
#include "aic3254_reg.h"

struct Aic3254Configs {
    uint8_t regOffset;
    uint8_t regVal;
};

//soft reset
static const struct Aic3254Configs SoftReset[] {
    {TLV320AIC3254_REG_PAGE_SELECT, 0x0},
    {TLV320AIC3254_REG_RESET, 0x1},
};

static const struct Aic3254Configs Mute[] {
    {TLV320AIC3254_REG_PAGE_SELECT, 0x0},
    {TLV320AIC3254_REG_RDAC_DVOL_CR, 0x81},
};

//config codec clock/pll and dividers

static const struct Aic3254Configs ConfigurePll[] {
    //PLL_clkin = MCLK, codec_clkin = PLL_CLK,
    {TLV320AIC3254_REG_PAGE_SELECT, 0x0}, //==

    //Page 0 / Register 4: Clock Setting Register 1, Multiplexers - 0x00 / 0x04 (P0_R4)
    //11: PLL Clock is CODEC_CLKIN
    {TLV320AIC3254_REG_CLK_SRC_MUX, 0x3}, //----------------------- 0x00

    //Page 0 / Register 5:
    //001: R = 1
    //001: P = 1
    //1: PLL is powered up
    {TLV320AIC3254_REG_PLL_P_R, 0x91}, //---------------------------------

    //Page 0 / Register 6:    J = 8
    {TLV320AIC3254_REG_PLL_J, 0x8}, //-----------------------------------

    //Page 0 / Register 7: Clock Setting Register 4, PLL D Values (MSB) - 0x00 / 0x07 (P0_R7)
    //00 0000 0000 0000: D = 0000
    {TLV320AIC3254_REG_PLL_D_MSB, 0x0}, //--------------------------------

    //Page 0 / Register 8: Clock Setting Register 5, PLL D Values (LSB) - 0x00 / 0x08 (P0_R8)
    //00 0000 0000 0000: D = 0000
    {TLV320AIC3254_REG_PLL_D_LSB, 0x0},//------------------------------------

    //NDAC = 2, MDAC = 8, dividers powered on
    {TLV320AIC3254_REG_NDAC, 0x81}, //==
    {TLV320AIC3254_REG_MDAC, 0x84}, //----------------------------- 0x81

    //DOSR = 128
    {TLV320AIC3254_REG_DOSR_MSB, 0x00}, //--------------------------------0x02
    {TLV320AIC3254_REG_DOSR_LSB, 0x80}, //--------------------------------0x00
    //NADC = 2, MADC = 8, dividers powered on
    {TLV320AIC3254_REG_NADC, 0x81}, //==
    {TLV320AIC3254_REG_MADC, 0x84}, //==
    //AOSR = 128
    //1000 0000: AOSR = 128 (Use with PRB_R1 to PRB_R6, ADC Filter Type A) !!!
    {TLV320AIC3254_REG_AOSR, 0x80}, //==
};

//static const struct Aic3254Configs ConfigurePll[] { //as MUROM
//    {TLV320AIC3254_REG_PAGE_SELECT, 0x00},
//    {TLV320AIC3254_REG_CLK_SRC_MUX, 0x00},
//    {TLV320AIC3254_REG_NDAC, 0x81},
//    {TLV320AIC3254_REG_MDAC, 0x81},
//    {TLV320AIC3254_REG_DOSR_MSB, 0x02},
//    {TLV320AIC3254_REG_DOSR_LSB, 0x00},
//    {TLV320AIC3254_REG_NADC, 0x81},
//    {TLV320AIC3254_REG_MADC, 0x84},
//    {TLV320AIC3254_REG_AOSR, 0x80},
//};


static const struct Aic3254Configs ConfigurePower[] {
    //Configure Power Supplies
    { TLV320AIC3254_REG_PAGE_SELECT, 0x1 }, //==
    //Page 1 / Register 2: LDO Control Register - 0x01 / 0x02 (P1_R2)
    //1: AVDD LDO Powered up
    //1: Analog Blocks Disabled
    //# Power up AVDD LDO
    { TLV320AIC3254_REG_LDO_CR, 0x9 }, //==

    //Page 1 / Register 1: Power Configuration Register - 0x01 / 0x01 (P1_R1)
    //1: Disabled weak connection of AVDD with DVDD
    { TLV320AIC3254_REG_PWR_CFG, 0x8 }, //==

    //# Enable Master Analog Power Control
    //# Power up AVDD LDO
    { TLV320AIC3254_REG_LDO_CR, 0x1 }, //==

    //# Set full chip common mode to 0.9V
    //# HP output CM = 1.65V
    //# HP driver supply = LDOin voltage
    //# Line output CM = 1.65V
    //# Line output supply = LDOin voltage
    { TLV320AIC3254_REG_CM_CR, 0x3B }, //---------------------- 0x43
    //#
    //# Select ADC PTM_R4
    { TLV320AIC3254_REG_ADC_PTUNE_CFG, 0x00}, //==
    //w 30 3d 00
    //#
    //# Select DAC PTM_P3/4
    { TLV320AIC3254_REG_PLAYBACK_CFG1, 0x00 }, //==
    { TLV320AIC3254_REG_PLAYBACK_CFG2, 0x00 }, //==


    //# Set the input power-up time to 3.1ms (for ADC)
    { TLV320AIC3254_REG_AINPUT_CHARGE_CFG, 0x32 }, //==
    //# Set the REF charging time to 40ms
    { TLV320AIC3254_REG_REF_PWRUP_CFG, 0x01 }, //==
    // У Мурома еще { TLV320AIC3254_REG_HPHONE_STARTUP_CR, 0x40 }
    { TLV320AIC3254_REG_HPHONE_STARTUP_CR, 0x40 }
};

//static const struct Aic3254Configs ConfigurePower[] {//as MUROM
//    { TLV320AIC3254_REG_PAGE_SELECT, 0x01 },
//    { TLV320AIC3254_REG_LDO_CR, 0x09 },
//    { TLV320AIC3254_REG_PWR_CFG, 0x08 },
//    { TLV320AIC3254_REG_LDO_CR, 0x01 },
//    { TLV320AIC3254_REG_CM_CR, 0x43 },
//    { TLV320AIC3254_REG_PLAYBACK_CFG1, 0x00},
//    { TLV320AIC3254_REG_PLAYBACK_CFG2, 0x00},
//    { TLV320AIC3254_REG_ADC_PTUNE_CFG, 0x00},
//    { TLV320AIC3254_REG_AINPUT_CHARGE_CFG, 0x32 },
//    { TLV320AIC3254_REG_REF_PWRUP_CFG, 0x01 },
//    { TLV320AIC3254_REG_HPHONE_STARTUP_CR, 0x40 },
//};

static const struct Aic3254Configs ConfigureADC[] {
    //Select Page 1
    {TLV320AIC3254_REG_PAGE_SELECT, 0x01},

    //IN2L is routed to Left MICPGA with 10k resistance
    {TLV320AIC3254_REG_LMICPGA_PMUX, 0x10}, //52(0x34) регистр -------------------- 0x40

    //IN2R is routed to Left MICPGA with 10k resistance
    {TLV320AIC3254_REG_LMICPGA_NMUX, 0x10}, //54(0x36) регистр --------------------- 0x40

    //IN2R is routed to Right MICPGA with 10k resistance
    //IN2L is routed to Right MICPGA with 10k resistance
    {TLV320AIC3254_REG_RMICPGA_PMUX, 0x11}, //55(0x37) регистр

    //IN1 на земле IN3L IN3R реализую позднее


    //101 1111: Volume Control = 47.5dB
    //101 0000: Volume Control = 40.0dB
    //000 0000: Volume Control = 0.0dB
//    {TLV320AIC3254_REG_LMICPGA_VOL, 0x00}, //59(0x3B) регистр

    {TLV320AIC3254_REG_RMICPGA_VOL, 0x5f}, //60(0x3C) регистр

    //Select Page 0
    {TLV320AIC3254_REG_PAGE_SELECT, 0x00},
    //Power up LADC/RADC
    {TLV320AIC3254_REG_ADC_SETUP, 0xC0}, //88(0x58) регистр //==
    //Unmute LADC/RADC
    {TLV320AIC3254_REG_ADC_FGAIN, 0x00}, //82(0x52) регистр

};

//static const struct Aic3254Configs ConfigureADC[] {//as MUROM
//    {TLV320AIC3254_REG_PAGE_SELECT, 0x01},
//    {TLV320AIC3254_REG_LMICPGA_PMUX, 0x40}, //IN1L is routed to Left MICPGA with 10k resistance ???
//    {TLV320AIC3254_REG_LMICPGA_NMUX, 0x40}, //CM is routed to Left MICPGA via CM1L with 10k resistance
//    {TLV320AIC3254_REG_LMICPGA_VOL, 0x2F},  // Volume
//    {TLV320AIC3254_REG_PAGE_SELECT, 0x00},
//    {TLV320AIC3254_REG_ADC_SETUP, 0xC0},    // Left Channel ADC is powered up  Right Channel ADC is powered up
//    {TLV320AIC3254_REG_ADC_FGAIN, 0x00},    // Left ADC Channel Un-muted Left ADC Channel Fine Gain = 0dB Right ADC Channel Un-muted Right ADC Channel Fine Gain = 0dB
//};

static const struct Aic3254Configs ConfigureDAC[] {
    {TLV320AIC3254_REG_PAGE_SELECT, 0x01},
    //Page 1 / Register 10: Common Mode Control Register - 0x01 / 0x0A (P1_R10)
    //: When Page-1, Reg-10, D1 = 1, then LDOIN input range is 1.8V to 3.6V
    //Output Common Mode for LOL and LOR is 1.65V and output is powered by LDOIN

//    {TLV320AIC3254_REG_CM_CR, 0xb},

    // Route LDAC/RDAC to LOL/LOR
    {TLV320AIC3254_REG_LOL_SEL, 0x08},
    {TLV320AIC3254_REG_LOR_SEL, 0x08},
    // Powerup HPL/HPR and LOL/LOR drivers
    {TLV320AIC3254_REG_OUTDRV_PWR_CR, 0x0C},
    // Unmute LOL/LOR driver, 0dB Gain
    {TLV320AIC3254_REG_LOL_GAIN, 0x00},//1c
    {TLV320AIC3254_REG_LOR_GAIN, 0x00},
    // Select Page 0
    {TLV320AIC3254_REG_PAGE_SELECT, 0x00},
    // DAC => 0dB
//    {TLV320AIC3254_REG_LDAC_DVOL_CR, 0x10},
    {TLV320AIC3254_REG_RDAC_DVOL_CR, 0xC0},   // ГРОМКОСТЬ -32dB
    // Powerup LDAC/RDAC
    {TLV320AIC3254_REG_DAC_SETUP1, 0xd4},
    // UnmuteLDAC/RDAC
    {TLV320AIC3254_REG_DAC_SETUP2, 0x00},
};

//static const struct Aic3254Configs ConfigureDAC[] {//as MUROM
//    {TLV320AIC3254_REG_PAGE_SELECT, 0x01},
//    {TLV320AIC3254_REG_HPL_SEL, 0x00}, //not routed to HPL
//    {TLV320AIC3254_REG_HPR_SEL, 0x00}, //not routed to HPR
//    {TLV320AIC3254_REG_LOL_SEL, 0x08}, //Left Channel DAC reconstruction filter output is routed to LOL
//    {TLV320AIC3254_REG_LOR_SEL, 0x08}, //Right Channel DAC reconstruction filter output is routed to LOR
//    {TLV320AIC3254_REG_OUTDRV_PWR_CR, 0x0C}, //LOL is powered up LOR is powered up
//    {TLV320AIC3254_REG_LOL_GAIN, 0x00}, //LOL driver is not muted   gain is 0dB
//    {TLV320AIC3254_REG_LOR_GAIN, 0x00}, //LOR driver is not muted   gain is 0dB
//    {TLV320AIC3254_REG_PAGE_SELECT, 0x00},
//    {TLV320AIC3254_REG_DAC_SETUP1, 0xD4}, //Left DAC Channel Powered Up Right DAC Channel Powered Up Left DAC data Left Channel Audio Interface Data Right DAC data Right Channel Audio Interface Data
//    {TLV320AIC3254_REG_DAC_SETUP2, 0x00}, //not muted
//};


static const struct Aic3254Configs ConfigureFilter[] {
    {TLV320AIC3254_REG_PAGE_SELECT, 8},
    {TLV320AIC3254_REG_PAGE_SELECT, 0x5},

    {TLV320AIC3254_REG_PAGE_SELECT, 44},
    {TLV320AIC3254_REG_PAGE_SELECT, 0x5},
};

TLV320AIC3254::TLV320AIC3254()
{
    pI2C = I2C::getInstance();
    pSAI = SAI::getInstance();
}

void TLV320AIC3254::softReset()
{
    for (uint32_t i = 0; i < sizeof(SoftReset) / sizeof(struct Aic3254Configs); i++)
    {
        I2C::getInstance()->writeRegister(I2C_ADDRESS, SoftReset[i].regOffset, SoftReset[i].regVal);
    }
}
void TLV320AIC3254::configPll()
{
    for (uint32_t i = 0; i < sizeof(ConfigurePll) / sizeof(struct Aic3254Configs); i++)
    {
        I2C::getInstance()->writeRegister(I2C_ADDRESS, ConfigurePll[i].regOffset, ConfigurePll[i].regVal);
    }
}
void TLV320AIC3254::configPower()
{
    for (uint32_t i = 0; i < sizeof(ConfigurePower) / sizeof(struct Aic3254Configs); i++)
    {
        I2C::getInstance()->writeRegister(I2C_ADDRESS, ConfigurePower[i].regOffset, ConfigurePower[i].regVal);
    }
}
void TLV320AIC3254::configDAC()
{
    for (uint32_t i = 0; i < sizeof(ConfigureDAC) / sizeof(struct Aic3254Configs); i++)
    {
        I2C::getInstance()->writeRegister(I2C_ADDRESS, ConfigureDAC[i].regOffset, ConfigureDAC[i].regVal);
    }
}
void TLV320AIC3254::configADC()
{
    for (uint32_t i = 0; i < sizeof(ConfigureADC) / sizeof(struct Aic3254Configs); i++)
    {
        I2C::getInstance()->writeRegister(I2C_ADDRESS, ConfigureADC[i].regOffset, ConfigureADC[i].regVal);
    }
}
void TLV320AIC3254::configFilter()
{
    for (uint32_t i = 0; i < sizeof(ConfigureFilter) / sizeof(struct Aic3254Configs); i++)
    {
        I2C::getInstance()->writeRegister(I2C_ADDRESS, ConfigureFilter[i].regOffset, ConfigureFilter[i].regVal);
    }
}
void TLV320AIC3254::mute()
{
    for (uint32_t i = 0; i < sizeof(Mute) / sizeof(struct Aic3254Configs); i++)
    {
        I2C::getInstance()->writeRegister(I2C_ADDRESS, Mute[i].regOffset, Mute[i].regVal);
    }
}

void TLV320AIC3254::codecInit()
{
osDelay(5000);

    softReset();
    configPll();
    configPower();
    configADC();
    configDAC();
    configFilter();

//Вывод всех регистров codec определенной страницы
//char buf[20];
//I2C::getInstance()->writeRegister(I2C_ADDRESS, 0x00, 0x01); //Тут установить номер страницы
//for (uint8_t i = 0; i < 255; ++i){
//uint8_t read = I2C::getInstance()->readRegister(I2C_ADDRESS,i);
//sprintf(buf,"reg %d Value = %d ",i,read);
//term(buf)
//}

}
