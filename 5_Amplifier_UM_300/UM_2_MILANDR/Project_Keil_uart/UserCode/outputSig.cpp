#include "main.h"
#include "outputSig.h"

OutputSig::OutputSig(std::shared_ptr<Milandr> mil)
{
    milandr = mil;

}

OutputSig::~OutputSig()
{
}


bool OutputSig::getOVERHEAT_MC() const
{
    return OVERHEAT_MC;
}

void OutputSig::setOVERHEAT_MC(bool newOVERHEAT_MC)
{
    OVERHEAT_MC = newOVERHEAT_MC;
}

bool OutputSig::getERROR_MC() const
{
    return ERROR_MC;
}

void OutputSig::setERROR_MC(bool newERROR_MC)
{
    ERROR_MC = newERROR_MC;
}

bool OutputSig::getTmpOVERHEAT_MC() const
{
    return tmpOVERHEAT_MC;
}

void OutputSig::setTmpOVERHEAT_MC(bool newTmpOVERHEAT_MC)
{
    tmpOVERHEAT_MC = newTmpOVERHEAT_MC;
}

bool OutputSig::getTmpBOARD_OK() const
{
    return tmpBOARD_OK;
}

void OutputSig::setTmpBOARD_OK(bool newTmpBOARD_OK)
{
    tmpBOARD_OK = newTmpBOARD_OK;
}

bool OutputSig::getTmpERROR_MC() const
{
    return tmpERROR_MC;
}

void OutputSig::setTmpERROR_MC(bool newTmpERROR_MC)
{
    tmpERROR_MC = newTmpERROR_MC;
}

bool OutputSig::getBOARD_OK() const
{
    return BOARD_OK;
}

void OutputSig::setBOARD_OK(bool newBOARD_OK)
{
    BOARD_OK = newBOARD_OK;
}

bool OutputSig::getToStmCmdSend() const
{
    return toStmCmdSend;
}

void OutputSig::setToStmCmdSend(bool newToStmCmdSend)
{
    toStmCmdSend = newToStmCmdSend;
}

char OutputSig::getToStmCmd() const
{
    return toStmCmd;
}

void OutputSig::checkOVERHEAT_MC()
{
    if (OVERHEAT_MC != tmpOVERHEAT_MC)
    {
        if(OVERHEAT_MC)
        {
            toStmCmd = cmdToStm_OVERHEAT_MC_ON;
        }
        if(!OVERHEAT_MC)
        {
            toStmCmd = cmdToStm_OVERHEAT_MC_OFF;
        }
        toStmCmdSend = false;
        tmpOVERHEAT_MC = OVERHEAT_MC;
    }
}

void OutputSig::checkBOARD_OK()
{
    if (BOARD_OK != tmpBOARD_OK)
    {
        if(BOARD_OK)
        {
            toStmCmd = cmdToStm_BOARD_OK_ON;
        }
        if(!BOARD_OK)
        {
            toStmCmd = cmdToStm_BOARD_OK_OFF;
        }
        toStmCmdSend = false;
        tmpBOARD_OK = BOARD_OK;
    }
}

void OutputSig::checkERROR_MC()
{
    if (ERROR_MC != tmpERROR_MC)
    {
        if(ERROR_MC)
        {
            toStmCmd = cmdToStm_ERROR_MC_ON;
        }
        if(!ERROR_MC)
        {
            toStmCmd = cmdToStm_ERROR_MC_OFF;
        }
        toStmCmdSend = false;
        tmpERROR_MC = ERROR_MC;
    }
}


