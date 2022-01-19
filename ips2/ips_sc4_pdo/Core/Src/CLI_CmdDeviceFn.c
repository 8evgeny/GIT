/**
  ******************************************************************************
  * @file    CLI_CmdDeviceFn.c
  * @author  dmib@@ETS
  * @brief   This file contains CLI device common functions implementation
  ******************************************************************************
  */
#include "stm32h7xx_hal.h"
#include "driver_pinIO.h"
#include "driver_ui.h"
#include "driver_EEPROM.h"
#include "CLI_io.h"
#include "crc32.h"
#include "CLI_CmdServiceFn.h"
#include "CLI_CmdDeviceFn.h"

/**
  * @brief CLI connect string1
  */
static const char txtTitle1[]="\033[36;1mGIT-Comm IPS \r\n";
/**
  * @brief CLI connect string2
  */
static const char txtTitle2[]="\033[32;1mConsole connected \r\nType \033[33;1mHELP\033[32;1m for commands list\r\n";

/**
  * @brief CLI help string
  */
static const char txtHelp[]="System commands:\r\n \033[33;1m DEVICE RESET \033[31;1m - soft restart MCU(connection will be lost)\r\n \033[33;1m DEVICE GET CFGUI \033[32;1m - get UI type\r\n";

/**
  * @brief PDO recode type array
  */
//							16 32 48 16H 32H
//const uint8_t PDO_Code[] = { 5, 4, 4,  5,  4 };
const uint8_t PDO_Code[] = { 1, 3, 5,  2,  4, 0, 0, 0 };    //new table

/**
  * @brief  print PDO device type
  * @retval none
  */
void printDeviceID() {
	CLI_print("Device type : ");
	switch (ui_get_uimode()) {
			case UI_mode_UI16: CLI_print("PDO-16\r\n"); break;
			case UI_mode_UI32: CLI_print("PDO-32\r\n"); break;
			case UI_mode_UI48: CLI_print("PDO-48\r\n"); break;
			case UI_mode_UI16N: CLI_print("PDO-16N\r\n"); break;
			case UI_mode_UI32N: CLI_print("PDO-32N\r\n"); break;
			case UI_mode_NOUI: CLI_print("UI not connectes\r\n"); break;
			default: CLI_print("unknown UI\r\n"); break;
	};
}

/**
  * @brief  check and print firmware information
  * @retval none
  */
void checkFirmware()
{
	int32_t len, b_num, crc32;
	SystemBlockHdr *sysSB = eeprom_get_system_SB();

	b_num = eeprom_find_block(sysSB, EEPROM_BLOCK_TYPE_FW);
	if (b_num<0) {
		CLI_print("Firmware : info not found\r\n");
		return;
	}
	len = sysSB->blocks[b_num].Size;
	if (!len) {
		CLI_print("Firmware : info not found\r\n");
		return;
	}

	crc32 = crc32_calc(0, (uint8_t *)FIRMWARE_GET_ADDR,sysSB->blocks[b_num].Size);
	CLI_print("Firmware : CRC32 = %.8X\r\n", crc32);
	CLI_print("Firmware : ver = %i\r\n", sysSB->blocks[b_num].Ver);
	CLI_print("Firmware : image CRC32 = %.8X\r\n", sysSB->blocks[b_num].CRC32);
}

void fnConnect(int numParam, char** params)
{
	(void)numParam;
	(void)params;
	CLI_print((char *)txtTitle1);
	printDeviceID();
	CLI_print((char *)txtTitle2);
}

void fnHelp(int numParam, char** params)
{
	(void)numParam;
	(void)params;
	CLI_print((char *)txtHelp);
}

void fnGetType(int numParam, char** params)
{
	(void)numParam;
	(void)params;
	uint8_t cfg_ui = PDO_Code[pinio_get_CFGUI()];
	CLI_print("CFGUI : %d\r\n", cfg_ui);
}

void fnResetDevice(int numParam, char** params)
{
	(void)numParam;
	(void)params;
	CLI_print("RESETING DEVICE...\r\n");
	NVIC_SystemReset();
}

// DEVICE FWINFO
void fnGetFirmwareInfo(int numParam, char** params)
{
	int32_t len, b_num, crc32;
	SystemBlockHdr *sysSB = eeprom_get_system_SB();

	b_num = eeprom_find_block(sysSB, EEPROM_BLOCK_TYPE_FW);
	if (b_num<0) {
		CLI_print("FWINFO : info not found\r\n");
		return;
	}
	len = sysSB->blocks[b_num].Size;
	if (!len) {
		CLI_print("FWINFO : info not found\r\n");
		return;
	}

	CLI_print("Saved firmware version : %.8X\r\n", sysSB->blocks[b_num].Ver);
	CLI_print("Saved firmware CRC32 : %.8X\r\n", sysSB->blocks[b_num].CRC32);
	CLI_print("Saved firmware size : %i bytes\r\n", sysSB->blocks[b_num].Size);
	crc32 = crc32_calc(0, (uint8_t *)FIRMWARE_GET_ADDR,sysSB->blocks[b_num].Size);
	CLI_print("Calculated CRC32 : %.8X\r\n", crc32);
}

// DEVICE SETFW : SZ : CRC : VER
// DEVICE SETFW : XXXXXXXX : XXXXXXXX : XXXXXXXX
void fnSetFirmwareInfo(int numParam, char** params)
{
	int32_t b_num, crc32, size, crc32new, ver;
	uint8_t _ok;

	SystemBlockHdr *sysSB = eeprom_get_system_SB();

	size = Hex2Int(params[0], 4, &_ok);
    if (!_ok) {
   	 CLI_print("Error : invalid param0\r\n");
   	 return;
    }
	crc32new = Hex2Int(params[1], 4, &_ok);
    if (!_ok) {
   	 CLI_print("Error : invalid param1\r\n");
   	 return;
    }
	ver = Hex2Int(params[2], 4, &_ok);
    if (!_ok) {
   	 CLI_print("Error : invalid param2\r\n");
   	 return;
    }

	crc32 = crc32_calc(0, (uint8_t *)FIRMWARE_GET_ADDR,size);
	if (crc32 != crc32new) {
		CLI_print("SETFW : CRC not match\r\n");
		return;
	}

	b_num = eeprom_find_block(sysSB, EEPROM_BLOCK_TYPE_FW);
	if (b_num<0) {
		CLI_print("SETFW : error EEPROM block not found\r\n");
		return;
	}

	sysSB->blocks[b_num].CRC32 = crc32new;
	sysSB->blocks[b_num].Size = size;
	sysSB->blocks[b_num].Ver = ver;

	if (eeprom_write_SB(sysSB) < 0) {
		CLI_print("SETFW : write error\r\n");
	} else {
		CLI_print("SETFW : OK\r\n");
	}
}
