/*
 * basesettings.c
 *
 *  Created on: 18 авг. 2020 г.
 *      Author: Dmitry Dmitry Donskih
 */

#include "basesettings.h"

struct baseSettings g_systemSettings={ .Secured={ .MACaddress={0x00,0x80,0xE1,0x00,0x00,0x00},
		.SerialNumber={'1','2','3','4','5','6','7','8'},
		.type=1}, .Networking={ .IPaddress={192,168,0,71}, .NetworkMask={255,255,255,0},
				.Gateway={192,168,0,1}}};

