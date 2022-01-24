/*===========================================================================
 *
 *  MCU firmware loader
 *
 *  Internet Society, 2014-2016
 *
 * THIS SOURCE CODE IS PROVIDED AS IS WITH NO WARRANTIES OF ANY KIND
 * INCLUDING THE WARRANTIES OF DESIGN, MERCHANTIBILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE, OR ARISING FROM A COURSE OF DEALING, USAGE OR
 * TRADE PRACTICE.
 *
 *---------------------------------------------------------------------------
 *
 *  File fmloader-1901bc1x-main.c: Firmware loader (MCU 1901BC1x).
 *
 *===========================================================================*/

#include "../../fmloader-common.h"

typedef unsigned char				u8;
typedef unsigned int				u32;
typedef volatile unsigned long		__io_reg;

typedef struct {
	__io_reg DR;
	__io_reg RSR_ECR;
	__io_reg __dummy0[4];
	__io_reg FR;
	__io_reg __dummy1;
	__io_reg ILPR;
	__io_reg IBRD;
	__io_reg FBRD;
	__io_reg LCR_H;
	__io_reg CR;
	__io_reg IFLS;
	__io_reg IMSC;
	__io_reg RIS;
	__io_reg MIS;
	__io_reg ICR;
	__io_reg DMACR;
} _uart;

#define EEPROM_KEY_VALUE    0x8AAA5551

//MCU flash memory start: 0x8000000, size: 128K
#define EEPROM_START_ADDR   0x08000000
#define EEPROM_END_ADDR	    (EEPROM_START_ADDR + 128 * 1024)

//EEPROM flag bit number at PER_CLOCK register
#define EEPROM_CLOCK_NUMBER	3

//PER_CLOCK register
#define PER_CLOCK			(*(__io_reg*)(0x40020000 + (0x07 << 2)))

//Read/write data within UART3 (MCU F0/F1 pins)
#define UART3				((_uart   *) 0x400D0000)

//EEPROM registers
#define EEPROM_CMD			(*(__io_reg *) 0x40018000)
#define EEPROM_ADR			(*(__io_reg *) 0x40018004)
#define EEPROM_DI			(*(__io_reg *) 0x40018008)
#define EEPROM_DO			(*(__io_reg *) 0x4001800C)
#define EEPROM_KEY			(*(__io_reg *) 0x40018010)

//EEPROM_CMD flags
#define CON					0x0001
#define XE					0x0040
#define YE					0x0080
#define SE					0x0100
#define IFREN				0x0200
#define ERASE				0x0400
#define MAS1				0x0800
#define PROG				0x1000
#define NVSTR				0x2000

//Id string length, chars
#define FMLOADER_ID_LENGTH		12

//Data buffer size at 32-bit words (must 64)
#define DATABUF_SIZE			64

//Id string
const unsigned char fmloader_id[FMLOADER_ID_LENGTH] = "1901bc1xfldr";

//Data buffer
u32 dataBuf[DATABUF_SIZE];
//Data buffer counter
u32 dataBufIndex;
//Current flash address
u32 currFmAddr;

/* Implemented at "startup_1986be9x.s" */
void MCUStart(void);

#if defined(__GNUC__)
#define __attribute_weak	__attribute__((weak))
#else
#define __attribute_weak
#endif


/* */
__attribute_weak
void WaitInterval(int usec)
{
    do
    {
		usec++;
		usec -= 2;
    }
    while (usec != 0);
}



/* */
u8 UARTReadByte(void)
{    
    //RXFE[4] == 1 - input fifo is empty
	while(UART3->FR & (1 << 4));
	return (u8)UART3->DR;
}



/* */
void UARTWriteByte(u8 val)
{    
    //TXFF[5] == 1 - output fifo is full
	while(UART3->FR & (1 << 5));
	UART3->DR = val;
	return;
}



/* */
u32 UARTReadU32(u8 *csum)
{
	u32 i, rd, val;
	u8 cs = 0;

    val = 0;
    for(i = 0; i < 4; i++)
    {
		rd = UARTReadByte();
		cs += rd;

		val = (val >> 8) | (rd << 24);
    }

	*csum = *csum + cs;
    return val;
}



/* */
void UARTWriteU32(u32 val)
{
    int i;
    for(i = 0; i < 4; i++)
    {
		UARTWriteByte(val & 0xFF);
		val >>= 8;
    }

    return;
}



/* */
void EraseFlash(void)
{
	u32 i, val;

	EEPROM_KEY = EEPROM_KEY_VALUE;
	EEPROM_CMD |= CON;

	//Erase all memory: A,B,C,D sectors
	for (i = 0; i < 16; i += 4)
	{
		EEPROM_ADR = EEPROM_START_ADDR + i;

		EEPROM_CMD &= ~(XE|YE|SE|MAS1|ERASE|NVSTR);

		EEPROM_CMD |= XE|MAS1|ERASE;
		WaitInterval(6);     //~6us

		EEPROM_CMD |= NVSTR;
		WaitInterval(50000); //~50ms

		EEPROM_CMD &= ~ERASE;
		WaitInterval(110);   //~110us

		EEPROM_CMD &= ~(XE|MAS1|NVSTR);
		WaitInterval(2);     //~2us
	} 

	//Check result: all cells == 0xFFFFFFFF
	for (i = EEPROM_START_ADDR; i < EEPROM_END_ADDR; i += 4)
	{
		EEPROM_ADR = i;

		EEPROM_CMD |= XE|YE|SE;
		WaitInterval(1);

		val = EEPROM_DO;

		EEPROM_CMD &= ~(XE|YE|SE);
		WaitInterval(1);

		if(val != 0xFFFFFFFF)
			break;
	}

	EEPROM_CMD &= ~CON;

	//Send status
	UARTWriteByte(CMD_FMERASE);
	UARTWriteU32(i);
	UARTWriteU32(val);
	return;
}



/* */
void WriteFlash(void)
{
	u32 i, j;
	u8 checkSum = 0;

	for(i = 0; i < DATABUF_SIZE; i++)
		dataBuf[i] = UARTReadU32(&checkSum);

	EEPROM_KEY = EEPROM_KEY_VALUE;
	EEPROM_CMD |= CON;
	EEPROM_CMD &= ~(XE|YE|SE|MAS1|ERASE|NVSTR|PROG);

	//For each sector: [A,B,C,D]
	for (i = 0; i < 16; i += 4)
	{
		EEPROM_ADR = currFmAddr + i;
		EEPROM_CMD |= XE|PROG;
		WaitInterval(6);  //~6us

		EEPROM_CMD |= NVSTR;
		WaitInterval(11); //~11us

		//Write current sector
		for(j = 0; j < 256; j += 16)
		{
			EEPROM_ADR = currFmAddr + j + i;
			EEPROM_DI  = dataBuf[((i + j) >> 2)];

			EEPROM_CMD |= YE;
			WaitInterval(50); //~50us

			EEPROM_CMD &= ~YE;
			WaitInterval(1);  //~1us
		}

		EEPROM_CMD &= ~PROG;
		WaitInterval(6); //~6us

		EEPROM_CMD &= ~(XE|NVSTR);
		WaitInterval(1); //~1us
	}

	EEPROM_CMD &= ~CON;

	//Increment of address by count of written bytes
	currFmAddr += (DATABUF_SIZE << 2);

	//Send checksum
	UARTWriteByte(checkSum);
}



/* */
void ReadFlash(void)
{
	int i;
	EEPROM_KEY = EEPROM_KEY_VALUE;
	EEPROM_CMD |= CON;
	EEPROM_CMD &= ~(XE|YE|SE);

	for(i = 0; i < DATABUF_SIZE; i++)
	{
		EEPROM_ADR = currFmAddr;
		currFmAddr += 4;

		EEPROM_CMD |= XE|YE|SE;
		WaitInterval(1);
		dataBuf[i] = EEPROM_DO;

		EEPROM_CMD &= ~(XE|YE|SE);
		WaitInterval(1);
	}

	EEPROM_CMD &= ~CON;
}



/* */
void VerifyData(void)
{
	u32 i;

    i = dataBufIndex;

    if(i == 0)
		ReadFlash();

    UARTWriteU32(dataBuf[i]);
    i++;

    UARTWriteU32(dataBuf[i]);
    i++;

    dataBufIndex = i & (DATABUF_SIZE - 1);
}



/* */
void main(void)
{
	u32 i;
	u8 checkSum;

	//EEPROM clock enable
	PER_CLOCK |= (1 << EEPROM_CLOCK_NUMBER);

	currFmAddr   = 0;
    dataBufIndex = 0;    	

    while(1)
    {
		i = UARTReadByte();
		switch(i)
		{
			case CMD_GETLOADERID:
			{
				for(i = 0; i < FMLOADER_ID_LENGTH; i++)
					UARTWriteByte(fmloader_id[i]);
				break;
			}

			case CMD_FMERASE:
			{
				EraseFlash();
				break;
			}

			case CMD_FMWRITE:
			{
				WriteFlash();
				break;
			}

			case CMD_FMREAD:
			{
				VerifyData();
				break;
			}

			case CMD_FMADDR:
			{
				checkSum     = 0;
				dataBufIndex = 0;
				currFmAddr   = UARTReadU32(&checkSum);
				UARTWriteByte(checkSum);
				break;
			}

			case CMD_START:
			{
				UARTWriteByte(CMD_START);
				WaitInterval(200);
				MCUStart();
				break;
			}
		 }
    }
}

/*===========================================================================
 * End of file fmloader-1901bc1x-main.c
 *===========================================================================*/
