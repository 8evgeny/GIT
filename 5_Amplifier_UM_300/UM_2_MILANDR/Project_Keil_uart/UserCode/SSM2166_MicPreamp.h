#pragma once
#include "main.h"
#include "milandr.h"
#include <memory>

class SSM2166_MicPreamp
{
public:
    SSM2166_MicPreamp(std::shared_ptr<Milandr>);
    ~SSM2166_MicPreamp();

    void gainAdjusteUP();
    void gainAdjusteDOWN();
    void compressionRatioUP();
    void compressionRatioDOWN();

    void micPreampON();
    void micPreampOFF();
    void micPreampCompressionON();
    void micPreampCompressionOFF();

private:
    std::shared_ptr<Milandr> milandr;

};
