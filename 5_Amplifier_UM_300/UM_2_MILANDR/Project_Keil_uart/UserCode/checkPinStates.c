#include "main.h"

void checkPinStates()
{

    //Для отладочной платы 2 кнопки
    uint8_t temp1 = PORT_ReadInputDataBit(MDR_PORTE, PORT_Pin_0);
    uint8_t temp2 = PORT_ReadInputDataBit(MDR_PORTE, PORT_Pin_1);
    if (temp1 != gpioOVERHEAT_60)
    {
        if (temp1)
        {
            if (!OVERHEAT_60)
            {
                OVERHEAT_60 = 1;
                toStmCmdSend = false;
                toStmCmd = cmdToStm_OVERHEAT_60_ON;
            }
        }
        if (!temp1)
        {
            if (OVERHEAT_60)
            {
                OVERHEAT_60 = 0;
                toStmCmdSend = false;
                toStmCmd = cmdToStm_OVERHEAT_60_OFF;
            }
        }
        gpioOVERHEAT_60 = temp1;
    }

    if (temp2 != gpioBOARD_OK)
    {
        if (temp2)
        {
            if (!BOARD_OK)
            {
                BOARD_OK = 1;
                toStmCmdSend = false;
                toStmCmd = cmdToStm_BOARD_OK_ON;
            }
        }
        if (!temp2)
        {
            if (OVERHEAT_60)
            {
                BOARD_OK = 0;
                toStmCmdSend = false;
                toStmCmd = cmdToStm_BOARD_OK_OFF;
            }
        }
        BOARD_OK = temp1;
    }


}
