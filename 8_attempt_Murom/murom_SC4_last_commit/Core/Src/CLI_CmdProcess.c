/**
  ******************************************************************************
  * @file    CLI_CmdProcess.c
  * @author  mixa@@ETS
  * @brief   This file contains CLI command decoder implementation
  ******************************************************************************
  */

#define _CLIDECODER_

#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include "CLI_io.h"
#include "CLI_CmdTable.h"
#include "CLI_CmdProcess.h"

/**
  * @brief string with current parsed command CLI parameters
  */
char arrStrPar[CLI_MAX_PARAMETERS_LENGTH];
/**
  * @brief array of pointers to current parsed command CLI parameters
  */
char* arrPSTR[CLI_MAX_NUMBER_OF_PARAMETERS];

unsigned char CLI_getParNumMsg(char* _pMsg)
{
	char* pMsg = _pMsg;
	unsigned char result = 0;
	char* pSubStr;
	while((pSubStr=strstr(pMsg, " : "))){
		result++;
		pMsg += (pSubStr-pMsg);
		pMsg += 3;
	}
	return result;
}

void CLI_parse_msg(char* _pMsg)
{
	char* pMsg = _pMsg;
	unsigned char index = 0;
	char* it = arrStrPar;
	char* pSubStr;
	memset(&arrStrPar, 0, CLI_MAX_PARAMETERS_LENGTH);
	while((pSubStr=strstr(pMsg, " : "))){
		if (it >= (it + CLI_MAX_PARAMETERS_LENGTH)) break;
		arrPSTR[index] = it;
		memcpy(it, pMsg, pSubStr-pMsg);
		it += (pSubStr-pMsg)+1;
		pMsg += (pSubStr-pMsg);
		pMsg += 3;
		if (index >= CLI_MAX_NUMBER_OF_PARAMETERS) break;
		index++;
	}
	arrPSTR[index] = it;
	strcpy(it, pMsg);
}

void CLI_executeCMD(char* _message, unsigned int _size)
{
	(void)_size;
char* pMsg = _message;
unsigned char parNum;
unsigned char calculate_parNum;
//unsigned char it;
while ((pMsg[_size-1]=='\n')||(pMsg[_size-1]=='\r')||(pMsg[_size-1] == '\0')) { pMsg[_size-1] = '\0'; _size--;};
const sCLI_cmd_desc* index_cmd = CLI_searchCMD(pMsg);
	if (index_cmd != NULL){
//читаем из структуры кол-во параметров
		parNum = (*index_cmd).par_num;
//проверяем сколько параметров в данном сообщении
		calculate_parNum = CLI_getParNumMsg(pMsg);
//сравниваем кол-во подсчитанных параметров с необходимым для данной команды и если не соответствует то сообщение об ошибке
		if (calculate_parNum < parNum){
			CLI_print("Error: not enough parameters\r\n");
		} else {
			CLI_parse_msg(pMsg);
//			for(it = 0; it < calculate_parNum+1; it++){
//				CLI_print("parameters : %s\n", arrPSTR[it]);
//			}
			(*index_cmd).func(calculate_parNum, &arrPSTR[1]);
		}
	} else {
		CLI_print("Error: Unknown command\r\n");
	}
}


