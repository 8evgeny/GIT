#pragma once
#include "main.h"
#include "milandr.h"
#include <memory>

class InputSig
{
public:
    InputSig(std::shared_ptr<Milandr>);
    ~InputSig();


private:
    std::shared_ptr<Milandr> milandr;
};
