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
 *  File FmConverter.h: Firmware format converter.
 *
 *===========================================================================*/

#ifndef _FMCONVERTER_H
#define _FMCONVERTER_H

#include "bastypes.h"

//Firmware file formats IDs
//Intel hex format
#define FFMT_IHEX				1
//Binary format
#define FFMT_BINARY				2

//Firmware file format names
//Intel hex format
#define FFMTNM_IHEX				"ihex"
//Binary format
#define FFMTNM_BINARY			"binary"

/** */
class FmConverter
{	
private:

	/** Upper Segment Base Address (USBA), bits [19..4] of SBA */
	u32 usbAddr;

	/** Current Upper Linear Base Address (ULBA), bits [31..16] of LBA */
	u32 ulbAddr;

	/** EIP register value */
	u32 regEIP;

	/** CS register value */
	u16 regCS;

	/** IP register value */
	u16 regIP;

	/** Convert hexadecimal digits to byte */
	u32 HexStrToByte(const char *str);

	/* Convert line at intel hex format to binary array */
	int IHexStringToBin(const char *str, u8 *dest, u32 baseAddr, u32 maxSize);

public:
	/** */
	FmConverter();

	/** Get EIP register value */
	u32 GetEIP();

	/** Get CS register value */
	u16 GetCS();

	/** Get IP register value */
	u16 GetIP();

	/* Convert intel hex file to binary data */
	int IHexToBin(const char *fmFile, u8 *dest, u32 baseAddr, u32 maxSize);	

	/* Read binary file to buffer */
	int BinToBin(const char *fmFile, u8 *dest, u32 maxSize);	

    /** Get file format ID by its name */
    static int GetFormatByName(const char *name);
};

#endif /* !_FMCONVERTER_H */

/*===========================================================================
 * End of file FmConverter.h
 *===========================================================================*/
