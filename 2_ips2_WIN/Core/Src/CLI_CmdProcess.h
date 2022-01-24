/**
  ******************************************************************************
  * @file    CLI_CmdProcess.h
  * @author  mixa@@ETS
  * @brief   This file contains CLI command processor module header
  ******************************************************************************
  */

#ifndef INC_CLIDECODER_H_
#define INC_CLIDECODER_H_

#ifdef _CLIDECODER_
#define __decl_CLIDECODER__
#else
#define __decl_CLIDECODER__ extern
#endif

/** @defgroup CLI_CmdProcess CLI command processor module
  * @brief CLI command processor implementation
  * @{
  */

/**
  * @brief  maximum number of CLI command parameters
  */
#define CLI_MAX_NUMBER_OF_PARAMETERS	8
/**
  * @brief  maximum summary length of CLI command parameters
  */
#define CLI_MAX_PARAMETERS_LENGTH		1200

/**
  * @brief array of pointers to current parsed command CLI parameters
  */
extern char* arrPSTR[CLI_MAX_NUMBER_OF_PARAMETERS];

/**
  * @brief  decode and execute CLI command
  * @param  [in] _message - CLI string
  * @param  [in] _size - size of CLI string in bytes
  * @retval none
  */
void CLI_executeCMD(char* _message, unsigned int _size);

/**
  * @brief  calculate number of parameters in CLI command
  * @param  [in] _pMsg - CLI command string
  * @retval none
  */
unsigned char CLI_getParNumMsg(char* _pMsg);

/**
  * @brief  parse CLI command into internal buffer
  * @param  [in] _pMsg - CLI command string
  * @retval none
  */
void CLI_parse_msg(char* _pMsg);

/**
  * @}
  */

#endif /* INC_CLIDECODER_H_ */
