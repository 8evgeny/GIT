#include "main.h"
#include "SSM2166_MicPreamp.h"

SSM2166_MicPreamp::SSM2166_MicPreamp(std::shared_ptr<Milandr> mil)
{
    milandr = mil;
    gainAdjusteUP();
    compressionRatioUP();
}

SSM2166_MicPreamp::~SSM2166_MicPreamp()
{
}

void SSM2166_MicPreamp::gainAdjusteUP()
{
    milandr->setGAIN_UPR(true);
}

void SSM2166_MicPreamp::gainAdjusteDOWN()
{
    milandr->setGAIN_UPR(false);
}


void SSM2166_MicPreamp::compressionRatioUP()
{
    milandr->setCOMP_UPR(true);
}

void SSM2166_MicPreamp::compressionRatioDOWN()
{
    milandr->setCOMP_UPR(false);
}
