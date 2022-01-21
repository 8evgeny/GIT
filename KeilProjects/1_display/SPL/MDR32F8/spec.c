
#include "spec.h"
#include "MDR1986VE8T.h"

#define JTAG_DELAY_EBC 1000
#define JTAG_DELAY_RAM 1000000

#define JTAG_DELAY  JTAG_DELAY_RAM



void POR_disable(void)
{
	int i = 0;
	for (i=0;i<JTAG_DELAY;i++) {}; // Pause to take control in alarm case
	
	BKP->KEY          = _KEY_; // Key to access batary domain control 
			
	BKP->REG_60_TMR0 |= 1<<28; // POR is disable
	BKP->REG_60_TMR1 |= 1<<28; // POR is disable
	BKP->REG_60_TMR2 |= 1<<28; // POR is disable
		
	CLK_CNTR->KEY     = _KEY_; // Key to access clock control 
	FT_CNTR->KEY      = _KEY_; // Key to access fault control  
		
  FT_CNTR->RESET_EVENT0 = 0x0; // hide all errors
  FT_CNTR->RESET_EVENT1 = 0x0; // hide all errors
  FT_CNTR->RESET_EVENT2 = 0x0; // hide all errors
  FT_CNTR->RESET_EVENT3 = 0x0; // hide all errors
  FT_CNTR->RESET_EVENT4 = 0x0; // hide all errors
}

void KEY_reg_accs(void)
{
	PORTA->KEY		=_KEY_;
	PORTB->KEY		=_KEY_;
	PORTC->KEY		=_KEY_;
	PORTD->KEY		=_KEY_;
	PORTE->KEY		=_KEY_;
}


/**
  * @brief  Unlock peripheral unit with a key record.
  * @param  Select the Unit peripheral to unlock.
  *         This parameter can be one of the following values:
  *           @arg ICACHE_key
  *           @arg DCACHE_key
  *           @arg SCACHE_key
  *           @arg FT_CNTR_key
  *           @arg OTP_CNTR_key
  *           @arg RAMD_CNTR_key
  *           @arg ROM_CNTR_key
  *           @arg EXT_BUS_CNTR_key
  *           @arg PWR_key
  *           @arg WDT_key
  *           @arg ADC0_key
  *           @arg ADC1_key
  *           @arg DAC0_key
  *           @arg DAC1_key
  *           @arg BKP_key
  *           @arg CLK_CNTR_key
  *           @arg PORTA_key
  *           @arg PORTB_key
  *           @arg PORTC_key
  *           @arg PORTD_key
  *           @arg PORTE_key
  *           @arg PORTF_key
  * @retval None
  */
void UNLOCK_UNIT (unit_type unit )
{
    switch (unit) 
    {
        case ICACHE_key: ICACHE->KEY = _KEY_;break;
        case DCACHE_key: DCACHE->KEY = _KEY_;break;
        case SCACHE_key: SCACHE->KEY = _KEY_;break;
        case FT_CNTR_key: FT_CNTR->KEY = _KEY_;break;
        case OTP_CNTR_key: OTP_CNTR->KEY = _KEY_;break;
        case RAMD_CNTR_key: RAMD_CNTR->KEY = _KEY_;break;
        case ROM_CNTR_key: ROM_CNTR->KEY = _KEY_;break;
        case EXT_BUS_CNTR_key: EXT_BUS_CNTR->KEY = _KEY_;break;
        case PWR_key: PWR->KEY = _KEY_;break;
        case WDT_key: WDT->KEY = _KEY_;break;
        case ADC0_key: ADC0->KEY = _KEY_;break;
        case ADC1_key: ADC1->KEY = _KEY_;break;
        case DAC0_key: DAC0->KEY = _KEY_;break;
        case DAC1_key: DAC1->KEY = _KEY_;break;
        case BKP_key: BKP->KEY = _KEY_;break;
        case CLK_CNTR_key: CLK_CNTR->KEY = _KEY_;break;
        case PORTA_key: PORTA->KEY = _KEY_;break;
        case PORTB_key: PORTB->KEY = _KEY_;break;
        case PORTC_key: PORTC->KEY = _KEY_;break;
        case PORTD_key: PORTD->KEY = _KEY_;break;
        case PORTE_key: PORTE->KEY = _KEY_;break;
        case PORTF_key: PORTF->KEY = _KEY_;break;
    }
 }
/**
  * @brief  Lock peripheral unit with a clear key record.
  * @param  Select the Unit peripheral to lock.
  *         This parameter can be one of the following values:
  *           @arg ICACHE_key
  *           @arg DCACHE_key
  *           @arg SCACHE_key
  *           @arg FT_CNTR_key
  *           @arg OTP_CNTR_key
  *           @arg RAMD_CNTR_key
  *           @arg ROM_CNTR_key
  *           @arg EXT_BUS_CNTR_key
  *           @arg PWR_key
  *           @arg WDT_key
  *           @arg ADC0_key
  *           @arg ADC1_key
  *           @arg DAC0_key
  *           @arg DAC1_key
  *           @arg BKP_key
  *           @arg CLK_CNTR_key
  *           @arg PORTA_key
  *           @arg PORTB_key
  *           @arg PORTC_key
  *           @arg PORTD_key
  *           @arg PORTE_key
  *           @arg PORTF_key
  * @retval None
  */
void LOCK_UNIT (unit_type unit)
{ 
    switch (unit) 
    {
        case ICACHE_key: ICACHE->KEY = 0;break;
        case DCACHE_key: DCACHE->KEY = 0;break;
        case SCACHE_key: SCACHE->KEY = 0;break;
        case FT_CNTR_key: FT_CNTR->KEY = 0;break;
        case OTP_CNTR_key: OTP_CNTR->KEY = 0;break;
        case RAMD_CNTR_key: RAMD_CNTR->KEY = 0;break;
        case ROM_CNTR_key: ROM_CNTR->KEY = 0;break;
        case EXT_BUS_CNTR_key: EXT_BUS_CNTR->KEY = 0;break;
        case PWR_key: PWR->KEY = 0;break;
        case WDT_key: WDT->KEY = 0;break;
        case ADC0_key: ADC0->KEY = 0;break;
        case ADC1_key: ADC1->KEY = 0;break;
        case DAC0_key: DAC0->KEY = 0;break;
        case DAC1_key: DAC1->KEY = 0;break;
        case BKP_key: BKP->KEY = 0;break;
        case CLK_CNTR_key: CLK_CNTR->KEY = 0;break;
        case PORTA_key: PORTA->KEY = 0;break;
        case PORTB_key: PORTB->KEY = 0;break;
        case PORTC_key: PORTC->KEY = 0;break;
        case PORTD_key: PORTD->KEY = 0;break;
        case PORTE_key: PORTE->KEY = 0;break;
        case PORTF_key: PORTF->KEY = 0;break;
    }
}
