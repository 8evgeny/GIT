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
 *  File MCUFmLoader.h: Firmware loader definitions.
 *
 *===========================================================================*/

#ifndef _MCUFMLOADER
#define _MCUFMLOADER

#include "bastypes.h"
#include "common.h"
#include "mcus.h"
#include "FmConverter.h"
#include "RS232.h"

using namespace std;

//Erase mcu flash memory
#define	FmErase                         0x01
//Write firmware to flash memory
#define FmWrite                         0x02
//Verify data into flash memory
#define FmVerify                        0x04
//Start MCU
#define FmStart                         0x08

//Port speed restrictions
//Minimum speed, bit/s
#define IOSPEED_MIN                     9600
//Maximum speed, bit/s
#define IOSPEED_MAX                     115200

//Read data timeout, ms
#define PORT_READ_TIMEOUT               5000

//Count of sync chars
#define SYNC_CHAR_COUNT                 2048

//Column number where percents is printed
#define COLUMN_PROGRESS                 32

//Defaults
#define DEFAULT_PORT                    0
#define DEFAULT_IOSPEED                 IOSPEED_MAX
#define DEFAULT_FORMAT                  FFMT_IHEX
#define DEFAULT_FORMAT_NAME             FFMTNM_IHEX

//Output string messages
#define STR_INPUT_PARAMETERS            "MCU: %s; FM: 0x%08X; RAM: 0x%08X\nLoader: %s\nFile: %s\n"
#define STR_EMBEDDED                    "embedded"
#define STR_NONE                        "none"

#ifdef __WINDOWS__

/* Windows version */
#define STR_CONVERT_LOADER_TO_BINARY    "Convert loader to binary...      "
#define STR_CONVERT_FIRMWARE_TO_BINARY  "Convert firmware to binary...    "
#define STR_OPEN_SERIAL_PORT            "Open serial port #%i...           "
#define STR_SYNCHRONIZATION             "Synchronization...               "
#define STR_SET_IO_SPEED                "Set iospeed %6i...            "
#define STR_WRITE_LOADER_TO_RAM         "Write loader to MCU RAM...       "
#define STR_VERIFYING_LOADER            "Verifying loader...              "
#define STR_START_LOADER                "Start loader...                  "
#define STR_CHECK_LOADER_ID             "Check loader ID...               "
#define STR_PROGRAMMING_STARTED         "Programming started (%i bytes)...\n"
#define STR_ERASING                     "Erasing...                       "
#define STR_WRITING                     "Writing...                       "
#define STR_VERIFYING                   "Verifying...                     "
#define STR_STARTING                    "Starting...                      "
#define STR_PROGRAMMING_COMPLETED       "Programming completed\n"

#else

/* Linux version */
#define STR_CONVERT_LOADER_TO_BINARY    "Convert loader to binary...\n"
#define STR_CONVERT_FIRMWARE_TO_BINARY  "Convert firmware to binary...\n"
#define STR_OPEN_SERIAL_PORT            "Open serial port #%i...\n"
#define STR_SYNCHRONIZATION             "Synchronization...\n"
#define STR_SET_IO_SPEED                "Set iospeed %i...\n"
#define STR_WRITE_LOADER_TO_RAM         "Write loader to MCU RAM...\n"
#define STR_VERIFYING_LOADER            "Verifying loader...\n"
#define STR_START_LOADER                "Start loader...\n"
#define STR_CHECK_LOADER_ID             "Check loader ID...\n"
#define STR_PROGRAMMING_STARTED         "Programming started (%i bytes)...\n"
#define STR_ERASING                     "Erasing...\n"
#define STR_WRITING                     "Writing...\n"
#define STR_VERIFYING                   "Verifying...\n"
#define STR_STARTING                    "Starting...\n"
#define STR_PROGRAMMING_COMPLETED       "Programming completed\n"

#endif

/** */
class MCUFmLoader
{
private:

	/** Serial port */
	RS232 serialPort;

	/** Current MCU parameters */
    MCUParams mcu;

    /** Port number */
    int ioport;

	/** Port i/o speed */
	int iospeed;

    /** Port data flow control */
    int ioctrl;

	/** Read data timeout, ms */
	int readTimeout;	

	/** Size of firmware */
	u32 fmCodeSize;	

	/** Pointer to firmware executable code buffer */
	u8 *fmCode;

	/** Format of loader file (see FFMT_x) */
	int ldFormat;

	/** Format of firmware file (see FFMT_x) */
	int fmFormat;

	/** Path to loader file */
	char ldFile[MAX_FILENAME_LENGTH];

	/** Path to firmware file */
	char fmFile[MAX_FILENAME_LENGTH];	

	/** Save filename and it format */
	int SetFile(char *dest, int *fmt,
				const char *file, const char *fmtName);	

	/** Free resource used by loader */
	int FreeLdResources();

	/** Free resource used by firmware */
	int FreeFmResources();

	/** Read data from serial port using specified buffer and timeout */
	int ReadPortData(void *data, size_t n);	

	/** Convert loader file to binary data */
	int ConvertLoaderFile();

	/** Convert loader file to binary data */
	int ConvertFirmwareFile();

	/** Send sync bytes (0x00) within serial port */
	int Synchronize(int syncCount);

	/** Set new baudrate */
	int SetBaudrate();

	/** Write loader executable code to MCU RAM */
	int WriteLoader();

	/** Verifying loader code */
	int VerifyingLoader();

	/** Start loader */
	int StartLoader();

	/** Check loader ID */
	int CheckLoaderID();

	/** Erase flash memory */
	int EraseFlash();

	/** Write firmware to flash memory */
	int WriteFlash();

	/** Verify data into flash memory */
	int VerifyData();

	/** Start MCU */
	int StartMCU();

	/** Read keyboard and send key codes within UART */
	//void *ThreadSendKeys(void *arg);

public:
	/** */
	MCUFmLoader();

	/** */
	~MCUFmLoader();

	/** Select port number */
    int SetIOPort(int portNumber);

	/** Select I/O speed, bit/s */
	int SetIOSpeed(int iospeed);

    /** Select port I/O flow control */
    int SetIOCtrl(int ctrl);

	/** Set read data timeout, ms */
	int SetRdTimeout(int value);    

	/** Set MCU model using specified id */
	int SetMCUModel(int id);

	/** Set MCU model using specified name */
	int SetMCUModel(const char *name);

	/** Set MCU model using specified parameters */
	int SetMCUModel(const MCUParams *m);

	/** Set destination flash address */
	int SetMCUFMAddr(int addr);

	/** Set destination ram address */
	int SetMCURAMAddr(int addr);

	/** Select loader file */
	int SetLdFile(const char *ldfile, const char *ldFormatName);

	/** Select firmware file to programming */
	int SetFmFile(const char *fmfile, const char *fmFormatName);	

	/** Programming flash memory */
	int ProgramFlash(int flags);	

	/** Read/write data from/to MCU within serial port */
	int ReadPortData();

	/** Free resources */
	int FreeResources();
};

#endif /* !_MCUFMLOADER */

/*===========================================================================
 * End of file MCUFmLoader.h
 *===========================================================================*/
