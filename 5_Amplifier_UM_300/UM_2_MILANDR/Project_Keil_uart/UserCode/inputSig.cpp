#include "main.h"
#include "inputSig.h"

InputSig::InputSig(std::shared_ptr<Milandr> mil)
{
    milandr = mil;

}

InputSig::~InputSig()
{
}


char InputSig::checkReceivedByte(uint8_t data)
{
    if ((char)data == fromStm_BlankCommand)
    {
        return 0x00;
    }
    if ((char)data == fromStm_BtnImpedance_On)
    {
        btnImpedance = true;
        return toStm_BtnImpedance_On_OK;
    }
    if ((char)data == fromStm_BtnImpedance_Off)
    {
        btnImpedance = false;
        return toStm_BtnImpedance_Off_OK;
    }
    if ((char)data == fromStm_BtnCalibr_On)
    {
        btnCalibrovka = true;
        return toStm_BtnCalibr_On_OK;
    }
    if ((char)data == fromStm_BtnCalibr_Off)
    {
        btnCalibrovka = false;
        return toStm_BtnCalibr_Off_OK;
    }
    if ((char)data == fromStm_BtnReset_On)
    {
        btnReset = true;
        return toStm_BtnReset_On_OK;
    }
    if ((char)data == fromStm_BtnReset_Off)
    {
        btnReset = false;
        return toStm_BtnReset_Off_OK;
    }
    if ((char)data == fromStm_SignalPowerOn_ON)
    {
        signalPowerOn = true;
        return toStm_SignalPowerOn_ON_OK;
    }
    if ((char)data == fromStm_SignalPowerOn_OFF)
    {
        signalPowerOn = false;
        return toStm_SignalPowerOn_OFF_OK;
    }
    if ((char)data == fromStm_SignalImpedanse_ON)
    {
        signalImpedance = true;
        return toStm_SignalImpedanse_ON_OK;
    }
    if ((char)data == fromStm_SignalImpedanse_OFF)
    {
        signalImpedance = false;
        return toStm_SignalImpedanse_OFF_OK;
    }
    if ((char)data == fromStm_SignalTranslate_ON)
    {
        signalTranslate = true;
        return toStm_SignalTranslate_ON_OK;
    }
    if ((char)data == fromStm_SignalTranslate_OFF)
    {
        signalTranslate = false;
        return toStm_SignalTranslate_OFF_OK;
    }
    if ((char)data == fromStm_SignalFromOut_ON)
    {
        signalVnesh = true;
        return toStm_SignalFromOut_ON_OK;
    }
    if ((char)data == fromStm_SignalFromOut_OFF)
    {
        signalVnesh = false;
        return toStm_SignalFromOut_OFF_OK;
    }
    if ((char)data == fromStm_SignalMic_ON)
    {
        signalMic = true;
        return toStm_SignalMic_ON_OK;
    }
    if ((char)data == fromStm_SignalMic_OFF)
    {
        signalMic = false;
        return toStm_SignalMic_OFF_OK;
    }

    return 0x00;
}
