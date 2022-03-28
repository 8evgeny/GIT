#include "main.h"



#if 0
bool getMAINPOWER();
bool getRESPOWER();
bool getFAULT();    ИНВЕРСНЫЙ
bool getCLIP_OTW(); ИНВЕРСНЫЙ
bool getPOW_READY();
void setGAIN_UPR (bool set);   Режимы D8
void setCOMP_UPR (bool set);   Режимы D8
void setFAN_ON (bool set);
void setRESET_UPR (bool set);  Запуск D2
#endif

//void checkGlobalVar()
//{
//    if(BOARD_OK)
//    {
//        setRESET_UPR(true); //Запуск D2
//        delay(0xFF);
//        setRESET_UPR(false);
//    }
//    if(!BOARD_OK)
//    {
//        setRESET_UPR(true); //Запуск D2
//    }

//}


//void logic()
//{
//    //BOARD_OK
//    if (signalPowerOn &&
//        (getMAINPOWER() || getRESPOWER()) &&
//        getPOW_READY())
//    {
//        tmpBOARD_OK = true;
//    }
//    else
//    {
//        tmpBOARD_OK = false;
//    }

//    if (tmpBOARD_OK != BOARD_OK)
//    {
//        if(tmpBOARD_OK)
//        {
//            toStmCmd = cmdToStm_BOARD_OK_ON;
//        }
//        if(!tmpBOARD_OK)
//        {
//            toStmCmd = cmdToStm_BOARD_OK_OFF;
//        }

//        toStmCmdSend = false;
//        BOARD_OK = tmpBOARD_OK;
//    }

//}
