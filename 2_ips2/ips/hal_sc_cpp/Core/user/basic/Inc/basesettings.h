/*
 * basesettings.h
 *
 *  Created on: 18 авг. 2020 г.
 *      Author: Dmitry Dmitry Donskih
 */

#ifndef USER_BASIC_INC_BASESETTINGS_H_
#define USER_BASIC_INC_BASESETTINGS_H_

struct baseSettings{
	struct {
		unsigned char MACaddress[6];
		unsigned char SerialNumber[8];
		unsigned short type;
	}Secured;
	struct {
		unsigned char IPaddress[4];
		unsigned char NetworkMask[4];
		unsigned char Gateway[4];
	}Networking;
};


extern struct baseSettings g_systemSettings;

#endif /* USER_BASIC_INC_BASESETTINGS_H_ */
