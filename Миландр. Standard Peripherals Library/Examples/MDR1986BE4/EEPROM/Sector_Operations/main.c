/**
  ******************************************************************************
  * @file    main.c
  * @author  Milandr Application Team
  * @version V2.0.2
  * @date    23/09/2021
  * @brief   Main program body.
  ******************************************************************************
  * <br><br>
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, MILANDR SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2021 Milandr</center></h2>
  */

/* Includes ------------------------------------------------------------------*/

#include "MDR32F9Qx_eeprom.h"
#include "MDR32F9Qx_port.h"
#include "MDR32F9Qx_rst_clk.h"

/** @addtogroup __MDR1986BE4_StdPeriph_Examples MDR1986BE4 StdPeriph Examples
  * @{
  */

/** @addtogroup __MDR1986BE4_EVAL MDR1986BE4 Evaluation Board
  * @{
  */

/** @addtogroup EEPROM_Sector_Operations EEPROM_Sector_Operations
  * @{
  */

/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/
#define EEPROM_PAGE_SIZE             (512)
#define MAIN_EEPAGE                  64
#define LED_MASK                     (PORT_Pin_11)
#define LED0_MASK                    PORT_Pin_11

/* Private macro -------------------------------------------------------------*/
#define LED_TST_NUM(num)             (((num) * LED0_MASK) & LED_MASK)

/* Private variables ---------------------------------------------------------*/
static PORT_InitTypeDef PORT_InitStructure;
uint8_t EraseErrMM;
uint8_t WriteErrMM;
uint8_t EraseErrIM;
uint8_t WriteErrIM;

/* Private function prototypes -----------------------------------------------*/

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Generation random values
  * @param  addr - memory address
  * @retval None
  */
uint32_t Pseudo_Rand(uint32_t addr)
{
    uint32_t hash = 0;
    uint32_t i = 0;
    uint8_t* key = (uint8_t *)&addr;

    for (i = 0; i < 4; i++)
    {
        hash += key[i];
        hash += (hash << 10);
        hash ^= (hash >> 6);
    }

    for (i = 0; i < 256; i++)
    {
        hash += (hash << 10);
        hash ^= (hash >> 6);
    }

    hash += (hash << 3);
    hash ^= (hash >> 11);
    hash += (hash << 15);
    return hash;
}

/**
  * @brief  Delay function
  * @param  None
  * @retval None
  */
void Delay(void)
{
    volatile uint32_t i = 0;
    for (i = 0; i < 100000; i++)
    {
    }
}

/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */
int main(void)
{
    uint32_t Address = 0;
    uint32_t Data = 0;
    uint32_t i = 0;
    uint32_t Leds = 0;
    EEPROM_Mem_Bank BankSelector = EEPROM_Main_Bank_Select;

    /* Enables the clock on PORTA */
    RST_CLK_PCLKcmd(RST_CLK_PCLK_PORTA, ENABLE);
    /* Enables the clock on EEPROM */
    RST_CLK_PCLKcmd(RST_CLK_PCLK_EEPROM, ENABLE);

    /* Configure PORTA pins 11 for output to switch LED on/off */
    PORT_InitStructure.PORT_Pin   = LED_MASK;
    PORT_InitStructure.PORT_OE    = PORT_OE_OUT;
    PORT_InitStructure.PORT_FUNC  = PORT_FUNC_PORT;
    PORT_InitStructure.PORT_MODE  = PORT_MODE_DIGITAL;
    PORT_InitStructure.PORT_SPEED = PORT_SPEED_SLOW;

    PORT_Init(MDR_PORTA, &PORT_InitStructure);

    /* Erase main memory page MAIN_EEPAGE */
    Address = 0x00000000 + MAIN_EEPAGE * EEPROM_PAGE_SIZE;
    BankSelector = EEPROM_Main_Bank_Select;
    EEPROM_ErasePage (Address, BankSelector);

    /* Check main memory page MAIN_EEPAGE */
    Data = 0xFFFFFFFF;
    for (i = 0; i < EEPROM_PAGE_SIZE; i += 4)
    {
        if (EEPROM_ReadWord (Address + i, BankSelector) != Data)
        {
            EraseErrMM = 1;
        }
    }

    /* Indicate status of erasing main memory page MAIN_EEPAGE */
    Leds = LED_TST_NUM(0x1);
    if (EraseErrMM == 0)
    {
        PORT_Write(MDR_PORTA, Leds);
    }
    else
    {
        while (1)
        {
            Leds ^= LED_TST_NUM(0x1);
            PORT_Write(MDR_PORTA, Leds);
            Delay();
        }
    }

    /* Fill main memory page MAIN_EEPAGE */
    Address = 0x00000000 + MAIN_EEPAGE * EEPROM_PAGE_SIZE;
    BankSelector = EEPROM_Main_Bank_Select;
    for (i = 0; i < EEPROM_PAGE_SIZE; i += 4)
    {
        Data = Pseudo_Rand (Address + i);
        EEPROM_ProgramWord (Address + i, BankSelector, Data);
    }

    /* Check main memory page MAIN_EEPAGE */
    Address = 0x00000000 + MAIN_EEPAGE * EEPROM_PAGE_SIZE;
    BankSelector = EEPROM_Main_Bank_Select;
    for (i = 0; i < EEPROM_PAGE_SIZE; i +=4 )
    {
        Data = Pseudo_Rand (Address + i);
        if (EEPROM_ReadWord (Address + i, BankSelector) != Data)
        {
            WriteErrMM = 1;
        }
    }

    /* Indicate status of writing main memory page MAIN_EEPAGE */
    Leds = LED_TST_NUM(0x2);
    if (EraseErrMM == 0)
    {
        PORT_Write(MDR_PORTA, Leds);
    }
    else
    {
        while (1)
        {
            Leds ^= LED_TST_NUM(0x2);
            PORT_Write(MDR_PORTA, Leds);
            Delay();
        }
    }

    /* Erase main memory page MAIN_EEPAGE */
    Address = MAIN_EEPAGE * EEPROM_PAGE_SIZE;
    BankSelector = EEPROM_Main_Bank_Select;
    EEPROM_ErasePage (Address, BankSelector);

    /* Check main memory page MAIN_EEPAGE */
    Data = 0xFFFFFFFF;
    for (i = 0; i < EEPROM_PAGE_SIZE; i += 4)
    {
        if (EEPROM_ReadWord (Address + i, BankSelector) != Data)
        {
            EraseErrMM = 1;
        }
    }

    /* Indicate status of erasing main memory page MAIN_EEPAGE */
    Leds = LED_TST_NUM(0x3);
    if (EraseErrMM == 0)
    {
        PORT_Write(MDR_PORTA, Leds);
    }
    else
    {
        while (1)
        {
            Leds ^= LED_TST_NUM(0x3);
            PORT_Write(MDR_PORTA, Leds);
            Delay();
        }
    }

    while (1);
}

/**
  * @brief  Reports the source file name, the source line number
  *         and expression text (if USE_ASSERT_INFO == 2) where
  *         the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @param  expr:
  * @retval None
  */
#if (USE_ASSERT_INFO == 1)
void assert_failed(uint8_t* file, uint32_t line)
{
    /* User can add his own implementation to report the source file name and line number.
       Ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

    /* Infinite loop */
    while (1)
    {
    }
}
#elif (USE_ASSERT_INFO == 2)
void assert_failed(uint8_t* file, uint32_t line, const uint8_t* expr)
{
    /* User can add his own implementation to report the source file name, line number and
       expression text.
       Ex: printf("Wrong parameters value (%s): file %s on line %d\r\n", expr, file, line) */

    /* Infinite loop */
    while (1)
    {
    }
}
#endif /* USE_ASSERT_INFO */

/** @} */ /* End of group EEPROM_Sector_Operations */

/** @} */ /* End of group __MDR1986BE4_EVAL */

/** @} */ /* End of group __MDR1986BE4_StdPeriph_Examples */

/******************* (C) COPYRIGHT 2021 Milandr ********************************
*
* END OF FILE main.c */

