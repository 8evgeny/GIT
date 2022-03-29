#include "main.h"
#include "TPA3255_Amplifier.h"

TPA3255_Amplifier::TPA3255_Amplifier(std::shared_ptr<Milandr> mil)
{
    milandr = mil;
    reset();
}

TPA3255_Amplifier::~TPA3255_Amplifier()
{
}

void TPA3255_Amplifier::reset()
{

    milandr->setRESET_UPR(false);
    delay(0xFF);
    milandr->setRESET_UPR(true);

}

bool TPA3255_Amplifier::isOverHeart()
{
    return !milandr->getCLIP_OTW(); //Инверсный
}
bool TPA3255_Amplifier::isFault()
{
    return !milandr->getFAULT(); //Инверсный
}
