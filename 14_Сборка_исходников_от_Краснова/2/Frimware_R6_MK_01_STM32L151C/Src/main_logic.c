/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   main_logic.c
 * Author: roman.ganaza
 * 
 * Created on February 22, 2023, 12:56 PM
 */

#include <string.h>
#include "main_logic.h"
#include "eeprom_i2c.h"
#include "global_vars.h"
#include "uart.h"
#include "a_main.h"

void initMainLogic(void) {
  uartInit();
}

void doMainLogic(void const * argument) {

}

void writeOutputs(uint8_t* out_data) {
  HAL_GPIO_WritePin(E1_GPIO_Port, E1_Pin, (GPIO_PinState) out_data[0]);
  HAL_GPIO_WritePin(E2_GPIO_Port, E2_Pin, (GPIO_PinState) out_data[1]);
  HAL_GPIO_WritePin(E3_GPIO_Port, E3_Pin, (GPIO_PinState) out_data[2]);
  HAL_GPIO_WritePin(E4_GPIO_Port, E4_Pin, (GPIO_PinState) out_data[3]);
  HAL_GPIO_WritePin(E5_GPIO_Port, E5_Pin, (GPIO_PinState) out_data[4]);
  HAL_GPIO_WritePin(E6_GPIO_Port, E6_Pin, (GPIO_PinState) out_data[5]);
}

void controlLines(uint8_t* out_data) {
  int32_t i;
  
  for(i = 0; i < NUM_LINES; i++) {
    line_en[i] = out_data[i];
    if (line_en[i]) setInputReleToAmp(i); else setInputReleToMeasCircuit(i);
  }
}
