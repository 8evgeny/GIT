/**
  ******************************************************************************
  * @file    CLI_CmdServiceFn.c
  * @author  dmib@@ETS
  * @brief   This file contains CLI service functions implementation
  ******************************************************************************
  */
#include "string.h"
#include "CLI_io.h"
#include "crc32.h"
#include "driver_EEPROM.h"
#include "system_settings.h"
#include "audio_process.h"
#include "fmware_flasher.h"
#include "CLI_CmdDeviceFn.h"
#include "CLI_CmdServiceFn.h"

// write data flags
#define CONF_WRITE_SB	0x01
#define CONF_WRITE_CNF	0x02
#define CONF_WRITE_TEL	0x04
#define CONF_WRITE_FW	0x08

// fnGetBlock/fnSetBlock block type
#define EntryType_SB		0
#define EntryType_CNF		1
#define EntryType_FW		2
#define EntryType_TEL		3

/// current service mode
servicemode_t service_mode;
/// current data write flags
uint8_t what_write;
/// buffer for command answer
char svc_block_answer[1040];

servicemode_t service_getmode()
{
  return service_mode;
}

void service_init() 
{
   service_mode = svcNone;
}

void fnConfigOn(int numParam, char** params){
	(void)numParam;
	(void)params;
	
	switch (service_mode)  {
      case svcNone:
      case svcConfig:   
	    service_mode = svcConfig;
	    what_write = 0;
	    audio_stop();
   	    CLI_print("Config on : OK\r\n");
	  break;
      case svcFirmware:     
	  default: 
   	    CLI_print("Config on : Error mode\r\n");	  
	  break;
	}
}

void fnConfigOFF(int numParam, char** params){
	(void)numParam;
	(void)params;
	switch (service_mode)  {
      case svcConfig:   
	    service_mode = svcNone;
   	    CLI_print("Config off : OK\r\n");
   	    if (what_write) {
		  // reset/reload configuration
   	      fnResetDevice(0, 0);
   	    }
   	    audio_start();
	  break;
      case svcNone:
      case svcFirmware:     
	  default: 
   	    CLI_print("Config off : Error mode\r\n");	  
	  break;
	}
}

void fnFmwareOn(int numParam, char** params){
	(void)numParam;
	(void)params;
	switch (service_mode)  {
      case svcNone:
      case svcFirmware:     
	    service_mode = svcFirmware;
	    audio_stop();
   	    CLI_print("Fmware on : OK\r\n");
	  break;
      case svcConfig:   
	  default: 
   	    CLI_print("Fmware on : Error mode\r\n");
	  break;
	}
}

void fnFmwareOFF(int numParam, char** params){
	(void)numParam;
	(void)params;
	switch (service_mode)  {
      case svcFirmware:     
	    service_mode = svcNone;
   	    CLI_print("Fmware off : OK\r\n");
   	    audio_start();
   	    if (what_write) {
		  // reset/reload configuration
   	      fnResetDevice(0, 0);
   	    }
	  break;
      case svcNone:
      case svcConfig:   
	  default: 
   	    CLI_print("Fmware off : Error mode\r\n");
	  break;
	}	
}

void fnGetBlock(int numParam, char** params){
	(void)numParam;
	char _crc_buff[9];
	uint8_t _ok;
	uint8_t* pBeginBlock;
	uint8_t type_block;
	uint32_t addr;
	uint32_t size;
	uint32_t _crc_block;
	
	type_block = Hex2Int(params[0], 1, &_ok);
	addr = Hex2Int(params[1], 4, &_ok);
	size = Hex2Int(params[2], 4, &_ok);
	
	switch (type_block) {
	  case EntryType_SB:
			if (service_mode == svcNone) {
				CLI_print("Get block data : Error mode\r\n");
				return;
			}
			if ((addr+size) > sizeof(SystemBlockHdr)) {
				CLI_print("Get block data : Error size\r\n");
				return;
			}
			pBeginBlock = (uint8_t *)(eeprom_get_system_SB()) + addr;
			break;
	  case EntryType_FW:
			if (service_mode != svcFirmware) {
				CLI_print("Get block data : Error mode\r\n");
				return;
			}
			if ((addr+size) > FIRMWARE_MAX_SIZE) {
				CLI_print("Get block data : Error size\r\n");
				return;
			}
			pBeginBlock = (uint8_t *)(FIRMWARE_GET_ADDR + addr);
			break;
	  case EntryType_CNF:
			if (service_mode != svcConfig) {
				CLI_print("Get block data : Error mode\r\n");
				return;
			}
			if ((addr+size) > JSONCONF_SIZE) {
				CLI_print("Get block data : Error size\r\n");
				return;
			}
			pBeginBlock = sysset_get_mainconf_adr() + addr;
			break;
	  case EntryType_TEL:
			if (service_mode != svcConfig) {
				CLI_print("Get block data : Error mode\r\n");
				return;
			}
			if ((addr+size) > PHONEBOOK_SIZE) {
				CLI_print("Get block data : Error size\r\n");
				return;
			}
			pBeginBlock = sysset_get_telconf_adr() + addr;
			break;
			
	  default:
	        CLI_print("Get block data : Error block type\r\n");
	        return;
	}
	
	Array2Hex(pBeginBlock, size, svc_block_answer);
	svc_block_answer[size<<1] = 0;
	
	_crc_block = crc32_calc(0, pBeginBlock, size);
	Int2Hex(_crc_block, 4, _crc_buff);
	_crc_buff[8] = 0;
	
	CLI_print("Get block data : %s : %s\r\n", svc_block_answer, _crc_buff);
}

void fnSetBlock(int numParam, char** params){
	(void)numParam;
	uint8_t _ok;
	uint8_t* pBeginBlock;
	uint8_t type_block;
	uint32_t addr;
	uint32_t size;
	uint32_t crc,_crc_block;

	type_block = Hex2Int(params[0], 1, &_ok);
	addr = Hex2Int(params[1], 4, &_ok);
	size = strlen(params[2]) >> 1;
	crc = Hex2Int(params[3], 4, &_ok);

	switch (type_block) {
	  case EntryType_SB:
			if (service_mode == svcNone) {
				CLI_print("Set block data : Error mode\r\n");
				return;
			}
			if ((addr+size) > sizeof(SystemBlockHdr)) {
				CLI_print("Set block data : Error size\r\n");
				return;
			}
			pBeginBlock = (uint8_t *)(eeprom_get_system_SB()) + addr;
			what_write |= CONF_WRITE_SB;
			break;
	  case EntryType_FW:
			if (service_mode != svcFirmware) {
				CLI_print("Set block data : Error mode\r\n");
				return;
			}
			if ((addr+size) > FIRMWARE_MAX_SIZE) {
				CLI_print("Set block data : Error size\r\n");
				return;
			}
			pBeginBlock = (uint8_t *)(FIRMWARE_SET_ADDR + addr);
			what_write |= CONF_WRITE_FW;
			break;
	  case EntryType_CNF:
			if (service_mode != svcConfig) {
				CLI_print("Set block data : Error mode\r\n");
				return;
			}
			if ((addr+size) > JSONCONF_SIZE) {
				CLI_print("Set block data : Error size\r\n");
				return;
			}
			pBeginBlock = sysset_get_mainconf_adr() + addr;
			what_write |= CONF_WRITE_CNF;
			break;
	  case EntryType_TEL:
			if (service_mode != svcConfig) {
				CLI_print("Set block data : Error mode\r\n");
				return;
			}
			if ((addr+size) > PHONEBOOK_SIZE) {
				CLI_print("Set block data : Error size\r\n");
				return;
			}
			pBeginBlock = sysset_get_telconf_adr() + addr;
			what_write |= CONF_WRITE_TEL;
			break;
			
	  default:
	        CLI_print("Set block data : Error block type\r\n");
	        return;
	}
		
	Hex2Array(params[2], size<<1, (unsigned char*)pBeginBlock);
	_crc_block = crc32_calc(0, pBeginBlock, size);
	
	if (_crc_block != crc)
		CLI_print("Set block data : BAD : Error write : Mismatch CRC block\r\n");
	else
		CLI_print("Set block data : OK\r\n");
}

void fnConfigStartWrite(int numParam, char** params){
   uint32_t crc;
   SystemBlockHdr *sb;
   int8_t b_num,b_num_conf, err;

   if (service_mode != svcConfig) {
  	  CLI_print("Config result : Error mode\r\n");
	  return;
   }

   if (!what_write) {
	   CLI_print("Config result : OK\r\n");
	   return;
   }

   // check config data
   err = 0;
   sb = eeprom_get_system_SB();
   b_num = eeprom_find_block(sb, EEPROM_BLOCK_TYPE_SB);
   if (b_num < 0) {
	   err = 1; goto config_data_err;
   }
   crc = sb->blocks[b_num].CRC32;
   eeprom_update_block_CRC(sb, 0, (uint8_t *)sb);
   if (crc != sb->blocks[b_num].CRC32) {
	   err = 1; goto config_data_err;
   }

   if (what_write & CONF_WRITE_CNF) {
    b_num_conf = eeprom_find_block(sb, EEPROM_BLOCK_TYPE_CNF);
    if (b_num_conf < 0) {
	    err = 1; goto config_data_err;
    }
    crc = crc32_calc(0, sysset_get_mainconf_adr(), sb->blocks[b_num_conf].Size);
    if (crc != sb->blocks[b_num_conf].CRC32) {
	    err = 1; goto config_data_err;
    }

    if (sysset_load()<0) {
	    err = 1; goto config_data_err;
    }
   } else b_num_conf = -1;

   // TODO at next stage check TELtable
config_data_err:
   if (err) {
	   CLI_print("Config result : Config data error\r\n");
	   // reset/reload configuration
	   fnResetDevice(0, 0);
   	   return;
   }

   // start EEPROM write
   err = 0;
   if (eeprom_write_SB(sb) < 0) {
	   err = 1; goto conf_write_err;
   }

   if (what_write & CONF_WRITE_CNF) {
    if (eeprom_write_block(sb, b_num_conf, sysset_get_mainconf_adr()) < 0) {
	   err = 1; goto conf_write_err;
    }
   }

   // TODO at next stage write TELtable

conf_write_err:
   if (err) {
	   CLI_print("Config result : Write error\r\n");
	   // reset/reload configuration
	   fnResetDevice(0, 0);
	   return;
   }

   what_write = 0;

   CLI_print("Config result : OK\r\n");

   CLI_print_lev(CLI_PRINTLEVEL_SERVICE, "Updating configuration...\r\n");

   // reset/reload configuration
   fnResetDevice(0, 0);
}

void fnFmwareStartWrite(int numParam, char** params){
   uint32_t crc;
   SystemBlockHdr *sb;
   int8_t b_num,b_num_fw, err;

   if (service_mode != svcFirmware) {
  	  CLI_print("Fmware start : Error mode\r\n");
	  return;
   }

   if ((!(what_write & CONF_WRITE_SB)) && (!(what_write & CONF_WRITE_FW))) { // need SB or firmware
	   err = 1; goto fmware_data_err;
   }

   // check data
   err = 0;
   sb = eeprom_get_system_SB();
   b_num = eeprom_find_block(sb, EEPROM_BLOCK_TYPE_SB);
   if (b_num < 0) {
	   err = 1; goto fmware_data_err;
   }
   crc = sb->blocks[b_num].CRC32;
   eeprom_update_block_CRC(sb, 0, (uint8_t *)sb);
   if (crc != sb->blocks[b_num].CRC32) {
	   err = 1; goto fmware_data_err;
   }

   if (what_write & CONF_WRITE_FW) {
     b_num_fw = eeprom_find_block(sb, EEPROM_BLOCK_TYPE_FW);
     if (b_num_fw < 0) {
	     err = 1; goto fmware_data_err;
     }
     crc = crc32_calc(0, (uint8_t *)FIRMWARE_SET_ADDR, sb->blocks[b_num_fw].Size);
     if (crc != sb->blocks[b_num_fw].CRC32) {
	     err = 1; goto fmware_data_err;
     }
   } else b_num_fw = -1;

fmware_data_err:
   if (err) {
  	   CLI_print("Fmware start : Firmware data error\r\n");
   	   // reset/reload configuration
   	   fnResetDevice(0, 0);
 	   return;
   }

   // start EEPROM write
   if (eeprom_write_SB(sb) < 0) {
	   CLI_print("Fmware start : Write error\r\n");
	   // reset/reload configuration
	   fnResetDevice(0, 0);
	   return;
   }

   CLI_print("Fmware start : OK\r\n");

   CLI_print_lev(CLI_PRINTLEVEL_SERVICE, "Updating firmware...\r\n");

   if (what_write & CONF_WRITE_FW)
    WriteFirmware((uint8_t *)FIRMWARE_SET_ADDR, sb->blocks[b_num_fw].Size);
}
