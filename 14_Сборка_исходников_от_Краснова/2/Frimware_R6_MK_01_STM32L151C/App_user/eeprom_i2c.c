/* Includes ------------------------------------------------------------------*/
#include "eeprom_i2c.h"

extern I2C_HandleTypeDef hi2c1; // экспортируем нужный модуль i2c, объявленный в main.c
#define USED_I2C			(&hi2c1)

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
	HAL_I2C_IsDeviceReady(USED_I2C, EEPROM_ADDR, 10, 3000);
	return HAL_I2C_GetError(USED_I2C);
}

void eepWriteSeq(uint32_t address, unsigned char * data, uint32_t size) {
	uint8_t timeout = 0;
	uint16_t dev_addr;
	
	//address &= 7FFFF; // обрезаем до 19 бит (не будем обрезать, лучше пусть ошибка будет по i2c)
	
	dev_addr = EEPROM_ADDR | ((address >> 16) << 1);
	
	while (HAL_I2C_Mem_Write(USED_I2C, dev_addr, address, ADDR_SIZE, data, size, 500) != HAL_OK) {
		HAL_Delay(1);
		timeout++;
		if (timeout > 100) {

			return;
		}
	}
}

void eepReadSeq(uint32_t address, unsigned char * data, uint32_t size) {
	uint8_t timeout = 0;
	uint16_t dev_addr;

	//address &= 7FFFF; // обрезаем до 19 бит (не будем обрезать, лучше путь ошибка будет по i2c)

	dev_addr = EEPROM_ADDR | ((address >> 16) << 1);
	
	while (HAL_I2C_Mem_Read(USED_I2C, dev_addr, address, ADDR_SIZE, data, size, 500) != HAL_OK) {
		HAL_Delay(1);
		timeout++;
		if (timeout > 100) {

			return;
		}
	}
}
