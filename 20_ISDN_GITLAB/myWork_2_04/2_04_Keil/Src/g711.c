
#include "main.h"
#include "g711.h"
#include "g711_data.h"
#include "math.h"
#include "audio_in_out.h"

/*  g711.c
 *  u-law, A-law and linear PCM conversions.
 */
tDacMode DAC_mode = DM_SOUND; // выводим звук или генерируем рингтон (или гудки)
//Tone duration (warble frequency) = 500 / COEF
//where: Frequency is in Hz, and 0≤COEF <256
//Highest frequency possible: 500 Hz
//Lowest frequency possible: 2.0 Hz
uint8_t warble_rate = 250; // частота чередования частот генератора рингтона, см. регистр MT9092 "TONE_RINGER_WARBLE_RATE_REGISTER"
uint8_t tone_coef_1 = 16; // первая частота генератора рингтона, см. регистр MT9092 "TONE_COEFFICIENT_REGISTER_1"
uint8_t tone_coef_2 = 8; // первая частота генератора рингтона, см. регистр MT9092 "TONE_COEFFICIENT_REGISTER_2"
float att = 1; // ослабление/усиление звука

//===========================================================
int alaw2linear(unsigned char a_val);
unsigned char linear2alaw(int pcm_val);

//========================================================================================
void AudioInit(void) 
{
  att = 1;
}


//========================================================================================
int16_t AudioSamleFromADC(void)
{
//	uint32_t tmp1 = 0, tmp2 = 0;
//	float smpl = 0;
//	uint8_t Cc;
//
//	for (Cc=0; Cc<ADCOVERSMPL; Cc++)
//	{
//		tmp1 += ADC_DMA_buffer[Cc] & 0xFFFF; // неинвертирующий вход
//		tmp2 += ADC_DMA_buffer[Cc] >> 16;// инвертирующий вход
////		smpl += (int32_t)tmp1-(int32_t)tmp2; // альт дифф.
//	}
//	smpl = (int32_t)tmp1-(int32_t)tmp2; // дифф.
//	smpl = (smpl*32767)/(4095*ADCOVERSMPL);
//
///*
//	smpl = (ADC_DMA_buffer[0] >> 16)* 65535 / 4095; // для теста без оверсемплинга
////	smpl = (ADC->CDR >> 16)* 65535 / 4095;
////	smpl -= (ADC->CDR & 0xFFFF)* 65535 / 4095;
//	smpl -= 32768;
////	smpl /= 2;
//*/
//	smpl = linear2alaw(smpl);
//
//	return smpl;
  
  return linear2alaw(AudioInPop());
  
}


//========================================================================================
void AudioSamleToDAC(uint8_t s)
{
	static uint16_t idx = 0;
	static int32_t swap = 0;
	int32_t smpl_new = 0;
	int32_t tone_coef;
  
	if (DAC_mode == DM_SOUND) // аналоговый канал
	{
		smpl_new = alaw2linear(s) * att;

		swap = 0; // интервал сначала
	}
	else if (DAC_mode == DM_TONE) // генерим свое
	{
		if (warble_rate != 0) // запрещенная комбинация
		{
      if (tone_coef_2 != 0) {
        if (swap >= 0) // первая частота
        {
          swap--;
          if (swap == 0) swap = -(int32_t)warble_rate * SMPLRATE / 500;
          tone_coef = tone_coef_1;
        }
        else // вторая частота
        {
          swap++;
          if (swap == 0) swap = (int32_t)warble_rate * SMPLRATE / 500;
          tone_coef = tone_coef_2;
        }
      } else {
        tone_coef = tone_coef_1;
      }
      
			if (tone_coef != 0)
			{
				idx += N_BUF / tone_coef; // DDS-генератор
				idx &= 0x3FF;
//				smpl_new = SineRef[idx]/2; // синус
				smpl_new = idx < (N_BUF/2)? 32767/2 : -32767/2; // меандр
			}
		}
	}
  
  AudioOutPush(smpl_new);
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

