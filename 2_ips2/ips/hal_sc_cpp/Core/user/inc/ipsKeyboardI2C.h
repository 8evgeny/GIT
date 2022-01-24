/*
 * ipsKeyboardI2C.h
 *
 *  Created on: Jul 22, 2020
 *      Author: Dmitry Donskih
 */

#ifndef USER_IPS_KEYBOARDS_IPSKEYBOARDI2C_H_
#define USER_IPS_KEYBOARDS_IPSKEYBOARDI2C_H_

#include "main.h"
#include <testi2c.h>

#define IPC_MAX_KB_UNITS	8

extern I2C_HandleTypeDef hi2c3;

class ipsKeyboardI2C {
public:
	ipsKeyboardI2C& getInstance();
	//I2C_HandleTypeDef getHwHandle(void);

private:
	ipsKeyboardI2C(I2C_HandleTypeDef *hI2Cport=&hi2c3);
	ipsKeyboardI2C(const ipsKeyboardI2C&);
	ipsKeyboardI2C& operator=(ipsKeyboardI2C&);

	uint8_t dev_address[IPC_MAX_KB_UNITS];
	uint8_t dev_num=0;
	I2C_HandleTypeDef hkeyboard;

	void uFW_I2CPort_Init();
};

#endif /* USER_IPS_KEYBOARDS_IPSKEYBOARDI2C_H_ */
