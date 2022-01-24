#ifndef UNIT_LIB_H
#define UNIT_LIB_H

#ifdef PLATFORM_WINDOWS
#include "stdwin.h"

//WHAR* StrDup(char* string) ;
#endif
#ifdef PLATFORM_LINUX
char* StrDup(char* string) ;
#define Sleep(a)	usleep(a*1000)
#endif

int OpenUnit() ;
void SaveUnitCfg() ;

#ifdef PLATFORM_WINDOWS

HANDLE OpenSerialPort(char *COMPORT_name) ;
void CloseSerialPort(HANDLE hCom) ;
int SetCommProp( HANDLE hCom, char *stringv, unsigned short timeout ) ;
int ReadSerial(HANDLE hCom, char *Bytes, int len) ;
int WriteSerial(HANDLE hCom, char *Bytes, int len) ;

#endif
#ifdef PLATFORM_LINUX

int OpenSerialPort(char *COMPORT_name) ;
void CloseSerialPort(int hCom) ;
int SetCommProp( int hCom, char *stringv, unsigned short timeout ) ;
int ReadSerial(int hCom, char *Bytes, int len) ;
int WriteSerial(int hCom, char *Bytes, int len) ;

#endif


int InitUnit() ;
void StopUnit() ;
void UnInitUnit() ;
int ConnectToChip() ;

void SetNextCmdSymbol() ;
int GetOneValuesFromCommand(char *cOperand) ;
int GetTwoValuesFromCommand() ;
int TryToGetTwoArguments() ;
int TryToGetOneArguments() ;

char *AppendCRC16(char *cPtr, int n) ;

int GetPacketFromController( char *cCommand232 ) ;
int GetPacketFromControllerLimited() ;
int SendCommandToController( char *str ) ;
int SendCommandToController_fast( char *str ) ;
int SendCommandToController_nowait( char *str ) ;

int TestCommandOK() ;


void procTest() ;
void ConfigureAxis() ;


void SaveAxisConfigure() ;
int SendCommandToController_LimitedWait( char *str ) ;

void delay_ms( int time) ;
int procGetReady( );


int StringPatternIsFound( char *sExecuteMessageBuffer, char *CommandString ) ;


int WriteMySerial(char *Bytes, int len) ;
int ReadMySerial(char *Bytes, int len) ;

#endif // UNIT_LIB_H
