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
 *  File FmConverter.cpp: Firmware format converter.
 *
 *===========================================================================*/

#include "FmConverter.h"

#include "bastypes.h"
#include "common.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

FmConverter::FmConverter()
{
	regEIP = (u32)0;
	regCS  = (u16)0;
	regIP  = (u16)0;
}



/** Get EIP register value */
u32 FmConverter::GetEIP()
{
	return regEIP;
}



/** Get CS register value */
u16 FmConverter::GetCS()
{
	return regCS;
}



/** Get IP register value */
u16 FmConverter::GetIP()
{
	return regIP;
}



/** Convert hexadecimal digits to byte */
u32 FmConverter::HexStrToByte(const char *str)
{
	u32 bh, bl;

	//[0..9] => [0x30..0x39]
	//[A..F] => [0x41..0x46]
	bh = str[0] - 0x30;
	bl = str[1] - 0x30;

	//[A..F]
	if (bh > 9)
		bh -= 7;

	if (bl > 9)
		bl -= 7;

	return ((bh << 4) | bl);
}



/* Convert line at intel hex format to binary array */
int FmConverter::IHexStringToBin(const char *str, u8 *dest, u32 baseAddr, u32 maxSize)
{
	int i;	
	u32 lbAddr;
	u16 offsetAddr;
	u8 length, recType, csum;
	u8 binData[256];	

	//Intel HEX format: ":LLAAAATTDD...CC" + CRLF
	//':'  - start char
	//LL   - count of DD bytes
	//AAAA - address
	//TT   - record type:
	// 00 Data Record
	// 01 End of File Record
	// 02 Extended Segment Address Record
	// 03 Start Segment Address Record
	// 04 Extended Linear Address Record
	// 05 Start Linear Address Record
	//DD... - data bytes
	//CC - checksum: LL+AAAA+TT+DD...+CC = 0

	//Each line at HEX file must starts with ':'
	if(str[0] != ':')
		return EINVFORMAT;	

	//LL
	length = (u8)HexStrToByte(&str[1]);

	//AAAA
	offsetAddr = (HexStrToByte(&str[3]) << 8) + HexStrToByte(&str[5]);

	//TT
	recType = (u8)HexStrToByte(&str[7]);

	//Solve checksum and get binary data
	csum = length + recType + (offsetAddr >> 8) + offsetAddr;
	for(i = 0; i < length + 1; i++)
	{
		binData[i] = (u8)HexStrToByte(&str[2 * i + 9]);
		csum += binData[i];
	}	

	if(csum != 0)
		return EINVFORMAT;

	switch(recType)
	{
		//Data record
		case 0x00:
		{
			//Solve LBA
			lbAddr = ulbAddr + usbAddr + offsetAddr;

			//Check address
			if(lbAddr < baseAddr ||
			  (lbAddr + length) > (baseAddr + maxSize))
				return EINVADDR;

			lbAddr -= baseAddr;

			//Copy binary data to destination buffer
			memcpy(&dest[lbAddr], binData, length);

			break;
		}

		//Extended Segment Address Record
		case 0x02:
		{
			if(length != 2)
				return EINVFORMAT;

			usbAddr = ((HexStrToByte(&str[9] ) << 8) +
						HexStrToByte(&str[11])) << 4;
			break;
		}

		//Extended Linear Address Record
		case 0x04:
		{
			if(length != 2)
				return EINVFORMAT;

			ulbAddr = ((HexStrToByte(&str[9] ) << 8) +
						HexStrToByte(&str[11])) << 16;
			break;
		}

		//Start Segment Address Record
		case 0x03:
		{
			if(length != 4)
				return EINVFORMAT;

			regCS = (u16)((HexStrToByte(&str[9 ]) << 8) +
						  (HexStrToByte(&str[11])     ));

			regIP = (u16)((HexStrToByte(&str[13]) << 8) +
						  (HexStrToByte(&str[15])     ));
			break;
		}

		//Start Linear Address Record
		case 0x05:
		{
			if(length != 4)
				return EINVFORMAT;

			regEIP = (HexStrToByte(&str[9] ) << 24) +
					 (HexStrToByte(&str[11]) << 16) +
					 (HexStrToByte(&str[13]) << 8 ) +
					 (HexStrToByte(&str[15])      );
			break;
		}

		//End of File Record
		case 0x01:
		{
			//no actions
			break;
		}
	}

	return ESUCCESS;
}



/* Convert intel hex file to mcu firmware binary data */
int FmConverter::IHexToBin(const char *file, u8 *dest, u32 baseAddr, u32 maxSize)
{
	#define IHEXSTRING_MAXLENGTH	544

	int i, nb, errCode;
	FILE  *hFile;
	char chd;
	char ihexString[IHEXSTRING_MAXLENGTH];

	usbAddr = (u32)0;
	ulbAddr = (u32)0;
	regEIP  = (u32)0;
	regCS   = (u16)0;
	regIP   = (u16)0;

	//Fill buffer with 0xFF
	memset(dest, 0xFF, maxSize);

	//Try open file
	if (!file || (hFile = fopen(file, "rb")) == NULL)
		return EFILEOPEN;

	//Read chars
	i = 0;
	while((nb = fread(&chd, 1, 1, hFile)) == 1)
	{
		//LF: try convert string at IHEX format to binary data
		if (chd == 0x0A)
		{
			if ((errCode = IHexStringToBin(ihexString, dest,
										   baseAddr, maxSize)) != 0)
			{
				fclose(hFile);
				return errCode;
			}

			i = 0;
		}
		else if (chd != 0x0D)
		{
			//Collect string chars
			ihexString[i++] = chd;

			//Check length
			if (i == IHEXSTRING_MAXLENGTH)
				return EINVFORMAT;
		}
	}

	//Close file
	fclose(hFile);

	return ESUCCESS;

	#undef IHEXSTRING_MAXLENGTH
}



/* Read binary file to buffer */
int FmConverter::BinToBin(const char *file, u8 *dest, u32 maxSize)
{
	int rdCount, errCode = ESUCCESS;
	u8 tmp;
	FILE  *hFile;

	//Fill buffer with 0xFF
	memset(dest, 0xFF, maxSize);

    //Try open file
	if (!file || (hFile = fopen(file, "rb")) == NULL)
		return EFILEOPEN;

	//Read binary content
	rdCount = fread(dest, 1, maxSize, hFile);

	//Check result
	if (rdCount <= 0)
		errCode = EINVFORMAT;
	//maxSize < file size: error
	else if ((rdCount = fread(&tmp, 1, 1, hFile)) > 0)
		errCode = EBUFSIZE;

	//Close file
	fclose(hFile);

	return errCode;
}



/** Get file format by name */
int FmConverter::GetFormatByName(const char *name)
{
    char nameLCase[64];

	if (name == 0 || strlen(name) == 0)
		return EINVFORMAT;

    StrToLower(nameLCase, name, 64);

    if (strcmp(nameLCase, FFMTNM_IHEX) == 0)
		return FFMT_IHEX;
    else if (strcmp(nameLCase, FFMTNM_BINARY) == 0)
		return FFMT_BINARY;
	else
		return EINVFORMAT;
}

/*===========================================================================
 * End of file FmConverter.cpp
 *===========================================================================*/

