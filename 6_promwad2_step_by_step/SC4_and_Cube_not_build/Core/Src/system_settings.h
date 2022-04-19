/**
  ******************************************************************************
  * @file    system_settings.h
  * @author  dmib@@ETS
  * @brief   This file contains system settings manager module header
  ******************************************************************************
  */

#ifndef SystemSettingsH
#define SystemSettingsH

#include "stdint.h"

/** @defgroup SYS_SET_module System settings module
  * @brief System settings module
  * @{
  */

/** @defgroup SYS_SET_defines System settings parameters
  * @brief System settings parameters
  * @{
  */
#define SET_CID_SIZE				8
#define SET_NAME_SIZE				10
#define SET_GROUP_MAX_STATIONS		10
#define SET_MAX_GROUPS				8
#define SET_MAX_BUTTONS				48

#define JSONCONF_SIZE	(256*448)		// 448 EEPROM sectors  // =112 * 1024
#define PHONEBOOK_NUM	1000			// 1000 entries
#define PHONEBOOK_SIZE	(PHONEBOOK_NUM * sizeof(sTelBlockEntry))  // bytes // 12 * 1000
/**
  * @}
  */

/**
  * @brief network settings structure
  */
typedef struct {
	uint8_t ip[4];
	uint8_t mask[4];
	uint8_t gateway[4];
	uint8_t ipdoc[4];
	uint16_t portlogdoc;
	uint16_t portrtpdoc;	
} sNetworkSettings;

/**
  * @brief station settings structure
  */
typedef struct {
	uint8_t CID[SET_CID_SIZE];
	uint8_t ip[4];
} sStationSettings;

/**
  * @brief group settings structure
  */
typedef struct {
	uint32_t id;
	sStationSettings stations[SET_GROUP_MAX_STATIONS];
} sGroupsSettings;

/**
  * @brief button settings structure
  */
typedef struct {
	uint8_t CID[SET_CID_SIZE];
	uint8_t ip[4];
	uint8_t func;
	uint8_t priority;
} sButtonSettings;

/**
  * @brief abonents list entry structure
  */
typedef struct{
	uint16_t tel_number;
	uint16_t IP;
	uint8_t CID[SET_CID_SIZE];
} sTelBlockEntry;

/**
  * @brief main PDO settings structure
  */
typedef struct {
	uint8_t CID[SET_CID_SIZE];
	sNetworkSettings network;
	sGroupsSettings groups[SET_MAX_GROUPS];
	sButtonSettings buttons[SET_MAX_BUTTONS];
} sPDOSettings;

/** @defgroup SYS_SET_func System settings functions
  * @brief System settings functions
  * @{
  */
  
/**
  * @brief  main system settings initialization and loading from EEPROM
  * @retval result - 0 - loaded from EEPROM; -1 - loading from EEPROM failed, default loaded
  */
int8_t sysset_init();

/**
  * @brief  get link to main system settings
  * @retval result - pointer to main system settings
  */
sPDOSettings *sysset_get_settings();

/**
  * @brief  print system settings
  * @param  [in] settings - system setting structure
  * @retval none
  */
void sysset_print_settings(sPDOSettings *settings);

/**
  * @brief  get link to settings JSON block
  * @retval result - pointer to settings JSON block
  */
uint8_t *sysset_get_mainconf_adr();

/**
  * @brief  get link to abonent list
  * @retval result - pointer to abonent list array
  */
uint8_t *sysset_get_telconf_adr();

/**
  * @brief  main system settings loading from JSON array
  * @retval result - 0 - loaded from JSON; -1 - failed, JSON parse error
  */
int8_t sysset_load();

/**
  * @}
  */

/**
  * @}
  */

#endif
