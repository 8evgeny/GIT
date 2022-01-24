/*===========================================================================
 *
 *  MCU firmware loader
 *
 *  Marusenkov S.A., "ostfriesland@mail.ru", 2016
 *
 * THIS SOURCE CODE IS PROVIDED AS IS WITH NO WARRANTIES OF ANY KIND
 * INCLUDING THE WARRANTIES OF DESIGN, MERCHANTIBILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE, OR ARISING FROM A COURSE OF DEALING, USAGE OR
 * TRADE PRACTICE.
 *
 *---------------------------------------------------------------------------
 *
 *  File mcus.c: MCU parameters.
 *
 *===========================================================================*/

#include "mcus.h"
#include "common.h"

#include <string.h>

#include "fmloader-1986be9x.hex.c"
#include "fmloader-1986be1x.hex.c"
#include "fmloader-1901bc1x.hex.c"

//#include "fmloader-1986be9x-gcc.bin.c"
//#include "fmloader-1986be1x-gcc.bin.c"
//#include "fmloader-1901bc1x-gcc.bin.c"

/** MCU Milandr 1986BE9x parameters  */
const MCUParams mcu1986BE9X =
{
	MCUNM_1986BE9X,	
	MCU_1986BE9X,
	0x08000000,
	0x20000000,
	(128 * 1024),
	(32 * 1024),
	9600,
    "1986be9xfldr",
    fmloader_1986be9x_hex_code,
    fmloader_1986be9x_hex_start,
    fmloader_1986be9x_hex_size
};

/** MCU Milandr 1986BE1x parameters  */
const MCUParams mcu1986BE1X =
{
	MCUNM_1986BE1X,	
	MCU_1986BE1X,
	0x00000000,
	0x20100000,
	(128 * 1024),
	(16 * 1024),
	9600,
    "1986be1xfldr",
    fmloader_1986be1x_hex_code,
    fmloader_1986be1x_hex_start,
    fmloader_1986be1x_hex_size
};

/** MCU Milandr 1901BC1x parameters  */
const MCUParams mcu1901BC1X =
{
	MCUNM_1901BC1X,
	MCU_1901BC1X,
	0x08000000,
	0x20000000,
	(128 * 1024),
	(32 * 1024),
	9600,
    "1901bc1xfldr",
    fmloader_1901bc1x_hex_code,
    fmloader_1901bc1x_hex_start,
    fmloader_1901bc1x_hex_size
};



const MCUParams *mcus[] =
{
	&mcu1986BE9X,
	&mcu1986BE1X,
	&mcu1901BC1X,	
	0
};



/** */
const MCUParams *GetMCUByName(const char *name)
{
	const MCUParams *m;
	int i = 0;
    char nameLCase[64];

	if (!name)
		return 0;

    StrToLower(nameLCase, name, 64);

	while((m = mcus[i++]) != 0)
        if (strcmp(m->name, nameLCase) == 0)
			return m;

	return 0;
}



/** */
const MCUParams *GetMCUByID(s32 id)
{
	const MCUParams *m;
	int i = 0;

	while((m = mcus[i++]) != 0)
		if (m->id == id)
			return m;

	return 0;
}

/*===========================================================================
 * End of file mcus.c
 *===========================================================================*/
