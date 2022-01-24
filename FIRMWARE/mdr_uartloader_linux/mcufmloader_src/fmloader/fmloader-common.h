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
 *  File fmloader-common.h: Firmware loaders common definitions.
 *
 *===========================================================================*/

#ifndef __FMLOADER_COMMON_H
#define __FMLOADER_COMMON_H

//Command list
//Embedded commands (see MCU manual)
//Set baudrate
#define CMD_SETBAUDRATE				'B'
//Write data to MCU RAM
#define CMD_WRITEMEM				'L'
//Read data from MCU memory
#define CMD_READMEM					'Y'
//Start from specified address
#define CMD_START					'R'
//Reply success
#define REPLY_OK					'K'

//User defined commands
//Get loader ID string
#define CMD_GETLOADERID				'I'
//Set flash memory address
#define CMD_FMADDR					'A'
//Erase flash memory
#define CMD_FMERASE					'E'
//Write flash memory
#define CMD_FMWRITE					'P'
//Read flash memory
#define CMD_FMREAD					'V'

#endif /* __FMLOADER_COMMON_H */

/*===========================================================================
 *  End of file fmloader-common.h
 *===========================================================================*/
