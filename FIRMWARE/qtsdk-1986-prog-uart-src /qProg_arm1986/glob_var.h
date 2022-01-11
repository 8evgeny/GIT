#ifndef GLOB_VAR_H
#define GLOB_VAR_H

#define PLATFORM_LINUX
//#define PLATFORM_WINDOWS


enum{
IDM_ERASE_FLASH = 0,
IDM_READ_FLASH,
IDM_WRITE_FLASH,
IDM_VERIFY,
IDM_RUN
};




typedef struct
{
    int iBaudsIndex;
    int iBits;
    int iStopBits;
    int iParityOn ;
    int iParityType ;
    int iHwFlow ;
    int iSwFlow ;
    int iAddCRC16 ;
    int iAddFastCRC16 ;
    int iBauds;
} TTY_MODE ;





typedef unsigned char uint8_t ;
typedef signed char int8_t ;
typedef unsigned short int uint16_t ;
typedef short int int16_t ;


#define DEF_MODE_VERIFY     0x00000001
#define DEF_MODE_RUN        0x00000002

typedef struct
{
    char sFileHistory[5][512] ;
    char sCurrentDirectory[512] ;
    int iCurrentFileIndex ;
    TTY_MODE tty_mode ;
    char COMPORT_name[200] ;
    int iUserMode ;
} TUNE_TYPE ;




#ifndef EXTERN

TUNE_TYPE  Tune ;


char cModeString[100] ;


int iMutex, iFlagStop = 1, iFlagPause = 0, iUnitOn = 0 ;
char cBuffer[256] ;

char  *CmdBufferPtr, cOperand1[20], cOperand2[20], cOperand3[20] ;
char  cCommand232[256], cReturn232[256] ;

int iRecievedSybbols = 0, iErrors ;

char Port_names[100][200] ;
int iNComPorts = 0 ;

int iChipConnectionSuccess = 0, iFlagBootLoaderActived = 0 ;

#else

extern TUNE_TYPE  Tune ;

extern char cModeString[100] ;


extern int iMutex, iFlagStop, iFlagPause, iUnitOn  ;
extern char cBuffer[256] ;

extern char  *CmdBufferPtr, cOperand1[20], cOperand2[20], cOperand3[20] ;
extern char  cCommand232[256], cReturn232[256] ;

extern int iRecievedSybbols, iErrors ;

extern char Port_names[100][200] ;
extern int iNComPorts ;

extern int iChipConnectionSuccess, iFlagBootLoaderActived ;

#endif


#endif // GLOB_VAR_H
