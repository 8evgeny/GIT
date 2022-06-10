/**
  ******************************************************************************
  * @file    MDR32F9Qx_eeprom.c
  * @author  Milandr Application Team
  * @version V2.0.2
  * @date    23/09/2021
  * @brief   This file contains all the EEPROM firmware functions.
  ******************************************************************************
  * <br><br>
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, MILANDR SHALL NOT BE HELD LIABLE FOR ANY DIRECT, INDIRECT
  * OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2021 Milandr</center></h2>
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "MDR32F9Qx_eeprom.h"

/** @addtogroup __MDR1986BE4_StdPeriph_Driver MDR1986BE4 Standard Peripherial Driver
  * @{
  */

/** @defgroup EEPROM EEPROM
  * @{
  */

/** @defgroup EEPROM_Private_Defines EEPROM Private Defines
  * @{
  */

#define EEPROM_REG_ACCESS_KEY           ((uint32_t)0x8AAA5551)
#define EEPROM_START_ADDR               ((uint32_t)0x00000000)

/** @} */ /* End of group EEPROM_Private_Defines */


/** @defgroup EEPROM_Private_Macros EEPROM Private Macros
  * @{
  */

#define IS_TWO_BYTE_ALLIGNED(ADDR)  ((ADDR & 1) == 0)
#define IS_FOUR_BYTE_ALLIGNED(ADDR) ((ADDR & 3) == 0)

#define DELAY_LOOP_CYCLES           (8UL)
#define GET_US_LOOPS(N)             ((uint32_t)((float)(N) * FLASH_PROG_FREQ_MHZ / DELAY_LOOP_CYCLES))

/** @} */ /* End of group EEPROM_Private_Macros */


/** @defgroup EEPROM_Private_Function_Prototypes EEPROM Private Functions Prototypes
  * @{
  */

__RAMFUNC static void ProgramDelay(uint32_t Loops) __attribute__((section("EXECUTABLE_MEMORY_SECTION")));

/** @} */ /* End of group EEPROM_Private_Function_Prototypes */


/** @defgroup EEPROM_Private_Functions EEPROM Private Functions
  * @{
  */

/**
  * @brief  Program delay.
  * @param  Loops: Number of the loops.
  * @retval None.
  */
__RAMFUNC static void ProgramDelay(uint32_t Loops)
{
    volatile uint32_t i = Loops;
    for (; i > 0; i--);
}

/**
  * @brief  Sets the code latency value.
  * @param  FLASH_Latency - @ref EEPROM_Latency_Cycles - specifies the FLASH Latency value.
  * @retval None.
  */
void EEPROM_SetLatency(EEPROM_Latency_Cycles EEPROM_Latency)
{
    /* Check the parameters */
    assert_param(IS_EEPROM_LATENCY(EEPROM_Latency));

    /* Set the new latency value */
    MDR_EEPROM->CMD = (uint32_t)EEPROM_Latency;
}

/**
  * @brief  Gets the code latency value.
  * @param  None.
  * @retval @ref EEPROM_Latency_Cycles - EEPROM Latency cycles value.
  */
EEPROM_Latency_Cycles EEPROM_GetLatency(void)
{
    /* Get current latency value */
    return (EEPROM_Latency_Cycles) ((MDR_EEPROM->CMD & EEPROM_Latency_MSK) >> EEPROM_CMD_DELAY_Pos);
}

/**
  * @brief  Reads the 8-bit EEPROM memory value.
  * @note   Hardware supports only 32-bit word reading.
  * @param  Address: The EEPROM memory byte address.
  * @param  BankSelector - @ref EEPROM_Mem_Bank - Selects EEPROM Bank.
  * @retval The selected EEPROM memory value.
  */
__RAMFUNC uint8_t EEPROM_ReadByte(uint32_t Address, EEPROM_Mem_Bank BankSelector)
{
    uint32_t Data;
    uint32_t Command;
    uint32_t Shift;

    assert_param(IS_EEPROM_BANK_SELECTOR(BankSelector));

    MDR_EEPROM->KEY = EEPROM_REG_ACCESS_KEY;
    Command = (MDR_EEPROM->CMD & EEPROM_CMD_DELAY_Msk) | EEPROM_CMD_CON;
    Command |= (BankSelector == EEPROM_Info_Bank_Select) ? EEPROM_CMD_IFREN : 0;
    MDR_EEPROM->CMD = Command;
    MDR_EEPROM->ADR = Address;
    MDR_EEPROM->CMD = Command | EEPROM_CMD_XE | EEPROM_CMD_YE | EEPROM_CMD_SE;
    MDR_EEPROM->DO;   /* Idle Reading for Delay */
    MDR_EEPROM->DO;   /* Idle Reading for Delay */
    MDR_EEPROM->DO;   /* Idle Reading for Delay */
    Data = MDR_EEPROM->DO;
    Command &= EEPROM_CMD_DELAY_Msk;
    MDR_EEPROM->CMD = Command;
    MDR_EEPROM->KEY = 0;

    Shift = (Address & 3) * 8;
    Data >>= Shift;

    return (uint8_t)Data;
}

/**
  * @brief  Reads the 16-bit EEPROM memory value.
  * @note   Hardware supports only 32-bit word reading.
  * @param  Address: The EEPROM memory half word address (two byte aligned).
  * @param  BankSelector - @ref EEPROM_Mem_Bank - Selects EEPROM Bank.
  * @retval The selected EEPROM memory value.
  */
__RAMFUNC uint16_t EEPROM_ReadHalfWord(uint32_t Address, EEPROM_Mem_Bank BankSelector)
{
    uint32_t Data;
    uint32_t Command;
    uint32_t Shift;

    assert_param(IS_EEPROM_BANK_SELECTOR(BankSelector));
    assert_param(IS_TWO_BYTE_ALLIGNED(Address));

    MDR_EEPROM->KEY = EEPROM_REG_ACCESS_KEY;
    Command = (MDR_EEPROM->CMD & EEPROM_CMD_DELAY_Msk) | EEPROM_CMD_CON;
    Command |= (BankSelector == EEPROM_Info_Bank_Select) ? EEPROM_CMD_IFREN : 0;
    MDR_EEPROM->CMD = Command;
    MDR_EEPROM->ADR = Address;
    MDR_EEPROM->CMD = Command | EEPROM_CMD_XE | EEPROM_CMD_YE | EEPROM_CMD_SE;
    MDR_EEPROM->DO;   /* Idle Reading for Delay */
    MDR_EEPROM->DO;   /* Idle Reading for Delay */
    MDR_EEPROM->DO;   /* Idle Reading for Delay */
    Data = MDR_EEPROM->DO;
    Command &= EEPROM_CMD_DELAY_Msk;
    MDR_EEPROM->CMD = Command;
    MDR_EEPROM->KEY = 0;

    Shift = (Address & 2) * 8;
    Data >>= Shift;

    return (uint16_t)Data;
}

/**
  * @brief  Reads the 32-bit EEPROM memory value.
  * @param  Address: The EEPROM memory word address (four byte aligned).
  * @param  BankSelector - @ref EEPROM_Mem_Bank - Selects EEPROM Bank.
  * @retval The selected EEPROM memory value.
  */
__RAMFUNC uint32_t EEPROM_ReadWord(uint32_t Address, EEPROM_Mem_Bank BankSelector)
{
    uint32_t Command;
    uint32_t Data;

    assert_param(IS_EEPROM_BANK_SELECTOR(BankSelector));
    assert_param(IS_FOUR_BYTE_ALLIGNED(Address));

    MDR_EEPROM->KEY = EEPROM_REG_ACCESS_KEY;
    Command = (MDR_EEPROM->CMD & EEPROM_CMD_DELAY_Msk) | EEPROM_CMD_CON;
    Command |= (BankSelector == EEPROM_Info_Bank_Select) ? EEPROM_CMD_IFREN : 0;
    MDR_EEPROM->CMD = Command;
    MDR_EEPROM->ADR = Address;
    MDR_EEPROM->CMD = Command | EEPROM_CMD_XE | EEPROM_CMD_YE | EEPROM_CMD_SE;
    MDR_EEPROM->DO;   /* Idle Reading for Delay */
    MDR_EEPROM->DO;   /* Idle Reading for Delay */
    MDR_EEPROM->DO;   /* Idle Reading for Delay */
    Data = MDR_EEPROM->DO;
    Command &= EEPROM_CMD_DELAY_Msk;
    MDR_EEPROM->CMD = Command;
    MDR_EEPROM->KEY = 0;

    return Data;
}

/**
  * @brief  Erases one page of the selected EEPROM memory bank.
  * @param  Address: Page Address in the EEPROM memory.
  * @param  BankSelector - @ref EEPROM_Mem_Bank - Selects EEPROM Bank.
  * @retval None
  */
__RAMFUNC void EEPROM_ErasePage(uint32_t Address, EEPROM_Mem_Bank BankSelector)
{
    uint32_t Command;

    assert_param(IS_EEPROM_BANK_SELECTOR(BankSelector));

    MDR_EEPROM->KEY = EEPROM_REG_ACCESS_KEY;
    Command = (MDR_EEPROM->CMD & EEPROM_CMD_DELAY_Msk) | EEPROM_CMD_CON;
    Command |= (BankSelector == EEPROM_Info_Bank_Select) ? EEPROM_CMD_IFREN : 0;
    MDR_EEPROM->CMD = Command;

    MDR_EEPROM->ADR = Address;          /* Page Address */
    MDR_EEPROM->DI = 0;
    Command |= EEPROM_CMD_XE | EEPROM_CMD_ERASE;
    MDR_EEPROM->CMD = Command;
    ProgramDelay(GET_US_LOOPS(10));     /* Wait for 10 us */
    Command |= EEPROM_CMD_NVSTR;
    MDR_EEPROM->CMD = Command;
    ProgramDelay(GET_US_LOOPS(40000));  /* Wait for 40 ms */
    Command &= ~EEPROM_CMD_ERASE;
    MDR_EEPROM->CMD = Command;
    ProgramDelay(GET_US_LOOPS(5));      /* Wait for 5 us */
    Command &= ~(EEPROM_CMD_XE | EEPROM_CMD_NVSTR);
    MDR_EEPROM->CMD = Command;
    ProgramDelay(GET_US_LOOPS(1));      /* Wait for 1 us */

    Command &= EEPROM_CMD_DELAY_Msk;
    MDR_EEPROM->CMD = Command;
    MDR_EEPROM->KEY = 0;
}

/**
  * @brief  Erases one block of the selected EEPROM memory bank.
  * @param  Address: Block Address in the EEPROM memory.
  * @param  BankSelector - @ref EEPROM_Mem_Bank - Selects EEPROM Bank.
  * @retval None
  */
__RAMFUNC void EEPROM_EraseBlock(uint32_t Address, EEPROM_Mem_Bank BankSelector)
{
    uint32_t Command;

    assert_param(IS_EEPROM_BANK_SELECTOR(BankSelector));

    MDR_EEPROM->KEY = EEPROM_REG_ACCESS_KEY;
    Command = (MDR_EEPROM->CMD & EEPROM_CMD_DELAY_Msk) | EEPROM_CMD_CON;
    Command |= (BankSelector == EEPROM_Info_Bank_Select) ? EEPROM_CMD_IFREN : 0;

    MDR_EEPROM->CMD = Command;

    MDR_EEPROM->ADR = Address;
    MDR_EEPROM->DI = 0;
    Command |= EEPROM_CMD_XE | EEPROM_CMD_MAS1 | EEPROM_CMD_ERASE;
    MDR_EEPROM->CMD = Command;
    ProgramDelay(GET_US_LOOPS(10));                /* Wait for 10 us */
    Command |= EEPROM_CMD_NVSTR;
    MDR_EEPROM->CMD = Command;
    ProgramDelay(GET_US_LOOPS(40000));            /* Wait for 40 ms */
    Command &= ~EEPROM_CMD_ERASE;
    MDR_EEPROM->CMD = Command;
    ProgramDelay(GET_US_LOOPS(100));              /* Wait for 100 us */
    Command &= ~(EEPROM_CMD_XE | EEPROM_CMD_MAS1 | EEPROM_CMD_NVSTR);
    MDR_EEPROM->CMD = Command;
    ProgramDelay(GET_US_LOOPS(1));                /* Wait for 1 us */

    Command &= EEPROM_CMD_DELAY_Msk;
    MDR_EEPROM->CMD = Command;
    MDR_EEPROM->KEY = 0;
}

/**
  * @brief  Programs the 8-bit EEPROM memory value.
  * @note   Hardware supports only 32-bit word programming.
  * @param  Address: The EEPROM memory byte address.
  * @param  BankSelector - @ref EEPROM_Mem_Bank - Selects EEPROM Bank.
  * @param  Data: The data value to be programmed.
  * @retval None
  */
__RAMFUNC void EEPROM_ProgramByte(uint32_t Address, EEPROM_Mem_Bank BankSelector, uint32_t Data)
{
    uint32_t Mask;
    uint32_t Tmp;
    uint32_t Shift;

    assert_param(IS_EEPROM_BANK_SELECTOR(BankSelector));

    Shift = (Address & 3) * 8;
    Data = Data << Shift;
    Mask = 0xFF << Shift;

    Tmp = EEPROM_ReadWord(Address, BankSelector);

    EEPROM_ProgramWord(Address, BankSelector, (Tmp & ~Mask) | (Data & Mask));
}

/**
  * @brief  Programs the 16-bit EEPROM memory value.
  * @note   Hardware supports only 32-bit word programming.
  * @param  Address: The EEPROM memory half word address (two byte aligned).
  * @param  BankSelector - @ref EEPROM_Mem_Bank - Selects EEPROM Bank.
  * @param  Data: The data value to be programmed.
  * @retval None
  */
__RAMFUNC void EEPROM_ProgramHalfWord(uint32_t Address, EEPROM_Mem_Bank BankSelector, uint32_t Data)
{
    uint32_t Mask;
    uint32_t Tmp;
    uint32_t Shift;

    assert_param(IS_EEPROM_BANK_SELECTOR(BankSelector));
    assert_param(IS_TWO_BYTE_ALLIGNED(Address));

    Shift = (Address & 2) * 8;
    Data = Data << Shift;
    Mask = 0xFFFF << Shift;

    Tmp = EEPROM_ReadWord(Address, BankSelector);

    EEPROM_ProgramWord(Address, BankSelector, (Tmp & ~Mask) | (Data & Mask));
}

/**
  * @brief  Programs the 32-bit EEPROM memory value.
  * @param  Address: The EEPROM memory word address (four byte aligned).
  * @param  BankSelector - @ref EEPROM_Mem_Bank - Selects EEPROM Bank.
  * @param  Data: The data value to be programmed.
  * @retval None
  */
__RAMFUNC void EEPROM_ProgramWord(uint32_t Address, EEPROM_Mem_Bank BankSelector, uint32_t Data)
{
    uint32_t Command;

    assert_param(IS_EEPROM_BANK_SELECTOR(BankSelector));
    assert_param(IS_FOUR_BYTE_ALLIGNED(Address));

    MDR_EEPROM->KEY = EEPROM_REG_ACCESS_KEY;
    Command = MDR_EEPROM->CMD & EEPROM_CMD_DELAY_Msk;
    Command |= EEPROM_CMD_CON;
    Command |= (BankSelector == EEPROM_Info_Bank_Select) ? EEPROM_CMD_IFREN : 0;
    MDR_EEPROM->CMD = Command;
    MDR_EEPROM->ADR = Address;
    MDR_EEPROM->DI  = Data;
    Command |= EEPROM_CMD_XE | EEPROM_CMD_PROG;
    MDR_EEPROM->CMD = Command;
    ProgramDelay(GET_US_LOOPS(5));                /* Wait for 5 us */
    Command |= EEPROM_CMD_NVSTR;
    MDR_EEPROM->CMD = Command;
    ProgramDelay(GET_US_LOOPS(10));               /* Wait for 10 us */
    Command |= EEPROM_CMD_YE;
    MDR_EEPROM->CMD = Command;
    ProgramDelay(GET_US_LOOPS(40));               /* Wait for 40 us */
    Command &= ~EEPROM_CMD_YE;
    MDR_EEPROM->CMD = Command;
    Command &= ~EEPROM_CMD_PROG;
    MDR_EEPROM->CMD = Command;
    ProgramDelay(GET_US_LOOPS(5));                /* Wait for 5 us */
    Command &= ~(EEPROM_CMD_XE | EEPROM_CMD_NVSTR);
    MDR_EEPROM->CMD = Command;
    ProgramDelay(GET_US_LOOPS(1));                /* Wait for 1 us */

    MDR_EEPROM->CMD = Command & EEPROM_CMD_DELAY_Msk;
    MDR_EEPROM->KEY = 0;
}

/**
  * @brief   Updates data cache.
  * @param   None
  * @retval  None
  */
__RAMFUNC void EEPROM_UpdateDCache(void)
{
    volatile uint32_t i, tmp;
    volatile uint32_t *ptr_eeprom = (uint32_t *) (EEPROM_START_ADDR);

    /* Reading EEPROM memory to update the data cache */
    for(i = 2; i > 0; i--)
    {
        tmp = ptr_eeprom[i];
    }
}

/** @} */ /* End of group EEPROM_Private_Functions */

/** @} */ /* End of group EEPROM */

/** @} */ /* End of group __MDR1986BE4_StdPeriph_Driver */

/*********************** (C) COPYRIGHT 2021 Milandr ****************************
*
* END OF FILE MDR32F9Qx_eeprom.c */
