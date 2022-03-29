#pragma once
#include "main.h"
#include "inputSig.h"


class Milandr
{
public:
    Milandr(std::shared_ptr<InputSig>);
    ~Milandr();

    void initGPIO();
    void initUART();

    bool getMAINPOWER();
    bool getRESPOWER();
    bool getFAULT();
    bool getCLIP_OTW();
    bool getPOW_READY();
    void setGAIN_UPR (bool set);
    void setCOMP_UPR (bool set);
    void setFAN_ON (bool set);
    void setRESET_UPR (bool set);

    bool isPowerOk();

private:
    bool StmNoReceiveCmd = 0;
    bool UncnownCmdFromStm = 0;

    std::shared_ptr<InputSig> _inp;

};
