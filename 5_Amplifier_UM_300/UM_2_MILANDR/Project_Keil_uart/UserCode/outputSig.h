#pragma once
#include "main.h"
#include "milandr.h"
#include <memory>

class OutputSig
{
public:
    OutputSig(std::shared_ptr<Milandr>);
    ~OutputSig();


private:
    std::shared_ptr<Milandr> milandr;
};
