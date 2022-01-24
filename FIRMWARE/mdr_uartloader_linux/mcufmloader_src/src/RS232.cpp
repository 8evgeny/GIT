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
 *  File RS232.cpp: Serial port class implementation.
 *
 *===========================================================================*/
#define _CRT_SECURE_NO_WARNINGS

#include "RS232.h"

#include <stdexcept>
#include <stdlib.h>
#include <stdio.h>   // Standard input/output definitions
#include <string.h>  // String function definitions
#include <fcntl.h>   // File control definitions
#include <errno.h>   // Error number definitions

/** */
RS232::RS232()
{
	port 		= SERPORT_0;
	speed 		= 57600;
	dataBits	= 8;
	parity		= PAR_NONE;
	stopBit		= 1;
	control		= CTRL_NONE;

    hSerial	= 0;
}



/** */
RS232::~RS232()
{
}



/** */
int RS232::getPort()
{
	return port;
}



/** */
void RS232::setPort(int number)
{	
    if (number >= 0 && number < 300)
		this->port = number;
}



/** */
int RS232::getSpeed()
{
	return speed;
}



/** */
void RS232::setSpeed(int speed)
{
    if (speed >= 1200 && speed <= 230400)
		this->speed = speed;
}



/** */
int RS232::getDataBits()
{
	return dataBits;
}



/** */
void RS232::setDataBits(int dataBits)
{
	if (dataBits >= 5 && dataBits <= 8) 
		this->dataBits = dataBits;
}



/** */
int RS232::getParity()
{
	return parity;
}



/** */
void RS232::setParity(int parity)
{
	this->parity = parity & 7;
}



/** */
int RS232::getStopBit()
{
	return stopBit;
}



/** */
void RS232::setStopBit(int stopBit)
{
	if (stopBit == 1 || stopBit == 2)
		this->stopBit = stopBit;
}



/** */
int RS232::getControl()
{
	return control;
}



/** */
void RS232::setControl(int control)
{
	this->control = control & 3;
}


#ifdef __WINDOWS__

/** Windows version */

/** */
int RS232::Open()
{
    DWORD        iospeed;
    DCB          dcb;
    COMMTIMEOUTS timeouts;
    int          portN, len;
    wchar_t      serialPortFile[16];
    u8 *pFnameBuf = (u8*)serialPortFile;

    memset(&dcb, 0, sizeof(DCB));
    memset(&timeouts, 0, sizeof(COMMTIMEOUTS));

    //"CreateFile" function used wide chars at lpFileName parameter,
    memset(serialPortFile, 0, 16 * sizeof(wchar_t));
    wcscpy(serialPortFile, L"COM");
    len = wcslen(serialPortFile) * sizeof(wchar_t);

    //getPort() returns port index [0..N]:
    //[0,1,2,...,N] => [COM1,COM2,...COM(N+1)]
    portN = getPort() + 1;

    if (portN < 10)
        pFnameBuf[len] = (u8)((portN % 10) + 0x30);
    else
    {
        pFnameBuf[len]     = (u8)((int)(portN / 10) + 0x30);
        pFnameBuf[len + 2] = (u8)((portN % 10) + 0x30);
    }

    //Open serial port
    hSerial = CreateFile(serialPortFile,
                         GENERIC_READ|GENERIC_WRITE, //read/write
                         0,                          //not shared
                         NULL,                       //no security
                         OPEN_EXISTING,              //open existing port
                         FILE_ATTRIBUTE_NORMAL,      //normal
                         NULL);                      //no templates

    if (hSerial == INVALID_HANDLE_VALUE)
    {
            fprintf(stderr, "open serial port COM%i error\r\n", portN);
            return -1;
    }

    dcb.DCBlength = sizeof(dcb);

    //Get current configuration
    if (GetCommState(hSerial, &dcb) == 0)
    {
            fprintf(stderr, "error getting parameters\n");
            CloseHandle(hSerial);
            return -10;
    }

    //Set binary mode, no EOF check
    dcb.fBinary = 1;

    //Set input/output speed
    switch(getSpeed())
    {
            case 115200: iospeed = CBR_115200; break;
            case 57600 : iospeed = CBR_57600;  break;
            case 38400 : iospeed = CBR_38400;  break;
            case 19200 : iospeed = CBR_19200;  break;
            case 9600  : iospeed = CBR_9600;   break;
            case 4800  : iospeed = CBR_4800;   break;
            case 2400  : iospeed = CBR_2400;   break;
            default    : iospeed = CBR_57600;  break;
    }

    dcb.BaudRate = iospeed;

    //Set parity
    switch(getParity())
    {
            case PAR_EVEN:  dcb.Parity = EVENPARITY; break;
            case PAR_ODD:   dcb.Parity = ODDPARITY;  break;
            case PAR_MARK:  dcb.Parity = MARKPARITY; break;
            case PAR_SPACE: dcb.Parity = SPACEPARITY; break;
            default: dcb.Parity = NOPARITY; break;
    }

    //Enable 2 stop bits
    if (getStopBit() == 2)
        dcb.StopBits = TWOSTOPBITS;
    else
        dcb.StopBits = ONESTOPBIT;

    //Symbol size
    switch(getDataBits())
    {
            case 8:  dcb.ByteSize = 8; break;
            case 7:  dcb.ByteSize = 7; break;
            case 6:  dcb.ByteSize = 6; break;
            case 5:  dcb.ByteSize = 5; break;
            default: dcb.ByteSize = 8; break;
    }

    dcb.fRtsControl = RTS_CONTROL_DISABLE;

    //Flow control
    //RTS/CTS
    if (getControl() & CTRL_RTSCTS)
    {
        dcb.fRtsControl = RTS_CONTROL_ENABLE;
        //dcb.fRtsControl = RTS_CONTROL_HANDSHAKE;
        //dcb.fRtsControl = RTS_CONTROL_TOGGLE;
    }

    //XON/XOFF
    if (getControl() & CTRL_XONXOFF)
    {
        dcb.fOutX = 1;
        dcb.fInX  = 1;
    }
    else
    {
        dcb.fOutX = 0;
        dcb.fInX  = 0;
    }

    /*printf("fOutX=%li, fInX=%li, XON=%02X, XOFF=%02X, fBinary=%li\n",
           dcb.fOutX,   dcb.fInX,
           dcb.XonChar, dcb.XoffChar,
           dcb.fBinary);*/

    //Update configuration
    if(SetCommState(hSerial, &dcb) == 0)
    {
            fprintf(stderr, "error setting parameters\n");
            CloseHandle(hSerial);
            return -11;
    }

    //Set COM port timeouts
    timeouts.ReadIntervalTimeout         = MAXDWORD;
    timeouts.ReadTotalTimeoutMultiplier  = 0;
    timeouts.ReadTotalTimeoutConstant    = 0;
    timeouts.WriteTotalTimeoutMultiplier = 0;
    timeouts.WriteTotalTimeoutConstant   = 0;

    if(SetCommTimeouts(hSerial, &timeouts) == 0)
    {
            fprintf(stderr, "error setting timeouts\n");
            CloseHandle(hSerial);
            return -12;
    }

    //Reset tx/rx queues
    PurgeComm(hSerial, PURGE_TXCLEAR | PURGE_RXCLEAR);

    return 0;
}



/** */
void RS232::Close()
{
    PurgeComm(hSerial, PURGE_TXCLEAR | PURGE_RXCLEAR);
    CloseHandle(hSerial);
}



/** */
int RS232::Write(const void *data, size_t n)
{
    DWORD wrCount;
    if (!WriteFile(hSerial, data, n, &wrCount, NULL))
        return -1;
    else
        return (int)wrCount;
}



/** */
int RS232::Read(void* data, size_t n)
{
    DWORD rdCount;
    if (!ReadFile(hSerial, data, n, &rdCount, NULL))
        return -1;
    else
        return (int)rdCount;
}

#else

/** Linux version */

#include <unistd.h>  // UNIX standard function definitions
#include <termios.h> // POSIX terminal control definitions

/** */
int RS232::Open()
{
	//TTY parameters
	termios tty;
	//Requested input/output speed
        speed_t iospeed;
	//Function result
	int result = 0, pnum;
	//Serial port filename
	char serialPortFile[32];	

	//Get port number
	pnum = getPort();

    if (pnum <= 3)       //[0..3]   => ttyS[0..3]
        sprintf(serialPortFile, "/dev/ttyS%i", pnum);
    else if (pnum <= 11) //[4..11]  => ttyUSB[0..7]
        sprintf(serialPortFile, "/dev/ttyUSB%i", pnum - 4);
    else                 //[12..19] => ttyACM[0..7]
        sprintf(serialPortFile, "/dev/ttyACM%i", pnum - 12);
	
	hSerial = open(serialPortFile, O_RDWR | O_NOCTTY | O_NDELAY);

	if (hSerial < 0)
	{
		hSerial = 0;
		fprintf(stderr, "open serial port (%s) error\r\n", serialPortFile);
		result = -1;
	}
	else
	{
		if (lockf(hSerial, F_TEST, 0) == -1)
		{	
			fprintf(stderr, "lock file (F_TEST) error\r\n");
			result = -2;
		};
		
		if (lockf(hSerial, F_LOCK, 0) == -1)
		{
			fprintf(stderr, "lock file (F_LOCK) error\r\n");
			result = -4;
		};
	}
	

	if (result == 0)
	{
		//success	
	}
	else
	{
		if (hSerial)
		{
			close(hSerial);
			hSerial = 0;
		}
	
		return result;
	}

	fcntl(hSerial, F_SETFL, FNDELAY);
	
	//Get current port configuration
	tcgetattr(hSerial, &tty);

	//Set input/output speed
	switch(getSpeed())
	{
		case 230400: iospeed = B230400; break;
		case 115200: iospeed = B115200; break;
		case 57600 : iospeed = B57600;  break;
		case 38400 : iospeed = B38400;  break;
		case 19200 : iospeed = B19200;  break;
		case 9600  : iospeed = B9600;   break;
		case 4800  : iospeed = B4800;   break;
		case 2400  : iospeed = B2400;   break;
		default    : iospeed = B57600;  break;
	}

	//Set input/output speed
	cfsetispeed(&tty, iospeed);
	cfsetospeed(&tty, iospeed); 
	
	//Reset flags
	//Control flags
	tty.c_cflag &= ~(CSIZE|CRTSCTS|CLOCAL|CREAD|CSTOPB|HUPCL|PARENB|PARODD);
	//Input mode flags
	tty.c_iflag &= ~(BRKINT|ICRNL|IGNBRK|IGNCR|IGNPAR|IMAXBEL|INLCR|INPCK|ISTRIP|IUCLC|IXANY|IXOFF|IXON|PARMRK|IUTF8);
	//Output mode flags
	tty.c_oflag &= ~(BSDLY|CRDLY|FFDLY|NLDLY|TABDLY|VTDLY|OCRNL|OFDEL|OFILL|OLCUC|ONLCR|ONLRET|ONOCR|OPOST|XTABS);
	//Local flags
	tty.c_lflag &= ~(ECHOCTL|ECHO|ECHOE|ECHOK|ECHONL|ECHOKE|ECHOPRT|ICANON|IEXTEN|ISIG|NOFLSH|TOSTOP|XCASE);

	//Set parity
	switch(getParity())
	{
		case PAR_EVEN: tty.c_cflag |= PARENB; break;
		case PAR_ODD:  tty.c_cflag |= PARODD; break;
	}
	
	//Enable 2 stop bits
	if (getStopBit() == 2)
		tty.c_cflag |= CSTOPB;

	//Symbol size
	switch(getDataBits())
	{
		case 8:  tty.c_cflag |= CS8; break;
		case 7:  tty.c_cflag |= CS7; break;
		case 6:  tty.c_cflag |= CS6; break;
		case 5:  tty.c_cflag |= CS5; break;
		default: tty.c_cflag |= CS8; break;
	}

	//Flow control
	//RTS/CTS
	if (getControl() & CTRL_RTSCTS)
		tty.c_cflag |= CRTSCTS;
	
	//XON/XOFF
	if (getControl() & CTRL_XONXOFF)
		tty.c_iflag |= (IXON|IXOFF);
	
	//Set other flags
	tty.c_cflag |= (CREAD); //|CLOCAL);
	//tty.c_iflag |= (0);
	tty.c_oflag |= (NL0|CR0|TAB0|BS0|VT0|FF0);
	tty.c_lflag |= (IEXTEN);

	//Control chars
	tty.c_cc[VMIN]  = 1;
	tty.c_cc[VTIME] = 0;
	
	//Apply configuration changes now
    tcsetattr(hSerial, TCSANOW, &tty);

	return result;
}



/** */
void RS232::Close()
{
    if (hSerial)
	{
        tcflush(hSerial, TCIOFLUSH);

        close(hSerial);

        hSerial = 0;
	}
}



/** */
int RS232::Write(const void *data, size_t n)
{
	int wrCount;

    if (hSerial < 1)
		return -1;

	//Write data to port
    wrCount = write(hSerial, data, n);

	//Wait while all data will send
    tcdrain(hSerial);

	return wrCount;
} 



/** */
int RS232::Read(void* data, size_t n)
{
	int rdCount;

    if (hSerial < 1)
		return -1;
	
    rdCount = read(hSerial, data, n);
	
	//Assume that command generated no response
	if (rdCount < 0 && errno == EAGAIN)
		return 0;

	return rdCount;
}

#endif /* !__WINDOWS__ */

/*===========================================================================
 * End of file RS232.cpp
 *===========================================================================*/

