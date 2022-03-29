#include "main.h"
#include "SSM2166_MicPreamp.h"

SSM2166_MicPreamp::SSM2166_MicPreamp(std::shared_ptr<Milandr> mil)
{
    milandr = mil;
}

SSM2166_MicPreamp::~SSM2166_MicPreamp()
{
}

void SSM2166_MicPreamp::gainAdjusteUP()
{
    milandr->setGAIN_UPR(false); //активный - ноль
}

void SSM2166_MicPreamp::gainAdjusteDOWN()
{
    milandr->setGAIN_UPR(true);
}


void SSM2166_MicPreamp::compressionRatioUP()
{
    milandr->setCOMP_UPR(false);//активный - ноль
}

void SSM2166_MicPreamp::compressionRatioDOWN()
{
    milandr->setCOMP_UPR(true);
}

void SSM2166_MicPreamp::micPreampON()
{
    gainAdjusteUP();
}

void SSM2166_MicPreamp::micPreampOFF()
{
    gainAdjusteDOWN();
}

void SSM2166_MicPreamp::micPreampCompressionON()
{
    compressionRatioUP();
}

void SSM2166_MicPreamp::micPreampCompressionOFF()
{
    compressionRatioDOWN();
}
