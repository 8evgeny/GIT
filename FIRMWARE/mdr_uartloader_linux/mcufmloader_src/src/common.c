/*===========================================================================
 *
 *  Basic functions
 *
 *  Marusenkov S.A., "ostfriesland@mail.ru", 2015
 *
 * THIS SOURCE CODE IS PROVIDED AS IS WITH NO WARRANTIES OF ANY KIND
 * INCLUDING THE WARRANTIES OF DESIGN, MERCHANTIBILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE, OR ARISING FROM A COURSE OF DEALING, USAGE OR
 * TRADE PRACTICE.
 *
 *---------------------------------------------------------------------------
 *
 *  File common.c: Basic functions.
 *
 *===========================================================================*/
#define _CRT_SECURE_NO_WARNINGS
#include "common.h"

#include <time.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#ifdef __WINDOWS__

#include <windows.h>

/** Indicates that colored output is turned on */
static int isColorOutput = 0;

/** */
int StartThread(ThreadParams *p)
{
    p->handle = CreateThread(NULL, 0, p->routine, (LPVOID)p->arg, 0, NULL);
    return (p->handle == NULL) ? -1 : 0;
}



/** */
void StopThread(HANDLE handle)
{
    if (handle)
        CloseHandle(handle);
}



/** Sleep specified number of milliseconds */
void ThreadSleep(int msec)
{
     //WaitInterval(msec);
     SleepEx(msec, 0);
}

#else

#include <unistd.h>
#include <pthread.h>

/** Indicates that colored output is turned on */
static int isColorOutput = 1;

/** */
int StartThread(ThreadParams *p)
{
    return pthread_create(&p->handle, NULL, p->routine, p->arg);
}



/** */
void StopThread(pthread_t handle)
{
    pthread_join(handle, NULL);
}



/** Sleep specified number of milliseconds */
void ThreadSleep(int msec)
{
    struct timespec req;
    struct timespec rem;

    req.tv_sec  = (time_t)(msec / 1000);
    req.tv_nsec = (msec - req.tv_sec * 1000) * 1000000;

    nanosleep(&req, &rem);
}

#endif /* !__WINDOWS__ */



typedef struct tag_ErrMsg
{
    int errCode;
    const char *errStr;
} ErrMsg;


static ErrMsg errMsg[] =
{
    { ESUCCESS,   STR_ESUCCESS   },
    { EINVPARAM,  STR_EINVPARAM  },
    { EINVMCU,    STR_EINVMCU    },
    { EINVADDR,   STR_EINVADDR   },
    { EINVSPEED,  STR_EINVSPEED  },
	{ EPORTOPEN,  STR_EPORTOPEN  },
	{ ECONNFAIL,  STR_ECONNFAIL  },
	{ EFILEUNDEF, STR_EFILEUNDEF },
    { EFILEOPEN,  STR_EFILEOPEN  },
    { EINVFORMAT, STR_EINVFORMAT },
	{ EINVDATA,   STR_EINVDATA   },
	{ EINVCSUM,   STR_EINVCSUM   },
	{ EBUFSIZE,   STR_EBUFSIZE   },
	{ EMALLOC,    STR_EMALLOC    },
	{ ENOLOADER,  STR_ENOLOADER  },
    { 0, "\0" }
};

#ifdef __WINDOWS__
#define STR_SUCCESS_FORMAT      " %-16s\n"
#define STR_FAILURE_FORMAT      " %s %i: \"%s\"\n"
#else
#define STR_SUCCESS_FORMAT      "%-16s\n"
#define STR_FAILURE_FORMAT      "%s %i: \"%s\"\n"
#endif

/** Print error message using specified error code */
void PrintErrMsg(int code)
{
	if (code == ESUCCESS)
	{
		if (isColorOutput)
			SetGraphic( ATTR_MESSAGE,
						FCOLOR_MESSAGE,
						BCOLOR_MESSAGE);

        printf(STR_SUCCESS_FORMAT, STR_SUCCESS);
	}
	else
	{
		int i = 0;
		const char *errStr = 0;

		if (isColorOutput)
			SetGraphic( ATTR_MESSAGE_ERR,
						FCOLOR_MESSAGE_ERR,
						BCOLOR_MESSAGE_ERR);

		while(strlen(errMsg[i].errStr) > 0)
		{
			if (errMsg[i].errCode == code)
				errStr = errMsg[i].errStr;
			i++;
		}

		if (!errStr)
			errStr = "Unknown error";

        printf(STR_FAILURE_FORMAT, STR_FAILURE, code, errStr);
	}

	if (isColorOutput)
		ResetGraphic();
}



/** Print error message using specified column and error code */
void PrintErrMsgPos(int column, int code)
{
	CurMove(CURUP, 1);
	CurMove(CURFWD, column);
	PrintErrMsg(code);
}



/** Convert string to 32-bit unsigned integer */
u32 StrToU32(const char *src, u32 isHex)
{
	s32 i = strlen(src);
	u32 res = 0, c;

	if (!src || i == 0)
		return (u32)0;

	if (isHex)
	{
		u32 shift = 0;
		i--;
		while(i >= 0 && shift < 32)
		{
			c = (u32)src[i--];

			//c at ['0'-'9', 'A'-'F', 'a'-'f']
			if (c >= 0x30 && c <= 0x39)
				c -= 0x30;
			else if (c >= 0x41 && c <= 0x46)
				c -= 0x37;
			else if (c >= 0x61 && c <= 0x66)
				c -= 0x57;
			else
				break;

			res |= c << shift;
			shift += 4;
		}

		return res;
	}
	else
		return (u32)atoi(src);
}




/** Convert string to upper/lower case */
static char *StrChangeCase(char *dest, const char *src, int n, int toUpper)
{
    int i = 0, len;

    len = strlen(src);

    if (n > len)
        n = len;

    memcpy(dest, src, n);
	dest[n] = 0;

	if (toUpper)
		while(i < n)
		{
			if (dest[i] >= 'a' && dest[i] <= 'z')
				dest[i] -= 0x20;
			i++;
		}
	else
		while(i < n)
		{
			if (dest[i] >= 'A' && dest[i] <= 'Z')
				dest[i] += 0x20;
			i++;
		}

	return dest;
}



/** Convert string to upper case */
char *StrToUpper(char *dest, const char *src, int n)
{
	return StrChangeCase(dest, src, n, 1);
}



/** Convert string to lower case */
char *StrToLower(char *dest, const char *src, int n)
{
    return StrChangeCase(dest, src, n, 0);
}



/** Get file name */
char *GetFileName(const char *path, char *fname, int noext)
{
    s32 i, exti, len, fnlen;

    if (!path || (len = strlen(path)) == 0)
    {
        fname[0] = 0;
        return fname;
    }

    //Last char
    i = len - 1;

    //Search first slash from right
    while(i >= 0 && !(path[i] == '\\' || path[i] == '/')) i--;

    //Search '.'
    if (noext)
    {
        exti = len - 1;
        while(exti >= 0 && !(path[exti] == '.' || path[exti] == '\\' || path[exti] == '/')) exti--;

        if (path[exti] == '.')
            len = exti;
    }

    if (i < 0)
    {
        memcpy(fname, path, len);
        fname[len] = 0;
    }
    else
    {
        fnlen = len - i - 1;

        if (fnlen > 0)
            memcpy(fname, &path[i + 1], fnlen);

        fname[fnlen] = 0;
    }

    return fname;
}



/** Returns current date and time at ASCII string format */
char *GetDateTime()
{
    time_t currTime;
    struct tm *ptm;

    time(&currTime);
    ptm = localtime(&currTime);

    /*printf("%i-%i-%i %i:%i:%i\n",
           1900 + ptm->tm_year, ptm->tm_mon, ptm->tm_mday,
           ptm->tm_hour, ptm->tm_min, ptm->tm_sec);*/

    return asctime(ptm);
}

/*===========================================================================
 * Text graphic functions
 *===========================================================================*/

#ifdef __WINDOWS__

/* Windows version */

/** Turn on/off color output */
void TurnColorOutput(int turn)
{
    isColorOutput = 0;
}



/** Set selected graphic mode */
void SetGraphic(int attr, int fColor, int bColor)
{
    //no actions at Windows version
}



/** Reset graphic mode to default state */
void ResetGraphic()
{
    //no actions at Windows version
}



/** Clear current line */
void ClearLine(char mode)
{
    //no actions at Windows version
}



/** Moves the cursor by the specified number of lines */
void CurMove(char curdir, int count)
{
    //no actions at Windows version
}



/** Set cursor position */
void SetCurPos(int column, int line)
{
    //no actions at Windows version
}



/** */
void PrintProgressMsg(const char *message, int progress)
{    
    printf("%s %4i%%\n", message, progress);
}



/** */
void PrintProgress(int column, int progress)
{
    printf("%c%c%c%2i%%", 0x08, 0x08, 0x08, progress);
}



/** */
void PrintBusyMsg(const char *message)
{
    printf(".");
}



/** */
void PrintBusy(int column)
{
    printf(".");
}

#else

/* Linux version */

//Text attributes values
//Attributes off
#define VAOFF			0
//Bold text (increased brightness)
#define VABOLD			1
//Underline text
#define VAUNDER			4
//Blink text
#define VABLINK			5
//Reverse colors
#define VAREVERSE		7
//Concealed
#define VACONCEALED		8

//Count of attributes
#define ATTR_COUNT		6

/** Escape sequence */
static const char ESC_CSI[] = { 0x1B, 0x5B, 0x00 };

/** Busy indication */
static const char *busyStrings[] =
{
    ".", "..", "...", "....", "....", "...", "..", "."
};

/** Current busy string index */
static int busyCharsIndex = 0;

/** Array of attributes values */
static const char attrValues[ATTR_COUNT] =
{
    VAOFF, VABOLD, VAUNDER, VABLINK, VAREVERSE, VACONCEALED
};



/** Turn on/off color output */
void TurnColorOutput(int turn)
{
    isColorOutput = turn & 1;

    if (!isColorOutput)
        SetGraphic(AOFF, CNONE, CNONE);
}



/** Set selected graphic mode */
void SetGraphic(int attr, int fColor, int bColor)
{
    int i = 0;

    //ESC start sequence
    printf("%s", ESC_CSI);

    //Add attributes
    if (!attr)
        printf(";");
    else
        while(i < ATTR_COUNT)
        {
            if (attr & 1)
                printf("%i;", attrValues[i]);

            attr >>= 1;
            i++;
        }

    if (fColor != CNONE)
        printf("%i", fColor);

    if (bColor != CNONE)
        printf(";%i", bColor + 10);

    printf("m");
}



/** Reset graphic mode to default state */
void ResetGraphic()
{
    SetGraphic(AOFF, CNONE, CNONE);
}



/** Clear current line */
void ClearLine(char mode)
{
    printf("%s%uK", ESC_CSI, mode);
}



/** Moves the cursor by the specified number of lines */
void CurMove(char curdir, int count)
{
    printf("%s%u%c", ESC_CSI, count, curdir);
}



/** Set cursor position */
void SetCurPos(int column, int line)
{
    printf("%s%u;%uH", ESC_CSI, line, column);
}



/** */
void PrintProgressMsg(const char *message, int progress)
{
    printf("%s", message);

    if (isColorOutput)
        SetGraphic(ATTR_PROGRESS, FCOLOR_PROGRESS, BCOLOR_PROGRESS);

    printf("%4i%%\n", progress);

    if (isColorOutput)
        ResetGraphic();

    CurMove(CURUP, 1);
}



/** */
void PrintProgress(int column, int progress)
{
    CurMove(CURUP, 1);
    CurMove(CURFWD, column);

    if (isColorOutput)
        SetGraphic(ATTR_PROGRESS, FCOLOR_PROGRESS, BCOLOR_PROGRESS);

    printf("%i%%\n", progress);

    if (isColorOutput)
        ResetGraphic();
}



/** */
void PrintBusyMsg(const char *message)
{
    printf("%s%-8s\n", message, busyStrings[busyCharsIndex++ & 0x07]);
    CurMove(CURUP, 1);
}



/** */
void PrintBusy(int column)
{
    CurMove(CURUP, 1);
    CurMove(CURFWD, column);
    printf("%-8s\n", busyStrings[busyCharsIndex++ & 0x07]);
}

#endif /* !__WINDOWS__ */

/*===========================================================================
 * End of file common.c
 *===========================================================================*/
