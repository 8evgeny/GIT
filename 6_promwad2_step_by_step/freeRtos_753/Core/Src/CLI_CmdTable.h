/**
  ******************************************************************************
  * @file    CLI_CmdTable.h
  * @author  mixa@@ETS
  * @brief   This file contains CLI command table module header
  ******************************************************************************
  */

#ifndef INC_TABLECOMMAND_H_
#define INC_TABLECOMMAND_H_

/** @defgroup CLI_CmdTable CLI command table module
  * @brief CLI command table implementation
  * @{
  */

/**
  * @brief CLI process command function type definition
  */
typedef void (*cmd_func)(int numParam, char** params);

/**
  * @brief CLI command description structure
  */
typedef struct {
  const char *cmd_text;
  cmd_func func;
  unsigned char par_num;
} sCLI_cmd_desc;

/**
  * @brief  search CLI command
  * @param  [in] _cmd - CLI string with expected command
  * @retval result - pointer to CLI command description (NULL - if command not found)
  */
const sCLI_cmd_desc* CLI_searchCMD(char* _cmd);

/**
  * @}
  */

#endif /* TABLECOMMAND */
