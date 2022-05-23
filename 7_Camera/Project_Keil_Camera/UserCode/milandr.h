#pragma once
#include "main.h"
#include "inputSig.h"
#include <memory>

class Milandr
{
public:
    Milandr(std::shared_ptr<InputSig>);
    ~Milandr();

    void initGPIO();
    void initUART();


private:

    std::shared_ptr<InputSig> _inp;

};
