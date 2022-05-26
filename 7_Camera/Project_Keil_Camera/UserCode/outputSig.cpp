#include "main.h"
#include "outputSig.h"

OutputSig::OutputSig(std::shared_ptr<Milandr> mil)
{
    milandr = mil;
    _moveOn = false;
}

OutputSig::~OutputSig()
{
}

bool OutputSig::getMoveOn() const
{
    return _moveOn;
}

void OutputSig::setMoveOn(bool newMoveOn)
{
//    _moveOn = newMoveOn;
//    if(newMoveOn) PORT_SetBits(MDR_PORTB, PORT_Pin_10);
//    if(!newMoveOn) PORT_ResetBits(MDR_PORTB, PORT_Pin_10);
}






