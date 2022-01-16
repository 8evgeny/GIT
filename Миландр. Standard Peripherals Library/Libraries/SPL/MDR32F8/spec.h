#ifndef SPEC_H
#define SPEC_H

#include "MDR1986VE8T.h"

#define _KEY_ 				0x8555AAA1

typedef enum {
    ICACHE_key= 0,
    DCACHE_key,
    SCACHE_key,
    FT_CNTR_key,
    OTP_CNTR_key,
    RAMD_CNTR_key,
    ROM_CNTR_key,
    EXT_BUS_CNTR_key,
    PWR_key,
    WDT_key,
    ADC0_key,
    ADC1_key,
    DAC0_key,
    DAC1_key,
    BKP_key,
    CLK_CNTR_key,
    PORTA_key,
    PORTB_key,
    PORTC_key,
    PORTD_key,
    PORTE_key,
    PORTF_key,
    
} unit_type;

void POR_disable(void);
void KEY_reg_accs(void);
void UNLOCK_UNIT (unit_type unit );
void LOCK_UNIT (unit_type unit);


#endif //SPEC_H
