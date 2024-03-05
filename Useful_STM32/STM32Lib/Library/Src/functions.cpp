/*
 * fubctions.cpp
 *
 *  Created on: 26 мар. 2021 г.
 *      Author: User
 */

#include "functions.h"

long map(long x, long in_min, long in_max, long out_min, long out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

float mapFloat(float x, float in_min, float in_max, float out_min, float out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

size_t min(uint64_t a, uint64_t b) {return (((a) < (b)) ? (a) : (b));}

size_t strlength(const char *s)
{
  uint16_t i = 0, t = 0;
  while (s[i] != '\0')
  {
    if(s[i] >= 0xC0)
    {
      i++;
    }
    i++;
    t++;
  }
  return t;
}
