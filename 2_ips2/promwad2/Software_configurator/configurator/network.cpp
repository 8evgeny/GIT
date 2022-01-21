#include "appcore.h"
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/uuid/uuid_io.hpp>

#include "Lib/qjson/include/QJson/Parser"
#include "Lib/qjson/include/QJson/Serializer"
#include "Lib/qjson/src/cmdlineparser.h"
#include "Lib/qjson/src/qjson_debug.h"
#include <QNetworkInterface>

#include <QFile>
#include <QHash>

#include <string>
#include <functional>
#include <QHostAddress>
#include <QNetworkDatagram>
#include <QUdpSocket>
#include <QThread>

static bool searchSubstring(const QString &substring)
{
    int j = 0;

    while ((j = substring.indexOf("loolback", j)) != -1) {
        qDebug() << "Found loolback at index position" << j;
        ++j;
        return true;
    }

    j = 0;
    while ((j = substring.indexOf("lo", j)) != -1) {
        qDebug() << "Found lo at index position" << j;
        ++j;
        return true;
    }

    return false;
}

void AppCore::getNetworkList()
{
    QList<QNetworkInterface> list = QNetworkInterface::allInterfaces();
    for (const auto &item : list) {
        qCritical() << item.name();

        //Returns the index of the interface whose name is name or 0 if there is no interface with that name.
        //This function should produce the same result as the following code, but will probably execute faster.
        qCritical() << QNetworkInterface::interfaceFromName(item.name()).index();

        if(searchSubstring(item.name())) {
            continue;
        }

        if (item.name() == networkCard) {
            emit addNetworkListGroupsOn(item.name());
        } else {
            emit addNetworkListGroups(item.name());
        }
    }
}

bool AppCore::sendDataByUdp(const QByteArray &data, const QString &Ip)
{
    QUdpSocket udpSocket;
    QNetworkInterface netint;

#ifdef __linux__
    udpSocket.bind(QHostAddress(QHostAddress::AnyIPv4), PORT_JSON, QUdpSocket::ShareAddress);
#elif _WIN32
    udpSocket.bind(PORT_JSON, QUdpSocket::ShareAddress);
#else

#endif

    QNetworkDatagram netDatagram(data, QHostAddress(Ip), PORT_JSON);
    uint index = static_cast<uint>(QNetworkInterface::interfaceFromName(networkCard).index());
    if (index) {
        netDatagram.setInterfaceIndex(index);
        udpSocket.writeDatagram(netDatagram);
        return true;
    }
    emit needSelectNetworkCard();
    return false;
}

bool AppCore::sendDataByUdpMulticast(const QByteArray &data, const QString &Ip)
{
    QUdpSocket udpSocket;
    QNetworkInterface netint;
    QHostAddress groupAddress(Ip);

#ifdef __linux__
    udpSocket.bind(QHostAddress(QHostAddress::AnyIPv4), PORT_JSON, QUdpSocket::ShareAddress);
#elif _WIN32
    udpSocket.bind(PORT_JSON, QUdpSocket::ShareAddress);
#else

#endif

    udpSocket.joinMulticastGroup(groupAddress);

    QNetworkDatagram netDatagram(data, groupAddress, PORT_JSON);

    uint index = static_cast<uint>(QNetworkInterface::interfaceFromName(networkCard).index());
    if (index) {
        netDatagram.setInterfaceIndex(index);
        udpSocket.writeDatagram(netDatagram);
        return true;
    }
    emit needSelectNetworkCard();
    return false;
}

void AppCore::saveCurrentNetworkCard(const QString &newNetworkCard)
{
    networkCard = newNetworkCard;
}

//send the full configuration to the board
void AppCore::sendConfigJsonToStationByEthernet(const QString &stationID, const QString &toId)
{
    QVariantMap all;

    QVariantList stations;

    QRegExp rx("[, ]"); // match a comma or a space
    QStringList list =  stationID.split(rx, QString::SkipEmptyParts);

    if (!(list.size() >= 2)) {
        qCritical() << "Something went wrong, own ID fail";
        return;
    }

    all.insert("ownId", list.at(1).toInt());

    //go over the list of stations
    for (const auto &listOfStation : listOfStations) {

        QVariantMap station;
        QVariantMap name;

        QRegExp rx("[, ]"); // match a comma or a space
        QStringList list =  listOfStation.nameOfID.split(rx, QString::SkipEmptyParts);

        if (!(list.size() >= 2)) {
            qCritical() << "Something went wrong, a name ID for stations";
            return;
        }

        //const QString& type = list.at(0);
        const QString &idStr = list.at(1);

        qint32 id = idStr.toInt();
        name.insert("id", id);

        name.insert("ip", listOfStation.ip);
        name.insert("mask", listOfStation.maskNetwork);
        name.insert("gateway", listOfStation.gateway);

        QVariantList keys;
        //go over the list of keys for the list of the station
        for (const auto &listOfKey : listOfStation.listOfKeys) {
            QVariantMap key;

            qint32 numberKey = listOfKey.numberOfKey.toInt();
            key.insert("key", numberKey);

            auto compareString = [](const QString & x) {
                if (x == QString("1"))
                    return directConnectionDuplex;

                if (x == QString("2"))
                    return groupCommunication;

                if (x == QString("3"))
                    return circularBond;

                if (x == QString("4"))
                    return conferenceCall;

                if (x == QString("5"))
                    return telephoneCommunications;

                if (x == QString("6"))
                    return directConnectionSimplex;

                if (x == QString("7"))
                    return hangUp;

                return noneConnection;
            };

            qint32 typeFunction = compareString(listOfKey.functionOfKey);
            if (typeFunction == 0) {
                qCritical() << "Something went wrong, a function for keys";
                return;
            }

            if (typeFunction == 1) {
                //duplex
                key.insert("link", 2);
                key.insert("function", 1);
            } else if (typeFunction == 6) {
                //simplex
                key.insert("link", 1);
                key.insert("function", 1);
            } else {
                key.insert("link", 0);
                key.insert("function", typeFunction);
            }

            QRegExp rx("[, ]"); // match a comma or a space
            QStringList list = listOfKey.assignedKey.split(rx, QString::SkipEmptyParts);

            if ((!(list.size() >= 2)) && ((typeFunction != 3) && (typeFunction != 4) && (typeFunction != 5) && (typeFunction != 7))) {
                qCritical() << "Something went wrong, CID ID for stations";
                return;
            } else {
                if ((typeFunction != 3) && (typeFunction != 4) && (typeFunction != 5) && (typeFunction != 7)) {
                    const QString &type = list.at(0);
                    const QString &idStr = list.at(1);

                    qint32 typeForAssign = -1;

                    //digital stations
                    if (type == QString("CID"))
                        typeForAssign = 0;

                    //groups
                    if (type == QString("GR"))
                        typeForAssign = 1;
                    //3, 4, 5, 7
                    if ((typeForAssign == -1) && (typeFunction != 7) && (typeFunction != 5) && (typeFunction != 4) && (typeFunction != 3)) {
                        qCritical() << "Something went wrong, type for assign";
                        return;
                    }
                    key.insert("type", typeForAssign);
                    //id of groups or stations
                    qint32 id = idStr.toInt();
                    key.insert("assign", id);
                } else {
                    key.insert("type", 0);
                    key.insert("assign", 0);
                }
            }

            qint32 modeKey = listOfKey.modeKey.toInt();
            key.insert("mode", modeKey);

            qint32 prioriryKey = listOfKey.prioriryKey.toInt();
            key.insert("prioriry", prioriryKey);

            keys << key;
        }

        station.insert("name", name);
        station.insert("keys", keys);

        stations << station;
    }

    QVariantList groups;
    //go over the list of groups
    for (const auto &listOfGroup : listOfGroups) {

        QVariantMap group;
        QVariantMap name;

        QRegExp rx("[, ]"); // match a comma or a space
        QStringList list =  listOfGroup.nameOfID.split(rx, QString::SkipEmptyParts);

        if (!(list.size() >= 2)) {
            qCritical() << "Something went wrong, a name ID for groups";
            return;
        }

        name.insert("number", list.at(1).toInt());

        QVariantList stations;
        //go over the list of keys for the list of the station
        for (const auto &j : listOfGroup.stations) {
            QVariantMap station;

            QRegExp rx("[, ]"); // match a comma or a space
            QStringList list =  j.nameOfID.split(rx, QString::SkipEmptyParts);

            if (!(list.size() >= 2)) {
                qCritical() << "Something went wrong, a name ID for stations in groups";
                return;
            }

            station.insert("id", list.at(1).toInt());
            stations << station;
        }

        group.insert("name", name);
        group.insert("stations", stations);

        groups << group;
    }

    all.insert("stationsAll", stations);
    all.insert("groupsAll", groups);

    //sending data to uart
    QJson::Serializer serializer;
    bool ok;
    QByteArray json = serializer.serialize(all, &ok);

    if (ok) {
        qDebug() << json;

        //max size of step for transferring
        constexpr qint32 SIZE_DEF = 256;
        constexpr qint32 TIME_OUT = 500;

        //write data
        qint32 sizeBuf = json.size();

        qint32 allPackegs = static_cast<int>(ceil(static_cast<double>(sizeBuf) / static_cast<double>(SIZE_DEF))) - 1;

        qint32 counterPack = 0;
        for (qint32 i = 0; i < sizeBuf; i += SIZE_DEF) {
            QByteArray tmp;

            for (qint32 j = i; j < (i < sizeBuf - SIZE_DEF ? i + SIZE_DEF : sizeBuf); j++) {
                tmp.append(json.at(j));
            }

            QVariantMap cmd;
            QRegExp rx("[, ]"); // match a comma or a space
            QStringList list =  toId.split(rx, QString::SkipEmptyParts);

            const QString &idStr = list.at(1);
            qint32 id = idStr.toInt();
            cmd.insert("writeConfigId", id);
            cmd.insert("number", counterPack);
            cmd.insert("all", allPackegs);
            cmd.insert("size", tmp.size());
            cmd.insert("config", tmp);

            counterPack++;
            QJson::Serializer serializerSend;
            bool okSend;
            QByteArray jsonSend = serializerSend.serialize(cmd, &okSend);

            if (okSend) {
                sendDataByUdpMulticast(jsonSend, groupAddress);
                QThread::msleep(TIME_OUT);
                sendDataByUdpMulticast(jsonSend, groupAddress);
                QThread::msleep(TIME_OUT);
            }

            //clear the temp buffer
            tmp.clear();
        }

    } else {
        qCritical() << "Something went wrong:" << serializer.errorMessage();
    }
}

void AppCore::startLoadFirmware(const QString &stationID, bool state)
{

    constexpr int SIZE_FIRMWARE = 512;
    if (firmwareForDownload.size()) {
        qint32 allPackegs = static_cast<int>(ceil(static_cast<double>(firmwareForDownload.size()) / static_cast<double>(SIZE_FIRMWARE))) - 1;
        qint32 currentState = 0;
        for (qint32 i = 0; i < firmwareForDownload.size(); i += SIZE_FIRMWARE) {

            qint32 sizeCopy = 0;
            qint32 sizeCurrent = abs(firmwareForDownload.size() - i);
            if (sizeCurrent > SIZE_FIRMWARE) {
                sizeCopy = SIZE_FIRMWARE;
            } else {
                sizeCopy = abs(firmwareForDownload.size() - i);
            }

            QVariantMap firmware;
            QByteArray tmp;

            for (qint32 j = i; j < i + sizeCopy; j++) {
                tmp.append(firmwareForDownload.at(j));
            }
            std::string strByte = hexStr(tmp);
            QString dataStr = QString::fromLocal8Bit(strByte.c_str());

            firmware.insert("cmd", "update");
            firmware.insert("ver", versionFirmware.toInt());
            firmware.insert("sub", subVersionFirmware.toInt());
            firmware.insert("size", dataStr.count());
            firmware.insert("current", currentState);
            firmware.insert("all",  allPackegs);
            firmware.insert("data", dataStr);

            QJson::Serializer serializer;

            bool ok;
            QByteArray json = serializer.serialize(firmware, &ok);

            if (ok) {
                qDebug() << json;
                if (state) {
                    sendDataByUdpMulticast(json, groupAddress);
                    QThread::msleep(100);
                    if (currentState == 0) QThread::msleep(5000);
                    sendDataByUdpMulticast(json, groupAddress);
                    QThread::msleep(100);
                    sendDataByUdpMulticast(json, groupAddress);
                    QThread::msleep(100);
                    sendDataByUdpMulticast(json, groupAddress);
                    QThread::msleep(100);
                    sendDataByUdpMulticast(json, groupAddress);
                    QThread::msleep(100);
                } else {
                    //for saving an index
                    int indexOfStation = 0;
                    bool isStation = false;

                    //look for an index of the station
                    for (int i = 0; i < listOfStations.size(); i++) {
                        if (listOfStations.at(i).nameOfID == stationID) {
                            indexOfStation = i;
                            isStation = true;
                            break;
                        }
                    }

                    if (isStation) {
                        sendDataByUdp(json, listOfStations.at(indexOfStation).ip);
                        QThread::msleep(100);
                        if (currentState == 0) QThread::msleep(5000);
                        sendDataByUdp(json, listOfStations.at(indexOfStation).ip);
                        QThread::msleep(100);
                        sendDataByUdp(json, listOfStations.at(indexOfStation).ip);
                        QThread::msleep(100);
                        sendDataByUdp(json, listOfStations.at(indexOfStation).ip);
                        QThread::msleep(100);
                        sendDataByUdp(json, listOfStations.at(indexOfStation).ip);
                        QThread::msleep(100);
                        sendDataByUdp(json, listOfStations.at(indexOfStation).ip);
                    }
                }
            } else {
                qCritical() << "Something went wrong:" << serializer.errorMessage();
            }
            currentState++;
            tmp.clear();
            firmware.clear();
        }
        versionFirmware = "";
        subVersionFirmware = "";
        firmwareForDownload.clear();
    }
}

void AppCore::rebootMcuByJson(const QString &nameId)
{
    QVariantMap cmd;
    QRegExp rx("[, ]"); // match a comma or a space
    QStringList list =  nameId.split(rx, QString::SkipEmptyParts);

    const QString &idStr = list.at(1);
    qint32 id = idStr.toInt();

    cmd.insert("rebootId", id);

    QJson::Serializer serializer;

    bool ok;
    QByteArray json = serializer.serialize(cmd, &ok);
    if (ok) {
        sendDataByUdpMulticast(json, groupAddress);
        QThread::msleep(100);
    }
}

void AppCore::writeConfigMcuByJson(const QString &nameId)
{
//    QVariantMap cmd;
//    QRegExp rx("[, ]"); // match a comma or a space
//    QStringList list =  nameId.split(rx, QString::SkipEmptyParts);

//    const QString &idStr = list.at(1);
//    qint32 id = idStr.toInt();

    sendConfigJsonToStationByEthernet(nameId, nameId);

}

void AppCore::deleteConfigMcuByJson(const QString &nameId)
{
    QVariantMap cmd;
    QRegExp rx("[, ]"); // match a comma or a space
    QStringList list =  nameId.split(rx, QString::SkipEmptyParts);

    const QString &idStr = list.at(1);
    qint32 id = idStr.toInt();

    cmd.insert("deleteConfigId", id);

    QJson::Serializer serializer;

    bool ok;
    QByteArray json = serializer.serialize(cmd, &ok);
    if (ok) {
        sendDataByUdpMulticast(json, groupAddress);
        QThread::msleep(100);
    }
}

