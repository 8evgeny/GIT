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
 *  File special.cpp: Convert firmware file to C-array.
 *
 *===========================================================================*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "FmConverter.h"
#include "common.h"
#include "mcus.h"

/** C-file header template string */
#define CFILE_HEADER_TEMPLATE \
"/*===========================================================================\n"\
" *\n"\
" *  MCU firmware loader\n"\
" *\n"\
" *  Marusenkov S.A., \"ostfriesland@mail.ru\", 2016\n"\
" *\n"\
" * THIS SOURCE CODE IS PROVIDED AS IS WITH NO WARRANTIES OF ANY KIND\n"\
" * INCLUDING THE WARRANTIES OF DESIGN, MERCHANTIBILITY AND FITNESS FOR\n"\
" * A PARTICULAR PURPOSE, OR ARISING FROM A COURSE OF DEALING, USAGE OR\n"\
" * TRADE PRACTICE.\n"\
" *\n"\
" *---------------------------------------------------------------------------\n"\
" *\n"\
" *  File %s: This file is automatically generated.\n"\
" *  Do not change anything at this file.\n"\
" *\n"\
" *===========================================================================*/\n\n"


/** C-file footer template string */
#define CFILE_FOOTER_TEMPLATE \
"/*===========================================================================\n"\
" * End of file %s\n"\
" *===========================================================================*/\n"

#define strend(str)						&str[strlen(str)]
static const char *fnameReplacedChars = " -`!@#$%^&*()+{}[]?<>,.:;'\"";


/** Convert executable code from specified format to C-array */
int FmFileToCArray(const char *mcuModel, const char *inFile, const char *format)
{
	int errCode;
	FmConverter convert;
	FILE *hOutFile;
	const MCUParams *mcu;
	char *destCArray = 0;
	u8 *ldCodeBuf = 0;
	u32 ldStartAddr;
	u32 ldCodeSize;
	u32 i;
	char outFile[MAX_FILENAME_LENGTH];
	char fname[MAX_FILENAME_LENGTH];
	char varname[MAX_FILENAME_LENGTH];

	//Get MCU parameters
	mcu = GetMCUByName(mcuModel);

	if (!mcu)
		return EINVMCU;

	//Generate output filename
	strncpy(outFile, inFile, MAX_FILENAME_LENGTH - 1);
	outFile[MAX_FILENAME_LENGTH - 1] = 0;

	if (strlen(outFile) < MAX_FILENAME_LENGTH - 6)
	{
		//Add extension
		sprintf(strend(outFile), "%s",".c");

		//Variable name
        GetFileName(inFile, varname, 0);
		i = 0;
		while (i < strlen(varname))
		{
			if (strchr(fnameReplacedChars, varname[i]) != 0)
				varname[i] = '_';

			i++;
		}
	}
	else
		return EINVPARAM;

	//Allocate buffer for loader binary codes
	ldCodeBuf = new u8[mcu->ramSize];

	if (!ldCodeBuf)
		return EMALLOC;

	//Convert file with specified format
    switch(FmConverter::GetFormatByName(format))
	{
		case FFMT_IHEX:
			errCode = convert.IHexToBin(inFile, ldCodeBuf, mcu->ramBaseAddr, mcu->ramSize);
			break;

		case FFMT_BINARY:
			errCode = convert.BinToBin(inFile, ldCodeBuf, mcu->ramSize);
			break;

		default:
			return EINVFORMAT;
	}


	if (errCode != ESUCCESS)
	{
		if (ldCodeBuf)
			delete[] ldCodeBuf;
		return errCode;
	}

	//Define start address (first none 0xFF byte)
	ldStartAddr = 0;
	while(ldStartAddr < mcu->ramSize && ldCodeBuf[ldStartAddr] == 0xFF)
		ldStartAddr++;

	//Define code size (first none 0xFF byte)
	ldCodeSize = (int)mcu->ramSize - 1;
	while (ldCodeSize > ldStartAddr && ldCodeBuf[ldCodeSize] == 0xFF)
		ldCodeSize--;

	//Align loader code by 8 bytes
	ldCodeSize = (8 + ldCodeSize - ldStartAddr) & 0xFFFFFFF8;

	//Allocate memory
	destCArray = new char[ldCodeSize * 6 + 4096];

	if (!destCArray)
	{
		if (ldCodeBuf)
			delete[] ldCodeBuf;
		return EMALLOC;
	}

    GetFileName(outFile, fname, 0);

	destCArray[0] = 0;
	sprintf(strend(destCArray), CFILE_HEADER_TEMPLATE, fname);
	sprintf(strend(destCArray), "#include \"bastypes.h\"\n\n");
	sprintf(strend(destCArray), "#define %s_start \t0x%08X\n", varname, ldStartAddr);
	sprintf(strend(destCArray), "#define %s_size  \t%u\n\n", varname, ldCodeSize);
	sprintf(strend(destCArray), "const u8 %s_code[] =\n{", varname);

	i = 0;
	while(i < ldCodeSize)
	{
		if (!(i & 0x07))
			sprintf(strend(destCArray), "\n\t");

		sprintf(strend(destCArray), "0x%02X", ldCodeBuf[ldStartAddr + i]);

		if ((i + 1) < ldCodeSize)
			sprintf(strend(destCArray), ", ");

		i++;
	}

	sprintf(strend(destCArray), "\n};\n\n");
	sprintf(strend(destCArray), CFILE_FOOTER_TEMPLATE, fname);

	//Save C content to file
	if ((hOutFile = fopen(outFile, "wb")) != NULL)
	{
		fwrite(destCArray, 1, strlen(destCArray), hOutFile);
		fclose(hOutFile);
	}
	else
		errCode = EFILEOPEN;

	if (destCArray)
		delete[] destCArray;

	if (ldCodeBuf)
		delete[] ldCodeBuf;

	return errCode;
}

/*===========================================================================
 * End of file special.cpp
 *===========================================================================*/
