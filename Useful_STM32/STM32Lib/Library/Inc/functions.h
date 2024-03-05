/*
 * functions.h
 *
 *  Created on: 26 мар. 2021 г.
 *      Author: User
 */

#ifndef FUNCTIONS_H_
#define FUNCTIONS_H_


#include "stdint.h"
#include <stdbool.h>
#include <stdio.h>

#define constrain(amt,low,high) ((amt)<(low)?(low):((amt)>(high)?(high):(amt)))
//
long map(long x, long in_min, long in_max, long out_min, long out_max);
float mapFloat(float x, float in_min, float in_max, float out_min, float out_max);

size_t min(uint64_t a, uint64_t b);
size_t strlength(const char *s);

#endif /* FUNCTIONS_H_ */
