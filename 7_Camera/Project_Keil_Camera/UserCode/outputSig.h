#pragma once
#include "main.h"
#include "milandr.h"
#include <memory>

class OutputSig
{
public:
    OutputSig(std::shared_ptr<Milandr>);
    ~OutputSig();

    bool getOVERHEAT_MC() const;

    bool getMoveOn() const;
    void setMoveOn(bool newMoveOn);

private:
    bool _moveOn;
    std::shared_ptr<Milandr> milandr;
};
