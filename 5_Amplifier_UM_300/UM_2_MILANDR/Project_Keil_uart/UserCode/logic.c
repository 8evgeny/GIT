#include "main.h"

void logic()
{
    //BOARD_OK
//    if (getMAINPOWER() && getRESPOWER() && getPOW_READY())
if (true)
    {
        tmpBOARD_OK = true;
    }
    else
    {
        tmpBOARD_OK = false;
    }

    if (tmpBOARD_OK != BOARD_OK)
    {
        if(tmpBOARD_OK)
        {
            toStmCmd = cmdToStm_BOARD_OK_ON;
        }
        if(!tmpBOARD_OK)
        {
            toStmCmd = cmdToStm_BOARD_OK_OFF;
        }
        toStmCmdSend = false;
        BOARD_OK = tmpBOARD_OK;
    }




}
