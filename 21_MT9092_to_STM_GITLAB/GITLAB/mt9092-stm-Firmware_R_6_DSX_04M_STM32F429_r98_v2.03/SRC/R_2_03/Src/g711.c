
#include "main.h"
#include "g711.h"
#include "mt9092_registers.h"
#include "g711_data.h"
#include "math.h"
#include "audio_in_out.h"
//#include "stm32f4xx_hal_flash_ex.h"

/*  g711.c
 *  u-law, A-law and linear PCM conversions.
 */
extern TIM_HandleTypeDef htim3;
extern TIM_HandleTypeDef htim4;


uint32_t ADC_DMA_buffer[ADCOVERSMPL];

FLASH_OBProgramInitTypeDef	Fuses;

//===========================================================
int alaw2linear(unsigned char a_val);
unsigned char linear2alaw(int pcm_val);
int32_t RectWinFilter(int32_t NewSample);
int16_t LowPassFilter(int16_t NewSample);


//========================================================================================
void ReadProtect(void) // защита от считывания
{

	HAL_FLASHEx_OBGetConfig(&Fuses);

	if (Fuses.RDPLevel != OB_RDP_LEVEL_1)
	{
		HAL_FLASH_Unlock(); // разрешаем запись
		HAL_FLASH_OB_Unlock(); // разрешаем запись

		Fuses.RDPLevel = OB_RDP_LEVEL_1;
		HAL_FLASHEx_OBProgram(&Fuses);

		HAL_FLASH_OB_Launch();
		HAL_FLASH_OB_Lock(); // запрещаем запись
		HAL_FLASH_Lock(); // запрещаем запись
	}
}


//========================================================================================
void AudioInit(void)
{
//	float Sum = 0;
//	uint16_t i;
//
//	DMA2_Stream0->NDTR = ADCOVERSMPL; // количество пересылок
//	DMA2_Stream0->PAR = (uint32_t)&ADC->CDR;
//	DMA2_Stream0->M0AR = (uint32_t)ADC_DMA_buffer;
//
//	DMA2->LIFCR |= DMA_LIFCR_CTCIF0; // сбрасываем флаг прерывания (обязательно перед включением)
//	DMA2_Stream0->CR |= DMA_SxCR_EN; // запуск DMA
//
//	ADC2->CR2 |= ADC_CR2_ADON;
//	ADC1->CR2 |= ADC_CR2_ADON;
//	ADC->CCR |= ADC_CCR_DDS;
//
//	ADC1->CR2 |= ADC_CR2_SWSTART;
//
//
//	DAC->CR |= DAC_CR_DMAEN1; // разрешение запросов ДМА
//	DAC->CR |= DAC_CR_EN1; // включаем канал
//	DAC->CR |= DAC_CR_EN2; // включаем канал
//
//	TIM4->ARR = (SYSTEMCORECLOCK/2/FRQOVERSMPL)-1;
//	TIM4->CR1 |= TIM_CR1_CEN;
}


//========================================================================================
int16_t AudioSamleFromADC(void) {
  return linear2alaw(AudioInPop());
}


//========================================================================================
void AudioSamleToDAC(int16_t s)
{
	static uint16_t idx = 0;
	static int32_t swap = 0;
	int32_t smpl_new = 0;
	int32_t Cc;
//  static int32_t i;
//
//  i += 8;
//  if (i >= 96) i = 0;

	if (mt9092_reg[DSP_CONTROL_REGISTER] == 0x21) // аналоговый канал
	{
		smpl_new = alaw2linear(s);

		swap = 0; // интервал сначала
	}
	else if (mt9092_reg[DSP_CONTROL_REGISTER] == 0x61) // генерим свое
	{
		Cc = mt9092_reg[TONE_RINGER_WARBLE_RATE_REGISTER]; // чередование
//Cc = 16;
		if (Cc != 0) // запрещенная комбинация
    {
      if (mt9092_reg[TONE_COEFFICIENT_REGISTER_2] != 0) {
        if (swap >= 0) // первая частота
        {
          swap--;
          if (swap == 0) swap = -Cc * SMPLRATE / 500;
          Cc = mt9092_reg[TONE_COEFFICIENT_REGISTER_1];
  //Cc = 8;
        }
        else // вторая частота
        {
          swap++;
          if (swap == 0) swap = Cc * SMPLRATE / 500;
          Cc = mt9092_reg[TONE_COEFFICIENT_REGISTER_2];
  //Cc = 16;
        }
      } else {
        Cc = mt9092_reg[TONE_COEFFICIENT_REGISTER_1];
      }
      if (Cc != 0)
      {
        idx += N_BUF / Cc; // DDS-генератор
        idx &= 0x3FF;
        smpl_new = SineRef[idx]/2;
      }
		}
	}

  AudioOutPush(smpl_new);
}


/**************************************************************
Filter type: Low Pass
Filter model: Rectangular Window
Sampling Frequency: 48 KHz
Cut Frequency: 4.0000 KHz
***************************************************************/
#define Ntap 30
int32_t RectWinFilter(int32_t NewSample)
{
    float FIRCoef[Ntap] = { 
        0.01858024325044370400,
        0.01009879726552912300,
        -0.00264701685528990910,
        -0.01696862799963352900,
        -0.02931160107680494400,
        -0.03593843239551080400,
        -0.03371546426813806100,
        -0.02083981178134741000,
        0.00265380933137534690,
        0.03472700459217326100,
        0.07156560917400466000,
        0.10817343840901698000,
        0.13922584608045724000,
        0.16001973322557106000,
        0.16733275633181097000,
        0.16001973322557106000,
        0.13922584608045724000,
        0.10817343840901698000,
        0.07156560917400466000,
        0.03472700459217326100,
        0.00265380933137534690,
        -0.02083981178134741000,
        -0.03371546426813806100,
        -0.03593843239551080400,
        -0.02931160107680494400,
        -0.01696862799963352900,
        -0.00264701685528990910,
        0.01009879726552912300,
        0.01858024325044370400,
        0.02142018976449556300
    };

    static int16_t x[Ntap]; //input samples
	float y = 0;            //output sample
    uint8_t n;

    //shift the old samples
    for(n=Ntap-1; n>0; n--)
       x[n] = x[n-1];

    //Calculate the new output
    x[0] = NewSample;
    for(n=0; n<Ntap; n++)
        y += FIRCoef[n] * x[n];
	
    return y;
}



/**************************************************************
Filter model: Bessel
Filter order: 3
Sampling Frequency: 144 KHz
Cut Frequency: 8.000000 KHz
оверсемплинг х18
***************************************************************/
int16_t LowPassFilter(int16_t NewSample)
{
  const float ACoef[] = {
        0.00362974049593185480,
        0.01088922148779556400,
        0.01088922148779556400,
        0.00362974049593185480
    };
    const float BCoef[] = {
        1.00000000000000000000,
        -2.20750784731757890000,
        1.66151573835036510000,
        -0.42498498118089445000
    };

    static float y[4]; //output samples
    static float x[4]; //input samples

	x[3] = x[2]; x[2] = x[1]; x[1] = x[0];
	y[3] = y[2]; y[2] = y[1]; y[1] = y[0]; //shift the old samples

	x[0] = NewSample;
	y[0] = ACoef[0] * x[0];
	y[0] += (ACoef[1] * x[1])-(BCoef[1] * y[1]);
	y[0] += (ACoef[2] * x[2])-(BCoef[2] * y[2]);
	y[0] += (ACoef[3] * x[3])-(BCoef[3] * y[3]);
    
    return y[0];
}


//========================================================================================
static int
search(
    int        val,
    const short      *table,
    int        size)
{
    int        i;

    for (i = 0; i < size; i++) {
        if (val <= *table++)
            return (i);
    }
    return (size);
}


/*
 * linear2alaw() - Convert a 16-bit linear PCM value to 8-bit A-law
 *
 * linear2alaw() accepts an 16-bit integer and encodes it as A-law data.
 *
 *        Linear Input Code    Compressed Code
 *    ------------------------    ---------------
 *    0000000wxyza            000wxyz
 *    0000001wxyza            001wxyz
 *    000001wxyzab            010wxyz
 *    00001wxyzabc            011wxyz
 *    0001wxyzabcd            100wxyz
 *    001wxyzabcde            101wxyz
 *    01wxyzabcdef            110wxyz
 *    1wxyzabcdefg            111wxyz
 *
 * For further information see John C. Bellamy's Digital Telephony, 1982,
 * John Wiley & Sons, pps 98-111 and 472-476.
 */
unsigned char
linear2alaw(
    int        pcm_val)    /* 2's complement (16-bit range) */
{
    int        mask;
    int        seg;
    unsigned char    aval;

    if (pcm_val >= 0) {
        mask = 0xD5;        /* sign (7th) bit = 1 */
    } else {
        mask = 0x55;        /* sign bit = 0 */
        pcm_val = -pcm_val - 8;
    }

    /* Convert the scaled magnitude to segment number. */
    seg = search(pcm_val, seg_end, 8);

    /* Combine the sign, segment, and quantization bits. */

    if (seg >= 8)        /* out of range, return maximum value. */
        return (0x7F ^ mask);
    else {
        aval = seg << SEG_SHIFT;
        if (seg < 2)
            aval |= (pcm_val >> 4) & QUANT_MASK;
        else
            aval |= (pcm_val >> (seg + 3)) & QUANT_MASK;
        return (aval ^ mask);
    }
}

/*
 * alaw2linear() - Convert an A-law value to 16-bit linear PCM
 *
 */
int
alaw2linear(
    unsigned char    a_val)
{
    int        t;
    int        seg;

    a_val ^= 0x55;

    t = (a_val & QUANT_MASK) << 4;
    seg = ((unsigned)a_val & SEG_MASK) >> SEG_SHIFT;
    switch (seg) {
    case 0:
        t += 8;
        break;
    case 1:
        t += 0x108;
        break;
    default:
        t += 0x108;
        t <<= seg - 1;
    }
    return ((a_val & SIGN_BIT) ? t : -t);
}

#define    BIAS        (0x84)        /* Bias for linear code. */

/*
 * linear2ulaw() - Convert a linear PCM value to u-law
 *
 * In order to simplify the encoding process, the original linear magnitude
 * is biased by adding 33 which shifts the encoding range from (0 - 8158) to
 * (33 - 8191). The result can be seen in the following encoding table:
 *
 *    Biased Linear Input Code    Compressed Code
 *    ------------------------    ---------------
 *    00000001wxyza            000wxyz
 *    0000001wxyzab            001wxyz
 *    000001wxyzabc            010wxyz
 *    00001wxyzabcd            011wxyz
 *    0001wxyzabcde            100wxyz
 *    001wxyzabcdef            101wxyz
 *    01wxyzabcdefg            110wxyz
 *    1wxyzabcdefgh            111wxyz
 *
 * Each biased linear code has a leading 1 which identifies the segment
 * number. The value of the segment number is equal to 7 minus the number
 * of leading 0's. The quantization interval is directly available as the
 * four bits wxyz.  * The trailing bits (a - h) are ignored.
 *
 * Ordinarily the complement of the resulting code word is used for
 * transmission, and so the code word is complemented before it is returned.
 *
 * For further information see John C. Bellamy's Digital Telephony, 1982,
 * John Wiley & Sons, pps 98-111 and 472-476.
 */
unsigned char
linear2ulaw(
    int        pcm_val)    /* 2's complement (16-bit range) */
{
    int        mask;
    int        seg;
    unsigned char    uval;

    /* Get the sign and the magnitude of the value. */
    if (pcm_val < 0) {
        pcm_val = BIAS - pcm_val;
        mask = 0x7F;
    } else {
        pcm_val += BIAS;
        mask = 0xFF;
    }

    /* Convert the scaled magnitude to segment number. */
    seg = search(pcm_val, seg_end, 8);

    /*
     * Combine the sign, segment, quantization bits;
     * and complement the code word.
     */
    if (seg >= 8)        /* out of range, return maximum value. */
        return (0x7F ^ mask);
    else {
        uval = (seg << 4) | ((pcm_val >> (seg + 3)) & 0xF);
        return (uval ^ mask);
    }

}

/*
 * ulaw2linear() - Convert a u-law value to 16-bit linear PCM
 *
 * First, a biased linear code is derived from the code word. An unbiased
 * output can then be obtained by subtracting 33 from the biased code.
 *
 * Note that this function expects to be passed the complement of the
 * original code word. This is in keeping with ISDN conventions.
 */
int
ulaw2linear(
    unsigned char    u_val)
{
    int        t;

    /* Complement to obtain normal u-law value. */
    u_val = ~u_val;

    /*
     * Extract and bias the quantization bits. Then
     * shift up by the segment number and subtract out the bias.
     */
    t = ((u_val & QUANT_MASK) << 3) + BIAS;
    t <<= ((unsigned)u_val & SEG_MASK) >> SEG_SHIFT;

    return ((u_val & SIGN_BIT) ? (BIAS - t) : (t - BIAS));
}

/* A-law to u-law conversion */
unsigned char
alaw2ulaw(
    unsigned char    aval)
{
    aval &= 0xff;
    return ((aval & 0x80) ? (0xFF ^ _a2u[aval ^ 0xD5]) :
        (0x7F ^ _a2u[aval ^ 0x55]));
}

/* u-law to A-law conversion */
unsigned char
ulaw2alaw(
    unsigned char    uval)
{
    uval &= 0xff;
    return ((uval & 0x80) ? (0xD5 ^ (_u2a[0xFF ^ uval] - 1)) :
        (0x55 ^ (_u2a[0x7F ^ uval] - 1)));
}

