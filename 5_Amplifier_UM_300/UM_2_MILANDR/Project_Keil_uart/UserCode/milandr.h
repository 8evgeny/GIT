#pragma once
#include "main.h"


class Milandr
{
public:
    Milandr();
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

private:


};
