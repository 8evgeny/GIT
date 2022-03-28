#include "main.h"
#include "SSM2166_MicPreamp.h"

SSM2166_MicPreamp::SSM2166_MicPreamp(std::shared_ptr<Milandr> mil)
{
    gainAdjusteUP(mil);
    compressionRatioUP(mil);
}

SSM2166_MicPreamp::~SSM2166_MicPreamp()
{
}

void SSM2166_MicPreamp::gainAdjusteUP(std::shared_ptr<Milandr> m)
{
    m->setGAIN_UPR(true);
}

void SSM2166_MicPreamp::gainAdjusteDOWN(std::shared_ptr<Milandr> m)
{
    m->setGAIN_UPR(false);
}


void SSM2166_MicPreamp::compressionRatioUP(std::shared_ptr<Milandr> m)
{
    m->setCOMP_UPR(true);
}

void SSM2166_MicPreamp::compressionRatioDOWN(std::shared_ptr<Milandr> m)
{
    m->setCOMP_UPR(false);
}
