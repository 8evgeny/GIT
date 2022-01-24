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
 *  File main.cpp: Main function
 *
 *===========================================================================*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "MCUFmLoader.h"
#include "FmConverter.h"
#include "common.h"
#include "mcus.h"

//MCU model
#define ARG_MCU         "-mcu"
//Destination address at MCU flash memory
#define ARG_FMADDR      "-fmaddr"
//Destination address at MCU RAM memory for loader executable code
#define ARG_RAMADDR     "-ramaddr"
//Port index
#define ARG_PORT        "-port"
//Requsted port speed
#define ARG_IOSPEED     "-iospeed"
//Data flow control
#define ARG_IOFLOW      "-flow"
//Read data timeout, ms
#define ARG_RDTM	    "-rdtm"
//Firmware file
#define ARG_FILE        "-file"
//Firmware file format
#define ARG_FMT         "-fmt"
//Firmware file
#define ARG_LDFILE      "-ldfile"
//Firmware file format
#define ARG_LDFMT       "-ldfmt"
//Erase flash memory
#define ARG_ERASE       "-erase"
//Write flash memory
#define ARG_WRITE       "-write"
//Verify data at flash memory
#define ARG_VERIFY      "-verify"
//Start MCU
#define ARG_START		"-start"
//Convert loader executable code to C-array
#define ARG_LDCONV  	"-ldconv"
//Print help
#define ARG_HELP		"--help"
//Print list of supported MCUs
#define ARG_LIST		"--list"
//Read data (from MCU) witihin serial port
#define ARG_RDATA		"--rdata"

//ARG_IOFLOW valid argument values
#define STR_NONE        "none"
#define STR_RTSCTS      "rtscts"
#define STR_XONXOFF     "xonxoff"

/** */
#define IsArg(name)     (strcmp(argName, name) == 0)

/** */
#define IsHex(val)		(val != 0 && strlen(val) > 2 && \
						 val[0] == '0' && (val[1] == 'x' || val[1] == 'X'))

/** */
#define PrintArgError(code) { lastErrCode = code;\
							  printf("Key '%s', ", argName);\
                              PrintErrMsg(code); }

/** Convert executable code from specified format to C-array */
int FmFileToCArray(const char *mcuModel, const char *file, const char *format);

/** Print help */
void PrintHelp(const char *name)
{
	char fname[MAX_FILENAME_LENGTH];

    GetFileName(name, fname, 0);

	printf("Usage:\n");
    printf("\t%s %s model [%s path %s name] [%s path %s name]\n"
           "\t\t\t[%s addr] [%s addr]\n"
           "\t\t\t[%s n] [%s n] [%s n]\n"
           "\t\t\t[%s] [%s] [%s] [%s]\n",
           fname,
           ARG_MCU, ARG_FILE, ARG_FMT, ARG_LDFILE, ARG_LDFMT,
           ARG_FMADDR, ARG_RAMADDR,
           ARG_PORT, ARG_IOSPEED, ARG_RDTM,
           ARG_ERASE, ARG_WRITE, ARG_VERIFY, ARG_START);

    printf("\t%s %s model %s name %s path\n",
            fname, ARG_MCU, ARG_LDFMT, ARG_LDCONV);

    printf("\t%s [%s n] [%s n] [%s ctrl] --rdata\n",
            fname, ARG_PORT, ARG_IOSPEED, ARG_IOFLOW);

    printf("\t%s %s\n", fname, ARG_HELP);
    printf("\t%s %s\n", fname, ARG_LIST);
	printf("\n\tLoad firmware into MCU flash memory using serial port.\n");
	printf("Options:\n");
    printf("\t%s:        print help and exit.\n", ARG_HELP);
    printf("\t%s:        print list of supported MCUs and exit.\n", ARG_LIST);
    printf("\t%s:       read/write data within serial port.\n", ARG_RDATA);
    printf("\t%s model:    select target MCU (Do %s %s to get list of supported MCUs).\n",
            ARG_MCU, fname, ARG_LIST);
    printf("\t%s path:    firmware file to load into MCU flash memory.\n", ARG_FILE);
    printf("\t%s name:     firmware file format: [%s, %s]. Default: %s\n",
            ARG_FMT, FFMTNM_IHEX, FFMTNM_BINARY, DEFAULT_FORMAT_NAME);
    printf("\t%s path:  loader file.\n", ARG_LDFILE);
    printf("\t%s name:   loader file format: [%s, %s]. Default: %s\n",
            ARG_LDFMT, FFMTNM_IHEX, FFMTNM_BINARY, DEFAULT_FORMAT_NAME);
    printf("\t%s addr:  select destination address of firmware at MCU flash memory\n",
            ARG_FMADDR);
    printf("\t%s addr: select destination address of loader code at MCU RAM\n",
            ARG_RAMADDR);

    #ifdef __WINDOWS__
    printf("\t%-10s n: select serial port: [0,1,...] => [COM1,COM2,...]. Default: %i\n",
                            ARG_PORT, DEFAULT_PORT);
    #else
    printf("\t%-10s n:  select serial port: \n"
           "\t\t\t[0..3]       => ttyS[0..3]\n"
           "\t\t\t[\"u0\"..\"u7\"] => ttyUSB[0..7]\n"
           "\t\t\t[\"a0\"..\"a7\"] => ttyACM[0..7]\n"
           "\t\t\tDefault: %i\n",
                            ARG_PORT, DEFAULT_PORT);
    #endif /* __WINDOWS__ */

    printf("\t%s n:    select serial port speed: [%i..%i]. Default: %i\n",
            ARG_IOSPEED, IOSPEED_MIN, IOSPEED_MAX, DEFAULT_IOSPEED);
    printf("\t%s ctrl:    set data flow control: [%s, %s, %s]\n",
            ARG_IOFLOW, STR_NONE, STR_RTSCTS, STR_XONXOFF);
    printf("\t%s n:       set read data timeout, ms. Default: %i\n",
            ARG_RDTM, PORT_READ_TIMEOUT);
    printf("\t%s:        erase flash memory\n", ARG_ERASE);
    printf("\t%s:        erase and write flash memory\n", ARG_WRITE);
    printf("\t%s:       verify data into flash memory\n", ARG_VERIFY);
    printf("\t%s:        start MCU\n", ARG_START);
    printf("\t%s path:  converts loader file to C array and stores "
           "it to file with adding \".cpp\" suffix\n", ARG_LDCONV);
    printf("Author:\n\tMarusenkov S.A. <ostfriesland@mail.ru> 2017-2020\n");
}



/** Print MCUs list */
void PrintMCUList()
{
	const MCUParams *m;
	int i = 0;
	printf("Supported MCUs:\n");
	printf("%-10s %-16s %-10s %-10s\n", "ID", "Name", "FM addr", "RAM addr");
	while((m = mcus[i++]) != 0)
	{
		printf("%-10i %-16s 0x%08X 0x%08X\n",
		m->id,
		m->name,
		m->fmBaseAddr,
		m->ramBaseAddr);
	}	
}



/** Main function */
int main(int argc, char* argv[])
{        
        int i, lastErrCode, errCode, isArgRdata;
        const char *argName;
        const char *argVal;
        const char *fmFile;
		const char *ldFile;
		const char *fmFormatName;
		const char *ldFormatName;
		const char *mcuModel;
        int fmFlags;
        MCUFmLoader fml;

        //fml = new MCUFmLoader;

		//Default values
		ldFile = 0;
		ldFormatName = 0;

		fmFile       = 0;
		fmFormatName = 0;
		fmFlags      = 0;

		mcuModel	 = 0;       

        isArgRdata   = 0;

        //Check count of arguments
        if (argc == 1)
        {
            PrintHelp(argv[0]);
            return EINVPARAM;
        }

		//Parse arguments
        i = 1;
		errCode = ESUCCESS;
		lastErrCode = ESUCCESS;
        while(i < argc)
        {
                argName = argv[i++];          

				if (argName[0] == '-')
                {                                        
					if (i < argc && argv[i][0] != '-')
                        argVal = argv[i++];
                    else
                        argVal = 0;                   
                }
                else
                    continue;				

				//Print help
				if (IsArg(ARG_HELP))
				{
					PrintHelp(argv[0]);
					return 0;
				}
				//Print MCU list
				else if (IsArg(ARG_LIST))
				{
					PrintMCUList();
					return 0;
				}
				//Read data from port
				else if (IsArg(ARG_RDATA))
				{
                    isArgRdata = 1;
				}
				//Convert loader executabel codes to C-array (none-documented)
				else if (IsArg(ARG_LDCONV))
				{
					printf("Convert to C array...\n");
					errCode = FmFileToCArray(mcuModel,
											  argVal,
											  ldFormatName);
					PrintErrMsgPos(COLUMN_PROGRESS, errCode);

					return errCode;
				}
				//MCU model
				else if (IsArg(ARG_MCU))
                {
                    mcuModel = argVal;
                    if ((errCode = fml.SetMCUModel(argVal)) != ESUCCESS)
                        PrintArgError(errCode)                   
                }
                //Destination address at MCU flash memory
                else if (IsArg(ARG_FMADDR))
                {
					int s = (IsHex(argVal)) ? 2 : 0;
                    if ((errCode = fml.SetMCUFMAddr(StrToU32(&argVal[s], s))) != ESUCCESS)
						PrintArgError(errCode)                    
                }
                //Destination address at MCU RAM memory for loader executable code
                else if (IsArg(ARG_RAMADDR))
                {
					int s = IsHex(argVal) ? 2 : 0;
                    if ((errCode = fml.SetMCURAMAddr(StrToU32(&argVal[s], s))) != ESUCCESS)
						PrintArgError(errCode)
                }
                //Port index
                else if (IsArg(ARG_PORT))
                {
                    //Parse port number
                    int port;

                    #ifdef __WINDOWS__
                    //Windows version: COMx
                    port = atoi(argVal);

                    #else
                    //Linux version: ttySx, ttyUSBx, ttyACMx
                    if (argVal[0] == 'u')
                        port = atoi(&argVal[1]) + 4;  //["u0".."u7"] => ttyUSB[0..7] => port [4..11]
                    else if (argVal[0] == 'a')
                        port = atoi(&argVal[1]) + 12; //["a0".."a7"] => ttyACM[0..7] => port [12..19]
                    else
                        port = atoi(argVal); //["0".."3"] => ttyS[0..3] => port [0..3]
                    #endif

                    if ((errCode = fml.SetIOPort(port)) != ESUCCESS)
						PrintArgError(errCode)
                }
                //Requsted port speed
                else if (IsArg(ARG_IOSPEED))
                {
                    if ((errCode = fml.SetIOSpeed(atoi(argVal))) != ESUCCESS)
						PrintArgError(errCode)
                }
                //Data flow control
                else if (IsArg(ARG_IOFLOW))
                {
                    if (strcmp(argVal, STR_RTSCTS) == 0)
                        fml.SetIOCtrl(CTRL_RTSCTS);
                    else if (strcmp(argVal, STR_XONXOFF) == 0)
                        fml.SetIOCtrl(CTRL_XONXOFF);
                    else
                        fml.SetIOCtrl(CTRL_NONE);
                }
				//Read data timeout
				else if (IsArg(ARG_RDTM))
				{
                    if ((errCode = fml.SetRdTimeout(atoi(argVal))) != ESUCCESS)
						PrintArgError(errCode)
				}
				//Firmware file
				else if (IsArg(ARG_LDFILE))
					ldFile = argVal;
				//Firmware file format
				else if (IsArg(ARG_LDFMT))
					ldFormatName = argVal;
                //Firmware file
                else if (IsArg(ARG_FILE))                
                    fmFile = argVal;					                
                //Firmware file format
				else if (IsArg(ARG_FMT))
					fmFormatName = argVal;
                //Flags
                else if (IsArg(ARG_ERASE))
                    fmFlags |= FmErase;
                else if (IsArg(ARG_WRITE))
                    fmFlags |= FmWrite;
                else if (IsArg(ARG_VERIFY))					
                    fmFlags |= FmVerify;
				else if (IsArg(ARG_START))
					fmFlags |= FmStart;
                //Unknown key used
				else
				{
					SetGraphic(AOFF, CYELLOW, CNONE);
                    printf("Unknown key '%s', skipped\n", argName);				
					ResetGraphic();
				}
        } //while(i < argc)        

        //Read/write user-defined data
        if (isArgRdata)
            return fml.ReadPortData();

		//Set loader file
        if (ldFile && (errCode = fml.SetLdFile(ldFile, ldFormatName)) != ESUCCESS)
        {
			argName = (errCode == EINVFORMAT) ? ARG_LDFMT : ARG_LDFILE;
			PrintArgError(errCode)
		}        

        //Set firmware file
		if ((fmFlags & (FmWrite|FmVerify)) &&
            (errCode = fml.SetFmFile(fmFile, fmFormatName)) != ESUCCESS)
        {            
			argName = (errCode == EINVFORMAT) ? ARG_FMT : ARG_FILE;
			PrintArgError(errCode)
        }      

		//Start programming flash or exit with error
		if (lastErrCode == ESUCCESS)
		{			
            errCode = fml.ProgramFlash(fmFlags);
		}
        else
        {
			printf("One or more arguments have invalid values, exit\n");
			errCode = lastErrCode;
		}

		//Free resources
        fml.FreeResources();

		return errCode;
}

/*===========================================================================
 * End of file main.cpp
 *===========================================================================*/
