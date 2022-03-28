#include "main.h"

bool getMAINPOWER()
{
    if (1 == PORT_ReadInputDataBit(MDR_PORTA, PORT_Pin_0))
    {
        return true;
    }
    return false;
}

bool getRESPOWER()
{
    if (1 == PORT_ReadInputDataBit(MDR_PORTA, PORT_Pin_1))
    {
        return true;
    }
    return false;
}

bool getFAULT() //Инверсный
{
    if (1 == PORT_ReadInputDataBit(MDR_PORTA, PORT_Pin_4))
    {
        return true;
    }
    return false;
}

bool getCLIP_OTW() //Инверсный
{
    if (1 == PORT_ReadInputDataBit(MDR_PORTA, PORT_Pin_5))
    {
        return true;
    }
    return false;
}

bool getPOW_READY()
{
    if (1 == PORT_ReadInputDataBit(MDR_PORTA, PORT_Pin_6))
    {
        return true;
    }
    return false;
}

