/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   potentiometer.c
 * Author: roman.ganaza
 * 
 * Created on May 23, 2023, 4:05 PM
 */

#include "potentiometer.h"
#include "main.h"
#include "codec_TLV320AIC3100.h"
#include <stdio.h>
#include "audio_in_out.h"

#define ABS(x)         (x<0)?(-x):x

#define ADC_NUM_CHANNELS 4 // Количество каналов выборки
#define ADC_NUM_SAMPLES 128u // количество выборок одного канала, используется для усреднения

uint32_t adc1_val_buf [ADC_NUM_CHANNELS*ADC_NUM_SAMPLES]; // Переход в DMA для хранения массива значений многоканальной выборки
uint32_t adc1_aver_val[ADC_NUM_CHANNELS] = {0}; // Массив, который сохраняет среднее значение выборки для нескольких каналов
uint32_t pot_value[ADC_NUM_CHANNELS] = {0};

double Filter0(double NewSample);
double Filter1(double NewSample);
double Filter2(double NewSample);
double Filter3(double NewSample);

void potInit(void) {
  HAL_ADC_Start_DMA(&hadc1,(uint32_t*) &adc1_val_buf, (ADC_NUM_CHANNELS*ADC_NUM_SAMPLES));
}

uint32_t getPotValue(uint8_t pot) {
  return pot_value[pot];
}

void HAL_ADC_ConvCallback(uint32_t offset) {
  uint32_t i;

  for (i = 0; i < ADC_NUM_SAMPLES / 2; i++) {
    adc1_aver_val[0] = 0;
    adc1_aver_val[1] = 0;
    adc1_aver_val[2] = 0;
    adc1_aver_val[3] = 0;
  }
  for (i = 0; i < ADC_NUM_SAMPLES / 2; i++) {
    adc1_aver_val[0] += adc1_val_buf[(offset + i) * 4 + 0];
    adc1_aver_val[1] += adc1_val_buf[(offset + i) * 4 + 1];
    adc1_aver_val[2] += adc1_val_buf[(offset + i) * 4 + 2];
    adc1_aver_val[3] += adc1_val_buf[(offset + i) * 4 + 3];
  }
  /*Усреднение значений выборки каждого канала по очереди */
  for (i = 0; i < ADC_NUM_CHANNELS; i++) {
    adc1_aver_val[i] /= ADC_NUM_SAMPLES / 2;
  }
  // фильтрация
  pot_value[0] = Filter0(adc1_aver_val[0]);
  pot_value[1] = Filter1(adc1_aver_val[1]);
  pot_value[2] = Filter2(adc1_aver_val[2]);
  pot_value[3] = Filter3(adc1_aver_val[3]);
}

void HAL_ADC_ConvHalfCpltCallback(ADC_HandleTypeDef * hadc) {
  HAL_ADC_ConvCallback(0);
}

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef * hadc) {
  HAL_ADC_ConvCallback(ADC_NUM_SAMPLES / 2);
}

//////////////// Фильтр Никитоса ///////////////////////////////////

const double delta = 3; // зона нечувствительности

double Filter0(double nw)
{
	static double state = 0; // значение после фильтра
	static double prev_state = 0; // предыдущее значение после фильтра

	state = (state*(1 - 0.004)) + (nw*0.004); // фильтр Калмана
  
  if (ABS(state - prev_state) > delta) prev_state = state;

	return prev_state;
}
double Filter1(double nw)
{
	static double state = 0; // значение после фильтра
	static double prev_state = 0; // предыдущее значение после фильтра

	state = (state*(1 - 0.004)) + (nw*0.004); // фильтр Калмана
  
  if (ABS(state - prev_state) > delta) prev_state = state;

	return prev_state;
}
double Filter2(double nw)
{
	static double state = 0; // значение после фильтра
	static double prev_state = 0; // предыдущее значение после фильтра

	state = (state*(1 - 0.004)) + (nw*0.004); // фильтр Калмана
  
  if (ABS(state - prev_state) > delta) prev_state = state;

	return prev_state;
}
double Filter3(double nw)
{
	static double state = 0; // значение после фильтра
	static double prev_state = 0; // предыдущее значение после фильтра

	state = (state*(1 - 0.004)) + (nw*0.004); // фильтр Калмана
  
  if (ABS(state - prev_state) > delta) prev_state = state;

	return prev_state;
}
