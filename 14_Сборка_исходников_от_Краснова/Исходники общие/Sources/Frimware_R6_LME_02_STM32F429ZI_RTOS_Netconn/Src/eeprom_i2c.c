/* Includes ------------------------------------------------------------------*/
#include "eeprom_i2c.h"
#include "dbg_uart.h"
#include <string.h>


extern I2C_HandleTypeDef hi2c1; // экспортируем нужный модуль i2c, объявленный в main.c
uint8_t flag_eeprom_error = 0;
uint8_t flag_no_debug = 0;
extern char str[];

#define USED_I2C			(&hi2c1)

#define EEPROM_DEBUG
// TODO: поставить мьютекс на чтение и запись
//#define HAL_I2C_ERROR_NONE      (0x00000000U)    /*!< No error              */
//#define HAL_I2C_ERROR_BERR      (0x00000001U)    /*!< BERR error            */
//#define HAL_I2C_ERROR_ARLO      (0x00000002U)    /*!< ARLO error            */
//#define HAL_I2C_ERROR_AF        (0x00000004U)    /*!< ACKF error            */
//#define HAL_I2C_ERROR_OVR       (0x00000008U)    /*!< OVR error             */
//#define HAL_I2C_ERROR_DMA       (0x00000010U)    /*!< DMA transfer error    */
//#define HAL_I2C_ERROR_TIMEOUT   (0x00000020U)    /*!< Timeout error         */
//#define HAL_I2C_ERROR_SIZE      (0x00000040U)    /*!< Size Management error */
//#define HAL_I2C_ERROR_DMA_PARAM (0x00000080U)    /*!< DMA Parameter Error   */
//#define HAL_I2C_ERROR_INVALID_CALLBACK  (0x00000100U)    /*!< Invalid Callback error */
//#define HAL_I2C_ERROR_INVALID_PARAM     (0x00000200U)    /*!< Invalid Parameters error  */
// Возвращает значение из списка выше
uint32_t eepInit(void) {
	uint32_t e;
	
	HAL_I2C_IsDeviceReady(USED_I2C, EEPROM_ADDR, 10, 3000);
	e = HAL_I2C_GetError(USED_I2C);
	if (e != HAL_I2C_ERROR_NONE) flag_eeprom_error = 1; else flag_eeprom_error = 0;
	return HAL_I2C_GetError(USED_I2C);
}
/// пакетная запись size байт, но не более EEP_PAGE_SIZE  (32 для 24С64+, 16 для 24С16)////
// последовательность данных не должна пересекать адреса, кратные EEP_PAGE_SIZE
// иначе начнет переписывать начало страницы
// address - адрес начала пакета, data - массив данных, size - число байт для записи
uint8_t eepWritePage(uint32_t address, unsigned char * data, uint16_t size) {
	uint8_t timeout = 0;
	uint16_t dev_addr;

#ifdef EEPROM_DEBUG
//	static unsigned char str[256];
#endif
	
	//address &= 7FFFF; // обрезаем до 19 бит (не будем обрезать, лучше пусть ошибка будет по i2c)
	#if (ADDR_SIZE == 2)
	dev_addr = EEPROM_ADDR | ((address >> 16) << 1);
	#endif

	#if (ADDR_SIZE == 1)
	dev_addr = EEPROM_ADDR | ((address >> 8) << 1);	
	#endif	

	while (HAL_I2C_Mem_Write(USED_I2C, dev_addr, address, ADDR_SIZE, data, size > EEP_PAGE_SIZE? EEP_PAGE_SIZE : size, 500) != HAL_OK) {
		HAL_Delay(1);
		timeout++;
		if (timeout > 100) {

			flag_eeprom_error = 1;
			return 1; // ERROR
		}
	}
	return 0;
}
/////////// пакетная запись size байт, любое количество
// address - адрес начала пакета, data - массив данных, size - число байт для записи
void eepWriteSeq(uint32_t address, unsigned char * data, uint16_t size) {
	uint8_t u;
	uint16_t left;
	uint8_t err;

	if (flag_eeprom_error) {
		dbgUartPuts("EEP read ERROR: flag_eeprom_error was set on eep_init\r");
		return;
	}

	while (size > 0) {
			u = address & (EEP_PAGE_SIZE - 1); // остаток от деления на EEP_PAGE_SIZE, т.е. на сколько единиц наш адрес превышает адрес начала страницы

			left = EEP_PAGE_SIZE - u; // осталось байт до начала следующей страницы

			if (left > size) left = size;

			err = eepWritePage(address, data, left);

			data += left;
			address += left;
			size -= left;

			if (err) {
#ifdef EEPROM_DEBUG
			dbgUartPuts("EEP write ERROR\r");
#endif
			return;
			}
	}
#ifdef EEPROM_DEBUG
	dbgUartPuts("EEP write OK\r");
//	eepReadSeq(address, str, size);
//	if (strncmp((const char *)str, (const char *)data, size) == 0) dbgUartPuts("EEP write OK\r");
//	else dbgUartPuts("EEP verification ERROR\r");
#endif
}
void eepReadSeq(uint32_t address, unsigned char * data, uint16_t size) {
	uint8_t timeout = 0;
	uint16_t dev_addr;
	
	if (flag_eeprom_error) {
			dbgUartPuts("EEP read ERROR: flag_eeprom_error was set on eep_init\r");
			return;
	}

	//address &= 7FFFF; // обрезаем до 19 бит (не будем обрезать, лучше путь ошибка будет по i2c)

#if (ADDR_SIZE == 2)
	dev_addr = EEPROM_ADDR | ((address >> 16) << 1);
#endif

#if (ADDR_SIZE == 1)
	dev_addr = EEPROM_ADDR | ((address >> 8) << 1);
#endif 
	
	while (HAL_I2C_Mem_Read(USED_I2C, dev_addr, address, ADDR_SIZE, data, size, 500) != HAL_OK) {
		HAL_Delay(1);
		timeout++;
		if (timeout > 100) {
#ifdef EEPROM_DEBUG
			dbgUartPuts("EEP read ERROR\r");
			dbgUartPuts("ERROR code: ");
			HexToChar(str, USED_I2C->ErrorCode);
			dbgUartPuts(str);
			dbgUartPutchar('\r');
#endif
			return;
		}
	}
#ifdef EEPROM_DEBUG
	if (!flag_no_debug) dbgUartPuts("EEP read OK\r");
	else flag_no_debug = 0;
#endif

	// код ниже нельзя использовать, т.к. микросхема может быть занята после предшествующей записи
//	HAL_I2C_Mem_Read(USED_I2C, dev_addr, address, ADDR_SIZE, data, size, 5000);
//	if (USED_I2C->ErrorCode == HAL_I2C_ERROR_NONE) {
//		if (!flag_no_debug) dbgUartPuts("EEP read OK\r");
//		else flag_no_debug = 0;
//		
//	} else {
//			dbgUartPuts("EEP read ERROR\r");
//			dbgUartPuts("ERROR code: 0x");
//			HexToChar(str, USED_I2C->ErrorCode);
//			dbgUartPuts(str);
//			dbgUartPutchar('\r');
//	}
}
