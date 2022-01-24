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
 *  File MCUFmLoader.cpp: Firmware loader implementation.
 *
 *===========================================================================*/

#include "MCUFmLoader.h"
#include "FmConverter.h"
#include "common.h"
#include "mcus.h"

#include "fmloader/fmloader-common.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

//Initial I/O speed, must 9600
#define INITIAL_IOSPEED		9600

//Sync char
#define SYNC_CHAR			0x00

/** Check error code, close serial port and return */
#define CheckErrCode(code)  PrintErrMsgPos(COLUMN_PROGRESS, code);\
							 if (code != ESUCCESS)\
							 { serialPort.Close();\
							   FreeFmResources();\
							   return code; }

/** */
MCUFmLoader::MCUFmLoader()
{
	//Set defaults
	//Initial serial port parameters
	serialPort.setPort    (DEFAULT_PORT);
	serialPort.setSpeed   (INITIAL_IOSPEED);
	serialPort.setDataBits(8);
	serialPort.setParity  (PAR_NONE);
	serialPort.setStopBit (1);
	serialPort.setControl (CTRL_NONE);

	//Default timeouts
	readTimeout = PORT_READ_TIMEOUT;

    //Selected port
    ioport  = DEFAULT_PORT;

	//Requested i/o speed
    iospeed = DEFAULT_IOSPEED;

    //No data flow control
    ioctrl  = CTRL_NONE;

	//No MCU selected
	memset(&mcu, 0, sizeof(MCUParams));

	//No initial loader file
	ldFile[0] = 0;
	//Default loader format
	ldFormat  = DEFAULT_FORMAT;

	//No initial firmware file
	fmFile[0]  = 0;
	//Default file format
	fmFormat   = DEFAULT_FORMAT;
	//No size
	fmCodeSize = 0;
	//No buffer
	fmCode     = 0;
}



/** */
MCUFmLoader::~MCUFmLoader()
{
	FreeResources();
}



/** Free resource used by loader */
int MCUFmLoader::FreeLdResources()
{
	mcu.ldCodeSize = 0;

	if (mcu.ldCode)
	{
		delete[] mcu.ldCode;
		mcu.ldCode = 0;
	}

	return ESUCCESS;
}



/** Free resource used by firmware */
int MCUFmLoader::FreeFmResources()
{
	fmCodeSize = 0;

	if (fmCode)
	{
		delete[] fmCode;
		fmCode = 0;
	}

	return ESUCCESS;
}



/** Free resources */
int MCUFmLoader::FreeResources()
{
	FreeLdResources();

	FreeFmResources();

	return ESUCCESS;
}



/** Save filename and it format */
int MCUFmLoader::SetFile(char *dest, int *fmt,
						 const char *file, const char *fmtName)
{
    int tmp;

	//Reset to default values
    dest[0] = 0;

	if (!file || strlen(file) == 0)
		return EFILEUNDEF;
    else if ((tmp = FmConverter::GetFormatByName(fmtName)) < 0)
        return tmp;

	//Save file format
	*fmt = tmp;   

	//Save filename
	strncpy(dest, file, MAX_FILENAME_LENGTH - 1);
    dest[MAX_FILENAME_LENGTH - 1] = 0;

	return ESUCCESS;
}



/** */
int MCUFmLoader::SetFmFile(const char *fmFile, const char *fmFormatName)
{
	return SetFile(this->fmFile, &this->fmFormat, fmFile, fmFormatName);
}



/** */
int MCUFmLoader::SetLdFile(const char *ldFile, const char *ldFormatName)
{    
	return SetFile(this->ldFile, &this->ldFormat, ldFile, ldFormatName);
}



/** */
int MCUFmLoader::SetIOPort(int portNumber)
{
    this->ioport = portNumber;
	return ESUCCESS;
}



/** Select port I/O speed, bit/s */
int MCUFmLoader::SetIOSpeed(int iospeed)
{
	if (iospeed < IOSPEED_MIN ||
		iospeed > IOSPEED_MAX)
		return EINVPARAM;

	this->iospeed = iospeed;
	return ESUCCESS;
}



/** Select port I/O flow control */
int MCUFmLoader::SetIOCtrl(int ctrl)
{
    this->ioctrl = ctrl;
    return ESUCCESS;
}



/** Set read data timeout, ms */
int MCUFmLoader::SetRdTimeout(int value)
{
	this->readTimeout = value;
	return ESUCCESS;
}



/** */
int MCUFmLoader::SetMCUModel(const MCUParams *m)
{
	FreeLdResources();

	if (!m)
	{
		memset(&mcu, 0, sizeof(MCUParams));      
		return EINVMCU;
	}
	else
	{
		//Create full copy of MCUParams
		//Copy fields
        memcpy(&this->mcu, m, sizeof(MCUParams));

		if (!m->ldCodeSize)
		{
			mcu.ldCode = 0;
			mcu.ldCodeSize = 0;
			return ESUCCESS;
		}              

		//Copy loader code
        this->mcu.ldCode = new u8[m->ldCodeSize];

		if (mcu.ldCode)
		{
            memcpy((void*)mcu.ldCode, m->ldCode, m->ldCodeSize);
			return ESUCCESS;
		}
		else
		{
			memset(&mcu, 0, sizeof(MCUParams));            
			return EMALLOC;
		}
	}
}



/** */
int MCUFmLoader::SetMCUModel(int id)
{
	return SetMCUModel(GetMCUByID(id));
}



/** */
int MCUFmLoader::SetMCUModel(const char *name)
{
    //const MCUParams *m = GetMCUByName(name);
    return SetMCUModel(GetMCUByName(name));
}



/** */
int MCUFmLoader::SetMCUFMAddr(int addr)
{
	if (!mcu.id)
        return EINVMCU;

    mcu.fmBaseAddr = addr;
	return ESUCCESS;
}



/** */
int MCUFmLoader::SetMCURAMAddr(int addr)
{
	if (!mcu.id)
        return EINVMCU;

    mcu.ramBaseAddr = addr;
	return ESUCCESS;
}



/** Read data from serial port using specified buffer and timeout */
int MCUFmLoader::ReadPortData(void *data, size_t n)
{
	int r, tm = readTimeout;
	size_t rdCount = 0;
	u8 *buf = (u8*)data;

	//While specified number of bytes is not read
	//and timeout counter > 0
	while((n - rdCount) > 0 && tm > 0)
	{
		r = serialPort.Read(&buf[rdCount], n - rdCount);

		if (r > 0)
		{
			//Increment byte counter and restore timeout
			rdCount += r;
			tm = readTimeout;
		}
		else if (r == 0)
		{
			//Sleep to wait incoming data
			ThreadSleep(5);
			tm -= 10;
		}
		else if (r < 0)
			//Read error, exit
			break;
	}

	return rdCount;
}



/** Convert loader file to binary data */
int MCUFmLoader::ConvertLoaderFile()
{
	int errCode = ESUCCESS;
	FmConverter convert;
	u8 *tmpBuf;
	u8 *ldCode;

	FreeLdResources();

	tmpBuf = new u8[mcu.ramSize];

	if (!tmpBuf)
		return EMALLOC;

	//Convert file with specified format
	switch(ldFormat)
	{
		case FFMT_IHEX:
			errCode = convert.IHexToBin(ldFile, tmpBuf,
										mcu.ramBaseAddr, mcu.ramSize);
			break;

		case FFMT_BINARY:
			errCode = convert.BinToBin(ldFile, tmpBuf, mcu.ramSize);
			break;

		default:
			return EINVFORMAT;
	}

	if (errCode != ESUCCESS)
		return errCode;

	//Define start address (first none 0xFF byte)
	mcu.ldStartAddr = 0;
	while(mcu.ldStartAddr < mcu.ramSize && tmpBuf[mcu.ldStartAddr] == 0xFF)
		mcu.ldStartAddr++;

	//Define code size (first none 0xFF byte)
	mcu.ldCodeSize = (int)mcu.ramSize - 1;
	while (mcu.ldCodeSize > mcu.ldStartAddr && tmpBuf[mcu.ldCodeSize] == 0xFF)
		mcu.ldCodeSize--;

	//Align loader code by 8 bytes
	mcu.ldCodeSize = (8 + mcu.ldCodeSize - mcu.ldStartAddr) & 0xFFFFFFF8;

	ldCode = new u8[mcu.ldCodeSize];

	if (ldCode)
	{
		memcpy(ldCode, &tmpBuf[mcu.ldStartAddr], mcu.ldCodeSize);
		mcu.ldCode = ldCode;
	}
	else
		errCode = EMALLOC;

	delete[] tmpBuf;

	return errCode;
}



/** Convert loader file to binary data */
int MCUFmLoader::ConvertFirmwareFile()
{
	int errCode = ESUCCESS;
	FmConverter convert;
	u8 *tmpBuf;

	FreeFmResources();

	tmpBuf = new u8[mcu.fmSize];

	if (!tmpBuf)
		return EMALLOC;

	switch(fmFormat)
	{
		case FFMT_IHEX:
        {
            //FILE *of;

			errCode = convert.IHexToBin(fmFile, tmpBuf,
										mcu.fmBaseAddr, mcu.fmSize);

            /*
            of = fopen("/home/mars/MODEM_disk.bin", "wb");
            fwrite(tmpBuf, 1, mcu.fmSize, of);
            fclose(of);
            printf("OKOKOK\n");*/
			break;
        }

		case FFMT_BINARY:
			errCode = convert.BinToBin(fmFile, tmpBuf, mcu.fmSize);
			break;

		default:
			return EINVFORMAT;
	}

	if (errCode != ESUCCESS)
		return errCode;

	//Define code size (first none 0xFF byte from max address)
	fmCodeSize = mcu.fmSize - 1;
	while (fmCodeSize > 0 && tmpBuf[fmCodeSize] == 0xFF)
		fmCodeSize--;

	//Align firmware code by 256 bytes
	fmCodeSize = ((fmCodeSize + 256) & 0xFFFFFF00);

	fmCode = new u8[fmCodeSize];

	if (fmCode)
		memcpy(fmCode, &tmpBuf[0], fmCodeSize);
	else
		errCode = EMALLOC;	

	return errCode;
}



/** Send sync bytes (0x00) within serial port */
int MCUFmLoader::Synchronize(int syncCount)
{
	int i;
	u8 syncChar = SYNC_CHAR;
	u8 recvChar;

	//Send zeros to synchronize
	for(i = 0; i < syncCount; i++)
	{
		serialPort.Write(&syncChar, 1);				

		//Read data and print progress each N sync chars
		if (!(i & 0x3F))
		{
			//CR received: sync success, return			 
			if (serialPort.Read(&recvChar, 1) > 0 &&
				recvChar == 0x0D)
				return ESUCCESS;

			PrintProgress(COLUMN_PROGRESS, i * 100 / syncCount);
		}
	}

	ThreadSleep(10);

	//Read last data
	while(serialPort.Read(&recvChar, 1) > 0);

	return ECONNFAIL;
}



/** Set new baudrate */
int MCUFmLoader::SetBaudrate()
{
	int rdCount;
	u8 tranBuf[8];
	u8 recvBuf[8];

	//Fill receive buffer with zeros
	memset(recvBuf, 0, 8);

	//Command
	tranBuf[0] = CMD_SETBAUDRATE;
	//Speed, 4 bytes
	memcpy(&tranBuf[1], &iospeed, 4);

	//Send command
	serialPort.Write(tranBuf, 5);

	ThreadSleep(100);
	while(serialPort.Read(recvBuf, 1));

	//Change baudrate at host side
	//Close port
	serialPort.Close();

	//Wait
	ThreadSleep(100);

	//Set new baudrate and try open port
	serialPort.setSpeed(iospeed);
	if (serialPort.Open() != 0)
		return EPORTOPEN;

	//Send CR to get welcome message
	tranBuf[0] = 0x0D;

	//Send CR
	serialPort.Write(tranBuf, 1);

	//Read incoming data
	rdCount = ReadPortData(recvBuf, 3);

	//Receive buffer must contains CR+LF+'>'
	if (rdCount < 3)
		return ECONNFAIL;
	else if (recvBuf[0] == 0x0D &&
			 recvBuf[1] == 0x0A &&
			 recvBuf[2] == 0x3E)
		return ESUCCESS;
	else
		return EINVDATA;
}



/** Write loader executable code to MCU RAM */
int MCUFmLoader::WriteLoader()
{
	int rdCount;
	u32 tranBufU32[3];
	u8 recvBuf[12];
	u8 *tranBuf = (u8*)tranBufU32;

	//Command
	tranBuf[3] = CMD_WRITEMEM;
	//Address
	tranBufU32[1] = mcu.ramBaseAddr + mcu.ldStartAddr;
	//Size, bytes
	tranBufU32[2] = mcu.ldCodeSize;	

	//Send command
	serialPort.Write(&tranBuf[3], 9);

	//Read incoming data
	rdCount = ReadPortData(recvBuf, 1);

	//Check incoming data
	if (rdCount != 1)
		return ECONNFAIL;
	else if (recvBuf[0] != CMD_WRITEMEM)
		return EINVDATA;

	//Send loader executable code
	//serialPort.Write(&ldCodeBuf[ldStartAddr], ldCodeSize);
	serialPort.Write(mcu.ldCode, mcu.ldCodeSize);

	//Read incoming data
	rdCount = ReadPortData(recvBuf, 1);

	//Check incoming data
	if (rdCount != 1)
		return ECONNFAIL;
	else if (recvBuf[0] != REPLY_OK)
		return EINVDATA;

	return ESUCCESS;	
}



/** Verifying loader code */
int MCUFmLoader::VerifyingLoader()
{
	int rdCount;
	u32 i, j;
	u32 tranBufU32[3];
	u8 recvBuf[12];
	u8 *tranBuf = (u8*)tranBufU32;

	//Command
	tranBuf[3] = CMD_READMEM;
	//Address
	tranBufU32[1] = mcu.ramBaseAddr + mcu.ldStartAddr;
	//Size, bytes
	tranBufU32[2] = 8;	

	for(i = 0; i < (mcu.ldCodeSize >> 3); i++)
	{
		//Send command
		serialPort.Write(&tranBuf[3], 9);

		//Increment address by 8
		tranBufU32[1] += 8;

		//Read incoming data: [CMD:1]+[data:8]+[OK:1] = 10 bytes
		rdCount = ReadPortData(recvBuf, 10);

		//Check incoming data
		if (rdCount < 10)
			return ECONNFAIL;
		else if (recvBuf[0] == CMD_READMEM &&
				 recvBuf[9] == REPLY_OK)
		{			
			for(j = 0; j < 8; j++)
				if(recvBuf[j + 1] != mcu.ldCode[8 * i + j])
					return EINVDATA;
		}
		else		
			return EINVDATA;

		//Print progress
		if ((i & 0x07) == 0x00)
			PrintProgress(COLUMN_PROGRESS, i * 100 / (mcu.ldCodeSize >> 3));
	}//for

	return ESUCCESS;
}



/** Start loader */
int MCUFmLoader::StartLoader()
{
	int rdCount;
	u32 tranBufU32[2];
	u8 recvBuf[8];
	u8 *tranBuf = (u8*)tranBufU32;

	//Command
	tranBuf[3] = CMD_START;
	//Address
	if (mcu.id == MCU_1986BE9X || mcu.id == MCU_1901BC1X)
		tranBufU32[1] = mcu.ramBaseAddr + mcu.ldStartAddr;
	else if (mcu.id == MCU_1986BE1X)
		tranBufU32[1] = (*(u32*)&mcu.ldCode[4]) & 0xFFFFFFFEU;

	//Send command
	serialPort.Write(&tranBuf[3], 5);

	//Read incoming data
	rdCount = ReadPortData(recvBuf, 1);

	//Check incoming data
	if (rdCount != 1)
		return ECONNFAIL;
	else if	(recvBuf[0] != CMD_START)
		return EINVDATA;

	return ESUCCESS;
}



/** Check loader ID */
int MCUFmLoader::CheckLoaderID()
{
	int rdCount;
	int i, idLen;
	u8 tranBuf[4];
	u8 recvBuf[256];

	//Get length id string
	idLen = strlen(mcu.ldIdStr);

	//Command
	tranBuf[0] = CMD_GETLOADERID;

	//Send command
	serialPort.Write(tranBuf, 1);

	//Read incoming data
	rdCount = ReadPortData(recvBuf, idLen);

	//Check ID string
	if(rdCount == idLen)
	{
		for(i = 0; i < idLen; i++)
			if(recvBuf[i] != mcu.ldIdStr[i])
				return EINVDATA;
	}
	else if (rdCount > 0)
		return EINVDATA;
	else
		return ECONNFAIL;

	return ESUCCESS;
}



/** Erase flash memory */
int MCUFmLoader::EraseFlash()
{
	int rdCount;
	u32 address, value;
	u8 tranBuf[8];
	u8 recvBuf[16];
	u8 csum;	

	//Command
	tranBuf[0] = CMD_FMERASE;

	if (mcu.id == MCU_1986BE1X)
	{
		//Erase address
		memcpy(&tranBuf[1], &mcu.fmBaseAddr, 4);

		//Solve address checksum
		csum = (u8)(( mcu.fmBaseAddr        & 0xFF) +
					((mcu.fmBaseAddr >> 8 ) & 0xFF) +
					((mcu.fmBaseAddr >> 16) & 0xFF) +
					((mcu.fmBaseAddr >> 24) & 0xFF) );

		//Send command
		serialPort.Write(tranBuf, 5);

		//Read incoming data (checksum)
		rdCount = ReadPortData(recvBuf, 1);

		if (rdCount != 1)		
			return ECONNFAIL;		
		else if ((u8)recvBuf[0] != csum)
			return EINVDATA;

		//serialPort.Write(tranBuf, 1);
	}
	else
	{
		//Send command
		serialPort.Write(tranBuf, 1);
	}

	//Read incoming data: [CMD:1]+[ADDR:4]+[VALUE:4]
	rdCount = ReadPortData(recvBuf, 9);

	if (rdCount != 9)	
		return ECONNFAIL;	
	else if (recvBuf[0] != CMD_FMERASE)
		return EINVDATA;

	memcpy(&address, &recvBuf[1], 4);
	memcpy(&value,   &recvBuf[5], 4);

	if (address != (mcu.fmBaseAddr + mcu.fmSize))
		return EINVADDR;
	else if (value != 0xFFFFFFFFU)
		return EINVDATA;

	return ESUCCESS;
}



/* Write firmware to flash memory */
int MCUFmLoader::WriteFlash()
{
	int rdCount;
	u32 i, j;
	u32 tranBufU32[2];
	u8 recvBuf[4];
	u8 *tranBuf = (u8*)tranBufU32;
	u8 csum;

	if (!fmCode || !fmCodeSize)
		return ESUCCESS;

	//Command
	tranBuf[3] = CMD_FMADDR;
	//Dest address
	tranBufU32[1] = mcu.fmBaseAddr;

	//Solve address checksum
	csum = (u8)(( mcu.fmBaseAddr        & 0xFF) +
				((mcu.fmBaseAddr >> 8 ) & 0xFF) +
				((mcu.fmBaseAddr >> 16) & 0xFF) +
				((mcu.fmBaseAddr >> 24) & 0xFF) );

	//Send command
	serialPort.Write(&tranBuf[3], 5);

	//Read incoming data
	rdCount = ReadPortData(recvBuf, 1);

	if(rdCount != 1)
		return ECONNFAIL;
	else if (recvBuf[0] != csum)
		return EINVDATA;

	tranBuf[0] = CMD_FMWRITE;
	for(i = 0; i < (fmCodeSize >> 8); i++)
	{
		serialPort.Write(tranBuf, 1);
		serialPort.Write(&fmCode[(i << 8)], 256);

		//Solve data checksum
		csum = 0;
		for(j = 0; j < 256; j++)
			csum += fmCode[j + (i << 8)];

		//Read incoming data (checksum)
		rdCount = ReadPortData(recvBuf, 1);

		if(rdCount != 1)
			return ECONNFAIL;
		else if (recvBuf[0] != csum)
			return EINVCSUM;

		//Print progress
		PrintProgress(COLUMN_PROGRESS, i * 100 / (fmCodeSize >> 8));
	}	

	return ESUCCESS;
}



/* Verify data into flash memory */
int MCUFmLoader::VerifyData()
{
	int rdCount;
	u32 i, j, k;
	u32 tranBufU32[2];
	u8 recvBuf[16];
	u8 *tranBuf = (u8*)tranBufU32;
	u8 csum;

	if (!fmCode || !fmCodeSize)
		return EINVDATA;

	//Command
	tranBuf[3] = CMD_FMADDR;
	//Address
	tranBufU32[1] = mcu.fmBaseAddr;

	//Solve address checksum
	csum = (u8)(( mcu.fmBaseAddr        & 0xFF) +
				((mcu.fmBaseAddr >> 8 ) & 0xFF) +
				((mcu.fmBaseAddr >> 16) & 0xFF) +
				((mcu.fmBaseAddr >> 24) & 0xFF) );

	//Send command
	serialPort.Write(&tranBuf[3], 5);

	//Read incoming data
	rdCount = ReadPortData(recvBuf, 1);	

	if(rdCount != 1)
		return ECONNFAIL;
	else if (recvBuf[0] != csum)
		return EINVDATA;

	tranBuf[0] = CMD_FMREAD;
	for(i = 0; i < (fmCodeSize >> 8); i++)
	{
		//256 = 32 x 8
		for(j = 0; j < 32; j++)
		{
			serialPort.Write(tranBuf, 1);

			rdCount = ReadPortData(recvBuf, 8);

			if(rdCount != 8)
				return ECONNFAIL;

			for(k = 0; k < 8; k++)
			{
				if(recvBuf[k] != fmCode[k + (j << 3) + (i << 8)])
				{
					printf("Address=0x%08X\nValue=0x%02X\n",
							mcu.fmBaseAddr + k + (j << 3) + (i << 8),
							recvBuf[k]);
					return EINVDATA;
				}
			}
		}

		//Print progress
		PrintProgress(COLUMN_PROGRESS, i * 100 / (fmCodeSize >> 8));
	}

	return ESUCCESS;
}



/* Start MCU */
int MCUFmLoader::StartMCU()
{
	int rdCount;
	u8 tranBuf[4];
	u8 recvBuf[4];

	//Command
	tranBuf[0] = CMD_START;

	//Send command
	serialPort.Write(tranBuf, 1);

	//Read incoming data
	rdCount = ReadPortData(recvBuf, 1);

	if (rdCount != 1)
		return ECONNFAIL;
	else if (recvBuf[0] != CMD_START)
		return EINVDATA;

	return ESUCCESS;
}



/** Programming flash memory */
int MCUFmLoader::ProgramFlash(int flags)
{
    int errCode = ESUCCESS;

	if ((flags & (FmWrite|FmVerify)) &&
		(fmFile == 0 || strlen(fmFile) == 0))
	{
		PrintErrMsg(EFILEUNDEF);
		return EFILEUNDEF;
	}
	else if (!mcu.id)
	{        
		PrintErrMsg(EINVMCU);
		return EINVMCU;
	}

    printf(STR_INPUT_PARAMETERS,
			mcu.name,
			mcu.fmBaseAddr,
			mcu.ramBaseAddr,
            strlen(ldFile) > 0 ? ldFile : STR_EMBEDDED,
            strlen(fmFile) > 0 ? fmFile : STR_NONE);

    printf(STR_CONVERT_LOADER_TO_BINARY);
	//User-defined loader used
	if (strlen(ldFile) > 0)
	{
		errCode = ConvertLoaderFile();
		PrintErrMsgPos(COLUMN_PROGRESS, errCode);
		if (errCode != ESUCCESS)
			return errCode;
	}

	//Error when loader code is NULL
	if (mcu.ldCode == 0 || mcu.ldCodeSize == 0)
	{
		errCode = ENOLOADER;
		PrintErrMsgPos(COLUMN_PROGRESS, errCode);
		return errCode;
	}
	else
		PrintErrMsgPos(COLUMN_PROGRESS, errCode);

	if (flags & (FmWrite|FmVerify))
	{
        printf(STR_CONVERT_FIRMWARE_TO_BINARY);
		errCode = ConvertFirmwareFile();
		PrintErrMsgPos(COLUMN_PROGRESS, errCode);
		if (errCode != ESUCCESS)
		{
			FreeFmResources();
			return errCode;
		}
	}

    printf(STR_OPEN_SERIAL_PORT, serialPort.getPort());

    //Select port
    serialPort.setPort(ioport);

    //Set initial speed
	serialPort.setSpeed(mcu.ldInitSpeed);

    //No data flow control
    serialPort.setControl(CTRL_NONE);

    //Try open port
	if(serialPort.Open() != 0)
	{
		errCode = EPORTOPEN;
		PrintErrMsgPos(COLUMN_PROGRESS, errCode);
		FreeFmResources();
		return errCode;
	}
	else
		PrintErrMsgPos(COLUMN_PROGRESS, errCode);

    printf(STR_SYNCHRONIZATION);
	errCode = Synchronize(SYNC_CHAR_COUNT);
	CheckErrCode(errCode)

    printf(STR_SET_IO_SPEED, iospeed);
	errCode = SetBaudrate();
	CheckErrCode(errCode)

    printf(STR_WRITE_LOADER_TO_RAM);
	errCode = WriteLoader();
	CheckErrCode(errCode)

    printf(STR_VERIFYING_LOADER);
	errCode = VerifyingLoader();
	CheckErrCode(errCode)

    printf(STR_START_LOADER);
	errCode = StartLoader();
	CheckErrCode(errCode)

    printf(STR_CHECK_LOADER_ID);
	errCode = CheckLoaderID();
	CheckErrCode(errCode)

    printf(STR_PROGRAMMING_STARTED, fmCodeSize);

	//Don't change action sequence.
	//Valid sequence: erase, write, verify, start
	if (flags & (FmErase|FmWrite))
	{
        printf(STR_ERASING);
		errCode = EraseFlash();
		CheckErrCode(errCode)
	}

	if (flags & FmWrite)
	{
        printf(STR_WRITING);
		errCode = WriteFlash();
		CheckErrCode(errCode)
	}

	if (flags & FmVerify)
	{
        printf(STR_VERIFYING);
		errCode = VerifyData();
		CheckErrCode(errCode)
	}

	if (flags & FmStart)
	{
        printf(STR_STARTING);
		errCode = StartMCU();
		CheckErrCode(errCode)
	}

	serialPort.Close();

	FreeFmResources();

    printf(STR_PROGRAMMING_COMPLETED);

	return ESUCCESS;
}

/*===========================================================================
 * Console input/output
 *===========================================================================*/

/** Read keyboard and send key codes within UART */
#ifdef __WINDOWS__

/** Windows version */

/** */
DWORD WINAPI ThreadReadKeys(LPVOID arg)
{
    #define BUFO_SIZE	128
    int ch, bufIndex = 0;
    RS232 *sp = (RS232 *)arg;
    u8 bufo[BUFO_SIZE];

    while(1)
    {
        ch = getchar();        

        bufo[bufIndex++] = (u8)ch;

        //printf("%02X ", ch);

        if (bufIndex == BUFO_SIZE || ch == 0x0A)
        {
            if (bufIndex == 3 &&
                 bufo[0] == 0x5E && bufo[2] == 0x0A &&
                (bufo[1] >= 0x41 && bufo[1] <= 0x5D))
            {
                bufo[0] = bufo[1] - 0x40;
                bufIndex = 1;
            }

            sp->Write(bufo, bufIndex);
            bufIndex = 0;
           // printf("\n");
        }
    }

    return 0;
    #undef BUFO_SIZE
}



/** Read/write data from/to MCU within serial port */
int MCUFmLoader::ReadPortData()
{
    #define BUFI_SIZE	4096
    int errCode, rdCount = 0;
    RS232 serialPort;
    ThreadParams tp;
    char bufi[BUFI_SIZE];

    //Open port with selected speed and data flow control
    serialPort.setPort(ioport);
    serialPort.setSpeed(iospeed);    
    serialPort.setControl(ioctrl);
    serialPort.setDataBits(8);
    serialPort.setParity  (PAR_NONE);
    serialPort.setStopBit (1);

    if(serialPort.Open() != 0)
    {
        fprintf(stderr, STR_OPEN_SERIAL_PORT, serialPort.getPort());
        errCode = EPORTOPEN;
        PrintErrMsgPos(COLUMN_PROGRESS, errCode);
        FreeFmResources();
        return errCode;
    }

    tp.handle  = 0;
    tp.routine = &ThreadReadKeys;
    tp.arg     = &serialPort;

    if (StartThread(&tp) != 0)
    {
        fprintf(stderr, "ERROR: can't start read key thread\n");
        return EINVDATA;
    }

    do
    {
        if ((rdCount = serialPort.Read(bufi, BUFI_SIZE - 1)) > 0)
        {
            //Print buf to standard output and
            bufi[rdCount] = 0x00;
            fprintf(stdout, "%s", bufi);
        }
        else
            ThreadSleep(5);

    } while(rdCount >= 0);

    StopThread(tp.handle);

    serialPort.Close();

    fprintf(stderr, "ERROR: read data stopped\n");

    return rdCount < 0 ? EINVDATA : ESUCCESS;
    #undef BUFI_SIZE
}

#else

/** Linux version */
//Data buffer size, samples (must 2^N, N > 1)
#define INDATABUF_SIZE		8192

//Buffer index mask
#define INDATABUF_MASK		(INDATABUF_SIZE - 1)

//Buffer
u8 inDataBuf[INDATABUF_SIZE];

//Read/write counters
volatile u32 inDataWritei = 0;
volatile u32 inDataReadi = 0;

//Get count of data bytes at buffer
#define GetInDataCount()		(inDataWritei - inDataReadi)

//Get free space at buffer, bytes
#define GetInDataFreeSpace()	(INDATABUF_SIZE - GetInDataCount())

//Read/write data byte from buffer
#define InDataRead()		inDataBuf[(inDataReadi++  & INDATABUF_MASK)]
#define InDataWrite(b)		inDataBuf[(inDataWritei++ & INDATABUF_MASK)] = b
#define InDataItem(i)		inDataBuf[((inDataReadi + (i)) & INDATABUF_MASK)]


/** */
static void *ThreadReadKeys(void *arg)
{	
	int ch;
	while(1)
	{		
		ch = getchar();		

		if (GetInDataFreeSpace() > 0)
		{
			InDataWrite((u8)ch);
		}
	}

	return 0;	
}



/** Read/write data from/to MCU within serial port */
int MCUFmLoader::ReadPortData()
{
	#define BUFI_SIZE	4096	
	int errCode, rdCount = 0, wrCount;
	RS232 serialPort;	
	ThreadParams tp;
	char bufi[BUFI_SIZE];

    //Open port with selected speed and data flow control
    serialPort.setPort(ioport);
	serialPort.setSpeed(iospeed);
    serialPort.setControl(ioctrl);
    serialPort.setDataBits(8);
    serialPort.setParity  (PAR_NONE);
    serialPort.setStopBit (1);

	if(serialPort.Open() != 0)
	{
        fprintf(stderr, STR_OPEN_SERIAL_PORT, serialPort.getPort());
		errCode = EPORTOPEN;
		PrintErrMsgPos(COLUMN_PROGRESS, errCode);
		FreeFmResources();
		return errCode;
	}	

	tp.handle  = 0;
	tp.routine = &ThreadReadKeys;
	tp.arg     = &serialPort;

	if (StartThread(&tp) != 0)
	{
		fprintf(stderr, "ERROR: can't start read key thread\n");
		return EINVDATA;
	}

	do
	{
		if ((rdCount = serialPort.Read(bufi, BUFI_SIZE - 1)) > 0)
		{
			//Print buf to standard output and
			//call flush to immediately print received chars
			bufi[rdCount] = 0x00;
			fprintf(stdout, "%s", bufi);
			fflush(stdout);			
		}		
		else
			ThreadSleep(5);

		if ((wrCount = GetInDataCount()) > 0)
		{
            u8 ch;

            if (GetInDataCount() >= 3 &&
                InDataItem(0) == 0x5E && //'^'
                InDataItem(2) == 0x0A && //'\n'
                (InDataItem(1) >= 0x41 && InDataItem(1) <= 0x5D)) //['A'..']']
            {
				ch = InDataItem(1) - 0x40;

                InDataRead();
                InDataRead();
                InDataRead();				
            }
            else
            {
                ch = InDataRead();
            }

			serialPort.Write(&ch, 1);
		}

	} while(rdCount >= 0);

	StopThread(tp.handle);

	serialPort.Close();	

	fprintf(stderr, "ERROR: read data stopped\n");

	return rdCount < 0 ? EINVDATA : ESUCCESS;
    #undef BUFI_SIZE
}

#endif

/*===========================================================================
 * End of file MCUFmLoader.cpp
 *===========================================================================*/
