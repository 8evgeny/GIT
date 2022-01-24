/**
  ******************************************************************************
  * @file    system_settings.c
  * @author  dmib@@ETS
  * @brief   This file contains system settings manager module
  ******************************************************************************
  */

#include "string.h"
#include "CLI_io.h"
#include "driver_EEPROM.h"
#include "json_settings_parser.h"
#include "system_settings.h"

#define SYSSET_CLI_ERROR_LEVEL	CLI_PRINTLEVEL_ALWAYS

/**
  * @brief default network settings structure
  */
const sNetworkSettings default_net_settings = {
						{192,168,0,101},
						{255,255,255,0},
						{192,168,0,254},
						{192,168,0,102},
						12341,
						12344
                       };

/*const sNetworkSettings default_net_settings = {
						{192,168,66,102},
						{255,255,255,0},
						{192,168,66,254},
						{192,168,66,101},
						12343,
						12344
                       };*/

/**
  * @brief system settings json string array
  */
uint8_t json_static_block[JSONCONF_SIZE] __attribute__((section(".ExtRamData")));
/**
  * @brief abonent list array
  */
sTelBlockEntry phonebook_block[PHONEBOOK_NUM] __attribute__((section(".ExtRamData")));

/**
  * @brief main PDO system settings structure
  */
sPDOSettings system_settings;
/**
  * @brief link to EEPROM system block
  */
SystemBlockHdr *system_SB;

/**
  * @brief print IP address macro
  */
#define PrintIP(s)	CLI_print("%i.%i.%i.%i",s[0],s[1],s[2],s[3])

/**
  * @brief  print system settings
  * @param  [in] settings - system setting structure
  * @retval none
  */
void sysset_print_settings(sPDOSettings *settings)
{
	uint8_t i,j;

	CLI_print("----------\r\n");
	CLI_print("Settings\r\n");
	CLI_print("- CID : %s\r\n", settings->CID);
	CLI_print("- Network :\r\n");
	CLI_print("-- IP : ");PrintIP(settings->network.ip);CLI_print("\r\n");
	CLI_print("-- Mask : ");PrintIP(settings->network.mask);CLI_print("\r\n");
	CLI_print("-- Gateway : ");PrintIP(settings->network.gateway);CLI_print("\r\n");
	CLI_print("-- IP doc : ");PrintIP(settings->network.ipdoc);CLI_print("\r\n");
	CLI_print("-- Port log doc : %i\r\n", settings->network.portlogdoc);
	CLI_print("-- Port rtp doc : %i\r\n", settings->network.portrtpdoc);

	for (i = 0; i < SET_MAX_BUTTONS; i++) {
	  CLI_print("- Button %i:\r\n" , i);
	  CLI_print("-- CID : %s\r\n", settings->buttons[i].CID);
	  CLI_print("-- IP : ");PrintIP(settings->buttons[i].ip);CLI_print("\r\n");
	  CLI_print("-- Function : %i\r\n", settings->buttons[i].func);
	  CLI_print("-- Priority : %i\r\n", settings->buttons[i].priority);
	}

	for (i = 0; i < SET_MAX_GROUPS; i++) {
	  CLI_print("- Group %i:\r\n", i);
	  CLI_print("-- ID : GID%i\r\n", settings->groups[i].id);
	  for (j = 0; j < SET_GROUP_MAX_STATIONS; j++) {
		CLI_print("-- Station %i:\r\n", j);
		CLI_print("-- CID : %s\r\n", settings->groups[i].stations[j].CID);
		CLI_print("-- IP : ");PrintIP(settings->groups[i].stations[j].ip);CLI_print("\r\n");
	  }
	}
	CLI_print("----------");
}

/**
  * @brief  default system settings initialization
  * @param  [inout] settings - system setting structure
  * @retval none
  */
void sysset_init_default(sPDOSettings *settings)
{
	memset((uint8_t *)settings, 0, sizeof(sPDOSettings));
	memcpy(&settings->network, &default_net_settings, sizeof(sNetworkSettings));
}

/**
  * @brief  main system settings initialization and loading from EEPROM
  * @retval result - 0 - loaded from EEPROM; -1 - loading from EEPROM failed, default loaded
  */
int8_t sysset_init()
{
	int8_t b_num, res;
	
	sysset_init_default(&system_settings);

	memset((uint8_t *)&json_static_block, 0, JSONCONF_SIZE);
	memset((uint8_t *)&phonebook_block, 0, PHONEBOOK_SIZE);

	system_SB = eeprom_get_system_SB();
	
	b_num = eeprom_find_block(system_SB, EEPROM_BLOCK_TYPE_CNF);
	if (b_num<0) {
		CLI_print_lev(SYSSET_CLI_ERROR_LEVEL, "EEPROM : no config data, using default\r\n");
		return -1;
	}
	if (!system_SB->blocks[b_num].Size){
		CLI_print_lev(SYSSET_CLI_ERROR_LEVEL,"EEPROM : no config data, using default\r\n");
		return -1;
	}
	
	res = eeprom_read_block(system_SB, b_num, &json_static_block[0]);
	if (res<0){
		CLI_print_lev(SYSSET_CLI_ERROR_LEVEL,"EEPROM : load config data error, using default\r\n");
		return -1;
	}

	res = Parse_JSON((char *)&json_static_block[0], system_SB->blocks[b_num].Size, &system_settings);
	if (res<0){
		CLI_print_lev(SYSSET_CLI_ERROR_LEVEL,"EEPROM : parse config data error, using default\r\n");
		sysset_init_default(&system_settings);
		return -1;
	}

	// TODO at next stage - load EEPROM_BLOCK_TYPE_TEL

	//sysset_print_settings(&system_settings);

	return 0;
}

/**
  * @brief  get link to settings JSON block
  * @retval result - pointer to settings JSON block
  */
uint8_t *sysset_get_mainconf_adr()
{
	return &json_static_block[0];
}

/**
  * @brief  get link to abonent list
  * @retval result - pointer to abonent list array
  */
uint8_t *sysset_get_telconf_adr()
{
	return (uint8_t *)&phonebook_block[0];
}

/**
  * @brief  get link to main system settings
  * @retval result - pointer to main system settings
  */
sPDOSettings *sysset_get_settings()
{
	return &system_settings;
}

/**
  * @brief  main system settings loading from JSON array
  * @retval result - 0 - loaded from JSON; -1 - failed, JSON parse error
  */
int8_t sysset_load()
{
	int32_t len, b_num;

	b_num = eeprom_find_block(system_SB, EEPROM_BLOCK_TYPE_CNF);
	if (b_num<0) return -1;
	len = system_SB->blocks[b_num].Size;
	if (Parse_JSON((char *)&json_static_block[0], len, &system_settings) != JSON_OK) return -1;

	return 0;
}
