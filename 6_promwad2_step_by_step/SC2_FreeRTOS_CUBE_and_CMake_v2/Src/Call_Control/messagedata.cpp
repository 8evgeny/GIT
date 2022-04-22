#include "messagedata.h"
#include "../UDP_JSON/udp_multicast.h"

void MessageData::parse(JsonDocument &doc)
{
    field.ownId = doc["Own_Id"];
    field.linkData = doc["Link_Data"];
    field.priority = doc["Priority"];
//    field.distPriority = doc["Dist_Priority"];
    field.linkMode = doc["Direct_Link_Mode"];
    field.callType = doc["Call_Type"];

    distIdArrSize = doc["Dist_Id"].size();
    if (distIdArrSize > 1) {
        for (uint32_t i = 0; i < distIdArrSize; ++i) {
            if (Json::getInstance()->thisStation.id == doc["Dist_Id"][i]) {
                field.distId = doc["Dist_Id"][i];
                break;
            } else field.distId = 0;
        }
    } else {
        field.distId = doc["Dist_Id"][0];
    }
}

void MessageDataBuff::clear()
{
    if (recvMessageBuff != nullptr) {
        delete [] recvMessageBuff;
        recvMessageBuff = nullptr;
    }
}

MessageDataBuff:: ~MessageDataBuff()
{
    clear();
}
