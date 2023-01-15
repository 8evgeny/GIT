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
extern char uid[25];
extern uint32_t uid1;
extern uint32_t uid2;
extern uint32_t uid3;
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

void UID::getUID()
{
//    char temp[100];
    uid1 = UID::getInstance().getFirstPartOfUid();
    uid2 = UID::getInstance().getSecondPartOfUid();
    uid3 = UID::getInstance().getThirdPartOfUid();
    sprintf(uid,"%08X%08X%08X",uid1, uid2, uid3);
//    sprintf(temp,"UID - %s",uid);
//    term2(temp)
}
