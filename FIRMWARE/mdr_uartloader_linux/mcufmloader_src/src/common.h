/*===========================================================================
 *
 *  Basic functions
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
 *  File common.h: Base definitions and functions prototypes.
 *
 *===========================================================================*/

#ifndef _COMMON_H
#define _COMMON_H

//Uncomment to build for Windows
#if (defined _WIN32) || (defined _WIN32_WCE)
#define __WINDOWS__     1
#endif

#ifdef __cplusplus
extern "C" {
#endif

#include "bastypes.h"

//Maximum length of filename
#define MAX_FILENAME_LENGTH		512

//Error codes
#define ESUCCESS          0
#define EINVPARAM         -1
#define EINVMCU           -2
#define EINVADDR          -3
#define EINVSPEED         -4
#define EPORTOPEN         -5
#define ECONNFAIL         -6
#define EFILEUNDEF        -7
#define EFILEOPEN         -8
#define EINVFORMAT        -9
#define EINVDATA          -10
#define EINVCSUM		  -11
#define EBUFSIZE		  -12
#define EMALLOC			  -13
#define ENOLOADER		  -14

#define STR_SUCCESS        "OK"
#define STR_FAILURE        "ERROR"

#define STR_ESUCCESS       STR_SUCCESS
#define STR_EINVPARAM      "Invalid parameter"
#define STR_EINVMCU        "Invalid MCU model"
#define STR_EINVADDR       "Invalid address"
#define STR_EINVSPEED      "Invalid I/O speed"
#define STR_EPORTOPEN	   "Port open error"
#define STR_ECONNFAIL      "Connection failed"
#define STR_EFILEUNDEF     "File is not defined"
#define STR_EFILEOPEN      "File open error"
#define STR_EINVFORMAT     "Invalid data format"
#define STR_EINVDATA       "Invalid data"
#define STR_EINVCSUM       "Invalid checksum"
#define STR_EBUFSIZE	   "Invalid buffer size"
#define STR_EMALLOC		   "Memory allocation error"
#define STR_ENOLOADER	   "Loader does not have executable code"

//ESC commands definitions
//Move cursor direction
//Move up
#define CURUP		'A'
//Move down
#define CURDOWN		'B'
//Move forward
#define CURFWD		'C'
//Move back
#define CURBACK		'D'

//Clear line modes
//Clear line from cursor to end
#define CLREND		0
//Clear line from cursor to start
#define CLRBEG		1
//Clear current line
#define CLRLINE		2

//Text attributes
//No attributes
#define ANONE		0
//Attributes off
#define AOFF		1
//Bold text (increased brightness)
#define ABOLD		2
//Underline text
#define AUNDER		4
//Blink text
#define ABLINK		8
//Reverse colors
#define AREVERSE	16
//Concealed
#define ACONCEALED	32

//Fore/back colors
#define CNONE		0
#define CBLACK		30
#define CRED		31
#define CGREEN		32
#define CYELLOW		33
#define CBLUE		34
#define CMAGENTA	35
#define CCYAN		36
#define CWHITE		37

/** Function output colors */
/** Progress */
#define ATTR_PROGRESS			AOFF
#define FCOLOR_PROGRESS			CYELLOW
#define BCOLOR_PROGRESS			CNONE
/** Success message */
#define ATTR_MESSAGE			AOFF
#define FCOLOR_MESSAGE			CGREEN
#define BCOLOR_MESSAGE			CNONE
/** Error message */
#define ATTR_MESSAGE_ERR		AOFF
#define FCOLOR_MESSAGE_ERR		CRED
#define BCOLOR_MESSAGE_ERR		CNONE


#ifndef __WINDOWS__
#include <pthread.h>
typedef void* (*pThreadRoutine)(void *arg);
typedef pThreadRoutine LpThreadRoutine;

typedef struct tag_ThreadParams
{
	pthread_t handle;
	LpThreadRoutine routine;
	void *arg;
} ThreadParams;


/** */
void StopThread(pthread_t handle);

#else
#include <windows.h>
typedef DWORD WINAPI (*pThreadRoutine)(LPVOID arg);
typedef pThreadRoutine LpThreadRoutine;

typedef struct tag_ThreadParams
{
	HANDLE handle;
	LpThreadRoutine routine;
	void *arg;
} ThreadParams;

/** */
void StopThread(HANDLE handle);

#endif

/** */
int StartThread(ThreadParams *tp);

/** Sleep specified number of milliseconds */
void ThreadSleep(int msec);

/** Print error message using specified error code */
void PrintErrMsg(int code);

/** Print error message using specified column and error code */
void PrintErrMsgPos(int column, int code);

/** Convert string to 32-bit unsigned integer */
u32 StrToU32(const char *src, u32 isHex);

/** Convert string to upper case */
char *StrToUpper(char *dest, const char *src, int n);

/** Convert string to lower case */
char *StrToLower(char *dest, const char *src, int n);

/** Get file name */
char *GetFileName(const char *path, char *fname, int noext);

/** Returns current date and time at ASCII string format */
char *GetDateTime();

/** Turn on/off color output */
void TurnColorOutput(int turn);

/** Set selected graphic mode */
void SetGraphic(int attr, int fColor, int bColor);

/** Reset graphic mode to default state */
void ResetGraphic();

/** Clear current line */
void ClearLine(char mode);

/** Moves the cursor by the specified number of lines */
void CurMove(char curdir, int count);

/** Set cursor position */
void SetCurPos(int column, int line);

/** Print progress (percent) */
void PrintProgressMsg(const char *message, int progress);

/** Print progress (percent) */
void PrintProgress(int column, int progress);

/** Print busy indication */
void PrintBusyMsg(const char *message);

/** Print busy indication */
void PrintBusy(int column);

#ifdef __cplusplus
}
#endif

#endif /* !_COMMON_H */

/*===========================================================================
 * End of file common.h
 *===========================================================================*/
