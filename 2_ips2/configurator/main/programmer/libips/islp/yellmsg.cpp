#include "yellmsg.hpp"
#include <QRandomGenerator>
#include <cstring>

#include "details.hpp"
#include "../../libips/ips_helpers.hpp"
#include "../../libips/debug_macro.h"
#include "../../libips/tools/nswfl_crc32.hpp"
#include "../../libips/islp/setstationmessage.hpp"
#include "../../libips/tools/intercomstation.hpp"

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
    basicMessage(QRandomGenerator::QRandomGenerator::securelySeeded().bounded(static_cast<long unsigned int>(ISLP_YELL_MINSIZE), static_cast<unsigned int>(ISLP_YELL_MAXSIZE)))
{
    keyring=crypt;    
    generatePayload();
    Helpers::setU32to(message.data(),ISLP_YELL_MAGIC,magicnumber_locator);
    Helpers::setU32to(message.data(),ISLP_YELL_FUNCPLACE,ISLP::Functions::Yell);
    setCryptKey();
}

arcCrypt *yellMsg::Key()
{
  return keyring;
}

uint8_t *yellMsg::operator()()
{    
    return message.data();
}

void yellMsg::setCryptKey()
{    
    NSWFL::Hashing::CRC32 worker;
    worker.Initialize();
    uint32_t crc32=worker.FullCRC(message.data(),message.size());
    if(keyring!=nullptr){
        keyring->setKey(&crc32,sizeof(uint32_t));
    }else{       
        keyring=new arcCrypt(&crc32,sizeof(crc32));
    }

    Helpers::appendToByteVectorByVal<uint32_t>(&message, keyring->encrypt(magicnumber_locator));
}

void yellMsg::generatePayload()
{    
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

bool yellMsg::validateYell(void *data, size_t size)
{    
    if((reinterpret_cast<int*>(data))[0]!= static_cast<int>(magicnumber_locator)){
        return false;
    }    
    if((reinterpret_cast<int*>(data))[1]!=ISLP::Functions::Yell){        
        return false;
    }
    NSWFL::Hashing::CRC32 worker;
    worker.Initialize();
    std::uint32_t key=worker.FullCRC(reinterpret_cast<std::uint8_t*>(data),size-sizeof(std::uint32_t));
    arcCrypt krypto(&key,ISLP_YELL_KEYSIZE);   
    if(krypto.decrypt(*reinterpret_cast<std::uint32_t*>((reinterpret_cast<char*>(data)+(size-sizeof(int)))))!=magicnumber_locator)
    {        
        return false;
    }
    return true;

}

bool yellMsg::validateYell(basicMessage *msg)
{
    return validateYell((reinterpret_cast<yellMsg*>(msg))->message.data(),msg->size());
}
}
