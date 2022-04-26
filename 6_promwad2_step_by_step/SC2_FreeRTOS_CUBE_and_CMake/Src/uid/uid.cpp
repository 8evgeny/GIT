/*!
 \file uid.cpp
 \authors Shulenkov R.A.

 \code

    uint32_t firstPart = UID::getInstance().getFirstPartOfUid();
    uint32_t secondPart = UID::getInstance().getSecondPartOfUid();
    uint32_t thirdPart = UID::getInstance().getThirdPartOfUid();

    RS232::getInstance().term << "First part " << firstPart << "\n";
    RS232::getInstance().term << "Second part " << secondPart << "\n";
    RS232::getInstance().term << "Third part " << thirdPart << "\n";

 \endcode
*/

#include "uid.h"
#include "rs232.h"

UID *UID::pInstance = nullptr;
UIDDestroyer UID::destroyer;

UIDDestroyer::~UIDDestroyer()
{
    delete pInstance;
}

void UIDDestroyer::initialize(UID *p)
{
    pInstance = p;
}

UID &UID::getInstance()
{
    if (!pInstance)     {
        pInstance = new UID();
        destroyer.initialize(pInstance);
    }
    return *pInstance;
}

void UID::test()
{
    uint32_t firstPart = UID::getInstance().getFirstPartOfUid();
    uint32_t secondPart = UID::getInstance().getSecondPartOfUid();
    uint32_t thirdPart = UID::getInstance().getThirdPartOfUid();

    RS232::getInstance().term << "First part " << firstPart << "\n";
    RS232::getInstance().term << "Second part " << secondPart << "\n";
    RS232::getInstance().term << "Third part " << thirdPart << "\n";
}
