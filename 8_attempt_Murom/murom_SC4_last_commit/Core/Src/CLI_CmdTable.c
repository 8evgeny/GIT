/**
  ******************************************************************************
  * @file    CLI_CmdTable.c
  * @author  mixa@@ETS
  * @brief   This file contains CLI command table module implementation
  ******************************************************************************
  */

#define _TABLECOMMAND_

#include <stdlib.h>
#include <string.h>

#include "CLI_io.h"
#include "CLI_CmdProcess.h"
#include "CLI_CmdDeviceFn.h"
#include "CLI_CmdTestFn.h"
#include "CLI_CmdServiceFn.h"
#include "connect_manager.h"
#include "CLI_CmdTable.h"

/** @addtogroup CLI_CmdTable
  * @{
  */

/** @defgroup CLI_CmdTable_tbl CLI command strings
  * @brief CLI command strings
  * @{
  */
//список команд тестирования
const char cmdTestOn[]="TEST ON";
const char cmdTestOff[]="TEST OFF";
const char cmdTestSRAMOn[]="TEST SRAM";
const char cmdTestEEPROMOn[]="TEST EEPROM";
const char cmdTestCodecOn[]="TEST AIC";
const char cmdTestPHYOn[]="TEST PHY";
const char cmdTestUI[]="TEST UI";
const char cmdTestLEDOn[]="TEST LED ON";
const char cmdTestLEDOff[]="TEST LED OFF";
const char cmdTestBTNOn[]="TEST BTN ON";
const char cmdTestBTNOff[]="TEST BTN OFF";
const char cmdTestALoopOn[]="TEST AUDIO ON";
const char cmdTestALoopOff[]="TEST AUDIO OFF";
const char cmdSetPin[]="TEST SET PIN";  // nameS , data1
const char cmdGetPin[]="TEST GET PIN";  // nameS
const char cmdSetAIC[]="TEST SET AIC";  // reg8 , data8
const char cmdGetAIC[]="TEST GET AIC";  // reg8
const char cmdSetETH[]="TEST SET ETH";  // reg8 , data16
const char cmdGetETH[]="TEST GET ETH";  // reg8
const char cmdSetLED[]="TEST SET LED";  // led8
const char cmdGetBTN[]="TEST GET BTN";  // btn8
const char cmdSetEC[]="TEST SET EC";  
const char cmdGetEC[]="TEST GET EC";  

const char cmdTestTone[]="TEST TONE";  //  idx8 , len16
const char cmdTestPrn[]="TEST PRINT";

const char cmdDebugOn[]="DEBUG ON";
const char cmdDebugOff[]="DEBUG OFF";
const char cmdServiceOn[]="SERVICE ON";
const char cmdServiceOff[]="SERVICE OFF";

//список команд для прошивки и конфигурирования
const char cmdSetBlock[]="SET BLOCK";
const char cmdGetBlock[]="GET BLOCK";
//список команд для прошивки и конфигурирования
const char cmdConfigOn[]="CONFIG ON";
const char cmdConfigStartWrite[]="CONFIG START WRITE";
const char cmdConfigOff[]="CONFIG OFF";
const char cmdFmWareOn[]="FMWARE ON";
const char cmdFmWareStartWrite[]="FMWARE START WRITE";
const char cmdFmWareOff[]="FMWARE OFF";

//список сервисных команд
const char cmdGetType[]="DEVICE GET CFGUI";
const char cmdSetResetDevice[]="DEVICE RESET";
const char cmdSetFWInfo[]="DEVICE SETFW";
const char cmdGetFWInfo[]="DEVICE FWINFO";

const char cmdConnect[]="CONNECT";
const char cmdHelp[]="HELP";

extern const char cmdIPSPING[];
extern const char cmdIPSPONG[];
extern const char cmdINVITE[];
extern const char cmdACK[];
extern const char cmdINVITEACK[];
extern const char cmdBYE[];
extern const char cmdLOG[];
/**
  * @}
  */

/**
  * @brief CLI commands table
  */
const sCLI_cmd_desc cmd_tab[]={
	{&cmdTestOn[0] , fnStartTestMode, 0},
	{&cmdTestOff[0] , fnStopTestMode, 0},
	{&cmdDebugOn[0] , fnStartDebugMode, 0},
	{&cmdDebugOff[0] , fnStopDebugMode, 0},
	{&cmdServiceOn[0] , fnStartServiceMode, 0},
	{&cmdServiceOff[0] , fnStopServiceMode, 0},

	{&cmdTestSRAMOn[0] , fnTestSRAM, 0},
	{&cmdTestEEPROMOn[0] , fnTestEEPROM, 0},
	{&cmdTestCodecOn[0] , fnTestCODEC, 0},
	{&cmdTestPHYOn[0] , fnTestPHY, 0},

	{&cmdTestUI[0] , fnTestUI, 0},
	{&cmdTestLEDOn[0] , fnTestLEDOn, 0},
	{&cmdTestLEDOff[0] , fnTestLEDOff, 0},
	{&cmdTestBTNOn[0] , fnTestBTNOn, 0},
	{&cmdTestBTNOff[0] , fnTestBTNOff, 0},
	{&cmdTestALoopOn[0] , fnTestAudioLoopOn, 0},
	{&cmdTestALoopOff[0] , fnTestAudioLoopOff, 0},

	{&cmdTestTone[0] , fnTestTone, 2},
	{&cmdTestPrn[0] , fnTestPrn, 0},

	{&cmdSetPin[0] , fnSetPIN, 2},
	{&cmdGetPin[0] , fnGetPIN, 1},
	{&cmdSetLED[0] , fnSetLED, 2},
	{&cmdGetBTN[0] , fnGetBTN, 1},

	{&cmdSetAIC[0] , fnSetAIC, 3},
	{&cmdGetAIC[0] , fnGetAIC, 2},
	{&cmdSetETH[0] , fnSetETH, 2},
	{&cmdGetETH[0] , fnGetETH, 1},

	{&cmdSetEC[0] , fnSetEC, 6},
	{&cmdGetEC[0] , fnGetEC, 0},
	
	{&cmdConfigOn[0] , fnConfigOn, 0},
	{&cmdConfigStartWrite[0] , fnConfigStartWrite, 0},
	{&cmdConfigOff[0] , fnConfigOFF, 0},

	{&cmdFmWareOn[0] , fnFmwareOn, 0},
	{&cmdFmWareStartWrite[0] , fnFmwareStartWrite, 0},
	{&cmdFmWareOff[0] , fnFmwareOFF, 0},

	{&cmdSetBlock[0] , fnSetBlock, 4},
	{&cmdGetBlock[0] , fnGetBlock, 3},

	{&cmdGetType[0] , fnGetType, 0},
	{&cmdSetResetDevice[0] , fnResetDevice, 0},
	{&cmdSetFWInfo[0] , fnSetFirmwareInfo, 3},
	{&cmdGetFWInfo[0] , fnGetFirmwareInfo, 0},

	{&cmdConnect[0] , fnConnect, 0},
	{&cmdHelp[0] , fnHelp, 0},

	{&cmdIPSPING[0], fnReceivedIPSPING, 2},
	{&cmdIPSPONG[0], fnReceivedIPSPONG, 2},
	{&cmdINVITE[0], fnReceivedINVITE, 3},
	{&cmdACK[0], fnReceivedACK, 3},
	{&cmdINVITEACK[0], fnReceivedINVITEACK, 1},
	{&cmdBYE[0], fnReceivedBYE, 1},
	{&cmdLOG[0], fnReceivedLOG, 2},

	{NULL , NULL, 0}
};
/**
  * @}
  */

const sCLI_cmd_desc* CLI_searchCMD(char* _cmd){
	unsigned char index;
	char* pSubStr;
	uint8_t cmdLen;

	pSubStr=strstr(_cmd, " : ");
	if (pSubStr) cmdLen = pSubStr-_cmd;
	else cmdLen = strlen(_cmd);

	for(index = 0; cmd_tab[index].cmd_text; index++){
		if (!strncmp(_cmd, cmd_tab[index].cmd_text, cmdLen))
			return &cmd_tab[index];
	}
	return NULL;
}

