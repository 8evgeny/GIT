#include "yellmsg.hpp"
#include <QRandomGenerator>
#include <cstring>

#include "details.hpp"
#include "ips_helpers.hpp"
#include "nswfl_crc32.hpp"

#include <debug_macro.h>

namespace ISLP {

#define ISLP_YELL_PAYLOADSTART (ISLP_YELL_HEADERSIZE-4)
#define ISLP_YELL_PAYLOADEND   (message.size()-4)
#define ISLP_YELL_CRC           ISLP_YELL_PAYLOADEND
#define ISLP_YELL_MAGIC         0
#define ISLP_YELL_FUNCPLACE     4
#define ISLP_YELL_KEYOS         8
#define ISLP_YELL_CHECKOS       12

#ifdef IPS_TESTING
#define ISLP_KEY_(x)      ((uint8_t*)data)[((uint8_t*)data)[ISLP_YELL_KEYOS+x]]
#define ISLP_CHECK_       (*((uint32_t*)(((uint8_t*)data)+((uint8_t*)data)[ISLP_YELL_CHECKOS])))
#define ISLP_CRC32_       (*(uint32_t*)((uint8_t*)data+(size-sizeof(uint32_t))))
#endif

yellMsg::yellMsg(arcCrypt *crypt):
    basicMessage(QRandomGenerator::QRandomGenerator::securelySeeded().bounded(ISLP_YELL_MINSIZE,ISLP_YELL_MAXSIZE))
{
    keyring=crypt;
    IPS_FUNCTION_DBG
    generatePayload();
    Helpers::setU32to(message.data(),ISLP_YELL_MAGIC,magicnumber_locator);
    Helpers::setU32to(message.data(),ISLP_YELL_FUNCPLACE,ISLP::Functions::Yell);
    setCryptKey();
}


uint8_t *yellMsg::operator()()
{
    IPS_FUNCTION_DBG
    return message.data();
}

void yellMsg::setCryptKey()
{
    IPS_FUNCTION_DBG
    NSWFL::Hashing::CRC32 worker;
    worker.Initialize();
    uint32_t crc32=worker.FullCRC(message.data(),message.size());
    if(keyring!=nullptr){
        keyring->setKey(&crc32,sizeof(uint32_t));
    }else{
        keyring=new arcCrypt(&crc32,sizeof(crc32));
    }

    Helpers::appendToByteVectorByVal<uint32_t>(&message,keyring->encrypt(magicnumber_locator));
}

void yellMsg::generatePayload()
{
    IPS_FUNCTION_DBG
    QRandomGenerator rng;
    rng.securelySeeded();
    for(uint32_t i=ISLP_YELL_PAYLOADSTART;i<ISLP_YELL_PAYLOADEND;i++){
        message[i]=static_cast<uint8_t>(rng.bounded(0xFF));
    }
}

/*
 * =============================================================================================================
 *  Message validation function for tests
 * =============================================================================================================
 */

#ifdef IPS_TESTING
bool yellMsg::validateYell(void *data, size_t size)
{
    if(((int*)data)[0]!=magicnumber_locator){
        IPS_FUNCTION_DBG
        IPS_DEBUG_OUTPUT_FCN<<"Magic number incorrect M="<<((int*)data)[0]<<"!="<<magicnumber_locator<<"\n";
        return false;
    }
    if(((int*)data)[1]!=ISLP::Functions::Yell){
        IPS_FUNCTION_DBG
        IPS_DEBUG_OUTPUT_FCN<<"Function incorrect f="<<((int*)data)[1]<<"!="<<ISLP::Functions::Yell<<"\n";
        return false;
    }
    NSWFL::Hashing::CRC32 worker;
    worker.Initialize();
    uint32_t key=worker.FullCRC((uint8_t*)data,size-sizeof(uint32_t));
    arcCrypt krypto(&key,ISLP_YELL_KEYSIZE);
    if(krypto.decrypt(*(uint32_t*)((char*)data+(size-sizeof(int))))!=magicnumber_locator)
    {
        IPS_FUNCTION_DBG
        IPS_DEBUG_OUTPUT_FCN<<"Key check failed\n";
        IPS_DEBUG_OUTPUT_FCN<<"key="<<key<<"\n";
        return false;
    }
    return true;

}

bool yellMsg::validateYell(basicMessage *msg)
{
    return validateYell(((yellMsg*)msg)->message.data(),msg->size());
}
#endif

}
