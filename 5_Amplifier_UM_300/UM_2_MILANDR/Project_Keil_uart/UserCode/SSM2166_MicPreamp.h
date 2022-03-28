#pragma once
#include "main.h"
#include "milandr.h"
#include <memory>

class SSM2166_MicPreamp
{
public:
    SSM2166_MicPreamp(std::shared_ptr<Milandr>);
    ~SSM2166_MicPreamp();
    void gainAdjusteUP(std::shared_ptr<Milandr>);
    void gainAdjusteDOWN(std::shared_ptr<Milandr>);
    void compressionRatioUP(std::shared_ptr<Milandr>);
    void compressionRatioDOWN(std::shared_ptr<Milandr>);

private:

};
