/*
 * TLC59116F.h
 *
 *  Created on: 27 июл. 2020 г.
 *      Author: Dmitry Dmitry Donskih
 */

#ifndef USER_INC_TLC59116F_H_
#define USER_INC_TLC59116F_H_
#include <cstdint>
#include <basicDO.h>

class TLC59116F public: basicDOUnit
{
public:
	TLC59116F(uint8_t addr, uint8_t capacity);
	void setDO(uint8_t output);
	void resetDO(uint8_t output);
	void toggleDO(uint8_t output);
	uint8_t readDO(uint8_t output);
};



#endif /* USER_INC_TLC59116F_H_ */
