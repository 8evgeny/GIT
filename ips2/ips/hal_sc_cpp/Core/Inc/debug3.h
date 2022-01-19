/*
 * debug3.h
 *
 *  Created on: 7 июн. 2020 г.
 *      Author: Donskikh Dmitry
 */

/*
MIT License

Copyright (c) 2020 Dmitry Donskikh

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/



#ifndef INC_DEBUG3_H_
#define INC_DEBUG3_H_
#include "ips_globals.hpp"
#include "stm32h7xx_hal.h"



#define DEBUG_IPS(x) (ipsSystem::debug)->printfl("DEBUG: %s @%i \r\n",x,HAL_GetTick());


#endif /* INC_DEBUG3_H_ */
