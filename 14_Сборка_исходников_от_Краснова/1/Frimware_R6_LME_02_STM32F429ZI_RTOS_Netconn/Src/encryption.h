/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   encryption.h
 * Author: roman.ganaza
 *
 * Created on May 16, 2023, 10:59 AM
 */

#ifndef ENCRYPTION_H
#define ENCRYPTION_H

#include <stdint.h>

uint8_t encode(char *hash, char *str);
void decode(char *hash, char *str);

#endif /* ENCRYPTION_H */
