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

//config codec clock/pll and dividers

static const struct Aic3254Configs ConfigurePll[] {
    //PLL_clkin = MCLK, codec_clkin = PLL_CLK,
    {TLV320AIC3254_REG_PAGE_SELECT, 0x0},
    //Page 0 / Register 4: Clock Setting Register 1, Multiplexers - 0x00 / 0x04 (P0_R4)
    //11: PLL Clock is CODEC_CLKIN
    {TLV320AIC3254_REG_CLK_SRC_MUX, 0x3},
    //001: R = 1
    //001: P = 1
    //1: PLL is powered up
    {TLV320AIC3254_REG_PLL_P_R, 0x91},
    //J = 8
    {TLV320AIC3254_REG_PLL_J, 0x8},
    //Page 0 / Register 7: Clock Setting Register 4, PLL D Values (MSB) - 0x00 / 0x07 (P0_R7)
    //00 0000 0000 0000: D = 0000
    {TLV320AIC3254_REG_PLL_D_MSB, 0x0},
    //Page 0 / Register 8: Clock Setting Register 5, PLL D Values (LSB) - 0x00 / 0x08 (P0_R8)
    //00 0000 0000 0000: D = 0000
    {TLV320AIC3254_REG_PLL_D_LSB, 0x0},
    //NDAC = 2, MDAC = 8, dividers powered on
    {TLV320AIC3254_REG_NDAC, 0x81},
    {TLV320AIC3254_REG_MDAC, 0x84},
    //DOSR = 128
    {TLV320AIC3254_REG_DOSR_MSB, 0x00},
    {TLV320AIC3254_REG_DOSR_LSB, 0x80},
    //NADC = 2, MADC = 8, dividers powered on
    {TLV320AIC3254_REG_NADC, 0x81},
    {TLV320AIC3254_REG_MADC, 0x84},
    //AOSR = 128
    //1000 0000: AOSR = 128 (Use with PRB_R1 to PRB_R6, ADC Filter Type A) !!!
    {TLV320AIC3254_REG_AOSR, 0x80},
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


// Configure Power Supplies

static const struct Aic3254Configs ConfigurePower[] {
    //Configure Power Supplies
    { TLV320AIC3254_REG_PAGE_SELECT, 0x1 },
    //Page 1 / Register 2: LDO Control Register - 0x01 / 0x02 (P1_R2)
    //1: AVDD LDO Powered up
    //1: Analog Blocks Disabled
    //# Power up AVDD LDO
    { TLV320AIC3254_REG_LDO_CR, 0x9 },

    //Page 1 / Register 1: Power Configuration Register - 0x01 / 0x01 (P1_R1)
    //1: Disabled weak connection of AVDD with DVDD
    { TLV320AIC3254_REG_PWR_CFG, 0x8 },

    //# Enable Master Analog Power Control
    //# Power up AVDD LDO
    { TLV320AIC3254_REG_LDO_CR, 0x1 },

    //# Set full chip common mode to 0.9V
    //# HP output CM = 1.65V
    //# HP driver supply = LDOin voltage
    //# Line output CM = 1.65V
    //# Line output supply = LDOin voltage
    { TLV320AIC3254_REG_CM_CR, 0x3B },
    //#
    //# Select ADC PTM_R4
    { TLV320AIC3254_REG_ADC_PTUNE_CFG, 0x00},
    //w 30 3d 00
    //#
    //# Select DAC PTM_P3/4
    { TLV320AIC3254_REG_PLAYBACK_CFG1, 0x00 },
    { TLV320AIC3254_REG_PLAYBACK_CFG2, 0x00 },


    //# Set the input power-up time to 3.1ms (for ADC)
    { TLV320AIC3254_REG_AINPUT_CHARGE_CFG, 0x32 },
    //# Set the REF charging time to 40ms
    { TLV320AIC3254_REG_REF_PWRUP_CFG, 0x01 },
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

//IN1_L -> MIC1_IN+
//IN1_R -> MIC1_IN-

//IN2_L -> MIC2_IN+
//IN2_R -> MIC2_IN-

static const struct Aic3254Configs ConfigureADC[] {
    //Select Page 1
    {TLV320AIC3254_REG_PAGE_SELECT, 0x01},

//Добавил_CUBE_03_05_2022
    //Route IN2L to LEFT_P with 10K input impedance
    {TLV320AIC3254_REG_LMICPGA_PMUX, 0x10},
    //Route IN2R to LEFT_M with 10K input impedance
    {TLV320AIC3254_REG_LMICPGA_NMUX, 0x10},
    //Route IN1R to RIGHT_P with 10K input impedance
    {TLV320AIC3254_REG_RMICPGA_PMUX, 0x40},
    //Route IN1L to RIGHT_M with 10K input impedance
    {TLV320AIC3254_REG_RMICPGA_NMUX, 0x10},


//IN1 не используется
//IN2L -> LEFT_P
//IN2R -> LEFT_N
//IN3L -> RIGHT_P
//IN3R -> RIGHT_N

    //Route IN2L to LEFT_P with 10K input impedance
    {TLV320AIC3254_REG_LMICPGA_PMUX, 0x10},
    //Route IN2R to LEFT_M with 10K input impedance
    {TLV320AIC3254_REG_LMICPGA_NMUX, 0x10},
    //Route IN3R to RIGHT_P with 10K input impedance
    {TLV320AIC3254_REG_RMICPGA_PMUX, 0x04},
    //Route IN3L to RIGHT_M with 10K input impedance
    {TLV320AIC3254_REG_RMICPGA_NMUX, 0x04},

    //Unmute Left MICPGA, Gain selection of 6dB to
    //make channel gain 0dB, since 20K input
    //impedance is usedsingle ended
    //101 1111: Volume Control = 47.5dB
    {TLV320AIC3254_REG_LMICPGA_VOL, 0x5f},

    //Unmute Right MICPGA, Gain selection of 6dB to
    //make channel gain 0dB, since 20K input
    //impedance is usedsingle ended
    {TLV320AIC3254_REG_RMICPGA_VOL, 0x5f},

    //micbias
    //10: MICBIAS = 2.075V(CM = 0.75V) or MICBIAS = 2.5V(CM = 0.9V)
    //1: MICBIAS powered up
    //1: MICBIAS voltage is generated from LDOIN

//    {TLV320AIC3254_REG_MICBIAS_CFG, 0x58},
//Отключил micbias
    {TLV320AIC3254_REG_MICBIAS_CFG, 0x18},


    //Select Page 0
    {TLV320AIC3254_REG_PAGE_SELECT, 0x00},
    //Power up LADC/RADC
    {TLV320AIC3254_REG_ADC_SETUP, 0xc0},
    //Unmute LADC/RADC
//    {TLV320AIC3254_REG_ADC_FGAIN, 0x00},

    //mute LADC/RADC
    {TLV320AIC3254_REG_ADC_FGAIN, 0x88},
};

//static const struct Aic3254Configs ConfigureADC[] {//as MUROM
//    {TLV320AIC3254_REG_PAGE_SELECT, 0x01},
//    {TLV320AIC3254_REG_LMICPGA_PMUX, 0x40},
//    {TLV320AIC3254_REG_LMICPGA_NMUX, 0x40},
//    {TLV320AIC3254_REG_LMICPGA_VOL, 0x2F},
//    {TLV320AIC3254_REG_PAGE_SELECT, 0x00},
//    {TLV320AIC3254_REG_ADC_SETUP, 0xC0},
//    {TLV320AIC3254_REG_ADC_FGAIN, 0x00},
//};

//LOL
//LOR
static const struct Aic3254Configs ConfigureDAC[] {
    {TLV320AIC3254_REG_PAGE_SELECT, 0x01},
    //Page 1 / Register 10: Common Mode Control Register - 0x01 / 0x0A (P1_R10)
    //: When Page-1, Reg-10, D1 = 1, then LDOIN input range is 1.8V to 3.6V
    //Output Common Mode for LOL and LOR is 1.65V and output is powered by LDOIN

    {TLV320AIC3254_REG_CM_CR, 0xb},
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
    {TLV320AIC3254_REG_LDAC_DVOL_CR, 0x10},
    {TLV320AIC3254_REG_RDAC_DVOL_CR, 0x10},
    // Powerup LDAC/RDAC
    {TLV320AIC3254_REG_DAC_SETUP1, 0xd4},
    // UnmuteLDAC/RDAC
    {TLV320AIC3254_REG_DAC_SETUP2, 0x00},
};

//static const struct Aic3254Configs ConfigureDAC[] {//as MUROM
//    {TLV320AIC3254_REG_PAGE_SELECT, 0x01},
//    {TLV320AIC3254_REG_HPL_SEL, 0x00},
//    {TLV320AIC3254_REG_HPR_SEL, 0x00},
//    {TLV320AIC3254_REG_LOL_SEL, 0x08},
//    {TLV320AIC3254_REG_LOR_SEL, 0x08},
//    {TLV320AIC3254_REG_OUTDRV_PWR_CR, 0x0C},
//    {TLV320AIC3254_REG_LOL_GAIN, 0x00},
//    {TLV320AIC3254_REG_LOR_GAIN, 0x00},
//    {TLV320AIC3254_REG_PAGE_SELECT, 0x00},
//    {TLV320AIC3254_REG_DAC_SETUP1, 0xD4},
//    {TLV320AIC3254_REG_DAC_SETUP2, 0x00},
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

void TLV320AIC3254::codecInit()
{
osDelay(5000);
    uint32_t i = 0;
    /* Initialize the Control interface of the Audio Codec */

    for (i = 0;
            i < sizeof(SoftReset) / sizeof(struct Aic3254Configs);
            i++)
    {
        I2C::getInstance()->writeRegister(I2C_ADDRESS, SoftReset[i].regOffset, SoftReset[i].regVal);
    }
term("codec SoftReset ")
    for (i = 0;
            i < sizeof(ConfigurePll) / sizeof(struct Aic3254Configs);
            i++)
    {
        I2C::getInstance()->writeRegister(I2C_ADDRESS, ConfigurePll[i].regOffset, ConfigurePll[i].regVal);
    }
term("codec ConfigurePll ")
    for (i = 0;
            i < sizeof(ConfigurePower) / sizeof(struct Aic3254Configs);
            i++)
    {
        I2C::getInstance()->writeRegister(I2C_ADDRESS, ConfigurePower[i].regOffset, ConfigurePower[i].regVal);
    }
term("codec ConfigurePower ")
    for (i = 0;
            i < sizeof(ConfigureDAC) / sizeof(struct Aic3254Configs);
            i++)
    {
        I2C::getInstance()->writeRegister(I2C_ADDRESS, ConfigureDAC[i].regOffset, ConfigureDAC[i].regVal);
    }
term("codec ConfigureDAC ")
    for (i = 0;
            i < sizeof(ConfigureADC) / sizeof(struct Aic3254Configs);
            i++)
    {
        I2C::getInstance()->writeRegister(I2C_ADDRESS, ConfigureADC[i].regOffset, ConfigureADC[i].regVal);
    }
term("codec ConfigureADC ")
    for (i = 0;
            i < sizeof(ConfigureFilter) / sizeof(struct Aic3254Configs);
            i++)
    {
        I2C::getInstance()->writeRegister(I2C_ADDRESS, ConfigureADC[i].regOffset, ConfigureFilter[i].regVal);
    }
term("codec ConfigureADC ")

//Вывод всех регистров codec определенной страницы
//char buf[20];
//I2C::getInstance()->writeRegister(I2C_ADDRESS, 0x00, 0x01); //Тут установить номер страницы
//for (uint8_t i = 0; i < 255; ++i){
//uint8_t read = I2C::getInstance()->readRegister(I2C_ADDRESS,i);
//sprintf(buf,"reg %d Value = %d ",i,read);
//term(buf)
//}

}
