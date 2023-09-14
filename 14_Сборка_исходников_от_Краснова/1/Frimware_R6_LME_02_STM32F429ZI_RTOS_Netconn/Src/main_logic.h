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

void makeLoginPassword(char *login_password, char *login, char* password);
void initEepVars(void);
void ThreadMainLogic(void const * argument);
void defaultTaskMainLoop(void);
void doForceStartPolling(void); // опрос вывода принудительного старта измерения (в прерывании 1 мс)

#endif /* MAIN_LOGIC_H */
