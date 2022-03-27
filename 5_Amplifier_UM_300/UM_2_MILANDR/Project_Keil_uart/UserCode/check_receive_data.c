#include "main.h"

char checkReceivedByte(uint8_t data)
{
    if ((char)data == fromStm_BlankCommand)
    {
        return 0x00;
    }
    if ((char)data == fromStm_BtnImpedance_On)
    {
        return toStm_BtnImpedance_On_OK;
    }
    if ((char)data == fromStm_BtnImpedance_Off)
    {
        return toStm_BtnImpedance_Off_OK;
    }
    if ((char)data == fromStm_BtnCalibr_On)
    {
        return toStm_BtnCalibr_On_OK;
    }
    if ((char)data == fromStm_BtnCalibr_Off)
    {
        return toStm_BtnCalibr_Off_OK;
    }
    if ((char)data == fromStm_BtnReset_On)
    {
        return toStm_BtnReset_On_OK;
    }
    if ((char)data == fromStm_BtnReset_Off)
    {
        return toStm_BtnReset_Off_OK;
    }
    if ((char)data == fromStm_SignalPowerOn_ON)
    {
        return toStm_SignalPowerOn_ON_OK;
    }
    if ((char)data == fromStm_SignalPowerOn_OFF)
    {
        return toStm_SignalPowerOn_OFF_OK;
    }
    if ((char)data == fromStm_SignalImpedanse_ON)
    {
        return toStm_SignalImpedanse_ON_OK;
    }
    if ((char)data == fromStm_SignalImpedanse_OFF)
    {
        return toStm_SignalImpedanse_OFF_OK;
    }
    if ((char)data == fromStm_SignalTranslate_ON)
    {
        return toStm_SignalTranslate_ON_OK;
    }
    if ((char)data == fromStm_SignalTranslate_OFF)
    {
        return toStm_SignalTranslate_OFF_OK;
    }
    if ((char)data == fromStm_SignalFromOut_ON)
    {
        return toStm_SignalFromOut_ON_OK;
    }
    if ((char)data == fromStm_SignalFromOut_OFF)
    {
        return toStm_SignalFromOut_OFF_OK;
    }
    if ((char)data == fromStm_SignalMic_ON)
    {
        return toStm_SignalMic_ON_OK;
    }
    if ((char)data == fromStm_SignalMic_OFF)
    {
        return toStm_SignalMic_OFF_OK;
    }

    UncnownCmdFromStm = 1;
    return UncnownCmd;
}
