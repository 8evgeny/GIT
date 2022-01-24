#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>



#include <errno.h>





#define EXTERN
#include "glob_var.h"


#include "unit_lib.h"

//static WCHAR WBuffer[100] ;
static wchar_t WBuffer[100] ;


#include <cstdlib>


/*static char sCommandBuffer[256] ;
static char sNextCommandBuffer[256] ;*/


#ifdef PLATFORM_WINDOWS

#include <time.h>
#include <io.h>
#include <math.h>
#include <process.h>
#include <dos.h>
#include <direct.h>
#include <conio.h>


HANDLE hCom1 ;
DCB  dcb;
COMMTIMEOUTS CommTimeouts;
#endif
#ifdef PLATFORM_LINUX

int hCom1 ;
//DCB  dcb;
int CommTimeouts;

#endif

int CRCTest(char *cBuffer, int iLen) ;
char *AppendCRC16(char *cPtr, int n) ;



/*
void PrintHelp()
{

        printf("\n\n PLT commands interpretator - terminal.\n");
        printf("\n\n InteriorCommands:\n\n");
        printf("\nhelp - print this text.\n");
        printf("\nfile <FileName> - Execute program in file.\n");
        printf("\nHX   - Go to reper X.\n");
        printf("\nLT   - Go to ZERO (x=0 y=0) \n");
        printf("\nGX   - Get current X-position \n");
        printf("GY   - Get current Y-position \n");
        printf("\nSX <Value>  - Set current X-position \n");
        printf("SY <Value>  - Set current Y-position \n");
        printf("\nVS <Value>  - Set speed of cut \n");
        printf("\nLASER_F <Value>  - Set laser frequensy for current speed of cut \n");
        printf("\nPU <ValueX> <ValueY> - Fast movement to point (X,Y) \n");
        printf("\nPD <ValueX> <ValueY> - Work movement to point (X,Y) \n");
        printf("\nSEOFF - Set END_SWITCHs Off. \n");
        printf("\nSEON  - Set END_SWITCHs On. \n");
        printf("\nTEST - Test controllers XY and Y axises. \n");
        printf("\nquit - Exit to Windows. \n");
}

*/









/**************************************************************************/
/* wchar_t to char */
char* wtoc(const wchar_t* w, size_t max)
{
  char* c = new char[max];
  wcstombs(c,w,max);
  return c;
}

/* char to wchar_t */
wchar_t* ctow(const char* c, size_t max)
{
  wchar_t* w = new wchar_t[max];
  mbstowcs(w,c,max);
  return w;
}


inline void wfree( wchar_t *buff )
{ delete [] buff; }

inline void cfree( char *buff )
{ delete [] buff; }


// code

/*  size_t slen = strlen(text);
  wchar_t* pSzToRender = ctow( text, slen );
  pSzToRender[slen+1] = '\0';
  m_pFont->DrawTextW( ... pSzToRender );
  wfree( pSzToRender );

*/

/************************************************************/




//static char COMPORT_name_ext[100] ;




#ifdef PLATFORM_WINDOWS




WCHAR* StrDup(char* string)
{

    mbstowcs( WBuffer, string, strlen(string) );

    return WBuffer ;
}


//static char COMPORT_name_ext[100] ;

HANDLE OpenSerialPort( char *COMPORT_name)
{
        HANDLE  hCom ;
        DWORD err ;

        //strcat( COMPORT_name_ext, "//.//" ) ;
        //sprintf( COMPORT_name_ext, "\\\\.\\%s", COMPORT_name ) ;


        hCom = CreateFile(  StrDup(COMPORT_name) , // Pointer to the name of the port
                      GENERIC_READ| GENERIC_WRITE,  // Access (read-write) mode
                      1,            // Share mode
                      NULL,         // Pointer to the security attribute
                      OPEN_EXISTING,// How to open the serial port
                      0,            // Port attributes
                      NULL);        // Handle to port with attribute
                                    // to copy


        err = GetLastError() ;
        if(err == EACCES)
            return hCom ;
        if(err == ENOENT)
            return hCom ;

        if ( hCom == INVALID_HANDLE_VALUE ) return hCom ;

        return hCom ;
}




void CloseSerialPort(HANDLE hCom)
{
        if( (hCom != INVALID_HANDLE_VALUE ) && (hCom >0 ) )
            CloseHandle(hCom);
}






int SetCommProp( HANDLE hCom, char *stringv, unsigned short timeout )
{
        unsigned short   ret = -1 ;
        char  sDef[256] ;	// pointer to device-control string
        int Baud_Rate, Parity, Data_Bits ;
        double Stop_Bits ;

       // return 0 ;

        if( (hCom != INVALID_HANDLE_VALUE ) && (hCom >0 ) )
        {
                strcpy(sDef,"baud=19200 parity=N data=8 stop=1");
                if(stringv!=NULL) strcpy(sDef,stringv);

                printf("\nsDef=%s\n",sDef);
                sscanf(sDef,"baud=%d parity=%d data=%d stop=", &Baud_Rate, &Parity, &Data_Bits)  ;
                int i = 0 ;
                while( strncmp(&sDef[i],"stop=",4) !=0 )
                {
                    i++ ;
                }

                Stop_Bits = atof( &sDef[i+5] ) ;

               // BuildCommDCB( StrDup(sDef), &dcb);
                printf("Stop_Bits=%.1f\n",Stop_Bits) ;
                fflush(stdout) ;

                dcb.DCBlength = sizeof (DCB);


                GetCommState (hCom, &dcb);

                dcb.BaudRate = Baud_Rate ;              // Current baud
                dcb.fBinary = FALSE;               // Binary mode; no EOF check
                dcb.fParity = FALSE;               // Enable parity checking
                dcb.fOutxCtsFlow = FALSE;         // No CTS output flow control
                dcb.fOutxDsrFlow = FALSE;         // No DSR output flow control
                dcb.fDtrControl = DTR_CONTROL_DISABLE;//ENABLE;
                                                                                        // DTR flow control type
                dcb.fDsrSensitivity = FALSE;      // DSR sensitivity
                dcb.fTXContinueOnXoff = FALSE;     // XOFF continues Tx
                dcb.fOutX = FALSE;                // No XON/XOFF out flow control
                dcb.fInX = FALSE;                 // No XON/XOFF in flow control
                dcb.fErrorChar = FALSE;           // Disable error replacement
                dcb.fNull = FALSE;                // Disable null stripping
                dcb.fRtsControl = RTS_CONTROL_DISABLE;//ENABLE;
                                                                                                // RTS flow control
                dcb.fAbortOnError = FALSE;        // Do not abort reads/writes on
                                                                  // error
                dcb.ByteSize = 8;                 // Number of bits/byte, 4-8
                dcb.Parity = NOPARITY;            // 0-4=no,odd,even,mark,space
                dcb.StopBits = ONESTOPBIT; // 0,1,2 = 1, 1.5, 2
                if( Stop_Bits>=1 ) dcb.StopBits = (Stop_Bits-1)*2 ;


                /*sPtr=stringv;//sDef;
                do{
                        sscanf(sPtr,"%s",sBuff);
                        if(strncmp("baud=",sBuff,5)==0) dcb.BaudRate=atoi(&sBuff[5]);
                        if(strncmp("parity=",sBuff,7)==0)
                        {
                                dcb.Parity=atoi(&sBuff[7]);
                                if(dcb.Parity!=0) dcb.fParity = TRUE;
                        }
                        if(strncmp("stop=",sBuff,5)==0) dcb.StopBits=atoi(&sBuff[5]);
                        while( (*sPtr!=' ')&&(*sPtr!=0x00) ) sPtr++;
                        while( *sPtr==' ' ) sPtr++;

                } while(*sPtr!=0x00);*/


                ret = SetCommState (hCom, &dcb) ;
                GetCommTimeouts (hCom, &CommTimeouts) ;

                // Change the COMMTIMEOUTS structure settings.
                CommTimeouts.ReadIntervalTimeout = 0;
                CommTimeouts.ReadTotalTimeoutMultiplier = timeout;
                CommTimeouts.ReadTotalTimeoutConstant = 0;
                CommTimeouts.WriteTotalTimeoutMultiplier = 0;
                CommTimeouts.WriteTotalTimeoutConstant = MAXDWORD;

                // Set the time-out parameters for all read and write operations
                // on the port.
                SetCommTimeouts (hCom, &CommTimeouts);


                ret=SetCommState (hCom, &dcb);
        }

        return ret ;
}









int ReadSerial(HANDLE hCom, char *Bytes, int len)
{

        DWORD dwBytesTransferred;


        if( (hCom != INVALID_HANDLE_VALUE ) && (hCom >0 ) )
        {
                        ReadFile (hCom,              // Port handle
                                        Bytes,                // Pointer to data to read
                                        len,                    // Number of bytes to read
                                        &dwBytesTransferred,  // Pointer to number of bytes
                                                                // read
                                        NULL                  // Must be NULL for Windows CE
                                        );
                        /**(int*)(res->intv)= ReadFile(
                                                hCom1, (LPVOID)(value->stringv),
                                                len, &len2,
                                                NULL );*/

                        return  dwBytesTransferred;

        }


        return 0 ;
}






int WriteSerial(HANDLE hCom, char *Bytes, int len)
{

        DWORD dwNumBytesWritten;

        if( (hCom != INVALID_HANDLE_VALUE ) && (hCom >0 ) )
        {

                        WriteFile (hCom,              // Port handle
                   Bytes,              // Pointer to the data to write
                       len,                  // Number of bytes to write
                           &dwNumBytesWritten, // Pointer to the number of bytes
                               // written
                                NULL                // Must be NULL for Windows CE
                                );


                        return dwNumBytesWritten;

        }

        return 0 ;
}

#endif
#ifdef PLATFORM_LINUX

#define INVALID_HANDLE_VALUE	-1


#include <unistd.h>

#include <malloc.h>
#include <math.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <termios.h>
#include <sys/signal.h>
#include <dirent.h>                        /* win */


#define Sleep(a)	usleep(a*1000)


int OpenSerialPort( char *COMPORT_name)
{

     return  open( COMPORT_name, O_RDWR | O_NOCTTY  | O_NONBLOCK ); 

}




void CloseSerialPort(int   hCom)
{
        if( (hCom != INVALID_HANDLE_VALUE ) && (hCom >0 ) )
            close(hCom);
}













static int Baud_Rate, Data_Bits, Parity, Stop_Bits=1;

static int BAUD, DATABITS, PARITYON, PARITY, STOPBITS;


int SetSerialParams()
{
     switch (Baud_Rate)
      {
         case 115200:
            BAUD = B115200;
            break;
         case 57600:
            BAUD = B57600;
            break;
         case 38400:
            BAUD = B38400;
            break;
         case 19200:
            BAUD  = B19200;
            break;
         case 9600:
            BAUD  = B9600;
            break;
         case 4800:
            BAUD  = B4800;
            break;
         case 2400:
            BAUD  = B2400;
            break;
         case 1800:
            BAUD  = B1800;
            break;
         case 1200:
            BAUD  = B1200;
            break;
         case 600:
            BAUD  = B600;
            break;
         case 300:
            BAUD  = B300;
            break;
         case 200:
            BAUD  = B200;
            break;
         case 150:
            BAUD  = B150;
            break;
         case 134:
            BAUD  = B134;
            break;
         case 110:
            BAUD  = B110;
            break;
         case 75:
            BAUD  = B75;
            break;
         case 50:
            BAUD  = B50;
            break;
         default:
            BAUD = B38400;
            break;
      }  //end of switch baud_rate
      switch (Data_Bits)
      {
         case 8:
         default:
            DATABITS = CS8;
            break;
         case 7:
            DATABITS = CS7;
            break;
         case 6:
            DATABITS = CS6;
            break;
         case 5:
            DATABITS = CS5;
            break;
      }  //end of switch data_bits
      switch (Stop_Bits)
      {
         case 2:
            STOPBITS = CSTOPB;
            break;
        case 1:
        default:
         STOPBITS = 0;
         //break;
      }  //end of switch stop bits
      switch (Parity)
      {
         case 0:
         default:                       //none
            PARITYON = 0;
            PARITY = 0;
            break;
         case 1:                        //odd
            PARITYON = PARENB;
            PARITY = PARODD;
            break;
         case 2:                        //even
            PARITYON = PARENB;
            PARITY = 0;
            break;
      }  //end of switch parity
      
      return 0;
}       





int SetCommProp( int  hCom, char *stringv, unsigned short timeout )
{
        unsigned short   ret = -1 ;
        char  sDef[256] ;	// pointer to device-control string
		struct termios oldtio,newtio;


       // return 0 ;

        if( (hCom != INVALID_HANDLE_VALUE ) && (hCom >0 ) )
        {
                strcpy(sDef,"baud=115200 parity=N data=8 stop=1");
			if(stringv!=NULL) strcpy(sDef,stringv);

			printf("\nsDef=%s\n",sDef);
                        sscanf(sDef,"baud=%d parity=%d data=%d stop=%d", &Baud_Rate, &Parity, &Data_Bits, &Stop_Bits);
                        int i ;
                        char *cPtr = sDef ;
                        for(i=0;i<strlen(sDef);i++)
                        {
                            if( strncmp(cPtr,"baud=",5) == 0 )
                            {
                                cPtr+=5 ;
                                sscanf( cPtr,"%d",&Baud_Rate ) ;
                            }
                            if( strncmp(cPtr,"parity=",5) == 0 )
                            {
                                cPtr+=7 ;
                                sscanf( cPtr,"%d",&Parity ) ;
                            }
                            if( strncmp(cPtr,"data=",5) == 0 )
                            {
                                cPtr+=5 ;
                                sscanf( cPtr,"%d",&Data_Bits ) ;
                            }
                            if( strncmp(cPtr,"stop=",5) == 0 )
                            {
                                cPtr+=5 ;
                                sscanf( cPtr,"%d",&Stop_Bits ) ;
                            }
                            cPtr++ ;
                        }

			printf("New values  baud=%d parity=%d data=%d stop=%d\n", Baud_Rate, Parity, Data_Bits, Stop_Bits);
			//BOUD, DATABITS, PARITYON;


			SetSerialParams();
			

      //install the serial handler before making the device asynchronous
      /*saio.sa_handler = signal_handler_IO;
      sigemptyset(&saio.sa_mask);   //saio.sa_mask = 0;
      saio.sa_flags = 0;
      saio.sa_restorer = NULL;
      sigaction(SIGIO,&saio,NULL);

      // allow the process to receive SIGIO
      fcntl(hCom1, F_SETOWN, getpid());
      // Make the file descriptor asynchronous (the manual page says only
      // O_APPEND and O_NONBLOCK, will work with F_SETFL...)*/
      //fcntl(hCom1, F_SETFL, FASYNC);

      // set new port settings for canonical input processing 
      newtio.c_cflag = BAUD | DATABITS | STOPBITS | PARITYON | PARITY | CLOCAL | CREAD ;
      //BAUD | CRTSCTS | DATABITS | STOPBITS | PARITYON | PARITY | CLOCAL | CREAD ;
      newtio.c_iflag = IGNPAR | IGNBRK;
      newtio.c_oflag = ONOCR;
      newtio.c_lflag = 0;//ICANON;
      newtio.c_cc[VMIN]=4;
      newtio.c_cc[VTIME]=timeout;
      tcflush(hCom, TCIFLUSH);
     ret=tcsetattr(hCom,TCSANOW /*| TCOON | TCION*/ ,&newtio);
      //printf("BOUD_MACRO=%d   19200=%d\n",BAUD,B19200);
      tcgetattr(hCom,&oldtio); // save current port settings 
      if(oldtio.c_cflag == newtio.c_cflag )
        printf("c_cflag OK\n") ;
      if(oldtio.c_iflag == newtio.c_iflag)
        printf("c_iflag OK\n") ;
      if(oldtio.c_oflag == newtio.c_oflag)
        printf("c_oflag OK\n") ;
      if(oldtio.c_lflag == newtio.c_lflag)
        printf("c_lflag OK\n") ;       //ICANON;
      if(oldtio.c_cc[VMIN]==newtio.c_cc[VMIN])
        printf("c_cc[VMIN] OK\n") ;
      if(oldtio.c_cc[VTIME]==newtio.c_cc[VTIME])
        printf("c_cc[VTIME] OK\n") ;

        }

        return 1/*ret*/ ;
}










int ReadSerial(int port, char *Bytes, int len)
{
	return read ( port,      // Port handle
		Bytes,                // Pointer to data to read
		len                    // Number of bytes to read
		);
}

int WriteSerial(int port, char *Bytes, int len)
{
	return write ( port,      // Port handle
		Bytes,                // Pointer to data to read
		len                    // Number of bytes to read
		);
}



#endif

/**************************************************************************************************

        UNIT Serial Data IO

**************************************************************************************************/


int InitUnit()
{
        FILE *fp ;
        char cPort[20]="COM3:" ;
        //int i ;

        fp=fopen("port.cfg","rb") ;
        if(fp!=NULL)
        {
                fgets(cPort,20,fp);
                fclose(fp) ;
        }


        //cPort[5]= 0 ;
        iFlagStop = 0 ;
        iMutex = 0 ;

        strcpy( Tune.COMPORT_name, cPort ) ;

        return  OpenUnit() ;
}

int OpenUnit()
{
    int  i ;
    int iLen ;

    Sleep(1) ;

        hCom1 = OpenSerialPort( Tune.COMPORT_name ) ;

        if(hCom1 == INVALID_HANDLE_VALUE )
        {
                printf("Port  %s  not opened!\n", Tune.COMPORT_name ) ;
                printf("\n") ;
                //getchar() ;
                fflush(stdout) ;
                return -1 ;
        }

        printf("Port  %s opened, fp=%d\n", Tune.COMPORT_name, hCom1 ) ;
        fflush(stdout) ;

        i = SetCommProp( hCom1, (char*)("baud=9600 parity=0 data=8 stop=1") , 1 /*timeout*/ ) ;
        //i = SetCommProp( hCom1, cModeString , 1 /*timeout*/ ) ;

        iFlagBootLoaderActived = 0 ;

        iChipConnectionSuccess = ConnectToChip() ;

        if(iChipConnectionSuccess==1)
        {
            Sleep(10) ;
            do
                iLen = GetPacketFromController( cReturn232 ) ;
            while(iLen>0) ;
            Sleep(10) ;

            //CloseSerialPort( hCom1 ) ;
            //hCom1 = OpenSerialPort( Tune.COMPORT_name ) ;
            i = SetCommProp( hCom1, cModeString , 1 /*timeout*/ ) ;
            Sleep(10) ;
            do
                iLen = GetPacketFromController( cReturn232 ) ;
            while(iLen>0) ;

            *cCommand232 = 13 ;
            i = WriteSerial(hCom1, cCommand232 , 1 ) ;
            Sleep(10) ;
            iLen = GetPacketFromController( cReturn232 ) ;

            if(strncmp(cReturn232,"\r\n>",3)!=0) iChipConnectionSuccess = 0 ;
        }

        fflush(stdout) ;

        /*i = SendCommandToController_LimitedWait((char*)"0TEST:") ;
        if( i==0 ) return 0 ;
        else if( strcmp(cReturn232,"XY-Motors controller v.  0.0") ==0 ) return 1 ;
        else return 0 ;*/

return i ;
}



void StopUnit()
{
        iFlagStop = 1 ;
}


void UnInitUnit()
{
        //  Cae?uaaai EII-ii?o
        CloseSerialPort( hCom1 ) ;
        hCom1 = INVALID_HANDLE_VALUE ;

}


void SaveUnitCfg()
{
    FILE *fp ;

    fp=fopen("port.cfg","wt") ;
    if(fp!=NULL)
    {
        fputs(Tune.COMPORT_name,fp);
        fclose(fp) ;
    }
}




/**************************************************************************************************/


void SetNextCmdSymbol()
{
        CmdBufferPtr++ ;
}


int GetOneValuesFromCommand(char *cOperand)
{
        int i=0 ;
        char *cPtr1, *cPtr2;


        //return 0;

        cPtr1=CmdBufferPtr;
        cPtr2=cOperand;
        while(*cPtr1==32 ) cPtr1++;	/*   ' ' "SPACE"  */
        if(*cPtr1==0x2D)				/*   '-'   */
        {
                *cPtr2=*cPtr1;
                cPtr1++; cPtr2++;
                i++;
        }

        //while( ( (*cPtr1>=0x30/*'0'*/) && (*cPtr1<=0x39/*'9'*/) ) &&  (i<20) )
        while( ( (*cPtr1!=':') && (*cPtr1!=',') ) && ( (*cPtr1!=' ') && (*cPtr1!=0x00)  && (i<20) ) )
        {
                *cPtr2=*cPtr1;
                cPtr1++; cPtr2++;
                i++;
        }

        if(i==20)  return -1;

        CmdBufferPtr=cPtr1;
        *cPtr2=0x00;

        return 1;
}



int GetTwoValuesFromCommand()
{
        int i=0 ;
        char *cPtr1, *cPtr2;


        cPtr1=CmdBufferPtr;
        cPtr2=cOperand1;
        while( ( (*cPtr1!=58/*':'*/) && (*cPtr1!=',') ) && ( (*cPtr1!=' ') && (*cPtr1!=0x00)  && (i<20) ) )
//	while( ( (*cPtr1!=/*','*/ 0x2c) && (*cPtr1!= 0x00) ) && (i<20) )
        {
                *cPtr2=*cPtr1;
                cPtr1++; cPtr2++;
                i++;
        }

        if((i==20) || (*cPtr1== 0x00))  return -1;
        *cPtr2=0x00;

        i=0;
        while( (*cPtr1==0x2c) || (*cPtr1==' ') )*cPtr1++;
        cPtr2=cOperand2;
        while( ( (*cPtr1!=58) && (*cPtr1!=' ') && (*cPtr1!=0x00) ) && (i<20) )
        //while( (cPtr1!=0x00) && (i<20) )
        {
                *cPtr2=*cPtr1;
                cPtr1++; cPtr2++;
                i++;
        }

        if((i==20) || (i==0))  return -1;
        *cPtr2=0x00;

        //*iOperand1=atol(cOperand1);
        //*iOperand2=atol(cOperand2);

        return 1;
}




int TryToGetTwoArguments()
{

        //SetNextCmdSymbol();
        if(GetOneValuesFromCommand(cOperand1)==-1)	return 0;

        if( (*CmdBufferPtr!=0x2c) && (*CmdBufferPtr!=' ') ) 	return 0;

        SetNextCmdSymbol();
        if(GetOneValuesFromCommand(cOperand2)==-1)	return 0;

        //if(*CmdBufferPtr!=58)	return 0;

        return 1;
}


int TryToGetOneArguments()
{

        //SetNextCmdSymbol();
        if(GetOneValuesFromCommand(cOperand1)==-1)	return 0;

        //if(*CmdBufferPtr!=58)	return 0;

        return 1;
}




static char iCommRet ;

int GetPacketFromController( char *cCommand232 )
{
        int i = 0, j ;
        char *cPtr = cCommand232 ;

        while( (j=ReadSerial(hCom1, cPtr,1 ) )>0 )
        {
                Sleep(1) ;
                cPtr+=j ;
                i+=j ;
                if(i>=128)
                {
                    cPtr = cReturn232 ;
                    cReturn232[i] = 0 ;
                    iRecievedSybbols = i ;
                    return i ;
                }
        };
        cReturn232[i] = 0 ;
        iRecievedSybbols = i ;
        return i ;
}





int GetPacketFromControllerLimited()
{
        int i = 0, j ;
        char *cPtr = cReturn232 ;

        while( (j=ReadSerial(hCom1, cPtr,1 ) )>0 )
        {
                //Sleep(1) ;
                cPtr+=j ;
                i+=j ;
                if(i>=127)
                {
                         cPtr = cReturn232 ;
                         cReturn232[i] = 0 ;
                         iRecievedSybbols = i ;
                         return i ;
                }
        };
        cReturn232[i] = 0 ;
        iRecievedSybbols = i ;
        return i ;
}





void ClearSerialFIFOBuffer()
{
    int i ;

    i=GetPacketFromController( cReturn232 ) ;
}





int SendCommandToController_nowait( char *str )
{
        int iLen, i ;

        strcpy(cCommand232,str) ;

        iLen = strlen( cCommand232 ) ;
        if(iLen==0) return 0 ;


        i = WriteSerial(hCom1, cCommand232 , iLen ) ;

        cReturn232[0] = 0 ;
        if(i<=0)
        {
            printf("\nError: Cannot write to port %s\n", Tune.COMPORT_name ) ;
        }
        //Sleep(2) ;
        //i = GetPacketFromController( cReturn232 ) ;

   return i ;
}




int SendCommandToController( char *str )
{
        int i = 0, iCount = 100 ;
        int TestCommandOK() ;


        //Sleep(2) ;
        //printf("        # %s \n",cCommand232 ) ;
        i=GetPacketFromController( cReturn232 ) ;
        do{
                i = SendCommandToController_nowait( str ) ;
                iCount--;
        }while( ( ( (cReturn232[0] == 21 ) || (cReturn232[0] == 24 ) ) || (i==0) ) && ( iCount>0 ) ) ;

        iCommRet = cReturn232[0] ;

        /*if(iCount==0)
        {
            void ErrorMessage();
            ErrorMessage();
            Sleep(100) ;
            return -1 ;
        }*/

        return i ;
}



int SendCommandToController_fast( char *str )
{
        int i = 0 ;
        int TestCommandOK() ;


        //Sleep(2) ;
        //printf("        # %s \n",cCommand232 ) ;
        //i=GetPacketFromController( cReturn232 ) ;
        do{

            i = SendCommandToController_nowait( str ) ;

        }while( ( (cReturn232[0] == 21 ) || (cReturn232[0] == 24 ) ) || (i==0) );


        return i ;
}




int SendCommandToController_LimitedWait( char *str )
{
        int i = 0, iCount = 100 ;
        int TestCommandOK() ;


        //Sleep(2) ;
        //printf("        # %s \n",cCommand232 ) ;
        i=GetPacketFromController( cReturn232 ) ;
        do{
                i = SendCommandToController_nowait( str ) ;
                iCount-- ;
        }while( ( ( (cReturn232[0] == 21 ) || (cReturn232[0] == 24 ) ) || (i==0) ) && ( iCount>0 ) ) ;


        return i ;
}


int SendCommandToController_long_wait( char *str )
{
        int i = 0 ;
        int TestCommandOK() ;


        //Sleep(2) ;
        //printf("        # %s \n",cCommand232 ) ;
        i=GetPacketFromController( cReturn232 ) ;
        do{
                i = SendCommandToController_nowait( str ) ;
                Sleep(1) ;
        }while( ( (cReturn232[0] == 21 ) || (cReturn232[0] == 24 ) ) || (i==0) );

        iCommRet = cReturn232[0] ;

        return i ;
}




int ConnectToChip()
{
        int iLen, i, iCount = 1000 ;


        //*cCommand232 = 0x0 ;
        for(i=0;i<100;i++) cCommand232[i] = 0x0 ;

        iLen = ReadSerial(hCom1, cReturn232 , 1 ) ;


        do{
            i = WriteSerial(hCom1, cCommand232 , 10 ) ;

            Sleep(12) ;
            cReturn232[0] = 0 ;
            //iLen = ReadSerial(hCom1, cReturn232 , 1 ) ;
            iLen = GetPacketFromController( cReturn232 ) ;

        }while((iLen<=0)&&((iCount--) > 0) ) ;

        if(iCount<=0)
        {
            //printf("\nError: Cannot connect to chip\n", Tune.COMPORT_name ) ;
            return 0 ;
        }
        //Sleep(2) ;
        if(strncmp(cReturn232,"\r\n>",3)!=0) return 0 ;

        do
            iLen = GetPacketFromController( cReturn232 ) ;
        while( iLen>0 ) ;


        uint32_t uiBouds = Tune.tty_mode.iBauds ;

        *cCommand232 = 0x42 ; // BOUDS SELECT
        cCommand232[1] = uiBouds & 0x000000ff ;
        uiBouds >>= 8 ;
        cCommand232[2] = uiBouds & 0x000000ff ;
        uiBouds >>= 8 ;
        cCommand232[3] = uiBouds & 0x000000ff ;
        uiBouds >>= 8 ;
        cCommand232[4] = uiBouds & 0x000000ff ;
        Sleep(2) ;
        i = WriteSerial(hCom1, cCommand232 , 5 ) ;
        //Sleep(200) ;
        //iLen = GetPacketFromController( cReturn232 ) ;
        //i = WriteSerial(hCom1, &cCommand232[1] , 4 ) ;
        Sleep(20) ;
        iLen = GetPacketFromController( cReturn232 ) ;
        //if(iLen>0) return 0 ;

   return 1 ;
}





int TestCommandOK()
{
        if( (iRecievedSybbols>0)&&(cReturn232[0] == 27) )
        {
                //printf(" OK") ;
                return 1 ;
        }

        printf("ERROR  in '%s'   code %d\n", cCommand232, cReturn232[0] ) ;
        iErrors++;
        return -1 ;
}

















void ConfigureAxis()
{
        FILE *fp ;


        fp=fopen("unit.cfg","rw") ;
        if(fp!=NULL)
        {

            fread(&Tune.COMPORT_name,sizeof(Tune.COMPORT_name),1,fp) ;
            fread(&Tune.tty_mode,sizeof(Tune.tty_mode),1,fp) ;

            fclose(fp) ;
        }

        if( hCom1 == INVALID_HANDLE_VALUE ) return ;


}




void SaveAxisConfigure()
{
        FILE *fp ;


        fp=fopen("unit.cfg","wb") ;
        if(fp!=NULL)
        {
            fwrite(&Tune.COMPORT_name,sizeof(Tune.COMPORT_name),1,fp) ;
            fwrite(&Tune.tty_mode,sizeof(Tune.tty_mode),1,fp) ;
            fclose(fp) ;
        }
}








void delay_ms( int time)
{

    Sleep(time) ;
}




int WriteMySerial(char *Bytes, int len)
{
    return WriteSerial( hCom1, Bytes,  len) ;
}


int ReadMySerial(char *Bytes, int len)
{
    return ReadSerial( hCom1, Bytes,  len) ;
}
