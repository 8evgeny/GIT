#include "main.h"

void logic()
{
    //BOARD_OK
    if (getMAINPOWER() && getRESPOWER() && getPOW_READY())
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
            toStmCmd = fromStm_BOARD_OK_ON_OK;
        }
        if(!tmpBOARD_OK)
        {
            toStmCmd = fromStm_BOARD_OK_OFF_OK;
        }
        toStmCmdSend = false;
        BOARD_OK = tmpBOARD_OK;
    }




}
