#include "main.h"

void setGAIN_UPR (bool set)
{
    if(set)  PORT_SetBits(MDR_PORTA, PORT_Pin_2);
    if(!set) PORT_ResetBits(MDR_PORTA, PORT_Pin_2);
}

void setCOMP_UPR (bool set)
{
    if(set)  PORT_SetBits(MDR_PORTA, PORT_Pin_3);
    if(!set) PORT_ResetBits(MDR_PORTA, PORT_Pin_3);
}

void setFAN_ON (bool set)
{
    if(set)  PORT_SetBits(MDR_PORTA, PORT_Pin_7);
    if(!set) PORT_ResetBits(MDR_PORTA, PORT_Pin_7);
}

void setRESET_UPR (bool set)
{
    if(set)  PORT_SetBits(MDR_PORTB, PORT_Pin_8);
    if(!set) PORT_ResetBits(MDR_PORTB, PORT_Pin_8);
}
