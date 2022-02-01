/**
  ******************************************************************************
  * @file    CLI_CmdTestFn.c
  * @author  dmib@@ETS
  * @brief   This file contains CLI test mode functions implementation
  ******************************************************************************
  */
#include "string.h"
#include "stm32h7xx_hal.h"
#include "main.h"		  // for port defines
#include "CLI_io.h"
#include "driver_UI.h"
#include "driver_AIC.h"
#include "driver_EEPROM.h"
#include "dp83848.h"
#include "MemTest.h"
#include "tone_gen_state_processor.h"
#include "system_settings.h"
#include "audio_process.h"
#include "CLI_CmdDeviceFn.h"
#include "CLI_CmdTestFn.h"

/// compile with LWIP support
#define USE_LWIP

#ifdef USE_LWIP

#include "lwip.h"
#include "ping.h"

/// default ping test IP-address
const ip_addr_t default_test_ping_IP = IPADDR4_INIT_BYTES(192,168,0,137);

#endif

/// current test mode level
uint8_t TestModeOn;

/// test LEDs enabled state
uint8_t TestLEDOn;
/// test buttons enabled state
uint8_t TestBTNOn;
/// LEDs time for test
uint32_t testLEDtime;
/// LEDs test internal state
uint8_t testLEDst;

/// PIN description structure
typedef struct {
  char name[10] ;
  GPIO_TypeDef * port;
  uint16_t pin;
} sPIN_DESC;

/// out PIN description array
const sPIN_DESC PinNamesOut[]={
             {"POW_DOWN", POW_DOWN_GPIO_Port, POW_DOWN_Pin},
			 {"UP2_FAN", UPR2_FAN_GPIO_Port, UPR2_FAN_Pin},
#ifdef SC_4
             {"NORMA_UPR", NORMA_UPR_GPIO_Port, NORMA_UPR_Pin},
             {"UPR1_SP", UPR1_SP_GPIO_Port, UPR1_SP_Pin},
             {"MKVKL_UPR", MKVKL_UPR_GPIO_Port, MKVKL_UPR_Pin},
             {"RELE_UPR", RELE_UPR_GPIO_Port, RELE_UPR_Pin},
#endif
#ifdef SC_2
             {"L1", GPIOC, LED1},
             {"L2", GPIOC, LED2},
             {"L3", GPIOC, LED3},
             {"L4", GPIOG, LED4},
             {"L5", GPIOG, LED5},
             {"L6", GPIOG, LED6},
#endif
             {"TEST_LED", TEST_LED_GPIO_Port, TEST_LED_Pin}
           };

/// in PIN description array
const sPIN_DESC PinNamesIn[]=
{
    #ifdef SC_4
    {"INT_BUT", INT_BUT_GPIO_Port, INT_BUT_Pin},
    #endif
    {"DET_48V", DET_48V_GPIO_Port, DET_48V_Pin},
    {"UPR_PHONE", UPR_PHONE_GPIO_Port, UPR_PHONE_Pin}, //пин переопределяется в зависимости от SC_2
    {"UPR_MIC2", UPR_MIC2_GPIO_Port, UPR_MIC2_Pin},
    {"VOL+", VOL_UP_GPIO_Port, VOL_UP_Pin},
    {"VOL-", VOL_DOWN_GPIO_Port, VOL_DOWN_Pin},
    {"SENS+", SENS_UP_GPIO_Port, SENS_UP_Pin},
    {"SENS-", SENS_DOWN_GPIO_Port, SENS_DOWN_Pin},
    {"TEST_BUT", TEST_BUT_GPIO_Port, TEST_BUT_Pin},
    #ifdef SC_4
    {"CFG_UI0", CFG_UI0_GPIO_Port, CFG_UI0_Pin},
    {"CFG_UI1", CFG_UI1_GPIO_Port, CFG_UI1_Pin},
    {"CFG_UI2", CFG_UI2_GPIO_Port, CFG_UI2_Pin},
    #endif
    {"K1", GPIOG, K1},
    {"K2", GPIOG, K2},
    {"K3", GPIOG, K3},
    {"K4", GPIOG, K4},
    {"K5", GPIOG, K5},
    {"K6", GPIOG, K6}
};

/// out PIN number
#define PinNamesOutNum (sizeof(PinNamesOut)/sizeof(sPIN_DESC))
/// in PIN number
#define PinNamesInNum (sizeof(PinNamesIn)/sizeof(sPIN_DESC))
		   
/**
  * @brief  search pin description
  * @param  [in] _pinname - string with pin name
  * @param  [in] pintab - array of PIN description
  * @param  [in] num - number of elements in pintab
  * @retval result - index of pin; - 1 - not found
  */
int8_t searchPIN(char* _pinname, const sPIN_DESC *pintab, uint8_t num ){
	uint8_t i;
	for(i = 0; i < num; i++){
		if (strstr(_pinname, pintab[i].name) != NULL) return i;
	}
	return -1;
}

void fnSetPIN(int numParam, char** params)
{
	int8_t pinnum, pinval;

	if (!(TestModeOn&TEST_MODE_ON)) {
	 CLI_print("Error : not in test mode\r\n");
	 return;
	}

	pinnum = searchPIN(params[0], &PinNamesOut[0], PinNamesOutNum );
	if (pinnum>=0) {
	 pinval = params[1][0] - '0';
	 if ((pinval==0) || (pinval==1)) {
	  HAL_GPIO_WritePin(PinNamesOut[pinnum].port, PinNamesOut[pinnum].pin, pinval);
	  CLI_print("Pin %s set to %i\r\n", params[0], pinval);
	 }
	 else CLI_print("Error : wrong pin value\r\n");
	} else {
	 CLI_print("Error : wrong pin name\r\n");
	}
}

void fnGetPIN(int numParam, char** params)
{
	int8_t pinnum, pinval;

	if (!(TestModeOn&TEST_MODE_ON)) {
	 CLI_print("Error : not in test mode\r\n");
	 return;
	}

	pinnum = searchPIN(params[0], &PinNamesIn[0], PinNamesInNum );
	if (pinnum>=0) {
	  pinval = HAL_GPIO_ReadPin(PinNamesIn[pinnum].port, PinNamesIn[pinnum].pin);
	  CLI_print("Input pin %s : %i\r\n", params[0], pinval);
	} else {
	 CLI_print("Error : wrong pin name\r\n");
	}	
}

// SetLED idx8 st8
void fnSetLED(int numParam, char** params)
{
	uint8_t ledidx, ledst, _ok;
	
	if (!(TestModeOn&TEST_MODE_ON)) {
	 CLI_print("Error : not in test mode\r\n");
	 return;
	}
	
    ledidx = Hex2Int(params[0], 1, &_ok);
    if (!_ok) {
   	 CLI_print("Error : invalid param0\r\n");
   	 return;
    }
	ledst = Hex2Int(params[1], 1, &_ok);
    if (!_ok) {
   	 CLI_print("Error : invalid param1\r\n");
   	 return;
    }
	
	if (ledidx>=LED_NUM) {
	 CLI_print("Error : invalid led number\r\n");
	 return;
	}
	
	ui_setledstate(ledidx, ledst);
	CLI_print("Set led state ok\r\n");
}

// GetBTN idx8
void fnGetBTN(int numParam, char** params)
{
	uint8_t btnidx, btnval, _ok;
	
	if (!(TestModeOn&TEST_MODE_ON)) {
	 CLI_print("Error : not in test mode\r\n");
	 return;
	}

    btnidx = Hex2Int(params[0], 1, &_ok);
    if (!_ok) {
   	 CLI_print("Error : invalid param0\r\n");
   	 return;
    }
	if (btnidx>=BTN_NUM) {
	 CLI_print("Error : invalid btn number\r\n");
	 return;
	}

    btnval = ui_getbtnval(btnidx);
	CLI_print("Get btn %.2X val : %i\r\n", btnidx, btnval);	
}

/// link to I2C2 hardware module control/status structure
extern I2C_HandleTypeDef hi2c2;

// SetAIC page8 reg8 data8
void fnSetAIC(int numParam, char** params)
{
	uint8_t res, page, reg, data, _ok;
	
	if (!(TestModeOn&TEST_MODE_DEBUG)) {
	 CLI_print("Error : not in debug mode\r\n");
	 return;
	}

	page = Hex2Int(params[0], 1, &_ok);
    if (!_ok) {
   	 CLI_print("Error : invalid param0\r\n");
   	 return;
    }
	reg = Hex2Int(params[1], 1, &_ok);
    if (!_ok) {
   	 CLI_print("Error : invalid param1\r\n");
   	 return;
    }
	data = Hex2Int(params[2], 1, &_ok);
    if (!_ok) {
   	 CLI_print("Error : invalid param2\r\n");
   	 return;
    }
	res = TLV320_WritePage(page, reg, data);
	if (res == HAL_OK)
	  CLI_print("AIC reg %.2X val : %.2X\r\n", reg, data);	
	else
	  CLI_print("Error : AIC write error\r\n");
}

// GetAIC page8 reg8
void fnGetAIC(int numParam, char** params)
{
	uint8_t res, page, reg, data, _ok;
	
	if (!(TestModeOn&TEST_MODE_DEBUG)) {
	 CLI_print("Error : not in debug mode\r\n");
	 return;
	}
	
	page = Hex2Int(params[0], 1, &_ok);
    if (!_ok) {
   	 CLI_print("Error : invalid param0\r\n");
   	 return;
    }
	reg = Hex2Int(params[1], 1, &_ok);
    if (!_ok) {
   	 CLI_print("Error : invalid param1\r\n");
   	 return;
    }
	res = TLV320_ReadPage(page, reg, &data);
	if (res == HAL_OK)
	  CLI_print("AIC reg %.2X val : %.2X\r\n", reg, data);	
	else
	  CLI_print("Error : AIC read error\r\n");
}

extern ETH_HandleTypeDef heth;

// SetETH reg8 data16
void fnSetETH(int numParam, char** params)
{
	uint8_t res, reg, _ok;
	uint16_t data;

	if (!(TestModeOn&TEST_MODE_DEBUG)) {
	 CLI_print("Error : not in debug mode\r\n");
	 return;
	}
	
	reg = Hex2Int(params[0], 1, &_ok);
    if (!_ok) {
   	 CLI_print("Error : invalid param0\r\n");
   	 return;
    }
	data = Hex2Int(params[1], 2, &_ok);
    if (!_ok) {
   	 CLI_print("Error : invalid param1\r\n");
   	 return;
    }
	res = HAL_ETH_WritePHYRegister(&heth, 1, reg, data);
	if (res == HAL_OK)
	  CLI_print("ETHPHY reg %.2X val : %.4X\r\n", reg, data);	
	else
	  CLI_print("Error : ETHPHY write error\r\n");
}

// GetETH reg8
void fnGetETH(int numParam, char** params)
{
	uint8_t res, reg,_ok;
	uint32_t data;

	if (!(TestModeOn&TEST_MODE_DEBUG)) {
	 CLI_print("Error : not in debug mode\r\n");
	 return;
	}

	reg = Hex2Int(params[0], 1, &_ok);
    if (!_ok) {
   	 CLI_print("Error : invalid param0\r\n");
   	 return;
    }
	res = HAL_ETH_ReadPHYRegister(&heth, 1, reg, &data);
	if (res == HAL_OK)
	  CLI_print("ETHPHY reg %.2X val : %.4X\r\n", reg, data);	
	else
	  CLI_print("Error : ETHPHY read error\r\n");
}

void fnTestSRAM(int numParam, char** params)
{
  SRam_Status ram_st;

  if (!(TestModeOn&TEST_MODE_ON)) {
	CLI_print("Error : not in test mode\r\n");
	return;
  }

  SCB_InvalidateDCache();
  SCB_DisableDCache();

  Test_RAM(0x60000000, 0x100000, &ram_st, 1);

  SCB_EnableDCache();
}

uint8_t eeprom_write_bytes(uint32_t address, uint8_t *data, uint32_t size);
uint8_t eeprom_waitwrite_polling(uint32_t adr, uint32_t *delay);

void fnTestEEPROM(int numParam, char** params)
{
	uint32_t resHAL, delay;
	int8_t res;
	SystemBlockHdr *eeprom_hdr;

	if (!(TestModeOn&TEST_MODE_ON)) {
		CLI_print("Error : not in test mode\r\n");
		return;
	}

	eeprom_hdr = eeprom_get_system_SB();

	if (numParam>0) {
		if (!strcmp(params[0],"FORMAT")) {
			eeprom_default_SB(eeprom_hdr);
			resHAL = eeprom_write_bytes(0, (uint8_t *)eeprom_hdr, sizeof(SystemBlockHdr));
			if (resHAL == HAL_OK) {
			 resHAL = eeprom_waitwrite_polling(0,&delay);
			 if (resHAL == HAL_OK) CLI_print("EEPROM formatted, write delay %i\r\n",delay);
			}
			if (resHAL != HAL_OK) {
			 CLI_print("Error : EEPROM write error\r\n");
			 return;
			}
		} else
		if (!strcmp(params[0],"TIME")) {
			eeprom_test_writedelay();
			return;
		}
	}

	res = eeprom_read_SB(eeprom_hdr);

	switch (res) {
	 case 0: CLI_print("Test EEPROM ok\r\n");break;
	 case -1: CLI_print("Error : EEPROM read error\r\n");break;
	 case -2: CLI_print("Error : EEPROM invalid filesystem\r\n");break;
	 case -3: CLI_print("Error : EEPROM invalid CRC\r\n");break;
	 default:CLI_print("Error : EEPROM ???\r\n");break;
	}
}

void fnTestCODEC(int numParam, char** params)
{
  if (!(TestModeOn&TEST_MODE_ON)) {
	CLI_print("Error : not in test mode\r\n");
	return;
  }

  aic_deinit();
  if (!aic_init()) CLI_print("Test AIC ok\r\n");
}

void fnTestTone(int numParam, char** params)
{
  uint16_t tone_idx, tone_len;
  uint8_t _ok;

  if (!(TestModeOn&TEST_MODE_ON)) {
	CLI_print("Error : not in test mode\r\n");
	return;
  }

  tone_idx = Hex2Int(params[0], 1, &_ok);
  if (!_ok) {
 	 CLI_print("Error : invalid param0\r\n");
 	 return;
  }
  tone_len = Hex2Int(params[1], 2, &_ok);
  if (!_ok) {
 	 CLI_print("Error : invalid param1\r\n");
 	 return;
  }

  tone_genPlayTone(tone_idx, tone_len);

  CLI_print("Start test tone %.2X len %.4X\r\n", tone_idx, tone_len);
}

void fnTestPrn(int numParam, char** params)
{
   if (!(TestModeOn&TEST_MODE_ON)) {
 	CLI_print("Error : not in test mode\r\n");
	return;
   }

   if (!numParam) {
	CLI_print("Test print ok\r\n");
   } else {
	if (!strcmp(params[0],"SB")) eeprom_print_SB();
	else
	if (!strcmp(params[0],"CONF")) sysset_print_settings(sysset_get_settings());
   }
}

/// timeout for phy autonegotiation
#define TEST_PHYAN_TIMEOUT		3000
/// timeout for ping test
#define TEST_PING_TIMEOUT		5000

/// link to PHY controller control/status structure
extern DP83848_Object_t DP83848;
/// link to ETHERNET hardware module control/status structure
extern ETH_HandleTypeDef heth;

/// print PHY link status
void test_print_PhyStatus(int32_t res) {
	switch (res) {
		case DP83848_STATUS_READ_ERROR: CLI_print("Test PHY : read error\r\n");break;
		case DP83848_STATUS_100MBITS_FULLDUPLEX: CLI_print("Test PHY : 100 MBit FD\r\n");break;
		case DP83848_STATUS_100MBITS_HALFDUPLEX: CLI_print("Test PHY : 100 MBit HD\r\n");break;
		case DP83848_STATUS_10MBITS_FULLDUPLEX: CLI_print("Test PHY : 10 MBit FD\r\n");break;
		case DP83848_STATUS_10MBITS_HALFDUPLEX: CLI_print("Test PHY : 10 MBit HD\r\n");break;
		case DP83848_STATUS_AUTONEGO_NOTDONE: CLI_print("Test PHY : autonegotiation in process\r\n");break;
		case DP83848_STATUS_LINK_DOWN: CLI_print("Test PHY : link down\r\n");break;
		default:CLI_print("Test PHY : status unknown\r\n");break;
	}
}

void fnTestPHY(int numParam, char** params)
{
	uint32_t res, timeout;

	if (!(TestModeOn&TEST_MODE_ON)) {
	 	CLI_print("Error : not in test mode\r\n");
	 	return;
	}

	if (numParam>0) {
		if (!strcmp(params[0],"AN")) {
			HAL_ETH_Stop(&heth);
			if (DP83848_StartAutoNego(&DP83848) == DP83848_STATUS_OK)
			 CLI_print("Test PHY : restart autonegotiation\r\n");
			else
			 CLI_print("Error : restart autonegotiation\r\n");
			return;
		} else
		if (!strcmp(params[0],"STATUS")) {
			res = DP83848_GetLinkState(&DP83848);
			test_print_PhyStatus(res);
		} else
		if (!strcmp(params[0],"PING")) {
#ifdef USE_LWIP
			ip_addr_t test_ping_IP;

			if (numParam > 1) {
				if (!ip4addr_aton(params[1], &test_ping_IP)) {
				 	CLI_print("Error : invalid param1\r\n");
					return;
				}
			} else {
				test_ping_IP.addr = default_test_ping_IP.addr;
			}
			ping_init(&test_ping_IP);
			ping_send();
			timeout = HAL_GetTick();
			while (1) {
				ui_task();
				MX_LWIP_Process();
				if (ping_get_result()) {
			 		CLI_print("Test PHY PING : OK , %i ms\r\n",HAL_GetTick() - timeout);
			 		break;
				}
				if ((HAL_GetTick() - timeout) > TEST_PING_TIMEOUT)
				{
			 		CLI_print("Test PHY PING : PING timeout\r\n");
			 		break;
				}
			}
			ping_free();
#endif
		}
	} else {

	 HAL_ETH_Stop(&heth);
	 DP83848_DeInit(&DP83848);

	 res = DP83848_Init(&DP83848);
	 if (res != DP83848_STATUS_OK) {
		CLI_print("Error : PHY init\r\n");
		return;
	 }

	 timeout = HAL_GetTick();
	 while (1) {
		 res = DP83848_GetLinkState(&DP83848);
	 	 if (res == DP83848_STATUS_READ_ERROR) {
	 		CLI_print("Test PHY : read error\r\n");
	 		return;
	 	 } else if ((res >= DP83848_STATUS_100MBITS_FULLDUPLEX) && (res <= DP83848_STATUS_10MBITS_HALFDUPLEX))
	 		break;
		if ((HAL_GetTick() - timeout) > TEST_PHYAN_TIMEOUT)
		{
	 		CLI_print("Test PHY : PHY AN timeout\r\n");
	 		break;
		}
     }
	 res = DP83848_GetLinkState(&DP83848);
	 test_print_PhyStatus(res);
	 if ((res >= DP83848_STATUS_100MBITS_FULLDUPLEX) && (res <= DP83848_STATUS_10MBITS_HALFDUPLEX)) CLI_print("Test PHY ok\r\n");
   }
}

void fnTestUI(int numParam, char** params)
{
	if (!(TestModeOn&TEST_MODE_ON)) {
	 CLI_print("Error: not in test mode\r\n");
	 return;
	}

	CLI_print("Test UI board\r\n");

	printDeviceID();

	ui_test_board(1);

	CLI_print("Test UI end\r\n");
}

void fnTestLEDOn(int numParam, char** params)
{
	if (!(TestModeOn&TEST_MODE_ON)) {
	 CLI_print("Error: not in test mode\r\n");
	 return;
	}

	if (TestBTNOn) fnTestBTNOff( numParam, params);
	TestLEDOn = 1;
	testLEDtime = HAL_GetTick(); 
	testLEDst = 0;
	
	CLI_print("LED Test on\r\n");
}

void fnTestLEDOff(int numParam, char** params)
{
	if (!(TestModeOn&TEST_MODE_ON)) {
	 CLI_print("Error: not in test mode\r\n");
	 return;
	}

	TestLEDOn = 0;
	ui_init();
	
	CLI_print("LED Test off\r\n");
}

void fnTestBTNOn(int numParam, char** params)
{
	if (!(TestModeOn&TEST_MODE_ON)) {
	 CLI_print("Error: not in test mode\r\n");
	 return;
	}
	
	if (TestLEDOn) fnTestLEDOff( numParam, params);
	TestBTNOn = 1;
	CLI_print("BTN Test on\r\n");
}

void fnTestBTNOff(int numParam, char** params)
{
	if (!(TestModeOn&TEST_MODE_ON)) {
	 CLI_print("Error: not in test mode\r\n");
	 return;
	}
	
	TestBTNOn = 0;
	ui_init();
	
	CLI_print("BTN Test off\r\n");
}

void fnTestAudioLoopOn(int numParam, char** params)
{
	if (!(TestModeOn&TEST_MODE_ON)) {
	 CLI_print("Error: not in test mode\r\n");
	 return;
	}

	audio_stop();
	aic_setSAIloop(1);
	CLI_print("AudioLoop Test on\r\n");
}

void fnTestAudioLoopOff(int numParam, char** params)
{
	if (!(TestModeOn&TEST_MODE_ON)) {
	 CLI_print("Error: not in test mode\r\n");
	 return;
	}

	aic_setSAIloop(0);
	audio_start();
	CLI_print("AudioLoop Test off\r\n");
}

/// print test mode start info
void test_print_TestStartInfo()
{
	int32_t b_num;
	sPDOSettings *sset = sysset_get_settings();
	SystemBlockHdr *sysSB = eeprom_get_system_SB();

	CLI_print("GIT-Comm IPS\r\n");
	printDeviceID();

	CLI_print("Serial No : %.12s\r\n",sysSB->SN);

	b_num = eeprom_find_block(sysSB, EEPROM_BLOCK_TYPE_FW);
	if (b_num<0) {
		CLI_print("Firmware : info not found\r\n");
	} else {
		if (sysSB->blocks[b_num].Size)
			CLI_print("Firmware : %.8X\r\n", sysSB->blocks[b_num].Ver);
		else
			CLI_print("Firmware : unknown\r\n");
	}
	b_num = eeprom_find_block(sysSB, EEPROM_BLOCK_TYPE_CNF);
	if (b_num<0) {
		CLI_print("Configuration : info not found\r\n");
	} else {
		CLI_print("Configuration : %.8X\r\n", sysSB->blocks[b_num].Ver);
	}

	CLI_print("IP address : %i.%i.%i.%i\r\n",sset->network.ip[0],sset->network.ip[1],sset->network.ip[2],sset->network.ip[3]);
	CLI_print("MAC address : %.2X-%.2X-%.2X-%.2X-%.2X-%.2X\r\n",
			heth.Init.MACAddr[0],heth.Init.MACAddr[1],heth.Init.MACAddr[2],
			heth.Init.MACAddr[3],heth.Init.MACAddr[4],heth.Init.MACAddr[5]);
	CLI_print("CID : %s\r\n", sset->CID);
	CLI_print("\r\n");
}

/// set current print level according test mode level
void update_CLI_PrintLevel()
{
	uint8_t pl;

	pl = CLI_PRINTLEVEL_WORK;

	if (TestModeOn | TEST_MODE_ON) pl = CLI_PRINTLEVEL_TEST;
	if (TestModeOn | TEST_MODE_SERVICE) pl = CLI_PRINTLEVEL_SERVICE;
	if (TestModeOn | TEST_MODE_DEBUG) pl = CLI_PRINTLEVEL_SVCDEBUG;

	CLI_setPrintLevel(pl);
}

void fnStartTestMode(int numParam, char** params)
{
  TestModeOn |= TEST_MODE_ON;

  ui_setledstate(LED_TEST, LED_STATE_ON);
  test_print_TestStartInfo();
  update_CLI_PrintLevel();
  CLI_print("TEST MODE ENABLED\r\n");
}

void fnStopTestMode(int numParam, char** params)
{
  TestModeOn &= ~TEST_MODE_ON;
  
  if (TestLEDOn) fnTestLEDOff( 0, 0);
  if (TestBTNOn) fnTestBTNOff( 0, 0);
  fnTestAudioLoopOff(0, 0);
  ui_setledstate(LED_TEST, LED_STATE_OFF);
  update_CLI_PrintLevel();
  
  CLI_print("TEST MODE DISABLED\r\n");
}

void fnStartDebugMode(int numParam, char** params)
{
  TestModeOn |= TEST_MODE_DEBUG;
  test_print_TestStartInfo();
  update_CLI_PrintLevel();
  CLI_print("DEBUG MODE ENABLED\r\n");
}

void fnStopDebugMode(int numParam, char** params)
{
  TestModeOn &= ~TEST_MODE_DEBUG;
  update_CLI_PrintLevel();
  CLI_print("DEBUG MODE DISABLED\r\n");
}

void fnStartServiceMode(int numParam, char** params)
{
  TestModeOn |= TEST_MODE_SERVICE;
  test_print_TestStartInfo();
  update_CLI_PrintLevel();
  CLI_print("SERVICE MODE ENABLED\r\n");
}

void fnStopServiceMode(int numParam, char** params)
{
  TestModeOn &= ~TEST_MODE_SERVICE;
  update_CLI_PrintLevel();
  CLI_print("SERVICE MODE DISABLED\r\n");
}

void test_init()
{
  TestModeOn = 0;
  //TestModeOn = TEST_MODE_ON; // TODO !!!
  TestLEDOn = 0;
  TestBTNOn = 0;
}

void test_task()
{
   uint8_t i, btidx;
   
   if (TestModeOn&TEST_MODE_ON) {
     if (TestLEDOn) {
      if ((HAL_GetTick() - testLEDtime) > 1000) {
		testLEDtime = HAL_GetTick(); 
		testLEDst ^= 1;
		i = LED_AB1R;
		if (testLEDst) {
			ui_setledstate(LED_TEST, LED_STATE_OFF);
			ui_setledstate(LED_NORMA, LED_STATE_OFF);
			ui_setledstate(LED_MKVKL, LED_STATE_ON);

		 while (i<LED_NUM) {
			ui_setledstate(i++, LED_STATE_ON);
			ui_setledstate(i++, LED_STATE_OFF);
		 }
		} else {
		 ui_setledstate(LED_TEST, LED_STATE_ON);
		 ui_setledstate(LED_NORMA, LED_STATE_ON);
		 ui_setledstate(LED_MKVKL, LED_STATE_OFF);
		 while (i<LED_NUM) {
			ui_setledstate(i++, LED_STATE_OFF);
			ui_setledstate(i++, LED_STATE_ON);
		 }
		}
      }	 
	 } else if (TestBTNOn) {
	   btidx = ui_getactivebtn();
	   if ((btidx!=BTN_NONE) && (btidx>=BTN_AB1))
	    if (ui_getbtnval(btidx)==BTN_VAL_DOWN) {
	    	CLI_print("BTN %.2X\r\n",btidx);
	     if (btidx>=BTN_NN1) {
		   btidx = LED_AB1R + ((btidx - BTN_NN1) << 1); // red led corresponding to button
		   if (ui_getledstate(btidx)==LED_STATE_OFF) 
		    ui_setledstate(btidx,LED_STATE_ON);
		   else 	
		    ui_setledstate(btidx,LED_STATE_OFF);		   
		 } else {
		   btidx = LED_AB1G + ((btidx - BTN_AB1) << 1); // green led corresponding to button
		   if (ui_getledstate(btidx)==LED_STATE_OFF) 
		    ui_setledstate(btidx,LED_STATE_ON);
		   else 	
		    ui_setledstate(btidx,LED_STATE_OFF);
		 }	   
	   }
	 }      
   }
}

uint8_t test_get_mode()
{
	return TestModeOn;
}

//-------------------------------------------
extern uint32_t ec_enable;
extern uint32_t ec_GR_threshold;
extern uint32_t ec_MIC_threshold;
extern uint32_t ec_fade_level;
extern uint32_t ec_MIC_disable_time;
extern uint32_t ec_GR_enable_time;
extern uint32_t ec_GR_disable_time;
extern uint32_t ec_GR_max_energy;
extern uint32_t ec_MIC_max_energy;

//             EN   GRT    MCT    FDL  MCDT GRET GRDT
//TEST SET EC : X : XXXX : XXXX : XX : XX : XX : XX
void fnSetEC(int numParam, char** params)
{
  ec_enable =  atoi(params[0]);
  ec_GR_threshold = atoi(params[1]);
  ec_MIC_threshold = atoi(params[2]);
  ec_fade_level = atoi(params[3]);
  ec_MIC_disable_time = atoi(params[4]);
  ec_GR_enable_time = atoi(params[5]);
  ec_GR_disable_time = atoi(params[6]);
  fnGetEC(numParam, params);
}

void fnGetEC(int numParam, char** params)
{
  CLI_print("Audioloop disabler param:\r\n");
  CLI_print("ELC enabled   : %i\r\n",ec_enable);
  CLI_print("GR Threshold  : %i\r\n",ec_GR_threshold);
  CLI_print("MIC Threshold : %i\r\n",ec_MIC_threshold);
  CLI_print("Fade level    : %i\r\n",ec_fade_level);
  CLI_print("MIC dis time  : %i\r\n",ec_MIC_disable_time);
  CLI_print("GR en time    : %i\r\n",ec_GR_enable_time);
  CLI_print("GR dis time   : %i\r\n",ec_GR_disable_time);
  CLI_print("GR dis time   : %i\r\n",ec_GR_disable_time);
  
  CLI_print("MAX MIC level  : %i\r\n",ec_MIC_max_energy);
  CLI_print("MAX GR  level  : %i\r\n",ec_GR_max_energy);
  ec_MIC_max_energy = 0;
  ec_GR_max_energy = 0;
}
