/**
  ******************************************************************************
  * @file    CLI_CmdTestFn.h
  * @author  dmib@@ETS
  * @brief   This file contains CLI test mode functions module header
  ******************************************************************************
  */
#ifndef __CLITESTFN__
#define __CLITESTFN__

/** @defgroup CLI_CmdTestFn CLI test mode functions module
  * @brief CLI test mode functions implementation
  * @{
  */

/** @defgroup CLI_CmdTestFn_mode CLI test mode level flags
  * @brief CLI test mode level flags
  * @{
  */
#define		TEST_MODE_ON 		1	///< test mode level
#define		TEST_MODE_SERVICE	2	///< service mode level
#define		TEST_MODE_DEBUG		4	///< debug mode level
/**
  * @}
  */

/**
  * @brief  test mode module initialization
  * @retval none
  */
void test_init();

/**
  * @brief  periodic test mode process task
  * @retval none
  */
void test_task();

/**
  * @brief  get current test module mode
  * @retval result - current test module mode
  */
uint8_t test_get_mode();

/**
  * @brief  process start TEST mode command
  * @param  [in] numParam - number of command parameters
  * @param  [in] params - array of pointers to string parameters
  * @retval none
  */
void fnStartTestMode(int numParam, char** params);

/**
  * @brief  process stop TEST mode command
  * @param  [in] numParam - number of command parameters
  * @param  [in] params - array of pointers to string parameters
  * @retval none
  */

void fnStopTestMode(int numParam, char** params);

/**
  * @brief  process start DEBUG mode command
  * @param  [in] numParam - number of command parameters
  * @param  [in] params - array of pointers to string parameters
  * @retval none
  */
void fnStartDebugMode(int numParam, char** params);

/**
  * @brief  process stop DEBUG mode command
  * @param  [in] numParam - number of command parameters
  * @param  [in] params - array of pointers to string parameters
  * @retval none
  */
void fnStopDebugMode(int numParam, char** params);

/**
  * @brief  process start SERVICE mode command
  * @param  [in] numParam - number of command parameters
  * @param  [in] params - array of pointers to string parameters
  * @retval none
  */
void fnStartServiceMode(int numParam, char** params);

/**
  * @brief  process stop SERVICE mode command
  * @param  [in] numParam - number of command parameters
  * @param  [in] params - array of pointers to string parameters
  * @retval none
  */
void fnStopServiceMode(int numParam, char** params);

/**
  * @brief  process test RAM command
  * @param  [in] numParam - number of command parameters
  * @param  [in] params - array of pointers to string parameters
  * @retval none
  */
void fnTestSRAM(int numParam, char** params);

/**
  * @brief  process test EEPROM command
  * @param  [in] numParam - number of command parameters
  * @param  [in] params - array of pointers to string parameters
  * @retval none
  */
void fnTestEEPROM(int numParam, char** params);

/**
  * @brief  process test audiocodec command
  * @param  [in] numParam - number of command parameters
  * @param  [in] params - array of pointers to string parameters
  * @retval none
  */
void fnTestCODEC(int numParam, char** params);

/**
  * @brief  process PHY-controller command
  * @param  [in] numParam - number of command parameters
  * @param  [in] params - array of pointers to string parameters
  * @retval none
  */
void fnTestPHY(int numParam, char** params);

void fnTestUI(int numParam, char** params);

/**
  * @brief  process test LED on command
  * @param  [in] numParam - number of command parameters
  * @param  [in] params - array of pointers to string parameters
  * @retval none
  */
void fnTestLEDOn(int numParam, char** params);

/**
  * @brief  process test LED off command
  * @param  [in] numParam - number of command parameters
  * @param  [in] params - array of pointers to string parameters
  * @retval none
  */
void fnTestLEDOff(int numParam, char** params);

/**
  * @brief  process test UI buttons on command
  * @param  [in] numParam - number of command parameters
  * @param  [in] params - array of pointers to string parameters
  * @retval none
  */
void fnTestBTNOn(int numParam, char** params);

/**
  * @brief  process test UI buttons off command
  * @param  [in] numParam - number of command parameters
  * @param  [in] params - array of pointers to string parameters
  * @retval none
  */
void fnTestBTNOff(int numParam, char** params);

/**
  * @brief  process test audio loop on command
  * @param  [in] numParam - number of command parameters
  * @param  [in] params - array of pointers to string parameters
  * @retval none
  */
void fnTestAudioLoopOn(int numParam, char** params);

/**
  * @brief  process test audio loop off command
  * @param  [in] numParam - number of command parameters
  * @param  [in] params - array of pointers to string parameters
  * @retval none
  */
void fnTestAudioLoopOff(int numParam, char** params);

/**
  * @brief  process test tone signal command
  * @param  [in] numParam - number of command parameters
  * @param  [in] params - array of pointers to string parameters
  * @retval none
  */
void fnTestTone(int numParam, char** params);

/**
  * @brief  process test print info command
  * @param  [in] numParam - number of command parameters
  * @param  [in] params - array of pointers to string parameters
  * @retval none
  */
void fnTestPrn(int numParam, char** params);

/**
  * @brief  process set out pin value command
  * @param  [in] numParam - number of command parameters
  * @param  [in] params - array of pointers to string parameters
  * @retval none
  */
void fnSetPIN(int numParam, char** params);

/**
  * @brief  process get in pin value command
  * @param  [in] numParam - number of command parameters
  * @param  [in] params - array of pointers to string parameters
  * @retval none
  */
void fnGetPIN(int numParam, char** params);

/**
  * @brief  process set LED state command
  * @param  [in] numParam - number of command parameters
  * @param  [in] params - array of pointers to string parameters
  * @retval none
  */
void fnSetLED(int numParam, char** params);

/**
  * @brief  process get button state command
  * @param  [in] numParam - number of command parameters
  * @param  [in] params - array of pointers to string parameters
  * @retval none
  */
void fnGetBTN(int numParam, char** params);

/**
  * @brief  process set AIC register command
  * @param  [in] numParam - number of command parameters
  * @param  [in] params - array of pointers to string parameters
  * @retval none
  */
void fnSetAIC(int numParam, char** params);

/**
  * @brief  process get AIC register command
  * @param  [in] numParam - number of command parameters
  * @param  [in] params - array of pointers to string parameters
  * @retval none
  */
void fnGetAIC(int numParam, char** params);

/**
  * @brief  process set PHY-controller register value command
  * @param  [in] numParam - number of command parameters
  * @param  [in] params - array of pointers to string parameters
  * @retval none
  */
void fnSetETH(int numParam, char** params);

/**
  * @brief  process get PHY-controller register value command
  * @param  [in] numParam - number of command parameters
  * @param  [in] params - array of pointers to string parameters
  * @retval none
  */
void fnGetETH(int numParam, char** params);

/**
  * @brief  process set SELC module settings
  * @param  [in] numParam - number of command parameters
  * @param  [in] params - array of pointers to string parameters
  * @retval none
  */
void fnSetEC(int numParam, char** params);

/**
  * @brief  process get SELC module settings
  * @param  [in] numParam - number of command parameters
  * @param  [in] params - array of pointers to string parameters
  * @retval none
  */
void fnGetEC(int numParam, char** params);

/**
  * @}
  */

#endif
