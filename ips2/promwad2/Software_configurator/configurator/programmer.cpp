#include "appcore.h"

#include "Lib/qjson/include/QJson/Parser"
#include "Lib/qjson/include/QJson/Serializer"
#include "Lib/qjson/src/cmdlineparser.h"
#include "Lib/qjson/src/qjson_debug.h"

void AppCore::getInformationForProgrammerWindow()
{
    for (const auto &listOfStation : listOfStations) {
        emit fillInfoForProgrammerWindow(listOfStation.nameOfID, listOfStation.nameOfStation, listOfStation.ip);
    }
}

void AppCore::requestSendJson()
{
    emit sendJsonUseSelectedBox();
}

bool AppCore::receiveDataByUdpMulticast(const QByteArray &data, const QString &Ip)
{
    return false;
}

void AppCore::readStatusFromStations(const QString &nameID)
{

    QVariantMap firmware;
    firmware.insert("who", "");

    QJson::Serializer serializer;

    listOfStatus.clear();

    //append stations to list of status
    for (const auto &iter : listOfStations) {
        ListOfStationsStatus tmp;
        tmp.colorStation = "grey";
        tmp.stations.append(iter);
        listOfStatus.append(tmp);
    }

    bool ok;
    QByteArray json = serializer.serialize(firmware, &ok);
    if (ok) {
        qDebug() << json;
        sendDataByUdpMulticast(json, groupAddress);
    }
}

void AppCore::statusChangedJson(const QString &idJson, const QString &versionJson, const QString &macJson, const QString &ipJson)
{
    //clear list
    emit clearListOfStationsStatus();

    //append stations to list of status
    for (auto &iter : listOfStatus) {
        if(iter.stations.at(0).nameOfID == QString("CID ") + idJson){
            if(iter.stations.at(0).ip == ipJson){
                iter.colorStation = "green";
            }
            else{
                iter.colorStation = "red";
            }
            iter.version =  versionJson;
            iter.mac = macJson;
        }
        emit fillInfoForProgrammerWindowByJson(iter.stations.at(0).nameOfID, iter.stations.at(0).nameOfStation, iter.stations.at(0).ip, iter.colorStation, iter.mac, iter.version);
    }

    //update list
}
