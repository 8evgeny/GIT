#pragma once
#include "main.h"
#include "milandr.h"
#include <memory>

class TPA3255_Amplifier
{
public:
    TPA3255_Amplifier(std::shared_ptr<Milandr>);
    ~TPA3255_Amplifier();
    void reset();
//Прописать функции Overheate


private:
    std::shared_ptr<Milandr> milandr;
};
