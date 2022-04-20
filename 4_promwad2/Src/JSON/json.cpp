
#include "json.h"


Json::Json()
{
    thisStation.keysBuffer.reserve(32);
    thisStation.groupsBuffer.reserve(32);
}

Json *Json::getInstance()
{
    if(!pInstance) pInstance = new Json();
    return pInstance;
}

Json *Json::pInstance = nullptr;

bool Json::deserialize(JsonDocument &doc, void *jsonBuff, size_t size)
{
    auto error = deserializeJson(doc, reinterpret_cast<const char *>(jsonBuff), size);
    if (error) {
        deserializeJsonFlag = JsonFlags::ERR;
        Debug::getInstance().dbg << "deserialize UDP_Json() failed with code ";
        Debug::getInstance().dbg << error.c_str() << "\n";
        return false;
    } else {
        deserializeJsonFlag = JsonFlags::OK;
        return true;
    }
}

inline void Json::read()
{
    /*boot_config file opening*/
    lfs_file_open(FsForEeprom::getInstance().lfsPtr, FsForEeprom::getInstance().filePtr, "boot_config", LFS_O_RDONLY);
    lfs_file_rewind(FsForEeprom::getInstance().lfsPtr, FsForEeprom::getInstance().filePtr);

    /*size getting of boot_config file*/
    fileSize = lfs_file_size(FsForEeprom::getInstance().lfsPtr, FsForEeprom::getInstance().filePtr);

    if (fileSize) {

        /*memory allocating*/
        tempBuff = new char [fileSize];
        if(tempBuff == nullptr) {
            Debug::getInstance().dbg << "not enough space" << fileSize << "\n";
        }

        /*filling of tempBuff*/
        std::fill(tempBuff, (tempBuff + fileSize), 0);

        /*boot_config file reading*/
        lfs_file_rewind(FsForEeprom::getInstance().lfsPtr, FsForEeprom::getInstance().filePtr);
        lfs_file_read(FsForEeprom::getInstance().lfsPtr, FsForEeprom::getInstance().filePtr, (char *)tempBuff, fileSize);
        SRAM::getInstance()->writeData((uint32_t *)tempBuff, fileSize, (uint32_t *)0x60000000);
    }
}

void Json::configStation()
{

//    ThisStation thisStation;                //thisStation object creating
    thisStation.keysBuffer.clear();     //clearing of keysBuffer

    read();         //boot_config file reading

//    if (fileSize < 8000) modifiedfileSize = fileSize + (8000 - fileSize +512); //The filesize must be more than 1KB
//    else modifiedfileSize = fileSize;

    DynamicJsonDocument doc(15 * 1024);  //memory allocating
//    doc[]

    if (deserialize(doc, reinterpret_cast<char *>(tempBuff), fileSize)) {   //deserialize json



        JsonObject obj = doc.as<JsonObject>();
//        JsonArray objArray = doc.as<JsonArray>();

        uint16_t stationsId;
        thisStation.id = obj["ownId"];  //getting of own ID

        thisStation.groupsNumber = doc["groupsAll"].size();
        Groups groups[thisStation.groupsNumber];

        for (uint16_t j = 0; j < thisStation.groupsNumber; ++j) {

            groups[j].name = obj["groupsAll"][j]["name"];


            groups[j].stSize = obj["groupsAll"][j]["stations"].size();

            for (uint16_t n = 0; n < groups[j].stSize; ++n) {
                groups[j].stantions[n] = obj["groupsAll"][j]["stations"][n]["id"];
            }
            thisStation.groupsBuffer.push_back(groups[j]);
        }

        /*Json file prsing*/
        for (uint16_t i = 0; i < obj["stationsAll"].size(); ++i) {

            stationsId = obj["stationsAll"][i]["name"]["id"];

            if(stationsId == thisStation.id) {

                std::fill(thisStation.ip, thisStation.ip + sizeof(thisStation.ip), 0);
                std::fill(thisStation.mask, thisStation.mask + sizeof(thisStation.mask), 0);
                std::fill(thisStation.gateway, thisStation.gateway + sizeof(thisStation.gateway), 0);

                std::strcpy(thisStation.ip, obj["stationsAll"][i]["name"]["ip"]);
                std::strcpy(thisStation.mask, obj["stationsAll"][i]["name"]["mask"]);
                std::strcpy(thisStation.gateway, obj["stationsAll"][i]["name"]["gateway"]);

                thisStation.keysNumber = obj["stationsAll"][i]["keys"].size();
                Keys keys[thisStation.keysNumber];

                for (uint8_t j = 0; j < thisStation.keysNumber; ++j) {

                    keys[j].assign = obj["stationsAll"][i]["keys"][j]["assign"];
                    keys[j].function = obj["stationsAll"][i]["keys"][j]["function"];
                    keys[j].key = obj["stationsAll"][i]["keys"][j]["key"];
                    keys[j].type = obj["stationsAll"][i]["keys"][j]["type"];
                    keys[j].mode = obj["stationsAll"][i]["keys"][j]["mode"];
                    keys[j].priority = obj["stationsAll"][i]["keys"][j]["prioriry"];
                    keys[j].directLinkMode = obj["stationsAll"][i]["keys"][j]["link"];

                    thisStation.keysBuffer.push_back(keys[j]);
                }


            }
        }

        delete[] tempBuff;
    } else delete[] tempBuff;

}

//void Json::sendJson(JsonDocument &doc, const size_t capacity, portType port)
//{
//    osMutexWait(Call_Control::getInstance()->mutex_outputJson_id, osWaitForever);
//    std::fill(outputJson, outputJson + outputJson_size, 0);
//    serializeJson(doc, outputJson, capacity);
//    sendUdpMulticast(outputJson, strlen(outputJson));
//    osMutexRelease(Call_Control::getInstance()->mutex_outputJson_id);
//}
