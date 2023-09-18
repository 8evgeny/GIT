/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   main_logic.h
 * Author: roman.ganaza
 *
 * Created on February 22, 2023, 12:56 PM
 */

#ifndef MAIN_LOGIC_H
#define MAIN_LOGIC_H

#include <stdint.h>

void initMainLogic(void);
void doMainLogic(void const * argument);
void writeOutputs(uint8_t* out_data);
void controlLines(uint8_t* out_data);


#endif /* MAIN_LOGIC_H */
