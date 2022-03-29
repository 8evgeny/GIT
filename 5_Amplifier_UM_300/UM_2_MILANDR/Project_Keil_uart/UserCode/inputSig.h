#pragma once
#include "main.h"
#include "milandr.h"
#include <memory>

class InputSig
{
public:
    InputSig(std::shared_ptr<Milandr>);
    ~InputSig();

    char checkReceivedByte(uint8_t data);

private:
    //Stm -> Milandr
    bool btnImpedance = 0;
    bool btnCalibrovka = 0;
    bool btnReset = 0;
    bool signalPowerOn = 0;
    bool signalImpedance = 0;
    bool signalTranslate = 0;
    bool input_IMP_UPR = 0;
    bool input_VOLT_UPR = 0;
    bool input_CUR_UPR1 = 0;
    bool input_CUR_UPR2 = 0;
    bool signalVnesh = 0;
    bool signalMic = 0;


    std::shared_ptr<Milandr> milandr;
};
