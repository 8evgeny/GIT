#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include <stdio.h>
#include <math.h>



#include "work_object.h"

#include <QLayout>
#include <QDockWidget>
#include <QFileDialog>
#include <QMessageBox>
#include <QInputDialog>
#include <QPaintEvent>
#include <QScrollBar>
#include <QAction>



#define EXTERN

#include "glob_var.h"
#include "unit_lib.h"


#ifdef PLATFORM_LINUX

#define INVALID_HANDLE_VALUE	-1

#include <unistd.h>
#endif



ProcessingThreadObject::ProcessingThreadObject(QObject *parent) :
    QObject(parent)
{

}










#ifdef PLATFORM_LINUX
#define BYTE    unsigned char
#define WORD    unsigned int
#define DWORD   unsigned long int
#endif

static char bufcurdir[512];
static char txdbuf[512];
static char rxdbuf[512];
static BYTE bufcod[0x20000];
static BYTE bufram[0x8000];
static BYTE buf_hexstr[530];
static BYTE buf_data_hex[256];
static BYTE bl_hex,btype_hex;
static WORD wadr_offs_hex;
static DWORD dwadr_seg_hex,dwadr_lineoffs_hex;
static DWORD dwadr_boot;
static int ilboot,ilcod;




BYTE GetHexByte(int ibuf)
{
BYTE bh,bl;
    bh = buf_hexstr[ibuf] - 0x30;
    if (bh>9)
        bh -= 7;
    bh <<= 4;
    bl = buf_hexstr[ibuf+1] - 0x30;
    if (bl>9)
        bl -= 7;
    return bh+bl;
}



int GetDataHex(void)
{
int i;
BYTE ks;
    if(buf_hexstr[0] != 0x3a)
        return	0;
    ks = 0;
    bl_hex = GetHexByte(1);
    wadr_offs_hex = ((WORD)GetHexByte(3)<<8)+GetHexByte(5);
    btype_hex = GetHexByte(7);
    ks = bl_hex + btype_hex + (wadr_offs_hex>>8) + wadr_offs_hex;
    for(i=0;i<bl_hex+1;i++)
    {
        buf_data_hex[i] = GetHexByte(2*i+9);
        ks += buf_data_hex[i];
    }
    if(ks!=0)
        return 0;
    if(btype_hex == 2)
        dwadr_seg_hex =	(((WORD)GetHexByte(9)<<8)+GetHexByte(11))<<4;
    if(btype_hex == 4)
        dwadr_lineoffs_hex = (((WORD)GetHexByte(9)<<8)+GetHexByte(11))<<16;

    return 1;
}





int GetHexCod(char *filename)
{
int i,nb;
FILE *file;
char strfn[512];
char chd;
DWORD	dwadr;
/*
    for(i=0;i<sizeof(bufcod);i++)
    {
        bufcod[i] = i;
    }
    ilcod = 512*0x100;
    return 1;
*/
    dwadr_seg_hex = 0;
    dwadr_lineoffs_hex = 0;
    for(i=0;i<sizeof(bufcod);i++)
    {
        bufcod[i] = 0xff;
    }

    file=fopen(filename,"rt") ;
    if(!file)
        return 0;
    nb = 1;
    while(nb == 1)
    {
        i = 0;
        do
        {
            //nb = file.Read(&chd,1);
            nb = fread(&chd,1,1,file);
            buf_hexstr[i] = chd;
            i++;
        }
        while(chd!='\n');
        if(nb != 1)
        {
            fclose(file);
            for(i=(sizeof(bufcod)-1);i>=0;i--)
            {
                if(bufcod[i] != 0xff)
                    break;
            }
            if(i & 0xff)
                i = ((i + 0x100) & 0xffffff00);
            ilcod = i;
            return 1;
        }
        if(!GetDataHex())
        {
            fclose(file);
            return 0;
        }
        if(btype_hex == 0)
        {
            dwadr = dwadr_lineoffs_hex + dwadr_seg_hex + wadr_offs_hex;
            if((dwadr<0x08000000)||((dwadr+bl_hex)>0x08020000))
            {
                fclose(file);
                return 0;
            }
            dwadr -= 0x08000000;
            for(i=0;i<bl_hex;i++)
                bufcod[dwadr+i] = buf_data_hex[i];
        }
    }
    return 0;
}





int GetBinCod(char *filename)
{
int i,nb;
FILE *file;
char strfn[512];
char chd;
unsigned char *cPtr ;
DWORD	dwadr;
/*
    for(i=0;i<sizeof(bufcod);i++)
    {
        bufcod[i] = i;
    }
    ilcod = 512*0x100;
    return 1;
*/
    dwadr_seg_hex = 0;
    dwadr_lineoffs_hex = 0;
    for(i=0;i<sizeof(bufcod);i++)
    {
        bufcod[i] = 0xff;
    }

    file=fopen(filename,"rb") ;
    if(!file)
        return 0;

    cPtr = bufcod ;
    i = 0 ;
    do{

        nb = fread(cPtr,1,1,file);
        if( nb>0)
        {
            i++ ;
            cPtr++ ;
        }

    }while(feof(file)==0) ;

    fclose(file) ;

    ilcod = i;

    return 1 ;
}



int open_boot_code()
{
int i,nb;
FILE *file;
char strfn[512];
char chd;

DWORD	dwadr;
    dwadr_seg_hex = 0;
    dwadr_lineoffs_hex = 0;
    for(i=0;i<sizeof(bufram);i++)
    {
        bufram[i] = 0xff;
    }
    strcpy(strfn,"1986_BOOT_UART.hex") ;

    //strcpy(strfn, bufcurdir) ;
    //strcat(strfn, strfn ) ;

    file=fopen(strfn,"rt");
    if(!file) return 0;
    nb = 1;
    while(nb == 1)
    {
        i = 0;
        do
        {
            nb = fread(&chd,1,1,file);
            buf_hexstr[i] = chd;
            i++;
        }
        while(chd!='\n');
        if(nb != 1)
        {
            fclose(file);
            for(i=0;i<sizeof(bufram);i++)
            {
                if(bufram[i] != 0xff)
                    break;
            }
            dwadr_boot = i;
            for(i=(sizeof(bufram)-1);i>=0;i--)
            {
                if(bufram[i] != 0xff)
                    break;
            }
            ilboot = (i+16 - dwadr_boot) & 0xfffffff7;;
            return 1;
        }
        if(!GetDataHex())
        {
            fclose(file);
            return 0;
        }
        if(btype_hex == 0)
        {
            dwadr = dwadr_lineoffs_hex + dwadr_seg_hex + wadr_offs_hex;
            if((dwadr<0x20000000)||((dwadr+bl_hex)>0x20008000))
            {
                fclose(file);
                return 0;
            }
            dwadr -= 0x20000000;
            for(i=0;i<bl_hex;i++)
                bufram[dwadr+i] = buf_data_hex[i];
        }
    }
    return 0;
}













void ProcessingThreadObject::Work(int iOperation)
{

    switch(iOperation)
    {
    case IDM_READ_FLASH:  Sleep(1000) ; break ;

    case IDM_ERASE_FLASH:
        if( iFlagBootLoaderActived <= 0 )
            iFlagBootLoaderActived = LoadBootBinToChip() ;
        if( iFlagBootLoaderActived == 1 )
            Erase() ;
        break ;

    case IDM_RUN:
        if( iFlagBootLoaderActived <= 0 )
            iFlagBootLoaderActived = LoadBootBinToChip() ;
        if( iFlagBootLoaderActived == 1 )
            Run() ;
        break ;

    case IDM_WRITE_FLASH:
        //usleep(1000000) ;
        if( iFlagBootLoaderActived <= 0 )
            iFlagBootLoaderActived = LoadBootBinToChip() ;
        if( iFlagBootLoaderActived == 1 )
            if(Erase())
            {
                ProgramHexFileToChip( filename ) ;

                if((Tune.iUserMode&DEF_MODE_VERIFY)!=0)
                    VerifyHexFileToChip( filename ) ;
                if((Tune.iUserMode&DEF_MODE_RUN)!=0)
                    Run() ;
            }
        break ;


    case IDM_VERIFY:
        //usleep(1000000) ;
        if( iFlagBootLoaderActived == 0 )
            iFlagBootLoaderActived = LoadBootBinToChip() ;
        if( iFlagBootLoaderActived == 1 )
            VerifyHexFileToChip( filename ) ;
        break ;
    default : ;
    }


    emit signalWorkEnd() ;

    return ;
}





int ProcessingThreadObject::LoadBootBinToChip()
{
        int iLen, ires, i, j, iCount = 1000 ;
        unsigned int uiAddress ;
        bool f ;

        QString str ;

        if(open_boot_code()!=1) return -1;
        uiAddress = dwadr_boot ;
        iLen = ilboot ;


        uint32_t uiLenght = iLen ;

        *cCommand232 = 0x4C ; // LOAD
        cCommand232[1] = uiAddress & 0x000000ff ;
        uiAddress >>= 8 ;
        cCommand232[2] = uiAddress & 0x000000ff ;
        uiAddress >>= 8 ;
        cCommand232[3] = 0x00 ;//uiAddress & 0x000000ff ;
        uiAddress >>= 8 ;
        cCommand232[4] = 0x20 ;//uiAddress & 0x000000ff ;

        cCommand232[5] = uiLenght & 0x000000ff ;
        uiLenght >>= 8 ;
        cCommand232[6] = uiLenght & 0x000000ff ;
        uiLenght >>= 8 ;
        cCommand232[7] = uiLenght & 0x000000ff ;
        uiLenght >>= 8 ;
        cCommand232[8] = uiLenght & 0x000000ff ;
        Sleep(2) ;
        i = WriteMySerial( cCommand232 , 9 ) ;

        Sleep(4) ;
        i = GetPacketFromController( cReturn232 ) ;
        if(i==0) return 0 ;

        if(*cReturn232!='L') return 0 ;

        WriteMySerial( (char*)(bufram+dwadr_boot) , iLen ) ;

        Sleep(40) ;
        do
            i = GetPacketFromController( cReturn232 ) ;
        while( i == 0 ) ;
        if(*cReturn232!='K') return 0 ;

        Sleep(4) ;


        for(i=0;i<(ilboot>>3);i++)
        {
            txdbuf[0] = 'Y';
            txdbuf[1] = (dwadr_boot+8*i) & 0xff;
            txdbuf[2] = ((dwadr_boot+8*i)>>8) & 0xff;
            txdbuf[3] = 0;
            txdbuf[4] = 0x20;
            txdbuf[5] = 8;
            txdbuf[6] = 0;
            txdbuf[7] = 0;
            txdbuf[8] = 0;
            WriteMySerial(txdbuf,9);
            f = true;
            Sleep(4) ;
            if((ReadMySerial( rxdbuf,10))&&(rxdbuf[0]=='Y')&&(rxdbuf[9]=='K'))
            {
                for(j=0;j<8;j++)
                {
                    if(rxdbuf[j+1] != (char)bufram[dwadr_boot+8*i+j])
                        f= false;
                }
            }
            else
                f= false;
            if(!f)
            {
                str = "IO Error";
                emit signalUpdateStatusBar(str) ;
               return 0;
            }
        }



        Sleep(40) ;

        uiAddress = dwadr_boot ;
        *cCommand232 = 0x52 ; // RUN
        cCommand232[1] = uiAddress & 0x000000ff ;
        uiAddress >>= 8 ;
        cCommand232[2] = uiAddress & 0x000000ff ;
        uiAddress >>= 8 ;
        cCommand232[3] = 0x00 ;
        uiAddress >>= 8 ;
        cCommand232[4] = 0x20 ;
        i = WriteMySerial( cCommand232 , 5 ) ;
        Sleep(4) ;
        i = GetPacketFromController( cReturn232 ) ;
        if(i==0) return 0 ;
        if(*cReturn232!='R') return 0 ;

        Sleep(400) ;
        *cCommand232 = 'I' ;
        i = WriteMySerial( cCommand232 , 1 ) ;
        Sleep(400) ;
        i = GetPacketFromController( cReturn232 ) ;

        if(strncmp(cReturn232,"1986BOOTUART",12)!=0) return 0 ;

   return 1 ;
}










bool ProcessingThreadObject::Program(void)
{
int i,j;
    BYTE ks;
    QString str ;

    emit signalUpdateProgressBarRange(ilcod>>8) ;
    str.sprintf("Program %i byte...",ilcod) ;
    emit signalUpdateStatusBar(str) ;
    emit signalUpdateProgressBar(0) ;

    txdbuf[0] = 'A';
    txdbuf[1] = 0x00;
    txdbuf[2] = 0x00;
    txdbuf[3] = 0x00;
    txdbuf[4] = 0x08;
    WriteMySerial(txdbuf,5);
    Sleep(2) ;
    if((!ReadMySerial(rxdbuf,1))||(rxdbuf[0]!=0x08))
    {
        str = "IO Error";
        emit signalUpdateStatusBar(str) ;
        return 0;
    }
    txdbuf[0] = 'P';
    for(i=0;i<(ilcod>>8);i++)
    {
#ifdef PLATFORM_LINUX
        Sleep(7) ;
#endif
#ifdef PLATFORM_WINDOWS
        Sleep(1) ;
#endif

        WriteMySerial(txdbuf,1);
        WriteMySerial((char *)(bufcod+(i<<8)),256);
        ks =0;
        for(j=0;j<256;j++)
            ks += bufcod[j+(i<<8)];

#ifdef PLATFORM_LINUX
        Sleep(30) ;
#endif
#ifdef PLATFORM_WINDOWS
        Sleep(1) ;
#endif
        if((!ReadMySerial(rxdbuf,1))||((BYTE)rxdbuf[0]!=ks))
        {
            str = "IO Error";
            emit signalUpdateStatusBar(str) ;
            return 0;
        }
        emit signalUpdateProgressBar(i+1);
    }
    str = "Program Done";
    emit signalUpdateStatusBar(str) ;

    return 1;
}



bool ProcessingThreadObject::Verify(void)
{
    int i,j,k, ires;
    QString str ;

    emit signalUpdateProgressBarRange(ilcod>>8) ;
    str.sprintf("Verify %i byte...",ilcod) ;
    emit signalUpdateStatusBar(str) ;
    emit signalUpdateProgressBar(0) ;

    txdbuf[0] = 'A';
    txdbuf[1] = 0x00;
    txdbuf[2] = 0x00;
    txdbuf[3] = 0x00;
    txdbuf[4] = 0x08;
    WriteMySerial(txdbuf,5);
    Sleep(50) ;
    if((!ReadMySerial(rxdbuf,1))||(rxdbuf[0]!=0x08))
    {
        str = "IO Error";
        emit signalUpdateStatusBar(str) ;
        return 0;
    }
    Sleep(50) ;
    txdbuf[0] = 'V';
    for(i=0;i<(ilcod>>8);i++)
    {
        for(j=0;j<32;j++)
        {
            WriteMySerial(txdbuf,1);

            do
                ires = ReadMySerial(rxdbuf,8) ;

            while(ires<=0) ;


            if(ires!=8)
            {
                str = "IO Error";
                emit signalUpdateStatusBar(str) ;
                return 0;
            }
            for(k=0;k<8;k++)
            {
                if((BYTE)rxdbuf[k] != bufcod[k+(j<<3)+(i<<8)])
                {

                    str.sprintf("Verify adr=0x%08x dataw=0x%02x datar=0x%02x",
                        0x08000000+k+(j<<3)+(i<<8),bufcod[k+(j<<3)+(i<<8)],(BYTE)rxdbuf[k]);
                    emit signalUpdateStatusBar(str) ;
                    return 0;
                }
            }
        }
        emit signalUpdateProgressBar(i+1);
    }

    str = "Verify Done";
    emit signalUpdateStatusBar(str) ;

    return 1;
}



int ProcessingThreadObject::ProgramHexFileToChip(char *filename)
{
    char *cPtr ;

    cPtr = strchr(filename,'.') ;
    if( cPtr == NULL ) cPtr = filename ;

    if((strncmp(cPtr,".hex",4) == 0 )||(strncmp(cPtr,".HEX",4) == 0 ) )
    {
        if( GetHexCod(filename) == 0 ) return 0 ;
    }
    else
    {
        if( GetBinCod(filename) == 0 ) return 0 ;
    }


    Program() ;

    return 0 ;
}


int ProcessingThreadObject::VerifyHexFileToChip(char *filename)
{
    char *cPtr ;

    cPtr = strchr(filename,'.') ;
    if( cPtr == NULL ) cPtr = filename ;

    if((strncmp(cPtr,".hex",4) == 0 )||(strncmp(cPtr,".HEX",4) == 0 ) )
    {
        if( GetHexCod(filename) == 0 ) return 0 ;
    }
    else
    {
        if( GetBinCod(filename) == 0 ) return 0 ;
    }


    Verify() ;

    return 0 ;
}





void ProcessingThreadObject::Run()
{
    QString str ;

    txdbuf[0] = 'R';
    WriteMySerial(txdbuf,1);
    Sleep(100) ;
    if	((!ReadMySerial(rxdbuf,1))||(rxdbuf[0]!='R'))
    {
        str = "IO Error";
        emit signalUpdateStatusBar(str) ;
    }
    else
    {
        str = "Run at 0x08000000 OK!";
        emit signalUpdateStatusBar(str) ;
    }

    return;
}




bool ProcessingThreadObject::Erase(void)
{
    DWORD	adr,data;
    QString str ;

    emit signalUpdateProgressBarRange(ilcod>>8) ;
    str.sprintf("Erase...") ;
    emit signalUpdateStatusBar(str) ;
    emit signalUpdateProgressBar(0) ;

    txdbuf[0] = 'E';
    WriteMySerial(txdbuf,1);
    Sleep(1000);
    if	((!ReadMySerial(rxdbuf,9))||(rxdbuf[0]!='E'))
    {
        str = "IO Error";
        emit signalUpdateStatusBar(str) ;
        return 0;
    }
    adr = (BYTE)rxdbuf[1] + (((BYTE)rxdbuf[2])<<8) + (((BYTE)rxdbuf[3])<<16)
        + (((BYTE)rxdbuf[4])<<24);
    data = (BYTE)rxdbuf[5] + (((BYTE)rxdbuf[6])<<8) + (((BYTE)rxdbuf[7])<<16)
        + (((BYTE)rxdbuf[8])<<24);

    if((adr == 0x08020000)&&(data == 0xffffffff))
    {
         str = "Full chip erase done!";
        emit signalUpdateStatusBar(str) ;
        return 1;
    }

    str.sprintf("Full chip erase failed adr=0x%08x data=0x%08x",adr,data);
    emit signalUpdateStatusBar(str) ;
    return 0;
}
