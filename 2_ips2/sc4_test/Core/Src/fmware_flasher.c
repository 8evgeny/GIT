/**
  ******************************************************************************
  * @file    fmware_flasher.c
  * @author  dmib@@ETS
  * @brief   This file contains write firmware to internal flash implementation
  ******************************************************************************
  */
#include "stm32h7xx_hal.h"
#include "fmware_flasher.h"

extern UART_HandleTypeDef huart7;

/** @addtogroup FIRMWARE_FLASHER_module
  * @{
  */

/** @defgroup FIRMWARE_FLASHER_defines Firmware flasher settings
  * @brief Firmware flasher settings
  * @{
  */
#define FLASH_FW_ADDRESS		FLASH_BANK1_BASE
#define FLASH_FW_BANK			FLASH_BANK_1
#define FLASH_VoltageRange		FLASH_VOLTAGE_RANGE_3
#define FLASH_NB_BYTES_IN_FLASHWORD	(FLASH_NB_32BITWORD_IN_FLASHWORD<<2)
/**
  * @}
  */

/** @defgroup FIRMWARE_FLASHER_msg Firmware flasher messages
  * @brief Firmware flasher messages
  * @{
  */
char ram_HexTab[]="0123456789ABCDEF";
char FLASH_msg_start[]="Flash : start write\r\n";
char FLASH_msg_err_u[]="Flash : unlock error\r\n";
char FLASH_msg_err_e[]="Flash : erase error\r\n";
char FLASH_msg_err_w[]="Flash : write error\r\n";
char FLASH_msg_end[]="Flash : write finished, reseting...\r\n";
/**
  * @}
  */

/**
  * @}
  */

/**
  * @brief  convert uint32 to hex string
  * @note RAM adapted function
  */
__RAM_FUNC void Int2Hex_ram(uint32_t _src, uint8_t _size, char* _dst)
{
  _size = _size << 1;
  _dst += _size - 1;
  while (_size--) {
   *_dst-- = ram_HexTab[_src & 0xF];
   _src >>= 4;
  }
}

/**
  * @brief  print string to UART7
  * @note RAM adapted function
  */
__RAM_FUNC void UART_print_ram(char *str)
{
  while (*str!=0) {
   while ((huart7.Instance->ISR & UART_FLAG_TXE) != UART_FLAG_TXE) ;
   huart7.Instance->TDR = (uint8_t)*str;
   str++;
  }
} 

/**
  * @brief  STM32 internal flash wait end of operation
  * @note RAM adapted function from HAL Library (FLASH_WaitForLastOperation)
  */
__RAM_FUNC HAL_StatusTypeDef FLASH_WaitOperation(uint32_t bank)
{
  uint32_t bsyflag = FLASH_FLAG_QW_BANK1;
  uint32_t errorflag = FLASH->SR1 & FLASH_FLAG_ALL_ERRORS_BANK1;

  if (bank == FLASH_BANK_2)
  {
    /* Get Error Flags */
    errorflag = (FLASH->SR2 & FLASH_FLAG_ALL_ERRORS_BANK2) | 0x80000000U;
    /* Select bsyflag depending on Bank */
    bsyflag = FLASH_FLAG_QW_BANK2;
  }

  while(__HAL_FLASH_GET_FLAG(bsyflag)) ;

  /* In case of error reported in Flash SR1 or SR2 register */
  if((errorflag & 0x7FFFFFFFU) != 0U)
  {
    /* Clear error programming flags */
    __HAL_FLASH_CLEAR_FLAG(errorflag);

    return HAL_ERROR;
  }

  /* Check FLASH End of Operation flag  */
  if(bank == FLASH_BANK_1)
  {
    if (__HAL_FLASH_GET_FLAG_BANK1(FLASH_FLAG_EOP_BANK1))
    {
      /* Clear FLASH End of Operation pending bit */
      __HAL_FLASH_CLEAR_FLAG_BANK1(FLASH_FLAG_EOP_BANK1);
    }
  }
  else
  {
    if (__HAL_FLASH_GET_FLAG_BANK2(FLASH_FLAG_EOP_BANK2))
    {
      /* Clear FLASH End of Operation pending bit */
      __HAL_FLASH_CLEAR_FLAG_BANK2(FLASH_FLAG_EOP_BANK2);
    }
  }

  return HAL_OK; 
}

/**
  * @brief  STM32 internal flash lock
  * @note RAM adapted function from HAL Library
  */
__RAM_FUNC HAL_StatusTypeDef FLASH_Lock(uint32_t bank)
{
  if (bank == FLASH_BANK_1) {
	SET_BIT(FLASH->CR1, FLASH_CR_LOCK);
	/* Verify Flash Bank1 is locked */
	if (READ_BIT(FLASH->CR1, FLASH_CR_LOCK) == 0U) return HAL_ERROR;
  } if (bank == FLASH_BANK_2) {
	SET_BIT(FLASH->CR2, FLASH_CR_LOCK);
	/* Verify Flash Bank2 is locked */
	if (READ_BIT(FLASH->CR2, FLASH_CR_LOCK) == 0U) return HAL_ERROR;
  }
  return HAL_OK;
}

/**
  * @brief  STM32 internal flash unlock
  * @note RAM adapted function from HAL Library
  */
__RAM_FUNC HAL_StatusTypeDef FLASH_Unlock(uint32_t bank)
{
  if (bank == FLASH_BANK_1) {
   if(READ_BIT(FLASH->CR1, FLASH_CR_LOCK) != 0U)
   {
    /* Authorize the FLASH Bank1 Registers access */
    WRITE_REG(FLASH->KEYR1, FLASH_KEY1);
    WRITE_REG(FLASH->KEYR1, FLASH_KEY2);

    /* Verify Flash Bank1 is unlocked */
    if (READ_BIT(FLASH->CR1, FLASH_CR_LOCK) != 0U)
    {
      return HAL_ERROR;
    }
   }
  } else { 
   if(READ_BIT(FLASH->CR2, FLASH_CR_LOCK) != 0U)
   {
    /* Authorize the FLASH Bank2 Registers access */
    WRITE_REG(FLASH->KEYR2, FLASH_KEY1);
    WRITE_REG(FLASH->KEYR2, FLASH_KEY2);

    /* Verify Flash Bank2 is unlocked */
    if (READ_BIT(FLASH->CR2, FLASH_CR_LOCK) != 0U)
    {
      return HAL_ERROR;
    }
   }
  }

  return HAL_OK;
}

/**
  * @brief  STM32 internal flash erase
  * @note RAM adapted function from HAL Library
  */
__RAM_FUNC HAL_StatusTypeDef FLASH_Erase(uint32_t bank)
{
	HAL_StatusTypeDef status;
	
	if (FLASH_WaitOperation(bank) != HAL_OK)
    {
      return HAL_ERROR;
    }

	if (bank == FLASH_BANK_1) {
		/* Set voltage range */
		FLASH->CR1 &= (~FLASH_CR_PSIZE);
		FLASH->CR1 |= FLASH_VoltageRange;
		/* Erase Bank1 */
		FLASH->CR1 |= (FLASH_CR_BER | FLASH_CR_START);
	} else {
		/* Set Program/erase VoltageRange for Bank2 */
		FLASH->CR2 &= (~FLASH_CR_PSIZE);
		FLASH->CR2 |= FLASH_VoltageRange;
		/* Erase Bank2 */
		FLASH->CR2 |= (FLASH_CR_BER | FLASH_CR_START);
	}
	
	status =  FLASH_WaitOperation(bank);
	
	if (bank == FLASH_BANK_1) {
		/* if the erase operation is completed, disable the Bank1 BER Bit */
		FLASH->CR1 &= (~FLASH_CR_BER);
	} else {
		/* if the erase operation is completed, disable the Bank2 BER Bit */
        FLASH->CR2 &= (~FLASH_CR_BER);
	}
	
	return status;
}

/**
  * @brief  STM32 internal flash program words
  * @note RAM adapted function from HAL Library
  */
__RAM_FUNC HAL_StatusTypeDef FLASH_Program(uint32_t bank, uint32_t FlashAddress, uint32_t DataAddress)
{
	HAL_StatusTypeDef status;
	__IO uint32_t *dest_addr = (__IO uint32_t *)FlashAddress;
	__IO uint32_t *src_addr = (__IO uint32_t*)DataAddress;
	uint8_t row_index = FLASH_NB_32BITWORD_IN_FLASHWORD;
	
	if(bank == FLASH_BANK_1)
    {
		/* Set PG bit */
        SET_BIT(FLASH->CR1, FLASH_CR_PG);
	} else {
		/* Set PG bit */
		SET_BIT(FLASH->CR2, FLASH_CR_PG);	
	}
	
    __ISB();
    __DSB();

	do {
        *dest_addr = *src_addr;
        dest_addr++;
        src_addr++;
        row_index--;
	} while (row_index != 0U);	
	
    __ISB();
    __DSB();

	status =  FLASH_WaitOperation(bank);

	if(bank == FLASH_BANK_1)
    {
		/* If the program operation is completed, disable the PG */
        CLEAR_BIT(FLASH->CR1, FLASH_CR_PG);
    }
    else
    {
        /* If the program operation is completed, disable the PG */
        CLEAR_BIT(FLASH->CR2, FLASH_CR_PG);
    }	
	
	return status;
}


/**
  * @brief  write firmware to flash
  * @param  [in] fmware_ptr - firmware data
  * @param  [in] size - size in bytes of firmware
  * @retval none
  */
__RAM_FUNC void WriteFirmware(uint8_t *fmware_ptr, uint32_t size)
{
	uint32_t nrows;
	HAL_StatusTypeDef res;
	__IO uint32_t dest_addr = FLASH_FW_ADDRESS;
	__IO uint32_t src_addr = (uint32_t)fmware_ptr;
	
	__disable_irq();

	SCB_DisableDCache();
	SCB_DisableICache();
	
	UART_print_ram(FLASH_msg_start);
	
	res = FLASH_Unlock(FLASH_FW_BANK);
	if (res != HAL_OK) {
		UART_print_ram(FLASH_msg_err_u);
	};
	res = FLASH_Erase(FLASH_FW_BANK);
	if (res != HAL_OK) {
		UART_print_ram(FLASH_msg_err_e);
	};
	
	if (res == HAL_OK) {
	 nrows = size / FLASH_NB_BYTES_IN_FLASHWORD;
	 if (size % FLASH_NB_BYTES_IN_FLASHWORD) nrows++;

	 do {
        if (FLASH_Program(FLASH_FW_BANK, dest_addr, src_addr) != HAL_OK) {
		    UART_print_ram(FLASH_msg_err_w);
			break;
		}
        dest_addr+=FLASH_NB_BYTES_IN_FLASHWORD;
        src_addr+=FLASH_NB_BYTES_IN_FLASHWORD;
        nrows--;
	 } while (nrows != 0U);
	}
	
	FLASH_Lock(FLASH_FW_BANK);

	UART_print_ram(FLASH_msg_end);

#if (FLASH_FW_BANK == FLASH_BANK_1)
	//inline __NVIC_SystemReset();
	__DSB();  // Ensure all outstanding memory accesses included buffered write are completed before reset
	SCB->AIRCR  = (uint32_t)((0x5FAUL << SCB_AIRCR_VECTKEY_Pos)    |
	                           (SCB->AIRCR & SCB_AIRCR_PRIGROUP_Msk) |
	                            SCB_AIRCR_SYSRESETREQ_Msk    );         // Keep priority group unchanged
	__DSB();                                                          // Ensure completion of memory access
	for(;;) // wait until reset
	{
	    __NOP();
	}
#else
	SCB_EnableICache();
	SCB_EnableDCache();
	__enable_irq();
#endif
}
