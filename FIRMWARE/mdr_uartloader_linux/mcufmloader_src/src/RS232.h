/*===========================================================================
 *
 *  I/O ports
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
 *  File RS232.h: Serial port class interface.
 *
 *===========================================================================*/

#ifndef _RS232_H
#define _RS232_H

#include <stdlib.h>
#include "common.h"

#ifdef __WINDOWS__
#include <windows.h>
#endif

#define SERPORT_0		0
#define SERPORT_1		1
#define SERPORT_2		2
#define SERPORT_3		3
#define SERPORT_4		4
#define SERPORT_5		5
#define SERPORT_6		6
#define SERPORT_7		7

#define PAR_NONE		0
#define PAR_EVEN		1
#define PAR_ODD			2
#define PAR_MARK		3
#define PAR_SPACE		4

#define CTRL_NONE		0
#define CTRL_RTSCTS		1
#define CTRL_XONXOFF		2
#define CTRL_RTSCTSXONXOFF	3

using namespace std;

/* Provides serial port interface */
class RS232 
{
private:

    #ifdef __WINDOWS__
    HANDLE hSerial;
    #else
    int hSerial;
    #endif

	//Serial port options:
	int port;	
	int speed;
	int dataBits;	
	int parity;
	int stopBit;
	int control;

public:

	RS232();
	~RS232();

   	int  Open();

	void Close();

   	int  Read(void *data, size_t n);
   
   	int  Write(const void *data, size_t n);

	int getPort();
	void setPort(int number);
	
	int getSpeed();
	void setSpeed(int speed);

	int getDataBits();
	void setDataBits(int dataBits);
	
	int getParity();
	void setParity(int parity);
	
	int getStopBit();
	void setStopBit(int stopBit);

	int getControl();
	void setControl(int control);
};

#endif /* !_RS232_H */

/*===========================================================================
 * End of file RS232.h
 *===========================================================================*/
