/**
  ******************************************************************************
  * @file    SELC_sound_process.c
  * @author  dmib@@ETS
  * @brief   This file contains sound echo loop cancelation processing implementation
  ******************************************************************************
  */
#include "string.h"
#include "CLI_io.h"
#include "driver_AIC.h"
#include "SELC_sound_process.h"

#ifdef USE_SELC

// FADE enable level
// max SAI_DMA_BUFFER_SIZE * i16 * i16 / 256 = SAI_DMA_BUFFER_SIZE* 2^15*2^15 / 256 =
//                = 160*32768*128 = 671088640
//#define EC_GR_ENABLE_ENERGY_LEVEL     500000
#define EC_GR_ENABLE_ENERGY_LEVEL     80000
// number of SAI_DMA_BUFFER buffers to fade
#define EC_FADE_MIC_CNT	10
#define EC_FADE_GR_EN_CNT	1
#define EC_FADE_GR_DIS_CNT	10
// fade level sig_in_f(x) = sig_in(x)/(2^EC_FADE_PARAM), where EC_FADE_PARAM=2..14
#define EC_FADE_PARAM	4

#define EC_TONE_DELAY_CNT	10

#define MIC_LEVELS	8

/// MIC_threshold levels, each level +-1.5 dB, depends on VOL_MIC_STEP
const uint32_t ec_MIC_threshold_param[(MIC_LEVELS*2) + 1] = {
                                // 0
                                400000,
                                // -1 -2 -3 -4 -5 -6 -7 -8
                                283200,200505,141957,100506,71158,50380,35669,25253,
                                //  1  2  3  4  5  6  7  8
                                564971,797982,1127094,1591940,2248504,3175853,4485668,6335690
};

uint32_t ec_enable;
uint32_t ec_GR_max_energy, ec_MIC_max_energy;
uint32_t ec_MIC_disable;
uint32_t ec_GR_disable;
uint32_t ec_MIC_time;
uint32_t ec_GR_time;

uint32_t ec_GR_threshold;
uint32_t ec_MIC_threshold;
uint32_t ec_fade_level;
uint32_t ec_MIC_disable_time;
uint32_t ec_GR_enable_time;
uint32_t ec_GR_disable_time;

void selc_init()
{
    ec_enable = 1;
    ec_MIC_disable = 1;
    ec_GR_disable = 1;
    ec_MIC_time = 0;
    ec_GR_time = 0;

    ec_GR_threshold = EC_GR_ENABLE_ENERGY_LEVEL;
    ec_MIC_threshold = ec_MIC_threshold_param[0]; // 0 dB
    ec_fade_level = EC_FADE_PARAM;
    ec_MIC_disable_time = EC_FADE_MIC_CNT;
    ec_GR_enable_time = EC_FADE_GR_EN_CNT;
    ec_GR_disable_time = EC_FADE_GR_DIS_CNT;
}

void selc_adjust_threshold_level(int8_t mic_vol)
{
    int8_t mic_lev;

    mic_lev = mic_vol/VOL_MIC_STEP;

    if (mic_lev<0) {
        mic_lev = -mic_lev;
        if (mic_lev<=MIC_LEVELS) {
            ec_MIC_threshold = ec_MIC_threshold_param[mic_lev];
        }
    }
    else if (mic_lev>0)
    {
        if (mic_lev<=MIC_LEVELS) {
            ec_MIC_threshold = ec_MIC_threshold_param[MIC_LEVELS + mic_lev];
        }
    } else
        ec_MIC_threshold = ec_MIC_threshold_param[0];
//	CLI_print("MIC Threshold : %i\r\n",ec_MIC_threshold);		// debug!!!
}

uint32_t selc_sound_energy_factor(int16_t *sndbuf, uint16_t smpnum)
{
    uint16_t i;
    uint32_t sum;
    sum = 0;

    for (i=0;i<smpnum;i++)
        sum = sum + ((sndbuf[i] * sndbuf[i]) >> 8);

    return sum;
}

void selc_sound_dampen(int16_t *sndbuf, uint16_t smpnum)
{
    uint16_t i;
    for (i=0;i<smpnum;i++)
     sndbuf[i] = sndbuf[i] >> ec_fade_level;
}

void selc_set_enable(uint8_t enable)
{
    ec_enable = enable;
    if (enable) {
        ec_MIC_time = 0;
        ec_GR_time = 0;
        ec_GR_disable = 0;
        ec_MIC_disable = 1;
    } else {
        ec_GR_disable = 0;
        ec_MIC_disable = 0;
    }
}

void selc_process_sound(int16_t *srcbuf, int16_t *dstbuf, uint16_t smpnum)
{
    if (!ec_enable) return;

    uint32_t eng_GR, eng_MIC;

    eng_GR = selc_sound_energy_factor(dstbuf,smpnum);
    eng_MIC = selc_sound_energy_factor(srcbuf,smpnum);

    if (eng_GR > ec_GR_max_energy) ec_GR_max_energy = eng_GR;
    if (eng_MIC > ec_MIC_max_energy) ec_MIC_max_energy = eng_MIC;

    if (ec_GR_disable) {
        if (eng_MIC > ec_MIC_threshold) {
//			if (ec_MIC_disable) CLI_print("MIC EN\r\n");		// debug!!!
            ec_MIC_disable = 0;
            ec_MIC_time = 0;
        } else {
            if ((!ec_MIC_disable) && (!ec_MIC_time)) ec_MIC_time = ec_MIC_disable_time;
        }
        if (ec_MIC_time) {
            ec_MIC_time--;
            if (!ec_MIC_time) {
                ec_MIC_disable = 1;
//			    CLI_print("MIC DIS\r\n");		// debug!!!
            }
        }
    }

    if (eng_GR > ec_GR_threshold) {
        ec_MIC_disable = 1;
        if (ec_GR_disable&(!ec_GR_time)) {
            ec_GR_time = ec_GR_enable_time;
            ec_MIC_time = 0;
        } else
        if ((!ec_GR_disable)&ec_GR_time) {
            ec_GR_time = 0;
        }
    } else {
        if ((!ec_GR_disable)&(!ec_GR_time)) {
            ec_GR_time = ec_GR_disable_time;
        } else
        if ((ec_GR_disable)&ec_GR_time) {
                ec_GR_time = 0;
        }
    }

    if (ec_GR_time) {
        ec_GR_time--;
        if (!ec_GR_time) {
            ec_GR_disable ^= 1;
//	    	CLI_print("GR DIS:%i\r\n", ec_GR_disable); 		// debug!!!
        }
    }

    if (ec_MIC_disable) {
        selc_sound_dampen(srcbuf,smpnum);
    };

    if (ec_GR_disable) {
        selc_sound_dampen(dstbuf,smpnum);
    };
}

void selc_set_tone_gen()
{
    ec_MIC_disable = 1;
    ec_GR_disable = 0;
    ec_GR_time = EC_TONE_DELAY_CNT;
}

#endif
