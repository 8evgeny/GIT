/**
  ******************************************************************************
  * @file    tone_generator.c
  * @author  dmib@@ETS
  * @brief   This file contains tone generator implementaion
  ******************************************************************************
  */
#include <math.h>
#include "tone_generator.h"

// array frequency coeff and star impulse, first 8 - DTMF, next - can add
// k1 = a = 256*2*cos(2pi * freq/Fd)
// z1= -sin(2pi * freq/Fd)
// k2 = z1*32767 * 10^((-3 - x)/20)
const int16_t gen_freq_coef[FREQ_GEN_MAX][2] =
  {
{437, -12073},//256 * 2.0 * cos(2.0*3.1415*697.0/8000.0),    // DTMF fa1		
{421, -13189},//256 * 2.0 * cos(2.0*3.1415*770.0/8000.0),    // DTMF fa2		
{402, -14389},//256 * 2.0 * cos(2.0*3.1415*852.0/8000.0),    // DTMF fa3		
{378, -15625},//256 * 2.0 * cos(2.0*3.1415*941.0/8000.0),    // DTMF fa4		
{298, -18863},//256 * 2.0 * cos(2.0*3.1415*1209.0/8000.0),   // DTMF fb1
{255, -20113},//256 * 2.0 * cos(2.0*3.1415*1336.0/8000.0),   // DTMF fb2
{204, -21268},//256 * 2.0 * cos(2.0*3.1415*1477.0/8000.0),   // DTMF fb3
{146, -22240},//256 * 2.0 * cos(2.0*3.1415*1633.0/8000.0),   // DTMF fb4
{484,  -7600},//256 * 2.0 * cos(2.0*3.1415*425.0/8000.0),    // 425 Hz  -3 dB   (0dB analog out)
//{484,  -1909},//256 * 2.0 * cos(2.0*3.1415*425.0/8000.0),    // 425 Hz		-15 dB  (-12dB analog out)
//{456, -10531},//256 * 2.0 * cos(2.0*3.1415*600.0/8000.0),    // 600 Hz		-3 dB   (0dB analog out)
//{362, -16402},//256 * 2.0 * cos(2.0*3.1415*1000.0/8000.0),   // 1000 Hz		-3 dB   (0dB analog out)
//{301, -18767},//256 * 2.0 * cos(2.0*3.1415*1200.0/8000.0),   // 1200 Hz		-3 dB   (0dB analog out)
//{-40, -23125} //256 * 2.0 * cos(2.0*3.1415*2100.0/8000.0)    // 2100 Hz		-3 dB   (0dB analog out)
{456, -1872},//256 * 2.0 * cos(2.0*3.1415*600.0/8000.0),    // 600 Hz		-15 dB   (-12dB analog out)
{362, -2916},//256 * 2.0 * cos(2.0*3.1415*1000.0/8000.0),   // 1000 Hz		-15 dB   (-12dB analog out)
{301, -3337},//256 * 2.0 * cos(2.0*3.1415*1200.0/8000.0),   // 1200 Hz		-15 dB   (-12dB analog out)
{-40, -4112} //256 * 2.0 * cos(2.0*3.1415*2100.0/8000.0)    // 2100 Hz		-15 dB   (-12dB analog out)
  };

//--------------sinetone generator section-----------------------------------
void Freq_GenInit(uint32_t k, int16_t amp, int16_t *X){
  X[1]=0;
  X[2]=amp;
}

int16_t Freq_GenSample(uint32_t k, int16_t *X){
  X[0] = ((k*X[1])>>8) - X[2];
  X[2] = X[1];
  X[1] = X[0];
  return X[0];
}

//--------------DTMF generator section-----------------------------------
//fb/fa  1209 1336 1477 1633
// 697     1   2    3    A
// 770     4   5    6    B
// 852     7   8    9    C
// 941     *   0    #    D
const uint8_t DTMFFreq[32]={
		3,5, 	// 0
		0,4, 	// 1
		0,5, 	// 2
		0,6, 	// 3
		1,4, 	// 4
		1,5, 	// 5
		1,6, 	// 6
		2,4, 	// 7
		2,5, 	// 8
		2,6, 	// 9
		0,7, 	// A
		1,7, 	// B
		2,7, 	// C
		3,7, 	// D
		3,4, 	// *
		3,6 	// #
	  };

// инициализация генерации DTMF номера
void DTMF_GenInit(DTMF_Gen_State *st, uint16_t amp, uint8_t DTMFNum){
  DTMFNum <<= 1;
  st->D_K1 = gen_freq_coef[DTMFFreq[DTMFNum]][0];
  st->D_K2 = gen_freq_coef[DTMFFreq[DTMFNum+1]][0];
  st->D_X0[1]=0;
  st->D_X0[2]=gen_freq_coef[DTMFFreq[DTMFNum]][1];
  st->D_X1[1]=0;
  st->D_X1[2]=gen_freq_coef[DTMFFreq[DTMFNum]][1];
}

// генерация одного отсчета DTMF-сигнала
int16_t DTMF_GenSample(DTMF_Gen_State *st){
  st->D_X0[0] = ((st->D_K1*st->D_X0[1])>>8) - st->D_X0[2];
  st->D_X0[2] = st->D_X0[1];
  st->D_X0[1] = st->D_X0[0];

  st->D_X1[0] = ((st->D_K2*st->D_X1[1])>>8) - st->D_X1[2];
  st->D_X1[2] = st->D_X1[1];
  st->D_X1[1] = st->D_X1[0];

  return (st->D_X0[0]+st->D_X1[0])>>1;
}

